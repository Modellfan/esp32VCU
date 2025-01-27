/*
 * This file is part of the ZombieVerter project.
 *
 * Copyright (C) 2011-2019 Johannes Huebner <dev@johanneshuebner.com>
 * Copyright (C) 2019-2022 Damien Maguire <info@evbmw.com>
 * Changes by Tom de Bree <tom@voltinflux.com> 2024
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
#define VER 2.20.A


/* Entries must be ordered as follows:
   1. Saveable parameters (id != 0)
   2. Temporary parameters (id = 0)
   3. Display values
 */
//Next param id (increase when adding new parameter!): 139
/*              category     name         unit       min     max     default id */
#define PARAM_LIST \
    PARAM_ENTRY(CAT_SETUP,     Inverter,     INVMODES, 0,      8,      0,      5  ) \
    PARAM_ENTRY(CAT_SETUP,     Vehicle,      VEHMODES, 0,      8,      0,      6  ) \
    PARAM_ENTRY(CAT_SETUP,     GearLvr,      SHIFTERS, 0,      4,      0,      108 ) \
    PARAM_ENTRY(CAT_SETUP,     Transmission, TRNMODES, 0,      1,      0,      78 ) \
    PARAM_ENTRY(CAT_PWM,       CP_PWM,   "",        1,      100, 10,   132 ) \
    VALUE_ENTRY(version,       VERSTR,              2000 ) \


//Next value Id: 2104

//Dead params
/*
    PARAM_ENTRY(CAT_THROTTLE,  bmslimhigh,  "%",       0,      100,    50,     17 ) \
    PARAM_ENTRY(CAT_THROTTLE,  bmslimlow,   "%",      -100,    0,     -1,      18 ) \
*/
//////////

#define VERSTR STRINGIFY(4=VER)
#define PINFUNCS     "0=None, 1=ChaDeMoAlw, 2=OBCEnable, 3=HeaterEnable, 4=RunIndication, 5=WarnIndication," \
                     "6=CoolantPump, 7=NegContactor, 8=BrakeLight, 9=ReverseLight, 10=HeatReq, 11=HVRequest," \
                     "12=DCFCRequest, 13=BrakeVacPump, 14=CoolingFan, 15=HvActive, 16=PwmTim3, 17=CpSpoof,"\
                     "18=GS450pump"
#define APINFUNCS    "0=None, 1=ProxPilot, 2=BrakeVacSensor"
#define SHIFTERS     "0=None, 1=BMW_F30, 2=JLR_G1, 3=JLR_G2, 4=BMW_E65"
#define SHNTYPE      "0=None, 1=ISA, 2=SBOX, 3=VAG"
#define DMODES       "0=CLOSED, 1=OPEN, 2=ERROR, 3=INVALID"
#define POTMODES     "0=SingleChannel, 1=DualChannel"
#define BTNSWITCH    "0=Button, 1=Switch, 2=CAN"
#define DIRMODES     "0=Button, 1=Switch, 2=ButtonReversed, 3=SwitchReversed, 4=DefaultForward"
#define INVMODES     "0=None, 1=Leaf_Gen1, 2=GS450H, 3=UserCAN, 4=OpenI, 5=Prius_Gen3, 6=Outlander, 7=GS300H, 8=RearOutlander"
#define PLTMODES     "0=Absent, 1=ACStd, 2=ACchg, 3=Error, 4=CCS_Not_Rdy, 5=CCS_Rdy, 6=Static"
#define VEHMODES     "0=BMW_E46, 1=BMW_E6x+, 2=Classic, 3=None, 5=BMW_E39, 6=VAG, 7=Subaru, 8=BMW_E31"
#define BMSMODES     "0=Off, 1=SimpBMS, 2=TiDaisychainSingle, 3=TiDaisychainDual, 4=LeafBms, 5=RenaultKangoo33"
#define OPMODES      "0=Off, 1=Run, 2=Precharge, 3=PchFail, 4=Charge"
#define DOW          "0=Sun, 1=Mon, 2=Tue, 3=Wed, 4=Thu, 5=Fri, 6=Sat"
#define CHGTYPS      "0=Off, 1=AC, 2=DCFC"
#define DCDCTYPES    "0=NoDCDC, 1=TeslaG2"
#define STATUS_VCU       "0=None, 1=UdcLow, 2=UdcHigh, 4=UdcBelowUdcSw, 8=UdcLim, 16=EmcyStop, 32=MProt, 64=PotPressed, 128=TmpHs, 256=WaitStart"
#define CCS_STATUS   "0=NotRdy, 1=ready, 2=SWoff, 3=interruption, 4=Prech, 5=insulmon, 6=estop, 7=malfunction, 15=invalid"
#define DIRS         "-1=Reverse, 0=Neutral, 1=Drive, 2=Park"
#define ONOFF        "0=Off, 1=On, 2=na"
#define LOWHIGH      "0=LOW, 1=HIGH, 2=AUTO, 3=HIGHFWDLOWREV"
#define OKERR        "0=Error, 1=Ok, 2=na"
#define CANSPEEDS    "0=125k, 1=250k, 2=500k, 3=800k, 4=1M"
#define CANIOS       "1=Cruise, 2=Start, 4=Brake, 8=Fwd, 16=Rev, 32=Bms"
#define CANPERIODS   "0=100ms, 1=10ms"
#define ERRLIGHTS    "0=Off, 4=EPC, 8=engine"
#define CRUISESTATES "0=None, 1=On, 2=Disable, 4=Set, 8=Resume"
#define CDMSTAT      "1=Charging, 2=Malfunction, 4=ConnLock, 8=BatIncomp, 16=SystemMalfunction, 32=Stop"
#define HTTYPE       "0=None, 1=Ampera, 2=VW, 3=OutlanderCan"
#define HTCTRL       "0=Disable, 1=Enable, 2=Timer"
#define CHGMODS      "0=Off, 1=EXT_DIGI, 2=Volt_Ampera, 3=Leaf_PDM, 4=TeslaOI, 5=Out_lander, 6=Elcon"
#define CHGCTRL      "0=Enable, 1=Disable, 2=Timer"
#define CHGINT       "0=Unused, 1=i3LIM, 2=Chademo, 3=CPC, 4=Focci"
#define CAN3SPD      "0=k33.3, 1=k500, 2=k100"
#define TRNMODES     "0=Manual, 1=Auto"
#define CAN_DEV      "0=CAN1, 1=CAN2"
#define CAT_THROTTLE "Throttle"
#define CAT_POWER    "Power Limit"
#define CAT_CONTACT  "Contactor Control"
#define CAT_TEST     "Testing"
#define CAT_COMM     "Communication"
#define CAT_SETUP    "General Setup"
#define CAT_CLOCK    "RTC Module"
#define CAT_HEATER   "Heater Module"
#define CAT_BMS      "Battery Management"
#define CAT_CRUISE   "Cruise Control"
#define CAT_LEXUS    "Gearbox Control"
#define CAT_CHARGER  "Charger Control"
#define CAT_DCDC     "DC-DC Converter"
#define CAT_SHUNT    "ISA Shunt Control"
#define CAT_IOPINS   "General Purpose I/O"
#define CAT_PWM      "PWM Control"
#define MotorsAct    "0=Mg1and2, 1=Mg1, 2=Mg2, 3=BlendingMG2and1"
#define PumpOutType  "0=GS450hOil, 1=TachoOut"
#define LIMITREASON  "0=None, 1=UDClimLow, 2=UDClimHigh, 4=IDClimLow, 8=IDClimHigh, 16=TempLim"

#define CAN_PERIOD_100MS    0
#define CAN_PERIOD_10MS     1

#define FIRST_IO_PARAM Param::Out1Func
#define SEC_IO_PARAM Param::PB1InFunc
#define FIRST_AI_PARAM Param::GPA1Func
enum modes
{
    MOD_OFF = 0,
    MOD_RUN,
    MOD_PRECHARGE,
    MOD_PCHFAIL,
    MOD_CHARGE,
    MOD_LAST
};

enum ctyps
{
    OFF = 0,
    AC,
    DCFC
};

enum _tripmodes
{
    TRIP_ALLOFF = 0,
    TRIP_DCSWON,
    TRIP_PRECHARGEON
};

enum _dirmodes
{
    DIR_BUTTON = 0,
    DIR_SWITCH = 1,
    DIR_REVERSED = 2,
    DIR_DEFAULTFORWARD = 4
};

enum InvModes
{
    NoInv =0,
    Leaf_Gen1 = 1,
    GS450H = 2,
    UserCAN = 3,
    OpenI = 4,
    Prius_Gen3 = 5,
    Outlander = 6,
    GS300H = 7,
    RearOutlander = 8
};

enum ChargeModes
{
    Off = 0,
    EXT_DIGI = 1,
    Volt_Ampera = 2,
    Leaf_PDM = 3,
    TeslaOI = 4,
    Out_lander = 5,
    Elcon = 6
};

enum ChargeInterfaces
{
    Unused = 0,
    i3LIM = 1,
    Chademo = 2,
    CPC = 3,
    Focci = 4
};

enum HeatType
{
    Noheater = 0,
    AmpHeater = 1,
    VW = 2,
        OutlanderHeater = 3
};

enum BMSModes
{
    BMSModeNoBMS = 0,
    BMSModeSimpBMS = 1,
    BMSModeDaisychainSingleBMS = 2,
    BMSModeDaisychainDualBMS = 3,
    BMSModeLeafBMS = 4,
    BMSRenaultKangoo33BMS = 5
};

enum DCDCModes
{
    NoDCDC = 0,
    TeslaG2 = 1

};

enum ShifterModes
{
    NoShifter = 0,
    BMWF30 = 1,
    JLRG1 = 2,
    JLRG2 =3,
    BMWE65 =4

};

enum ChargeControl
{
    Enable = 0,
    Disable = 1,
    Timer = 2
};


enum vehicles
{
    vBMW_E46 = 0,
    vBMW_E65 = 1,
    Classic = 2, //used as a flag
    None = 4,
    vBMW_E39 = 5,
    vVAG = 6,
    vSUBARU = 7,
    vBMW_E31 = 8
};

enum _potmodes
{
    POTMODE_SINGLECHANNEL = 0,
    POTMODE_DUALCHANNEL,
};

enum _canio
{
    CAN_IO_CRUISE = 1,
    CAN_IO_START = 2,
    CAN_IO_BRAKE = 4,
    CAN_IO_FWD = 8,
    CAN_IO_REV = 16,
    CAN_IO_BMS = 32
};

enum status
{
    STAT_NONE = 0,
    STAT_UDCLOW = 1,
    STAT_UDCHIGH = 2,
    STAT_UDCBELOWUDCSW = 4,
    STAT_UDCLIM = 8,
    STAT_EMCYSTOP = 16,
    STAT_MPROT = 32,
    STAT_POTPRESSED = 64,
    STAT_TMPHS = 128,
    STAT_WAITSTART = 256
};

enum ccs_status
{
    CCS_NOTREADY = 0,
    CCS_READY = 1,
    CCS_SWITCHOFF = 2,
    CCS_INTERRUPTION = 3,
    CCS_PRECHARGE = 4,
    CCS_INSULATION = 5,
    CCS_ESTOP = 6,
    CCS_MALFUNCTION = 7,
    CCS_INVAID = 15
};

enum can_devices
{
    CAN_DEV1 = 0,
    CAN_DEV2 = 1
};





