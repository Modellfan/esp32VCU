/*
 * This file is part of the stm32-... project.
 *
 * Copyright (C) 2021 Johannes Huebner <dev@johanneshuebner.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
// #include <libopencm3/stm32/desig.h>
// #include <libopencm3/cm3/scb.h>
#include "cansdo.h"
#include "param_save.h"
#include <ESP32-TWAI-CAN.hpp>
#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
// #include "my_math.h"

extern File jsonParamFile;

// Some functions use the "register" keyword which C++ doesn't like
// We can safely ignore that as we don't even use those functions
//  #pragma GCC diagnostic push
//  #pragma GCC diagnostic ignored "-Wregister"
//  #include <libopencm3/cm3/cortex.h>
//  #pragma GCC diagnostic pop

#define SDO_REQUEST_DOWNLOAD (1 << 5)
#define SDO_REQUEST_UPLOAD (2 << 5)
#define SDO_REQUEST_SEGMENT (3 << 5)
#define SDO_TOGGLE_BIT (1 << 4)
#define SDO_RESPONSE_UPLOAD (2 << 5)
#define SDO_RESPONSE_DOWNLOAD (3 << 5)
#define SDO_EXPEDITED (1 << 1)
#define SDO_SIZE_SPECIFIED (1)
#define SDO_WRITE (SDO_REQUEST_DOWNLOAD | SDO_EXPEDITED | SDO_SIZE_SPECIFIED)
#define SDO_READ SDO_REQUEST_UPLOAD
#define SDO_ABORT 0x80
#define SDO_WRITE_REPLY SDO_RESPONSE_DOWNLOAD
#define SDO_READ_REPLY (SDO_RESPONSE_UPLOAD | SDO_EXPEDITED | SDO_SIZE_SPECIFIED)
#define SDO_ERR_INVIDX 0x06020000
#define SDO_ERR_RANGE 0x06090030
#define SDO_ERR_GENERAL 0x08000000
#define SDO_REQ_ID_BASE 0x600U
#define SDO_REP_ID_BASE 0x580U

#define SDO_INDEX_PARAMS 0x2000
#define SDO_INDEX_PARAM_UID 0x2100
#define SDO_INDEX_MAP_TX 0x3000
#define SDO_INDEX_MAP_RX 0x3001
#define SDO_INDEX_MAP_RD 0x3100
#define SDO_INDEX_SERIAL 0x5000
#define SDO_INDEX_STRINGS 0x5001
#define SDO_INDEX_COMMANDS 0x5002
#define SDO_CMD_SAVE 0
#define SDO_CMD_LOAD 1
#define SDO_CMD_RESET 2
#define SDO_CMD_DEFAULTS 3

#define DESIG_UNIQUE_ID0 ((uint32_t)(ESP.getEfuseMac() & 0xFFFFFFFF))         // Lower 32 bits
#define DESIG_UNIQUE_ID1 ((uint32_t)((ESP.getEfuseMac() >> 32) & 0xFFFFFFFF)) // Upper 32 bits
#define DESIG_UNIQUE_ID2 (0x00000000)                                         // ESP32 does not have a 96-bit unique ID, so this is 0

CanSdo::CanSdo()
    : nodeId(1), remoteNodeId(255), printRequest(-1), saveEnabled(true)
{
   // canHardware->AddCallback(this);
   // HandleClear();
}

/* // Somebody (perhaps us) has cleared all user messages. Register them again
void CanSdo::HandleClear()
{
   canHardware->RegisterUserMessage(SDO_REQ_ID_BASE + nodeId);

   if (remoteNodeId < 64)
      canHardware->RegisterUserMessage(SDO_REP_ID_BASE + remoteNodeId);
} */

void CanSdo::HandleRx(CanFrame *msg)
{
   uint32_t sdoData[2];

   sdoData[0] = (msg->data[0]) |
                (msg->data[1] << 8) |
                (msg->data[2] << 16) |
                (msg->data[3] << 24);

   sdoData[1] = (msg->data[4]) |
                (msg->data[5] << 8) |
                (msg->data[6] << 16) |
                (msg->data[7] << 24);

   if (msg->identifier == (SDO_REQ_ID_BASE + nodeId)) // SDO request
   {
      ProcessSDO(sdoData);
   }
   else if (msg->identifier == (SDO_REP_ID_BASE + remoteNodeId))
   {
      sdoReplyValid = (sdoData[0] & 0xFF) != SDO_ABORT;
      sdoReplyData = sdoData[1];
   }
}

void CanSdo::SDOWrite(uint8_t nodeId, uint16_t index, uint8_t subIndex, uint32_t data)
{
   InitiateSDOTransfer(SDO_WRITE, nodeId, index, subIndex, data);
}

void CanSdo::SDORead(uint8_t nodeId, uint16_t index, uint8_t subIndex)
{
   InitiateSDOTransfer(SDO_READ, nodeId, index, subIndex, 0);
}

bool CanSdo::SDOReadReply(uint32_t &data)
{
   data = sdoReplyData;
   return sdoReplyValid;
}

void CanSdo::SetNodeId(uint8_t id)
{
   nodeId = id;
   // canHardware->ClearUserMessages();
}

void CanSdo::InitiateSDOTransfer(uint8_t req, uint8_t nodeId, uint16_t index, uint8_t subIndex, uint32_t data)
{
   // uint32_t d[2];
   // CAN_SDO *sdo = (CAN_SDO *)d;

   // sdo->cmd = req;
   // sdo->index = index;
   // sdo->subIndex = subIndex;
   // sdo->data = data;

   // if (nodeId != remoteNodeId)
   // {
   //    remoteNodeId = nodeId;
   //    // This registers the reply message
   //    canHardware->ClearUserMessages();
   // }

   // sdoReplyValid = false;
   // canHardware->Send(SDO_REQ_ID_BASE + remoteNodeId, d);
   //==================================================================================
   CanFrame msg = {0};

   // Populate the message
   msg.identifier = SDO_REQ_ID_BASE + remoteNodeId; // 11-bit or 29-bit CAN ID
   msg.data_length_code = 8;                        // SDO requests typically use 8 bytes

   // Prepare the data according to the CAN_SDO structure
   msg.data[0] = req;                 // Command specifier
   msg.data[1] = index & 0xFF;        // Index low byte
   msg.data[2] = (index >> 8) & 0xFF; // Index high byte
   msg.data[3] = subIndex;            // SubIndex
   msg.data[4] = data & 0xFF;         // Data byte 0
   msg.data[5] = (data >> 8) & 0xFF;  // Data byte 1
   msg.data[6] = (data >> 16) & 0xFF; // Data byte 2
   msg.data[7] = (data >> 24) & 0xFF; // Data byte 3

   // Send the message
   ESP32Can.writeFrame(&msg);
}

// http://www.byteme.org.uk/canopenparent/canopen/sdo-service-data-objects-canopen/
void CanSdo::ProcessSDO(uint32_t data[2])
{
   CAN_SDO *sdo = (CAN_SDO *)data;

   // Serial.println("CAN_SDO Contents:");
   // Serial.printf("  Command (cmd): 0x%02X\n", sdo->cmd);
   // Serial.printf("  Index: 0x%04X\n", sdo->index);
   // Serial.printf("  SubIndex: 0x%02X\n", sdo->subIndex);
   // Serial.printf("  Data: 0x%08X\n", sdo->data);

   if ((sdo->cmd & SDO_REQUEST_SEGMENT) == SDO_REQUEST_SEGMENT)
   {
      const int bytesPerMessage = 7;
      uint8_t *bytes = (uint8_t *)data;
      int i = 1; // Start writing data from the second byte

      sdo->cmd = sdo->cmd & SDO_TOGGLE_BIT;

      if (jsonTransferInProgress)
      {
         // Check if the file is valid
         if (!jsonParamFile)
         {
            Serial.println("Parameter file is not loaded.");
            sdo->cmd = SDO_ABORT;
            sdo->data = SDO_ERR_GENERAL;
            return;
         }

         // Seek to the current transfer position
         jsonParamFile.seek(jsonTransferPosition);

         // Read up to 7 bytes from the current position
         int bytesRead = jsonParamFile.read(&bytes[1], bytesPerMessage);
         jsonTransferPosition += bytesRead;

         // Print progress
         Serial.printf("JSON Transfer Progress: %d/%d bytes read\n", jsonTransferPosition, jsonTransferSize);

         // Check if we reached the end of the file
         if (jsonTransferPosition >= jsonTransferSize)
         {
            jsonTransferInProgress = false;
            sdo->cmd |= SDO_SIZE_SPECIFIED;                 // Indicate end of transfer
            sdo->cmd |= (bytesPerMessage - bytesRead) << 1; // Specify how many bytes do NOT contain data
            Serial.println("JSON Transfer Complete.");
         }
      }
      else
      {
         // No JSON transfer in progress; abort the SDO
         sdo->cmd = SDO_ABORT;
         sdo->data = SDO_ERR_GENERAL;
         Serial.println("No JSON transfer in progress. Aborting.");
      }
   }

   else if (sdo->index == SDO_INDEX_PARAMS || (sdo->index & 0xFF00) == SDO_INDEX_PARAM_UID)
   {
      Param::PARAM_NUM paramIdx = (Param::PARAM_NUM)sdo->subIndex;

      // SDO index 0x21xx will look up the parameter by its unique ID
      // using subIndex as low byte and xx as high byte of ID
      if ((sdo->index & 0xFF00) == SDO_INDEX_PARAM_UID)
         paramIdx = Param::NumFromId(sdo->subIndex + ((sdo->index & 0xFF) << 8));

      if (paramIdx < Param::PARAM_LAST)
      {
         if (sdo->cmd == SDO_WRITE)
         {
            if (Param::Set(paramIdx, sdo->data) == 0)
            {
               sdo->cmd = SDO_WRITE_REPLY;
            }
            else
            {
               sdo->cmd = SDO_ABORT;
               sdo->data = SDO_ERR_RANGE;
            }
         }
         else if (sdo->cmd == SDO_READ)
         {
            sdo->data = Param::Get(paramIdx);
            sdo->cmd = SDO_READ_REPLY;
         }
      }
      else
      {
         sdo->cmd = SDO_ABORT;
         sdo->data = SDO_ERR_INVIDX;
      }
   }
   // else if (0 != canMap && sdo->index == SDO_INDEX_MAP_TX)
   // {
   //    AddCanMap(sdo, false);
   // }
   // else if (0 != canMap && sdo->index == SDO_INDEX_MAP_RX)
   // {
   //    AddCanMap(sdo, true);
   // }
   // else if (0 != canMap && (sdo->index & 0xFF00) == SDO_INDEX_MAP_RD)
   // {
   //    ReadOrDeleteCanMap(sdo);
   // }
   else
   {
      ProcessSpecialSDOObjects(sdo);
   }

   CanFrame msg = {0};
   msg.identifier = 0x580 + nodeId;      // 11-bit or 29-bit CAN ID
   msg.data_length_code = 8;             // SDO requests typically use 8 bytes
   msg.data[0] = data[0] & 0xFF;         // Data[0] byte 0
   msg.data[1] = (data[0] >> 8) & 0xFF;  // Data[0] byte 1
   msg.data[2] = (data[0] >> 16) & 0xFF; // Data[0] byte 2
   msg.data[3] = (data[0] >> 24) & 0xFF; // Data[0] byte 3
   msg.data[4] = data[1] & 0xFF;         // Data[1] byte 0
   msg.data[5] = (data[1] >> 8) & 0xFF;  // Data[1] byte 1
   msg.data[6] = (data[1] >> 16) & 0xFF; // Data[1] byte 2
   msg.data[7] = (data[1] >> 24) & 0xFF; // Data[1] byte 3

   // Send the message
   ESP32Can.writeFrame(&msg);
}

void CanSdo::ProcessSpecialSDOObjects(CAN_SDO *sdo)
{
   // Debug variable to indicate if `canMap` is available
   bool canMapAvailable = false; //(canMap != nullptr);

   Serial.printf("Processing SDO: Index=0x%04X, SubIndex=0x%02X, Cmd=0x%02X\n", sdo->index, sdo->subIndex, sdo->cmd);

   // Write hardware serial number to UI
   if (sdo->index == SDO_INDEX_SERIAL && sdo->cmd == SDO_READ)
   {
      sdo->cmd = SDO_READ_REPLY;
      switch (sdo->subIndex)
      {
      case 0:
         sdo->data = DESIG_UNIQUE_ID0;
         break;
      case 1:
         sdo->data = DESIG_UNIQUE_ID1;
         break;
      case 2:
         sdo->data = DESIG_UNIQUE_ID2;
         break;
      case 3:
         sdo->data = Param::GetIdSum();
         break;
      default:
         sdo->cmd = SDO_ABORT;
         sdo->data = SDO_ERR_INVIDX;
      }
   }
   // Write out JSON file with all parameters
   else if (sdo->index == SDO_INDEX_STRINGS && sdo->cmd == SDO_READ)
   {
      // Check if the parameter file is loaded and SPIFFS is initialized
      if (!jsonParamFile)
      {
         Serial.println("Parameter file is not loaded.");
         sdo->cmd = SDO_ABORT;
         sdo->data = SDO_ERR_GENERAL;
         return;
      }

      // Reset transfer variables
      jsonTransferSize = jsonParamFile.size();
      jsonTransferPosition = 0;
      jsonTransferInProgress = true;

      Serial.printf("params.json size: %d bytes\n", jsonTransferSize);

      // Set SDO response
      sdo->data = jsonTransferSize;
      sdo->cmd = SDO_RESPONSE_UPLOAD | SDO_SIZE_SPECIFIED;
   }

     // Handle TX mapping
   else if (sdo->index == SDO_INDEX_MAP_TX && canMapAvailable)
   {
      Serial.println("Processing SDO_INDEX_MAP_TX...");
      // AddCanMap(sdo, false); // Handle TX mapping
   }
   // Handle RX mapping
   else if (sdo->index == SDO_INDEX_MAP_RX && canMapAvailable)
   {
      Serial.println("Processing SDO_INDEX_MAP_RX...");
      // AddCanMap(sdo, true); // Handle RX mapping
   }
   // Handle reading or deleting CAN maps
   else if ((sdo->index & 0xFF00) == SDO_INDEX_MAP_RD && canMapAvailable)
   {
      Serial.println("Processing SDO_INDEX_MAP_RD...");
      // ReadOrDeleteCanMap(sdo); // Read or delete map entry
   }
   // Unknown or unsupported command
   else
   {
      Serial.println("Unknown or unsupported SDO command.");
      sdo->cmd = SDO_ABORT;
      sdo->data = SDO_ERR_INVIDX;
   }
}

/* void CanSdo::ReadOrDeleteCanMap(CAN_SDO *sdo)
{
   bool rx = (sdo->index & 0x80) != 0;
   uint32_t canId;
   uint8_t itemIdx = MAX(0, sdo->subIndex - 1) / 2;
   const CanMap::CANPOS *canPos = canMap->GetMap(rx, sdo->index & 0x3f, itemIdx, canId);

   if (sdo->cmd == SDO_READ)
   {
      if (canPos != 0)
      {
         uint16_t id = Param::GetAttrib((Param::PARAM_NUM)canPos->mapParam)->id;

         if (sdo->subIndex == 0) // 0 contains COB Id
            sdo->data = canId;
         else if (sdo->subIndex & 1) // odd sub indexes have data id, position and length
            sdo->data = id | (canPos->offsetBits << 16) | (canPos->numBits << 24);
         else // even sub indexes except 0 have gain and offset
            sdo->data = (uint32_t)(((int32_t)(canPos->gain * 1000)) & 0xFFFFFF) | (canPos->offset << 24);
         sdo->cmd = SDO_READ_REPLY;
      }
      else
      {
         sdo->cmd = SDO_ABORT;
         sdo->data = SDO_ERR_INVIDX;
      }
   }
   else if (sdo->cmd == SDO_WRITE && canPos != 0 && sdo->data == 0)
   {
      canMap->Remove(rx, sdo->index & 0x3f, itemIdx);
   }
   else
   {
      sdo->cmd = SDO_ABORT;
      sdo->data = SDO_ERR_INVIDX;
   }
} */

/* void CanSdo::AddCanMap(CAN_SDO *sdo, bool rx)
{
   if (sdo->cmd == SDO_WRITE)
   {
      int result = -1;

      if (sdo->subIndex == 0)
      {
         if (sdo->data < 0x20000000 || (sdo->data & ~CAN_FORCE_EXTENDED) < 0x800)
         {
            mapId = sdo->data;
            result = 0;
         }
         else
         {
            mapId = 0xFFFFFFFF;
         }
      }
      else if (mapId != 0xFFFFFFFF && sdo->subIndex == 1)
      {
         // Now we receive UID of value to be mapped along with bit start and length
         mapInfo.mapParam = Param::NumFromId(sdo->data & 0xFFFF);
         mapInfo.offsetBits = (sdo->data >> 16) & 0x3F;
         mapInfo.numBits = ((int32_t)sdo->data >> 24);
         result = mapInfo.mapParam < Param::PARAM_LAST ? 0 : -1;
      }
      else if (mapInfo.numBits != 0 && sdo->subIndex == 2) // This sort of verifies that we received subindex 1
      {
         // Now we receive gain and offset and add the map

         // sign extend the 24-bit integer to a 32-bit integer
         int32_t gainFixedPoint = (sdo->data & 0xFFFFFF) << (32 - 24);
         gainFixedPoint >>= (32 - 24);
         mapInfo.gain = gainFixedPoint / 1000.0f;
         mapInfo.offset = sdo->data >> 24;

         if (rx) // RX map
            result = canMap->AddRecv((Param::PARAM_NUM)mapInfo.mapParam, mapId, mapInfo.offsetBits, mapInfo.numBits, mapInfo.gain, mapInfo.offset);
         else
            result = canMap->AddSend((Param::PARAM_NUM)mapInfo.mapParam, mapId, mapInfo.offsetBits, mapInfo.numBits, mapInfo.gain, mapInfo.offset);

         mapInfo.numBits = 0;
         mapId = 0xFFFFFFFF;
      }

      if (result >= 0)
      {
         sdo->cmd = SDO_WRITE_REPLY;
      }
      else
      {
         sdo->cmd = SDO_ABORT;
         sdo->data = SDO_ERR_INVIDX;
      }
   }
} */