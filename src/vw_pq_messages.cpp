#include "vw_pq_messages.h"
#include "986_can_codec.h"
#include "can_utils.h"
#include <math.h>

namespace params {
VwPqSignals vw_pq;
}

// Message 0x572 (1394) - ZAS_1
// DLC: 2, TX: XXX
bool decodeVwPq0x572(const CANMessage &frame) {
    if (frame.id != 0x572 || frame.len < 2) {
        return false;
    }

    // Fehlerspeichereintrag__ZAS_: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.zas_1_572_fehlerspeichereintrag_zas = readBitsLE(frame.data, 15, 1) != 0;
    // Frei_ZAS_1_3: start=8 len=7 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_zas_1_572_frei_zas_1_3 = readBitsLE(frame.data, 8, 7);
    params::vw_pq.zas_1_572_frei_zas_1_3 = raw_zas_1_572_frei_zas_1_3 * 1.0f + 0.0f;
    // Frei_ZAS_1_2: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.zas_1_572_frei_zas_1_2 = readBitsLE(frame.data, 7, 1) != 0;
    // Klemme_15_SV: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.zas_1_572_klemme_15_sv = readBitsLE(frame.data, 6, 1) != 0;
    // Frei_ZAS_1_1: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.zas_1_572_frei_zas_1_1 = readBitsLE(frame.data, 5, 1) != 0;
    // Klemme_P__Parklichtstellung_: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.zas_1_572_klemme_p_parklichtstellung = readBitsLE(frame.data, 4, 1) != 0;
    // Klemme_50__Starten_: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.zas_1_572_klemme_50_starten = readBitsLE(frame.data, 3, 1) != 0;
    // Klemme_X__Startvorgang_: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.zas_1_572_klemme_x_startvorgang = readBitsLE(frame.data, 2, 1) != 0;
    // Klemme_15__Z_ndung_ein_: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.zas_1_572_klemme_15_z_ndung_ein = readBitsLE(frame.data, 1, 1) != 0;
    // S_Kontakt__Schl_ssel_steckt_: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.zas_1_572_s_kontakt_schl_ssel_steckt = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x572(CANMessage &frame) {
    frame.id = 0x572;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 15, 1, params::vw_pq.zas_1_572_fehlerspeichereintrag_zas ? 1U : 0U);
    // Frei_ZAS_1_3
    int64_t raw_tmp_zas_1_572_frei_zas_1_3 = (int64_t)llround((params::vw_pq.zas_1_572_frei_zas_1_3 - 0.0f) / 1.0f);
    uint64_t raw_zas_1_572_frei_zas_1_3 = raw_tmp_zas_1_572_frei_zas_1_3 < 0 ? 0ULL : (uint64_t)raw_tmp_zas_1_572_frei_zas_1_3;
    if (raw_zas_1_572_frei_zas_1_3 > 0x7FULL) raw_zas_1_572_frei_zas_1_3 = 0x7FULL;
    writeBitsLE(frame.data, 8, 7, (uint32_t)raw_zas_1_572_frei_zas_1_3);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.zas_1_572_frei_zas_1_2 ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.zas_1_572_klemme_15_sv ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.zas_1_572_frei_zas_1_1 ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.zas_1_572_klemme_p_parklichtstellung ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.zas_1_572_klemme_50_starten ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.zas_1_572_klemme_x_startvorgang ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.zas_1_572_klemme_15_z_ndung_ein ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.zas_1_572_s_kontakt_schl_ssel_steckt ? 1U : 0U);
}

// Message 0x538 (1336) - Wischer_1
// DLC: 2, TX: XXX
bool decodeVwPq0x538(const CANMessage &frame) {
    if (frame.id != 0x538 || frame.len < 2) {
        return false;
    }

    // Blockierung_Heckwischer_erkannt: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_blockierung_heckwischer_erkannt = readBitsLE(frame.data, 15, 1) != 0;
    // Frei_Wischer_1_2: start=12 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_wischer_1_538_frei_wischer_1_2 = readBitsLE(frame.data, 12, 3);
    params::vw_pq.wischer_1_538_frei_wischer_1_2 = raw_wischer_1_538_frei_wischer_1_2 * 1.0f + 0.0f;
    // Fehlerspeichereintrag__Wischer_: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_fehlerspeichereintrag_wischer = readBitsLE(frame.data, 11, 1) != 0;
    // Ansteuerung_Scheibenwischer_Hec: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_ansteuerung_scheibenwischer_hec = readBitsLE(frame.data, 10, 1) != 0;
    // Ansteuerung_Wascher_Heck: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_ansteuerung_wascher_heck = readBitsLE(frame.data, 9, 1) != 0;
    // Scheibenwischer_Heck_eingeschal: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_scheibenwischer_heck_eingeschal = readBitsLE(frame.data, 8, 1) != 0;
    // Blockierung_Frontwischer_erkann: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_blockierung_frontwischer_erkann = readBitsLE(frame.data, 7, 1) != 0;
    // Frei_Wischer_1_1: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_frei_wischer_1_1 = readBitsLE(frame.data, 6, 1) != 0;
    // Status_Waschduesenheizung: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_status_waschduesenheizung = readBitsLE(frame.data, 5, 1) != 0;
    // Parklage_Frontwischer: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_parklage_frontwischer = readBitsLE(frame.data, 4, 1) != 0;
    // Ansteuerung_Frontwischer_Schnel: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_ansteuerung_frontwischer_schnel = readBitsLE(frame.data, 3, 1) != 0;
    // Ansteuerung_Frontwischer_Normal: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_ansteuerung_frontwischer_normal = readBitsLE(frame.data, 2, 1) != 0;
    // Ansteuerung_Wascher_Front: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_ansteuerung_wascher_front = readBitsLE(frame.data, 1, 1) != 0;
    // Frontwischer__eingeschaltet: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wischer_1_538_frontwischer_eingeschaltet = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x538(CANMessage &frame) {
    frame.id = 0x538;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 15, 1, params::vw_pq.wischer_1_538_blockierung_heckwischer_erkannt ? 1U : 0U);
    // Frei_Wischer_1_2
    int64_t raw_tmp_wischer_1_538_frei_wischer_1_2 = (int64_t)llround((params::vw_pq.wischer_1_538_frei_wischer_1_2 - 0.0f) / 1.0f);
    uint64_t raw_wischer_1_538_frei_wischer_1_2 = raw_tmp_wischer_1_538_frei_wischer_1_2 < 0 ? 0ULL : (uint64_t)raw_tmp_wischer_1_538_frei_wischer_1_2;
    if (raw_wischer_1_538_frei_wischer_1_2 > 0x7ULL) raw_wischer_1_538_frei_wischer_1_2 = 0x7ULL;
    writeBitsLE(frame.data, 12, 3, (uint32_t)raw_wischer_1_538_frei_wischer_1_2);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.wischer_1_538_fehlerspeichereintrag_wischer ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.wischer_1_538_ansteuerung_scheibenwischer_hec ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.wischer_1_538_ansteuerung_wascher_heck ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.wischer_1_538_scheibenwischer_heck_eingeschal ? 1U : 0U);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.wischer_1_538_blockierung_frontwischer_erkann ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.wischer_1_538_frei_wischer_1_1 ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.wischer_1_538_status_waschduesenheizung ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.wischer_1_538_parklage_frontwischer ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.wischer_1_538_ansteuerung_frontwischer_schnel ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.wischer_1_538_ansteuerung_frontwischer_normal ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.wischer_1_538_ansteuerung_wascher_front ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.wischer_1_538_frontwischer_eingeschaltet ? 1U : 0U);
}

// Message 0x5B8 (1464) - WFS_1
// DLC: 2, TX: XXX
bool decodeVwPq0x5B8(const CANMessage &frame) {
    if (frame.id != 0x5B8 || frame.len < 2) {
        return false;
    }

    // WFS_Textbits: start=8 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_wfs_1_5b8_wfs_textbits = readBitsLE(frame.data, 8, 8);
    params::vw_pq.wfs_1_5b8_wfs_textbits = raw_wfs_1_5b8_wfs_textbits * 1.0f + 0.0f;
    // Frei_WFS_1_1: start=1 len=7 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_wfs_1_5b8_frei_wfs_1_1 = readBitsLE(frame.data, 1, 7);
    params::vw_pq.wfs_1_5b8_frei_wfs_1_1 = raw_wfs_1_5b8_frei_wfs_1_1 * 1.0f + 0.0f;
    // WFS_LED: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.wfs_1_5b8_wfs_led = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x5B8(CANMessage &frame) {
    frame.id = 0x5B8;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // WFS_Textbits
    int64_t raw_tmp_wfs_1_5b8_wfs_textbits = (int64_t)llround((params::vw_pq.wfs_1_5b8_wfs_textbits - 0.0f) / 1.0f);
    uint64_t raw_wfs_1_5b8_wfs_textbits = raw_tmp_wfs_1_5b8_wfs_textbits < 0 ? 0ULL : (uint64_t)raw_tmp_wfs_1_5b8_wfs_textbits;
    if (raw_wfs_1_5b8_wfs_textbits > 0xFFULL) raw_wfs_1_5b8_wfs_textbits = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_wfs_1_5b8_wfs_textbits);
    // Frei_WFS_1_1
    int64_t raw_tmp_wfs_1_5b8_frei_wfs_1_1 = (int64_t)llround((params::vw_pq.wfs_1_5b8_frei_wfs_1_1 - 0.0f) / 1.0f);
    uint64_t raw_wfs_1_5b8_frei_wfs_1_1 = raw_tmp_wfs_1_5b8_frei_wfs_1_1 < 0 ? 0ULL : (uint64_t)raw_tmp_wfs_1_5b8_frei_wfs_1_1;
    if (raw_wfs_1_5b8_frei_wfs_1_1 > 0x7FULL) raw_wfs_1_5b8_frei_wfs_1_1 = 0x7FULL;
    writeBitsLE(frame.data, 1, 7, (uint32_t)raw_wfs_1_5b8_frei_wfs_1_1);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.wfs_1_5b8_wfs_led ? 1U : 0U);
}

// Message 0x448 (1096) - Waehlhebel_1
// DLC: 4, TX: XXX
bool decodeVwPq0x448(const CANMessage &frame) {
    if (frame.id != 0x448 || frame.len < 4) {
        return false;
    }

    // Frei_Waehlhebel_1_1: start=29 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_waehlhebel_1_448_frei_waehlhebel_1_1 = readBitsLE(frame.data, 29, 3);
    params::vw_pq.waehlhebel_1_448_frei_waehlhebel_1_1 = raw_waehlhebel_1_448_frei_waehlhebel_1_1 * 1.0f + 0.0f;
    // Test_aktiv_Flag: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.waehlhebel_1_448_test_aktiv_flag = readBitsLE(frame.data, 28, 1) != 0;
    // Zaehler_Waehlhebel_1: start=24 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Counter Waehlhebel_1
    const uint32_t raw_waehlhebel_1_448_zaehler_waehlhebel_1 = readBitsLE(frame.data, 24, 4);
    params::vw_pq.waehlhebel_1_448_zaehler_waehlhebel_1 = raw_waehlhebel_1_448_zaehler_waehlhebel_1 * 1.0f + 0.0f;
    // Waehlhebel_Testergebnis: start=16 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_waehlhebel_1_448_waehlhebel_testergebnis = readBitsLE(frame.data, 16, 8);
    params::vw_pq.waehlhebel_1_448_waehlhebel_testergebnis = raw_waehlhebel_1_448_waehlhebel_testergebnis * 1.0f + 0.0f;
    // Fehler_Waehlhebel: start=8 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_waehlhebel_1_448_fehler_waehlhebel = readBitsLE(frame.data, 8, 8);
    params::vw_pq.waehlhebel_1_448_fehler_waehlhebel = raw_waehlhebel_1_448_fehler_waehlhebel * 1.0f + 0.0f;
    // Waehlhebelposition: start=4 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_waehlhebel_1_448_waehlhebelposition = readBitsLE(frame.data, 4, 4);
    params::vw_pq.waehlhebel_1_448_waehlhebelposition = raw_waehlhebel_1_448_waehlhebelposition * 1.0f + 0.0f;
    // Waehlhebel_Initialisierung: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.waehlhebel_1_448_waehlhebel_initialisierung = readBitsLE(frame.data, 3, 1) != 0;
    // Shiftlock_Position: start=0 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_waehlhebel_1_448_shiftlock_position = readBitsLE(frame.data, 0, 3);
    params::vw_pq.waehlhebel_1_448_shiftlock_position = raw_waehlhebel_1_448_shiftlock_position * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x448(CANMessage &frame) {
    frame.id = 0x448;
    frame.len = 4;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Frei_Waehlhebel_1_1
    int64_t raw_tmp_waehlhebel_1_448_frei_waehlhebel_1_1 = (int64_t)llround((params::vw_pq.waehlhebel_1_448_frei_waehlhebel_1_1 - 0.0f) / 1.0f);
    uint64_t raw_waehlhebel_1_448_frei_waehlhebel_1_1 = raw_tmp_waehlhebel_1_448_frei_waehlhebel_1_1 < 0 ? 0ULL : (uint64_t)raw_tmp_waehlhebel_1_448_frei_waehlhebel_1_1;
    if (raw_waehlhebel_1_448_frei_waehlhebel_1_1 > 0x7ULL) raw_waehlhebel_1_448_frei_waehlhebel_1_1 = 0x7ULL;
    writeBitsLE(frame.data, 29, 3, (uint32_t)raw_waehlhebel_1_448_frei_waehlhebel_1_1);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.waehlhebel_1_448_test_aktiv_flag ? 1U : 0U);
    // Zaehler_Waehlhebel_1
    int64_t raw_tmp_waehlhebel_1_448_zaehler_waehlhebel_1 = (int64_t)llround((params::vw_pq.waehlhebel_1_448_zaehler_waehlhebel_1 - 0.0f) / 1.0f);
    uint64_t raw_waehlhebel_1_448_zaehler_waehlhebel_1 = raw_tmp_waehlhebel_1_448_zaehler_waehlhebel_1 < 0 ? 0ULL : (uint64_t)raw_tmp_waehlhebel_1_448_zaehler_waehlhebel_1;
    if (raw_waehlhebel_1_448_zaehler_waehlhebel_1 > 0xFULL) raw_waehlhebel_1_448_zaehler_waehlhebel_1 = 0xFULL;
    writeBitsLE(frame.data, 24, 4, (uint32_t)raw_waehlhebel_1_448_zaehler_waehlhebel_1);
    // Waehlhebel_Testergebnis
    int64_t raw_tmp_waehlhebel_1_448_waehlhebel_testergebnis = (int64_t)llround((params::vw_pq.waehlhebel_1_448_waehlhebel_testergebnis - 0.0f) / 1.0f);
    uint64_t raw_waehlhebel_1_448_waehlhebel_testergebnis = raw_tmp_waehlhebel_1_448_waehlhebel_testergebnis < 0 ? 0ULL : (uint64_t)raw_tmp_waehlhebel_1_448_waehlhebel_testergebnis;
    if (raw_waehlhebel_1_448_waehlhebel_testergebnis > 0xFFULL) raw_waehlhebel_1_448_waehlhebel_testergebnis = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_waehlhebel_1_448_waehlhebel_testergebnis);
    // Fehler_Waehlhebel
    int64_t raw_tmp_waehlhebel_1_448_fehler_waehlhebel = (int64_t)llround((params::vw_pq.waehlhebel_1_448_fehler_waehlhebel - 0.0f) / 1.0f);
    uint64_t raw_waehlhebel_1_448_fehler_waehlhebel = raw_tmp_waehlhebel_1_448_fehler_waehlhebel < 0 ? 0ULL : (uint64_t)raw_tmp_waehlhebel_1_448_fehler_waehlhebel;
    if (raw_waehlhebel_1_448_fehler_waehlhebel > 0xFFULL) raw_waehlhebel_1_448_fehler_waehlhebel = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_waehlhebel_1_448_fehler_waehlhebel);
    // Waehlhebelposition
    int64_t raw_tmp_waehlhebel_1_448_waehlhebelposition = (int64_t)llround((params::vw_pq.waehlhebel_1_448_waehlhebelposition - 0.0f) / 1.0f);
    uint64_t raw_waehlhebel_1_448_waehlhebelposition = raw_tmp_waehlhebel_1_448_waehlhebelposition < 0 ? 0ULL : (uint64_t)raw_tmp_waehlhebel_1_448_waehlhebelposition;
    if (raw_waehlhebel_1_448_waehlhebelposition > 0xFULL) raw_waehlhebel_1_448_waehlhebelposition = 0xFULL;
    writeBitsLE(frame.data, 4, 4, (uint32_t)raw_waehlhebel_1_448_waehlhebelposition);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.waehlhebel_1_448_waehlhebel_initialisierung ? 1U : 0U);
    // Shiftlock_Position
    int64_t raw_tmp_waehlhebel_1_448_shiftlock_position = (int64_t)llround((params::vw_pq.waehlhebel_1_448_shiftlock_position - 0.0f) / 1.0f);
    uint64_t raw_waehlhebel_1_448_shiftlock_position = raw_tmp_waehlhebel_1_448_shiftlock_position < 0 ? 0ULL : (uint64_t)raw_tmp_waehlhebel_1_448_shiftlock_position;
    if (raw_waehlhebel_1_448_shiftlock_position > 0x7ULL) raw_waehlhebel_1_448_shiftlock_position = 0x7ULL;
    writeBitsLE(frame.data, 0, 3, (uint32_t)raw_waehlhebel_1_448_shiftlock_position);
}

// Message 0x5D8 (1496) - Verbauliste_1
// DLC: 8, TX: XXX
bool decodeVwPq0x5D8(const CANMessage &frame) {
    if (frame.id != 0x5D8 || frame.len < 8) {
        return false;
    }

    // TV_Tuner: start=63 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_tv_tuner = readBitsLE(frame.data, 63, 1) != 0;
    // DSP: start=62 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_dsp = readBitsLE(frame.data, 62, 1) != 0;
    // CD_Wechsler: start=61 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_cd_wechsler = readBitsLE(frame.data, 61, 1) != 0;
    // Spracheingabe: start=60 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_spracheingabe = readBitsLE(frame.data, 60, 1) != 0;
    // Telematik: start=59 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_telematik = readBitsLE(frame.data, 59, 1) != 0;
    // Navigation: start=58 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_navigation = readBitsLE(frame.data, 58, 1) != 0;
    // Telefon: start=57 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_telefon = readBitsLE(frame.data, 57, 1) != 0;
    // Radio: start=56 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_radio = readBitsLE(frame.data, 56, 1) != 0;
    // MMI_vorne: start=55 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_mmi_vorne = readBitsLE(frame.data, 55, 1) != 0;
    // MMI_hinten: start=54 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_mmi_hinten = readBitsLE(frame.data, 54, 1) != 0;
    // Frei_Verbauliste_1_11: start=53 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_frei_verbauliste_1_11 = readBitsLE(frame.data, 53, 1) != 0;
    // Klimabedienteil_HL: start=52 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_klimabedienteil_hl = readBitsLE(frame.data, 52, 1) != 0;
    // Frei_Verbauliste_1_10: start=50 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_verbauliste_1_5d8_frei_verbauliste_1_10 = readBitsLE(frame.data, 50, 2);
    params::vw_pq.verbauliste_1_5d8_frei_verbauliste_1_10 = raw_verbauliste_1_5d8_frei_verbauliste_1_10 * 1.0f + 0.0f;
    // Tankgeber: start=49 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_tankgeber = readBitsLE(frame.data, 49, 1) != 0;
    // Assistenzfahrlicht: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_assistenzfahrlicht = readBitsLE(frame.data, 48, 1) != 0;
    // Memory_hinter_Fahrer: start=47 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_memory_hinter_fahrer = readBitsLE(frame.data, 47, 1) != 0;
    // Sitzmemory_hinten: start=46 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_sitzmemory_hinten = readBitsLE(frame.data, 46, 1) != 0;
    // Sitzmemory_Beifahrer: start=45 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_sitzmemory_beifahrer = readBitsLE(frame.data, 45, 1) != 0;
    // Anh_ngersteuergeraet: start=44 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_anh_ngersteuergeraet = readBitsLE(frame.data, 44, 1) != 0;
    // Energiemanagement: start=43 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_energiemanagement = readBitsLE(frame.data, 43, 1) != 0;
    // Wischermodul: start=42 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_wischermodul = readBitsLE(frame.data, 42, 1) != 0;
    // EZS___Kessy__Komfort_: start=41 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_ezs_kessy_komfort = readBitsLE(frame.data, 41, 1) != 0;
    // Verdecksteuergeraet: start=40 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_verdecksteuergeraet = readBitsLE(frame.data, 40, 1) != 0;
    // Standheizung: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_standheizung = readBitsLE(frame.data, 39, 1) != 0;
    // Frei_Verbauliste_1_9: start=38 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_frei_verbauliste_1_9 = readBitsLE(frame.data, 38, 1) != 0;
    // Einparkhilfe: start=37 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_einparkhilfe = readBitsLE(frame.data, 37, 1) != 0;
    // Klimasteuergeraet_Komfort: start=36 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_klimasteuergeraet_komfort = readBitsLE(frame.data, 36, 1) != 0;
    // Gateway: start=35 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_gateway = readBitsLE(frame.data, 35, 1) != 0;
    // Lenksaeulenmodul: start=34 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_lenksaeulenmodul = readBitsLE(frame.data, 34, 1) != 0;
    // Reifendruck: start=33 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_reifendruck = readBitsLE(frame.data, 33, 1) != 0;
    // Kombiinstrument_Komfort: start=32 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_kombiinstrument_komfort = readBitsLE(frame.data, 32, 1) != 0;
    // Dachmodul: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_dachmodul = readBitsLE(frame.data, 31, 1) != 0;
    // Memory: start=30 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_memory = readBitsLE(frame.data, 30, 1) != 0;
    // TSG_HR: start=29 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_tsg_hr = readBitsLE(frame.data, 29, 1) != 0;
    // TSG_HL: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_tsg_hl = readBitsLE(frame.data, 28, 1) != 0;
    // TSG_BT: start=27 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_tsg_bt = readBitsLE(frame.data, 27, 1) != 0;
    // TSG_FT: start=26 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_tsg_ft = readBitsLE(frame.data, 26, 1) != 0;
    // ZKE: start=25 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_zke = readBitsLE(frame.data, 25, 1) != 0;
    // BSG_Komfort: start=24 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_bsg_komfort = readBitsLE(frame.data, 24, 1) != 0;
    // Frei_Verbauliste_1_8: start=19 len=5 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_verbauliste_1_5d8_frei_verbauliste_1_8 = readBitsLE(frame.data, 19, 5);
    params::vw_pq.verbauliste_1_5d8_frei_verbauliste_1_8 = raw_verbauliste_1_5d8_frei_verbauliste_1_8 * 1.0f + 0.0f;
    // Stabi_Entkopplung: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_stabi_entkopplung = readBitsLE(frame.data, 18, 1) != 0;
    // Waehlhebel: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_waehlhebel = readBitsLE(frame.data, 17, 1) != 0;
    // Batteriemanager: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_batteriemanager = readBitsLE(frame.data, 16, 1) != 0;
    // Daempfer_SG: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_daempfer_sg = readBitsLE(frame.data, 15, 1) != 0;
    // Niveauregulierung: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_niveauregulierung = readBitsLE(frame.data, 14, 1) != 0;
    // EZS___Kessy__Antrieb_: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_ezs_kessy_antrieb = readBitsLE(frame.data, 13, 1) != 0;
    // Bremsbooster: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_bremsbooster = readBitsLE(frame.data, 12, 1) != 0;
    // Lenkhilfe: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_lenkhilfe = readBitsLE(frame.data, 11, 1) != 0;
    // Kombi_Antrieb: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_kombi_antrieb = readBitsLE(frame.data, 10, 1) != 0;
    // Einspritzpumpe: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_einspritzpumpe = readBitsLE(frame.data, 9, 1) != 0;
    // Lenkwinkel: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_lenkwinkel = readBitsLE(frame.data, 8, 1) != 0;
    // Allrad: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_allrad = readBitsLE(frame.data, 7, 1) != 0;
    // BSG_Antrieb: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_bsg_antrieb = readBitsLE(frame.data, 6, 1) != 0;
    // ADR: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_adr = readBitsLE(frame.data, 5, 1) != 0;
    // Airbag: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_airbag = readBitsLE(frame.data, 4, 1) != 0;
    // Climatronic_Antrieb: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_climatronic_antrieb = readBitsLE(frame.data, 3, 1) != 0;
    // ABS: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_abs = readBitsLE(frame.data, 2, 1) != 0;
    // Getriebesteuergeraet: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_getriebesteuergeraet = readBitsLE(frame.data, 1, 1) != 0;
    // Motorsteuergeraet: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.verbauliste_1_5d8_motorsteuergeraet = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x5D8(CANMessage &frame) {
    frame.id = 0x5D8;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 63, 1, params::vw_pq.verbauliste_1_5d8_tv_tuner ? 1U : 0U);
    writeBitsLE(frame.data, 62, 1, params::vw_pq.verbauliste_1_5d8_dsp ? 1U : 0U);
    writeBitsLE(frame.data, 61, 1, params::vw_pq.verbauliste_1_5d8_cd_wechsler ? 1U : 0U);
    writeBitsLE(frame.data, 60, 1, params::vw_pq.verbauliste_1_5d8_spracheingabe ? 1U : 0U);
    writeBitsLE(frame.data, 59, 1, params::vw_pq.verbauliste_1_5d8_telematik ? 1U : 0U);
    writeBitsLE(frame.data, 58, 1, params::vw_pq.verbauliste_1_5d8_navigation ? 1U : 0U);
    writeBitsLE(frame.data, 57, 1, params::vw_pq.verbauliste_1_5d8_telefon ? 1U : 0U);
    writeBitsLE(frame.data, 56, 1, params::vw_pq.verbauliste_1_5d8_radio ? 1U : 0U);
    writeBitsLE(frame.data, 55, 1, params::vw_pq.verbauliste_1_5d8_mmi_vorne ? 1U : 0U);
    writeBitsLE(frame.data, 54, 1, params::vw_pq.verbauliste_1_5d8_mmi_hinten ? 1U : 0U);
    writeBitsLE(frame.data, 53, 1, params::vw_pq.verbauliste_1_5d8_frei_verbauliste_1_11 ? 1U : 0U);
    writeBitsLE(frame.data, 52, 1, params::vw_pq.verbauliste_1_5d8_klimabedienteil_hl ? 1U : 0U);
    // Frei_Verbauliste_1_10
    int64_t raw_tmp_verbauliste_1_5d8_frei_verbauliste_1_10 = (int64_t)llround((params::vw_pq.verbauliste_1_5d8_frei_verbauliste_1_10 - 0.0f) / 1.0f);
    uint64_t raw_verbauliste_1_5d8_frei_verbauliste_1_10 = raw_tmp_verbauliste_1_5d8_frei_verbauliste_1_10 < 0 ? 0ULL : (uint64_t)raw_tmp_verbauliste_1_5d8_frei_verbauliste_1_10;
    if (raw_verbauliste_1_5d8_frei_verbauliste_1_10 > 0x3ULL) raw_verbauliste_1_5d8_frei_verbauliste_1_10 = 0x3ULL;
    writeBitsLE(frame.data, 50, 2, (uint32_t)raw_verbauliste_1_5d8_frei_verbauliste_1_10);
    writeBitsLE(frame.data, 49, 1, params::vw_pq.verbauliste_1_5d8_tankgeber ? 1U : 0U);
    writeBitsLE(frame.data, 48, 1, params::vw_pq.verbauliste_1_5d8_assistenzfahrlicht ? 1U : 0U);
    writeBitsLE(frame.data, 47, 1, params::vw_pq.verbauliste_1_5d8_memory_hinter_fahrer ? 1U : 0U);
    writeBitsLE(frame.data, 46, 1, params::vw_pq.verbauliste_1_5d8_sitzmemory_hinten ? 1U : 0U);
    writeBitsLE(frame.data, 45, 1, params::vw_pq.verbauliste_1_5d8_sitzmemory_beifahrer ? 1U : 0U);
    writeBitsLE(frame.data, 44, 1, params::vw_pq.verbauliste_1_5d8_anh_ngersteuergeraet ? 1U : 0U);
    writeBitsLE(frame.data, 43, 1, params::vw_pq.verbauliste_1_5d8_energiemanagement ? 1U : 0U);
    writeBitsLE(frame.data, 42, 1, params::vw_pq.verbauliste_1_5d8_wischermodul ? 1U : 0U);
    writeBitsLE(frame.data, 41, 1, params::vw_pq.verbauliste_1_5d8_ezs_kessy_komfort ? 1U : 0U);
    writeBitsLE(frame.data, 40, 1, params::vw_pq.verbauliste_1_5d8_verdecksteuergeraet ? 1U : 0U);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.verbauliste_1_5d8_standheizung ? 1U : 0U);
    writeBitsLE(frame.data, 38, 1, params::vw_pq.verbauliste_1_5d8_frei_verbauliste_1_9 ? 1U : 0U);
    writeBitsLE(frame.data, 37, 1, params::vw_pq.verbauliste_1_5d8_einparkhilfe ? 1U : 0U);
    writeBitsLE(frame.data, 36, 1, params::vw_pq.verbauliste_1_5d8_klimasteuergeraet_komfort ? 1U : 0U);
    writeBitsLE(frame.data, 35, 1, params::vw_pq.verbauliste_1_5d8_gateway ? 1U : 0U);
    writeBitsLE(frame.data, 34, 1, params::vw_pq.verbauliste_1_5d8_lenksaeulenmodul ? 1U : 0U);
    writeBitsLE(frame.data, 33, 1, params::vw_pq.verbauliste_1_5d8_reifendruck ? 1U : 0U);
    writeBitsLE(frame.data, 32, 1, params::vw_pq.verbauliste_1_5d8_kombiinstrument_komfort ? 1U : 0U);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.verbauliste_1_5d8_dachmodul ? 1U : 0U);
    writeBitsLE(frame.data, 30, 1, params::vw_pq.verbauliste_1_5d8_memory ? 1U : 0U);
    writeBitsLE(frame.data, 29, 1, params::vw_pq.verbauliste_1_5d8_tsg_hr ? 1U : 0U);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.verbauliste_1_5d8_tsg_hl ? 1U : 0U);
    writeBitsLE(frame.data, 27, 1, params::vw_pq.verbauliste_1_5d8_tsg_bt ? 1U : 0U);
    writeBitsLE(frame.data, 26, 1, params::vw_pq.verbauliste_1_5d8_tsg_ft ? 1U : 0U);
    writeBitsLE(frame.data, 25, 1, params::vw_pq.verbauliste_1_5d8_zke ? 1U : 0U);
    writeBitsLE(frame.data, 24, 1, params::vw_pq.verbauliste_1_5d8_bsg_komfort ? 1U : 0U);
    // Frei_Verbauliste_1_8
    int64_t raw_tmp_verbauliste_1_5d8_frei_verbauliste_1_8 = (int64_t)llround((params::vw_pq.verbauliste_1_5d8_frei_verbauliste_1_8 - 0.0f) / 1.0f);
    uint64_t raw_verbauliste_1_5d8_frei_verbauliste_1_8 = raw_tmp_verbauliste_1_5d8_frei_verbauliste_1_8 < 0 ? 0ULL : (uint64_t)raw_tmp_verbauliste_1_5d8_frei_verbauliste_1_8;
    if (raw_verbauliste_1_5d8_frei_verbauliste_1_8 > 0x1FULL) raw_verbauliste_1_5d8_frei_verbauliste_1_8 = 0x1FULL;
    writeBitsLE(frame.data, 19, 5, (uint32_t)raw_verbauliste_1_5d8_frei_verbauliste_1_8);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.verbauliste_1_5d8_stabi_entkopplung ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.verbauliste_1_5d8_waehlhebel ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.verbauliste_1_5d8_batteriemanager ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.verbauliste_1_5d8_daempfer_sg ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.verbauliste_1_5d8_niveauregulierung ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.verbauliste_1_5d8_ezs_kessy_antrieb ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.verbauliste_1_5d8_bremsbooster ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.verbauliste_1_5d8_lenkhilfe ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.verbauliste_1_5d8_kombi_antrieb ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.verbauliste_1_5d8_einspritzpumpe ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.verbauliste_1_5d8_lenkwinkel ? 1U : 0U);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.verbauliste_1_5d8_allrad ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.verbauliste_1_5d8_bsg_antrieb ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.verbauliste_1_5d8_adr ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.verbauliste_1_5d8_airbag ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.verbauliste_1_5d8_climatronic_antrieb ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.verbauliste_1_5d8_abs ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.verbauliste_1_5d8_getriebesteuergeraet ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.verbauliste_1_5d8_motorsteuergeraet ? 1U : 0U);
}

// Message 0x5D0 (1488) - Systeminfo_1
// DLC: 6, TX: XXX
bool decodeVwPq0x5D0(const CANMessage &frame) {
    if (frame.id != 0x5D0 || frame.len < 6) {
        return false;
    }

    // CAN_Stand_4_1_Antrieb_Daten_Hau: start=44 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_hau = readBitsLE(frame.data, 44, 4);
    params::vw_pq.systeminfo_1_5d0_can_stand_4_1_antrieb_daten_hau = raw_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_hau * 1.0f + 0.0f;
    // CAN_Stand_4_1_Antrieb_Daten_Neb: start=40 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_neb = readBitsLE(frame.data, 40, 4);
    params::vw_pq.systeminfo_1_5d0_can_stand_4_1_antrieb_daten_neb = raw_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_neb * 1.0f + 0.0f;
    // CAN_Stand_4_1_Komfort_Daten_Hau: start=36 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_systeminfo_1_5d0_can_stand_4_1_komfort_daten_hau = readBitsLE(frame.data, 36, 4);
    params::vw_pq.systeminfo_1_5d0_can_stand_4_1_komfort_daten_hau = raw_systeminfo_1_5d0_can_stand_4_1_komfort_daten_hau * 1.0f + 0.0f;
    // CAN_Stand_4_1_Komfort_Daten_Neb: start=32 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_systeminfo_1_5d0_can_stand_4_1_komfort_daten_neb = readBitsLE(frame.data, 32, 4);
    params::vw_pq.systeminfo_1_5d0_can_stand_4_1_komfort_daten_neb = raw_systeminfo_1_5d0_can_stand_4_1_komfort_daten_neb * 1.0f + 0.0f;
    // Frei_Systeminfo_1_6: start=30 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.systeminfo_1_5d0_frei_systeminfo_1_6 = readBitsLE(frame.data, 30, 1) != 0;
    // Viertuerer: start=29 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.systeminfo_1_5d0_viertuerer = readBitsLE(frame.data, 29, 1) != 0;
    // Rechtslenker: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.systeminfo_1_5d0_rechtslenker = readBitsLE(frame.data, 28, 1) != 0;
    // Fahrzeug_Index: start=24 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_systeminfo_1_5d0_fahrzeug_index = readBitsLE(frame.data, 24, 4);
    params::vw_pq.systeminfo_1_5d0_fahrzeug_index = raw_systeminfo_1_5d0_fahrzeug_index * 1.0f + 0.0f;
    // Fahrzeug_Generation: start=20 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_systeminfo_1_5d0_fahrzeug_generation = readBitsLE(frame.data, 20, 4);
    params::vw_pq.systeminfo_1_5d0_fahrzeug_generation = raw_systeminfo_1_5d0_fahrzeug_generation * 1.0f + 0.0f;
    // Fahrzeug_Derivat: start=16 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_systeminfo_1_5d0_fahrzeug_derivat = readBitsLE(frame.data, 16, 4);
    params::vw_pq.systeminfo_1_5d0_fahrzeug_derivat = raw_systeminfo_1_5d0_fahrzeug_derivat * 1.0f + 0.0f;
    // Fahrzeug_Marke_2: start=12 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_systeminfo_1_5d0_fahrzeug_marke_2 = readBitsLE(frame.data, 12, 4);
    params::vw_pq.systeminfo_1_5d0_fahrzeug_marke_2 = raw_systeminfo_1_5d0_fahrzeug_marke_2 * 1.0f + 0.0f;
    // Fahrzeug_Klasse: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_systeminfo_1_5d0_fahrzeug_klasse = readBitsLE(frame.data, 8, 4);
    params::vw_pq.systeminfo_1_5d0_fahrzeug_klasse = raw_systeminfo_1_5d0_fahrzeug_klasse * 1.0f + 0.0f;
    // Verbauinformation_gueltig: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.systeminfo_1_5d0_verbauinformation_gueltig = readBitsLE(frame.data, 7, 1) != 0;
    // CAN_Infotainment_verbaut: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.systeminfo_1_5d0_can_infotainment_verbaut = readBitsLE(frame.data, 6, 1) != 0;
    // CAN_Infotainment_in_Diagnose: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.systeminfo_1_5d0_can_infotainment_in_diagnose = readBitsLE(frame.data, 5, 1) != 0;
    // Sleep_CAN_Infotainment: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.systeminfo_1_5d0_sleep_can_infotainment = readBitsLE(frame.data, 4, 1) != 0;
    // CAN_Komfort_in_Diagnose: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.systeminfo_1_5d0_can_komfort_in_diagnose = readBitsLE(frame.data, 3, 1) != 0;
    // Sleep_CAN_Komfort: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.systeminfo_1_5d0_sleep_can_komfort = readBitsLE(frame.data, 2, 1) != 0;
    // CAN_Antrieb_in_Diagnose: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.systeminfo_1_5d0_can_antrieb_in_diagnose = readBitsLE(frame.data, 1, 1) != 0;
    // CAN_Extern_zugeschaltet: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.systeminfo_1_5d0_can_extern_zugeschaltet = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x5D0(CANMessage &frame) {
    frame.id = 0x5D0;
    frame.len = 6;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // CAN_Stand_4_1_Antrieb_Daten_Hau
    int64_t raw_tmp_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_hau = (int64_t)llround((params::vw_pq.systeminfo_1_5d0_can_stand_4_1_antrieb_daten_hau - 0.0f) / 1.0f);
    uint64_t raw_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_hau = raw_tmp_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_hau < 0 ? 0ULL : (uint64_t)raw_tmp_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_hau;
    if (raw_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_hau > 0xFULL) raw_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_hau = 0xFULL;
    writeBitsLE(frame.data, 44, 4, (uint32_t)raw_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_hau);
    // CAN_Stand_4_1_Antrieb_Daten_Neb
    int64_t raw_tmp_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_neb = (int64_t)llround((params::vw_pq.systeminfo_1_5d0_can_stand_4_1_antrieb_daten_neb - 0.0f) / 1.0f);
    uint64_t raw_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_neb = raw_tmp_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_neb < 0 ? 0ULL : (uint64_t)raw_tmp_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_neb;
    if (raw_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_neb > 0xFULL) raw_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_neb = 0xFULL;
    writeBitsLE(frame.data, 40, 4, (uint32_t)raw_systeminfo_1_5d0_can_stand_4_1_antrieb_daten_neb);
    // CAN_Stand_4_1_Komfort_Daten_Hau
    int64_t raw_tmp_systeminfo_1_5d0_can_stand_4_1_komfort_daten_hau = (int64_t)llround((params::vw_pq.systeminfo_1_5d0_can_stand_4_1_komfort_daten_hau - 0.0f) / 1.0f);
    uint64_t raw_systeminfo_1_5d0_can_stand_4_1_komfort_daten_hau = raw_tmp_systeminfo_1_5d0_can_stand_4_1_komfort_daten_hau < 0 ? 0ULL : (uint64_t)raw_tmp_systeminfo_1_5d0_can_stand_4_1_komfort_daten_hau;
    if (raw_systeminfo_1_5d0_can_stand_4_1_komfort_daten_hau > 0xFULL) raw_systeminfo_1_5d0_can_stand_4_1_komfort_daten_hau = 0xFULL;
    writeBitsLE(frame.data, 36, 4, (uint32_t)raw_systeminfo_1_5d0_can_stand_4_1_komfort_daten_hau);
    // CAN_Stand_4_1_Komfort_Daten_Neb
    int64_t raw_tmp_systeminfo_1_5d0_can_stand_4_1_komfort_daten_neb = (int64_t)llround((params::vw_pq.systeminfo_1_5d0_can_stand_4_1_komfort_daten_neb - 0.0f) / 1.0f);
    uint64_t raw_systeminfo_1_5d0_can_stand_4_1_komfort_daten_neb = raw_tmp_systeminfo_1_5d0_can_stand_4_1_komfort_daten_neb < 0 ? 0ULL : (uint64_t)raw_tmp_systeminfo_1_5d0_can_stand_4_1_komfort_daten_neb;
    if (raw_systeminfo_1_5d0_can_stand_4_1_komfort_daten_neb > 0xFULL) raw_systeminfo_1_5d0_can_stand_4_1_komfort_daten_neb = 0xFULL;
    writeBitsLE(frame.data, 32, 4, (uint32_t)raw_systeminfo_1_5d0_can_stand_4_1_komfort_daten_neb);
    writeBitsLE(frame.data, 30, 1, params::vw_pq.systeminfo_1_5d0_frei_systeminfo_1_6 ? 1U : 0U);
    writeBitsLE(frame.data, 29, 1, params::vw_pq.systeminfo_1_5d0_viertuerer ? 1U : 0U);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.systeminfo_1_5d0_rechtslenker ? 1U : 0U);
    // Fahrzeug_Index
    int64_t raw_tmp_systeminfo_1_5d0_fahrzeug_index = (int64_t)llround((params::vw_pq.systeminfo_1_5d0_fahrzeug_index - 0.0f) / 1.0f);
    uint64_t raw_systeminfo_1_5d0_fahrzeug_index = raw_tmp_systeminfo_1_5d0_fahrzeug_index < 0 ? 0ULL : (uint64_t)raw_tmp_systeminfo_1_5d0_fahrzeug_index;
    if (raw_systeminfo_1_5d0_fahrzeug_index > 0xFULL) raw_systeminfo_1_5d0_fahrzeug_index = 0xFULL;
    writeBitsLE(frame.data, 24, 4, (uint32_t)raw_systeminfo_1_5d0_fahrzeug_index);
    // Fahrzeug_Generation
    int64_t raw_tmp_systeminfo_1_5d0_fahrzeug_generation = (int64_t)llround((params::vw_pq.systeminfo_1_5d0_fahrzeug_generation - 0.0f) / 1.0f);
    uint64_t raw_systeminfo_1_5d0_fahrzeug_generation = raw_tmp_systeminfo_1_5d0_fahrzeug_generation < 0 ? 0ULL : (uint64_t)raw_tmp_systeminfo_1_5d0_fahrzeug_generation;
    if (raw_systeminfo_1_5d0_fahrzeug_generation > 0xFULL) raw_systeminfo_1_5d0_fahrzeug_generation = 0xFULL;
    writeBitsLE(frame.data, 20, 4, (uint32_t)raw_systeminfo_1_5d0_fahrzeug_generation);
    // Fahrzeug_Derivat
    int64_t raw_tmp_systeminfo_1_5d0_fahrzeug_derivat = (int64_t)llround((params::vw_pq.systeminfo_1_5d0_fahrzeug_derivat - 0.0f) / 1.0f);
    uint64_t raw_systeminfo_1_5d0_fahrzeug_derivat = raw_tmp_systeminfo_1_5d0_fahrzeug_derivat < 0 ? 0ULL : (uint64_t)raw_tmp_systeminfo_1_5d0_fahrzeug_derivat;
    if (raw_systeminfo_1_5d0_fahrzeug_derivat > 0xFULL) raw_systeminfo_1_5d0_fahrzeug_derivat = 0xFULL;
    writeBitsLE(frame.data, 16, 4, (uint32_t)raw_systeminfo_1_5d0_fahrzeug_derivat);
    // Fahrzeug_Marke_2
    int64_t raw_tmp_systeminfo_1_5d0_fahrzeug_marke_2 = (int64_t)llround((params::vw_pq.systeminfo_1_5d0_fahrzeug_marke_2 - 0.0f) / 1.0f);
    uint64_t raw_systeminfo_1_5d0_fahrzeug_marke_2 = raw_tmp_systeminfo_1_5d0_fahrzeug_marke_2 < 0 ? 0ULL : (uint64_t)raw_tmp_systeminfo_1_5d0_fahrzeug_marke_2;
    if (raw_systeminfo_1_5d0_fahrzeug_marke_2 > 0xFULL) raw_systeminfo_1_5d0_fahrzeug_marke_2 = 0xFULL;
    writeBitsLE(frame.data, 12, 4, (uint32_t)raw_systeminfo_1_5d0_fahrzeug_marke_2);
    // Fahrzeug_Klasse
    int64_t raw_tmp_systeminfo_1_5d0_fahrzeug_klasse = (int64_t)llround((params::vw_pq.systeminfo_1_5d0_fahrzeug_klasse - 0.0f) / 1.0f);
    uint64_t raw_systeminfo_1_5d0_fahrzeug_klasse = raw_tmp_systeminfo_1_5d0_fahrzeug_klasse < 0 ? 0ULL : (uint64_t)raw_tmp_systeminfo_1_5d0_fahrzeug_klasse;
    if (raw_systeminfo_1_5d0_fahrzeug_klasse > 0xFULL) raw_systeminfo_1_5d0_fahrzeug_klasse = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_systeminfo_1_5d0_fahrzeug_klasse);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.systeminfo_1_5d0_verbauinformation_gueltig ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.systeminfo_1_5d0_can_infotainment_verbaut ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.systeminfo_1_5d0_can_infotainment_in_diagnose ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.systeminfo_1_5d0_sleep_can_infotainment ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.systeminfo_1_5d0_can_komfort_in_diagnose ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.systeminfo_1_5d0_sleep_can_komfort ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.systeminfo_1_5d0_can_antrieb_in_diagnose ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.systeminfo_1_5d0_can_extern_zugeschaltet ? 1U : 0U);
}

// Message 0x10C (268) - Slave_1
// DLC: 8, TX: XXX
bool decodeVwPq0x10C(const CANMessage &frame) {
    if (frame.id != 0x10C || frame.len < 8) {
        return false;
    }

    // Delta_reduziertes_Sollmoment: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.slave_1_10c_delta_reduziertes_sollmoment = readBitsLE(frame.data, 48, 1) != 0;
    // Delta_Drosselklappenwinkel: start=32 len=16 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_slave_1_10c_delta_drosselklappenwinkel = readBitsLE(frame.data, 32, 16);
    params::vw_pq.slave_1_10c_delta_drosselklappenwinkel = raw_slave_1_10c_delta_drosselklappenwinkel * 1.0f + 0.0f;
    // Frei_Slave_1_1: start=25 len=7 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_slave_1_10c_frei_slave_1_1 = readBitsLE(frame.data, 25, 7);
    params::vw_pq.slave_1_10c_frei_slave_1_1 = raw_slave_1_10c_frei_slave_1_1 * 1.0f + 0.0f;
    // Delta_Zuendwinkelausgabe: start=24 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.slave_1_10c_delta_zuendwinkelausgabe = readBitsLE(frame.data, 24, 1) != 0;
    // Ubat_Freigabe_DVE__Slave_: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.slave_1_10c_ubat_freigabe_dve_slave = readBitsLE(frame.data, 23, 1) != 0;
    // Drosselklappe_Sollwertbegrenzun: start=22 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.slave_1_10c_drosselklappe_sollwertbegrenzun = readBitsLE(frame.data, 22, 1) != 0;
    // Einspritzverbot_lernen__Slave_: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.slave_1_10c_einspritzverbot_lernen_slave = readBitsLE(frame.data, 21, 1) != 0;
    // Master_erkannt__Slave_: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.slave_1_10c_master_erkannt_slave = readBitsLE(frame.data, 20, 1) != 0;
    // Fehler_Momentenausgabe: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.slave_1_10c_fehler_momentenausgabe = readBitsLE(frame.data, 19, 1) != 0;
    // Fehler_Getriebe_Moment: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.slave_1_10c_fehler_getriebe_moment = readBitsLE(frame.data, 18, 1) != 0;
    // Fehler_Bremsenbotschaft: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.slave_1_10c_fehler_bremsenbotschaft = readBitsLE(frame.data, 17, 1) != 0;
    // Indiziertes_Istmoment__Slave_: start=0 len=16 endian=1 sign=+ factor=0.0015259 offset=0 unit="%"
    const uint32_t raw_slave_1_10c_indiziertes_istmoment_slave = readBitsLE(frame.data, 0, 16);
    params::vw_pq.slave_1_10c_indiziertes_istmoment_slave = raw_slave_1_10c_indiziertes_istmoment_slave * 0.0015259f + 0.0f;
    // Timeout_Bremsenbotschaft: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.slave_1_10c_timeout_bremsenbotschaft = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x10C(CANMessage &frame) {
    frame.id = 0x10C;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 48, 1, params::vw_pq.slave_1_10c_delta_reduziertes_sollmoment ? 1U : 0U);
    // Delta_Drosselklappenwinkel
    int64_t raw_tmp_slave_1_10c_delta_drosselklappenwinkel = (int64_t)llround((params::vw_pq.slave_1_10c_delta_drosselklappenwinkel - 0.0f) / 1.0f);
    uint64_t raw_slave_1_10c_delta_drosselklappenwinkel = raw_tmp_slave_1_10c_delta_drosselklappenwinkel < 0 ? 0ULL : (uint64_t)raw_tmp_slave_1_10c_delta_drosselklappenwinkel;
    if (raw_slave_1_10c_delta_drosselklappenwinkel > 0xFFFFULL) raw_slave_1_10c_delta_drosselklappenwinkel = 0xFFFFULL;
    writeBitsLE(frame.data, 32, 16, (uint32_t)raw_slave_1_10c_delta_drosselklappenwinkel);
    // Frei_Slave_1_1
    int64_t raw_tmp_slave_1_10c_frei_slave_1_1 = (int64_t)llround((params::vw_pq.slave_1_10c_frei_slave_1_1 - 0.0f) / 1.0f);
    uint64_t raw_slave_1_10c_frei_slave_1_1 = raw_tmp_slave_1_10c_frei_slave_1_1 < 0 ? 0ULL : (uint64_t)raw_tmp_slave_1_10c_frei_slave_1_1;
    if (raw_slave_1_10c_frei_slave_1_1 > 0x7FULL) raw_slave_1_10c_frei_slave_1_1 = 0x7FULL;
    writeBitsLE(frame.data, 25, 7, (uint32_t)raw_slave_1_10c_frei_slave_1_1);
    writeBitsLE(frame.data, 24, 1, params::vw_pq.slave_1_10c_delta_zuendwinkelausgabe ? 1U : 0U);
    writeBitsLE(frame.data, 23, 1, params::vw_pq.slave_1_10c_ubat_freigabe_dve_slave ? 1U : 0U);
    writeBitsLE(frame.data, 22, 1, params::vw_pq.slave_1_10c_drosselklappe_sollwertbegrenzun ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.slave_1_10c_einspritzverbot_lernen_slave ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.slave_1_10c_master_erkannt_slave ? 1U : 0U);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.slave_1_10c_fehler_momentenausgabe ? 1U : 0U);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.slave_1_10c_fehler_getriebe_moment ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.slave_1_10c_fehler_bremsenbotschaft ? 1U : 0U);
    // Indiziertes_Istmoment__Slave_
    int64_t raw_tmp_slave_1_10c_indiziertes_istmoment_slave = (int64_t)llround((params::vw_pq.slave_1_10c_indiziertes_istmoment_slave - 0.0f) / 0.0015259f);
    uint64_t raw_slave_1_10c_indiziertes_istmoment_slave = raw_tmp_slave_1_10c_indiziertes_istmoment_slave < 0 ? 0ULL : (uint64_t)raw_tmp_slave_1_10c_indiziertes_istmoment_slave;
    if (raw_slave_1_10c_indiziertes_istmoment_slave > 0xFFFFULL) raw_slave_1_10c_indiziertes_istmoment_slave = 0xFFFFULL;
    writeBitsLE(frame.data, 0, 16, (uint32_t)raw_slave_1_10c_indiziertes_istmoment_slave);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.slave_1_10c_timeout_bremsenbotschaft ? 1U : 0U);
}

// Message 0x534 (1332) - Sitz_info
// DLC: 2, TX: XXX
bool decodeVwPq0x534(const CANMessage &frame) {
    if (frame.id != 0x534 || frame.len < 2) {
        return false;
    }

    // Positionserkennung_Beifahrersit: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.sitz_info_534_positionserkennung_beifahrersit = readBitsLE(frame.data, 15, 1) != 0;
    // Beifahrersitz_im_vorderen_Dritt: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.sitz_info_534_beifahrersitz_im_vorderen_dritt = readBitsLE(frame.data, 14, 1) != 0;
    // Positionserkennung_Fahrersitz_u: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.sitz_info_534_positionserkennung_fahrersitz_u = readBitsLE(frame.data, 13, 1) != 0;
    // Fahrersitz_im_vorderen_Drittel: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.sitz_info_534_fahrersitz_im_vorderen_drittel = readBitsLE(frame.data, 12, 1) != 0;
    // Zaehler_Sitzinfo: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_sitz_info_534_zaehler_sitzinfo = readBitsLE(frame.data, 8, 4);
    params::vw_pq.sitz_info_534_zaehler_sitzinfo = raw_sitz_info_534_zaehler_sitzinfo * 1.0f + 0.0f;
    // Checksumme_Sitzinfo: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_sitz_info_534_checksumme_sitzinfo = readBitsLE(frame.data, 0, 8);
    params::vw_pq.sitz_info_534_checksumme_sitzinfo = raw_sitz_info_534_checksumme_sitzinfo * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x534(CANMessage &frame) {
    frame.id = 0x534;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 15, 1, params::vw_pq.sitz_info_534_positionserkennung_beifahrersit ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.sitz_info_534_beifahrersitz_im_vorderen_dritt ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.sitz_info_534_positionserkennung_fahrersitz_u ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.sitz_info_534_fahrersitz_im_vorderen_drittel ? 1U : 0U);
    // Zaehler_Sitzinfo
    int64_t raw_tmp_sitz_info_534_zaehler_sitzinfo = (int64_t)llround((params::vw_pq.sitz_info_534_zaehler_sitzinfo - 0.0f) / 1.0f);
    uint64_t raw_sitz_info_534_zaehler_sitzinfo = raw_tmp_sitz_info_534_zaehler_sitzinfo < 0 ? 0ULL : (uint64_t)raw_tmp_sitz_info_534_zaehler_sitzinfo;
    if (raw_sitz_info_534_zaehler_sitzinfo > 0xFULL) raw_sitz_info_534_zaehler_sitzinfo = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_sitz_info_534_zaehler_sitzinfo);
    // Checksumme_Sitzinfo
    int64_t raw_tmp_sitz_info_534_checksumme_sitzinfo = (int64_t)llround((params::vw_pq.sitz_info_534_checksumme_sitzinfo - 0.0f) / 1.0f);
    uint64_t raw_sitz_info_534_checksumme_sitzinfo = raw_tmp_sitz_info_534_checksumme_sitzinfo < 0 ? 0ULL : (uint64_t)raw_tmp_sitz_info_534_checksumme_sitzinfo;
    if (raw_sitz_info_534_checksumme_sitzinfo > 0xFFULL) raw_sitz_info_534_checksumme_sitzinfo = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_sitz_info_534_checksumme_sitzinfo);
}

// Message 0x712 (1810) - PSG_3
// DLC: 2, TX: XXX
bool decodeVwPq0x712(const CANMessage &frame) {
    if (frame.id != 0x712 || frame.len < 2) {
        return false;
    }

    // Selbsttestergebnis: start=0 len=16 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_psg_3_712_selbsttestergebnis = readBitsLE(frame.data, 0, 16);
    params::vw_pq.psg_3_712_selbsttestergebnis = raw_psg_3_712_selbsttestergebnis * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x712(CANMessage &frame) {
    frame.id = 0x712;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Selbsttestergebnis
    int64_t raw_tmp_psg_3_712_selbsttestergebnis = (int64_t)llround((params::vw_pq.psg_3_712_selbsttestergebnis - 0.0f) / 1.0f);
    uint64_t raw_psg_3_712_selbsttestergebnis = raw_tmp_psg_3_712_selbsttestergebnis < 0 ? 0ULL : (uint64_t)raw_tmp_psg_3_712_selbsttestergebnis;
    if (raw_psg_3_712_selbsttestergebnis > 0xFFFFULL) raw_psg_3_712_selbsttestergebnis = 0xFFFFULL;
    writeBitsLE(frame.data, 0, 16, (uint32_t)raw_psg_3_712_selbsttestergebnis);
}

// Message 0x512 (1298) - PSG_2
// DLC: 8, TX: XXX
bool decodeVwPq0x512(const CANMessage &frame) {
    if (frame.id != 0x512 || frame.len < 8) {
        return false;
    }

    // RAM_Inhalt_4: start=48 len=16 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_psg_2_512_ram_inhalt_4 = readBitsLE(frame.data, 48, 16);
    params::vw_pq.psg_2_512_ram_inhalt_4 = raw_psg_2_512_ram_inhalt_4 * 1.0f + 0.0f;
    // RAM_Inhalt_3: start=32 len=16 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_psg_2_512_ram_inhalt_3 = readBitsLE(frame.data, 32, 16);
    params::vw_pq.psg_2_512_ram_inhalt_3 = raw_psg_2_512_ram_inhalt_3 * 1.0f + 0.0f;
    // RAM_Inhalt_2: start=16 len=16 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_psg_2_512_ram_inhalt_2 = readBitsLE(frame.data, 16, 16);
    params::vw_pq.psg_2_512_ram_inhalt_2 = raw_psg_2_512_ram_inhalt_2 * 1.0f + 0.0f;
    // RAM_Inhalt_1: start=0 len=16 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_psg_2_512_ram_inhalt_1 = readBitsLE(frame.data, 0, 16);
    params::vw_pq.psg_2_512_ram_inhalt_1 = raw_psg_2_512_ram_inhalt_1 * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x512(CANMessage &frame) {
    frame.id = 0x512;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // RAM_Inhalt_4
    int64_t raw_tmp_psg_2_512_ram_inhalt_4 = (int64_t)llround((params::vw_pq.psg_2_512_ram_inhalt_4 - 0.0f) / 1.0f);
    uint64_t raw_psg_2_512_ram_inhalt_4 = raw_tmp_psg_2_512_ram_inhalt_4 < 0 ? 0ULL : (uint64_t)raw_tmp_psg_2_512_ram_inhalt_4;
    if (raw_psg_2_512_ram_inhalt_4 > 0xFFFFULL) raw_psg_2_512_ram_inhalt_4 = 0xFFFFULL;
    writeBitsLE(frame.data, 48, 16, (uint32_t)raw_psg_2_512_ram_inhalt_4);
    // RAM_Inhalt_3
    int64_t raw_tmp_psg_2_512_ram_inhalt_3 = (int64_t)llround((params::vw_pq.psg_2_512_ram_inhalt_3 - 0.0f) / 1.0f);
    uint64_t raw_psg_2_512_ram_inhalt_3 = raw_tmp_psg_2_512_ram_inhalt_3 < 0 ? 0ULL : (uint64_t)raw_tmp_psg_2_512_ram_inhalt_3;
    if (raw_psg_2_512_ram_inhalt_3 > 0xFFFFULL) raw_psg_2_512_ram_inhalt_3 = 0xFFFFULL;
    writeBitsLE(frame.data, 32, 16, (uint32_t)raw_psg_2_512_ram_inhalt_3);
    // RAM_Inhalt_2
    int64_t raw_tmp_psg_2_512_ram_inhalt_2 = (int64_t)llround((params::vw_pq.psg_2_512_ram_inhalt_2 - 0.0f) / 1.0f);
    uint64_t raw_psg_2_512_ram_inhalt_2 = raw_tmp_psg_2_512_ram_inhalt_2 < 0 ? 0ULL : (uint64_t)raw_tmp_psg_2_512_ram_inhalt_2;
    if (raw_psg_2_512_ram_inhalt_2 > 0xFFFFULL) raw_psg_2_512_ram_inhalt_2 = 0xFFFFULL;
    writeBitsLE(frame.data, 16, 16, (uint32_t)raw_psg_2_512_ram_inhalt_2);
    // RAM_Inhalt_1
    int64_t raw_tmp_psg_2_512_ram_inhalt_1 = (int64_t)llround((params::vw_pq.psg_2_512_ram_inhalt_1 - 0.0f) / 1.0f);
    uint64_t raw_psg_2_512_ram_inhalt_1 = raw_tmp_psg_2_512_ram_inhalt_1 < 0 ? 0ULL : (uint64_t)raw_tmp_psg_2_512_ram_inhalt_1;
    if (raw_psg_2_512_ram_inhalt_1 > 0xFFFFULL) raw_psg_2_512_ram_inhalt_1 = 0xFFFFULL;
    writeBitsLE(frame.data, 0, 16, (uint32_t)raw_psg_2_512_ram_inhalt_1);
}

// Message 0x112 (274) - PSG_1
// DLC: 8, TX: XXX
bool decodeVwPq0x112(const CANMessage &frame) {
    if (frame.id != 0x112 || frame.len < 8) {
        return false;
    }

    // Pumpentemperatur__2_1_: start=48 len=16 endian=1 sign=+ factor=0.0625 offset=0 unit="K"
    const uint32_t raw_psg_1_112_pumpentemperatur_2_1 = readBitsLE(frame.data, 48, 16);
    params::vw_pq.psg_1_112_pumpentemperatur_2_1 = raw_psg_1_112_pumpentemperatur_2_1 * 0.0625f + 0.0f;
    // Pumpentemperatur__3_2_2_: start=44 len=12 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_psg_1_112_pumpentemperatur_3_2_2 = readBitsLE(frame.data, 44, 12);
    params::vw_pq.psg_1_112_pumpentemperatur_3_2_2 = raw_psg_1_112_pumpentemperatur_3_2_2 * 1.0f + 0.0f;
    // Zylinderzaehler__3_2_2_: start=43 len=3 endian=1 sign=+ factor=1 offset=0 unit="Zaehler"
    const uint32_t raw_psg_1_112_zylinderzaehler_3_2_2 = readBitsLE(frame.data, 43, 3);
    params::vw_pq.psg_1_112_zylinderzaehler_3_2_2 = raw_psg_1_112_zylinderzaehler_3_2_2 * 1.0f + 0.0f;
    // Ansteuerdauer__3_2_2_: start=32 len=11 endian=1 sign=+ factor=0.0469 offset=0 unit="NW"
    const uint32_t raw_psg_1_112_ansteuerdauer_3_2_2 = readBitsLE(frame.data, 32, 11);
    params::vw_pq.psg_1_112_ansteuerdauer_3_2_2 = raw_psg_1_112_ansteuerdauer_3_2_2 * 0.0469f + 0.0f;
    // Nockenwellendrehzahl__3_2_2_: start=20 len=12 endian=1 sign=+ factor=4 offset=0 unit="upm"
    const uint32_t raw_psg_1_112_nockenwellendrehzahl_3_2_2 = readBitsLE(frame.data, 20, 12);
    params::vw_pq.psg_1_112_nockenwellendrehzahl_3_2_2 = raw_psg_1_112_nockenwellendrehzahl_3_2_2 * 4.0f + 0.0f;
    // Pumpen_Statuswort__3_2_2_: start=0 len=20 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_psg_1_112_pumpen_statuswort_3_2_2 = readBitsLE(frame.data, 0, 20);
    params::vw_pq.psg_1_112_pumpen_statuswort_3_2_2 = raw_psg_1_112_pumpen_statuswort_3_2_2 * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x112(CANMessage &frame) {
    frame.id = 0x112;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Pumpentemperatur__2_1_
    int64_t raw_tmp_psg_1_112_pumpentemperatur_2_1 = (int64_t)llround((params::vw_pq.psg_1_112_pumpentemperatur_2_1 - 0.0f) / 0.0625f);
    uint64_t raw_psg_1_112_pumpentemperatur_2_1 = raw_tmp_psg_1_112_pumpentemperatur_2_1 < 0 ? 0ULL : (uint64_t)raw_tmp_psg_1_112_pumpentemperatur_2_1;
    if (raw_psg_1_112_pumpentemperatur_2_1 > 0xFFFFULL) raw_psg_1_112_pumpentemperatur_2_1 = 0xFFFFULL;
    writeBitsLE(frame.data, 48, 16, (uint32_t)raw_psg_1_112_pumpentemperatur_2_1);
    // Pumpentemperatur__3_2_2_
    int64_t raw_tmp_psg_1_112_pumpentemperatur_3_2_2 = (int64_t)llround((params::vw_pq.psg_1_112_pumpentemperatur_3_2_2 - 0.0f) / 1.0f);
    uint64_t raw_psg_1_112_pumpentemperatur_3_2_2 = raw_tmp_psg_1_112_pumpentemperatur_3_2_2 < 0 ? 0ULL : (uint64_t)raw_tmp_psg_1_112_pumpentemperatur_3_2_2;
    if (raw_psg_1_112_pumpentemperatur_3_2_2 > 0xFFFULL) raw_psg_1_112_pumpentemperatur_3_2_2 = 0xFFFULL;
    writeBitsLE(frame.data, 44, 12, (uint32_t)raw_psg_1_112_pumpentemperatur_3_2_2);
    // Zylinderzaehler__3_2_2_
    int64_t raw_tmp_psg_1_112_zylinderzaehler_3_2_2 = (int64_t)llround((params::vw_pq.psg_1_112_zylinderzaehler_3_2_2 - 0.0f) / 1.0f);
    uint64_t raw_psg_1_112_zylinderzaehler_3_2_2 = raw_tmp_psg_1_112_zylinderzaehler_3_2_2 < 0 ? 0ULL : (uint64_t)raw_tmp_psg_1_112_zylinderzaehler_3_2_2;
    if (raw_psg_1_112_zylinderzaehler_3_2_2 > 0x7ULL) raw_psg_1_112_zylinderzaehler_3_2_2 = 0x7ULL;
    writeBitsLE(frame.data, 43, 3, (uint32_t)raw_psg_1_112_zylinderzaehler_3_2_2);
    // Ansteuerdauer__3_2_2_
    int64_t raw_tmp_psg_1_112_ansteuerdauer_3_2_2 = (int64_t)llround((params::vw_pq.psg_1_112_ansteuerdauer_3_2_2 - 0.0f) / 0.0469f);
    uint64_t raw_psg_1_112_ansteuerdauer_3_2_2 = raw_tmp_psg_1_112_ansteuerdauer_3_2_2 < 0 ? 0ULL : (uint64_t)raw_tmp_psg_1_112_ansteuerdauer_3_2_2;
    if (raw_psg_1_112_ansteuerdauer_3_2_2 > 0x7FFULL) raw_psg_1_112_ansteuerdauer_3_2_2 = 0x7FFULL;
    writeBitsLE(frame.data, 32, 11, (uint32_t)raw_psg_1_112_ansteuerdauer_3_2_2);
    // Nockenwellendrehzahl__3_2_2_
    int64_t raw_tmp_psg_1_112_nockenwellendrehzahl_3_2_2 = (int64_t)llround((params::vw_pq.psg_1_112_nockenwellendrehzahl_3_2_2 - 0.0f) / 4.0f);
    uint64_t raw_psg_1_112_nockenwellendrehzahl_3_2_2 = raw_tmp_psg_1_112_nockenwellendrehzahl_3_2_2 < 0 ? 0ULL : (uint64_t)raw_tmp_psg_1_112_nockenwellendrehzahl_3_2_2;
    if (raw_psg_1_112_nockenwellendrehzahl_3_2_2 > 0xFFFULL) raw_psg_1_112_nockenwellendrehzahl_3_2_2 = 0xFFFULL;
    writeBitsLE(frame.data, 20, 12, (uint32_t)raw_psg_1_112_nockenwellendrehzahl_3_2_2);
    // Pumpen_Statuswort__3_2_2_
    int64_t raw_tmp_psg_1_112_pumpen_statuswort_3_2_2 = (int64_t)llround((params::vw_pq.psg_1_112_pumpen_statuswort_3_2_2 - 0.0f) / 1.0f);
    uint64_t raw_psg_1_112_pumpen_statuswort_3_2_2 = raw_tmp_psg_1_112_pumpen_statuswort_3_2_2 < 0 ? 0ULL : (uint64_t)raw_tmp_psg_1_112_pumpen_statuswort_3_2_2;
    if (raw_psg_1_112_pumpen_statuswort_3_2_2 > 0xFFFFFULL) raw_psg_1_112_pumpen_statuswort_3_2_2 = 0xFFFFFULL;
    writeBitsLE(frame.data, 0, 20, (uint32_t)raw_psg_1_112_pumpen_statuswort_3_2_2);
}

// Message 0x514 (1300) - NOX_1
// DLC: 8, TX: XXX
bool decodeVwPq0x514(const CANMessage &frame) {
    if (frame.id != 0x514 || frame.len < 8) {
        return false;
    }

    // OBD_fuer_NOX: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_nox_1_514_obd_fuer_nox = readBitsLE(frame.data, 56, 8);
    params::vw_pq.nox_1_514_obd_fuer_nox = raw_nox_1_514_obd_fuer_nox * 1.0f + 0.0f;
    // Frei_NOX1_4: start=53 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_nox_1_514_frei_nox1_4 = readBitsLE(frame.data, 53, 3);
    params::vw_pq.nox_1_514_frei_nox1_4 = raw_nox_1_514_frei_nox1_4 * 1.0f + 0.0f;
    // IP2: start=52 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.nox_1_514_ip2 = readBitsLE(frame.data, 52, 1) != 0;
    // IP1: start=51 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.nox_1_514_ip1 = readBitsLE(frame.data, 51, 1) != 0;
    // IP0: start=50 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.nox_1_514_ip0 = readBitsLE(frame.data, 50, 1) != 0;
    // Sondenheizung_NOX: start=49 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.nox_1_514_sondenheizung_nox = readBitsLE(frame.data, 49, 1) != 0;
    // Offsetkorrektur_NOX: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.nox_1_514_offsetkorrektur_nox = readBitsLE(frame.data, 48, 1) != 0;
    // Frei_NOX1_3: start=43 len=5 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_nox_1_514_frei_nox1_3 = readBitsLE(frame.data, 43, 5);
    params::vw_pq.nox_1_514_frei_nox1_3 = raw_nox_1_514_frei_nox1_3 * 1.0f + 0.0f;
    // Sauerstoff_binaer: start=32 len=11 endian=1 sign=+ factor=1 offset=-200 unit="mV"
    const uint32_t raw_nox_1_514_sauerstoff_binaer = readBitsLE(frame.data, 32, 11);
    params::vw_pq.nox_1_514_sauerstoff_binaer = raw_nox_1_514_sauerstoff_binaer * 1.0f + -200.0f;
    // Frei_NOX1_2: start=27 len=5 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_nox_1_514_frei_nox1_2 = readBitsLE(frame.data, 27, 5);
    params::vw_pq.nox_1_514_frei_nox1_2 = raw_nox_1_514_frei_nox1_2 * 1.0f + 0.0f;
    // Sauerstoff_linear: start=16 len=11 endian=1 sign=+ factor=1 offset=0 unit="1000/Lambd"
    const uint32_t raw_nox_1_514_sauerstoff_linear = readBitsLE(frame.data, 16, 11);
    params::vw_pq.nox_1_514_sauerstoff_linear = raw_nox_1_514_sauerstoff_linear * 1.0f + 0.0f;
    // Frei_NOX1_1: start=11 len=5 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_nox_1_514_frei_nox1_1 = readBitsLE(frame.data, 11, 5);
    params::vw_pq.nox_1_514_frei_nox1_1 = raw_nox_1_514_frei_nox1_1 * 1.0f + 0.0f;
    // NOX_Signal: start=0 len=11 endian=1 sign=+ factor=1 offset=0 unit="ppm"
    const uint32_t raw_nox_1_514_nox_signal = readBitsLE(frame.data, 0, 11);
    params::vw_pq.nox_1_514_nox_signal = raw_nox_1_514_nox_signal * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x514(CANMessage &frame) {
    frame.id = 0x514;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // OBD_fuer_NOX
    int64_t raw_tmp_nox_1_514_obd_fuer_nox = (int64_t)llround((params::vw_pq.nox_1_514_obd_fuer_nox - 0.0f) / 1.0f);
    uint64_t raw_nox_1_514_obd_fuer_nox = raw_tmp_nox_1_514_obd_fuer_nox < 0 ? 0ULL : (uint64_t)raw_tmp_nox_1_514_obd_fuer_nox;
    if (raw_nox_1_514_obd_fuer_nox > 0xFFULL) raw_nox_1_514_obd_fuer_nox = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_nox_1_514_obd_fuer_nox);
    // Frei_NOX1_4
    int64_t raw_tmp_nox_1_514_frei_nox1_4 = (int64_t)llround((params::vw_pq.nox_1_514_frei_nox1_4 - 0.0f) / 1.0f);
    uint64_t raw_nox_1_514_frei_nox1_4 = raw_tmp_nox_1_514_frei_nox1_4 < 0 ? 0ULL : (uint64_t)raw_tmp_nox_1_514_frei_nox1_4;
    if (raw_nox_1_514_frei_nox1_4 > 0x7ULL) raw_nox_1_514_frei_nox1_4 = 0x7ULL;
    writeBitsLE(frame.data, 53, 3, (uint32_t)raw_nox_1_514_frei_nox1_4);
    writeBitsLE(frame.data, 52, 1, params::vw_pq.nox_1_514_ip2 ? 1U : 0U);
    writeBitsLE(frame.data, 51, 1, params::vw_pq.nox_1_514_ip1 ? 1U : 0U);
    writeBitsLE(frame.data, 50, 1, params::vw_pq.nox_1_514_ip0 ? 1U : 0U);
    writeBitsLE(frame.data, 49, 1, params::vw_pq.nox_1_514_sondenheizung_nox ? 1U : 0U);
    writeBitsLE(frame.data, 48, 1, params::vw_pq.nox_1_514_offsetkorrektur_nox ? 1U : 0U);
    // Frei_NOX1_3
    int64_t raw_tmp_nox_1_514_frei_nox1_3 = (int64_t)llround((params::vw_pq.nox_1_514_frei_nox1_3 - 0.0f) / 1.0f);
    uint64_t raw_nox_1_514_frei_nox1_3 = raw_tmp_nox_1_514_frei_nox1_3 < 0 ? 0ULL : (uint64_t)raw_tmp_nox_1_514_frei_nox1_3;
    if (raw_nox_1_514_frei_nox1_3 > 0x1FULL) raw_nox_1_514_frei_nox1_3 = 0x1FULL;
    writeBitsLE(frame.data, 43, 5, (uint32_t)raw_nox_1_514_frei_nox1_3);
    // Sauerstoff_binaer
    int64_t raw_tmp_nox_1_514_sauerstoff_binaer = (int64_t)llround((params::vw_pq.nox_1_514_sauerstoff_binaer - -200.0f) / 1.0f);
    uint64_t raw_nox_1_514_sauerstoff_binaer = raw_tmp_nox_1_514_sauerstoff_binaer < 0 ? 0ULL : (uint64_t)raw_tmp_nox_1_514_sauerstoff_binaer;
    if (raw_nox_1_514_sauerstoff_binaer > 0x7FFULL) raw_nox_1_514_sauerstoff_binaer = 0x7FFULL;
    writeBitsLE(frame.data, 32, 11, (uint32_t)raw_nox_1_514_sauerstoff_binaer);
    // Frei_NOX1_2
    int64_t raw_tmp_nox_1_514_frei_nox1_2 = (int64_t)llround((params::vw_pq.nox_1_514_frei_nox1_2 - 0.0f) / 1.0f);
    uint64_t raw_nox_1_514_frei_nox1_2 = raw_tmp_nox_1_514_frei_nox1_2 < 0 ? 0ULL : (uint64_t)raw_tmp_nox_1_514_frei_nox1_2;
    if (raw_nox_1_514_frei_nox1_2 > 0x1FULL) raw_nox_1_514_frei_nox1_2 = 0x1FULL;
    writeBitsLE(frame.data, 27, 5, (uint32_t)raw_nox_1_514_frei_nox1_2);
    // Sauerstoff_linear
    int64_t raw_tmp_nox_1_514_sauerstoff_linear = (int64_t)llround((params::vw_pq.nox_1_514_sauerstoff_linear - 0.0f) / 1.0f);
    uint64_t raw_nox_1_514_sauerstoff_linear = raw_tmp_nox_1_514_sauerstoff_linear < 0 ? 0ULL : (uint64_t)raw_tmp_nox_1_514_sauerstoff_linear;
    if (raw_nox_1_514_sauerstoff_linear > 0x7FFULL) raw_nox_1_514_sauerstoff_linear = 0x7FFULL;
    writeBitsLE(frame.data, 16, 11, (uint32_t)raw_nox_1_514_sauerstoff_linear);
    // Frei_NOX1_1
    int64_t raw_tmp_nox_1_514_frei_nox1_1 = (int64_t)llround((params::vw_pq.nox_1_514_frei_nox1_1 - 0.0f) / 1.0f);
    uint64_t raw_nox_1_514_frei_nox1_1 = raw_tmp_nox_1_514_frei_nox1_1 < 0 ? 0ULL : (uint64_t)raw_tmp_nox_1_514_frei_nox1_1;
    if (raw_nox_1_514_frei_nox1_1 > 0x1FULL) raw_nox_1_514_frei_nox1_1 = 0x1FULL;
    writeBitsLE(frame.data, 11, 5, (uint32_t)raw_nox_1_514_frei_nox1_1);
    // NOX_Signal
    int64_t raw_tmp_nox_1_514_nox_signal = (int64_t)llround((params::vw_pq.nox_1_514_nox_signal - 0.0f) / 1.0f);
    uint64_t raw_nox_1_514_nox_signal = raw_tmp_nox_1_514_nox_signal < 0 ? 0ULL : (uint64_t)raw_tmp_nox_1_514_nox_signal;
    if (raw_nox_1_514_nox_signal > 0x7FFULL) raw_nox_1_514_nox_signal = 0x7FFULL;
    writeBitsLE(frame.data, 0, 11, (uint32_t)raw_nox_1_514_nox_signal);
}

// Message 0x590 (1424) - Niveau_1
// DLC: 6, TX: XXX
bool decodeVwPq0x590(const CANMessage &frame) {
    if (frame.id != 0x590 || frame.len < 6) {
        return false;
    }

    // Beladungszustand: start=40 len=8 endian=1 sign=+ factor=1 offset=0 unit="Zuladung"
    const uint32_t raw_niveau_1_590_beladungszustand = readBitsLE(frame.data, 40, 8);
    params::vw_pq.niveau_1_590_beladungszustand = raw_niveau_1_590_beladungszustand * 1.0f + 0.0f;
    // Fehlerspeichereintrag__Niveau_1: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_fehlerspeichereintrag_niveau_1 = readBitsLE(frame.data, 39, 1) != 0;
    // Systemstatus__Niveau_1_: start=38 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_systemstatus_niveau_1 = readBitsLE(frame.data, 38, 1) != 0;
    // Reserve_Fahrzeugart: start=37 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_reserve_fahrzeugart = readBitsLE(frame.data, 37, 1) != 0;
    // Fahrzeugart_Niveau: start=36 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_fahrzeugart_niveau = readBitsLE(frame.data, 36, 1) != 0;
    // Textbits__Niveau_1_: start=32 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_niveau_1_590_textbits_niveau_1 = readBitsLE(frame.data, 32, 4);
    params::vw_pq.niveau_1_590_textbits_niveau_1 = raw_niveau_1_590_textbits_niveau_1 * 1.0f + 0.0f;
    // Verstellung_HL: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_verstellung_hl = readBitsLE(frame.data, 31, 1) != 0;
    // Verstellung_HR: start=30 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_verstellung_hr = readBitsLE(frame.data, 30, 1) != 0;
    // Verstellung_VL: start=29 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_verstellung_vl = readBitsLE(frame.data, 29, 1) != 0;
    // Verstellung_VR: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_verstellung_vr = readBitsLE(frame.data, 28, 1) != 0;
    // Absenkung_Fahrzeug: start=27 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_absenkung_fahrzeug = readBitsLE(frame.data, 27, 1) != 0;
    // Anhebung_Fahrzeug: start=26 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_anhebung_fahrzeug = readBitsLE(frame.data, 26, 1) != 0;
    // Verstellung_aktiv: start=25 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_verstellung_aktiv = readBitsLE(frame.data, 25, 1) != 0;
    // Kompressorlauf_in_Kuerze: start=24 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_kompressorlauf_in_kuerze = readBitsLE(frame.data, 24, 1) != 0;
    // Frei_Niveau_1_5: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_frei_niveau_1_5 = readBitsLE(frame.data, 23, 1) != 0;
    // Taster_Niveau: start=22 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_taster_niveau = readBitsLE(frame.data, 22, 1) != 0;
    // Parkniveau: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_parkniveau = readBitsLE(frame.data, 21, 1) != 0;
    // Zwischenniveau: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_zwischenniveau = readBitsLE(frame.data, 20, 1) != 0;
    // Niveaustati: start=16 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_niveau_1_590_niveaustati = readBitsLE(frame.data, 16, 4);
    params::vw_pq.niveau_1_590_niveaustati = raw_niveau_1_590_niveaustati * 1.0f + 0.0f;
    // MSG_Einschraenkung: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_msg_einschraenkung = readBitsLE(frame.data, 15, 1) != 0;
    // ESP_Beeinflussung: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_esp_beeinflussung = readBitsLE(frame.data, 14, 1) != 0;
    // Warnlampe_Niveau_1: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_warnlampe_niveau_1 = readBitsLE(frame.data, 13, 1) != 0;
    // Frei_Niveau_1_1: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.niveau_1_590_frei_niveau_1_1 = readBitsLE(frame.data, 12, 1) != 0;
    // Zaehler_Niveau_1: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit="Zaehler"
    const uint32_t raw_niveau_1_590_zaehler_niveau_1 = readBitsLE(frame.data, 8, 4);
    params::vw_pq.niveau_1_590_zaehler_niveau_1 = raw_niveau_1_590_zaehler_niveau_1 * 1.0f + 0.0f;
    // Checksumme_Niveau_1: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_niveau_1_590_checksumme_niveau_1 = readBitsLE(frame.data, 0, 8);
    params::vw_pq.niveau_1_590_checksumme_niveau_1 = raw_niveau_1_590_checksumme_niveau_1 * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x590(CANMessage &frame) {
    frame.id = 0x590;
    frame.len = 6;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Beladungszustand
    int64_t raw_tmp_niveau_1_590_beladungszustand = (int64_t)llround((params::vw_pq.niveau_1_590_beladungszustand - 0.0f) / 1.0f);
    uint64_t raw_niveau_1_590_beladungszustand = raw_tmp_niveau_1_590_beladungszustand < 0 ? 0ULL : (uint64_t)raw_tmp_niveau_1_590_beladungszustand;
    if (raw_niveau_1_590_beladungszustand > 0xFFULL) raw_niveau_1_590_beladungszustand = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_niveau_1_590_beladungszustand);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.niveau_1_590_fehlerspeichereintrag_niveau_1 ? 1U : 0U);
    writeBitsLE(frame.data, 38, 1, params::vw_pq.niveau_1_590_systemstatus_niveau_1 ? 1U : 0U);
    writeBitsLE(frame.data, 37, 1, params::vw_pq.niveau_1_590_reserve_fahrzeugart ? 1U : 0U);
    writeBitsLE(frame.data, 36, 1, params::vw_pq.niveau_1_590_fahrzeugart_niveau ? 1U : 0U);
    // Textbits__Niveau_1_
    int64_t raw_tmp_niveau_1_590_textbits_niveau_1 = (int64_t)llround((params::vw_pq.niveau_1_590_textbits_niveau_1 - 0.0f) / 1.0f);
    uint64_t raw_niveau_1_590_textbits_niveau_1 = raw_tmp_niveau_1_590_textbits_niveau_1 < 0 ? 0ULL : (uint64_t)raw_tmp_niveau_1_590_textbits_niveau_1;
    if (raw_niveau_1_590_textbits_niveau_1 > 0xFULL) raw_niveau_1_590_textbits_niveau_1 = 0xFULL;
    writeBitsLE(frame.data, 32, 4, (uint32_t)raw_niveau_1_590_textbits_niveau_1);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.niveau_1_590_verstellung_hl ? 1U : 0U);
    writeBitsLE(frame.data, 30, 1, params::vw_pq.niveau_1_590_verstellung_hr ? 1U : 0U);
    writeBitsLE(frame.data, 29, 1, params::vw_pq.niveau_1_590_verstellung_vl ? 1U : 0U);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.niveau_1_590_verstellung_vr ? 1U : 0U);
    writeBitsLE(frame.data, 27, 1, params::vw_pq.niveau_1_590_absenkung_fahrzeug ? 1U : 0U);
    writeBitsLE(frame.data, 26, 1, params::vw_pq.niveau_1_590_anhebung_fahrzeug ? 1U : 0U);
    writeBitsLE(frame.data, 25, 1, params::vw_pq.niveau_1_590_verstellung_aktiv ? 1U : 0U);
    writeBitsLE(frame.data, 24, 1, params::vw_pq.niveau_1_590_kompressorlauf_in_kuerze ? 1U : 0U);
    writeBitsLE(frame.data, 23, 1, params::vw_pq.niveau_1_590_frei_niveau_1_5 ? 1U : 0U);
    writeBitsLE(frame.data, 22, 1, params::vw_pq.niveau_1_590_taster_niveau ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.niveau_1_590_parkniveau ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.niveau_1_590_zwischenniveau ? 1U : 0U);
    // Niveaustati
    int64_t raw_tmp_niveau_1_590_niveaustati = (int64_t)llround((params::vw_pq.niveau_1_590_niveaustati - 0.0f) / 1.0f);
    uint64_t raw_niveau_1_590_niveaustati = raw_tmp_niveau_1_590_niveaustati < 0 ? 0ULL : (uint64_t)raw_tmp_niveau_1_590_niveaustati;
    if (raw_niveau_1_590_niveaustati > 0xFULL) raw_niveau_1_590_niveaustati = 0xFULL;
    writeBitsLE(frame.data, 16, 4, (uint32_t)raw_niveau_1_590_niveaustati);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.niveau_1_590_msg_einschraenkung ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.niveau_1_590_esp_beeinflussung ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.niveau_1_590_warnlampe_niveau_1 ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.niveau_1_590_frei_niveau_1_1 ? 1U : 0U);
    // Zaehler_Niveau_1
    int64_t raw_tmp_niveau_1_590_zaehler_niveau_1 = (int64_t)llround((params::vw_pq.niveau_1_590_zaehler_niveau_1 - 0.0f) / 1.0f);
    uint64_t raw_niveau_1_590_zaehler_niveau_1 = raw_tmp_niveau_1_590_zaehler_niveau_1 < 0 ? 0ULL : (uint64_t)raw_tmp_niveau_1_590_zaehler_niveau_1;
    if (raw_niveau_1_590_zaehler_niveau_1 > 0xFULL) raw_niveau_1_590_zaehler_niveau_1 = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_niveau_1_590_zaehler_niveau_1);
    // Checksumme_Niveau_1
    int64_t raw_tmp_niveau_1_590_checksumme_niveau_1 = (int64_t)llround((params::vw_pq.niveau_1_590_checksumme_niveau_1 - 0.0f) / 1.0f);
    uint64_t raw_niveau_1_590_checksumme_niveau_1 = raw_tmp_niveau_1_590_checksumme_niveau_1 < 0 ? 0ULL : (uint64_t)raw_tmp_niveau_1_590_checksumme_niveau_1;
    if (raw_niveau_1_590_checksumme_niveau_1 > 0xFFULL) raw_niveau_1_590_checksumme_niveau_1 = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_niveau_1_590_checksumme_niveau_1);
}

// Message 0x530 (1328) - Navigation_1
// DLC: 7, TX: XXX
bool decodeVwPq0x530(const CANMessage &frame) {
    if (frame.id != 0x530 || frame.len < 7) {
        return false;
    }

    // Kreuzungstyp: start=54 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_navigation_1_530_kreuzungstyp = readBitsLE(frame.data, 54, 2);
    params::vw_pq.navigation_1_530_kreuzungstyp = raw_navigation_1_530_kreuzungstyp * 1.0f + 0.0f;
    // Entfernung_bis_Kreuzung: start=48 len=6 endian=1 sign=+ factor=5 offset=0 unit="m"
    const uint32_t raw_navigation_1_530_entfernung_bis_kreuzung = readBitsLE(frame.data, 48, 6);
    params::vw_pq.navigation_1_530_entfernung_bis_kreuzung = raw_navigation_1_530_entfernung_bis_kreuzung * 5.0f + 0.0f;
    // Entfernung_bis_Kurvenanfang: start=40 len=8 endian=1 sign=+ factor=1 offset=0 unit="m"
    const uint32_t raw_navigation_1_530_entfernung_bis_kurvenanfang = readBitsLE(frame.data, 40, 8);
    params::vw_pq.navigation_1_530_entfernung_bis_kurvenanfang = raw_navigation_1_530_entfernung_bis_kurvenanfang * 1.0f + 0.0f;
    // Voarusliegende_Kurvenrichtung: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.navigation_1_530_voarusliegende_kurvenrichtung = readBitsLE(frame.data, 39, 1) != 0;
    // Vorausliegender_Kurvenverlauf: start=32 len=7 endian=1 sign=+ factor=50 offset=0 unit="m"
    const uint32_t raw_navigation_1_530_vorausliegender_kurvenverlauf = readBitsLE(frame.data, 32, 7);
    params::vw_pq.navigation_1_530_vorausliegender_kurvenverlauf = raw_navigation_1_530_vorausliegender_kurvenverlauf * 50.0f + 0.0f;
    // Fehler_Navigation: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.navigation_1_530_fehler_navigation = readBitsLE(frame.data, 31, 1) != 0;
    // Anzahl_Fahrbahnen__0_ist_unguel: start=28 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_navigation_1_530_anzahl_fahrbahnen_0_ist_unguel = readBitsLE(frame.data, 28, 3);
    params::vw_pq.navigation_1_530_anzahl_fahrbahnen_0_ist_unguel = raw_navigation_1_530_anzahl_fahrbahnen_0_ist_unguel * 1.0f + 0.0f;
    // Strassentyp: start=24 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_navigation_1_530_strassentyp = readBitsLE(frame.data, 24, 4);
    params::vw_pq.navigation_1_530_strassentyp = raw_navigation_1_530_strassentyp * 1.0f + 0.0f;
    // Laenderkennung: start=16 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_navigation_1_530_laenderkennung = readBitsLE(frame.data, 16, 8);
    params::vw_pq.navigation_1_530_laenderkennung = raw_navigation_1_530_laenderkennung * 1.0f + 0.0f;
    // Vorzeichen_Gierrate______: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.navigation_1_530_vorzeichen_gierrate = readBitsLE(frame.data, 15, 1) != 0;
    // Gierratenfehler: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.navigation_1_530_gierratenfehler = readBitsLE(frame.data, 14, 1) != 0;
    // Gierrate: start=0 len=14 endian=1 sign=+ factor=0.01 offset=0 unit="deg/sek"
    const uint32_t raw_navigation_1_530_gierrate = readBitsLE(frame.data, 0, 14);
    params::vw_pq.navigation_1_530_gierrate = raw_navigation_1_530_gierrate * 0.01f + 0.0f;
    return true;
}

void encodeVwPq0x530(CANMessage &frame) {
    frame.id = 0x530;
    frame.len = 7;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Kreuzungstyp
    int64_t raw_tmp_navigation_1_530_kreuzungstyp = (int64_t)llround((params::vw_pq.navigation_1_530_kreuzungstyp - 0.0f) / 1.0f);
    uint64_t raw_navigation_1_530_kreuzungstyp = raw_tmp_navigation_1_530_kreuzungstyp < 0 ? 0ULL : (uint64_t)raw_tmp_navigation_1_530_kreuzungstyp;
    if (raw_navigation_1_530_kreuzungstyp > 0x3ULL) raw_navigation_1_530_kreuzungstyp = 0x3ULL;
    writeBitsLE(frame.data, 54, 2, (uint32_t)raw_navigation_1_530_kreuzungstyp);
    // Entfernung_bis_Kreuzung
    int64_t raw_tmp_navigation_1_530_entfernung_bis_kreuzung = (int64_t)llround((params::vw_pq.navigation_1_530_entfernung_bis_kreuzung - 0.0f) / 5.0f);
    uint64_t raw_navigation_1_530_entfernung_bis_kreuzung = raw_tmp_navigation_1_530_entfernung_bis_kreuzung < 0 ? 0ULL : (uint64_t)raw_tmp_navigation_1_530_entfernung_bis_kreuzung;
    if (raw_navigation_1_530_entfernung_bis_kreuzung > 0x3FULL) raw_navigation_1_530_entfernung_bis_kreuzung = 0x3FULL;
    writeBitsLE(frame.data, 48, 6, (uint32_t)raw_navigation_1_530_entfernung_bis_kreuzung);
    // Entfernung_bis_Kurvenanfang
    int64_t raw_tmp_navigation_1_530_entfernung_bis_kurvenanfang = (int64_t)llround((params::vw_pq.navigation_1_530_entfernung_bis_kurvenanfang - 0.0f) / 1.0f);
    uint64_t raw_navigation_1_530_entfernung_bis_kurvenanfang = raw_tmp_navigation_1_530_entfernung_bis_kurvenanfang < 0 ? 0ULL : (uint64_t)raw_tmp_navigation_1_530_entfernung_bis_kurvenanfang;
    if (raw_navigation_1_530_entfernung_bis_kurvenanfang > 0xFFULL) raw_navigation_1_530_entfernung_bis_kurvenanfang = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_navigation_1_530_entfernung_bis_kurvenanfang);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.navigation_1_530_voarusliegende_kurvenrichtung ? 1U : 0U);
    // Vorausliegender_Kurvenverlauf
    int64_t raw_tmp_navigation_1_530_vorausliegender_kurvenverlauf = (int64_t)llround((params::vw_pq.navigation_1_530_vorausliegender_kurvenverlauf - 0.0f) / 50.0f);
    uint64_t raw_navigation_1_530_vorausliegender_kurvenverlauf = raw_tmp_navigation_1_530_vorausliegender_kurvenverlauf < 0 ? 0ULL : (uint64_t)raw_tmp_navigation_1_530_vorausliegender_kurvenverlauf;
    if (raw_navigation_1_530_vorausliegender_kurvenverlauf > 0x7FULL) raw_navigation_1_530_vorausliegender_kurvenverlauf = 0x7FULL;
    writeBitsLE(frame.data, 32, 7, (uint32_t)raw_navigation_1_530_vorausliegender_kurvenverlauf);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.navigation_1_530_fehler_navigation ? 1U : 0U);
    // Anzahl_Fahrbahnen__0_ist_unguel
    int64_t raw_tmp_navigation_1_530_anzahl_fahrbahnen_0_ist_unguel = (int64_t)llround((params::vw_pq.navigation_1_530_anzahl_fahrbahnen_0_ist_unguel - 0.0f) / 1.0f);
    uint64_t raw_navigation_1_530_anzahl_fahrbahnen_0_ist_unguel = raw_tmp_navigation_1_530_anzahl_fahrbahnen_0_ist_unguel < 0 ? 0ULL : (uint64_t)raw_tmp_navigation_1_530_anzahl_fahrbahnen_0_ist_unguel;
    if (raw_navigation_1_530_anzahl_fahrbahnen_0_ist_unguel > 0x7ULL) raw_navigation_1_530_anzahl_fahrbahnen_0_ist_unguel = 0x7ULL;
    writeBitsLE(frame.data, 28, 3, (uint32_t)raw_navigation_1_530_anzahl_fahrbahnen_0_ist_unguel);
    // Strassentyp
    int64_t raw_tmp_navigation_1_530_strassentyp = (int64_t)llround((params::vw_pq.navigation_1_530_strassentyp - 0.0f) / 1.0f);
    uint64_t raw_navigation_1_530_strassentyp = raw_tmp_navigation_1_530_strassentyp < 0 ? 0ULL : (uint64_t)raw_tmp_navigation_1_530_strassentyp;
    if (raw_navigation_1_530_strassentyp > 0xFULL) raw_navigation_1_530_strassentyp = 0xFULL;
    writeBitsLE(frame.data, 24, 4, (uint32_t)raw_navigation_1_530_strassentyp);
    // Laenderkennung
    int64_t raw_tmp_navigation_1_530_laenderkennung = (int64_t)llround((params::vw_pq.navigation_1_530_laenderkennung - 0.0f) / 1.0f);
    uint64_t raw_navigation_1_530_laenderkennung = raw_tmp_navigation_1_530_laenderkennung < 0 ? 0ULL : (uint64_t)raw_tmp_navigation_1_530_laenderkennung;
    if (raw_navigation_1_530_laenderkennung > 0xFFULL) raw_navigation_1_530_laenderkennung = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_navigation_1_530_laenderkennung);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.navigation_1_530_vorzeichen_gierrate ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.navigation_1_530_gierratenfehler ? 1U : 0U);
    // Gierrate
    int64_t raw_tmp_navigation_1_530_gierrate = (int64_t)llround((params::vw_pq.navigation_1_530_gierrate - 0.0f) / 0.01f);
    uint64_t raw_navigation_1_530_gierrate = raw_tmp_navigation_1_530_gierrate < 0 ? 0ULL : (uint64_t)raw_tmp_navigation_1_530_gierrate;
    if (raw_navigation_1_530_gierrate > 0x3FFFULL) raw_navigation_1_530_gierrate = 0x3FFFULL;
    writeBitsLE(frame.data, 0, 14, (uint32_t)raw_navigation_1_530_gierrate);
}

// Message 0x700 (1792) - MSG_3
// DLC: 3, TX: XXX
bool decodeVwPq0x700(const CANMessage &frame) {
    if (frame.id != 0x700 || frame.len < 3) {
        return false;
    }

    // MSG_Konfiguration: start=16 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_msg_3_700_msg_konfiguration = readBitsLE(frame.data, 16, 8);
    params::vw_pq.msg_3_700_msg_konfiguration = raw_msg_3_700_msg_konfiguration * 1.0f + 0.0f;
    // Lage_des_OT_Impuls: start=0 len=16 endian=1 sign=+ factor=0.01172 offset=-384 unit="KW"
    const uint32_t raw_msg_3_700_lage_des_ot_impuls = readBitsLE(frame.data, 0, 16);
    params::vw_pq.msg_3_700_lage_des_ot_impuls = raw_msg_3_700_lage_des_ot_impuls * 0.01172f + -384.0f;
    return true;
}

void encodeVwPq0x700(CANMessage &frame) {
    frame.id = 0x700;
    frame.len = 3;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // MSG_Konfiguration
    int64_t raw_tmp_msg_3_700_msg_konfiguration = (int64_t)llround((params::vw_pq.msg_3_700_msg_konfiguration - 0.0f) / 1.0f);
    uint64_t raw_msg_3_700_msg_konfiguration = raw_tmp_msg_3_700_msg_konfiguration < 0 ? 0ULL : (uint64_t)raw_tmp_msg_3_700_msg_konfiguration;
    if (raw_msg_3_700_msg_konfiguration > 0xFFULL) raw_msg_3_700_msg_konfiguration = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_msg_3_700_msg_konfiguration);
    // Lage_des_OT_Impuls
    int64_t raw_tmp_msg_3_700_lage_des_ot_impuls = (int64_t)llround((params::vw_pq.msg_3_700_lage_des_ot_impuls - -384.0f) / 0.01172f);
    uint64_t raw_msg_3_700_lage_des_ot_impuls = raw_tmp_msg_3_700_lage_des_ot_impuls < 0 ? 0ULL : (uint64_t)raw_tmp_msg_3_700_lage_des_ot_impuls;
    if (raw_msg_3_700_lage_des_ot_impuls > 0xFFFFULL) raw_msg_3_700_lage_des_ot_impuls = 0xFFFFULL;
    writeBitsLE(frame.data, 0, 16, (uint32_t)raw_msg_3_700_lage_des_ot_impuls);
}

// Message 0x500 (1280) - MSG_2
// DLC: 8, TX: XXX
bool decodeVwPq0x500(const CANMessage &frame) {
    if (frame.id != 0x500 || frame.len < 8) {
        return false;
    }

    // RAM_Adresse_4: start=48 len=16 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_msg_2_500_ram_adresse_4 = readBitsLE(frame.data, 48, 16);
    params::vw_pq.msg_2_500_ram_adresse_4 = raw_msg_2_500_ram_adresse_4 * 1.0f + 0.0f;
    // RAM_Adresse_3: start=32 len=16 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_msg_2_500_ram_adresse_3 = readBitsLE(frame.data, 32, 16);
    params::vw_pq.msg_2_500_ram_adresse_3 = raw_msg_2_500_ram_adresse_3 * 1.0f + 0.0f;
    // RAM_Adresse_2: start=16 len=16 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_msg_2_500_ram_adresse_2 = readBitsLE(frame.data, 16, 16);
    params::vw_pq.msg_2_500_ram_adresse_2 = raw_msg_2_500_ram_adresse_2 * 1.0f + 0.0f;
    // Ram_Adresse_1: start=0 len=16 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_msg_2_500_ram_adresse_1 = readBitsLE(frame.data, 0, 16);
    params::vw_pq.msg_2_500_ram_adresse_1 = raw_msg_2_500_ram_adresse_1 * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x500(CANMessage &frame) {
    frame.id = 0x500;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // RAM_Adresse_4
    int64_t raw_tmp_msg_2_500_ram_adresse_4 = (int64_t)llround((params::vw_pq.msg_2_500_ram_adresse_4 - 0.0f) / 1.0f);
    uint64_t raw_msg_2_500_ram_adresse_4 = raw_tmp_msg_2_500_ram_adresse_4 < 0 ? 0ULL : (uint64_t)raw_tmp_msg_2_500_ram_adresse_4;
    if (raw_msg_2_500_ram_adresse_4 > 0xFFFFULL) raw_msg_2_500_ram_adresse_4 = 0xFFFFULL;
    writeBitsLE(frame.data, 48, 16, (uint32_t)raw_msg_2_500_ram_adresse_4);
    // RAM_Adresse_3
    int64_t raw_tmp_msg_2_500_ram_adresse_3 = (int64_t)llround((params::vw_pq.msg_2_500_ram_adresse_3 - 0.0f) / 1.0f);
    uint64_t raw_msg_2_500_ram_adresse_3 = raw_tmp_msg_2_500_ram_adresse_3 < 0 ? 0ULL : (uint64_t)raw_tmp_msg_2_500_ram_adresse_3;
    if (raw_msg_2_500_ram_adresse_3 > 0xFFFFULL) raw_msg_2_500_ram_adresse_3 = 0xFFFFULL;
    writeBitsLE(frame.data, 32, 16, (uint32_t)raw_msg_2_500_ram_adresse_3);
    // RAM_Adresse_2
    int64_t raw_tmp_msg_2_500_ram_adresse_2 = (int64_t)llround((params::vw_pq.msg_2_500_ram_adresse_2 - 0.0f) / 1.0f);
    uint64_t raw_msg_2_500_ram_adresse_2 = raw_tmp_msg_2_500_ram_adresse_2 < 0 ? 0ULL : (uint64_t)raw_tmp_msg_2_500_ram_adresse_2;
    if (raw_msg_2_500_ram_adresse_2 > 0xFFFFULL) raw_msg_2_500_ram_adresse_2 = 0xFFFFULL;
    writeBitsLE(frame.data, 16, 16, (uint32_t)raw_msg_2_500_ram_adresse_2);
    // Ram_Adresse_1
    int64_t raw_tmp_msg_2_500_ram_adresse_1 = (int64_t)llround((params::vw_pq.msg_2_500_ram_adresse_1 - 0.0f) / 1.0f);
    uint64_t raw_msg_2_500_ram_adresse_1 = raw_tmp_msg_2_500_ram_adresse_1 < 0 ? 0ULL : (uint64_t)raw_tmp_msg_2_500_ram_adresse_1;
    if (raw_msg_2_500_ram_adresse_1 > 0xFFFFULL) raw_msg_2_500_ram_adresse_1 = 0xFFFFULL;
    writeBitsLE(frame.data, 0, 16, (uint32_t)raw_msg_2_500_ram_adresse_1);
}

// Message 0x100 (256) - MSG_1
// DLC: 8, TX: XXX
bool decodeVwPq0x100(const CANMessage &frame) {
    if (frame.id != 0x100 || frame.len < 8) {
        return false;
    }

    // Kurbelwellendrehzahl__3_2_2_: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_msg_1_100_kurbelwellendrehzahl_3_2_2 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.msg_1_100_kurbelwellendrehzahl_3_2_2 = raw_msg_1_100_kurbelwellendrehzahl_3_2_2 * 1.0f + 0.0f;
    // Soll_Foerderbeginn_KW__3_2_2_: start=40 len=16 endian=1 sign=+ factor=0.01172 offset=-384 unit="KW"
    const uint32_t raw_msg_1_100_soll_foerderbeginn_kw_3_2_2 = readBitsLE(frame.data, 40, 16);
    params::vw_pq.msg_1_100_soll_foerderbeginn_kw_3_2_2 = raw_msg_1_100_soll_foerderbeginn_kw_3_2_2 * 0.01172f + -384.0f;
    // Soll_Foerderbeginn_NW__3_2_2_: start=28 len=12 endian=1 sign=+ factor=0.01172 offset=0 unit="degNW"
    const uint32_t raw_msg_1_100_soll_foerderbeginn_nw_3_2_2 = readBitsLE(frame.data, 28, 12);
    params::vw_pq.msg_1_100_soll_foerderbeginn_nw_3_2_2 = raw_msg_1_100_soll_foerderbeginn_nw_3_2_2 * 0.01172f + 0.0f;
    // Soll_Voreinspritzung: start=16 len=12 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_msg_1_100_soll_voreinspritzung = readBitsLE(frame.data, 16, 12);
    params::vw_pq.msg_1_100_soll_voreinspritzung = raw_msg_1_100_soll_voreinspritzung * 1.0f + 0.0f;
    // Soll_Einspritzmenge: start=0 len=16 endian=1 sign=+ factor=0.03125 offset=0 unit="mg/H"
    const uint32_t raw_msg_1_100_soll_einspritzmenge = readBitsLE(frame.data, 0, 16);
    params::vw_pq.msg_1_100_soll_einspritzmenge = raw_msg_1_100_soll_einspritzmenge * 0.03125f + 0.0f;
    return true;
}

void encodeVwPq0x100(CANMessage &frame) {
    frame.id = 0x100;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Kurbelwellendrehzahl__3_2_2_
    int64_t raw_tmp_msg_1_100_kurbelwellendrehzahl_3_2_2 = (int64_t)llround((params::vw_pq.msg_1_100_kurbelwellendrehzahl_3_2_2 - 0.0f) / 1.0f);
    uint64_t raw_msg_1_100_kurbelwellendrehzahl_3_2_2 = raw_tmp_msg_1_100_kurbelwellendrehzahl_3_2_2 < 0 ? 0ULL : (uint64_t)raw_tmp_msg_1_100_kurbelwellendrehzahl_3_2_2;
    if (raw_msg_1_100_kurbelwellendrehzahl_3_2_2 > 0xFFULL) raw_msg_1_100_kurbelwellendrehzahl_3_2_2 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_msg_1_100_kurbelwellendrehzahl_3_2_2);
    // Soll_Foerderbeginn_KW__3_2_2_
    int64_t raw_tmp_msg_1_100_soll_foerderbeginn_kw_3_2_2 = (int64_t)llround((params::vw_pq.msg_1_100_soll_foerderbeginn_kw_3_2_2 - -384.0f) / 0.01172f);
    uint64_t raw_msg_1_100_soll_foerderbeginn_kw_3_2_2 = raw_tmp_msg_1_100_soll_foerderbeginn_kw_3_2_2 < 0 ? 0ULL : (uint64_t)raw_tmp_msg_1_100_soll_foerderbeginn_kw_3_2_2;
    if (raw_msg_1_100_soll_foerderbeginn_kw_3_2_2 > 0xFFFFULL) raw_msg_1_100_soll_foerderbeginn_kw_3_2_2 = 0xFFFFULL;
    writeBitsLE(frame.data, 40, 16, (uint32_t)raw_msg_1_100_soll_foerderbeginn_kw_3_2_2);
    // Soll_Foerderbeginn_NW__3_2_2_
    int64_t raw_tmp_msg_1_100_soll_foerderbeginn_nw_3_2_2 = (int64_t)llround((params::vw_pq.msg_1_100_soll_foerderbeginn_nw_3_2_2 - 0.0f) / 0.01172f);
    uint64_t raw_msg_1_100_soll_foerderbeginn_nw_3_2_2 = raw_tmp_msg_1_100_soll_foerderbeginn_nw_3_2_2 < 0 ? 0ULL : (uint64_t)raw_tmp_msg_1_100_soll_foerderbeginn_nw_3_2_2;
    if (raw_msg_1_100_soll_foerderbeginn_nw_3_2_2 > 0xFFFULL) raw_msg_1_100_soll_foerderbeginn_nw_3_2_2 = 0xFFFULL;
    writeBitsLE(frame.data, 28, 12, (uint32_t)raw_msg_1_100_soll_foerderbeginn_nw_3_2_2);
    // Soll_Voreinspritzung
    int64_t raw_tmp_msg_1_100_soll_voreinspritzung = (int64_t)llround((params::vw_pq.msg_1_100_soll_voreinspritzung - 0.0f) / 1.0f);
    uint64_t raw_msg_1_100_soll_voreinspritzung = raw_tmp_msg_1_100_soll_voreinspritzung < 0 ? 0ULL : (uint64_t)raw_tmp_msg_1_100_soll_voreinspritzung;
    if (raw_msg_1_100_soll_voreinspritzung > 0xFFFULL) raw_msg_1_100_soll_voreinspritzung = 0xFFFULL;
    writeBitsLE(frame.data, 16, 12, (uint32_t)raw_msg_1_100_soll_voreinspritzung);
    // Soll_Einspritzmenge
    int64_t raw_tmp_msg_1_100_soll_einspritzmenge = (int64_t)llround((params::vw_pq.msg_1_100_soll_einspritzmenge - 0.0f) / 0.03125f);
    uint64_t raw_msg_1_100_soll_einspritzmenge = raw_tmp_msg_1_100_soll_einspritzmenge < 0 ? 0ULL : (uint64_t)raw_tmp_msg_1_100_soll_einspritzmenge;
    if (raw_msg_1_100_soll_einspritzmenge > 0xFFFFULL) raw_msg_1_100_soll_einspritzmenge = 0xFFFFULL;
    writeBitsLE(frame.data, 0, 16, (uint32_t)raw_msg_1_100_soll_einspritzmenge);
}

// Message 0x704 (1796) - Motor_NOX
// DLC: 8, TX: XXX
bool decodeVwPq0x704(const CANMessage &frame) {
    if (frame.id != 0x704 || frame.len < 8) {
        return false;
    }

    // Frei_Motor_NOX_1_2: start=24 len=40 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_nox_704_frei_motor_nox_1_2 = readBitsLE(frame.data, 24, 40);
    params::vw_pq.motor_nox_704_frei_motor_nox_1_2 = raw_motor_nox_704_frei_motor_nox_1_2 * 1.0f + 0.0f;
    // Frei_Motor_NOX_1_1: start=19 len=5 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_nox_704_frei_motor_nox_1_1 = readBitsLE(frame.data, 19, 5);
    params::vw_pq.motor_nox_704_frei_motor_nox_1_1 = raw_motor_nox_704_frei_motor_nox_1_1 * 1.0f + 0.0f;
    // Heizleistungsanforderung: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_nox_704_heizleistungsanforderung = readBitsLE(frame.data, 18, 1) != 0;
    // Offsetkorrektur_moeglich: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_nox_704_offsetkorrektur_moeglich = readBitsLE(frame.data, 17, 1) != 0;
    // Betriebsbereich: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_nox_704_betriebsbereich = readBitsLE(frame.data, 16, 1) != 0;
    // Abgastemperatur_NOX: start=8 len=8 endian=1 sign=+ factor=5 offset=-40 unit="C"
    const uint32_t raw_motor_nox_704_abgastemperatur_nox = readBitsLE(frame.data, 8, 8);
    params::vw_pq.motor_nox_704_abgastemperatur_nox = raw_motor_nox_704_abgastemperatur_nox * 5.0f + -40.0f;
    // Abgasdruck_NOX: start=0 len=8 endian=1 sign=+ factor=5 offset=600 unit="mbar"
    const uint32_t raw_motor_nox_704_abgasdruck_nox = readBitsLE(frame.data, 0, 8);
    params::vw_pq.motor_nox_704_abgasdruck_nox = raw_motor_nox_704_abgasdruck_nox * 5.0f + 600.0f;
    return true;
}

void encodeVwPq0x704(CANMessage &frame) {
    frame.id = 0x704;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Frei_Motor_NOX_1_2
    int64_t raw_tmp_motor_nox_704_frei_motor_nox_1_2 = (int64_t)llround((params::vw_pq.motor_nox_704_frei_motor_nox_1_2 - 0.0f) / 1.0f);
    uint64_t raw_motor_nox_704_frei_motor_nox_1_2 = raw_tmp_motor_nox_704_frei_motor_nox_1_2 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_nox_704_frei_motor_nox_1_2;
    if (raw_motor_nox_704_frei_motor_nox_1_2 > 0xFFFFFFFFFFULL) raw_motor_nox_704_frei_motor_nox_1_2 = 0xFFFFFFFFFFULL;
    writeBitsLE(frame.data, 24, 40, (uint32_t)raw_motor_nox_704_frei_motor_nox_1_2);
    // Frei_Motor_NOX_1_1
    int64_t raw_tmp_motor_nox_704_frei_motor_nox_1_1 = (int64_t)llround((params::vw_pq.motor_nox_704_frei_motor_nox_1_1 - 0.0f) / 1.0f);
    uint64_t raw_motor_nox_704_frei_motor_nox_1_1 = raw_tmp_motor_nox_704_frei_motor_nox_1_1 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_nox_704_frei_motor_nox_1_1;
    if (raw_motor_nox_704_frei_motor_nox_1_1 > 0x1FULL) raw_motor_nox_704_frei_motor_nox_1_1 = 0x1FULL;
    writeBitsLE(frame.data, 19, 5, (uint32_t)raw_motor_nox_704_frei_motor_nox_1_1);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.motor_nox_704_heizleistungsanforderung ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.motor_nox_704_offsetkorrektur_moeglich ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.motor_nox_704_betriebsbereich ? 1U : 0U);
    // Abgastemperatur_NOX
    int64_t raw_tmp_motor_nox_704_abgastemperatur_nox = (int64_t)llround((params::vw_pq.motor_nox_704_abgastemperatur_nox - -40.0f) / 5.0f);
    uint64_t raw_motor_nox_704_abgastemperatur_nox = raw_tmp_motor_nox_704_abgastemperatur_nox < 0 ? 0ULL : (uint64_t)raw_tmp_motor_nox_704_abgastemperatur_nox;
    if (raw_motor_nox_704_abgastemperatur_nox > 0xFFULL) raw_motor_nox_704_abgastemperatur_nox = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_motor_nox_704_abgastemperatur_nox);
    // Abgasdruck_NOX
    int64_t raw_tmp_motor_nox_704_abgasdruck_nox = (int64_t)llround((params::vw_pq.motor_nox_704_abgasdruck_nox - 600.0f) / 5.0f);
    uint64_t raw_motor_nox_704_abgasdruck_nox = raw_tmp_motor_nox_704_abgasdruck_nox < 0 ? 0ULL : (uint64_t)raw_tmp_motor_nox_704_abgasdruck_nox;
    if (raw_motor_nox_704_abgasdruck_nox > 0xFFULL) raw_motor_nox_704_abgasdruck_nox = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_motor_nox_704_abgasdruck_nox);
}

// Message 0x384 (900) - Motor_Momente
// DLC: 8, TX: XXX
bool decodeVwPq0x384(const CANMessage &frame) {
    if (frame.id != 0x384 || frame.len < 8) {
        return false;
    }

    // Momentenangaben_ungenau__Moment: start=35 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_momente_384_momentenangaben_ungenau_moment = readBitsLE(frame.data, 35, 2);
    params::vw_pq.motor_momente_384_momentenangaben_ungenau_moment = raw_motor_momente_384_momentenangaben_ungenau_moment * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x384(CANMessage &frame) {
    frame.id = 0x384;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Momentenangaben_ungenau__Moment
    int64_t raw_tmp_motor_momente_384_momentenangaben_ungenau_moment = (int64_t)llround((params::vw_pq.motor_momente_384_momentenangaben_ungenau_moment - 0.0f) / 1.0f);
    uint64_t raw_motor_momente_384_momentenangaben_ungenau_moment = raw_tmp_motor_momente_384_momentenangaben_ungenau_moment < 0 ? 0ULL : (uint64_t)raw_tmp_motor_momente_384_momentenangaben_ungenau_moment;
    if (raw_motor_momente_384_momentenangaben_ungenau_moment > 0x3ULL) raw_motor_momente_384_momentenangaben_ungenau_moment = 0x3ULL;
    writeBitsLE(frame.data, 35, 2, (uint32_t)raw_motor_momente_384_momentenangaben_ungenau_moment);
}

// Message 0x580 (1408) - Motor_Flexia
// DLC: 8, TX: XXX
bool decodeVwPq0x580(const CANMessage &frame) {
    if (frame.id != 0x580 || frame.len < 8) {
        return false;
    }

    // Ansaugsystem m0: start=63 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_flexia_580_ansaugsystem_m0 = readBitsLE(frame.data, 63, 1) != 0;
    // Hubraum m0: start=56 len=7 endian=1 sign=+ factor=0.1 offset=0 unit="l"
    const uint32_t raw_motor_flexia_580_hubraum_m0 = readBitsLE(frame.data, 56, 7);
    params::vw_pq.motor_flexia_580_hubraum_m0 = raw_motor_flexia_580_hubraum_m0 * 0.1f + 0.0f;
    // Steigung_der_Befuellungskennlin m1: start=56 len=8 endian=1 sign=+ factor=0.001 offset=0 unit="l/mm"
    const uint32_t raw_motor_flexia_580_steigung_der_befuellungskennlin_m1 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.motor_flexia_580_steigung_der_befuellungskennlin_m1 = raw_motor_flexia_580_steigung_der_befuellungskennlin_m1 * 0.001f + 0.0f;
    // Anzahl_Zylinder m0: start=52 len=4 endian=1 sign=+ factor=1 offset=0 unit="Vent./Zyl."
    const uint32_t raw_motor_flexia_580_anzahl_zylinder_m0 = readBitsLE(frame.data, 52, 4);
    params::vw_pq.motor_flexia_580_anzahl_zylinder_m0 = raw_motor_flexia_580_anzahl_zylinder_m0 * 1.0f + 0.0f;
    // Bewertungsfaktor_Russindex_Turb m1: start=50 len=6 endian=1 sign=+ factor=0.1 offset=0 unit=""
    const uint32_t raw_motor_flexia_580_bewertungsfaktor_russindex_turb_m1 = readBitsLE(frame.data, 50, 6);
    params::vw_pq.motor_flexia_580_bewertungsfaktor_russindex_turb_m1 = raw_motor_flexia_580_bewertungsfaktor_russindex_turb_m1 * 0.1f + 0.0f;
    // Anzahl_Ventile m0: start=49 len=3 endian=1 sign=+ factor=1 offset=0 unit="Vent./Zyl."
    const uint32_t raw_motor_flexia_580_anzahl_ventile_m0 = readBitsLE(frame.data, 49, 3);
    params::vw_pq.motor_flexia_580_anzahl_ventile_m0 = raw_motor_flexia_580_anzahl_ventile_m0 * 1.0f + 0.0f;
    // Bewertungsfaktor_Verschleissind m1: start=44 len=6 endian=1 sign=+ factor=0.1 offset=0 unit=""
    const uint32_t raw_motor_flexia_580_bewertungsfaktor_verschleissind_m1 = readBitsLE(frame.data, 44, 6);
    params::vw_pq.motor_flexia_580_bewertungsfaktor_verschleissind_m1 = raw_motor_flexia_580_bewertungsfaktor_verschleissind_m1 * 0.1f + 0.0f;
    // Hersteller_Code m1: start=40 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_flexia_580_hersteller_code_m1 = readBitsLE(frame.data, 40, 4);
    params::vw_pq.motor_flexia_580_hersteller_code_m1 = raw_motor_flexia_580_hersteller_code_m1 * 1.0f + 0.0f;
    // Motorleistung m0: start=40 len=9 endian=1 sign=+ factor=1 offset=0 unit="KW"
    const uint32_t raw_motor_flexia_580_motorleistung_m0 = readBitsLE(frame.data, 40, 9);
    params::vw_pq.motor_flexia_580_motorleistung_m0 = raw_motor_flexia_580_motorleistung_m0 * 1.0f + 0.0f;
    // Max_Drehmoment m0: start=32 len=8 endian=1 sign=+ factor=10 offset=0 unit="Nm"
    const uint32_t raw_motor_flexia_580_max_drehmoment_m0 = readBitsLE(frame.data, 32, 8);
    params::vw_pq.motor_flexia_580_max_drehmoment_m0 = raw_motor_flexia_580_max_drehmoment_m0 * 10.0f + 0.0f;
    // Normierter_Verbrauch m1: start=32 len=8 endian=1 sign=+ factor=10 offset=0 unit="l/Zyl."
    const uint32_t raw_motor_flexia_580_normierter_verbrauch_m1 = readBitsLE(frame.data, 32, 8);
    params::vw_pq.motor_flexia_580_normierter_verbrauch_m1 = raw_motor_flexia_580_normierter_verbrauch_m1 * 10.0f + 0.0f;
    // Oelniveauschwelle m1: start=24 len=8 endian=1 sign=+ factor=0.25 offset=0 unit="cm"
    const uint32_t raw_motor_flexia_580_oelniveauschwelle_m1 = readBitsLE(frame.data, 24, 8);
    params::vw_pq.motor_flexia_580_oelniveauschwelle_m1 = raw_motor_flexia_580_oelniveauschwelle_m1 * 0.25f + 0.0f;
    // Drehzahl_MaxNorm m0: start=24 len=8 endian=1 sign=+ factor=100 offset=0 unit="U/min"
    const uint32_t raw_motor_flexia_580_drehzahl_maxnorm_m0 = readBitsLE(frame.data, 24, 8);
    params::vw_pq.motor_flexia_580_drehzahl_maxnorm_m0 = raw_motor_flexia_580_drehzahl_maxnorm_m0 * 100.0f + 0.0f;
    // Verschleissindex: start=16 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_flexia_580_verschleissindex = readBitsLE(frame.data, 16, 8);
    params::vw_pq.motor_flexia_580_verschleissindex = raw_motor_flexia_580_verschleissindex * 1.0f + 0.0f;
    // Russindex: start=8 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_flexia_580_russindex = readBitsLE(frame.data, 8, 8);
    params::vw_pq.motor_flexia_580_russindex = raw_motor_flexia_580_russindex * 1.0f + 0.0f;
    // Verbrennungsart: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Type of combustion
    params::vw_pq.motor_flexia_580_verbrennungsart = readBitsLE(frame.data, 7, 1) != 0;
    // Frei_Motor_Flexia_1: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_flexia_580_frei_motor_flexia_1 = readBitsLE(frame.data, 6, 1) != 0;
    // Warm_Up_Cycle: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_flexia_580_warm_up_cycle = readBitsLE(frame.data, 5, 1) != 0;
    // Driving_Cycle: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_flexia_580_driving_cycle = readBitsLE(frame.data, 4, 1) != 0;
    // Zaehler_Motor_Flexia: start=1 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Counter Motor_Flexia
    const uint32_t raw_motor_flexia_580_zaehler_motor_flexia = readBitsLE(frame.data, 1, 3);
    params::vw_pq.motor_flexia_580_zaehler_motor_flexia = raw_motor_flexia_580_zaehler_motor_flexia * 1.0f + 0.0f;
    // Multiplex_Schalter_Motor_Flexia M: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_flexia_580_multiplex_schalter_motor_flexia_m = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x580(CANMessage &frame) {
    frame.id = 0x580;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 63, 1, params::vw_pq.motor_flexia_580_ansaugsystem_m0 ? 1U : 0U);
    // Hubraum m0
    int64_t raw_tmp_motor_flexia_580_hubraum_m0 = (int64_t)llround((params::vw_pq.motor_flexia_580_hubraum_m0 - 0.0f) / 0.1f);
    uint64_t raw_motor_flexia_580_hubraum_m0 = raw_tmp_motor_flexia_580_hubraum_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_hubraum_m0;
    if (raw_motor_flexia_580_hubraum_m0 > 0x7FULL) raw_motor_flexia_580_hubraum_m0 = 0x7FULL;
    writeBitsLE(frame.data, 56, 7, (uint32_t)raw_motor_flexia_580_hubraum_m0);
    // Steigung_der_Befuellungskennlin m1
    int64_t raw_tmp_motor_flexia_580_steigung_der_befuellungskennlin_m1 = (int64_t)llround((params::vw_pq.motor_flexia_580_steigung_der_befuellungskennlin_m1 - 0.0f) / 0.001f);
    uint64_t raw_motor_flexia_580_steigung_der_befuellungskennlin_m1 = raw_tmp_motor_flexia_580_steigung_der_befuellungskennlin_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_steigung_der_befuellungskennlin_m1;
    if (raw_motor_flexia_580_steigung_der_befuellungskennlin_m1 > 0xFFULL) raw_motor_flexia_580_steigung_der_befuellungskennlin_m1 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_motor_flexia_580_steigung_der_befuellungskennlin_m1);
    // Anzahl_Zylinder m0
    int64_t raw_tmp_motor_flexia_580_anzahl_zylinder_m0 = (int64_t)llround((params::vw_pq.motor_flexia_580_anzahl_zylinder_m0 - 0.0f) / 1.0f);
    uint64_t raw_motor_flexia_580_anzahl_zylinder_m0 = raw_tmp_motor_flexia_580_anzahl_zylinder_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_anzahl_zylinder_m0;
    if (raw_motor_flexia_580_anzahl_zylinder_m0 > 0xFULL) raw_motor_flexia_580_anzahl_zylinder_m0 = 0xFULL;
    writeBitsLE(frame.data, 52, 4, (uint32_t)raw_motor_flexia_580_anzahl_zylinder_m0);
    // Bewertungsfaktor_Russindex_Turb m1
    int64_t raw_tmp_motor_flexia_580_bewertungsfaktor_russindex_turb_m1 = (int64_t)llround((params::vw_pq.motor_flexia_580_bewertungsfaktor_russindex_turb_m1 - 0.0f) / 0.1f);
    uint64_t raw_motor_flexia_580_bewertungsfaktor_russindex_turb_m1 = raw_tmp_motor_flexia_580_bewertungsfaktor_russindex_turb_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_bewertungsfaktor_russindex_turb_m1;
    if (raw_motor_flexia_580_bewertungsfaktor_russindex_turb_m1 > 0x3FULL) raw_motor_flexia_580_bewertungsfaktor_russindex_turb_m1 = 0x3FULL;
    writeBitsLE(frame.data, 50, 6, (uint32_t)raw_motor_flexia_580_bewertungsfaktor_russindex_turb_m1);
    // Anzahl_Ventile m0
    int64_t raw_tmp_motor_flexia_580_anzahl_ventile_m0 = (int64_t)llround((params::vw_pq.motor_flexia_580_anzahl_ventile_m0 - 0.0f) / 1.0f);
    uint64_t raw_motor_flexia_580_anzahl_ventile_m0 = raw_tmp_motor_flexia_580_anzahl_ventile_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_anzahl_ventile_m0;
    if (raw_motor_flexia_580_anzahl_ventile_m0 > 0x7ULL) raw_motor_flexia_580_anzahl_ventile_m0 = 0x7ULL;
    writeBitsLE(frame.data, 49, 3, (uint32_t)raw_motor_flexia_580_anzahl_ventile_m0);
    // Bewertungsfaktor_Verschleissind m1
    int64_t raw_tmp_motor_flexia_580_bewertungsfaktor_verschleissind_m1 = (int64_t)llround((params::vw_pq.motor_flexia_580_bewertungsfaktor_verschleissind_m1 - 0.0f) / 0.1f);
    uint64_t raw_motor_flexia_580_bewertungsfaktor_verschleissind_m1 = raw_tmp_motor_flexia_580_bewertungsfaktor_verschleissind_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_bewertungsfaktor_verschleissind_m1;
    if (raw_motor_flexia_580_bewertungsfaktor_verschleissind_m1 > 0x3FULL) raw_motor_flexia_580_bewertungsfaktor_verschleissind_m1 = 0x3FULL;
    writeBitsLE(frame.data, 44, 6, (uint32_t)raw_motor_flexia_580_bewertungsfaktor_verschleissind_m1);
    // Hersteller_Code m1
    int64_t raw_tmp_motor_flexia_580_hersteller_code_m1 = (int64_t)llround((params::vw_pq.motor_flexia_580_hersteller_code_m1 - 0.0f) / 1.0f);
    uint64_t raw_motor_flexia_580_hersteller_code_m1 = raw_tmp_motor_flexia_580_hersteller_code_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_hersteller_code_m1;
    if (raw_motor_flexia_580_hersteller_code_m1 > 0xFULL) raw_motor_flexia_580_hersteller_code_m1 = 0xFULL;
    writeBitsLE(frame.data, 40, 4, (uint32_t)raw_motor_flexia_580_hersteller_code_m1);
    // Motorleistung m0
    int64_t raw_tmp_motor_flexia_580_motorleistung_m0 = (int64_t)llround((params::vw_pq.motor_flexia_580_motorleistung_m0 - 0.0f) / 1.0f);
    uint64_t raw_motor_flexia_580_motorleistung_m0 = raw_tmp_motor_flexia_580_motorleistung_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_motorleistung_m0;
    if (raw_motor_flexia_580_motorleistung_m0 > 0x1FFULL) raw_motor_flexia_580_motorleistung_m0 = 0x1FFULL;
    writeBitsLE(frame.data, 40, 9, (uint32_t)raw_motor_flexia_580_motorleistung_m0);
    // Max_Drehmoment m0
    int64_t raw_tmp_motor_flexia_580_max_drehmoment_m0 = (int64_t)llround((params::vw_pq.motor_flexia_580_max_drehmoment_m0 - 0.0f) / 10.0f);
    uint64_t raw_motor_flexia_580_max_drehmoment_m0 = raw_tmp_motor_flexia_580_max_drehmoment_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_max_drehmoment_m0;
    if (raw_motor_flexia_580_max_drehmoment_m0 > 0xFFULL) raw_motor_flexia_580_max_drehmoment_m0 = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_motor_flexia_580_max_drehmoment_m0);
    // Normierter_Verbrauch m1
    int64_t raw_tmp_motor_flexia_580_normierter_verbrauch_m1 = (int64_t)llround((params::vw_pq.motor_flexia_580_normierter_verbrauch_m1 - 0.0f) / 10.0f);
    uint64_t raw_motor_flexia_580_normierter_verbrauch_m1 = raw_tmp_motor_flexia_580_normierter_verbrauch_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_normierter_verbrauch_m1;
    if (raw_motor_flexia_580_normierter_verbrauch_m1 > 0xFFULL) raw_motor_flexia_580_normierter_verbrauch_m1 = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_motor_flexia_580_normierter_verbrauch_m1);
    // Oelniveauschwelle m1
    int64_t raw_tmp_motor_flexia_580_oelniveauschwelle_m1 = (int64_t)llround((params::vw_pq.motor_flexia_580_oelniveauschwelle_m1 - 0.0f) / 0.25f);
    uint64_t raw_motor_flexia_580_oelniveauschwelle_m1 = raw_tmp_motor_flexia_580_oelniveauschwelle_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_oelniveauschwelle_m1;
    if (raw_motor_flexia_580_oelniveauschwelle_m1 > 0xFFULL) raw_motor_flexia_580_oelniveauschwelle_m1 = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_motor_flexia_580_oelniveauschwelle_m1);
    // Drehzahl_MaxNorm m0
    int64_t raw_tmp_motor_flexia_580_drehzahl_maxnorm_m0 = (int64_t)llround((params::vw_pq.motor_flexia_580_drehzahl_maxnorm_m0 - 0.0f) / 100.0f);
    uint64_t raw_motor_flexia_580_drehzahl_maxnorm_m0 = raw_tmp_motor_flexia_580_drehzahl_maxnorm_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_drehzahl_maxnorm_m0;
    if (raw_motor_flexia_580_drehzahl_maxnorm_m0 > 0xFFULL) raw_motor_flexia_580_drehzahl_maxnorm_m0 = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_motor_flexia_580_drehzahl_maxnorm_m0);
    // Verschleissindex
    int64_t raw_tmp_motor_flexia_580_verschleissindex = (int64_t)llround((params::vw_pq.motor_flexia_580_verschleissindex - 0.0f) / 1.0f);
    uint64_t raw_motor_flexia_580_verschleissindex = raw_tmp_motor_flexia_580_verschleissindex < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_verschleissindex;
    if (raw_motor_flexia_580_verschleissindex > 0xFFULL) raw_motor_flexia_580_verschleissindex = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_motor_flexia_580_verschleissindex);
    // Russindex
    int64_t raw_tmp_motor_flexia_580_russindex = (int64_t)llround((params::vw_pq.motor_flexia_580_russindex - 0.0f) / 1.0f);
    uint64_t raw_motor_flexia_580_russindex = raw_tmp_motor_flexia_580_russindex < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_russindex;
    if (raw_motor_flexia_580_russindex > 0xFFULL) raw_motor_flexia_580_russindex = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_motor_flexia_580_russindex);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.motor_flexia_580_verbrennungsart ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.motor_flexia_580_frei_motor_flexia_1 ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.motor_flexia_580_warm_up_cycle ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.motor_flexia_580_driving_cycle ? 1U : 0U);
    // Zaehler_Motor_Flexia
    int64_t raw_tmp_motor_flexia_580_zaehler_motor_flexia = (int64_t)llround((params::vw_pq.motor_flexia_580_zaehler_motor_flexia - 0.0f) / 1.0f);
    uint64_t raw_motor_flexia_580_zaehler_motor_flexia = raw_tmp_motor_flexia_580_zaehler_motor_flexia < 0 ? 0ULL : (uint64_t)raw_tmp_motor_flexia_580_zaehler_motor_flexia;
    if (raw_motor_flexia_580_zaehler_motor_flexia > 0x7ULL) raw_motor_flexia_580_zaehler_motor_flexia = 0x7ULL;
    writeBitsLE(frame.data, 1, 3, (uint32_t)raw_motor_flexia_580_zaehler_motor_flexia);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.motor_flexia_580_multiplex_schalter_motor_flexia_m ? 1U : 0U);
}

// Message 0x588 (1416) - Motor_7
// DLC: 8, TX: XXX
bool decodeVwPq0x588(const CANMessage &frame) {
    if (frame.id != 0x588 || frame.len < 8) {
        return false;
    }

    // Oltemperatur: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Oil temperature
    const uint32_t raw_motor_7_588_oltemperatur = readBitsLE(frame.data, 56, 8);
    params::vw_pq.motor_7_588_oltemperatur = raw_motor_7_588_oltemperatur * 1.0f + 0.0f;
    // Frei_Motor_7_3: start=40 len=16 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_7_588_frei_motor_7_3 = readBitsLE(frame.data, 40, 16);
    params::vw_pq.motor_7_588_frei_motor_7_3 = raw_motor_7_588_frei_motor_7_3 * 1.0f + 0.0f;
    // Ladedruck: start=32 len=8 endian=1 sign=+ factor=0.01 offset=0 unit="bar"
    // DBC comment: Boost Pressure
    const uint32_t raw_motor_7_588_ladedruck = readBitsLE(frame.data, 32, 8);
    params::vw_pq.motor_7_588_ladedruck = raw_motor_7_588_ladedruck * 0.01f + 0.0f;
    // Vorzeichen_Motordrehzahlgradien: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_7_588_vorzeichen_motordrehzahlgradien = readBitsLE(frame.data, 31, 1) != 0;
    // Motordrehzahlgradient: start=24 len=7 endian=1 sign=+ factor=1 offset=0 unit="U/min"
    // DBC comment: Engine speed gradient
    const uint32_t raw_motor_7_588_motordrehzahlgradient = readBitsLE(frame.data, 24, 7);
    params::vw_pq.motor_7_588_motordrehzahlgradient = raw_motor_7_588_motordrehzahlgradient * 1.0f + 0.0f;
    // Hoeheninfo__Motor_7_: start=16 len=8 endian=1 sign=+ factor=0.00787 offset=0 unit=""
    // DBC comment: Altitude correction factor
    const uint32_t raw_motor_7_588_hoeheninfo_motor_7 = readBitsLE(frame.data, 16, 8);
    params::vw_pq.motor_7_588_hoeheninfo_motor_7 = raw_motor_7_588_hoeheninfo_motor_7 * 0.00787f + 0.0f;
    // Klemme_DFM: start=8 len=8 endian=1 sign=+ factor=0.4 offset=0 unit="%"
    const uint32_t raw_motor_7_588_klemme_dfm = readBitsLE(frame.data, 8, 8);
    params::vw_pq.motor_7_588_klemme_dfm = raw_motor_7_588_klemme_dfm * 0.4f + 0.0f;
    // PTC___Gluehstifte_ausgeschaltet: start=5 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_7_588_ptc_gluehstifte_ausgeschaltet = readBitsLE(frame.data, 5, 3);
    params::vw_pq.motor_7_588_ptc_gluehstifte_ausgeschaltet = raw_motor_7_588_ptc_gluehstifte_ausgeschaltet * 1.0f + 0.0f;
    // Frei_Motor_7_1: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_7_588_frei_motor_7_1 = readBitsLE(frame.data, 4, 1) != 0;
    // Fehlerspeichereintrag__Motor_7_: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_7_588_fehlerspeichereintrag_motor_7 = readBitsLE(frame.data, 3, 1) != 0;
    // Statusbit_Geschwindikeitsbegren: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_7_588_statusbit_geschwindikeitsbegren = readBitsLE(frame.data, 2, 1) != 0;
    // Geschwindigkegrenzung_aktivierb: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_7_588_geschwindigkegrenzung_aktivierb = readBitsLE(frame.data, 1, 1) != 0;
    // Leerlauf_Solldrehzahl_auf_Max_W: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_7_588_leerlauf_solldrehzahl_auf_max_w = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x588(CANMessage &frame) {
    frame.id = 0x588;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Oltemperatur
    int64_t raw_tmp_motor_7_588_oltemperatur = (int64_t)llround((params::vw_pq.motor_7_588_oltemperatur - 0.0f) / 1.0f);
    uint64_t raw_motor_7_588_oltemperatur = raw_tmp_motor_7_588_oltemperatur < 0 ? 0ULL : (uint64_t)raw_tmp_motor_7_588_oltemperatur;
    if (raw_motor_7_588_oltemperatur > 0xFFULL) raw_motor_7_588_oltemperatur = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_motor_7_588_oltemperatur);
    // Frei_Motor_7_3
    int64_t raw_tmp_motor_7_588_frei_motor_7_3 = (int64_t)llround((params::vw_pq.motor_7_588_frei_motor_7_3 - 0.0f) / 1.0f);
    uint64_t raw_motor_7_588_frei_motor_7_3 = raw_tmp_motor_7_588_frei_motor_7_3 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_7_588_frei_motor_7_3;
    if (raw_motor_7_588_frei_motor_7_3 > 0xFFFFULL) raw_motor_7_588_frei_motor_7_3 = 0xFFFFULL;
    writeBitsLE(frame.data, 40, 16, (uint32_t)raw_motor_7_588_frei_motor_7_3);
    // Ladedruck
    int64_t raw_tmp_motor_7_588_ladedruck = (int64_t)llround((params::vw_pq.motor_7_588_ladedruck - 0.0f) / 0.01f);
    uint64_t raw_motor_7_588_ladedruck = raw_tmp_motor_7_588_ladedruck < 0 ? 0ULL : (uint64_t)raw_tmp_motor_7_588_ladedruck;
    if (raw_motor_7_588_ladedruck > 0xFFULL) raw_motor_7_588_ladedruck = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_motor_7_588_ladedruck);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.motor_7_588_vorzeichen_motordrehzahlgradien ? 1U : 0U);
    // Motordrehzahlgradient
    int64_t raw_tmp_motor_7_588_motordrehzahlgradient = (int64_t)llround((params::vw_pq.motor_7_588_motordrehzahlgradient - 0.0f) / 1.0f);
    uint64_t raw_motor_7_588_motordrehzahlgradient = raw_tmp_motor_7_588_motordrehzahlgradient < 0 ? 0ULL : (uint64_t)raw_tmp_motor_7_588_motordrehzahlgradient;
    if (raw_motor_7_588_motordrehzahlgradient > 0x7FULL) raw_motor_7_588_motordrehzahlgradient = 0x7FULL;
    writeBitsLE(frame.data, 24, 7, (uint32_t)raw_motor_7_588_motordrehzahlgradient);
    // Hoeheninfo__Motor_7_
    int64_t raw_tmp_motor_7_588_hoeheninfo_motor_7 = (int64_t)llround((params::vw_pq.motor_7_588_hoeheninfo_motor_7 - 0.0f) / 0.00787f);
    uint64_t raw_motor_7_588_hoeheninfo_motor_7 = raw_tmp_motor_7_588_hoeheninfo_motor_7 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_7_588_hoeheninfo_motor_7;
    if (raw_motor_7_588_hoeheninfo_motor_7 > 0xFFULL) raw_motor_7_588_hoeheninfo_motor_7 = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_motor_7_588_hoeheninfo_motor_7);
    // Klemme_DFM
    int64_t raw_tmp_motor_7_588_klemme_dfm = (int64_t)llround((params::vw_pq.motor_7_588_klemme_dfm - 0.0f) / 0.4f);
    uint64_t raw_motor_7_588_klemme_dfm = raw_tmp_motor_7_588_klemme_dfm < 0 ? 0ULL : (uint64_t)raw_tmp_motor_7_588_klemme_dfm;
    if (raw_motor_7_588_klemme_dfm > 0xFFULL) raw_motor_7_588_klemme_dfm = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_motor_7_588_klemme_dfm);
    // PTC___Gluehstifte_ausgeschaltet
    int64_t raw_tmp_motor_7_588_ptc_gluehstifte_ausgeschaltet = (int64_t)llround((params::vw_pq.motor_7_588_ptc_gluehstifte_ausgeschaltet - 0.0f) / 1.0f);
    uint64_t raw_motor_7_588_ptc_gluehstifte_ausgeschaltet = raw_tmp_motor_7_588_ptc_gluehstifte_ausgeschaltet < 0 ? 0ULL : (uint64_t)raw_tmp_motor_7_588_ptc_gluehstifte_ausgeschaltet;
    if (raw_motor_7_588_ptc_gluehstifte_ausgeschaltet > 0x7ULL) raw_motor_7_588_ptc_gluehstifte_ausgeschaltet = 0x7ULL;
    writeBitsLE(frame.data, 5, 3, (uint32_t)raw_motor_7_588_ptc_gluehstifte_ausgeschaltet);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.motor_7_588_frei_motor_7_1 ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.motor_7_588_fehlerspeichereintrag_motor_7 ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.motor_7_588_statusbit_geschwindikeitsbegren ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.motor_7_588_geschwindigkegrenzung_aktivierb ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.motor_7_588_leerlauf_solldrehzahl_auf_max_w ? 1U : 0U);
}

// Message 0x488 (1160) - Motor_6
// DLC: 8, TX: XXX
bool decodeVwPq0x488(const CANMessage &frame) {
    if (frame.id != 0x488 || frame.len < 8) {
        return false;
    }

    // Zaehler_Motor_6: start=60 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Counter Motor_6
    const uint32_t raw_motor_6_488_zaehler_motor_6 = readBitsLE(frame.data, 60, 4);
    params::vw_pq.motor_6_488_zaehler_motor_6 = raw_motor_6_488_zaehler_motor_6 * 1.0f + 0.0f;
    // Frei_Motor_6_4: start=58 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_6_488_frei_motor_6_4 = readBitsLE(frame.data, 58, 2);
    params::vw_pq.motor_6_488_frei_motor_6_4 = raw_motor_6_488_frei_motor_6_4 * 1.0f + 0.0f;
    // ltemperaturschutz: start=57 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_6_488_ltemperaturschutz = readBitsLE(frame.data, 57, 1) != 0;
    // GRA_Bremseingriff_Freigabe: start=56 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_6_488_gra_bremseingriff_freigabe = readBitsLE(frame.data, 56, 1) != 0;
    // Frei_Motor_6_3: start=48 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_6_488_frei_motor_6_3 = readBitsLE(frame.data, 48, 8);
    params::vw_pq.motor_6_488_frei_motor_6_3 = raw_motor_6_488_frei_motor_6_3 * 1.0f + 0.0f;
    // Ruckmeldung_Momenten: start=40 len=8 endian=1 sign=+ factor=0.39 offset=0 unit=""
    // DBC comment: Feedback torque-integral gear intervention
    const uint32_t raw_motor_6_488_ruckmeldung_momenten = readBitsLE(frame.data, 40, 8);
    params::vw_pq.motor_6_488_ruckmeldung_momenten = raw_motor_6_488_ruckmeldung_momenten * 0.39f + 0.0f;
    // GRA_Sollbeschleunigung: start=32 len=8 endian=1 sign=+ factor=0.024 offset=-3.984 unit="m/s2"
    // DBC comment: GRA target acceleration
    const uint32_t raw_motor_6_488_gra_sollbeschleunigung = readBitsLE(frame.data, 32, 8);
    params::vw_pq.motor_6_488_gra_sollbeschleunigung = raw_motor_6_488_gra_sollbeschleunigung * 0.024f + -3.984f;
    // Hoeheninfo__Motor_6_: start=24 len=8 endian=1 sign=+ factor=0.00787 offset=0 unit=""
    // DBC comment: Altitude Correction
    const uint32_t raw_motor_6_488_hoeheninfo_motor_6 = readBitsLE(frame.data, 24, 8);
    params::vw_pq.motor_6_488_hoeheninfo_motor_6 = raw_motor_6_488_hoeheninfo_motor_6 * 0.00787f + 0.0f;
    // Istmoment_f_r_Getriebe: start=16 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="MDI"
    // DBC comment: Actual torque for gear
    const uint32_t raw_motor_6_488_istmoment_f_r_getriebe = readBitsLE(frame.data, 16, 8);
    params::vw_pq.motor_6_488_istmoment_f_r_getriebe = raw_motor_6_488_istmoment_f_r_getriebe * 0.39f + 0.0f;
    // Sollmoment_f_r_Getriebe: start=8 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="MDI"
    // DBC comment: Target torque for gearbox
    const uint32_t raw_motor_6_488_sollmoment_f_r_getriebe = readBitsLE(frame.data, 8, 8);
    params::vw_pq.motor_6_488_sollmoment_f_r_getriebe = raw_motor_6_488_sollmoment_f_r_getriebe * 0.39f + 0.0f;
    // Checksumme_Motor_6: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Checksum Motor_6
    const uint32_t raw_motor_6_488_checksumme_motor_6 = readBitsLE(frame.data, 0, 8);
    params::vw_pq.motor_6_488_checksumme_motor_6 = raw_motor_6_488_checksumme_motor_6 * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x488(CANMessage &frame) {
    frame.id = 0x488;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Zaehler_Motor_6
    int64_t raw_tmp_motor_6_488_zaehler_motor_6 = (int64_t)llround((params::vw_pq.motor_6_488_zaehler_motor_6 - 0.0f) / 1.0f);
    uint64_t raw_motor_6_488_zaehler_motor_6 = raw_tmp_motor_6_488_zaehler_motor_6 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_6_488_zaehler_motor_6;
    if (raw_motor_6_488_zaehler_motor_6 > 0xFULL) raw_motor_6_488_zaehler_motor_6 = 0xFULL;
    writeBitsLE(frame.data, 60, 4, (uint32_t)raw_motor_6_488_zaehler_motor_6);
    // Frei_Motor_6_4
    int64_t raw_tmp_motor_6_488_frei_motor_6_4 = (int64_t)llround((params::vw_pq.motor_6_488_frei_motor_6_4 - 0.0f) / 1.0f);
    uint64_t raw_motor_6_488_frei_motor_6_4 = raw_tmp_motor_6_488_frei_motor_6_4 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_6_488_frei_motor_6_4;
    if (raw_motor_6_488_frei_motor_6_4 > 0x3ULL) raw_motor_6_488_frei_motor_6_4 = 0x3ULL;
    writeBitsLE(frame.data, 58, 2, (uint32_t)raw_motor_6_488_frei_motor_6_4);
    writeBitsLE(frame.data, 57, 1, params::vw_pq.motor_6_488_ltemperaturschutz ? 1U : 0U);
    writeBitsLE(frame.data, 56, 1, params::vw_pq.motor_6_488_gra_bremseingriff_freigabe ? 1U : 0U);
    // Frei_Motor_6_3
    int64_t raw_tmp_motor_6_488_frei_motor_6_3 = (int64_t)llround((params::vw_pq.motor_6_488_frei_motor_6_3 - 0.0f) / 1.0f);
    uint64_t raw_motor_6_488_frei_motor_6_3 = raw_tmp_motor_6_488_frei_motor_6_3 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_6_488_frei_motor_6_3;
    if (raw_motor_6_488_frei_motor_6_3 > 0xFFULL) raw_motor_6_488_frei_motor_6_3 = 0xFFULL;
    writeBitsLE(frame.data, 48, 8, (uint32_t)raw_motor_6_488_frei_motor_6_3);
    // Ruckmeldung_Momenten
    int64_t raw_tmp_motor_6_488_ruckmeldung_momenten = (int64_t)llround((params::vw_pq.motor_6_488_ruckmeldung_momenten - 0.0f) / 0.39f);
    uint64_t raw_motor_6_488_ruckmeldung_momenten = raw_tmp_motor_6_488_ruckmeldung_momenten < 0 ? 0ULL : (uint64_t)raw_tmp_motor_6_488_ruckmeldung_momenten;
    if (raw_motor_6_488_ruckmeldung_momenten > 0xFFULL) raw_motor_6_488_ruckmeldung_momenten = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_motor_6_488_ruckmeldung_momenten);
    // GRA_Sollbeschleunigung
    int64_t raw_tmp_motor_6_488_gra_sollbeschleunigung = (int64_t)llround((params::vw_pq.motor_6_488_gra_sollbeschleunigung - -3.984f) / 0.024f);
    uint64_t raw_motor_6_488_gra_sollbeschleunigung = raw_tmp_motor_6_488_gra_sollbeschleunigung < 0 ? 0ULL : (uint64_t)raw_tmp_motor_6_488_gra_sollbeschleunigung;
    if (raw_motor_6_488_gra_sollbeschleunigung > 0xFFULL) raw_motor_6_488_gra_sollbeschleunigung = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_motor_6_488_gra_sollbeschleunigung);
    // Hoeheninfo__Motor_6_
    int64_t raw_tmp_motor_6_488_hoeheninfo_motor_6 = (int64_t)llround((params::vw_pq.motor_6_488_hoeheninfo_motor_6 - 0.0f) / 0.00787f);
    uint64_t raw_motor_6_488_hoeheninfo_motor_6 = raw_tmp_motor_6_488_hoeheninfo_motor_6 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_6_488_hoeheninfo_motor_6;
    if (raw_motor_6_488_hoeheninfo_motor_6 > 0xFFULL) raw_motor_6_488_hoeheninfo_motor_6 = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_motor_6_488_hoeheninfo_motor_6);
    // Istmoment_f_r_Getriebe
    int64_t raw_tmp_motor_6_488_istmoment_f_r_getriebe = (int64_t)llround((params::vw_pq.motor_6_488_istmoment_f_r_getriebe - 0.0f) / 0.39f);
    uint64_t raw_motor_6_488_istmoment_f_r_getriebe = raw_tmp_motor_6_488_istmoment_f_r_getriebe < 0 ? 0ULL : (uint64_t)raw_tmp_motor_6_488_istmoment_f_r_getriebe;
    if (raw_motor_6_488_istmoment_f_r_getriebe > 0xFFULL) raw_motor_6_488_istmoment_f_r_getriebe = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_motor_6_488_istmoment_f_r_getriebe);
    // Sollmoment_f_r_Getriebe
    int64_t raw_tmp_motor_6_488_sollmoment_f_r_getriebe = (int64_t)llround((params::vw_pq.motor_6_488_sollmoment_f_r_getriebe - 0.0f) / 0.39f);
    uint64_t raw_motor_6_488_sollmoment_f_r_getriebe = raw_tmp_motor_6_488_sollmoment_f_r_getriebe < 0 ? 0ULL : (uint64_t)raw_tmp_motor_6_488_sollmoment_f_r_getriebe;
    if (raw_motor_6_488_sollmoment_f_r_getriebe > 0xFFULL) raw_motor_6_488_sollmoment_f_r_getriebe = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_motor_6_488_sollmoment_f_r_getriebe);
    // Checksumme_Motor_6
    int64_t raw_tmp_motor_6_488_checksumme_motor_6 = (int64_t)llround((params::vw_pq.motor_6_488_checksumme_motor_6 - 0.0f) / 1.0f);
    uint64_t raw_motor_6_488_checksumme_motor_6 = raw_tmp_motor_6_488_checksumme_motor_6 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_6_488_checksumme_motor_6;
    if (raw_motor_6_488_checksumme_motor_6 > 0xFFULL) raw_motor_6_488_checksumme_motor_6 = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_motor_6_488_checksumme_motor_6);
}

// Message 0x480 (1152) - Motor_5
// DLC: 8, TX: Motor
// Comment: Motor message 5 - Contains engine type, consumption, lamps, and diagnostic info
bool decodeVwPq0x480(const CANMessage &frame) {
    if (frame.id != 0x480 || frame.len < 8) {
        return false;
    }

    // MO5_Mp_Code M: start=6 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_5_480_mo5_mp_code_m = readBitsLE(frame.data, 6, 2);
    params::vw_pq.motor_5_480_mo5_mp_code_m = raw_motor_5_480_mo5_mp_code_m * 1.0f + 0.0f;
    // MO5_max_Moment m0: start=0 len=6 endian=1 sign=+ factor=10 offset=0 unit="Nm"
    const uint32_t raw_motor_5_480_mo5_max_moment_m0 = readBitsLE(frame.data, 0, 6);
    params::vw_pq.motor_5_480_mo5_max_moment_m0 = raw_motor_5_480_mo5_max_moment_m0 * 10.0f + 0.0f;
    // MO5_Drehzahl m1: start=0 len=6 endian=1 sign=+ factor=100 offset=0 unit="U/min"
    const uint32_t raw_motor_5_480_mo5_drehzahl_m1 = readBitsLE(frame.data, 0, 6);
    params::vw_pq.motor_5_480_mo5_drehzahl_m1 = raw_motor_5_480_mo5_drehzahl_m1 * 100.0f + 0.0f;
    // MO5_Motortyp m2: start=0 len=6 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_5_480_mo5_motortyp_m2 = readBitsLE(frame.data, 0, 6);
    params::vw_pq.motor_5_480_mo5_motortyp_m2 = raw_motor_5_480_mo5_motortyp_m2 * 1.0f + 0.0f;
    // MO5_Abgastyp m3: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_5_480_mo5_abgastyp_m3 = readBitsLE(frame.data, 0, 1) != 0;
    // MO5_Abgastyp2 m3: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_5_480_mo5_abgastyp2_m3 = readBitsLE(frame.data, 1, 1) != 0;
    // MO5_Abgastyp3 m3: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_5_480_mo5_abgastyp3_m3 = readBitsLE(frame.data, 2, 1) != 0;
    // OBD_Kaltstart_Denominator m3: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_5_480_obd_kaltstart_denominator_m3 = readBitsLE(frame.data, 3, 1) != 0;
    // OBD_Minimum_Trip m3: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_5_480_obd_minimum_trip_m3 = readBitsLE(frame.data, 4, 1) != 0;
    // MO5_DPF_reg m3: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_5_480_mo5_dpf_reg_m3 = readBitsLE(frame.data, 5, 1) != 0;
    // MO5_Vorgluehen: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Diesel pre-glow lamp
    // Value table: 0 "Lamp_off" 1 "Lamp_on"
    params::vw_pq.motor_5_480_mo5_vorgluehen = readBitsLE(frame.data, 9, 1) != 0;
    // MO5_E_Gas: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: E-Gas system lamp (petrol only)
    // Value table: 0 "Lamp_off" 1 "Lamp_on"
    params::vw_pq.motor_5_480_mo5_e_gas = readBitsLE(frame.data, 10, 1) != 0;
    // MO5_OBD_2: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: OBD/MIL lamp control
    // Value table: 0 "Lamp_off" 1 "Lamp_on"
    params::vw_pq.motor_5_480_mo5_obd_2 = readBitsLE(frame.data, 11, 1) != 0;
    // MO5_Heissl: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Coolant overheat warning lamp
    // Value table: 0 "Lamp_off" 1 "Lamp_on"
    params::vw_pq.motor_5_480_mo5_heissl = readBitsLE(frame.data, 12, 1) != 0;
    // MO5_KlimaKompr: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: AC compressor off request
    // Value table: 0 "no_Anforderung" 1 "Klimakompr_aus"
    params::vw_pq.motor_5_480_mo5_klimakompr = readBitsLE(frame.data, 13, 1) != 0;
    // MO5_Feld_kuehl: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Map cooling installed and OK
    // Value table: 0 "no" 1 "yes"
    params::vw_pq.motor_5_480_mo5_feld_kuehl = readBitsLE(frame.data, 14, 1) != 0;
    // MO5_KliKo_Red: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: AC compressor power reduction
    // Value table: 0 "no" 1 "yes"
    params::vw_pq.motor_5_480_mo5_kliko_red = readBitsLE(frame.data, 15, 1) != 0;
    // MO5_Verbrauch: start=16 len=15 endian=1 sign=+ factor=1 offset=0 unit="l"
    // DBC comment: Fuel consumption counter (15-bit, l)
    const uint32_t raw_motor_5_480_mo5_verbrauch = readBitsLE(frame.data, 16, 15);
    params::vw_pq.motor_5_480_mo5_verbrauch = raw_motor_5_480_mo5_verbrauch * 1.0f + 0.0f;
    // MO5_UeberlVerb: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Consumption counter overflow
    // Value table: 0 "no_Ueberlauf" 1 "mindestens_einmal_uebergelaufen"
    params::vw_pq.motor_5_480_mo5_ueberlverb = readBitsLE(frame.data, 31, 1) != 0;
    // MO5_Luefter: start=32 len=8 endian=1 sign=+ factor=0.4 offset=0 unit="%"
    // DBC comment: Cooling fan PWM control (0-101.6%)
    const uint32_t raw_motor_5_480_mo5_luefter = readBitsLE(frame.data, 32, 8);
    params::vw_pq.motor_5_480_mo5_luefter = raw_motor_5_480_mo5_luefter * 0.4f + 0.0f;
    // MO5_HLeuchte: start=40 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Overheat pre-warning
    // Value table: 0 "no_Warnung" 1 "Vorwarnung"
    params::vw_pq.motor_5_480_mo5_hleuchte = readBitsLE(frame.data, 40, 1) != 0;
    // MO5_PartikelLamp: start=41 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Particle filter lamp
    // Value table: 0 "Lamp_off" 1 "Lamp_on"
    params::vw_pq.motor_5_480_mo5_partikellamp = readBitsLE(frame.data, 41, 1) != 0;
    // MO5_Sta_BKU: start=42 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Brake vacuum pump status (Porsche only)
    // Value table: 0 "iO_oder_not_verbaut" 1 "n_iO"
    params::vw_pq.motor_5_480_mo5_sta_bku = readBitsLE(frame.data, 42, 1) != 0;
    // MO5_TypStartSteu: start=43 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Starter control type: 0=BSG/BCM, 1=MSG
    // Value table: 0 "Startersteuerung_BSG_BCM_KessyD1_ZAS" 1 "Startersteuerung_durch_MSG"
    params::vw_pq.motor_5_480_mo5_typstartsteu = readBitsLE(frame.data, 43, 1) != 0;
    // MO5_TDE_Lampe: start=44 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Fuel cap warning lamp (petrol only)
    // Value table: 0 "Lamp_off" 1 "Lamp_on"
    params::vw_pq.motor_5_480_mo5_tde_lampe = readBitsLE(frame.data, 44, 1) != 0;
    // MO5_TDE_Text: start=45 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Fuel cap text display (petrol only)
    // Value table: 0 "no_Text" 1 "Textanzeige_nach_ISO"
    params::vw_pq.motor_5_480_mo5_tde_text = readBitsLE(frame.data, 45, 1) != 0;
    // MO5_DZM_Daempf: start=46 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: RPM gauge damping control
    // Value table: 0 "normale_Daempfung" 1 "dynamische_Daempfung"
    params::vw_pq.motor_5_480_mo5_dzm_daempf = readBitsLE(frame.data, 46, 1) != 0;
    // MO5_Interlock: start=47 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Interlock switch for manual transmission start
    // Value table: 0 "Interlock_not_betaetigt" 1 "Interlock_betaetigt"
    params::vw_pq.motor_5_480_mo5_interlock = readBitsLE(frame.data, 47, 1) != 0;
    // MO5_Start: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Automatic start permission
    // Value table: 0 "Start_not_zulaessig" 1 "Startfreigabe"
    params::vw_pq.motor_5_480_mo5_start = readBitsLE(frame.data, 48, 1) != 0;
    // MO5_Anlasser: start=49 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Starter control permission
    // Value table: 0 "Anlasser_darf_angesteuert_werden" 1 "Anlasser_ausspuren_Ansteuerung_not_moeglich"
    params::vw_pq.motor_5_480_mo5_anlasser = readBitsLE(frame.data, 49, 1) != 0;
    // MO5_GRA_Hauptsch: start=50 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: GRA main switch / readiness
    // Value table: 0 "Off" 1 "On"
    params::vw_pq.motor_5_480_mo5_gra_hauptsch = readBitsLE(frame.data, 50, 1) != 0;
    // MO5_Momente: start=51 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: MDI torque values doubled
    // Value table: 0 "Einfach" 1 "Doppelt"
    params::vw_pq.motor_5_480_mo5_momente = readBitsLE(frame.data, 51, 1) != 0;
    // MO5_Motortext1: start=52 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Text: Engine workshop
    // Value table: 0 "no_Text" 1 "Text_1"
    params::vw_pq.motor_5_480_mo5_motortext1 = readBitsLE(frame.data, 52, 1) != 0;
    // MO5_Motortext2: start=53 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Text: Exhaust workshop (deprecated)
    // Value table: 0 "no_Text" 1 "Text_2"
    params::vw_pq.motor_5_480_mo5_motortext2 = readBitsLE(frame.data, 53, 1) != 0;
    // MO5_Motortext3: start=54 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Text: Fuel system fault
    // Value table: 0 "no_Text" 1 "Text_3"
    params::vw_pq.motor_5_480_mo5_motortext3 = readBitsLE(frame.data, 54, 1) != 0;
    // MO5_Motortext4: start=55 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Text: Particle filter needs regeneration
    // Value table: 0 "no_Text" 1 "Text_4"
    params::vw_pq.motor_5_480_mo5_motortext4 = readBitsLE(frame.data, 55, 1) != 0;
    // CHECKSUM: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Checksum with final value 0x0
    const uint32_t raw_motor_5_480_checksum = readBitsLE(frame.data, 56, 8);
    params::vw_pq.motor_5_480_checksum = raw_motor_5_480_checksum * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x480(CANMessage &frame) {
    frame.id = 0x480;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // MO5_Mp_Code M
    int64_t raw_tmp_motor_5_480_mo5_mp_code_m = (int64_t)llround((params::vw_pq.motor_5_480_mo5_mp_code_m - 0.0f) / 1.0f);
    uint64_t raw_motor_5_480_mo5_mp_code_m = raw_tmp_motor_5_480_mo5_mp_code_m < 0 ? 0ULL : (uint64_t)raw_tmp_motor_5_480_mo5_mp_code_m;
    if (raw_motor_5_480_mo5_mp_code_m > 0x3ULL) raw_motor_5_480_mo5_mp_code_m = 0x3ULL;
    writeBitsLE(frame.data, 6, 2, (uint32_t)raw_motor_5_480_mo5_mp_code_m);
    // MO5_max_Moment m0
    int64_t raw_tmp_motor_5_480_mo5_max_moment_m0 = (int64_t)llround((params::vw_pq.motor_5_480_mo5_max_moment_m0 - 0.0f) / 10.0f);
    uint64_t raw_motor_5_480_mo5_max_moment_m0 = raw_tmp_motor_5_480_mo5_max_moment_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_5_480_mo5_max_moment_m0;
    if (raw_motor_5_480_mo5_max_moment_m0 > 0x3FULL) raw_motor_5_480_mo5_max_moment_m0 = 0x3FULL;
    writeBitsLE(frame.data, 0, 6, (uint32_t)raw_motor_5_480_mo5_max_moment_m0);
    // MO5_Drehzahl m1
    int64_t raw_tmp_motor_5_480_mo5_drehzahl_m1 = (int64_t)llround((params::vw_pq.motor_5_480_mo5_drehzahl_m1 - 0.0f) / 100.0f);
    uint64_t raw_motor_5_480_mo5_drehzahl_m1 = raw_tmp_motor_5_480_mo5_drehzahl_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_5_480_mo5_drehzahl_m1;
    if (raw_motor_5_480_mo5_drehzahl_m1 > 0x3FULL) raw_motor_5_480_mo5_drehzahl_m1 = 0x3FULL;
    writeBitsLE(frame.data, 0, 6, (uint32_t)raw_motor_5_480_mo5_drehzahl_m1);
    // MO5_Motortyp m2
    int64_t raw_tmp_motor_5_480_mo5_motortyp_m2 = (int64_t)llround((params::vw_pq.motor_5_480_mo5_motortyp_m2 - 0.0f) / 1.0f);
    uint64_t raw_motor_5_480_mo5_motortyp_m2 = raw_tmp_motor_5_480_mo5_motortyp_m2 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_5_480_mo5_motortyp_m2;
    if (raw_motor_5_480_mo5_motortyp_m2 > 0x3FULL) raw_motor_5_480_mo5_motortyp_m2 = 0x3FULL;
    writeBitsLE(frame.data, 0, 6, (uint32_t)raw_motor_5_480_mo5_motortyp_m2);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.motor_5_480_mo5_abgastyp_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.motor_5_480_mo5_abgastyp2_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.motor_5_480_mo5_abgastyp3_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.motor_5_480_obd_kaltstart_denominator_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.motor_5_480_obd_minimum_trip_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.motor_5_480_mo5_dpf_reg_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.motor_5_480_mo5_vorgluehen ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.motor_5_480_mo5_e_gas ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.motor_5_480_mo5_obd_2 ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.motor_5_480_mo5_heissl ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.motor_5_480_mo5_klimakompr ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.motor_5_480_mo5_feld_kuehl ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.motor_5_480_mo5_kliko_red ? 1U : 0U);
    // MO5_Verbrauch
    int64_t raw_tmp_motor_5_480_mo5_verbrauch = (int64_t)llround((params::vw_pq.motor_5_480_mo5_verbrauch - 0.0f) / 1.0f);
    uint64_t raw_motor_5_480_mo5_verbrauch = raw_tmp_motor_5_480_mo5_verbrauch < 0 ? 0ULL : (uint64_t)raw_tmp_motor_5_480_mo5_verbrauch;
    if (raw_motor_5_480_mo5_verbrauch > 0x7FFFULL) raw_motor_5_480_mo5_verbrauch = 0x7FFFULL;
    writeBitsLE(frame.data, 16, 15, (uint32_t)raw_motor_5_480_mo5_verbrauch);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.motor_5_480_mo5_ueberlverb ? 1U : 0U);
    // MO5_Luefter
    int64_t raw_tmp_motor_5_480_mo5_luefter = (int64_t)llround((params::vw_pq.motor_5_480_mo5_luefter - 0.0f) / 0.4f);
    uint64_t raw_motor_5_480_mo5_luefter = raw_tmp_motor_5_480_mo5_luefter < 0 ? 0ULL : (uint64_t)raw_tmp_motor_5_480_mo5_luefter;
    if (raw_motor_5_480_mo5_luefter > 0xFFULL) raw_motor_5_480_mo5_luefter = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_motor_5_480_mo5_luefter);
    writeBitsLE(frame.data, 40, 1, params::vw_pq.motor_5_480_mo5_hleuchte ? 1U : 0U);
    writeBitsLE(frame.data, 41, 1, params::vw_pq.motor_5_480_mo5_partikellamp ? 1U : 0U);
    writeBitsLE(frame.data, 42, 1, params::vw_pq.motor_5_480_mo5_sta_bku ? 1U : 0U);
    writeBitsLE(frame.data, 43, 1, params::vw_pq.motor_5_480_mo5_typstartsteu ? 1U : 0U);
    writeBitsLE(frame.data, 44, 1, params::vw_pq.motor_5_480_mo5_tde_lampe ? 1U : 0U);
    writeBitsLE(frame.data, 45, 1, params::vw_pq.motor_5_480_mo5_tde_text ? 1U : 0U);
    writeBitsLE(frame.data, 46, 1, params::vw_pq.motor_5_480_mo5_dzm_daempf ? 1U : 0U);
    writeBitsLE(frame.data, 47, 1, params::vw_pq.motor_5_480_mo5_interlock ? 1U : 0U);
    writeBitsLE(frame.data, 48, 1, params::vw_pq.motor_5_480_mo5_start ? 1U : 0U);
    writeBitsLE(frame.data, 49, 1, params::vw_pq.motor_5_480_mo5_anlasser ? 1U : 0U);
    writeBitsLE(frame.data, 50, 1, params::vw_pq.motor_5_480_mo5_gra_hauptsch ? 1U : 0U);
    writeBitsLE(frame.data, 51, 1, params::vw_pq.motor_5_480_mo5_momente ? 1U : 0U);
    writeBitsLE(frame.data, 52, 1, params::vw_pq.motor_5_480_mo5_motortext1 ? 1U : 0U);
    writeBitsLE(frame.data, 53, 1, params::vw_pq.motor_5_480_mo5_motortext2 ? 1U : 0U);
    writeBitsLE(frame.data, 54, 1, params::vw_pq.motor_5_480_mo5_motortext3 ? 1U : 0U);
    writeBitsLE(frame.data, 55, 1, params::vw_pq.motor_5_480_mo5_motortext4 ? 1U : 0U);
    // CHECKSUM
    int64_t raw_tmp_motor_5_480_checksum = (int64_t)llround((params::vw_pq.motor_5_480_checksum - 0.0f) / 1.0f);
    uint64_t raw_motor_5_480_checksum = raw_tmp_motor_5_480_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_motor_5_480_checksum;
    if (raw_motor_5_480_checksum > 0xFFULL) raw_motor_5_480_checksum = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_motor_5_480_checksum);
}

// Message 0x380 (896) - Motor_3
// DLC: 8, TX: Motor
// Comment: Motor message 3 - Contains accelerator pedal, temperatures, and control signals
bool decodeVwPq0x380(const CANMessage &frame) {
    if (frame.id != 0x380 || frame.len < 8) {
        return false;
    }

    // MO3_Vorgluehen: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Pre-glow active (diesel engines)
    // Value table: 0 "no_Vorgluehen" 1 "Vorgluehen_active"
    params::vw_pq.motor_3_380_mo3_vorgluehen = readBitsLE(frame.data, 0, 1) != 0;
    // MO3_Prio_Dz: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Engine speed priority: 0=request, 1=mandatory
    // Value table: 0 "Wunsch" 1 "Zwang"
    params::vw_pq.motor_3_380_mo3_prio_dz = readBitsLE(frame.data, 1, 1) != 0;
    // MO_Schalter_StartStopp: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Start/stop deactivated by driver (LED on when 0)
    // Value table: 0 "StartStopp_ueber_Hauptschalter_deactivated_LED_an" 1 "StartStopp_ueber_Hauptschalter_activated_LED_aus"
    params::vw_pq.motor_3_380_mo_schalter_startstopp = readBitsLE(frame.data, 2, 1) != 0;
    // MO3_Winterprg: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Winter driving program active
    // Value table: 0 "not_active" 1 "active"
    params::vw_pq.motor_3_380_mo3_winterprg = readBitsLE(frame.data, 3, 1) != 0;
    // MO3_Sta_Pedal: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Accelerator pedal status: 0=OK, 1=substitute value
    // Value table: 0 "Fahrpedal_iO" 1 "Ersatzwert"
    params::vw_pq.motor_3_380_mo3_sta_pedal = readBitsLE(frame.data, 4, 1) != 0;
    // MO3_Sta_FPG M: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_3_380_mo3_sta_fpg_m = readBitsLE(frame.data, 5, 1) != 0;
    // MO3_Sign_FPG m1: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_3_380_mo3_sign_fpg_m1 = readBitsLE(frame.data, 6, 1) != 0;
    // MO3_Sta_Temp: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Intake air temperature status: 0=OK, 1=substitute value
    // Value table: 0 "Geber_iO" 1 "Ersatzwert"
    params::vw_pq.motor_3_380_mo3_sta_temp = readBitsLE(frame.data, 7, 1) != 0;
    // MO3_Offsentemp: start=8 len=8 endian=1 sign=+ factor=0.75 offset=-48 unit="C"
    // DBC comment: Intake air temperature
    const uint32_t raw_motor_3_380_mo3_offsentemp = readBitsLE(frame.data, 8, 8);
    params::vw_pq.motor_3_380_mo3_offsentemp = raw_motor_3_380_mo3_offsentemp * 0.75f + -48.0f;
    // MO3_Pedalwert: start=16 len=8 endian=1 sign=+ factor=0.4 offset=0 unit="%"
    // DBC comment: Accelerator pedal raw signal (0-101.6%)
    const uint32_t raw_motor_3_380_mo3_pedalwert = readBitsLE(frame.data, 16, 8);
    params::vw_pq.motor_3_380_mo3_pedalwert = raw_motor_3_380_mo3_pedalwert * 0.4f + 0.0f;
    // MO3_Rad_Wu_Mo: start=24 len=12 endian=1 sign=+ factor=0.39 offset=0 unit="%"
    // DBC comment: Wheel torque request (12-bit value, 0-1596.66% MDI)
    const uint32_t raw_motor_3_380_mo3_rad_wu_mo = readBitsLE(frame.data, 24, 12);
    params::vw_pq.motor_3_380_mo3_rad_wu_mo = raw_motor_3_380_mo3_rad_wu_mo * 0.39f + 0.0f;
    // MO3_Vorz_RadWu: start=36 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Wheel torque sign: 0=positive, 1=negative
    // Value table: 0 "positives_Vorzeichen" 1 "negatives_Vorzeichen"
    params::vw_pq.motor_3_380_mo3_vorz_radwu = readBitsLE(frame.data, 36, 1) != 0;
    // MO3_Freigabe_Segeln: start=37 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Sailing operation enable for transmission
    // Value table: 0 "Segelbetrieb_not_freigegeben" 1 "Segelbetrieb_freigegeben"
    params::vw_pq.motor_3_380_mo3_freigabe_segeln = readBitsLE(frame.data, 37, 1) != 0;
    // MO_StartStopp_StoppVorbereitung: start=38 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Engine stop in preparation
    // Value table: 0 "Motorstopp_not_in_Vorbereitung" 1 "Motorstopp_in_Vorbereitung"
    params::vw_pq.motor_3_380_mo_startstopp_stoppvorbereitung = readBitsLE(frame.data, 38, 1) != 0;
    // MO3_Dz_Beeinfl: start=40 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="%"
    // DBC comment: Engine speed influence factor (0-100% interpolation)
    const uint32_t raw_motor_3_380_mo3_dz_beeinfl = readBitsLE(frame.data, 40, 8);
    params::vw_pq.motor_3_380_mo3_dz_beeinfl = raw_motor_3_380_mo3_dz_beeinfl * 0.39f + 0.0f;
    // MO3_WunschDz: start=48 len=8 endian=1 sign=+ factor=25 offset=0 unit="U/min"
    // DBC comment: Desired engine speed (when MO3_Prio_Dz=1)
    const uint32_t raw_motor_3_380_mo3_wunschdz = readBitsLE(frame.data, 48, 8);
    params::vw_pq.motor_3_380_mo3_wunschdz = raw_motor_3_380_mo3_wunschdz * 25.0f + 0.0f;
    // MO3_DKW m0: start=56 len=8 endian=1 sign=+ factor=0.4 offset=0 unit="%"
    const uint32_t raw_motor_3_380_mo3_dkw_m0 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.motor_3_380_mo3_dkw_m0 = raw_motor_3_380_mo3_dkw_m0 * 0.4f + 0.0f;
    // MO3_FPGradient m1: start=56 len=8 endian=1 sign=+ factor=25 offset=0 unit="%/s"
    const uint32_t raw_motor_3_380_mo3_fpgradient_m1 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.motor_3_380_mo3_fpgradient_m1 = raw_motor_3_380_mo3_fpgradient_m1 * 25.0f + 0.0f;
    return true;
}

void encodeVwPq0x380(CANMessage &frame) {
    frame.id = 0x380;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 0, 1, params::vw_pq.motor_3_380_mo3_vorgluehen ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.motor_3_380_mo3_prio_dz ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.motor_3_380_mo_schalter_startstopp ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.motor_3_380_mo3_winterprg ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.motor_3_380_mo3_sta_pedal ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.motor_3_380_mo3_sta_fpg_m ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.motor_3_380_mo3_sign_fpg_m1 ? 1U : 0U);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.motor_3_380_mo3_sta_temp ? 1U : 0U);
    // MO3_Offsentemp
    int64_t raw_tmp_motor_3_380_mo3_offsentemp = (int64_t)llround((params::vw_pq.motor_3_380_mo3_offsentemp - -48.0f) / 0.75f);
    uint64_t raw_motor_3_380_mo3_offsentemp = raw_tmp_motor_3_380_mo3_offsentemp < 0 ? 0ULL : (uint64_t)raw_tmp_motor_3_380_mo3_offsentemp;
    if (raw_motor_3_380_mo3_offsentemp > 0xFFULL) raw_motor_3_380_mo3_offsentemp = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_motor_3_380_mo3_offsentemp);
    // MO3_Pedalwert
    int64_t raw_tmp_motor_3_380_mo3_pedalwert = (int64_t)llround((params::vw_pq.motor_3_380_mo3_pedalwert - 0.0f) / 0.4f);
    uint64_t raw_motor_3_380_mo3_pedalwert = raw_tmp_motor_3_380_mo3_pedalwert < 0 ? 0ULL : (uint64_t)raw_tmp_motor_3_380_mo3_pedalwert;
    if (raw_motor_3_380_mo3_pedalwert > 0xFFULL) raw_motor_3_380_mo3_pedalwert = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_motor_3_380_mo3_pedalwert);
    // MO3_Rad_Wu_Mo
    int64_t raw_tmp_motor_3_380_mo3_rad_wu_mo = (int64_t)llround((params::vw_pq.motor_3_380_mo3_rad_wu_mo - 0.0f) / 0.39f);
    uint64_t raw_motor_3_380_mo3_rad_wu_mo = raw_tmp_motor_3_380_mo3_rad_wu_mo < 0 ? 0ULL : (uint64_t)raw_tmp_motor_3_380_mo3_rad_wu_mo;
    if (raw_motor_3_380_mo3_rad_wu_mo > 0xFFFULL) raw_motor_3_380_mo3_rad_wu_mo = 0xFFFULL;
    writeBitsLE(frame.data, 24, 12, (uint32_t)raw_motor_3_380_mo3_rad_wu_mo);
    writeBitsLE(frame.data, 36, 1, params::vw_pq.motor_3_380_mo3_vorz_radwu ? 1U : 0U);
    writeBitsLE(frame.data, 37, 1, params::vw_pq.motor_3_380_mo3_freigabe_segeln ? 1U : 0U);
    writeBitsLE(frame.data, 38, 1, params::vw_pq.motor_3_380_mo_startstopp_stoppvorbereitung ? 1U : 0U);
    // MO3_Dz_Beeinfl
    int64_t raw_tmp_motor_3_380_mo3_dz_beeinfl = (int64_t)llround((params::vw_pq.motor_3_380_mo3_dz_beeinfl - 0.0f) / 0.39f);
    uint64_t raw_motor_3_380_mo3_dz_beeinfl = raw_tmp_motor_3_380_mo3_dz_beeinfl < 0 ? 0ULL : (uint64_t)raw_tmp_motor_3_380_mo3_dz_beeinfl;
    if (raw_motor_3_380_mo3_dz_beeinfl > 0xFFULL) raw_motor_3_380_mo3_dz_beeinfl = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_motor_3_380_mo3_dz_beeinfl);
    // MO3_WunschDz
    int64_t raw_tmp_motor_3_380_mo3_wunschdz = (int64_t)llround((params::vw_pq.motor_3_380_mo3_wunschdz - 0.0f) / 25.0f);
    uint64_t raw_motor_3_380_mo3_wunschdz = raw_tmp_motor_3_380_mo3_wunschdz < 0 ? 0ULL : (uint64_t)raw_tmp_motor_3_380_mo3_wunschdz;
    if (raw_motor_3_380_mo3_wunschdz > 0xFFULL) raw_motor_3_380_mo3_wunschdz = 0xFFULL;
    writeBitsLE(frame.data, 48, 8, (uint32_t)raw_motor_3_380_mo3_wunschdz);
    // MO3_DKW m0
    int64_t raw_tmp_motor_3_380_mo3_dkw_m0 = (int64_t)llround((params::vw_pq.motor_3_380_mo3_dkw_m0 - 0.0f) / 0.4f);
    uint64_t raw_motor_3_380_mo3_dkw_m0 = raw_tmp_motor_3_380_mo3_dkw_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_3_380_mo3_dkw_m0;
    if (raw_motor_3_380_mo3_dkw_m0 > 0xFFULL) raw_motor_3_380_mo3_dkw_m0 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_motor_3_380_mo3_dkw_m0);
    // MO3_FPGradient m1
    int64_t raw_tmp_motor_3_380_mo3_fpgradient_m1 = (int64_t)llround((params::vw_pq.motor_3_380_mo3_fpgradient_m1 - 0.0f) / 25.0f);
    uint64_t raw_motor_3_380_mo3_fpgradient_m1 = raw_tmp_motor_3_380_mo3_fpgradient_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_3_380_mo3_fpgradient_m1;
    if (raw_motor_3_380_mo3_fpgradient_m1 > 0xFFULL) raw_motor_3_380_mo3_fpgradient_m1 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_motor_3_380_mo3_fpgradient_m1);
}

// Message 0x288 (648) - Motor_2
// DLC: 8, TX: Motor
// Comment: Motor message 2 - Contains engine status, temperatures, and torque information
bool decodeVwPq0x288(const CANMessage &frame) {
    if (frame.id != 0x288 || frame.len < 8) {
        return false;
    }

    // MO2_Mp_Code m: start=6 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_2_288_mo2_mp_code_m = readBitsLE(frame.data, 6, 2);
    params::vw_pq.motor_2_288_mo2_mp_code_m = raw_motor_2_288_mo2_mp_code_m * 1.0f + 0.0f;
    // MO2_Getr_Code m2: start=0 len=6 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_2_288_mo2_getr_code_m2 = readBitsLE(frame.data, 0, 6);
    params::vw_pq.motor_2_288_mo2_getr_code_m2 = raw_motor_2_288_mo2_getr_code_m2 * 1.0f + 0.0f;
    // MO2_max_Mo m3: start=0 len=6 endian=1 sign=+ factor=10 offset=0 unit="Nm"
    const uint32_t raw_motor_2_288_mo2_max_mo_m3 = readBitsLE(frame.data, 0, 6);
    params::vw_pq.motor_2_288_mo2_max_mo_m3 = raw_motor_2_288_mo2_max_mo_m3 * 10.0f + 0.0f;
    // MO2_CAN_Vers m0: start=0 len=6 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_2_288_mo2_can_vers_m0 = readBitsLE(frame.data, 0, 6);
    params::vw_pq.motor_2_288_mo2_can_vers_m0 = raw_motor_2_288_mo2_can_vers_m0 * 1.0f + 0.0f;
    // MO2_Motor_Code m1: start=0 len=6 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_motor_2_288_mo2_motor_code_m1 = readBitsLE(frame.data, 0, 6);
    params::vw_pq.motor_2_288_mo2_motor_code_m1 = raw_motor_2_288_mo2_motor_code_m1 * 1.0f + 0.0f;
    // MO2_Kuehlm_T: start=8 len=8 endian=1 sign=+ factor=0.75 offset=-48 unit="C"
    // DBC comment: Engine coolant temperature or substitute value
    const uint32_t raw_motor_2_288_mo2_kuehlm_t = readBitsLE(frame.data, 8, 8);
    params::vw_pq.motor_2_288_mo2_kuehlm_t = raw_motor_2_288_mo2_kuehlm_t * 0.75f + -48.0f;
    // MO2_BLS: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Brake light switch - unfiltered raw signal
    // Value table: 0 "no_Bremsen" 1 "Bremse_betaetigt"
    params::vw_pq.motor_2_288_mo2_bls = readBitsLE(frame.data, 16, 1) != 0;
    // MO2_BTS: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Brake test switch
    // Value table: 0 "no_braking" 1 "Brake_activated"
    params::vw_pq.motor_2_288_mo2_bts = readBitsLE(frame.data, 17, 1) != 0;
    // MO2_Sta_Kuehlm: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Coolant temperature status - 0=OK, 1=Not OK
    // Value table: 0 "Temperature_ok" 1 "Temperature_not_ok"
    params::vw_pq.motor_2_288_mo2_sta_kuehlm = readBitsLE(frame.data, 18, 1) != 0;
    // MO2_Sta_Klima: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Air conditioning compressor status
    // Value table: 0 "Climate_off" 1 "Climate_on"
    params::vw_pq.motor_2_288_mo2_sta_klima = readBitsLE(frame.data, 19, 1) != 0;
    // MO2_Sta_No_Bet: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Normal operation status (Terminal 15 on, init complete)
    // Value table: 0 "no_Normalbetrieb" 1 "Normalbetrieb"
    params::vw_pq.motor_2_288_mo2_sta_no_bet = readBitsLE(frame.data, 20, 1) != 0;
    // MO2_Status_TSK: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Drive train coordinator status
    // Value table: 0 "TSK_not_available" 1 "TSK_available"
    params::vw_pq.motor_2_288_mo2_status_tsk = readBitsLE(frame.data, 21, 1) != 0;
    // MO2_Sta_GRA: start=22 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: GRA/ACC status
    // Value table: 0 "ADR_GRA_off" 1 "gra_activated_lamp_on" 2 "gra_overdriven_lamp_on" 3 "ADR_gra_faulted"
    const uint32_t raw_motor_2_288_mo2_sta_gra = readBitsLE(frame.data, 22, 2);
    params::vw_pq.motor_2_288_mo2_sta_gra = raw_motor_2_288_mo2_sta_gra * 1.0f + 0.0f;
    // TSK_Limiter_ausgewaehlt: start=25 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Speed limiter selected
    // Value table: 0 "no_limiter_selected" 1 "Limiter_Selected"
    params::vw_pq.motor_2_288_tsk_limiter_ausgewaehlt = readBitsLE(frame.data, 25, 1) != 0;
    // MO2_GRAregelt: start=26 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: GRA regulating to displayed speed
    // Value table: 0 "no" 1 "yes"
    params::vw_pq.motor_2_288_mo2_graregelt = readBitsLE(frame.data, 26, 1) != 0;
    // MO2_Sport_Error: start=27 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Sport mode error (Porsche Cayenne)
    // Value table: 0 "no_Failure" 1 "Failure"
    params::vw_pq.motor_2_288_mo2_sport_error = readBitsLE(frame.data, 27, 1) != 0;
    // MO2_OffRoad: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: OffRoad mode active
    // Value table: 0 "not_active" 1 "active"
    params::vw_pq.motor_2_288_mo2_offroad = readBitsLE(frame.data, 28, 1) != 0;
    // MO2_RME_Gehalt: start=29 len=3 endian=1 sign=+ factor=12.5 offset=0 unit="%"
    // DBC comment: Rapeseed oil methyl ester content (diesel)
    const uint32_t raw_motor_2_288_mo2_rme_gehalt = readBitsLE(frame.data, 29, 3);
    params::vw_pq.motor_2_288_mo2_rme_gehalt = raw_motor_2_288_mo2_rme_gehalt * 12.5f + 0.0f;
    // MO2_GRA_Soll: start=32 len=8 endian=1 sign=+ factor=1.28 offset=0 unit="km/h"
    // DBC comment: GRA target speed setpoint
    const uint32_t raw_motor_2_288_mo2_gra_soll = readBitsLE(frame.data, 32, 8);
    params::vw_pq.motor_2_288_mo2_gra_soll = raw_motor_2_288_mo2_gra_soll * 1.28f + 0.0f;
    // MO2_LL_Solldz: start=40 len=8 endian=1 sign=+ factor=10 offset=0 unit="1/min"
    // DBC comment: Idle target speed
    const uint32_t raw_motor_2_288_mo2_ll_solldz = readBitsLE(frame.data, 40, 8);
    params::vw_pq.motor_2_288_mo2_ll_solldz = raw_motor_2_288_mo2_ll_solldz * 10.0f + 0.0f;
    // MO2_Begr_Mo: start=48 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="%"
    // DBC comment: Limiting torque - max possible at speed
    const uint32_t raw_motor_2_288_mo2_begr_mo = readBitsLE(frame.data, 48, 8);
    params::vw_pq.motor_2_288_mo2_begr_mo = raw_motor_2_288_mo2_begr_mo * 0.39f + 0.0f;
    // MO2_Mo_ZWR: start=56 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="%"
    // DBC comment: Minimum engine torque with ignition angle retardation
    const uint32_t raw_motor_2_288_mo2_mo_zwr = readBitsLE(frame.data, 56, 8);
    params::vw_pq.motor_2_288_mo2_mo_zwr = raw_motor_2_288_mo2_mo_zwr * 0.39f + 0.0f;
    return true;
}

void encodeVwPq0x288(CANMessage &frame) {
    frame.id = 0x288;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // MO2_Mp_Code m
    int64_t raw_tmp_motor_2_288_mo2_mp_code_m = (int64_t)llround((params::vw_pq.motor_2_288_mo2_mp_code_m - 0.0f) / 1.0f);
    uint64_t raw_motor_2_288_mo2_mp_code_m = raw_tmp_motor_2_288_mo2_mp_code_m < 0 ? 0ULL : (uint64_t)raw_tmp_motor_2_288_mo2_mp_code_m;
    if (raw_motor_2_288_mo2_mp_code_m > 0x3ULL) raw_motor_2_288_mo2_mp_code_m = 0x3ULL;
    writeBitsLE(frame.data, 6, 2, (uint32_t)raw_motor_2_288_mo2_mp_code_m);
    // MO2_Getr_Code m2
    int64_t raw_tmp_motor_2_288_mo2_getr_code_m2 = (int64_t)llround((params::vw_pq.motor_2_288_mo2_getr_code_m2 - 0.0f) / 1.0f);
    uint64_t raw_motor_2_288_mo2_getr_code_m2 = raw_tmp_motor_2_288_mo2_getr_code_m2 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_2_288_mo2_getr_code_m2;
    if (raw_motor_2_288_mo2_getr_code_m2 > 0x3FULL) raw_motor_2_288_mo2_getr_code_m2 = 0x3FULL;
    writeBitsLE(frame.data, 0, 6, (uint32_t)raw_motor_2_288_mo2_getr_code_m2);
    // MO2_max_Mo m3
    int64_t raw_tmp_motor_2_288_mo2_max_mo_m3 = (int64_t)llround((params::vw_pq.motor_2_288_mo2_max_mo_m3 - 0.0f) / 10.0f);
    uint64_t raw_motor_2_288_mo2_max_mo_m3 = raw_tmp_motor_2_288_mo2_max_mo_m3 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_2_288_mo2_max_mo_m3;
    if (raw_motor_2_288_mo2_max_mo_m3 > 0x3FULL) raw_motor_2_288_mo2_max_mo_m3 = 0x3FULL;
    writeBitsLE(frame.data, 0, 6, (uint32_t)raw_motor_2_288_mo2_max_mo_m3);
    // MO2_CAN_Vers m0
    int64_t raw_tmp_motor_2_288_mo2_can_vers_m0 = (int64_t)llround((params::vw_pq.motor_2_288_mo2_can_vers_m0 - 0.0f) / 1.0f);
    uint64_t raw_motor_2_288_mo2_can_vers_m0 = raw_tmp_motor_2_288_mo2_can_vers_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_2_288_mo2_can_vers_m0;
    if (raw_motor_2_288_mo2_can_vers_m0 > 0x3FULL) raw_motor_2_288_mo2_can_vers_m0 = 0x3FULL;
    writeBitsLE(frame.data, 0, 6, (uint32_t)raw_motor_2_288_mo2_can_vers_m0);
    // MO2_Motor_Code m1
    int64_t raw_tmp_motor_2_288_mo2_motor_code_m1 = (int64_t)llround((params::vw_pq.motor_2_288_mo2_motor_code_m1 - 0.0f) / 1.0f);
    uint64_t raw_motor_2_288_mo2_motor_code_m1 = raw_tmp_motor_2_288_mo2_motor_code_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_2_288_mo2_motor_code_m1;
    if (raw_motor_2_288_mo2_motor_code_m1 > 0x3FULL) raw_motor_2_288_mo2_motor_code_m1 = 0x3FULL;
    writeBitsLE(frame.data, 0, 6, (uint32_t)raw_motor_2_288_mo2_motor_code_m1);
    // MO2_Kuehlm_T
    int64_t raw_tmp_motor_2_288_mo2_kuehlm_t = (int64_t)llround((params::vw_pq.motor_2_288_mo2_kuehlm_t - -48.0f) / 0.75f);
    uint64_t raw_motor_2_288_mo2_kuehlm_t = raw_tmp_motor_2_288_mo2_kuehlm_t < 0 ? 0ULL : (uint64_t)raw_tmp_motor_2_288_mo2_kuehlm_t;
    if (raw_motor_2_288_mo2_kuehlm_t > 0xFFULL) raw_motor_2_288_mo2_kuehlm_t = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_motor_2_288_mo2_kuehlm_t);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.motor_2_288_mo2_bls ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.motor_2_288_mo2_bts ? 1U : 0U);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.motor_2_288_mo2_sta_kuehlm ? 1U : 0U);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.motor_2_288_mo2_sta_klima ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.motor_2_288_mo2_sta_no_bet ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.motor_2_288_mo2_status_tsk ? 1U : 0U);
    // MO2_Sta_GRA
    int64_t raw_tmp_motor_2_288_mo2_sta_gra = (int64_t)llround((params::vw_pq.motor_2_288_mo2_sta_gra - 0.0f) / 1.0f);
    uint64_t raw_motor_2_288_mo2_sta_gra = raw_tmp_motor_2_288_mo2_sta_gra < 0 ? 0ULL : (uint64_t)raw_tmp_motor_2_288_mo2_sta_gra;
    if (raw_motor_2_288_mo2_sta_gra > 0x3ULL) raw_motor_2_288_mo2_sta_gra = 0x3ULL;
    writeBitsLE(frame.data, 22, 2, (uint32_t)raw_motor_2_288_mo2_sta_gra);
    writeBitsLE(frame.data, 25, 1, params::vw_pq.motor_2_288_tsk_limiter_ausgewaehlt ? 1U : 0U);
    writeBitsLE(frame.data, 26, 1, params::vw_pq.motor_2_288_mo2_graregelt ? 1U : 0U);
    writeBitsLE(frame.data, 27, 1, params::vw_pq.motor_2_288_mo2_sport_error ? 1U : 0U);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.motor_2_288_mo2_offroad ? 1U : 0U);
    // MO2_RME_Gehalt
    int64_t raw_tmp_motor_2_288_mo2_rme_gehalt = (int64_t)llround((params::vw_pq.motor_2_288_mo2_rme_gehalt - 0.0f) / 12.5f);
    uint64_t raw_motor_2_288_mo2_rme_gehalt = raw_tmp_motor_2_288_mo2_rme_gehalt < 0 ? 0ULL : (uint64_t)raw_tmp_motor_2_288_mo2_rme_gehalt;
    if (raw_motor_2_288_mo2_rme_gehalt > 0x7ULL) raw_motor_2_288_mo2_rme_gehalt = 0x7ULL;
    writeBitsLE(frame.data, 29, 3, (uint32_t)raw_motor_2_288_mo2_rme_gehalt);
    // MO2_GRA_Soll
    int64_t raw_tmp_motor_2_288_mo2_gra_soll = (int64_t)llround((params::vw_pq.motor_2_288_mo2_gra_soll - 0.0f) / 1.28f);
    uint64_t raw_motor_2_288_mo2_gra_soll = raw_tmp_motor_2_288_mo2_gra_soll < 0 ? 0ULL : (uint64_t)raw_tmp_motor_2_288_mo2_gra_soll;
    if (raw_motor_2_288_mo2_gra_soll > 0xFFULL) raw_motor_2_288_mo2_gra_soll = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_motor_2_288_mo2_gra_soll);
    // MO2_LL_Solldz
    int64_t raw_tmp_motor_2_288_mo2_ll_solldz = (int64_t)llround((params::vw_pq.motor_2_288_mo2_ll_solldz - 0.0f) / 10.0f);
    uint64_t raw_motor_2_288_mo2_ll_solldz = raw_tmp_motor_2_288_mo2_ll_solldz < 0 ? 0ULL : (uint64_t)raw_tmp_motor_2_288_mo2_ll_solldz;
    if (raw_motor_2_288_mo2_ll_solldz > 0xFFULL) raw_motor_2_288_mo2_ll_solldz = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_motor_2_288_mo2_ll_solldz);
    // MO2_Begr_Mo
    int64_t raw_tmp_motor_2_288_mo2_begr_mo = (int64_t)llround((params::vw_pq.motor_2_288_mo2_begr_mo - 0.0f) / 0.39f);
    uint64_t raw_motor_2_288_mo2_begr_mo = raw_tmp_motor_2_288_mo2_begr_mo < 0 ? 0ULL : (uint64_t)raw_tmp_motor_2_288_mo2_begr_mo;
    if (raw_motor_2_288_mo2_begr_mo > 0xFFULL) raw_motor_2_288_mo2_begr_mo = 0xFFULL;
    writeBitsLE(frame.data, 48, 8, (uint32_t)raw_motor_2_288_mo2_begr_mo);
    // MO2_Mo_ZWR
    int64_t raw_tmp_motor_2_288_mo2_mo_zwr = (int64_t)llround((params::vw_pq.motor_2_288_mo2_mo_zwr - 0.0f) / 0.39f);
    uint64_t raw_motor_2_288_mo2_mo_zwr = raw_tmp_motor_2_288_mo2_mo_zwr < 0 ? 0ULL : (uint64_t)raw_tmp_motor_2_288_mo2_mo_zwr;
    if (raw_motor_2_288_mo2_mo_zwr > 0xFFULL) raw_motor_2_288_mo2_mo_zwr = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_motor_2_288_mo2_mo_zwr);
}

// Message 0x280 (640) - Motor_1
// DLC: 8, TX: XXX
bool decodeVwPq0x280(const CANMessage &frame) {
    if (frame.id != 0x280 || frame.len < 8) {
        return false;
    }

    // Fahrerwunschmoment: start=56 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="MDI"
    // DBC comment: Driver Requested Torque
    const uint32_t raw_motor_1_280_fahrerwunschmoment = readBitsLE(frame.data, 56, 8);
    params::vw_pq.motor_1_280_fahrerwunschmoment = raw_motor_1_280_fahrerwunschmoment * 0.39f + 0.0f;
    // mechanisches_Motor_Verlustmomen: start=48 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="MDI"
    // DBC comment: Mechanical Torque Loss
    const uint32_t raw_motor_1_280_mechanisches_motor_verlustmomen = readBitsLE(frame.data, 48, 8);
    params::vw_pq.motor_1_280_mechanisches_motor_verlustmomen = raw_motor_1_280_mechanisches_motor_verlustmomen * 0.39f + 0.0f;
    // Fahrpedalwert_oder_Drosselklapp: start=40 len=8 endian=1 sign=+ factor=0.4 offset=0 unit="%"
    // DBC comment: Accelerator Pedal or Throttle Position
    const uint32_t raw_motor_1_280_fahrpedalwert_oder_drosselklapp = readBitsLE(frame.data, 40, 8);
    params::vw_pq.motor_1_280_fahrpedalwert_oder_drosselklapp = raw_motor_1_280_fahrpedalwert_oder_drosselklapp * 0.4f + 0.0f;
    // inneres_Motor_Moment_ohne_exter: start=32 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="MDI"
    // DBC comment: Inner torque without external
    const uint32_t raw_motor_1_280_inneres_motor_moment_ohne_exter = readBitsLE(frame.data, 32, 8);
    params::vw_pq.motor_1_280_inneres_motor_moment_ohne_exter = raw_motor_1_280_inneres_motor_moment_ohne_exter * 0.39f + 0.0f;
    // Motordrehzahl: start=16 len=16 endian=1 sign=+ factor=0.25 offset=0 unit="U/min"
    // DBC comment: Engine Speed
    const uint32_t raw_motor_1_280_motordrehzahl = readBitsLE(frame.data, 16, 16);
    params::vw_pq.motor_1_280_motordrehzahl = raw_motor_1_280_motordrehzahl * 0.25f + 0.0f;
    // inneres_Motor_Moment: start=8 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="MDI"
    // DBC comment: Engine Indicated Torque
    const uint32_t raw_motor_1_280_inneres_motor_moment = readBitsLE(frame.data, 8, 8);
    params::vw_pq.motor_1_280_inneres_motor_moment = raw_motor_1_280_inneres_motor_moment * 0.39f + 0.0f;
    // Momentenangaben_ungenau: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Approximate Torque Values
    params::vw_pq.motor_1_280_momentenangaben_ungenau = readBitsLE(frame.data, 7, 1) != 0;
    // Fehlerstatus_Getriebe_Momentene: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_1_280_fehlerstatus_getriebe_momentene = readBitsLE(frame.data, 6, 1) != 0;
    // Fehlerstatus_Brems_Momenteneing: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_1_280_fehlerstatus_brems_momenteneing = readBitsLE(frame.data, 5, 1) != 0;
    // Time_Out_Bremsen_Botschaft: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_1_280_time_out_bremsen_botschaft = readBitsLE(frame.data, 4, 1) != 0;
    // Kupplungsschalter: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_1_280_kupplungsschalter = readBitsLE(frame.data, 3, 1) != 0;
    // Kickdownschalter: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_1_280_kickdownschalter = readBitsLE(frame.data, 2, 1) != 0;
    // Fahrpedalwert_ungenau__Motor_1_ M: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_1_280_fahrpedalwert_ungenau_motor_1_m = readBitsLE(frame.data, 1, 1) != 0;
    // Leergasinformation: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_1_280_leergasinformation = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x280(CANMessage &frame) {
    frame.id = 0x280;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Fahrerwunschmoment
    int64_t raw_tmp_motor_1_280_fahrerwunschmoment = (int64_t)llround((params::vw_pq.motor_1_280_fahrerwunschmoment - 0.0f) / 0.39f);
    uint64_t raw_motor_1_280_fahrerwunschmoment = raw_tmp_motor_1_280_fahrerwunschmoment < 0 ? 0ULL : (uint64_t)raw_tmp_motor_1_280_fahrerwunschmoment;
    if (raw_motor_1_280_fahrerwunschmoment > 0xFFULL) raw_motor_1_280_fahrerwunschmoment = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_motor_1_280_fahrerwunschmoment);
    // mechanisches_Motor_Verlustmomen
    int64_t raw_tmp_motor_1_280_mechanisches_motor_verlustmomen = (int64_t)llround((params::vw_pq.motor_1_280_mechanisches_motor_verlustmomen - 0.0f) / 0.39f);
    uint64_t raw_motor_1_280_mechanisches_motor_verlustmomen = raw_tmp_motor_1_280_mechanisches_motor_verlustmomen < 0 ? 0ULL : (uint64_t)raw_tmp_motor_1_280_mechanisches_motor_verlustmomen;
    if (raw_motor_1_280_mechanisches_motor_verlustmomen > 0xFFULL) raw_motor_1_280_mechanisches_motor_verlustmomen = 0xFFULL;
    writeBitsLE(frame.data, 48, 8, (uint32_t)raw_motor_1_280_mechanisches_motor_verlustmomen);
    // Fahrpedalwert_oder_Drosselklapp
    int64_t raw_tmp_motor_1_280_fahrpedalwert_oder_drosselklapp = (int64_t)llround((params::vw_pq.motor_1_280_fahrpedalwert_oder_drosselklapp - 0.0f) / 0.4f);
    uint64_t raw_motor_1_280_fahrpedalwert_oder_drosselklapp = raw_tmp_motor_1_280_fahrpedalwert_oder_drosselklapp < 0 ? 0ULL : (uint64_t)raw_tmp_motor_1_280_fahrpedalwert_oder_drosselklapp;
    if (raw_motor_1_280_fahrpedalwert_oder_drosselklapp > 0xFFULL) raw_motor_1_280_fahrpedalwert_oder_drosselklapp = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_motor_1_280_fahrpedalwert_oder_drosselklapp);
    // inneres_Motor_Moment_ohne_exter
    int64_t raw_tmp_motor_1_280_inneres_motor_moment_ohne_exter = (int64_t)llround((params::vw_pq.motor_1_280_inneres_motor_moment_ohne_exter - 0.0f) / 0.39f);
    uint64_t raw_motor_1_280_inneres_motor_moment_ohne_exter = raw_tmp_motor_1_280_inneres_motor_moment_ohne_exter < 0 ? 0ULL : (uint64_t)raw_tmp_motor_1_280_inneres_motor_moment_ohne_exter;
    if (raw_motor_1_280_inneres_motor_moment_ohne_exter > 0xFFULL) raw_motor_1_280_inneres_motor_moment_ohne_exter = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_motor_1_280_inneres_motor_moment_ohne_exter);
    // Motordrehzahl
    int64_t raw_tmp_motor_1_280_motordrehzahl = (int64_t)llround((params::vw_pq.motor_1_280_motordrehzahl - 0.0f) / 0.25f);
    uint64_t raw_motor_1_280_motordrehzahl = raw_tmp_motor_1_280_motordrehzahl < 0 ? 0ULL : (uint64_t)raw_tmp_motor_1_280_motordrehzahl;
    if (raw_motor_1_280_motordrehzahl > 0xFFFFULL) raw_motor_1_280_motordrehzahl = 0xFFFFULL;
    writeBitsLE(frame.data, 16, 16, (uint32_t)raw_motor_1_280_motordrehzahl);
    // inneres_Motor_Moment
    int64_t raw_tmp_motor_1_280_inneres_motor_moment = (int64_t)llround((params::vw_pq.motor_1_280_inneres_motor_moment - 0.0f) / 0.39f);
    uint64_t raw_motor_1_280_inneres_motor_moment = raw_tmp_motor_1_280_inneres_motor_moment < 0 ? 0ULL : (uint64_t)raw_tmp_motor_1_280_inneres_motor_moment;
    if (raw_motor_1_280_inneres_motor_moment > 0xFFULL) raw_motor_1_280_inneres_motor_moment = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_motor_1_280_inneres_motor_moment);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.motor_1_280_momentenangaben_ungenau ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.motor_1_280_fehlerstatus_getriebe_momentene ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.motor_1_280_fehlerstatus_brems_momenteneing ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.motor_1_280_time_out_bremsen_botschaft ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.motor_1_280_kupplungsschalter ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.motor_1_280_kickdownschalter ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.motor_1_280_fahrpedalwert_ungenau_motor_1_m ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.motor_1_280_leergasinformation ? 1U : 0U);
}

// Message 0x106 (262) - Master_3
// DLC: 8, TX: XXX
bool decodeVwPq0x106(const CANMessage &frame) {
    if (frame.id != 0x106 || frame.len < 8) {
        return false;
    }

    // Frei_Master_3_1: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_master_3_106_frei_master_3_1 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.master_3_106_frei_master_3_1 = raw_master_3_106_frei_master_3_1 * 1.0f + 0.0f;
    // Motortemperatur_linearisiert: start=48 len=8 endian=1 sign=+ factor=0.75 offset=-48 unit=""
    const uint32_t raw_master_3_106_motortemperatur_linearisiert = readBitsLE(frame.data, 48, 8);
    params::vw_pq.master_3_106_motortemperatur_linearisiert = raw_master_3_106_motortemperatur_linearisiert * 0.75f + -48.0f;
    // Indiziertes_Sollmoment_f_r_Vmax: start=32 len=16 endian=1 sign=+ factor=0.0015259 offset=0 unit="%"
    const uint32_t raw_master_3_106_indiziertes_sollmoment_f_r_vmax = readBitsLE(frame.data, 32, 16);
    params::vw_pq.master_3_106_indiziertes_sollmoment_f_r_vmax = raw_master_3_106_indiziertes_sollmoment_f_r_vmax * 0.0015259f + 0.0f;
    // Relative_Momentenanforderung_de: start=16 len=16 endian=1 sign=+ factor=0.003052 offset=0 unit="%"
    const uint32_t raw_master_3_106_relative_momentenanforderung_de = readBitsLE(frame.data, 16, 16);
    params::vw_pq.master_3_106_relative_momentenanforderung_de = raw_master_3_106_relative_momentenanforderung_de * 0.003052f + 0.0f;
    // Delta_Motormoment_aus_Verlustmo: start=0 len=16 endian=1 sign=+ factor=0.003052 offset=-100 unit="%"
    const uint32_t raw_master_3_106_delta_motormoment_aus_verlustmo = readBitsLE(frame.data, 0, 16);
    params::vw_pq.master_3_106_delta_motormoment_aus_verlustmo = raw_master_3_106_delta_motormoment_aus_verlustmo * 0.003052f + -100.0f;
    return true;
}

void encodeVwPq0x106(CANMessage &frame) {
    frame.id = 0x106;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Frei_Master_3_1
    int64_t raw_tmp_master_3_106_frei_master_3_1 = (int64_t)llround((params::vw_pq.master_3_106_frei_master_3_1 - 0.0f) / 1.0f);
    uint64_t raw_master_3_106_frei_master_3_1 = raw_tmp_master_3_106_frei_master_3_1 < 0 ? 0ULL : (uint64_t)raw_tmp_master_3_106_frei_master_3_1;
    if (raw_master_3_106_frei_master_3_1 > 0xFFULL) raw_master_3_106_frei_master_3_1 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_master_3_106_frei_master_3_1);
    // Motortemperatur_linearisiert
    int64_t raw_tmp_master_3_106_motortemperatur_linearisiert = (int64_t)llround((params::vw_pq.master_3_106_motortemperatur_linearisiert - -48.0f) / 0.75f);
    uint64_t raw_master_3_106_motortemperatur_linearisiert = raw_tmp_master_3_106_motortemperatur_linearisiert < 0 ? 0ULL : (uint64_t)raw_tmp_master_3_106_motortemperatur_linearisiert;
    if (raw_master_3_106_motortemperatur_linearisiert > 0xFFULL) raw_master_3_106_motortemperatur_linearisiert = 0xFFULL;
    writeBitsLE(frame.data, 48, 8, (uint32_t)raw_master_3_106_motortemperatur_linearisiert);
    // Indiziertes_Sollmoment_f_r_Vmax
    int64_t raw_tmp_master_3_106_indiziertes_sollmoment_f_r_vmax = (int64_t)llround((params::vw_pq.master_3_106_indiziertes_sollmoment_f_r_vmax - 0.0f) / 0.0015259f);
    uint64_t raw_master_3_106_indiziertes_sollmoment_f_r_vmax = raw_tmp_master_3_106_indiziertes_sollmoment_f_r_vmax < 0 ? 0ULL : (uint64_t)raw_tmp_master_3_106_indiziertes_sollmoment_f_r_vmax;
    if (raw_master_3_106_indiziertes_sollmoment_f_r_vmax > 0xFFFFULL) raw_master_3_106_indiziertes_sollmoment_f_r_vmax = 0xFFFFULL;
    writeBitsLE(frame.data, 32, 16, (uint32_t)raw_master_3_106_indiziertes_sollmoment_f_r_vmax);
    // Relative_Momentenanforderung_de
    int64_t raw_tmp_master_3_106_relative_momentenanforderung_de = (int64_t)llround((params::vw_pq.master_3_106_relative_momentenanforderung_de - 0.0f) / 0.003052f);
    uint64_t raw_master_3_106_relative_momentenanforderung_de = raw_tmp_master_3_106_relative_momentenanforderung_de < 0 ? 0ULL : (uint64_t)raw_tmp_master_3_106_relative_momentenanforderung_de;
    if (raw_master_3_106_relative_momentenanforderung_de > 0xFFFFULL) raw_master_3_106_relative_momentenanforderung_de = 0xFFFFULL;
    writeBitsLE(frame.data, 16, 16, (uint32_t)raw_master_3_106_relative_momentenanforderung_de);
    // Delta_Motormoment_aus_Verlustmo
    int64_t raw_tmp_master_3_106_delta_motormoment_aus_verlustmo = (int64_t)llround((params::vw_pq.master_3_106_delta_motormoment_aus_verlustmo - -100.0f) / 0.003052f);
    uint64_t raw_master_3_106_delta_motormoment_aus_verlustmo = raw_tmp_master_3_106_delta_motormoment_aus_verlustmo < 0 ? 0ULL : (uint64_t)raw_tmp_master_3_106_delta_motormoment_aus_verlustmo;
    if (raw_master_3_106_delta_motormoment_aus_verlustmo > 0xFFFFULL) raw_master_3_106_delta_motormoment_aus_verlustmo = 0xFFFFULL;
    writeBitsLE(frame.data, 0, 16, (uint32_t)raw_master_3_106_delta_motormoment_aus_verlustmo);
}

// Message 0x104 (260) - Master_2
// DLC: 8, TX: XXX
bool decodeVwPq0x104(const CANMessage &frame) {
    if (frame.id != 0x104 || frame.len < 8) {
        return false;
    }

    // Ubat_Freigabe_DVE: start=45 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.master_2_104_ubat_freigabe_dve = readBitsLE(frame.data, 45, 1) != 0;
    // DK_Sollwertbegrenzung: start=44 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.master_2_104_dk_sollwertbegrenzung = readBitsLE(frame.data, 44, 1) != 0;
    // Einspritzverbot_DV_E_lernen: start=43 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.master_2_104_einspritzverbot_dv_e_lernen = readBitsLE(frame.data, 43, 1) != 0;
    // Master_erkannt: start=42 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.master_2_104_master_erkannt = readBitsLE(frame.data, 42, 1) != 0;
    // Vmax_Begrenzung_aktiv: start=41 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.master_2_104_vmax_begrenzung_aktiv = readBitsLE(frame.data, 41, 1) != 0;
    // SA_Verbot_von_FGR: start=40 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.master_2_104_sa_verbot_von_fgr = readBitsLE(frame.data, 40, 1) != 0;
    // Pedalwert_Komplement: start=24 len=16 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_master_2_104_pedalwert_komplement = readBitsLE(frame.data, 24, 16);
    params::vw_pq.master_2_104_pedalwert_komplement = raw_master_2_104_pedalwert_komplement * 1.0f + 0.0f;
    // Zaehler__Master_2_: start=16 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_master_2_104_zaehler_master_2 = readBitsLE(frame.data, 16, 8);
    params::vw_pq.master_2_104_zaehler_master_2 = raw_master_2_104_zaehler_master_2 * 1.0f + 0.0f;
    // Normierter_Fahrpedalwinkel: start=0 len=16 endian=1 sign=+ factor=0.001526 offset=0 unit="%"
    const uint32_t raw_master_2_104_normierter_fahrpedalwinkel = readBitsLE(frame.data, 0, 16);
    params::vw_pq.master_2_104_normierter_fahrpedalwinkel = raw_master_2_104_normierter_fahrpedalwinkel * 0.001526f + 0.0f;
    return true;
}

void encodeVwPq0x104(CANMessage &frame) {
    frame.id = 0x104;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 45, 1, params::vw_pq.master_2_104_ubat_freigabe_dve ? 1U : 0U);
    writeBitsLE(frame.data, 44, 1, params::vw_pq.master_2_104_dk_sollwertbegrenzung ? 1U : 0U);
    writeBitsLE(frame.data, 43, 1, params::vw_pq.master_2_104_einspritzverbot_dv_e_lernen ? 1U : 0U);
    writeBitsLE(frame.data, 42, 1, params::vw_pq.master_2_104_master_erkannt ? 1U : 0U);
    writeBitsLE(frame.data, 41, 1, params::vw_pq.master_2_104_vmax_begrenzung_aktiv ? 1U : 0U);
    writeBitsLE(frame.data, 40, 1, params::vw_pq.master_2_104_sa_verbot_von_fgr ? 1U : 0U);
    // Pedalwert_Komplement
    int64_t raw_tmp_master_2_104_pedalwert_komplement = (int64_t)llround((params::vw_pq.master_2_104_pedalwert_komplement - 0.0f) / 1.0f);
    uint64_t raw_master_2_104_pedalwert_komplement = raw_tmp_master_2_104_pedalwert_komplement < 0 ? 0ULL : (uint64_t)raw_tmp_master_2_104_pedalwert_komplement;
    if (raw_master_2_104_pedalwert_komplement > 0xFFFFULL) raw_master_2_104_pedalwert_komplement = 0xFFFFULL;
    writeBitsLE(frame.data, 24, 16, (uint32_t)raw_master_2_104_pedalwert_komplement);
    // Zaehler__Master_2_
    int64_t raw_tmp_master_2_104_zaehler_master_2 = (int64_t)llround((params::vw_pq.master_2_104_zaehler_master_2 - 0.0f) / 1.0f);
    uint64_t raw_master_2_104_zaehler_master_2 = raw_tmp_master_2_104_zaehler_master_2 < 0 ? 0ULL : (uint64_t)raw_tmp_master_2_104_zaehler_master_2;
    if (raw_master_2_104_zaehler_master_2 > 0xFFULL) raw_master_2_104_zaehler_master_2 = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_master_2_104_zaehler_master_2);
    // Normierter_Fahrpedalwinkel
    int64_t raw_tmp_master_2_104_normierter_fahrpedalwinkel = (int64_t)llround((params::vw_pq.master_2_104_normierter_fahrpedalwinkel - 0.0f) / 0.001526f);
    uint64_t raw_master_2_104_normierter_fahrpedalwinkel = raw_tmp_master_2_104_normierter_fahrpedalwinkel < 0 ? 0ULL : (uint64_t)raw_tmp_master_2_104_normierter_fahrpedalwinkel;
    if (raw_master_2_104_normierter_fahrpedalwinkel > 0xFFFFULL) raw_master_2_104_normierter_fahrpedalwinkel = 0xFFFFULL;
    writeBitsLE(frame.data, 0, 16, (uint32_t)raw_master_2_104_normierter_fahrpedalwinkel);
}

// Message 0x102 (258) - Master_1
// DLC: 8, TX: XXX
bool decodeVwPq0x102(const CANMessage &frame) {
    if (frame.id != 0x102 || frame.len < 8) {
        return false;
    }

    // Stationaere_Solldrehzahl: start=56 len=8 endian=1 sign=+ factor=10 offset=0 unit="Umin"
    const uint32_t raw_master_1_102_stationaere_solldrehzahl = readBitsLE(frame.data, 56, 8);
    params::vw_pq.master_1_102_stationaere_solldrehzahl = raw_master_1_102_stationaere_solldrehzahl * 10.0f + 0.0f;
    // Drehmoment_LLR__I_Anteil_: start=40 len=16 endian=1 sign=+ factor=0.003052 offset=-100 unit="%"
    const uint32_t raw_master_1_102_drehmoment_llr_i_anteil = readBitsLE(frame.data, 40, 16);
    params::vw_pq.master_1_102_drehmoment_llr_i_anteil = raw_master_1_102_drehmoment_llr_i_anteil * 0.003052f + -100.0f;
    // Drehmoment_LLR__PD_Anteil_: start=24 len=16 endian=1 sign=+ factor=0.003052 offset=-100 unit="%"
    const uint32_t raw_master_1_102_drehmoment_llr_pd_anteil = readBitsLE(frame.data, 24, 16);
    params::vw_pq.master_1_102_drehmoment_llr_pd_anteil = raw_master_1_102_drehmoment_llr_pd_anteil * 0.003052f + -100.0f;
    // Drehmoment_LLR__Luftpfad_: start=8 len=16 endian=1 sign=+ factor=0.003052 offset=-100 unit="%"
    const uint32_t raw_master_1_102_drehmoment_llr_luftpfad = readBitsLE(frame.data, 8, 16);
    params::vw_pq.master_1_102_drehmoment_llr_luftpfad = raw_master_1_102_drehmoment_llr_luftpfad * 0.003052f + -100.0f;
    // Frei_Master_1_1: start=4 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_master_1_102_frei_master_1_1 = readBitsLE(frame.data, 4, 4);
    params::vw_pq.master_1_102_frei_master_1_1 = raw_master_1_102_frei_master_1_1 * 1.0f + 0.0f;
    // LLR_ist_aktiv: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.master_1_102_llr_ist_aktiv = readBitsLE(frame.data, 3, 1) != 0;
    // I_Anteil_der_LLR_aktiv: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.master_1_102_i_anteil_der_llr_aktiv = readBitsLE(frame.data, 2, 1) != 0;
    // PD_Anteil_der_LLR_aktiv: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.master_1_102_pd_anteil_der_llr_aktiv = readBitsLE(frame.data, 1, 1) != 0;
    // LLR_Freigabe_nach_Start: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.master_1_102_llr_freigabe_nach_start = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x102(CANMessage &frame) {
    frame.id = 0x102;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Stationaere_Solldrehzahl
    int64_t raw_tmp_master_1_102_stationaere_solldrehzahl = (int64_t)llround((params::vw_pq.master_1_102_stationaere_solldrehzahl - 0.0f) / 10.0f);
    uint64_t raw_master_1_102_stationaere_solldrehzahl = raw_tmp_master_1_102_stationaere_solldrehzahl < 0 ? 0ULL : (uint64_t)raw_tmp_master_1_102_stationaere_solldrehzahl;
    if (raw_master_1_102_stationaere_solldrehzahl > 0xFFULL) raw_master_1_102_stationaere_solldrehzahl = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_master_1_102_stationaere_solldrehzahl);
    // Drehmoment_LLR__I_Anteil_
    int64_t raw_tmp_master_1_102_drehmoment_llr_i_anteil = (int64_t)llround((params::vw_pq.master_1_102_drehmoment_llr_i_anteil - -100.0f) / 0.003052f);
    uint64_t raw_master_1_102_drehmoment_llr_i_anteil = raw_tmp_master_1_102_drehmoment_llr_i_anteil < 0 ? 0ULL : (uint64_t)raw_tmp_master_1_102_drehmoment_llr_i_anteil;
    if (raw_master_1_102_drehmoment_llr_i_anteil > 0xFFFFULL) raw_master_1_102_drehmoment_llr_i_anteil = 0xFFFFULL;
    writeBitsLE(frame.data, 40, 16, (uint32_t)raw_master_1_102_drehmoment_llr_i_anteil);
    // Drehmoment_LLR__PD_Anteil_
    int64_t raw_tmp_master_1_102_drehmoment_llr_pd_anteil = (int64_t)llround((params::vw_pq.master_1_102_drehmoment_llr_pd_anteil - -100.0f) / 0.003052f);
    uint64_t raw_master_1_102_drehmoment_llr_pd_anteil = raw_tmp_master_1_102_drehmoment_llr_pd_anteil < 0 ? 0ULL : (uint64_t)raw_tmp_master_1_102_drehmoment_llr_pd_anteil;
    if (raw_master_1_102_drehmoment_llr_pd_anteil > 0xFFFFULL) raw_master_1_102_drehmoment_llr_pd_anteil = 0xFFFFULL;
    writeBitsLE(frame.data, 24, 16, (uint32_t)raw_master_1_102_drehmoment_llr_pd_anteil);
    // Drehmoment_LLR__Luftpfad_
    int64_t raw_tmp_master_1_102_drehmoment_llr_luftpfad = (int64_t)llround((params::vw_pq.master_1_102_drehmoment_llr_luftpfad - -100.0f) / 0.003052f);
    uint64_t raw_master_1_102_drehmoment_llr_luftpfad = raw_tmp_master_1_102_drehmoment_llr_luftpfad < 0 ? 0ULL : (uint64_t)raw_tmp_master_1_102_drehmoment_llr_luftpfad;
    if (raw_master_1_102_drehmoment_llr_luftpfad > 0xFFFFULL) raw_master_1_102_drehmoment_llr_luftpfad = 0xFFFFULL;
    writeBitsLE(frame.data, 8, 16, (uint32_t)raw_master_1_102_drehmoment_llr_luftpfad);
    // Frei_Master_1_1
    int64_t raw_tmp_master_1_102_frei_master_1_1 = (int64_t)llround((params::vw_pq.master_1_102_frei_master_1_1 - 0.0f) / 1.0f);
    uint64_t raw_master_1_102_frei_master_1_1 = raw_tmp_master_1_102_frei_master_1_1 < 0 ? 0ULL : (uint64_t)raw_tmp_master_1_102_frei_master_1_1;
    if (raw_master_1_102_frei_master_1_1 > 0xFULL) raw_master_1_102_frei_master_1_1 = 0xFULL;
    writeBitsLE(frame.data, 4, 4, (uint32_t)raw_master_1_102_frei_master_1_1);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.master_1_102_llr_ist_aktiv ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.master_1_102_i_anteil_der_llr_aktiv ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.master_1_102_pd_anteil_der_llr_aktiv ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.master_1_102_llr_freigabe_nach_start ? 1U : 0U);
}

// Message 0x7C0 (1984) - LWS_Kalibrierung
// DLC: 2, TX: XXX
bool decodeVwPq0x7C0(const CANMessage &frame) {
    if (frame.id != 0x7C0 || frame.len < 2) {
        return false;
    }

    // Frei_LWS_Kalibrierung_1_1: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lws_kalibrierung_7c0_frei_lws_kalibrierung_1_1 = readBitsLE(frame.data, 15, 1) != 0;
    // LWS_Identifier: start=8 len=7 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lws_kalibrierung_7c0_lws_identifier = readBitsLE(frame.data, 8, 7);
    params::vw_pq.lws_kalibrierung_7c0_lws_identifier = raw_lws_kalibrierung_7c0_lws_identifier * 1.0f + 0.0f;
    // Frei_LWS_Kalibrierung_1_2: start=4 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lws_kalibrierung_7c0_frei_lws_kalibrierung_1_2 = readBitsLE(frame.data, 4, 4);
    params::vw_pq.lws_kalibrierung_7c0_frei_lws_kalibrierung_1_2 = raw_lws_kalibrierung_7c0_frei_lws_kalibrierung_1_2 * 1.0f + 0.0f;
    // Command_Dode_Word: start=0 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lws_kalibrierung_7c0_command_dode_word = readBitsLE(frame.data, 0, 4);
    params::vw_pq.lws_kalibrierung_7c0_command_dode_word = raw_lws_kalibrierung_7c0_command_dode_word * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x7C0(CANMessage &frame) {
    frame.id = 0x7C0;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 15, 1, params::vw_pq.lws_kalibrierung_7c0_frei_lws_kalibrierung_1_1 ? 1U : 0U);
    // LWS_Identifier
    int64_t raw_tmp_lws_kalibrierung_7c0_lws_identifier = (int64_t)llround((params::vw_pq.lws_kalibrierung_7c0_lws_identifier - 0.0f) / 1.0f);
    uint64_t raw_lws_kalibrierung_7c0_lws_identifier = raw_tmp_lws_kalibrierung_7c0_lws_identifier < 0 ? 0ULL : (uint64_t)raw_tmp_lws_kalibrierung_7c0_lws_identifier;
    if (raw_lws_kalibrierung_7c0_lws_identifier > 0x7FULL) raw_lws_kalibrierung_7c0_lws_identifier = 0x7FULL;
    writeBitsLE(frame.data, 8, 7, (uint32_t)raw_lws_kalibrierung_7c0_lws_identifier);
    // Frei_LWS_Kalibrierung_1_2
    int64_t raw_tmp_lws_kalibrierung_7c0_frei_lws_kalibrierung_1_2 = (int64_t)llround((params::vw_pq.lws_kalibrierung_7c0_frei_lws_kalibrierung_1_2 - 0.0f) / 1.0f);
    uint64_t raw_lws_kalibrierung_7c0_frei_lws_kalibrierung_1_2 = raw_tmp_lws_kalibrierung_7c0_frei_lws_kalibrierung_1_2 < 0 ? 0ULL : (uint64_t)raw_tmp_lws_kalibrierung_7c0_frei_lws_kalibrierung_1_2;
    if (raw_lws_kalibrierung_7c0_frei_lws_kalibrierung_1_2 > 0xFULL) raw_lws_kalibrierung_7c0_frei_lws_kalibrierung_1_2 = 0xFULL;
    writeBitsLE(frame.data, 4, 4, (uint32_t)raw_lws_kalibrierung_7c0_frei_lws_kalibrierung_1_2);
    // Command_Dode_Word
    int64_t raw_tmp_lws_kalibrierung_7c0_command_dode_word = (int64_t)llround((params::vw_pq.lws_kalibrierung_7c0_command_dode_word - 0.0f) / 1.0f);
    uint64_t raw_lws_kalibrierung_7c0_command_dode_word = raw_tmp_lws_kalibrierung_7c0_command_dode_word < 0 ? 0ULL : (uint64_t)raw_tmp_lws_kalibrierung_7c0_command_dode_word;
    if (raw_lws_kalibrierung_7c0_command_dode_word > 0xFULL) raw_lws_kalibrierung_7c0_command_dode_word = 0xFULL;
    writeBitsLE(frame.data, 0, 4, (uint32_t)raw_lws_kalibrierung_7c0_command_dode_word);
}

// Message 0x7C2 (1986) - Lenkwinkel_Init
// DLC: 4, TX: XXX
bool decodeVwPq0x7C2(const CANMessage &frame) {
    if (frame.id != 0x7C2 || frame.len < 4) {
        return false;
    }

    // Kodierbytes: start=8 len=24 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkwinkel_init_7c2_kodierbytes = readBitsLE(frame.data, 8, 24);
    params::vw_pq.lenkwinkel_init_7c2_kodierbytes = raw_lenkwinkel_init_7c2_kodierbytes * 1.0f + 0.0f;
    // Identiifier_f_r_LWS_Init: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkwinkel_init_7c2_identiifier_f_r_lws_init = readBitsLE(frame.data, 0, 8);
    params::vw_pq.lenkwinkel_init_7c2_identiifier_f_r_lws_init = raw_lenkwinkel_init_7c2_identiifier_f_r_lws_init * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x7C2(CANMessage &frame) {
    frame.id = 0x7C2;
    frame.len = 4;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Kodierbytes
    int64_t raw_tmp_lenkwinkel_init_7c2_kodierbytes = (int64_t)llround((params::vw_pq.lenkwinkel_init_7c2_kodierbytes - 0.0f) / 1.0f);
    uint64_t raw_lenkwinkel_init_7c2_kodierbytes = raw_tmp_lenkwinkel_init_7c2_kodierbytes < 0 ? 0ULL : (uint64_t)raw_tmp_lenkwinkel_init_7c2_kodierbytes;
    if (raw_lenkwinkel_init_7c2_kodierbytes > 0xFFFFFFULL) raw_lenkwinkel_init_7c2_kodierbytes = 0xFFFFFFULL;
    writeBitsLE(frame.data, 8, 24, (uint32_t)raw_lenkwinkel_init_7c2_kodierbytes);
    // Identiifier_f_r_LWS_Init
    int64_t raw_tmp_lenkwinkel_init_7c2_identiifier_f_r_lws_init = (int64_t)llround((params::vw_pq.lenkwinkel_init_7c2_identiifier_f_r_lws_init - 0.0f) / 1.0f);
    uint64_t raw_lenkwinkel_init_7c2_identiifier_f_r_lws_init = raw_tmp_lenkwinkel_init_7c2_identiifier_f_r_lws_init < 0 ? 0ULL : (uint64_t)raw_tmp_lenkwinkel_init_7c2_identiifier_f_r_lws_init;
    if (raw_lenkwinkel_init_7c2_identiifier_f_r_lws_init > 0xFFULL) raw_lenkwinkel_init_7c2_identiifier_f_r_lws_init = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_lenkwinkel_init_7c2_identiifier_f_r_lws_init);
}

// Message 0x0C0 (192) - Lenkwinkel_1__RB_
// DLC: 2, TX: XXX
bool decodeVwPq0xC0(const CANMessage &frame) {
    if (frame.id != 0xC0 || frame.len < 2) {
        return false;
    }

    // Vorzeichen__RB_: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkwinkel_1_rb_0c0_vorzeichen_rb = readBitsLE(frame.data, 15, 1) != 0;
    // Lenkwinkel__RB_: start=5 len=10 endian=1 sign=+ factor=2.5 offset=-720 unit=""
    const uint32_t raw_lenkwinkel_1_rb_0c0_lenkwinkel_rb = readBitsLE(frame.data, 5, 10);
    params::vw_pq.lenkwinkel_1_rb_0c0_lenkwinkel_rb = raw_lenkwinkel_1_rb_0c0_lenkwinkel_rb * 2.5f + -720.0f;
    // LWS_OK__RB_: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkwinkel_1_rb_0c0_lws_ok_rb = readBitsLE(frame.data, 4, 1) != 0;
    // LWS_Abgleich__RB_: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkwinkel_1_rb_0c0_lws_abgleich_rb = readBitsLE(frame.data, 3, 1) != 0;
    // Frei_Lenkwinkel_1_1__RB_: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkwinkel_1_rb_0c0_frei_lenkwinkel_1_1_rb = readBitsLE(frame.data, 2, 1) != 0;
    // FINE_CHECK__RB_: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkwinkel_1_rb_0c0_fine_check_rb = readBitsLE(frame.data, 1, 1) != 0;
    // COARSE_CHECK__RB_: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkwinkel_1_rb_0c0_coarse_check_rb = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0xC0(CANMessage &frame) {
    frame.id = 0xC0;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 15, 1, params::vw_pq.lenkwinkel_1_rb_0c0_vorzeichen_rb ? 1U : 0U);
    // Lenkwinkel__RB_
    int64_t raw_tmp_lenkwinkel_1_rb_0c0_lenkwinkel_rb = (int64_t)llround((params::vw_pq.lenkwinkel_1_rb_0c0_lenkwinkel_rb - -720.0f) / 2.5f);
    uint64_t raw_lenkwinkel_1_rb_0c0_lenkwinkel_rb = raw_tmp_lenkwinkel_1_rb_0c0_lenkwinkel_rb < 0 ? 0ULL : (uint64_t)raw_tmp_lenkwinkel_1_rb_0c0_lenkwinkel_rb;
    if (raw_lenkwinkel_1_rb_0c0_lenkwinkel_rb > 0x3FFULL) raw_lenkwinkel_1_rb_0c0_lenkwinkel_rb = 0x3FFULL;
    writeBitsLE(frame.data, 5, 10, (uint32_t)raw_lenkwinkel_1_rb_0c0_lenkwinkel_rb);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.lenkwinkel_1_rb_0c0_lws_ok_rb ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.lenkwinkel_1_rb_0c0_lws_abgleich_rb ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.lenkwinkel_1_rb_0c0_frei_lenkwinkel_1_1_rb ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.lenkwinkel_1_rb_0c0_fine_check_rb ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.lenkwinkel_1_rb_0c0_coarse_check_rb ? 1U : 0U);
}

// Message 0x0C4 (196) - Lenkwinkel_1__ITT_
// DLC: 2, TX: XXX
bool decodeVwPq0xC4(const CANMessage &frame) {
    if (frame.id != 0xC4 || frame.len < 2) {
        return false;
    }

    // Vorzeichen__ITT_: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkwinkel_1_itt_0c4_vorzeichen_itt = readBitsLE(frame.data, 15, 1) != 0;
    // Lenkwinkel__ITT_: start=5 len=10 endian=1 sign=+ factor=1.5 offset=-768 unit=""
    const uint32_t raw_lenkwinkel_1_itt_0c4_lenkwinkel_itt = readBitsLE(frame.data, 5, 10);
    params::vw_pq.lenkwinkel_1_itt_0c4_lenkwinkel_itt = raw_lenkwinkel_1_itt_0c4_lenkwinkel_itt * 1.5f + -768.0f;
    // LWS_OK: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkwinkel_1_itt_0c4_lws_ok = readBitsLE(frame.data, 4, 1) != 0;
    // LWS_Abgleich__ITT_: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkwinkel_1_itt_0c4_lws_abgleich_itt = readBitsLE(frame.data, 3, 1) != 0;
    // LWS_Initialisierung__ITT_: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkwinkel_1_itt_0c4_lws_initialisierung_itt = readBitsLE(frame.data, 2, 1) != 0;
    // FINE_CHECK__ITT_: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkwinkel_1_itt_0c4_fine_check_itt = readBitsLE(frame.data, 1, 1) != 0;
    // COARSE_CHECK__ITT_: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkwinkel_1_itt_0c4_coarse_check_itt = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0xC4(CANMessage &frame) {
    frame.id = 0xC4;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 15, 1, params::vw_pq.lenkwinkel_1_itt_0c4_vorzeichen_itt ? 1U : 0U);
    // Lenkwinkel__ITT_
    int64_t raw_tmp_lenkwinkel_1_itt_0c4_lenkwinkel_itt = (int64_t)llround((params::vw_pq.lenkwinkel_1_itt_0c4_lenkwinkel_itt - -768.0f) / 1.5f);
    uint64_t raw_lenkwinkel_1_itt_0c4_lenkwinkel_itt = raw_tmp_lenkwinkel_1_itt_0c4_lenkwinkel_itt < 0 ? 0ULL : (uint64_t)raw_tmp_lenkwinkel_1_itt_0c4_lenkwinkel_itt;
    if (raw_lenkwinkel_1_itt_0c4_lenkwinkel_itt > 0x3FFULL) raw_lenkwinkel_1_itt_0c4_lenkwinkel_itt = 0x3FFULL;
    writeBitsLE(frame.data, 5, 10, (uint32_t)raw_lenkwinkel_1_itt_0c4_lenkwinkel_itt);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.lenkwinkel_1_itt_0c4_lws_ok ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.lenkwinkel_1_itt_0c4_lws_abgleich_itt ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.lenkwinkel_1_itt_0c4_lws_initialisierung_itt ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.lenkwinkel_1_itt_0c4_fine_check_itt ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.lenkwinkel_1_itt_0c4_coarse_check_itt ? 1U : 0U);
}

// Message 0x0C2 (194) - Lenkwinkel_1
// DLC: 8, TX: XXX
// Comment: Steering wheel angle message 1 - Contains steering angle, speed, and status information
bool decodeVwPq0xC2(const CANMessage &frame) {
    if (frame.id != 0xC2 || frame.len < 8) {
        return false;
    }

    // LW1_LRW: start=0 len=15 endian=1 sign=+ factor=0.04375 offset=0 unit="Grad"
    // DBC comment: Steering wheel angle (0 = straight ahead, 7FFFh = stop)
    const uint32_t raw_lenkwinkel_1_0c2_lw1_lrw = readBitsLE(frame.data, 0, 15);
    params::vw_pq.lenkwinkel_1_0c2_lw1_lrw = raw_lenkwinkel_1_0c2_lw1_lrw * 0.04375f + 0.0f;
    // LW1_LRW_Sign: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Steering angle sign: 0=positive (left), 1=negative (right)
    // Value table: 0 "positives_Vorzeichen" 1 "negatives_Vorzeichen"
    params::vw_pq.lenkwinkel_1_0c2_lw1_lrw_sign = readBitsLE(frame.data, 15, 1) != 0;
    // LW1_Lenk_Gesch: start=16 len=15 endian=1 sign=+ factor=0.04375 offset=0 unit="Grad/s"
    // DBC comment: Steering wheel angular velocity (0 = no movement, 7FFFh = max speed)
    const uint32_t raw_lenkwinkel_1_0c2_lw1_lenk_gesch = readBitsLE(frame.data, 16, 15);
    params::vw_pq.lenkwinkel_1_0c2_lw1_lenk_gesch = raw_lenkwinkel_1_0c2_lw1_lenk_gesch * 0.04375f + 0.0f;
    // LW1_Gesch_Sign: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Angular velocity sign: 0=positive, 1=negative
    // Value table: 0 "positives_Vorzeichen" 1 "negatives_Vorzeichen"
    params::vw_pq.lenkwinkel_1_0c2_lw1_gesch_sign = readBitsLE(frame.data, 31, 1) != 0;
    // LW1_ID: start=32 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Calibration ID - 0=not calibrated, 128=calibrated (PQ35/46/VW32x/VW411)
    // Value table: 0 "noch_not_kalibriert" 128 "kalibriert_nur_bei_PQ35_46_VW32x_VW411"
    const uint32_t raw_lenkwinkel_1_0c2_lw1_id = readBitsLE(frame.data, 32, 8);
    params::vw_pq.lenkwinkel_1_0c2_lw1_id = raw_lenkwinkel_1_0c2_lw1_id * 1.0f + 0.0f;
    // LW1_Initquelle: start=40 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Initialization source
    // Value table: 0 "Bremse_3" 1 "EPS_Bit"
    params::vw_pq.lenkwinkel_1_0c2_lw1_initquelle = readBitsLE(frame.data, 40, 1) != 0;
    // LW1_Status: start=41 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Sensor status
    // Value table: 0 "OK" 1 "no_initial" 2 "sporadic_failure" 3 "permanent_failure"
    const uint32_t raw_lenkwinkel_1_0c2_lw1_status = readBitsLE(frame.data, 41, 2);
    params::vw_pq.lenkwinkel_1_0c2_lw1_status = raw_lenkwinkel_1_0c2_lw1_status * 1.0f + 0.0f;
    // LW1_Sta_KL30: start=43 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Terminal 30 status (Kostal sensor)
    // Value table: 0 "ok" 1 "no_init_s"
    params::vw_pq.lenkwinkel_1_0c2_lw1_sta_kl30 = readBitsLE(frame.data, 43, 1) != 0;
    // LW1_Zaehler: start=44 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Free running message counter
    const uint32_t raw_lenkwinkel_1_0c2_lw1_zaehler = readBitsLE(frame.data, 44, 4);
    params::vw_pq.lenkwinkel_1_0c2_lw1_zaehler = raw_lenkwinkel_1_0c2_lw1_zaehler * 1.0f + 0.0f;
    // LW1_Kodier: start=48 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Coding data (multiplex output depends on counter)
    const uint32_t raw_lenkwinkel_1_0c2_lw1_kodier = readBitsLE(frame.data, 48, 8);
    params::vw_pq.lenkwinkel_1_0c2_lw1_kodier = raw_lenkwinkel_1_0c2_lw1_kodier * 1.0f + 0.0f;
    // LW1_CRC: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Checksum - One's complement of 8-bit sum of bytes 1,2,3,4,6
    const uint32_t raw_lenkwinkel_1_0c2_lw1_crc = readBitsLE(frame.data, 56, 8);
    params::vw_pq.lenkwinkel_1_0c2_lw1_crc = raw_lenkwinkel_1_0c2_lw1_crc * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0xC2(CANMessage &frame) {
    frame.id = 0xC2;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // LW1_LRW
    int64_t raw_tmp_lenkwinkel_1_0c2_lw1_lrw = (int64_t)llround((params::vw_pq.lenkwinkel_1_0c2_lw1_lrw - 0.0f) / 0.04375f);
    uint64_t raw_lenkwinkel_1_0c2_lw1_lrw = raw_tmp_lenkwinkel_1_0c2_lw1_lrw < 0 ? 0ULL : (uint64_t)raw_tmp_lenkwinkel_1_0c2_lw1_lrw;
    if (raw_lenkwinkel_1_0c2_lw1_lrw > 0x7FFFULL) raw_lenkwinkel_1_0c2_lw1_lrw = 0x7FFFULL;
    writeBitsLE(frame.data, 0, 15, (uint32_t)raw_lenkwinkel_1_0c2_lw1_lrw);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.lenkwinkel_1_0c2_lw1_lrw_sign ? 1U : 0U);
    // LW1_Lenk_Gesch
    int64_t raw_tmp_lenkwinkel_1_0c2_lw1_lenk_gesch = (int64_t)llround((params::vw_pq.lenkwinkel_1_0c2_lw1_lenk_gesch - 0.0f) / 0.04375f);
    uint64_t raw_lenkwinkel_1_0c2_lw1_lenk_gesch = raw_tmp_lenkwinkel_1_0c2_lw1_lenk_gesch < 0 ? 0ULL : (uint64_t)raw_tmp_lenkwinkel_1_0c2_lw1_lenk_gesch;
    if (raw_lenkwinkel_1_0c2_lw1_lenk_gesch > 0x7FFFULL) raw_lenkwinkel_1_0c2_lw1_lenk_gesch = 0x7FFFULL;
    writeBitsLE(frame.data, 16, 15, (uint32_t)raw_lenkwinkel_1_0c2_lw1_lenk_gesch);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.lenkwinkel_1_0c2_lw1_gesch_sign ? 1U : 0U);
    // LW1_ID
    int64_t raw_tmp_lenkwinkel_1_0c2_lw1_id = (int64_t)llround((params::vw_pq.lenkwinkel_1_0c2_lw1_id - 0.0f) / 1.0f);
    uint64_t raw_lenkwinkel_1_0c2_lw1_id = raw_tmp_lenkwinkel_1_0c2_lw1_id < 0 ? 0ULL : (uint64_t)raw_tmp_lenkwinkel_1_0c2_lw1_id;
    if (raw_lenkwinkel_1_0c2_lw1_id > 0xFFULL) raw_lenkwinkel_1_0c2_lw1_id = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_lenkwinkel_1_0c2_lw1_id);
    writeBitsLE(frame.data, 40, 1, params::vw_pq.lenkwinkel_1_0c2_lw1_initquelle ? 1U : 0U);
    // LW1_Status
    int64_t raw_tmp_lenkwinkel_1_0c2_lw1_status = (int64_t)llround((params::vw_pq.lenkwinkel_1_0c2_lw1_status - 0.0f) / 1.0f);
    uint64_t raw_lenkwinkel_1_0c2_lw1_status = raw_tmp_lenkwinkel_1_0c2_lw1_status < 0 ? 0ULL : (uint64_t)raw_tmp_lenkwinkel_1_0c2_lw1_status;
    if (raw_lenkwinkel_1_0c2_lw1_status > 0x3ULL) raw_lenkwinkel_1_0c2_lw1_status = 0x3ULL;
    writeBitsLE(frame.data, 41, 2, (uint32_t)raw_lenkwinkel_1_0c2_lw1_status);
    writeBitsLE(frame.data, 43, 1, params::vw_pq.lenkwinkel_1_0c2_lw1_sta_kl30 ? 1U : 0U);
    // LW1_Zaehler
    int64_t raw_tmp_lenkwinkel_1_0c2_lw1_zaehler = (int64_t)llround((params::vw_pq.lenkwinkel_1_0c2_lw1_zaehler - 0.0f) / 1.0f);
    uint64_t raw_lenkwinkel_1_0c2_lw1_zaehler = raw_tmp_lenkwinkel_1_0c2_lw1_zaehler < 0 ? 0ULL : (uint64_t)raw_tmp_lenkwinkel_1_0c2_lw1_zaehler;
    if (raw_lenkwinkel_1_0c2_lw1_zaehler > 0xFULL) raw_lenkwinkel_1_0c2_lw1_zaehler = 0xFULL;
    writeBitsLE(frame.data, 44, 4, (uint32_t)raw_lenkwinkel_1_0c2_lw1_zaehler);
    // LW1_Kodier
    int64_t raw_tmp_lenkwinkel_1_0c2_lw1_kodier = (int64_t)llround((params::vw_pq.lenkwinkel_1_0c2_lw1_kodier - 0.0f) / 1.0f);
    uint64_t raw_lenkwinkel_1_0c2_lw1_kodier = raw_tmp_lenkwinkel_1_0c2_lw1_kodier < 0 ? 0ULL : (uint64_t)raw_tmp_lenkwinkel_1_0c2_lw1_kodier;
    if (raw_lenkwinkel_1_0c2_lw1_kodier > 0xFFULL) raw_lenkwinkel_1_0c2_lw1_kodier = 0xFFULL;
    writeBitsLE(frame.data, 48, 8, (uint32_t)raw_lenkwinkel_1_0c2_lw1_kodier);
    // LW1_CRC
    int64_t raw_tmp_lenkwinkel_1_0c2_lw1_crc = (int64_t)llround((params::vw_pq.lenkwinkel_1_0c2_lw1_crc - 0.0f) / 1.0f);
    uint64_t raw_lenkwinkel_1_0c2_lw1_crc = raw_tmp_lenkwinkel_1_0c2_lw1_crc < 0 ? 0ULL : (uint64_t)raw_tmp_lenkwinkel_1_0c2_lw1_crc;
    if (raw_lenkwinkel_1_0c2_lw1_crc > 0xFFULL) raw_lenkwinkel_1_0c2_lw1_crc = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_lenkwinkel_1_0c2_lw1_crc);
}

// Message 0x5DE (1502) - Lenkhilfe_Fehler
// DLC: 7, TX: XXX
bool decodeVwPq0x5DE(const CANMessage &frame) {
    if (frame.id != 0x5DE || frame.len < 7) {
        return false;
    }

    // Werkstattcode: start=48 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_fehler_5de_werkstattcode = readBitsLE(frame.data, 48, 8);
    params::vw_pq.lenkhilfe_fehler_5de_werkstattcode = raw_lenkhilfe_fehler_5de_werkstattcode * 1.0f + 0.0f;
    // Multiplex_Signal: start=45 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_fehler_5de_multiplex_signal = readBitsLE(frame.data, 45, 3);
    params::vw_pq.lenkhilfe_fehler_5de_multiplex_signal = raw_lenkhilfe_fehler_5de_multiplex_signal * 1.0f + 0.0f;
    // Sensorcodierung_Lenkhilfe: start=44 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_sensorcodierung_lenkhilfe = readBitsLE(frame.data, 44, 1) != 0;
    // Kennliniencodierung_Lenkhilfe: start=40 len=4 endian=1 sign=+ factor=1 offset=0 unit="Kennlinie"
    const uint32_t raw_lenkhilfe_fehler_5de_kennliniencodierung_lenkhilfe = readBitsLE(frame.data, 40, 4);
    params::vw_pq.lenkhilfe_fehler_5de_kennliniencodierung_lenkhilfe = raw_lenkhilfe_fehler_5de_kennliniencodierung_lenkhilfe * 1.0f + 0.0f;
    // Geber_f__Lenkw__Speicher_ausles: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_geber_f_lenkw_speicher_ausles = readBitsLE(frame.data, 39, 1) != 0;
    // Kombiinstr__Speicher_auslesen: start=38 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_kombiinstr_speicher_auslesen = readBitsLE(frame.data, 38, 1) != 0;
    // Steuerger__Speicher_auslesen: start=37 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_steuerger_speicher_auslesen = readBitsLE(frame.data, 37, 1) != 0;
    // Lenkhilfe_Steuergeraet_defekt: start=36 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_steuergeraet_defekt = readBitsLE(frame.data, 36, 1) != 0;
    // Frei_Lenkhilfe_1_3: start=32 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_3 = readBitsLE(frame.data, 32, 4);
    params::vw_pq.lenkhilfe_fehler_5de_frei_lenkhilfe_1_3 = raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_3 * 1.0f + 0.0f;
    // Frei_Lenkhilfe_1_4: start=30 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_4 = readBitsLE(frame.data, 30, 2);
    params::vw_pq.lenkhilfe_fehler_5de_frei_lenkhilfe_1_4 = raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_4 * 1.0f + 0.0f;
    // Geber_f__Lenkwi__k__CAN_Komm_sp: start=29 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_geber_f_lenkwi_k_can_komm_sp = readBitsLE(frame.data, 29, 1) != 0;
    // Geber_f__Lenkwinkel_k__CAN_Komm: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_geber_f_lenkwinkel_k_can_komm = readBitsLE(frame.data, 28, 1) != 0;
    // Kombiinstr__k__CAN_Kommunik__sp: start=27 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_kombiinstr_k_can_kommunik_sp = readBitsLE(frame.data, 27, 1) != 0;
    // Kombiinstrument_k__CAN_Kommunik: start=26 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_kombiinstrument_k_can_kommunik = readBitsLE(frame.data, 26, 1) != 0;
    // Steuergeraet_k__CAN_Kommunik__S: start=25 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_steuergeraet_k_can_kommunik_s = readBitsLE(frame.data, 25, 1) != 0;
    // Steuergeraet_keine_CAN_Kommunik: start=24 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_steuergeraet_keine_can_kommunik = readBitsLE(frame.data, 24, 1) != 0;
    // Lenkhilfe_Spannung_Ks__nach_Mas: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_spannung_ks_nach_mas = readBitsLE(frame.data, 23, 1) != 0;
    // Lenkhilfe_Spannung_Ks_nach_Mass: start=22 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_spannung_ks_nach_mass = readBitsLE(frame.data, 22, 1) != 0;
    // Lenkhilfe_Spannung_Ks__nach___s: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_spannung_ks_nach_s = readBitsLE(frame.data, 21, 1) != 0;
    // Lenkhilfe_Spannung_Ks__nach__: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_spannung_ks_nach = readBitsLE(frame.data, 20, 1) != 0;
    // Lenkhilfe_Temperaturschutz_sp_: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_temperaturschutz_sp = readBitsLE(frame.data, 19, 1) != 0;
    // Lenkhilfe_Temperaturschutz: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_temperaturschutz = readBitsLE(frame.data, 18, 1) != 0;
    // Lenkhilfe_Betrieb_unpl__Sig__sp: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_betrieb_unpl_sig_sp = readBitsLE(frame.data, 17, 1) != 0;
    // Lenkhilfe_Betrieb_unpl__Signal: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_betrieb_unpl_signal = readBitsLE(frame.data, 16, 1) != 0;
    // Lenkhilfesensor_k__Kommunik__sp: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_k_kommunik_sp = readBitsLE(frame.data, 15, 1) != 0;
    // Lenkhilfesensor_keine_Kommunik_: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_keine_kommunik = readBitsLE(frame.data, 14, 1) != 0;
    // Lenkhilfesensor_defekt_sp_: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_defekt_sp = readBitsLE(frame.data, 13, 1) != 0;
    // Lenkhilfesensor_defekt: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_defekt = readBitsLE(frame.data, 12, 1) != 0;
    // Lenkhilfesensor_Unterbrechung_s: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_unterbrechung_s = readBitsLE(frame.data, 11, 1) != 0;
    // Lenkhilfesensor_Unterbrechung: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_unterbrechung = readBitsLE(frame.data, 10, 1) != 0;
    // Lenkhilfesensor_Ks_nach_Masse_s: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_ks_nach_masse_s = readBitsLE(frame.data, 9, 1) != 0;
    // Lenkhifesensor_Ks_nach_Masse: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_lenkhifesensor_ks_nach_masse = readBitsLE(frame.data, 8, 1) != 0;
    // Spannung_Kl_15_zu_klein_sp_: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_spannung_kl_15_zu_klein_sp = readBitsLE(frame.data, 7, 1) != 0;
    // Spannung_Kl_15_zu_klein: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_spannung_kl_15_zu_klein = readBitsLE(frame.data, 6, 1) != 0;
    // Frei_Lenkhilfe_1_5: start=4 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_5 = readBitsLE(frame.data, 4, 2);
    params::vw_pq.lenkhilfe_fehler_5de_frei_lenkhilfe_1_5 = raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_5 * 1.0f + 0.0f;
    // Spannung_Kl_30_zu_klein_sp_: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_spannung_kl_30_zu_klein_sp = readBitsLE(frame.data, 3, 1) != 0;
    // Spannung_Kl_30_zu_klein: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_spannung_kl_30_zu_klein = readBitsLE(frame.data, 2, 1) != 0;
    // Spannung_Kl_30_zu_gro__sp_: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_spannung_kl_30_zu_gro_sp = readBitsLE(frame.data, 1, 1) != 0;
    // Spannung_Kl_30_zu_gro_: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_fehler_5de_spannung_kl_30_zu_gro = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x5DE(CANMessage &frame) {
    frame.id = 0x5DE;
    frame.len = 7;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Werkstattcode
    int64_t raw_tmp_lenkhilfe_fehler_5de_werkstattcode = (int64_t)llround((params::vw_pq.lenkhilfe_fehler_5de_werkstattcode - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_fehler_5de_werkstattcode = raw_tmp_lenkhilfe_fehler_5de_werkstattcode < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_fehler_5de_werkstattcode;
    if (raw_lenkhilfe_fehler_5de_werkstattcode > 0xFFULL) raw_lenkhilfe_fehler_5de_werkstattcode = 0xFFULL;
    writeBitsLE(frame.data, 48, 8, (uint32_t)raw_lenkhilfe_fehler_5de_werkstattcode);
    // Multiplex_Signal
    int64_t raw_tmp_lenkhilfe_fehler_5de_multiplex_signal = (int64_t)llround((params::vw_pq.lenkhilfe_fehler_5de_multiplex_signal - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_fehler_5de_multiplex_signal = raw_tmp_lenkhilfe_fehler_5de_multiplex_signal < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_fehler_5de_multiplex_signal;
    if (raw_lenkhilfe_fehler_5de_multiplex_signal > 0x7ULL) raw_lenkhilfe_fehler_5de_multiplex_signal = 0x7ULL;
    writeBitsLE(frame.data, 45, 3, (uint32_t)raw_lenkhilfe_fehler_5de_multiplex_signal);
    writeBitsLE(frame.data, 44, 1, params::vw_pq.lenkhilfe_fehler_5de_sensorcodierung_lenkhilfe ? 1U : 0U);
    // Kennliniencodierung_Lenkhilfe
    int64_t raw_tmp_lenkhilfe_fehler_5de_kennliniencodierung_lenkhilfe = (int64_t)llround((params::vw_pq.lenkhilfe_fehler_5de_kennliniencodierung_lenkhilfe - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_fehler_5de_kennliniencodierung_lenkhilfe = raw_tmp_lenkhilfe_fehler_5de_kennliniencodierung_lenkhilfe < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_fehler_5de_kennliniencodierung_lenkhilfe;
    if (raw_lenkhilfe_fehler_5de_kennliniencodierung_lenkhilfe > 0xFULL) raw_lenkhilfe_fehler_5de_kennliniencodierung_lenkhilfe = 0xFULL;
    writeBitsLE(frame.data, 40, 4, (uint32_t)raw_lenkhilfe_fehler_5de_kennliniencodierung_lenkhilfe);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.lenkhilfe_fehler_5de_geber_f_lenkw_speicher_ausles ? 1U : 0U);
    writeBitsLE(frame.data, 38, 1, params::vw_pq.lenkhilfe_fehler_5de_kombiinstr_speicher_auslesen ? 1U : 0U);
    writeBitsLE(frame.data, 37, 1, params::vw_pq.lenkhilfe_fehler_5de_steuerger_speicher_auslesen ? 1U : 0U);
    writeBitsLE(frame.data, 36, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_steuergeraet_defekt ? 1U : 0U);
    // Frei_Lenkhilfe_1_3
    int64_t raw_tmp_lenkhilfe_fehler_5de_frei_lenkhilfe_1_3 = (int64_t)llround((params::vw_pq.lenkhilfe_fehler_5de_frei_lenkhilfe_1_3 - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_3 = raw_tmp_lenkhilfe_fehler_5de_frei_lenkhilfe_1_3 < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_fehler_5de_frei_lenkhilfe_1_3;
    if (raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_3 > 0xFULL) raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_3 = 0xFULL;
    writeBitsLE(frame.data, 32, 4, (uint32_t)raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_3);
    // Frei_Lenkhilfe_1_4
    int64_t raw_tmp_lenkhilfe_fehler_5de_frei_lenkhilfe_1_4 = (int64_t)llround((params::vw_pq.lenkhilfe_fehler_5de_frei_lenkhilfe_1_4 - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_4 = raw_tmp_lenkhilfe_fehler_5de_frei_lenkhilfe_1_4 < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_fehler_5de_frei_lenkhilfe_1_4;
    if (raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_4 > 0x3ULL) raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_4 = 0x3ULL;
    writeBitsLE(frame.data, 30, 2, (uint32_t)raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_4);
    writeBitsLE(frame.data, 29, 1, params::vw_pq.lenkhilfe_fehler_5de_geber_f_lenkwi_k_can_komm_sp ? 1U : 0U);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.lenkhilfe_fehler_5de_geber_f_lenkwinkel_k_can_komm ? 1U : 0U);
    writeBitsLE(frame.data, 27, 1, params::vw_pq.lenkhilfe_fehler_5de_kombiinstr_k_can_kommunik_sp ? 1U : 0U);
    writeBitsLE(frame.data, 26, 1, params::vw_pq.lenkhilfe_fehler_5de_kombiinstrument_k_can_kommunik ? 1U : 0U);
    writeBitsLE(frame.data, 25, 1, params::vw_pq.lenkhilfe_fehler_5de_steuergeraet_k_can_kommunik_s ? 1U : 0U);
    writeBitsLE(frame.data, 24, 1, params::vw_pq.lenkhilfe_fehler_5de_steuergeraet_keine_can_kommunik ? 1U : 0U);
    writeBitsLE(frame.data, 23, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_spannung_ks_nach_mas ? 1U : 0U);
    writeBitsLE(frame.data, 22, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_spannung_ks_nach_mass ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_spannung_ks_nach_s ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_spannung_ks_nach ? 1U : 0U);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_temperaturschutz_sp ? 1U : 0U);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_temperaturschutz ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_betrieb_unpl_sig_sp ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfe_betrieb_unpl_signal ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_k_kommunik_sp ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_keine_kommunik ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_defekt_sp ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_defekt ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_unterbrechung_s ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_unterbrechung ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhilfesensor_ks_nach_masse_s ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.lenkhilfe_fehler_5de_lenkhifesensor_ks_nach_masse ? 1U : 0U);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.lenkhilfe_fehler_5de_spannung_kl_15_zu_klein_sp ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.lenkhilfe_fehler_5de_spannung_kl_15_zu_klein ? 1U : 0U);
    // Frei_Lenkhilfe_1_5
    int64_t raw_tmp_lenkhilfe_fehler_5de_frei_lenkhilfe_1_5 = (int64_t)llround((params::vw_pq.lenkhilfe_fehler_5de_frei_lenkhilfe_1_5 - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_5 = raw_tmp_lenkhilfe_fehler_5de_frei_lenkhilfe_1_5 < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_fehler_5de_frei_lenkhilfe_1_5;
    if (raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_5 > 0x3ULL) raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_5 = 0x3ULL;
    writeBitsLE(frame.data, 4, 2, (uint32_t)raw_lenkhilfe_fehler_5de_frei_lenkhilfe_1_5);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.lenkhilfe_fehler_5de_spannung_kl_30_zu_klein_sp ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.lenkhilfe_fehler_5de_spannung_kl_30_zu_klein ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.lenkhilfe_fehler_5de_spannung_kl_30_zu_gro_sp ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.lenkhilfe_fehler_5de_spannung_kl_30_zu_gro ? 1U : 0U);
}

// Message 0x3D0 (976) - Lenkhilfe_1
// DLC: 2, TX: XXX
bool decodeVwPq0x3D0(const CANMessage &frame) {
    if (frame.id != 0x3D0 || frame.len < 2) {
        return false;
    }

    // LH1_Lastinfo: start=0 len=7 endian=1 sign=+ factor=1 offset=0 unit="A"
    const uint32_t raw_lenkhilfe_1_3d0_lh1_lastinfo = readBitsLE(frame.data, 0, 7);
    params::vw_pq.lenkhilfe_1_3d0_lh1_lastinfo = raw_lenkhilfe_1_3d0_lh1_lastinfo * 1.0f + 0.0f;
    // LH1_Fehler_LI: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_fehler_li = readBitsLE(frame.data, 7, 1) != 0;
    // LH1_Sicherheitslamp: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_sicherheitslamp = readBitsLE(frame.data, 8, 1) != 0;
    // LH1_Fehlerlampe: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_fehlerlampe = readBitsLE(frame.data, 9, 1) != 0;
    // LH1_Textbits: start=10 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_1_3d0_lh1_textbits = readBitsLE(frame.data, 10, 3);
    params::vw_pq.lenkhilfe_1_3d0_lh1_textbits = raw_lenkhilfe_1_3d0_lh1_textbits * 1.0f + 0.0f;
    // LH1_Akustiksign: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_akustiksign = readBitsLE(frame.data, 13, 1) != 0;
    // LH1_SleepInd: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_sleepind = readBitsLE(frame.data, 14, 1) != 0;
    // LH1_Fehlereintr: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_fehlereintr = readBitsLE(frame.data, 15, 1) != 0;
    // LH1_EPS_Diagmode: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_eps_diagmode = readBitsLE(frame.data, 16, 1) != 0;
    // LH1_gue_ECU_Temp: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_gue_ecu_temp = readBitsLE(frame.data, 19, 1) != 0;
    // LH1_Kuehlung: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_kuehlung = readBitsLE(frame.data, 20, 1) != 0;
    // LH1_Mode_Hybrid: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_mode_hybrid = readBitsLE(frame.data, 21, 1) != 0;
    // LH1_NL_Untersp: start=24 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_nl_untersp = readBitsLE(frame.data, 24, 1) != 0;
    // LH1_NL_Uebersp: start=25 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_nl_uebersp = readBitsLE(frame.data, 25, 1) != 0;
    // LH1_NL_Uebertemp: start=26 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_nl_uebertemp = readBitsLE(frame.data, 26, 1) != 0;
    // LH1_NL_Sensor: start=27 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_nl_sensor = readBitsLE(frame.data, 27, 1) != 0;
    // LH1_NL_CAN_SS: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_nl_can_ss = readBitsLE(frame.data, 28, 1) != 0;
    // LH1_NL_ECU: start=29 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_nl_ecu = readBitsLE(frame.data, 29, 1) != 0;
    // LH1_NL_Motor: start=30 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_nl_motor = readBitsLE(frame.data, 30, 1) != 0;
    // LH1_NL_LeistDichte: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_1_3d0_lh1_nl_leistdichte = readBitsLE(frame.data, 31, 1) != 0;
    // LH1_ULeistung: start=32 len=8 endian=1 sign=+ factor=0.5 offset=0 unit="%"
    const uint32_t raw_lenkhilfe_1_3d0_lh1_uleistung = readBitsLE(frame.data, 32, 8);
    params::vw_pq.lenkhilfe_1_3d0_lh1_uleistung = raw_lenkhilfe_1_3d0_lh1_uleistung * 0.5f + 0.0f;
    // LH1_ECU_Temp: start=40 len=8 endian=1 sign=+ factor=1 offset=-70 unit="C"
    const uint32_t raw_lenkhilfe_1_3d0_lh1_ecu_temp = readBitsLE(frame.data, 40, 8);
    params::vw_pq.lenkhilfe_1_3d0_lh1_ecu_temp = raw_lenkhilfe_1_3d0_lh1_ecu_temp * 1.0f + -70.0f;
    return true;
}

void encodeVwPq0x3D0(CANMessage &frame) {
    frame.id = 0x3D0;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // LH1_Lastinfo
    int64_t raw_tmp_lenkhilfe_1_3d0_lh1_lastinfo = (int64_t)llround((params::vw_pq.lenkhilfe_1_3d0_lh1_lastinfo - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_1_3d0_lh1_lastinfo = raw_tmp_lenkhilfe_1_3d0_lh1_lastinfo < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_1_3d0_lh1_lastinfo;
    if (raw_lenkhilfe_1_3d0_lh1_lastinfo > 0x7FULL) raw_lenkhilfe_1_3d0_lh1_lastinfo = 0x7FULL;
    writeBitsLE(frame.data, 0, 7, (uint32_t)raw_lenkhilfe_1_3d0_lh1_lastinfo);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_fehler_li ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_sicherheitslamp ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_fehlerlampe ? 1U : 0U);
    // LH1_Textbits
    int64_t raw_tmp_lenkhilfe_1_3d0_lh1_textbits = (int64_t)llround((params::vw_pq.lenkhilfe_1_3d0_lh1_textbits - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_1_3d0_lh1_textbits = raw_tmp_lenkhilfe_1_3d0_lh1_textbits < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_1_3d0_lh1_textbits;
    if (raw_lenkhilfe_1_3d0_lh1_textbits > 0x7ULL) raw_lenkhilfe_1_3d0_lh1_textbits = 0x7ULL;
    writeBitsLE(frame.data, 10, 3, (uint32_t)raw_lenkhilfe_1_3d0_lh1_textbits);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_akustiksign ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_sleepind ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_fehlereintr ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_eps_diagmode ? 1U : 0U);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_gue_ecu_temp ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_kuehlung ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_mode_hybrid ? 1U : 0U);
    writeBitsLE(frame.data, 24, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_nl_untersp ? 1U : 0U);
    writeBitsLE(frame.data, 25, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_nl_uebersp ? 1U : 0U);
    writeBitsLE(frame.data, 26, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_nl_uebertemp ? 1U : 0U);
    writeBitsLE(frame.data, 27, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_nl_sensor ? 1U : 0U);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_nl_can_ss ? 1U : 0U);
    writeBitsLE(frame.data, 29, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_nl_ecu ? 1U : 0U);
    writeBitsLE(frame.data, 30, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_nl_motor ? 1U : 0U);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.lenkhilfe_1_3d0_lh1_nl_leistdichte ? 1U : 0U);
    // LH1_ULeistung
    int64_t raw_tmp_lenkhilfe_1_3d0_lh1_uleistung = (int64_t)llround((params::vw_pq.lenkhilfe_1_3d0_lh1_uleistung - 0.0f) / 0.5f);
    uint64_t raw_lenkhilfe_1_3d0_lh1_uleistung = raw_tmp_lenkhilfe_1_3d0_lh1_uleistung < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_1_3d0_lh1_uleistung;
    if (raw_lenkhilfe_1_3d0_lh1_uleistung > 0xFFULL) raw_lenkhilfe_1_3d0_lh1_uleistung = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_lenkhilfe_1_3d0_lh1_uleistung);
    // LH1_ECU_Temp
    int64_t raw_tmp_lenkhilfe_1_3d0_lh1_ecu_temp = (int64_t)llround((params::vw_pq.lenkhilfe_1_3d0_lh1_ecu_temp - -70.0f) / 1.0f);
    uint64_t raw_lenkhilfe_1_3d0_lh1_ecu_temp = raw_tmp_lenkhilfe_1_3d0_lh1_ecu_temp < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_1_3d0_lh1_ecu_temp;
    if (raw_lenkhilfe_1_3d0_lh1_ecu_temp > 0xFFULL) raw_lenkhilfe_1_3d0_lh1_ecu_temp = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_lenkhilfe_1_3d0_lh1_ecu_temp);
}

// Message 0x520 (1312) - Kombi_3
// DLC: 8, TX: XXX
bool decodeVwPq0x520(const CANMessage &frame) {
    if (frame.id != 0x520 || frame.len < 8) {
        return false;
    }

    // Frei_Kombi_3_2: start=60 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_3_520_frei_kombi_3_2 = readBitsLE(frame.data, 60, 4);
    params::vw_pq.kombi_3_520_frei_kombi_3_2 = raw_kombi_3_520_frei_kombi_3_2 * 1.0f + 0.0f;
    // Kilometerstand: start=40 len=20 endian=1 sign=+ factor=1 offset=0 unit="km"
    const uint32_t raw_kombi_3_520_kilometerstand = readBitsLE(frame.data, 40, 20);
    params::vw_pq.kombi_3_520_kilometerstand = raw_kombi_3_520_kilometerstand * 1.0f + 0.0f;
    // Fehlerstatus_Standzeit: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_3_520_fehlerstatus_standzeit = readBitsLE(frame.data, 39, 1) != 0;
    // Standzeit: start=24 len=15 endian=1 sign=+ factor=4 offset=0 unit="sec"
    const uint32_t raw_kombi_3_520_standzeit = readBitsLE(frame.data, 24, 15);
    params::vw_pq.kombi_3_520_standzeit = raw_kombi_3_520_standzeit * 4.0f + 0.0f;
    // Frei_Kombi_3_1: start=20 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_3_520_frei_kombi_3_1 = readBitsLE(frame.data, 20, 4);
    params::vw_pq.kombi_3_520_frei_kombi_3_1 = raw_kombi_3_520_frei_kombi_3_1 * 1.0f + 0.0f;
    // Schluesselinfo: start=16 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_3_520_schluesselinfo = readBitsLE(frame.data, 16, 4);
    params::vw_pq.kombi_3_520_schluesselinfo = raw_kombi_3_520_schluesselinfo * 1.0f + 0.0f;
    // Kombi_Multiplex_Code M: start=14 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_3_520_kombi_multiplex_code_m = readBitsLE(frame.data, 14, 2);
    params::vw_pq.kombi_3_520_kombi_multiplex_code_m = raw_kombi_3_520_kombi_multiplex_code_m * 1.0f + 0.0f;
    // Kombi_Verbauliste_Niveauregulie m3: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_3_520_kombi_verbauliste_niveauregulie_m3 = readBitsLE(frame.data, 11, 1) != 0;
    // Kombi_Multiplex_Marke m1: start=11 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_3_520_kombi_multiplex_marke_m1 = readBitsLE(frame.data, 11, 3);
    params::vw_pq.kombi_3_520_kombi_multiplex_marke_m1 = raw_kombi_3_520_kombi_multiplex_marke_m1 * 1.0f + 0.0f;
    // Kombi_Verbauliste_Lenkhilfe m3: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_3_520_kombi_verbauliste_lenkhilfe_m3 = readBitsLE(frame.data, 10, 1) != 0;
    // Kombi_Verbauliste_Dieselpumpe m3: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_3_520_kombi_verbauliste_dieselpumpe_m3 = readBitsLE(frame.data, 9, 1) != 0;
    // Kombi_Verbauliste_Lenkwinkel m3: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_3_520_kombi_verbauliste_lenkwinkel_m3 = readBitsLE(frame.data, 8, 1) != 0;
    // Kombi_Multiplex_Baureihe m1: start=8 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_3_520_kombi_multiplex_baureihe_m1 = readBitsLE(frame.data, 8, 3);
    params::vw_pq.kombi_3_520_kombi_multiplex_baureihe_m1 = raw_kombi_3_520_kombi_multiplex_baureihe_m1 * 1.0f + 0.0f;
    // Kombi_Multiplex_Laendervariante m0: start=8 len=6 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_3_520_kombi_multiplex_laendervariante_m0 = readBitsLE(frame.data, 8, 6);
    params::vw_pq.kombi_3_520_kombi_multiplex_laendervariante_m0 = raw_kombi_3_520_kombi_multiplex_laendervariante_m0 * 1.0f + 0.0f;
    // Kombi_Verbauliste_Allrad m3: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_3_520_kombi_verbauliste_allrad_m3 = readBitsLE(frame.data, 7, 1) != 0;
    // Kombi_Verbauliste_Bordnetz m3: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_3_520_kombi_verbauliste_bordnetz_m3 = readBitsLE(frame.data, 6, 1) != 0;
    // Kombi_Verbauliste_ACC m3: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_3_520_kombi_verbauliste_acc_m3 = readBitsLE(frame.data, 5, 1) != 0;
    // Kombi_Verbauliste_Airbag m3: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_3_520_kombi_verbauliste_airbag_m3 = readBitsLE(frame.data, 4, 1) != 0;
    // Kombi_Multiplex_Generation m1: start=4 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_3_520_kombi_multiplex_generation_m1 = readBitsLE(frame.data, 4, 4);
    params::vw_pq.kombi_3_520_kombi_multiplex_generation_m1 = raw_kombi_3_520_kombi_multiplex_generation_m1 * 1.0f + 0.0f;
    // Kombi_Verbauliste_Klima m3: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_3_520_kombi_verbauliste_klima_m3 = readBitsLE(frame.data, 3, 1) != 0;
    // Kombi_Verbauliste_ABS m3: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_3_520_kombi_verbauliste_abs_m3 = readBitsLE(frame.data, 2, 1) != 0;
    // Kombi_Verbauliste_Motor m3: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_3_520_kombi_verbauliste_motor_m3 = readBitsLE(frame.data, 0, 1) != 0;
    // Kombi_Multiplex_Derivat m1: start=0 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_3_520_kombi_multiplex_derivat_m1 = readBitsLE(frame.data, 0, 4);
    params::vw_pq.kombi_3_520_kombi_multiplex_derivat_m1 = raw_kombi_3_520_kombi_multiplex_derivat_m1 * 1.0f + 0.0f;
    // Kombi_Multiplex_Sprachvariante m0: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_3_520_kombi_multiplex_sprachvariante_m0 = readBitsLE(frame.data, 0, 8);
    params::vw_pq.kombi_3_520_kombi_multiplex_sprachvariante_m0 = raw_kombi_3_520_kombi_multiplex_sprachvariante_m0 * 1.0f + 0.0f;
    // Kombi_Multiplex_Reifenumfang m2: start=0 len=12 endian=1 sign=+ factor=1 offset=0 unit="mm"
    const uint32_t raw_kombi_3_520_kombi_multiplex_reifenumfang_m2 = readBitsLE(frame.data, 0, 12);
    params::vw_pq.kombi_3_520_kombi_multiplex_reifenumfang_m2 = raw_kombi_3_520_kombi_multiplex_reifenumfang_m2 * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x520(CANMessage &frame) {
    frame.id = 0x520;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Frei_Kombi_3_2
    int64_t raw_tmp_kombi_3_520_frei_kombi_3_2 = (int64_t)llround((params::vw_pq.kombi_3_520_frei_kombi_3_2 - 0.0f) / 1.0f);
    uint64_t raw_kombi_3_520_frei_kombi_3_2 = raw_tmp_kombi_3_520_frei_kombi_3_2 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_frei_kombi_3_2;
    if (raw_kombi_3_520_frei_kombi_3_2 > 0xFULL) raw_kombi_3_520_frei_kombi_3_2 = 0xFULL;
    writeBitsLE(frame.data, 60, 4, (uint32_t)raw_kombi_3_520_frei_kombi_3_2);
    // Kilometerstand
    int64_t raw_tmp_kombi_3_520_kilometerstand = (int64_t)llround((params::vw_pq.kombi_3_520_kilometerstand - 0.0f) / 1.0f);
    uint64_t raw_kombi_3_520_kilometerstand = raw_tmp_kombi_3_520_kilometerstand < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_kilometerstand;
    if (raw_kombi_3_520_kilometerstand > 0xFFFFFULL) raw_kombi_3_520_kilometerstand = 0xFFFFFULL;
    writeBitsLE(frame.data, 40, 20, (uint32_t)raw_kombi_3_520_kilometerstand);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.kombi_3_520_fehlerstatus_standzeit ? 1U : 0U);
    // Standzeit
    int64_t raw_tmp_kombi_3_520_standzeit = (int64_t)llround((params::vw_pq.kombi_3_520_standzeit - 0.0f) / 4.0f);
    uint64_t raw_kombi_3_520_standzeit = raw_tmp_kombi_3_520_standzeit < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_standzeit;
    if (raw_kombi_3_520_standzeit > 0x7FFFULL) raw_kombi_3_520_standzeit = 0x7FFFULL;
    writeBitsLE(frame.data, 24, 15, (uint32_t)raw_kombi_3_520_standzeit);
    // Frei_Kombi_3_1
    int64_t raw_tmp_kombi_3_520_frei_kombi_3_1 = (int64_t)llround((params::vw_pq.kombi_3_520_frei_kombi_3_1 - 0.0f) / 1.0f);
    uint64_t raw_kombi_3_520_frei_kombi_3_1 = raw_tmp_kombi_3_520_frei_kombi_3_1 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_frei_kombi_3_1;
    if (raw_kombi_3_520_frei_kombi_3_1 > 0xFULL) raw_kombi_3_520_frei_kombi_3_1 = 0xFULL;
    writeBitsLE(frame.data, 20, 4, (uint32_t)raw_kombi_3_520_frei_kombi_3_1);
    // Schluesselinfo
    int64_t raw_tmp_kombi_3_520_schluesselinfo = (int64_t)llround((params::vw_pq.kombi_3_520_schluesselinfo - 0.0f) / 1.0f);
    uint64_t raw_kombi_3_520_schluesselinfo = raw_tmp_kombi_3_520_schluesselinfo < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_schluesselinfo;
    if (raw_kombi_3_520_schluesselinfo > 0xFULL) raw_kombi_3_520_schluesselinfo = 0xFULL;
    writeBitsLE(frame.data, 16, 4, (uint32_t)raw_kombi_3_520_schluesselinfo);
    // Kombi_Multiplex_Code M
    int64_t raw_tmp_kombi_3_520_kombi_multiplex_code_m = (int64_t)llround((params::vw_pq.kombi_3_520_kombi_multiplex_code_m - 0.0f) / 1.0f);
    uint64_t raw_kombi_3_520_kombi_multiplex_code_m = raw_tmp_kombi_3_520_kombi_multiplex_code_m < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_kombi_multiplex_code_m;
    if (raw_kombi_3_520_kombi_multiplex_code_m > 0x3ULL) raw_kombi_3_520_kombi_multiplex_code_m = 0x3ULL;
    writeBitsLE(frame.data, 14, 2, (uint32_t)raw_kombi_3_520_kombi_multiplex_code_m);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.kombi_3_520_kombi_verbauliste_niveauregulie_m3 ? 1U : 0U);
    // Kombi_Multiplex_Marke m1
    int64_t raw_tmp_kombi_3_520_kombi_multiplex_marke_m1 = (int64_t)llround((params::vw_pq.kombi_3_520_kombi_multiplex_marke_m1 - 0.0f) / 1.0f);
    uint64_t raw_kombi_3_520_kombi_multiplex_marke_m1 = raw_tmp_kombi_3_520_kombi_multiplex_marke_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_kombi_multiplex_marke_m1;
    if (raw_kombi_3_520_kombi_multiplex_marke_m1 > 0x7ULL) raw_kombi_3_520_kombi_multiplex_marke_m1 = 0x7ULL;
    writeBitsLE(frame.data, 11, 3, (uint32_t)raw_kombi_3_520_kombi_multiplex_marke_m1);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.kombi_3_520_kombi_verbauliste_lenkhilfe_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.kombi_3_520_kombi_verbauliste_dieselpumpe_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.kombi_3_520_kombi_verbauliste_lenkwinkel_m3 ? 1U : 0U);
    // Kombi_Multiplex_Baureihe m1
    int64_t raw_tmp_kombi_3_520_kombi_multiplex_baureihe_m1 = (int64_t)llround((params::vw_pq.kombi_3_520_kombi_multiplex_baureihe_m1 - 0.0f) / 1.0f);
    uint64_t raw_kombi_3_520_kombi_multiplex_baureihe_m1 = raw_tmp_kombi_3_520_kombi_multiplex_baureihe_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_kombi_multiplex_baureihe_m1;
    if (raw_kombi_3_520_kombi_multiplex_baureihe_m1 > 0x7ULL) raw_kombi_3_520_kombi_multiplex_baureihe_m1 = 0x7ULL;
    writeBitsLE(frame.data, 8, 3, (uint32_t)raw_kombi_3_520_kombi_multiplex_baureihe_m1);
    // Kombi_Multiplex_Laendervariante m0
    int64_t raw_tmp_kombi_3_520_kombi_multiplex_laendervariante_m0 = (int64_t)llround((params::vw_pq.kombi_3_520_kombi_multiplex_laendervariante_m0 - 0.0f) / 1.0f);
    uint64_t raw_kombi_3_520_kombi_multiplex_laendervariante_m0 = raw_tmp_kombi_3_520_kombi_multiplex_laendervariante_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_kombi_multiplex_laendervariante_m0;
    if (raw_kombi_3_520_kombi_multiplex_laendervariante_m0 > 0x3FULL) raw_kombi_3_520_kombi_multiplex_laendervariante_m0 = 0x3FULL;
    writeBitsLE(frame.data, 8, 6, (uint32_t)raw_kombi_3_520_kombi_multiplex_laendervariante_m0);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.kombi_3_520_kombi_verbauliste_allrad_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.kombi_3_520_kombi_verbauliste_bordnetz_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.kombi_3_520_kombi_verbauliste_acc_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.kombi_3_520_kombi_verbauliste_airbag_m3 ? 1U : 0U);
    // Kombi_Multiplex_Generation m1
    int64_t raw_tmp_kombi_3_520_kombi_multiplex_generation_m1 = (int64_t)llround((params::vw_pq.kombi_3_520_kombi_multiplex_generation_m1 - 0.0f) / 1.0f);
    uint64_t raw_kombi_3_520_kombi_multiplex_generation_m1 = raw_tmp_kombi_3_520_kombi_multiplex_generation_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_kombi_multiplex_generation_m1;
    if (raw_kombi_3_520_kombi_multiplex_generation_m1 > 0xFULL) raw_kombi_3_520_kombi_multiplex_generation_m1 = 0xFULL;
    writeBitsLE(frame.data, 4, 4, (uint32_t)raw_kombi_3_520_kombi_multiplex_generation_m1);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.kombi_3_520_kombi_verbauliste_klima_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.kombi_3_520_kombi_verbauliste_abs_m3 ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.kombi_3_520_kombi_verbauliste_motor_m3 ? 1U : 0U);
    // Kombi_Multiplex_Derivat m1
    int64_t raw_tmp_kombi_3_520_kombi_multiplex_derivat_m1 = (int64_t)llround((params::vw_pq.kombi_3_520_kombi_multiplex_derivat_m1 - 0.0f) / 1.0f);
    uint64_t raw_kombi_3_520_kombi_multiplex_derivat_m1 = raw_tmp_kombi_3_520_kombi_multiplex_derivat_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_kombi_multiplex_derivat_m1;
    if (raw_kombi_3_520_kombi_multiplex_derivat_m1 > 0xFULL) raw_kombi_3_520_kombi_multiplex_derivat_m1 = 0xFULL;
    writeBitsLE(frame.data, 0, 4, (uint32_t)raw_kombi_3_520_kombi_multiplex_derivat_m1);
    // Kombi_Multiplex_Sprachvariante m0
    int64_t raw_tmp_kombi_3_520_kombi_multiplex_sprachvariante_m0 = (int64_t)llround((params::vw_pq.kombi_3_520_kombi_multiplex_sprachvariante_m0 - 0.0f) / 1.0f);
    uint64_t raw_kombi_3_520_kombi_multiplex_sprachvariante_m0 = raw_tmp_kombi_3_520_kombi_multiplex_sprachvariante_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_kombi_multiplex_sprachvariante_m0;
    if (raw_kombi_3_520_kombi_multiplex_sprachvariante_m0 > 0xFFULL) raw_kombi_3_520_kombi_multiplex_sprachvariante_m0 = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_kombi_3_520_kombi_multiplex_sprachvariante_m0);
    // Kombi_Multiplex_Reifenumfang m2
    int64_t raw_tmp_kombi_3_520_kombi_multiplex_reifenumfang_m2 = (int64_t)llround((params::vw_pq.kombi_3_520_kombi_multiplex_reifenumfang_m2 - 0.0f) / 1.0f);
    uint64_t raw_kombi_3_520_kombi_multiplex_reifenumfang_m2 = raw_tmp_kombi_3_520_kombi_multiplex_reifenumfang_m2 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_3_520_kombi_multiplex_reifenumfang_m2;
    if (raw_kombi_3_520_kombi_multiplex_reifenumfang_m2 > 0xFFFULL) raw_kombi_3_520_kombi_multiplex_reifenumfang_m2 = 0xFFFULL;
    writeBitsLE(frame.data, 0, 12, (uint32_t)raw_kombi_3_520_kombi_multiplex_reifenumfang_m2);
}

// Message 0x420 (1056) - Kombi_2
// DLC: 8, TX: XXX
bool decodeVwPq0x420(const CANMessage &frame) {
    if (frame.id != 0x420 || frame.len < 8) {
        return false;
    }

    // Frei_Kombi_2_2: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_2_420_frei_kombi_2_2 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.kombi_2_420_frei_kombi_2_2 = raw_kombi_2_420_frei_kombi_2_2 * 1.0f + 0.0f;
    // Fehlerstatus_Kl__58_s: start=55 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_2_420_fehlerstatus_kl_58_s = readBitsLE(frame.data, 55, 1) != 0;
    // Klemme_58s__Kombi_2_: start=48 len=7 endian=1 sign=+ factor=1 offset=0 unit="%"
    const uint32_t raw_kombi_2_420_klemme_58s_kombi_2 = readBitsLE(frame.data, 48, 7);
    params::vw_pq.kombi_2_420_klemme_58s_kombi_2 = raw_kombi_2_420_klemme_58s_kombi_2 * 1.0f + 0.0f;
    // Fehlerstatus_Kl__58_d: start=47 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_2_420_fehlerstatus_kl_58_d = readBitsLE(frame.data, 47, 1) != 0;
    // Klemme_58d__Kombi_2_: start=40 len=7 endian=1 sign=+ factor=1 offset=0 unit="%"
    const uint32_t raw_kombi_2_420_klemme_58d_kombi_2 = readBitsLE(frame.data, 40, 7);
    params::vw_pq.kombi_2_420_klemme_58d_kombi_2 = raw_kombi_2_420_klemme_58d_kombi_2 * 1.0f + 0.0f;
    // Kuehlmitteltemp__4_1__Kombi_2_: start=32 len=8 endian=1 sign=+ factor=0.75 offset=-48 unit="C"
    // DBC comment: kombi coolant temperature
    const uint32_t raw_kombi_2_420_kuehlmitteltemp_4_1_kombi_2 = readBitsLE(frame.data, 32, 8);
    params::vw_pq.kombi_2_420_kuehlmitteltemp_4_1_kombi_2 = raw_kombi_2_420_kuehlmitteltemp_4_1_kombi_2 * 0.75f + -48.0f;
    // Oeltemperatur_4_1: start=24 len=8 endian=1 sign=+ factor=1 offset=-60 unit="C"
    // DBC comment: kombi oil temperature
    const uint32_t raw_kombi_2_420_oeltemperatur_4_1 = readBitsLE(frame.data, 24, 8);
    params::vw_pq.kombi_2_420_oeltemperatur_4_1 = raw_kombi_2_420_oeltemperatur_4_1 * 1.0f + -60.0f;
    // Aussentemp__ungefiltert_4_1__Ko: start=16 len=8 endian=1 sign=+ factor=0.5 offset=-50 unit="C"
    const uint32_t raw_kombi_2_420_aussentemp_ungefiltert_4_1_ko = readBitsLE(frame.data, 16, 8);
    params::vw_pq.kombi_2_420_aussentemp_ungefiltert_4_1_ko = raw_kombi_2_420_aussentemp_ungefiltert_4_1_ko * 0.5f + -50.0f;
    // Aussentemperatur_gefiltert: start=8 len=8 endian=1 sign=+ factor=0.5 offset=-50 unit="C"
    // DBC comment: outside temp, filtered
    const uint32_t raw_kombi_2_420_aussentemperatur_gefiltert = readBitsLE(frame.data, 8, 8);
    params::vw_pq.kombi_2_420_aussentemperatur_gefiltert = raw_kombi_2_420_aussentemperatur_gefiltert * 0.5f + -50.0f;
    // Fehlerspeichereintrag__Kombi_: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_2_420_fehlerspeichereintrag_kombi = readBitsLE(frame.data, 7, 1) != 0;
    // Frei_Kombi_2_1: start=4 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_2_420_frei_kombi_2_1 = readBitsLE(frame.data, 4, 3);
    params::vw_pq.kombi_2_420_frei_kombi_2_1 = raw_kombi_2_420_frei_kombi_2_1 * 1.0f + 0.0f;
    // Anhaenger_erkannt: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_2_420_anhaenger_erkannt = readBitsLE(frame.data, 3, 1) != 0;
    // Fehlerst__Kuehlmitteltemp__4_1: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: water temp error
    params::vw_pq.kombi_2_420_fehlerst_kuehlmitteltemp_4_1 = readBitsLE(frame.data, 2, 1) != 0;
    // Fehlerstatus_Oeltemperatur_4_1: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: oil temp error
    params::vw_pq.kombi_2_420_fehlerstatus_oeltemperatur_4_1 = readBitsLE(frame.data, 1, 1) != 0;
    // Fehlerstatus_Aussentemp__4_1: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: ambient temp error
    params::vw_pq.kombi_2_420_fehlerstatus_aussentemp_4_1 = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x420(CANMessage &frame) {
    frame.id = 0x420;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Frei_Kombi_2_2
    int64_t raw_tmp_kombi_2_420_frei_kombi_2_2 = (int64_t)llround((params::vw_pq.kombi_2_420_frei_kombi_2_2 - 0.0f) / 1.0f);
    uint64_t raw_kombi_2_420_frei_kombi_2_2 = raw_tmp_kombi_2_420_frei_kombi_2_2 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_2_420_frei_kombi_2_2;
    if (raw_kombi_2_420_frei_kombi_2_2 > 0xFFULL) raw_kombi_2_420_frei_kombi_2_2 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_kombi_2_420_frei_kombi_2_2);
    writeBitsLE(frame.data, 55, 1, params::vw_pq.kombi_2_420_fehlerstatus_kl_58_s ? 1U : 0U);
    // Klemme_58s__Kombi_2_
    int64_t raw_tmp_kombi_2_420_klemme_58s_kombi_2 = (int64_t)llround((params::vw_pq.kombi_2_420_klemme_58s_kombi_2 - 0.0f) / 1.0f);
    uint64_t raw_kombi_2_420_klemme_58s_kombi_2 = raw_tmp_kombi_2_420_klemme_58s_kombi_2 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_2_420_klemme_58s_kombi_2;
    if (raw_kombi_2_420_klemme_58s_kombi_2 > 0x7FULL) raw_kombi_2_420_klemme_58s_kombi_2 = 0x7FULL;
    writeBitsLE(frame.data, 48, 7, (uint32_t)raw_kombi_2_420_klemme_58s_kombi_2);
    writeBitsLE(frame.data, 47, 1, params::vw_pq.kombi_2_420_fehlerstatus_kl_58_d ? 1U : 0U);
    // Klemme_58d__Kombi_2_
    int64_t raw_tmp_kombi_2_420_klemme_58d_kombi_2 = (int64_t)llround((params::vw_pq.kombi_2_420_klemme_58d_kombi_2 - 0.0f) / 1.0f);
    uint64_t raw_kombi_2_420_klemme_58d_kombi_2 = raw_tmp_kombi_2_420_klemme_58d_kombi_2 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_2_420_klemme_58d_kombi_2;
    if (raw_kombi_2_420_klemme_58d_kombi_2 > 0x7FULL) raw_kombi_2_420_klemme_58d_kombi_2 = 0x7FULL;
    writeBitsLE(frame.data, 40, 7, (uint32_t)raw_kombi_2_420_klemme_58d_kombi_2);
    // Kuehlmitteltemp__4_1__Kombi_2_
    int64_t raw_tmp_kombi_2_420_kuehlmitteltemp_4_1_kombi_2 = (int64_t)llround((params::vw_pq.kombi_2_420_kuehlmitteltemp_4_1_kombi_2 - -48.0f) / 0.75f);
    uint64_t raw_kombi_2_420_kuehlmitteltemp_4_1_kombi_2 = raw_tmp_kombi_2_420_kuehlmitteltemp_4_1_kombi_2 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_2_420_kuehlmitteltemp_4_1_kombi_2;
    if (raw_kombi_2_420_kuehlmitteltemp_4_1_kombi_2 > 0xFFULL) raw_kombi_2_420_kuehlmitteltemp_4_1_kombi_2 = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_kombi_2_420_kuehlmitteltemp_4_1_kombi_2);
    // Oeltemperatur_4_1
    int64_t raw_tmp_kombi_2_420_oeltemperatur_4_1 = (int64_t)llround((params::vw_pq.kombi_2_420_oeltemperatur_4_1 - -60.0f) / 1.0f);
    uint64_t raw_kombi_2_420_oeltemperatur_4_1 = raw_tmp_kombi_2_420_oeltemperatur_4_1 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_2_420_oeltemperatur_4_1;
    if (raw_kombi_2_420_oeltemperatur_4_1 > 0xFFULL) raw_kombi_2_420_oeltemperatur_4_1 = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_kombi_2_420_oeltemperatur_4_1);
    // Aussentemp__ungefiltert_4_1__Ko
    int64_t raw_tmp_kombi_2_420_aussentemp_ungefiltert_4_1_ko = (int64_t)llround((params::vw_pq.kombi_2_420_aussentemp_ungefiltert_4_1_ko - -50.0f) / 0.5f);
    uint64_t raw_kombi_2_420_aussentemp_ungefiltert_4_1_ko = raw_tmp_kombi_2_420_aussentemp_ungefiltert_4_1_ko < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_2_420_aussentemp_ungefiltert_4_1_ko;
    if (raw_kombi_2_420_aussentemp_ungefiltert_4_1_ko > 0xFFULL) raw_kombi_2_420_aussentemp_ungefiltert_4_1_ko = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_kombi_2_420_aussentemp_ungefiltert_4_1_ko);
    // Aussentemperatur_gefiltert
    int64_t raw_tmp_kombi_2_420_aussentemperatur_gefiltert = (int64_t)llround((params::vw_pq.kombi_2_420_aussentemperatur_gefiltert - -50.0f) / 0.5f);
    uint64_t raw_kombi_2_420_aussentemperatur_gefiltert = raw_tmp_kombi_2_420_aussentemperatur_gefiltert < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_2_420_aussentemperatur_gefiltert;
    if (raw_kombi_2_420_aussentemperatur_gefiltert > 0xFFULL) raw_kombi_2_420_aussentemperatur_gefiltert = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_kombi_2_420_aussentemperatur_gefiltert);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.kombi_2_420_fehlerspeichereintrag_kombi ? 1U : 0U);
    // Frei_Kombi_2_1
    int64_t raw_tmp_kombi_2_420_frei_kombi_2_1 = (int64_t)llround((params::vw_pq.kombi_2_420_frei_kombi_2_1 - 0.0f) / 1.0f);
    uint64_t raw_kombi_2_420_frei_kombi_2_1 = raw_tmp_kombi_2_420_frei_kombi_2_1 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_2_420_frei_kombi_2_1;
    if (raw_kombi_2_420_frei_kombi_2_1 > 0x7ULL) raw_kombi_2_420_frei_kombi_2_1 = 0x7ULL;
    writeBitsLE(frame.data, 4, 3, (uint32_t)raw_kombi_2_420_frei_kombi_2_1);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.kombi_2_420_anhaenger_erkannt ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.kombi_2_420_fehlerst_kuehlmitteltemp_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.kombi_2_420_fehlerstatus_oeltemperatur_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.kombi_2_420_fehlerstatus_aussentemp_4_1 ? 1U : 0U);
}

// Message 0x320 (800) - Kombi_1
// DLC: 8, TX: XXX
bool decodeVwPq0x320(const CANMessage &frame) {
    if (frame.id != 0x320 || frame.len < 8) {
        return false;
    }

    // Frei_Kombi_1_3: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_1_320_frei_kombi_1_3 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.kombi_1_320_frei_kombi_1_3 = raw_kombi_1_320_frei_kombi_1_3 * 1.0f + 0.0f;
    // Angezeigte_Geschwindigkeit: start=46 len=10 endian=1 sign=+ factor=0.32 offset=0 unit="km/h"
    const uint32_t raw_kombi_1_320_angezeigte_geschwindigkeit = readBitsLE(frame.data, 46, 10);
    params::vw_pq.kombi_1_320_angezeigte_geschwindigkeit = raw_kombi_1_320_angezeigte_geschwindigkeit * 0.32f + 0.0f;
    // Blinker_rechts_4_1: start=45 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_blinker_rechts_4_1 = readBitsLE(frame.data, 45, 1) != 0;
    // Blinker_links_4_1: start=44 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_blinker_links_4_1 = readBitsLE(frame.data, 44, 1) != 0;
    // Gesetzte_Zeitluecke__Kombi_1_: start=43 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_gesetzte_zeitluecke_kombi_1 = readBitsLE(frame.data, 43, 1) != 0;
    // ADR_Summer_abgeschaltet: start=42 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_adr_summer_abgeschaltet = readBitsLE(frame.data, 42, 1) != 0;
    // Frei_Kombi_1_2: start=40 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_1_320_frei_kombi_1_2 = readBitsLE(frame.data, 40, 2);
    params::vw_pq.kombi_1_320_frei_kombi_1_2 = raw_kombi_1_320_frei_kombi_1_2 * 1.0f + 0.0f;
    // Geschwindigkeit__Kombi_1_: start=25 len=15 endian=1 sign=+ factor=0.01 offset=0 unit="km/h"
    const uint32_t raw_kombi_1_320_geschwindigkeit_kombi_1 = readBitsLE(frame.data, 25, 15);
    params::vw_pq.kombi_1_320_geschwindigkeit_kombi_1 = raw_kombi_1_320_geschwindigkeit_kombi_1 * 0.01f + 0.0f;
    // Signalquelle_Geschwindigkeit_4_: start=24 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_signalquelle_geschwindigkeit_4 = readBitsLE(frame.data, 24, 1) != 0;
    // Tankwarnung: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_tankwarnung = readBitsLE(frame.data, 23, 1) != 0;
    // Tankinhalt: start=16 len=7 endian=1 sign=+ factor=1 offset=0 unit="l"
    const uint32_t raw_kombi_1_320_tankinhalt = readBitsLE(frame.data, 16, 7);
    params::vw_pq.kombi_1_320_tankinhalt = raw_kombi_1_320_tankinhalt * 1.0f + 0.0f;
    // Tankstop: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_tankstop = readBitsLE(frame.data, 15, 1) != 0;
    // Frei_Kombi_1_7: start=12 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_1_320_frei_kombi_1_7 = readBitsLE(frame.data, 12, 3);
    params::vw_pq.kombi_1_320_frei_kombi_1_7 = raw_kombi_1_320_frei_kombi_1_7 * 1.0f + 0.0f;
    // Kombi_im_Stellgliedtest: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_kombi_im_stellgliedtest = readBitsLE(frame.data, 11, 1) != 0;
    // Ladekontroll_Lampe__Kombi_: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_ladekontroll_lampe_kombi = readBitsLE(frame.data, 10, 1) != 0;
    // Bremsinfo: start=8 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_kombi_1_320_bremsinfo = readBitsLE(frame.data, 8, 2);
    params::vw_pq.kombi_1_320_bremsinfo = raw_kombi_1_320_bremsinfo * 1.0f + 0.0f;
    // Vorgluehlampe__Kombi_1_: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_vorgluehlampe_kombi_1 = readBitsLE(frame.data, 7, 1) != 0;
    // Tankwarnlampe: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_tankwarnlampe = readBitsLE(frame.data, 6, 1) != 0;
    // Heissleuchten_Vorwarnung: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_heissleuchten_vorwarnung = readBitsLE(frame.data, 5, 1) != 0;
    // Kuehlmittelmangel: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_kuehlmittelmangel = readBitsLE(frame.data, 4, 1) != 0;
    // Dynamische_Oeldruckwarnung: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_dynamische_oeldruckwarnung = readBitsLE(frame.data, 3, 1) != 0;
    // Oeldruck: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_oeldruck = readBitsLE(frame.data, 2, 1) != 0;
    // Fehlerstatus_Tank: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_fehlerstatus_tank = readBitsLE(frame.data, 1, 1) != 0;
    // Fahrertuer_4_1: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.kombi_1_320_fahrertuer_4_1 = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x320(CANMessage &frame) {
    frame.id = 0x320;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Frei_Kombi_1_3
    int64_t raw_tmp_kombi_1_320_frei_kombi_1_3 = (int64_t)llround((params::vw_pq.kombi_1_320_frei_kombi_1_3 - 0.0f) / 1.0f);
    uint64_t raw_kombi_1_320_frei_kombi_1_3 = raw_tmp_kombi_1_320_frei_kombi_1_3 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_1_320_frei_kombi_1_3;
    if (raw_kombi_1_320_frei_kombi_1_3 > 0xFFULL) raw_kombi_1_320_frei_kombi_1_3 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_kombi_1_320_frei_kombi_1_3);
    // Angezeigte_Geschwindigkeit
    int64_t raw_tmp_kombi_1_320_angezeigte_geschwindigkeit = (int64_t)llround((params::vw_pq.kombi_1_320_angezeigte_geschwindigkeit - 0.0f) / 0.32f);
    uint64_t raw_kombi_1_320_angezeigte_geschwindigkeit = raw_tmp_kombi_1_320_angezeigte_geschwindigkeit < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_1_320_angezeigte_geschwindigkeit;
    if (raw_kombi_1_320_angezeigte_geschwindigkeit > 0x3FFULL) raw_kombi_1_320_angezeigte_geschwindigkeit = 0x3FFULL;
    writeBitsLE(frame.data, 46, 10, (uint32_t)raw_kombi_1_320_angezeigte_geschwindigkeit);
    writeBitsLE(frame.data, 45, 1, params::vw_pq.kombi_1_320_blinker_rechts_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 44, 1, params::vw_pq.kombi_1_320_blinker_links_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 43, 1, params::vw_pq.kombi_1_320_gesetzte_zeitluecke_kombi_1 ? 1U : 0U);
    writeBitsLE(frame.data, 42, 1, params::vw_pq.kombi_1_320_adr_summer_abgeschaltet ? 1U : 0U);
    // Frei_Kombi_1_2
    int64_t raw_tmp_kombi_1_320_frei_kombi_1_2 = (int64_t)llround((params::vw_pq.kombi_1_320_frei_kombi_1_2 - 0.0f) / 1.0f);
    uint64_t raw_kombi_1_320_frei_kombi_1_2 = raw_tmp_kombi_1_320_frei_kombi_1_2 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_1_320_frei_kombi_1_2;
    if (raw_kombi_1_320_frei_kombi_1_2 > 0x3ULL) raw_kombi_1_320_frei_kombi_1_2 = 0x3ULL;
    writeBitsLE(frame.data, 40, 2, (uint32_t)raw_kombi_1_320_frei_kombi_1_2);
    // Geschwindigkeit__Kombi_1_
    int64_t raw_tmp_kombi_1_320_geschwindigkeit_kombi_1 = (int64_t)llround((params::vw_pq.kombi_1_320_geschwindigkeit_kombi_1 - 0.0f) / 0.01f);
    uint64_t raw_kombi_1_320_geschwindigkeit_kombi_1 = raw_tmp_kombi_1_320_geschwindigkeit_kombi_1 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_1_320_geschwindigkeit_kombi_1;
    if (raw_kombi_1_320_geschwindigkeit_kombi_1 > 0x7FFFULL) raw_kombi_1_320_geschwindigkeit_kombi_1 = 0x7FFFULL;
    writeBitsLE(frame.data, 25, 15, (uint32_t)raw_kombi_1_320_geschwindigkeit_kombi_1);
    writeBitsLE(frame.data, 24, 1, params::vw_pq.kombi_1_320_signalquelle_geschwindigkeit_4 ? 1U : 0U);
    writeBitsLE(frame.data, 23, 1, params::vw_pq.kombi_1_320_tankwarnung ? 1U : 0U);
    // Tankinhalt
    int64_t raw_tmp_kombi_1_320_tankinhalt = (int64_t)llround((params::vw_pq.kombi_1_320_tankinhalt - 0.0f) / 1.0f);
    uint64_t raw_kombi_1_320_tankinhalt = raw_tmp_kombi_1_320_tankinhalt < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_1_320_tankinhalt;
    if (raw_kombi_1_320_tankinhalt > 0x7FULL) raw_kombi_1_320_tankinhalt = 0x7FULL;
    writeBitsLE(frame.data, 16, 7, (uint32_t)raw_kombi_1_320_tankinhalt);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.kombi_1_320_tankstop ? 1U : 0U);
    // Frei_Kombi_1_7
    int64_t raw_tmp_kombi_1_320_frei_kombi_1_7 = (int64_t)llround((params::vw_pq.kombi_1_320_frei_kombi_1_7 - 0.0f) / 1.0f);
    uint64_t raw_kombi_1_320_frei_kombi_1_7 = raw_tmp_kombi_1_320_frei_kombi_1_7 < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_1_320_frei_kombi_1_7;
    if (raw_kombi_1_320_frei_kombi_1_7 > 0x7ULL) raw_kombi_1_320_frei_kombi_1_7 = 0x7ULL;
    writeBitsLE(frame.data, 12, 3, (uint32_t)raw_kombi_1_320_frei_kombi_1_7);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.kombi_1_320_kombi_im_stellgliedtest ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.kombi_1_320_ladekontroll_lampe_kombi ? 1U : 0U);
    // Bremsinfo
    int64_t raw_tmp_kombi_1_320_bremsinfo = (int64_t)llround((params::vw_pq.kombi_1_320_bremsinfo - 0.0f) / 1.0f);
    uint64_t raw_kombi_1_320_bremsinfo = raw_tmp_kombi_1_320_bremsinfo < 0 ? 0ULL : (uint64_t)raw_tmp_kombi_1_320_bremsinfo;
    if (raw_kombi_1_320_bremsinfo > 0x3ULL) raw_kombi_1_320_bremsinfo = 0x3ULL;
    writeBitsLE(frame.data, 8, 2, (uint32_t)raw_kombi_1_320_bremsinfo);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.kombi_1_320_vorgluehlampe_kombi_1 ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.kombi_1_320_tankwarnlampe ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.kombi_1_320_heissleuchten_vorwarnung ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.kombi_1_320_kuehlmittelmangel ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.kombi_1_320_dynamische_oeldruckwarnung ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.kombi_1_320_oeldruck ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.kombi_1_320_fehlerstatus_tank ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.kombi_1_320_fahrertuer_4_1 ? 1U : 0U);
}

// Message 0x5E0 (1504) - Klima_1
// DLC: 8, TX: XXX
bool decodeVwPq0x5E0(const CANMessage &frame) {
    if (frame.id != 0x5E0 || frame.len < 8) {
        return false;
    }

    // Aussentemp__ungef__Sto_f__4_1: start=56 len=8 endian=1 sign=+ factor=0.5 offset=-50 unit="C"
    const uint32_t raw_klima_1_5e0_aussentemp_ungef_sto_f_4_1 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.klima_1_5e0_aussentemp_ungef_sto_f_4_1 = raw_klima_1_5e0_aussentemp_ungef_sto_f_4_1 * 0.5f + -50.0f;
    // Fehlerspeichereintrag__Klima_: start=55 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.klima_1_5e0_fehlerspeichereintrag_klima = readBitsLE(frame.data, 55, 1) != 0;
    // Frei_Klima_1_5: start=50 len=5 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_klima_1_5e0_frei_klima_1_5 = readBitsLE(frame.data, 50, 5);
    params::vw_pq.klima_1_5e0_frei_klima_1_5 = raw_klima_1_5e0_frei_klima_1_5 * 1.0f + 0.0f;
    // AC_Schalter: start=49 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.klima_1_5e0_ac_schalter = readBitsLE(frame.data, 49, 1) != 0;
    // Temperatureinheit: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.klima_1_5e0_temperatureinheit = readBitsLE(frame.data, 48, 1) != 0;
    // Kuehlerluefteransteuerung__Klim: start=40 len=8 endian=1 sign=+ factor=0.4 offset=0 unit="%"
    const uint32_t raw_klima_1_5e0_kuehlerluefteransteuerung_klim = readBitsLE(frame.data, 40, 8);
    params::vw_pq.klima_1_5e0_kuehlerluefteransteuerung_klim = raw_klima_1_5e0_kuehlerluefteransteuerung_klim * 0.4f + 0.0f;
    // Geblaeselast_4_1: start=32 len=8 endian=1 sign=+ factor=0.4 offset=0 unit="%"
    const uint32_t raw_klima_1_5e0_geblaeselast_4_1 = readBitsLE(frame.data, 32, 8);
    params::vw_pq.klima_1_5e0_geblaeselast_4_1 = raw_klima_1_5e0_geblaeselast_4_1 * 0.4f + 0.0f;
    // Kompressorlast: start=24 len=8 endian=1 sign=+ factor=0.25 offset=0 unit="Nm"
    const uint32_t raw_klima_1_5e0_kompressorlast = readBitsLE(frame.data, 24, 8);
    params::vw_pq.klima_1_5e0_kompressorlast = raw_klima_1_5e0_kompressorlast * 0.25f + 0.0f;
    // Klimadrucksignal__Klima_1_: start=16 len=8 endian=1 sign=+ factor=0.2 offset=0 unit="bar"
    const uint32_t raw_klima_1_5e0_klimadrucksignal_klima_1 = readBitsLE(frame.data, 16, 8);
    params::vw_pq.klima_1_5e0_klimadrucksignal_klima_1 = raw_klima_1_5e0_klimadrucksignal_klima_1 * 0.2f + 0.0f;
    // Aussentemp__ungef__4_1__Klima_1: start=8 len=8 endian=1 sign=+ factor=0.5 offset=-50 unit="C"
    const uint32_t raw_klima_1_5e0_aussentemp_ungef_4_1_klima_1 = readBitsLE(frame.data, 8, 8);
    params::vw_pq.klima_1_5e0_aussentemp_ungef_4_1_klima_1 = raw_klima_1_5e0_aussentemp_ungef_4_1_klima_1 * 0.5f + -50.0f;
    // Kaeltemitteldruck_veraltet: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.klima_1_5e0_kaeltemitteldruck_veraltet = readBitsLE(frame.data, 7, 1) != 0;
    // Kompressormoment_veraltet_4_1: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.klima_1_5e0_kompressormoment_veraltet_4_1 = readBitsLE(frame.data, 6, 1) != 0;
    // Keine_Heizleistg_gewuenscht_4_1: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.klima_1_5e0_keine_heizleistg_gewuenscht_4_1 = readBitsLE(frame.data, 5, 1) != 0;
    // Kompressorzustand__4_1_: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.klima_1_5e0_kompressorzustand_4_1 = readBitsLE(frame.data, 4, 1) != 0;
    // Heizbare_Frontscheibe: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.klima_1_5e0_heizbare_frontscheibe = readBitsLE(frame.data, 3, 1) != 0;
    // Heizbare_Heckscheibe: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.klima_1_5e0_heizbare_heckscheibe = readBitsLE(frame.data, 2, 1) != 0;
    // Fahrerwunsch_Zuheizer: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.klima_1_5e0_fahrerwunsch_zuheizer = readBitsLE(frame.data, 1, 1) != 0;
    // Drehzahlanhebung: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.klima_1_5e0_drehzahlanhebung = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x5E0(CANMessage &frame) {
    frame.id = 0x5E0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Aussentemp__ungef__Sto_f__4_1
    int64_t raw_tmp_klima_1_5e0_aussentemp_ungef_sto_f_4_1 = (int64_t)llround((params::vw_pq.klima_1_5e0_aussentemp_ungef_sto_f_4_1 - -50.0f) / 0.5f);
    uint64_t raw_klima_1_5e0_aussentemp_ungef_sto_f_4_1 = raw_tmp_klima_1_5e0_aussentemp_ungef_sto_f_4_1 < 0 ? 0ULL : (uint64_t)raw_tmp_klima_1_5e0_aussentemp_ungef_sto_f_4_1;
    if (raw_klima_1_5e0_aussentemp_ungef_sto_f_4_1 > 0xFFULL) raw_klima_1_5e0_aussentemp_ungef_sto_f_4_1 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_klima_1_5e0_aussentemp_ungef_sto_f_4_1);
    writeBitsLE(frame.data, 55, 1, params::vw_pq.klima_1_5e0_fehlerspeichereintrag_klima ? 1U : 0U);
    // Frei_Klima_1_5
    int64_t raw_tmp_klima_1_5e0_frei_klima_1_5 = (int64_t)llround((params::vw_pq.klima_1_5e0_frei_klima_1_5 - 0.0f) / 1.0f);
    uint64_t raw_klima_1_5e0_frei_klima_1_5 = raw_tmp_klima_1_5e0_frei_klima_1_5 < 0 ? 0ULL : (uint64_t)raw_tmp_klima_1_5e0_frei_klima_1_5;
    if (raw_klima_1_5e0_frei_klima_1_5 > 0x1FULL) raw_klima_1_5e0_frei_klima_1_5 = 0x1FULL;
    writeBitsLE(frame.data, 50, 5, (uint32_t)raw_klima_1_5e0_frei_klima_1_5);
    writeBitsLE(frame.data, 49, 1, params::vw_pq.klima_1_5e0_ac_schalter ? 1U : 0U);
    writeBitsLE(frame.data, 48, 1, params::vw_pq.klima_1_5e0_temperatureinheit ? 1U : 0U);
    // Kuehlerluefteransteuerung__Klim
    int64_t raw_tmp_klima_1_5e0_kuehlerluefteransteuerung_klim = (int64_t)llround((params::vw_pq.klima_1_5e0_kuehlerluefteransteuerung_klim - 0.0f) / 0.4f);
    uint64_t raw_klima_1_5e0_kuehlerluefteransteuerung_klim = raw_tmp_klima_1_5e0_kuehlerluefteransteuerung_klim < 0 ? 0ULL : (uint64_t)raw_tmp_klima_1_5e0_kuehlerluefteransteuerung_klim;
    if (raw_klima_1_5e0_kuehlerluefteransteuerung_klim > 0xFFULL) raw_klima_1_5e0_kuehlerluefteransteuerung_klim = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_klima_1_5e0_kuehlerluefteransteuerung_klim);
    // Geblaeselast_4_1
    int64_t raw_tmp_klima_1_5e0_geblaeselast_4_1 = (int64_t)llround((params::vw_pq.klima_1_5e0_geblaeselast_4_1 - 0.0f) / 0.4f);
    uint64_t raw_klima_1_5e0_geblaeselast_4_1 = raw_tmp_klima_1_5e0_geblaeselast_4_1 < 0 ? 0ULL : (uint64_t)raw_tmp_klima_1_5e0_geblaeselast_4_1;
    if (raw_klima_1_5e0_geblaeselast_4_1 > 0xFFULL) raw_klima_1_5e0_geblaeselast_4_1 = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_klima_1_5e0_geblaeselast_4_1);
    // Kompressorlast
    int64_t raw_tmp_klima_1_5e0_kompressorlast = (int64_t)llround((params::vw_pq.klima_1_5e0_kompressorlast - 0.0f) / 0.25f);
    uint64_t raw_klima_1_5e0_kompressorlast = raw_tmp_klima_1_5e0_kompressorlast < 0 ? 0ULL : (uint64_t)raw_tmp_klima_1_5e0_kompressorlast;
    if (raw_klima_1_5e0_kompressorlast > 0xFFULL) raw_klima_1_5e0_kompressorlast = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_klima_1_5e0_kompressorlast);
    // Klimadrucksignal__Klima_1_
    int64_t raw_tmp_klima_1_5e0_klimadrucksignal_klima_1 = (int64_t)llround((params::vw_pq.klima_1_5e0_klimadrucksignal_klima_1 - 0.0f) / 0.2f);
    uint64_t raw_klima_1_5e0_klimadrucksignal_klima_1 = raw_tmp_klima_1_5e0_klimadrucksignal_klima_1 < 0 ? 0ULL : (uint64_t)raw_tmp_klima_1_5e0_klimadrucksignal_klima_1;
    if (raw_klima_1_5e0_klimadrucksignal_klima_1 > 0xFFULL) raw_klima_1_5e0_klimadrucksignal_klima_1 = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_klima_1_5e0_klimadrucksignal_klima_1);
    // Aussentemp__ungef__4_1__Klima_1
    int64_t raw_tmp_klima_1_5e0_aussentemp_ungef_4_1_klima_1 = (int64_t)llround((params::vw_pq.klima_1_5e0_aussentemp_ungef_4_1_klima_1 - -50.0f) / 0.5f);
    uint64_t raw_klima_1_5e0_aussentemp_ungef_4_1_klima_1 = raw_tmp_klima_1_5e0_aussentemp_ungef_4_1_klima_1 < 0 ? 0ULL : (uint64_t)raw_tmp_klima_1_5e0_aussentemp_ungef_4_1_klima_1;
    if (raw_klima_1_5e0_aussentemp_ungef_4_1_klima_1 > 0xFFULL) raw_klima_1_5e0_aussentemp_ungef_4_1_klima_1 = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_klima_1_5e0_aussentemp_ungef_4_1_klima_1);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.klima_1_5e0_kaeltemitteldruck_veraltet ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.klima_1_5e0_kompressormoment_veraltet_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.klima_1_5e0_keine_heizleistg_gewuenscht_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.klima_1_5e0_kompressorzustand_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.klima_1_5e0_heizbare_frontscheibe ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.klima_1_5e0_heizbare_heckscheibe ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.klima_1_5e0_fahrerwunsch_zuheizer ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.klima_1_5e0_drehzahlanhebung ? 1U : 0U);
}

// Message 0x38A (906) - GRA_Neu
// DLC: 4, TX: XXX
bool decodeVwPq0x38A(const CANMessage &frame) {
    if (frame.id != 0x38A || frame.len < 4) {
        return false;
    }

    // CHECKSUM: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gra_neu_38a_checksum = readBitsLE(frame.data, 0, 8);
    params::vw_pq.gra_neu_38a_checksum = raw_gra_neu_38a_checksum * 1.0f + 0.0f;
    // GRA_Hauptschalt: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_hauptschalt = readBitsLE(frame.data, 8, 1) != 0;
    // GRA_Abbrechen: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_abbrechen = readBitsLE(frame.data, 9, 1) != 0;
    // GRA_Down_kurz: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_down_kurz = readBitsLE(frame.data, 10, 1) != 0;
    // GRA_Up_kurz: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_up_kurz = readBitsLE(frame.data, 11, 1) != 0;
    // GRA_Down_lang: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_down_lang = readBitsLE(frame.data, 12, 1) != 0;
    // GRA_Up_lang: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_up_lang = readBitsLE(frame.data, 13, 1) != 0;
    // GRA_Fehler_Bed: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_fehler_bed = readBitsLE(frame.data, 14, 1) != 0;
    // GRA_Kodierinfo: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_kodierinfo = readBitsLE(frame.data, 15, 1) != 0;
    // GRA_Neu_Setzen: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_neu_setzen = readBitsLE(frame.data, 16, 1) != 0;
    // GRA_Recall: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_recall = readBitsLE(frame.data, 17, 1) != 0;
    // GRA_Sender: start=18 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gra_neu_38a_gra_sender = readBitsLE(frame.data, 18, 2);
    params::vw_pq.gra_neu_38a_gra_sender = raw_gra_neu_38a_gra_sender * 1.0f + 0.0f;
    // COUNTER: start=20 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gra_neu_38a_counter = readBitsLE(frame.data, 20, 4);
    params::vw_pq.gra_neu_38a_counter = raw_gra_neu_38a_counter * 1.0f + 0.0f;
    // GRA_Tip_Down: start=24 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_tip_down = readBitsLE(frame.data, 24, 1) != 0;
    // GRA_Tip_Up: start=25 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_tip_up = readBitsLE(frame.data, 25, 1) != 0;
    // GRA_Zeitluecke: start=26 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gra_neu_38a_gra_zeitluecke = readBitsLE(frame.data, 26, 2);
    params::vw_pq.gra_neu_38a_gra_zeitluecke = raw_gra_neu_38a_gra_zeitluecke * 1.0f + 0.0f;
    // GRA_Sta_Limiter: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_sta_limiter = readBitsLE(frame.data, 28, 1) != 0;
    // GRA_Typ_Hauptschalt: start=29 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_typ_hauptschalt = readBitsLE(frame.data, 29, 1) != 0;
    // GRA_Sportschalter: start=30 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_sportschalter = readBitsLE(frame.data, 30, 1) != 0;
    // GRA_Fehler_Tip: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_neu_38a_gra_fehler_tip = readBitsLE(frame.data, 31, 1) != 0;
    return true;
}

void encodeVwPq0x38A(CANMessage &frame) {
    frame.id = 0x38A;
    frame.len = 4;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // CHECKSUM
    int64_t raw_tmp_gra_neu_38a_checksum = (int64_t)llround((params::vw_pq.gra_neu_38a_checksum - 0.0f) / 1.0f);
    uint64_t raw_gra_neu_38a_checksum = raw_tmp_gra_neu_38a_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_gra_neu_38a_checksum;
    if (raw_gra_neu_38a_checksum > 0xFFULL) raw_gra_neu_38a_checksum = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_gra_neu_38a_checksum);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.gra_neu_38a_gra_hauptschalt ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.gra_neu_38a_gra_abbrechen ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.gra_neu_38a_gra_down_kurz ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.gra_neu_38a_gra_up_kurz ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.gra_neu_38a_gra_down_lang ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.gra_neu_38a_gra_up_lang ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.gra_neu_38a_gra_fehler_bed ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.gra_neu_38a_gra_kodierinfo ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.gra_neu_38a_gra_neu_setzen ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.gra_neu_38a_gra_recall ? 1U : 0U);
    // GRA_Sender
    int64_t raw_tmp_gra_neu_38a_gra_sender = (int64_t)llround((params::vw_pq.gra_neu_38a_gra_sender - 0.0f) / 1.0f);
    uint64_t raw_gra_neu_38a_gra_sender = raw_tmp_gra_neu_38a_gra_sender < 0 ? 0ULL : (uint64_t)raw_tmp_gra_neu_38a_gra_sender;
    if (raw_gra_neu_38a_gra_sender > 0x3ULL) raw_gra_neu_38a_gra_sender = 0x3ULL;
    writeBitsLE(frame.data, 18, 2, (uint32_t)raw_gra_neu_38a_gra_sender);
    // COUNTER
    int64_t raw_tmp_gra_neu_38a_counter = (int64_t)llround((params::vw_pq.gra_neu_38a_counter - 0.0f) / 1.0f);
    uint64_t raw_gra_neu_38a_counter = raw_tmp_gra_neu_38a_counter < 0 ? 0ULL : (uint64_t)raw_tmp_gra_neu_38a_counter;
    if (raw_gra_neu_38a_counter > 0xFULL) raw_gra_neu_38a_counter = 0xFULL;
    writeBitsLE(frame.data, 20, 4, (uint32_t)raw_gra_neu_38a_counter);
    writeBitsLE(frame.data, 24, 1, params::vw_pq.gra_neu_38a_gra_tip_down ? 1U : 0U);
    writeBitsLE(frame.data, 25, 1, params::vw_pq.gra_neu_38a_gra_tip_up ? 1U : 0U);
    // GRA_Zeitluecke
    int64_t raw_tmp_gra_neu_38a_gra_zeitluecke = (int64_t)llround((params::vw_pq.gra_neu_38a_gra_zeitluecke - 0.0f) / 1.0f);
    uint64_t raw_gra_neu_38a_gra_zeitluecke = raw_tmp_gra_neu_38a_gra_zeitluecke < 0 ? 0ULL : (uint64_t)raw_tmp_gra_neu_38a_gra_zeitluecke;
    if (raw_gra_neu_38a_gra_zeitluecke > 0x3ULL) raw_gra_neu_38a_gra_zeitluecke = 0x3ULL;
    writeBitsLE(frame.data, 26, 2, (uint32_t)raw_gra_neu_38a_gra_zeitluecke);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.gra_neu_38a_gra_sta_limiter ? 1U : 0U);
    writeBitsLE(frame.data, 29, 1, params::vw_pq.gra_neu_38a_gra_typ_hauptschalt ? 1U : 0U);
    writeBitsLE(frame.data, 30, 1, params::vw_pq.gra_neu_38a_gra_sportschalter ? 1U : 0U);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.gra_neu_38a_gra_fehler_tip ? 1U : 0U);
}

// Message 0x388 (904) - GRA
// DLC: 3, TX: XXX
bool decodeVwPq0x388(const CANMessage &frame) {
    if (frame.id != 0x388 || frame.len < 3) {
        return false;
    }

    // Checksumme_GRA_alt: start=16 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gra_388_checksumme_gra_alt = readBitsLE(frame.data, 16, 8);
    params::vw_pq.gra_388_checksumme_gra_alt = raw_gra_388_checksumme_gra_alt * 1.0f + 0.0f;
    // Frei_GRA_alt: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_388_frei_gra_alt = readBitsLE(frame.data, 15, 1) != 0;
    // GRA_alt__ADR_Bedienteil_Fehler: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_388_gra_alt_adr_bedienteil_fehler = readBitsLE(frame.data, 14, 1) != 0;
    // GRA_alt__ADR_beschleunigen: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_388_gra_alt_adr_beschleunigen = readBitsLE(frame.data, 13, 1) != 0;
    // GRA_alt__ADR_verzoegern: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_388_gra_alt_adr_verzoegern = readBitsLE(frame.data, 12, 1) != 0;
    // GRA_alt__ADR___Tipschalter__Wie: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_388_gra_alt_adr_tipschalter_wie = readBitsLE(frame.data, 11, 1) != 0;
    // GRA_alt__ADR___Tipschalter__Set: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_388_gra_alt_adr_tipschalter_set = readBitsLE(frame.data, 10, 1) != 0;
    // GRA_alt__ADR___Tipschalter__Aus: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_388_gra_alt_adr_tipschalter_aus = readBitsLE(frame.data, 9, 1) != 0;
    // GRA_alt__ADR___Hauptschalter: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gra_388_gra_alt_adr_hauptschalter = readBitsLE(frame.data, 8, 1) != 0;
    // Zaehler_GRA_alt: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gra_388_zaehler_gra_alt = readBitsLE(frame.data, 0, 8);
    params::vw_pq.gra_388_zaehler_gra_alt = raw_gra_388_zaehler_gra_alt * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x388(CANMessage &frame) {
    frame.id = 0x388;
    frame.len = 3;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Checksumme_GRA_alt
    int64_t raw_tmp_gra_388_checksumme_gra_alt = (int64_t)llround((params::vw_pq.gra_388_checksumme_gra_alt - 0.0f) / 1.0f);
    uint64_t raw_gra_388_checksumme_gra_alt = raw_tmp_gra_388_checksumme_gra_alt < 0 ? 0ULL : (uint64_t)raw_tmp_gra_388_checksumme_gra_alt;
    if (raw_gra_388_checksumme_gra_alt > 0xFFULL) raw_gra_388_checksumme_gra_alt = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_gra_388_checksumme_gra_alt);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.gra_388_frei_gra_alt ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.gra_388_gra_alt_adr_bedienteil_fehler ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.gra_388_gra_alt_adr_beschleunigen ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.gra_388_gra_alt_adr_verzoegern ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.gra_388_gra_alt_adr_tipschalter_wie ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.gra_388_gra_alt_adr_tipschalter_set ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.gra_388_gra_alt_adr_tipschalter_aus ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.gra_388_gra_alt_adr_hauptschalter ? 1U : 0U);
    // Zaehler_GRA_alt
    int64_t raw_tmp_gra_388_zaehler_gra_alt = (int64_t)llround((params::vw_pq.gra_388_zaehler_gra_alt - 0.0f) / 1.0f);
    uint64_t raw_gra_388_zaehler_gra_alt = raw_tmp_gra_388_zaehler_gra_alt < 0 ? 0ULL : (uint64_t)raw_tmp_gra_388_zaehler_gra_alt;
    if (raw_gra_388_zaehler_gra_alt > 0xFFULL) raw_gra_388_zaehler_gra_alt = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_gra_388_zaehler_gra_alt);
}

// Message 0x548 (1352) - Getriebe_4
// DLC: 3, TX: XXX
bool decodeVwPq0x548(const CANMessage &frame) {
    if (frame.id != 0x548 || frame.len < 3) {
        return false;
    }

    // Testparameter_2: start=16 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_getriebe_4_548_testparameter_2 = readBitsLE(frame.data, 16, 8);
    params::vw_pq.getriebe_4_548_testparameter_2 = raw_getriebe_4_548_testparameter_2 * 1.0f + 0.0f;
    // Testparameter_1: start=8 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_getriebe_4_548_testparameter_1 = readBitsLE(frame.data, 8, 8);
    params::vw_pq.getriebe_4_548_testparameter_1 = raw_getriebe_4_548_testparameter_1 * 1.0f + 0.0f;
    // Waehlhebelausleuchtung: start=4 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_getriebe_4_548_waehlhebelausleuchtung = readBitsLE(frame.data, 4, 4);
    params::vw_pq.getriebe_4_548_waehlhebelausleuchtung = raw_getriebe_4_548_waehlhebelausleuchtung * 1.0f + 0.0f;
    // Frei_Getriebe_4_1: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_4_548_frei_getriebe_4_1 = readBitsLE(frame.data, 3, 1) != 0;
    // Testfreigabeflag: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_4_548_testfreigabeflag = readBitsLE(frame.data, 2, 1) != 0;
    // Handbremserinnerung_s_Lampe: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_4_548_handbremserinnerung_s_lampe = readBitsLE(frame.data, 1, 1) != 0;
    // Shiftlock_Getriebe_4: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_4_548_shiftlock_getriebe_4 = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x548(CANMessage &frame) {
    frame.id = 0x548;
    frame.len = 3;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Testparameter_2
    int64_t raw_tmp_getriebe_4_548_testparameter_2 = (int64_t)llround((params::vw_pq.getriebe_4_548_testparameter_2 - 0.0f) / 1.0f);
    uint64_t raw_getriebe_4_548_testparameter_2 = raw_tmp_getriebe_4_548_testparameter_2 < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_4_548_testparameter_2;
    if (raw_getriebe_4_548_testparameter_2 > 0xFFULL) raw_getriebe_4_548_testparameter_2 = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_getriebe_4_548_testparameter_2);
    // Testparameter_1
    int64_t raw_tmp_getriebe_4_548_testparameter_1 = (int64_t)llround((params::vw_pq.getriebe_4_548_testparameter_1 - 0.0f) / 1.0f);
    uint64_t raw_getriebe_4_548_testparameter_1 = raw_tmp_getriebe_4_548_testparameter_1 < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_4_548_testparameter_1;
    if (raw_getriebe_4_548_testparameter_1 > 0xFFULL) raw_getriebe_4_548_testparameter_1 = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_getriebe_4_548_testparameter_1);
    // Waehlhebelausleuchtung
    int64_t raw_tmp_getriebe_4_548_waehlhebelausleuchtung = (int64_t)llround((params::vw_pq.getriebe_4_548_waehlhebelausleuchtung - 0.0f) / 1.0f);
    uint64_t raw_getriebe_4_548_waehlhebelausleuchtung = raw_tmp_getriebe_4_548_waehlhebelausleuchtung < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_4_548_waehlhebelausleuchtung;
    if (raw_getriebe_4_548_waehlhebelausleuchtung > 0xFULL) raw_getriebe_4_548_waehlhebelausleuchtung = 0xFULL;
    writeBitsLE(frame.data, 4, 4, (uint32_t)raw_getriebe_4_548_waehlhebelausleuchtung);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.getriebe_4_548_frei_getriebe_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.getriebe_4_548_testfreigabeflag ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.getriebe_4_548_handbremserinnerung_s_lampe ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.getriebe_4_548_shiftlock_getriebe_4 ? 1U : 0U);
}

// Message 0x540 (1344) - Getriebe_2
// DLC: 8, TX: XXX
bool decodeVwPq0x540(const CANMessage &frame) {
    if (frame.id != 0x540 || frame.len < 8) {
        return false;
    }

    // eingelegte_Fahrstufe: start=60 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_getriebe_2_540_eingelegte_fahrstufe = readBitsLE(frame.data, 60, 4);
    params::vw_pq.getriebe_2_540_eingelegte_fahrstufe = raw_getriebe_2_540_eingelegte_fahrstufe * 1.0f + 0.0f;
    // Ganganzeige_Kombi___Getriebe_Va: start=56 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_getriebe_2_540_ganganzeige_kombi_getriebe_va = readBitsLE(frame.data, 56, 4);
    params::vw_pq.getriebe_2_540_ganganzeige_kombi_getriebe_va = raw_getriebe_2_540_ganganzeige_kombi_getriebe_va * 1.0f + 0.0f;
    // Fehlerlampe_f_r_Kupplung_bei_VL: start=55 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_2_540_fehlerlampe_f_r_kupplung_bei_vl = readBitsLE(frame.data, 55, 1) != 0;
    // Anforderung_Kriechadaption: start=54 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_2_540_anforderung_kriechadaption = readBitsLE(frame.data, 54, 1) != 0;
    // ECO_Anzeige__4_1_: start=53 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_2_540_eco_anzeige_4_1 = readBitsLE(frame.data, 53, 1) != 0;
    // Shift_Lock_Lampe: start=52 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_2_540_shift_lock_lampe = readBitsLE(frame.data, 52, 1) != 0;
    // Unterdrueckung_von_Warnungen: start=51 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_2_540_unterdrueckung_von_warnungen = readBitsLE(frame.data, 51, 1) != 0;
    // Gong: start=50 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_2_540_gong = readBitsLE(frame.data, 50, 1) != 0;
    // Starter_wird_angesteuert: start=49 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_2_540_starter_wird_angesteuert = readBitsLE(frame.data, 49, 1) != 0;
    // Hochschaltlampe: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Upshift Flag
    params::vw_pq.getriebe_2_540_hochschaltlampe = readBitsLE(frame.data, 48, 1) != 0;
    // Synchronisationszeit: start=40 len=8 endian=1 sign=+ factor=20 offset=0 unit="ms"
    const uint32_t raw_getriebe_2_540_synchronisationszeit = readBitsLE(frame.data, 40, 8);
    params::vw_pq.getriebe_2_540_synchronisationszeit = raw_getriebe_2_540_synchronisationszeit * 20.0f + 0.0f;
    // invertierte_Synchronisations_Wu: start=32 len=8 endian=1 sign=+ factor=25 offset=0 unit="U/min"
    const uint32_t raw_getriebe_2_540_invertierte_synchronisations_wu = readBitsLE(frame.data, 32, 8);
    params::vw_pq.getriebe_2_540_invertierte_synchronisations_wu = raw_getriebe_2_540_invertierte_synchronisations_wu * 25.0f + 0.0f;
    // Synchronisations_Wunschdrehzahl: start=24 len=8 endian=1 sign=+ factor=25 offset=0 unit="U/min"
    const uint32_t raw_getriebe_2_540_synchronisations_wunschdrehzahl = readBitsLE(frame.data, 24, 8);
    params::vw_pq.getriebe_2_540_synchronisations_wunschdrehzahl = raw_getriebe_2_540_synchronisations_wunschdrehzahl * 25.0f + 0.0f;
    // Gradientenbegrenzung: start=16 len=8 endian=1 sign=+ factor=10 offset=0 unit="Nm/s"
    const uint32_t raw_getriebe_2_540_gradientenbegrenzung = readBitsLE(frame.data, 16, 8);
    params::vw_pq.getriebe_2_540_gradientenbegrenzung = raw_getriebe_2_540_gradientenbegrenzung * 10.0f + 0.0f;
    // Leerlaufsolldrehzahl__Getriebe: start=8 len=8 endian=1 sign=+ factor=10 offset=0 unit="U/min"
    const uint32_t raw_getriebe_2_540_leerlaufsolldrehzahl_getriebe = readBitsLE(frame.data, 8, 8);
    params::vw_pq.getriebe_2_540_leerlaufsolldrehzahl_getriebe = raw_getriebe_2_540_leerlaufsolldrehzahl_getriebe * 10.0f + 0.0f;
    // Zahler_Getriebe_2: start=4 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Counter Getriebe_2
    const uint32_t raw_getriebe_2_540_zahler_getriebe_2 = readBitsLE(frame.data, 4, 4);
    params::vw_pq.getriebe_2_540_zahler_getriebe_2 = raw_getriebe_2_540_zahler_getriebe_2 * 1.0f + 0.0f;
    // Zwischengasflag: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_2_540_zwischengasflag = readBitsLE(frame.data, 3, 1) != 0;
    // Ecomatic__4_1_: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_2_540_ecomatic_4_1 = readBitsLE(frame.data, 2, 1) != 0;
    // Schubabschaltunterstuetzung: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_2_540_schubabschaltunterstuetzung = readBitsLE(frame.data, 1, 1) != 0;
    // LFR_Adaption_Freigabeflag: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.getriebe_2_540_lfr_adaption_freigabeflag = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x540(CANMessage &frame) {
    frame.id = 0x540;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // eingelegte_Fahrstufe
    int64_t raw_tmp_getriebe_2_540_eingelegte_fahrstufe = (int64_t)llround((params::vw_pq.getriebe_2_540_eingelegte_fahrstufe - 0.0f) / 1.0f);
    uint64_t raw_getriebe_2_540_eingelegte_fahrstufe = raw_tmp_getriebe_2_540_eingelegte_fahrstufe < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_2_540_eingelegte_fahrstufe;
    if (raw_getriebe_2_540_eingelegte_fahrstufe > 0xFULL) raw_getriebe_2_540_eingelegte_fahrstufe = 0xFULL;
    writeBitsLE(frame.data, 60, 4, (uint32_t)raw_getriebe_2_540_eingelegte_fahrstufe);
    // Ganganzeige_Kombi___Getriebe_Va
    int64_t raw_tmp_getriebe_2_540_ganganzeige_kombi_getriebe_va = (int64_t)llround((params::vw_pq.getriebe_2_540_ganganzeige_kombi_getriebe_va - 0.0f) / 1.0f);
    uint64_t raw_getriebe_2_540_ganganzeige_kombi_getriebe_va = raw_tmp_getriebe_2_540_ganganzeige_kombi_getriebe_va < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_2_540_ganganzeige_kombi_getriebe_va;
    if (raw_getriebe_2_540_ganganzeige_kombi_getriebe_va > 0xFULL) raw_getriebe_2_540_ganganzeige_kombi_getriebe_va = 0xFULL;
    writeBitsLE(frame.data, 56, 4, (uint32_t)raw_getriebe_2_540_ganganzeige_kombi_getriebe_va);
    writeBitsLE(frame.data, 55, 1, params::vw_pq.getriebe_2_540_fehlerlampe_f_r_kupplung_bei_vl ? 1U : 0U);
    writeBitsLE(frame.data, 54, 1, params::vw_pq.getriebe_2_540_anforderung_kriechadaption ? 1U : 0U);
    writeBitsLE(frame.data, 53, 1, params::vw_pq.getriebe_2_540_eco_anzeige_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 52, 1, params::vw_pq.getriebe_2_540_shift_lock_lampe ? 1U : 0U);
    writeBitsLE(frame.data, 51, 1, params::vw_pq.getriebe_2_540_unterdrueckung_von_warnungen ? 1U : 0U);
    writeBitsLE(frame.data, 50, 1, params::vw_pq.getriebe_2_540_gong ? 1U : 0U);
    writeBitsLE(frame.data, 49, 1, params::vw_pq.getriebe_2_540_starter_wird_angesteuert ? 1U : 0U);
    writeBitsLE(frame.data, 48, 1, params::vw_pq.getriebe_2_540_hochschaltlampe ? 1U : 0U);
    // Synchronisationszeit
    int64_t raw_tmp_getriebe_2_540_synchronisationszeit = (int64_t)llround((params::vw_pq.getriebe_2_540_synchronisationszeit - 0.0f) / 20.0f);
    uint64_t raw_getriebe_2_540_synchronisationszeit = raw_tmp_getriebe_2_540_synchronisationszeit < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_2_540_synchronisationszeit;
    if (raw_getriebe_2_540_synchronisationszeit > 0xFFULL) raw_getriebe_2_540_synchronisationszeit = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_getriebe_2_540_synchronisationszeit);
    // invertierte_Synchronisations_Wu
    int64_t raw_tmp_getriebe_2_540_invertierte_synchronisations_wu = (int64_t)llround((params::vw_pq.getriebe_2_540_invertierte_synchronisations_wu - 0.0f) / 25.0f);
    uint64_t raw_getriebe_2_540_invertierte_synchronisations_wu = raw_tmp_getriebe_2_540_invertierte_synchronisations_wu < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_2_540_invertierte_synchronisations_wu;
    if (raw_getriebe_2_540_invertierte_synchronisations_wu > 0xFFULL) raw_getriebe_2_540_invertierte_synchronisations_wu = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_getriebe_2_540_invertierte_synchronisations_wu);
    // Synchronisations_Wunschdrehzahl
    int64_t raw_tmp_getriebe_2_540_synchronisations_wunschdrehzahl = (int64_t)llround((params::vw_pq.getriebe_2_540_synchronisations_wunschdrehzahl - 0.0f) / 25.0f);
    uint64_t raw_getriebe_2_540_synchronisations_wunschdrehzahl = raw_tmp_getriebe_2_540_synchronisations_wunschdrehzahl < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_2_540_synchronisations_wunschdrehzahl;
    if (raw_getriebe_2_540_synchronisations_wunschdrehzahl > 0xFFULL) raw_getriebe_2_540_synchronisations_wunschdrehzahl = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_getriebe_2_540_synchronisations_wunschdrehzahl);
    // Gradientenbegrenzung
    int64_t raw_tmp_getriebe_2_540_gradientenbegrenzung = (int64_t)llround((params::vw_pq.getriebe_2_540_gradientenbegrenzung - 0.0f) / 10.0f);
    uint64_t raw_getriebe_2_540_gradientenbegrenzung = raw_tmp_getriebe_2_540_gradientenbegrenzung < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_2_540_gradientenbegrenzung;
    if (raw_getriebe_2_540_gradientenbegrenzung > 0xFFULL) raw_getriebe_2_540_gradientenbegrenzung = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_getriebe_2_540_gradientenbegrenzung);
    // Leerlaufsolldrehzahl__Getriebe
    int64_t raw_tmp_getriebe_2_540_leerlaufsolldrehzahl_getriebe = (int64_t)llround((params::vw_pq.getriebe_2_540_leerlaufsolldrehzahl_getriebe - 0.0f) / 10.0f);
    uint64_t raw_getriebe_2_540_leerlaufsolldrehzahl_getriebe = raw_tmp_getriebe_2_540_leerlaufsolldrehzahl_getriebe < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_2_540_leerlaufsolldrehzahl_getriebe;
    if (raw_getriebe_2_540_leerlaufsolldrehzahl_getriebe > 0xFFULL) raw_getriebe_2_540_leerlaufsolldrehzahl_getriebe = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_getriebe_2_540_leerlaufsolldrehzahl_getriebe);
    // Zahler_Getriebe_2
    int64_t raw_tmp_getriebe_2_540_zahler_getriebe_2 = (int64_t)llround((params::vw_pq.getriebe_2_540_zahler_getriebe_2 - 0.0f) / 1.0f);
    uint64_t raw_getriebe_2_540_zahler_getriebe_2 = raw_tmp_getriebe_2_540_zahler_getriebe_2 < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_2_540_zahler_getriebe_2;
    if (raw_getriebe_2_540_zahler_getriebe_2 > 0xFULL) raw_getriebe_2_540_zahler_getriebe_2 = 0xFULL;
    writeBitsLE(frame.data, 4, 4, (uint32_t)raw_getriebe_2_540_zahler_getriebe_2);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.getriebe_2_540_zwischengasflag ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.getriebe_2_540_ecomatic_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.getriebe_2_540_schubabschaltunterstuetzung ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.getriebe_2_540_lfr_adaption_freigabeflag ? 1U : 0U);
}

// Message 0x440 (1088) - Getriebe_1
// DLC: 8, TX: Transmission
// Comment: Getriebe 1 - Contains gear status, torque requests, and control signals
bool decodeVwPq0x440(const CANMessage &frame) {
    if (frame.id != 0x440 || frame.len < 8) {
        return false;
    }

    // GE1_Schaltung: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Gear shift active: 0=no shift, 1=shift in progress
    // Value table: 0 "no_circuit" 1 "circuit_is_running"
    params::vw_pq.getriebe_1_440_ge1_schaltung = readBitsLE(frame.data, 0, 1) != 0;
    // GE1_Sta_Schutz: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Transmission/converter protection request (power reduction or ignition angle)
    // Value table: 0 "no_protection" 1 "protection_requested"
    params::vw_pq.getriebe_1_440_ge1_sta_schutz = readBitsLE(frame.data, 1, 1) != 0;
    // GE1_Klimakompr: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: AC compressor off request
    // Value table: 0 "no_Anforderung" 1 "Kompressor_aus"
    params::vw_pq.getriebe_1_440_ge1_klimakompr = readBitsLE(frame.data, 2, 1) != 0;
    // GE1_WK: start=3 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Torque converter clutch status
    // Value table: 0 "WK_open" 1 "WK_regulated" 2 "WK_closed" 3 "Failure"
    const uint32_t raw_getriebe_1_440_ge1_wk = readBitsLE(frame.data, 3, 2);
    params::vw_pq.getriebe_1_440_ge1_wk = raw_getriebe_1_440_ge1_wk * 1.0f + 0.0f;
    // GE1_StSt_Info: start=5 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Start/stop coordinator info
    // Value table: 0 "Engine_running_not_necessary_stop_release" 1 "Motor_start_not_mandatory_necessary_stop_prohibition" 2 "Motor_start_absolute_necessary_start_request" 3 "System_error"
    const uint32_t raw_getriebe_1_440_ge1_stst_info = readBitsLE(frame.data, 5, 2);
    params::vw_pq.getriebe_1_440_ge1_stst_info = raw_getriebe_1_440_ge1_stst_info * 1.0f + 0.0f;
    // GE1_EGS_Anf: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: EGS request - byte 4 becomes valid when set
    // Value table: 0 "no_requirement" 1 "EGS_requirement"
    params::vw_pq.getriebe_1_440_ge1_egs_anf = readBitsLE(frame.data, 7, 1) != 0;
    // GE1_Zielgang: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Target gear when shifting or current gear
    // Value table: 0 "P (Park) | Disengaged" 1 "Gear 1 | Engaged" 2 "Gear 2 | Engaged" 3 "Gear 3 | Engaged" 4 "Gear 4 | Engaged" 5 "Gear 5 | Engaged" 6 "1m Gear" 7 "Gang_R" 8 "Gear 6 | Engaged" 9 "Gear 7 | Engaged" 10 "Gear 8 | Engaged" 14 "Speed Not Defined" 15 "Failure"
    const uint32_t raw_getriebe_1_440_ge1_zielgang = readBitsLE(frame.data, 8, 4);
    params::vw_pq.getriebe_1_440_ge1_zielgang = raw_getriebe_1_440_ge1_zielgang * 1.0f + 0.0f;
    // GE1_Wahl_Pos: start=12 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Selector lever position / driving program
    // Value table: 8 "P" 7 "R" 6 "N" 5 "D" 9 "U" 12 "S" 14 "T" 10 "T" 11 "T"
    const uint32_t raw_getriebe_1_440_ge1_wahl_pos = readBitsLE(frame.data, 12, 4);
    params::vw_pq.getriebe_1_440_ge1_wahl_pos = raw_getriebe_1_440_ge1_wahl_pos * 1.0f + 0.0f;
    // GE1_Mrad_Mkurb: start=16 len=8 endian=1 sign=+ factor=0.1 offset=0 unit=""
    // DBC comment: Wheel torque to crankshaft torque ratio (Mrad/Mcrank)
    const uint32_t raw_getriebe_1_440_ge1_mrad_mkurb = readBitsLE(frame.data, 16, 8);
    params::vw_pq.getriebe_1_440_ge1_mrad_mkurb = raw_getriebe_1_440_ge1_mrad_mkurb * 0.1f + 0.0f;
    // GE1_Soll_Mo: start=24 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="%"
    // DBC comment: Internal target engine torque request (0-99.06%)
    const uint32_t raw_getriebe_1_440_ge1_soll_mo = readBitsLE(frame.data, 24, 8);
    params::vw_pq.getriebe_1_440_ge1_soll_mo = raw_getriebe_1_440_ge1_soll_mo * 0.39f + 0.0f;
    // GE1_Fahrwistd: start=32 len=8 endian=1 sign=+ factor=0.249 offset=-31.623 unit=""
    // DBC comment: Driving resistance index (normalized % slope)
    const uint32_t raw_getriebe_1_440_ge1_fahrwistd = readBitsLE(frame.data, 32, 8);
    params::vw_pq.getriebe_1_440_ge1_fahrwistd = raw_getriebe_1_440_ge1_fahrwistd * 0.249f + -31.623f;
    // GE1_Notlauf: start=40 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Emergency mode status
    // Value table: 0 "No Emergency" 1 "No Switching" 2 "Switch to Neutral" 3 "50% Shifting Capacity" 4 "Driving Without UK" 5 "No Emeregency" 6 "No Emergency" 7 "No Emergency, Target Shifting Reached" 15 "Failure"
    const uint32_t raw_getriebe_1_440_ge1_notlauf = readBitsLE(frame.data, 40, 4);
    params::vw_pq.getriebe_1_440_ge1_notlauf = raw_getriebe_1_440_ge1_notlauf * 1.0f + 0.0f;
    // GE1_Kuehlung: start=44 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Cooling request level
    // Value table: 0 "No Additional Cooling Requested" 1 "20% Additional Fan Cooling Requested" 2 "40% Additional Fan Cooling Requested" 3 "Maximum Additional Fan Cooling Requested"
    const uint32_t raw_getriebe_1_440_ge1_kuehlung = readBitsLE(frame.data, 44, 2);
    params::vw_pq.getriebe_1_440_ge1_kuehlung = raw_getriebe_1_440_ge1_kuehlung * 1.0f + 0.0f;
    // GE1_Sta_OBD: start=47 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: OBD status - MIL lamp control
    // Value table: 0 "MIL Off" 1 "MIL On"
    params::vw_pq.getriebe_1_440_ge1_sta_obd = readBitsLE(frame.data, 47, 1) != 0;
    // GE1_LaunchControl: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Launch control active
    // Value table: 0 "Launch Control Not Active" 1 "Launch Control Program Active"
    params::vw_pq.getriebe_1_440_ge1_launchcontrol = readBitsLE(frame.data, 48, 1) != 0;
    // GE1_Infobit: start=49 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Transmission type: 0=stepped automatic, 1=CVT
    // Value table: 0 "Standard Automatic" 1 "CVT"
    params::vw_pq.getriebe_1_440_ge1_infobit = readBitsLE(frame.data, 49, 1) != 0;
    // GE1_SleepInd: start=50 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Sleep readiness after terminal 15 off
    // Value table: 0 "CAN is Required" 1 "Sleep Ready"
    params::vw_pq.getriebe_1_440_ge1_sleepind = readBitsLE(frame.data, 50, 1) != 0;
    // COUNTER: start=51 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_getriebe_1_440_counter = readBitsLE(frame.data, 51, 4);
    params::vw_pq.getriebe_1_440_counter = raw_getriebe_1_440_counter * 1.0f + 0.0f;
    // GE1_Fehlereintr: start=55 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Fault memory entry present
    // Value table: 0 "No Failure Memory Entry" 1 "Failure Memory Entry"
    params::vw_pq.getriebe_1_440_ge1_fehlereintr = readBitsLE(frame.data, 55, 1) != 0;
    // GE1_WaVerl_Mo: start=56 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="%"
    // DBC comment: Transmission torque loss / clutch torque (0-99.06%)
    const uint32_t raw_getriebe_1_440_ge1_waverl_mo = readBitsLE(frame.data, 56, 8);
    params::vw_pq.getriebe_1_440_ge1_waverl_mo = raw_getriebe_1_440_ge1_waverl_mo * 0.39f + 0.0f;
    return true;
}

void encodeVwPq0x440(CANMessage &frame) {
    frame.id = 0x440;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 0, 1, params::vw_pq.getriebe_1_440_ge1_schaltung ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.getriebe_1_440_ge1_sta_schutz ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.getriebe_1_440_ge1_klimakompr ? 1U : 0U);
    // GE1_WK
    int64_t raw_tmp_getriebe_1_440_ge1_wk = (int64_t)llround((params::vw_pq.getriebe_1_440_ge1_wk - 0.0f) / 1.0f);
    uint64_t raw_getriebe_1_440_ge1_wk = raw_tmp_getriebe_1_440_ge1_wk < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_1_440_ge1_wk;
    if (raw_getriebe_1_440_ge1_wk > 0x3ULL) raw_getriebe_1_440_ge1_wk = 0x3ULL;
    writeBitsLE(frame.data, 3, 2, (uint32_t)raw_getriebe_1_440_ge1_wk);
    // GE1_StSt_Info
    int64_t raw_tmp_getriebe_1_440_ge1_stst_info = (int64_t)llround((params::vw_pq.getriebe_1_440_ge1_stst_info - 0.0f) / 1.0f);
    uint64_t raw_getriebe_1_440_ge1_stst_info = raw_tmp_getriebe_1_440_ge1_stst_info < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_1_440_ge1_stst_info;
    if (raw_getriebe_1_440_ge1_stst_info > 0x3ULL) raw_getriebe_1_440_ge1_stst_info = 0x3ULL;
    writeBitsLE(frame.data, 5, 2, (uint32_t)raw_getriebe_1_440_ge1_stst_info);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.getriebe_1_440_ge1_egs_anf ? 1U : 0U);
    // GE1_Zielgang
    int64_t raw_tmp_getriebe_1_440_ge1_zielgang = (int64_t)llround((params::vw_pq.getriebe_1_440_ge1_zielgang - 0.0f) / 1.0f);
    uint64_t raw_getriebe_1_440_ge1_zielgang = raw_tmp_getriebe_1_440_ge1_zielgang < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_1_440_ge1_zielgang;
    if (raw_getriebe_1_440_ge1_zielgang > 0xFULL) raw_getriebe_1_440_ge1_zielgang = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_getriebe_1_440_ge1_zielgang);
    // GE1_Wahl_Pos
    int64_t raw_tmp_getriebe_1_440_ge1_wahl_pos = (int64_t)llround((params::vw_pq.getriebe_1_440_ge1_wahl_pos - 0.0f) / 1.0f);
    uint64_t raw_getriebe_1_440_ge1_wahl_pos = raw_tmp_getriebe_1_440_ge1_wahl_pos < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_1_440_ge1_wahl_pos;
    if (raw_getriebe_1_440_ge1_wahl_pos > 0xFULL) raw_getriebe_1_440_ge1_wahl_pos = 0xFULL;
    writeBitsLE(frame.data, 12, 4, (uint32_t)raw_getriebe_1_440_ge1_wahl_pos);
    // GE1_Mrad_Mkurb
    int64_t raw_tmp_getriebe_1_440_ge1_mrad_mkurb = (int64_t)llround((params::vw_pq.getriebe_1_440_ge1_mrad_mkurb - 0.0f) / 0.1f);
    uint64_t raw_getriebe_1_440_ge1_mrad_mkurb = raw_tmp_getriebe_1_440_ge1_mrad_mkurb < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_1_440_ge1_mrad_mkurb;
    if (raw_getriebe_1_440_ge1_mrad_mkurb > 0xFFULL) raw_getriebe_1_440_ge1_mrad_mkurb = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_getriebe_1_440_ge1_mrad_mkurb);
    // GE1_Soll_Mo
    int64_t raw_tmp_getriebe_1_440_ge1_soll_mo = (int64_t)llround((params::vw_pq.getriebe_1_440_ge1_soll_mo - 0.0f) / 0.39f);
    uint64_t raw_getriebe_1_440_ge1_soll_mo = raw_tmp_getriebe_1_440_ge1_soll_mo < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_1_440_ge1_soll_mo;
    if (raw_getriebe_1_440_ge1_soll_mo > 0xFFULL) raw_getriebe_1_440_ge1_soll_mo = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_getriebe_1_440_ge1_soll_mo);
    // GE1_Fahrwistd
    int64_t raw_tmp_getriebe_1_440_ge1_fahrwistd = (int64_t)llround((params::vw_pq.getriebe_1_440_ge1_fahrwistd - -31.623f) / 0.249f);
    uint64_t raw_getriebe_1_440_ge1_fahrwistd = raw_tmp_getriebe_1_440_ge1_fahrwistd < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_1_440_ge1_fahrwistd;
    if (raw_getriebe_1_440_ge1_fahrwistd > 0xFFULL) raw_getriebe_1_440_ge1_fahrwistd = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_getriebe_1_440_ge1_fahrwistd);
    // GE1_Notlauf
    int64_t raw_tmp_getriebe_1_440_ge1_notlauf = (int64_t)llround((params::vw_pq.getriebe_1_440_ge1_notlauf - 0.0f) / 1.0f);
    uint64_t raw_getriebe_1_440_ge1_notlauf = raw_tmp_getriebe_1_440_ge1_notlauf < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_1_440_ge1_notlauf;
    if (raw_getriebe_1_440_ge1_notlauf > 0xFULL) raw_getriebe_1_440_ge1_notlauf = 0xFULL;
    writeBitsLE(frame.data, 40, 4, (uint32_t)raw_getriebe_1_440_ge1_notlauf);
    // GE1_Kuehlung
    int64_t raw_tmp_getriebe_1_440_ge1_kuehlung = (int64_t)llround((params::vw_pq.getriebe_1_440_ge1_kuehlung - 0.0f) / 1.0f);
    uint64_t raw_getriebe_1_440_ge1_kuehlung = raw_tmp_getriebe_1_440_ge1_kuehlung < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_1_440_ge1_kuehlung;
    if (raw_getriebe_1_440_ge1_kuehlung > 0x3ULL) raw_getriebe_1_440_ge1_kuehlung = 0x3ULL;
    writeBitsLE(frame.data, 44, 2, (uint32_t)raw_getriebe_1_440_ge1_kuehlung);
    writeBitsLE(frame.data, 47, 1, params::vw_pq.getriebe_1_440_ge1_sta_obd ? 1U : 0U);
    writeBitsLE(frame.data, 48, 1, params::vw_pq.getriebe_1_440_ge1_launchcontrol ? 1U : 0U);
    writeBitsLE(frame.data, 49, 1, params::vw_pq.getriebe_1_440_ge1_infobit ? 1U : 0U);
    writeBitsLE(frame.data, 50, 1, params::vw_pq.getriebe_1_440_ge1_sleepind ? 1U : 0U);
    // COUNTER
    int64_t raw_tmp_getriebe_1_440_counter = (int64_t)llround((params::vw_pq.getriebe_1_440_counter - 0.0f) / 1.0f);
    uint64_t raw_getriebe_1_440_counter = raw_tmp_getriebe_1_440_counter < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_1_440_counter;
    if (raw_getriebe_1_440_counter > 0xFULL) raw_getriebe_1_440_counter = 0xFULL;
    writeBitsLE(frame.data, 51, 4, (uint32_t)raw_getriebe_1_440_counter);
    writeBitsLE(frame.data, 55, 1, params::vw_pq.getriebe_1_440_ge1_fehlereintr ? 1U : 0U);
    // GE1_WaVerl_Mo
    int64_t raw_tmp_getriebe_1_440_ge1_waverl_mo = (int64_t)llround((params::vw_pq.getriebe_1_440_ge1_waverl_mo - 0.0f) / 0.39f);
    uint64_t raw_getriebe_1_440_ge1_waverl_mo = raw_tmp_getriebe_1_440_ge1_waverl_mo < 0 ? 0ULL : (uint64_t)raw_tmp_getriebe_1_440_ge1_waverl_mo;
    if (raw_getriebe_1_440_ge1_waverl_mo > 0xFFULL) raw_getriebe_1_440_ge1_waverl_mo = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_getriebe_1_440_ge1_waverl_mo);
}

// Message 0x390 (912) - Gate_Komf_1
// DLC: 8, TX: XXX
bool decodeVwPq0x390(const CANMessage &frame) {
    if (frame.id != 0x390 || frame.len < 8) {
        return false;
    }

    // GK1_Sta_RDK_Warn: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sta_rdk_warn = readBitsLE(frame.data, 0, 1) != 0;
    // GK1_Sta_Anhaen: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sta_anhaen = readBitsLE(frame.data, 1, 1) != 0;
    // GK1_Sta_Licht1: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sta_licht1 = readBitsLE(frame.data, 2, 1) != 0;
    // GK1_Sta_Licht3: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sta_licht3 = readBitsLE(frame.data, 3, 1) != 0;
    // GK1_Sta_Tuerkont: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sta_tuerkont = readBitsLE(frame.data, 4, 1) != 0;
    // GK1_Sta_Li_vorn: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sta_li_vorn = readBitsLE(frame.data, 5, 1) != 0;
    // GK1_SleepAckn: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sleepackn = readBitsLE(frame.data, 7, 1) != 0;
    // GK1_CharismaModus m1: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gate_komf_1_390_gk1_charismamodus_m1 = readBitsLE(frame.data, 8, 4);
    params::vw_pq.gate_komf_1_390_gk1_charismamodus_m1 = raw_gate_komf_1_390_gk1_charismamodus_m1 * 1.0f + 0.0f;
    // GK1_SamFktNr M: start=12 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gate_komf_1_390_gk1_samfktnr_m = readBitsLE(frame.data, 12, 4);
    params::vw_pq.gate_komf_1_390_gk1_samfktnr_m = raw_gate_komf_1_390_gk1_samfktnr_m * 1.0f + 0.0f;
    // GK1_Fa_Tuerkont: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Status of the driver's door rotary latch
    params::vw_pq.gate_komf_1_390_gk1_fa_tuerkont = readBitsLE(frame.data, 16, 1) != 0;
    // GK1_RueckfahrSch: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_rueckfahrsch = readBitsLE(frame.data, 17, 1) != 0;
    // GK1_ELV_verrieg: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_elv_verrieg = readBitsLE(frame.data, 18, 1) != 0;
    // GK1_Sta_Kessy_2: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sta_kessy_2 = readBitsLE(frame.data, 19, 1) != 0;
    // GK1_Sta_Stdhzg: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sta_stdhzg = readBitsLE(frame.data, 20, 1) != 0;
    // GK1_SH_Verbau: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sh_verbau = readBitsLE(frame.data, 21, 1) != 0;
    // GK1_ParkFrontWi: start=22 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_parkfrontwi = readBitsLE(frame.data, 22, 1) != 0;
    // GK1_KW_Warm: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_kw_warm = readBitsLE(frame.data, 23, 1) != 0;
    // BCM_Remotestart_Betrieb: start=24 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_bcm_remotestart_betrieb = readBitsLE(frame.data, 24, 1) != 0;
    // BSK_HL_geoeffnet: start=26 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Status of the rear left door rotary latch
    params::vw_pq.gate_komf_1_390_bsk_hl_geoeffnet = readBitsLE(frame.data, 26, 1) != 0;
    // BSK_HR_geoeffnet: start=27 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Status of the rear right door rotary latch
    params::vw_pq.gate_komf_1_390_bsk_hr_geoeffnet = readBitsLE(frame.data, 27, 1) != 0;
    // GK1_Rueckfahr: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_rueckfahr = readBitsLE(frame.data, 28, 1) != 0;
    // GK1_BrLi_links: start=29 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_brli_links = readBitsLE(frame.data, 29, 1) != 0;
    // GK1_BrLi_rechts: start=30 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_brli_rechts = readBitsLE(frame.data, 30, 1) != 0;
    // GK1_BrLi_mitte: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_brli_mitte = readBitsLE(frame.data, 31, 1) != 0;
    // GK1_BLS_ILM: start=32 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_bls_ilm = readBitsLE(frame.data, 32, 1) != 0;
    // GK1_EDC_ILM: start=33 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_edc_ilm = readBitsLE(frame.data, 33, 1) != 0;
    // GK1_Blinker_li: start=34 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_blinker_li = readBitsLE(frame.data, 34, 1) != 0;
    // GK1_Blinker_re: start=35 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_blinker_re = readBitsLE(frame.data, 35, 1) != 0;
    // GK1_def_P_verr: start=36 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_def_p_verr = readBitsLE(frame.data, 36, 1) != 0;
    // GK1_LS1_Fernlicht: start=37 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_ls1_fernlicht = readBitsLE(frame.data, 37, 1) != 0;
    // GK1_Sta_Licht2: start=38 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sta_licht2 = readBitsLE(frame.data, 38, 1) != 0;
    // GK1_Sta_LSM: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sta_lsm = readBitsLE(frame.data, 39, 1) != 0;
    // GK1_Count_Anhaen: start=40 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gate_komf_1_390_gk1_count_anhaen = readBitsLE(frame.data, 40, 4);
    params::vw_pq.gate_komf_1_390_gk1_count_anhaen = raw_gate_komf_1_390_gk1_count_anhaen * 1.0f + 0.0f;
    // BSK_BT_geoeffnet: start=41 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Status of the passenger door rotary latch
    params::vw_pq.gate_komf_1_390_bsk_bt_geoeffnet = readBitsLE(frame.data, 41, 1) != 0;
    // BSK_HD_Hauptraste: start=43 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Status of trunk lid main detent
    params::vw_pq.gate_komf_1_390_bsk_hd_hauptraste = readBitsLE(frame.data, 43, 1) != 0;
    // GK1_BLS_AAG: start=44 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_bls_aag = readBitsLE(frame.data, 44, 1) != 0;
    // GK1_EDC_AAG: start=45 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_edc_aag = readBitsLE(frame.data, 45, 1) != 0;
    // GK1_Anhaenger: start=46 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_anhaenger = readBitsLE(frame.data, 46, 1) != 0;
    // GK1_BrLi_Anhaen: start=47 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_brli_anhaen = readBitsLE(frame.data, 47, 1) != 0;
    // GK1_Abblendlicht: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_abblendlicht = readBitsLE(frame.data, 48, 1) != 0;
    // GK1_Fernlicht: start=49 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_fernlicht = readBitsLE(frame.data, 49, 1) != 0;
    // GK1_Wischer_vorn: start=50 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_wischer_vorn = readBitsLE(frame.data, 50, 1) != 0;
    // GK1_Sta_ILM_F_1: start=51 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sta_ilm_f_1 = readBitsLE(frame.data, 51, 1) != 0;
    // GK1_Abbl_VL_def: start=52 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_abbl_vl_def = readBitsLE(frame.data, 52, 1) != 0;
    // GK1_Abbl_VR_def: start=53 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_abbl_vr_def = readBitsLE(frame.data, 53, 1) != 0;
    // GK1_Blink_Autob: start=54 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_blink_autob = readBitsLE(frame.data, 54, 1) != 0;
    // GK1_Warnblk_Status: start=55 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_warnblk_status = readBitsLE(frame.data, 55, 1) != 0;
    // GK1_SH_laeuft: start=56 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sh_laeuft = readBitsLE(frame.data, 56, 1) != 0;
    // SH1_ein_Wasserpumpe: start=57 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_sh1_ein_wasserpumpe = readBitsLE(frame.data, 57, 1) != 0;
    // GK1_Nebel_ein: start=58 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_nebel_ein = readBitsLE(frame.data, 58, 1) != 0;
    // GK1_Bremslicht: start=59 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_bremslicht = readBitsLE(frame.data, 59, 1) != 0;
    // GK1_Anh_abgesteckt: start=60 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_anh_abgesteckt = readBitsLE(frame.data, 60, 1) != 0;
    // GK1_AnhKonLamp: start=61 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_anhkonlamp = readBitsLE(frame.data, 61, 1) != 0;
    // LDS_Stellung_AFL: start=62 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_lds_stellung_afl = readBitsLE(frame.data, 62, 1) != 0;
    // GK1_SH_Zusatzfkt: start=63 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_1_390_gk1_sh_zusatzfkt = readBitsLE(frame.data, 63, 1) != 0;
    return true;
}

void encodeVwPq0x390(CANMessage &frame) {
    frame.id = 0x390;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 0, 1, params::vw_pq.gate_komf_1_390_gk1_sta_rdk_warn ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.gate_komf_1_390_gk1_sta_anhaen ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.gate_komf_1_390_gk1_sta_licht1 ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.gate_komf_1_390_gk1_sta_licht3 ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.gate_komf_1_390_gk1_sta_tuerkont ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.gate_komf_1_390_gk1_sta_li_vorn ? 1U : 0U);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.gate_komf_1_390_gk1_sleepackn ? 1U : 0U);
    // GK1_CharismaModus m1
    int64_t raw_tmp_gate_komf_1_390_gk1_charismamodus_m1 = (int64_t)llround((params::vw_pq.gate_komf_1_390_gk1_charismamodus_m1 - 0.0f) / 1.0f);
    uint64_t raw_gate_komf_1_390_gk1_charismamodus_m1 = raw_tmp_gate_komf_1_390_gk1_charismamodus_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_1_390_gk1_charismamodus_m1;
    if (raw_gate_komf_1_390_gk1_charismamodus_m1 > 0xFULL) raw_gate_komf_1_390_gk1_charismamodus_m1 = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_gate_komf_1_390_gk1_charismamodus_m1);
    // GK1_SamFktNr M
    int64_t raw_tmp_gate_komf_1_390_gk1_samfktnr_m = (int64_t)llround((params::vw_pq.gate_komf_1_390_gk1_samfktnr_m - 0.0f) / 1.0f);
    uint64_t raw_gate_komf_1_390_gk1_samfktnr_m = raw_tmp_gate_komf_1_390_gk1_samfktnr_m < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_1_390_gk1_samfktnr_m;
    if (raw_gate_komf_1_390_gk1_samfktnr_m > 0xFULL) raw_gate_komf_1_390_gk1_samfktnr_m = 0xFULL;
    writeBitsLE(frame.data, 12, 4, (uint32_t)raw_gate_komf_1_390_gk1_samfktnr_m);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.gate_komf_1_390_gk1_fa_tuerkont ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.gate_komf_1_390_gk1_rueckfahrsch ? 1U : 0U);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.gate_komf_1_390_gk1_elv_verrieg ? 1U : 0U);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.gate_komf_1_390_gk1_sta_kessy_2 ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.gate_komf_1_390_gk1_sta_stdhzg ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.gate_komf_1_390_gk1_sh_verbau ? 1U : 0U);
    writeBitsLE(frame.data, 22, 1, params::vw_pq.gate_komf_1_390_gk1_parkfrontwi ? 1U : 0U);
    writeBitsLE(frame.data, 23, 1, params::vw_pq.gate_komf_1_390_gk1_kw_warm ? 1U : 0U);
    writeBitsLE(frame.data, 24, 1, params::vw_pq.gate_komf_1_390_bcm_remotestart_betrieb ? 1U : 0U);
    writeBitsLE(frame.data, 26, 1, params::vw_pq.gate_komf_1_390_bsk_hl_geoeffnet ? 1U : 0U);
    writeBitsLE(frame.data, 27, 1, params::vw_pq.gate_komf_1_390_bsk_hr_geoeffnet ? 1U : 0U);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.gate_komf_1_390_gk1_rueckfahr ? 1U : 0U);
    writeBitsLE(frame.data, 29, 1, params::vw_pq.gate_komf_1_390_gk1_brli_links ? 1U : 0U);
    writeBitsLE(frame.data, 30, 1, params::vw_pq.gate_komf_1_390_gk1_brli_rechts ? 1U : 0U);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.gate_komf_1_390_gk1_brli_mitte ? 1U : 0U);
    writeBitsLE(frame.data, 32, 1, params::vw_pq.gate_komf_1_390_gk1_bls_ilm ? 1U : 0U);
    writeBitsLE(frame.data, 33, 1, params::vw_pq.gate_komf_1_390_gk1_edc_ilm ? 1U : 0U);
    writeBitsLE(frame.data, 34, 1, params::vw_pq.gate_komf_1_390_gk1_blinker_li ? 1U : 0U);
    writeBitsLE(frame.data, 35, 1, params::vw_pq.gate_komf_1_390_gk1_blinker_re ? 1U : 0U);
    writeBitsLE(frame.data, 36, 1, params::vw_pq.gate_komf_1_390_gk1_def_p_verr ? 1U : 0U);
    writeBitsLE(frame.data, 37, 1, params::vw_pq.gate_komf_1_390_gk1_ls1_fernlicht ? 1U : 0U);
    writeBitsLE(frame.data, 38, 1, params::vw_pq.gate_komf_1_390_gk1_sta_licht2 ? 1U : 0U);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.gate_komf_1_390_gk1_sta_lsm ? 1U : 0U);
    // GK1_Count_Anhaen
    int64_t raw_tmp_gate_komf_1_390_gk1_count_anhaen = (int64_t)llround((params::vw_pq.gate_komf_1_390_gk1_count_anhaen - 0.0f) / 1.0f);
    uint64_t raw_gate_komf_1_390_gk1_count_anhaen = raw_tmp_gate_komf_1_390_gk1_count_anhaen < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_1_390_gk1_count_anhaen;
    if (raw_gate_komf_1_390_gk1_count_anhaen > 0xFULL) raw_gate_komf_1_390_gk1_count_anhaen = 0xFULL;
    writeBitsLE(frame.data, 40, 4, (uint32_t)raw_gate_komf_1_390_gk1_count_anhaen);
    writeBitsLE(frame.data, 41, 1, params::vw_pq.gate_komf_1_390_bsk_bt_geoeffnet ? 1U : 0U);
    writeBitsLE(frame.data, 43, 1, params::vw_pq.gate_komf_1_390_bsk_hd_hauptraste ? 1U : 0U);
    writeBitsLE(frame.data, 44, 1, params::vw_pq.gate_komf_1_390_gk1_bls_aag ? 1U : 0U);
    writeBitsLE(frame.data, 45, 1, params::vw_pq.gate_komf_1_390_gk1_edc_aag ? 1U : 0U);
    writeBitsLE(frame.data, 46, 1, params::vw_pq.gate_komf_1_390_gk1_anhaenger ? 1U : 0U);
    writeBitsLE(frame.data, 47, 1, params::vw_pq.gate_komf_1_390_gk1_brli_anhaen ? 1U : 0U);
    writeBitsLE(frame.data, 48, 1, params::vw_pq.gate_komf_1_390_gk1_abblendlicht ? 1U : 0U);
    writeBitsLE(frame.data, 49, 1, params::vw_pq.gate_komf_1_390_gk1_fernlicht ? 1U : 0U);
    writeBitsLE(frame.data, 50, 1, params::vw_pq.gate_komf_1_390_gk1_wischer_vorn ? 1U : 0U);
    writeBitsLE(frame.data, 51, 1, params::vw_pq.gate_komf_1_390_gk1_sta_ilm_f_1 ? 1U : 0U);
    writeBitsLE(frame.data, 52, 1, params::vw_pq.gate_komf_1_390_gk1_abbl_vl_def ? 1U : 0U);
    writeBitsLE(frame.data, 53, 1, params::vw_pq.gate_komf_1_390_gk1_abbl_vr_def ? 1U : 0U);
    writeBitsLE(frame.data, 54, 1, params::vw_pq.gate_komf_1_390_gk1_blink_autob ? 1U : 0U);
    writeBitsLE(frame.data, 55, 1, params::vw_pq.gate_komf_1_390_gk1_warnblk_status ? 1U : 0U);
    writeBitsLE(frame.data, 56, 1, params::vw_pq.gate_komf_1_390_gk1_sh_laeuft ? 1U : 0U);
    writeBitsLE(frame.data, 57, 1, params::vw_pq.gate_komf_1_390_sh1_ein_wasserpumpe ? 1U : 0U);
    writeBitsLE(frame.data, 58, 1, params::vw_pq.gate_komf_1_390_gk1_nebel_ein ? 1U : 0U);
    writeBitsLE(frame.data, 59, 1, params::vw_pq.gate_komf_1_390_gk1_bremslicht ? 1U : 0U);
    writeBitsLE(frame.data, 60, 1, params::vw_pq.gate_komf_1_390_gk1_anh_abgesteckt ? 1U : 0U);
    writeBitsLE(frame.data, 61, 1, params::vw_pq.gate_komf_1_390_gk1_anhkonlamp ? 1U : 0U);
    writeBitsLE(frame.data, 62, 1, params::vw_pq.gate_komf_1_390_lds_stellung_afl ? 1U : 0U);
    writeBitsLE(frame.data, 63, 1, params::vw_pq.gate_komf_1_390_gk1_sh_zusatzfkt ? 1U : 0U);
}

// Message 0x53C (1340) - Fahrwerk_1
// DLC: 1, TX: XXX
bool decodeVwPq0x53C(const CANMessage &frame) {
    if (frame.id != 0x53C || frame.len < 1) {
        return false;
    }

    // Frei_Fahrwerk_1_2: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.fahrwerk_1_53c_frei_fahrwerk_1_2 = readBitsLE(frame.data, 7, 1) != 0;
    // Frei_Fahrwerk_1_1: start=6 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_fahrwerk_1_53c_frei_fahrwerk_1_1 = readBitsLE(frame.data, 6, 2);
    params::vw_pq.fahrwerk_1_53c_frei_fahrwerk_1_1 = raw_fahrwerk_1_53c_frei_fahrwerk_1_1 * 1.0f + 0.0f;
    // Einstellung_Fahrwerkdaempfung_4: start=4 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_fahrwerk_1_53c_einstellung_fahrwerkdaempfung_4 = readBitsLE(frame.data, 4, 3);
    params::vw_pq.fahrwerk_1_53c_einstellung_fahrwerkdaempfung_4 = raw_fahrwerk_1_53c_einstellung_fahrwerkdaempfung_4 * 1.0f + 0.0f;
    // Ansteuererung_Fahrzeugniveau: start=0 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_fahrwerk_1_53c_ansteuererung_fahrzeugniveau = readBitsLE(frame.data, 0, 4);
    params::vw_pq.fahrwerk_1_53c_ansteuererung_fahrzeugniveau = raw_fahrwerk_1_53c_ansteuererung_fahrzeugniveau * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x53C(CANMessage &frame) {
    frame.id = 0x53C;
    frame.len = 1;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 7, 1, params::vw_pq.fahrwerk_1_53c_frei_fahrwerk_1_2 ? 1U : 0U);
    // Frei_Fahrwerk_1_1
    int64_t raw_tmp_fahrwerk_1_53c_frei_fahrwerk_1_1 = (int64_t)llround((params::vw_pq.fahrwerk_1_53c_frei_fahrwerk_1_1 - 0.0f) / 1.0f);
    uint64_t raw_fahrwerk_1_53c_frei_fahrwerk_1_1 = raw_tmp_fahrwerk_1_53c_frei_fahrwerk_1_1 < 0 ? 0ULL : (uint64_t)raw_tmp_fahrwerk_1_53c_frei_fahrwerk_1_1;
    if (raw_fahrwerk_1_53c_frei_fahrwerk_1_1 > 0x3ULL) raw_fahrwerk_1_53c_frei_fahrwerk_1_1 = 0x3ULL;
    writeBitsLE(frame.data, 6, 2, (uint32_t)raw_fahrwerk_1_53c_frei_fahrwerk_1_1);
    // Einstellung_Fahrwerkdaempfung_4
    int64_t raw_tmp_fahrwerk_1_53c_einstellung_fahrwerkdaempfung_4 = (int64_t)llround((params::vw_pq.fahrwerk_1_53c_einstellung_fahrwerkdaempfung_4 - 0.0f) / 1.0f);
    uint64_t raw_fahrwerk_1_53c_einstellung_fahrwerkdaempfung_4 = raw_tmp_fahrwerk_1_53c_einstellung_fahrwerkdaempfung_4 < 0 ? 0ULL : (uint64_t)raw_tmp_fahrwerk_1_53c_einstellung_fahrwerkdaempfung_4;
    if (raw_fahrwerk_1_53c_einstellung_fahrwerkdaempfung_4 > 0x7ULL) raw_fahrwerk_1_53c_einstellung_fahrwerkdaempfung_4 = 0x7ULL;
    writeBitsLE(frame.data, 4, 3, (uint32_t)raw_fahrwerk_1_53c_einstellung_fahrwerkdaempfung_4);
    // Ansteuererung_Fahrzeugniveau
    int64_t raw_tmp_fahrwerk_1_53c_ansteuererung_fahrzeugniveau = (int64_t)llround((params::vw_pq.fahrwerk_1_53c_ansteuererung_fahrzeugniveau - 0.0f) / 1.0f);
    uint64_t raw_fahrwerk_1_53c_ansteuererung_fahrzeugniveau = raw_tmp_fahrwerk_1_53c_ansteuererung_fahrzeugniveau < 0 ? 0ULL : (uint64_t)raw_tmp_fahrwerk_1_53c_ansteuererung_fahrzeugniveau;
    if (raw_fahrwerk_1_53c_ansteuererung_fahrzeugniveau > 0xFULL) raw_fahrwerk_1_53c_ansteuererung_fahrzeugniveau = 0xFULL;
    writeBitsLE(frame.data, 0, 4, (uint32_t)raw_fahrwerk_1_53c_ansteuererung_fahrzeugniveau);
}

// Message 0x5C0 (1472) - EPB_1
// DLC: 8, TX: XXX
bool decodeVwPq0x5C0(const CANMessage &frame) {
    if (frame.id != 0x5C0 || frame.len < 8) {
        return false;
    }

    // COUNTER: start=0 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_epb_1_5c0_counter = readBitsLE(frame.data, 0, 4);
    params::vw_pq.epb_1_5c0_counter = raw_epb_1_5c0_counter * 1.0f + 0.0f;
    // EP1_Fehler_Sta: start=4 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "volle_Funktion" 1 "linke_Seite_fehlerhaft" 2 "rechte_Seite_fehlerhaft" 3 "beide_Seiten_fehlerhaft"
    const uint32_t raw_epb_1_5c0_ep1_fehler_sta = readBitsLE(frame.data, 4, 2);
    params::vw_pq.epb_1_5c0_ep1_fehler_sta = raw_epb_1_5c0_ep1_fehler_sta * 1.0f + 0.0f;
    // EP1_Sta_EPB: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Bremse_geoeffnet" 1 "Bremse_geschlossen"
    params::vw_pq.epb_1_5c0_ep1_sta_epb = readBitsLE(frame.data, 6, 1) != 0;
    // EP1_Sta_Schalter: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "volle_Funktion" 1 "Schalter_ausser_Funktion"
    params::vw_pq.epb_1_5c0_ep1_sta_schalter = readBitsLE(frame.data, 7, 1) != 0;
    // EP1_Spannkraft: start=8 len=5 endian=1 sign=+ factor=1 offset=0 unit="Unit_KiloNewto"
    // Value table: 31 "Fehler"
    const uint32_t raw_epb_1_5c0_ep1_spannkraft = readBitsLE(frame.data, 8, 5);
    params::vw_pq.epb_1_5c0_ep1_spannkraft = raw_epb_1_5c0_ep1_spannkraft * 1.0f + 0.0f;
    // EP1_Schalterinfo: start=13 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "keine_Fahreranforderung" 1 "Fahreranforderung_oeffnen" 2 "Fahreranforderung_schliessen" 3 "Schalterfehler"
    const uint32_t raw_epb_1_5c0_ep1_schalterinfo = readBitsLE(frame.data, 13, 2);
    params::vw_pq.epb_1_5c0_ep1_schalterinfo = raw_epb_1_5c0_ep1_schalterinfo * 1.0f + 0.0f;
    // EP1_Sta_NWS: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "volle_Funktion" 1 "keine_Funktion"
    params::vw_pq.epb_1_5c0_ep1_sta_nws = readBitsLE(frame.data, 15, 1) != 0;
    // EP1_Neig_winkel: start=16 len=8 endian=1 sign=+ factor=1 offset=-128 unit="Unit_PerCentOfForceOfGravi"
    const uint32_t raw_epb_1_5c0_ep1_neig_winkel = readBitsLE(frame.data, 16, 8);
    params::vw_pq.epb_1_5c0_ep1_neig_winkel = raw_epb_1_5c0_ep1_neig_winkel * 1.0f + -128.0f;
    // EP1_Verzoegerung: start=24 len=8 endian=1 sign=+ factor=0.048 offset=-7.968 unit="Unit_MeterPerSeconSquar"
    const uint32_t raw_epb_1_5c0_ep1_verzoegerung = readBitsLE(frame.data, 24, 8);
    params::vw_pq.epb_1_5c0_ep1_verzoegerung = raw_epb_1_5c0_ep1_verzoegerung * 0.048f + -7.968f;
    // EP1_Fehlereintr: start=32 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "kein_Fehlerspeichereintrag" 1 "Fehlerspeichereintrag"
    params::vw_pq.epb_1_5c0_ep1_fehlereintr = readBitsLE(frame.data, 32, 1) != 0;
    // EP1_Freigabe_Ver: start=33 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Verzoegerungsanf_nicht_freigegeb" 1 "Verzoegerungsanf_freigegeben"
    params::vw_pq.epb_1_5c0_ep1_freigabe_ver = readBitsLE(frame.data, 33, 1) != 0;
    // EP1_AutoHold_zul: start=34 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Pers_nicht_zulaessig" 1 "Pers_zulaessig"
    params::vw_pq.epb_1_5c0_ep1_autohold_zul = readBitsLE(frame.data, 34, 1) != 0;
    // EP1_AutoHold_aktiv: start=35 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "nein" 1 "ja"
    params::vw_pq.epb_1_5c0_ep1_autohold_aktiv = readBitsLE(frame.data, 35, 1) != 0;
    // EP1_SleepInd: start=36 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "CAN_wird_benoetigt" 1 "Sleep_bereit"
    params::vw_pq.epb_1_5c0_ep1_sleepind = readBitsLE(frame.data, 36, 1) != 0;
    // EP1_Status_Kl_15: start=37 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Kl_15_aus" 1 "Kl_15_ein"
    params::vw_pq.epb_1_5c0_ep1_status_kl_15 = readBitsLE(frame.data, 37, 1) != 0;
    // EP1_Lampe_AutoP: start=38 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Lampe_aus" 1 "Lampe_ein"
    params::vw_pq.epb_1_5c0_ep1_lampe_autop = readBitsLE(frame.data, 38, 1) != 0;
    // EP1_Bremslicht: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Aus" 1 "Ein"
    params::vw_pq.epb_1_5c0_ep1_bremslicht = readBitsLE(frame.data, 39, 1) != 0;
    // EP1_Warnton1: start=40 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Aus" 1 "Ein"
    params::vw_pq.epb_1_5c0_ep1_warnton1 = readBitsLE(frame.data, 40, 1) != 0;
    // EP1_Warnton2: start=41 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Aus" 1 "Ein"
    params::vw_pq.epb_1_5c0_ep1_warnton2 = readBitsLE(frame.data, 41, 1) != 0;
    // EP1_AnfShLock: start=42 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Aus" 1 "Ein"
    params::vw_pq.epb_1_5c0_ep1_anfshlock = readBitsLE(frame.data, 42, 1) != 0;
    // EPB_Autoholdlampe: start=43 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Lampe_aus" 1 "Lampe_ein"
    params::vw_pq.epb_1_5c0_epb_autoholdlampe = readBitsLE(frame.data, 43, 1) != 0;
    // EP1_QualNeigWi: start=44 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "gueltiger_Wert" 1 "Ersatz_Init_oder_Fehlerwert"
    params::vw_pq.epb_1_5c0_ep1_qualneigwi = readBitsLE(frame.data, 44, 1) != 0;
    // EP1_KuppModBer: start=45 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Kuppsensor_aus_Modbereich" 1 "Kupplsensor_im_Modbereich" 2 "Sensorsignal_ungenau" 3 "Sensor_defekt"
    const uint32_t raw_epb_1_5c0_ep1_kuppmodber = readBitsLE(frame.data, 45, 2);
    params::vw_pq.epb_1_5c0_ep1_kuppmodber = raw_epb_1_5c0_ep1_kuppmodber * 1.0f + 0.0f;
    // EP1_HydrHalten: start=47 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Fzg_nicht_hydr_geh" 1 "Fzg_hydr_geh"
    params::vw_pq.epb_1_5c0_ep1_hydrhalten = readBitsLE(frame.data, 47, 1) != 0;
    // EP1_Fkt_Lampe: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Lampe_aus" 1 "Lampe_ein"
    params::vw_pq.epb_1_5c0_ep1_fkt_lampe = readBitsLE(frame.data, 48, 1) != 0;
    // EP1_Warnton: start=49 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Warnton_aus" 1 "Warnton_an"
    params::vw_pq.epb_1_5c0_ep1_warnton = readBitsLE(frame.data, 49, 1) != 0;
    // EP1_Fehler_BKL: start=50 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "BKL_aus" 1 "BKL_an"
    params::vw_pq.epb_1_5c0_ep1_fehler_bkl = readBitsLE(frame.data, 50, 1) != 0;
    // EP1_Fehler_gelb: start=51 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Lampe_aus" 1 "Lampe_ein"
    params::vw_pq.epb_1_5c0_ep1_fehler_gelb = readBitsLE(frame.data, 51, 1) != 0;
    // EP1__Text: start=52 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "kein_Text" 1 "Text_1" 2 "Text_2" 3 "Text_3" 4 "Text_4" 5 "Text_5" 6 "reserviert" 7 "reserviert" 8 "Text_8"
    const uint32_t raw_epb_1_5c0_ep1_text = readBitsLE(frame.data, 52, 4);
    params::vw_pq.epb_1_5c0_ep1_text = raw_epb_1_5c0_ep1_text * 1.0f + 0.0f;
    // CHECKSUM: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_epb_1_5c0_checksum = readBitsLE(frame.data, 56, 8);
    params::vw_pq.epb_1_5c0_checksum = raw_epb_1_5c0_checksum * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x5C0(CANMessage &frame) {
    frame.id = 0x5C0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // COUNTER
    int64_t raw_tmp_epb_1_5c0_counter = (int64_t)llround((params::vw_pq.epb_1_5c0_counter - 0.0f) / 1.0f);
    uint64_t raw_epb_1_5c0_counter = raw_tmp_epb_1_5c0_counter < 0 ? 0ULL : (uint64_t)raw_tmp_epb_1_5c0_counter;
    if (raw_epb_1_5c0_counter > 0xFULL) raw_epb_1_5c0_counter = 0xFULL;
    writeBitsLE(frame.data, 0, 4, (uint32_t)raw_epb_1_5c0_counter);
    // EP1_Fehler_Sta
    int64_t raw_tmp_epb_1_5c0_ep1_fehler_sta = (int64_t)llround((params::vw_pq.epb_1_5c0_ep1_fehler_sta - 0.0f) / 1.0f);
    uint64_t raw_epb_1_5c0_ep1_fehler_sta = raw_tmp_epb_1_5c0_ep1_fehler_sta < 0 ? 0ULL : (uint64_t)raw_tmp_epb_1_5c0_ep1_fehler_sta;
    if (raw_epb_1_5c0_ep1_fehler_sta > 0x3ULL) raw_epb_1_5c0_ep1_fehler_sta = 0x3ULL;
    writeBitsLE(frame.data, 4, 2, (uint32_t)raw_epb_1_5c0_ep1_fehler_sta);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.epb_1_5c0_ep1_sta_epb ? 1U : 0U);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.epb_1_5c0_ep1_sta_schalter ? 1U : 0U);
    // EP1_Spannkraft
    int64_t raw_tmp_epb_1_5c0_ep1_spannkraft = (int64_t)llround((params::vw_pq.epb_1_5c0_ep1_spannkraft - 0.0f) / 1.0f);
    uint64_t raw_epb_1_5c0_ep1_spannkraft = raw_tmp_epb_1_5c0_ep1_spannkraft < 0 ? 0ULL : (uint64_t)raw_tmp_epb_1_5c0_ep1_spannkraft;
    if (raw_epb_1_5c0_ep1_spannkraft > 0x1FULL) raw_epb_1_5c0_ep1_spannkraft = 0x1FULL;
    writeBitsLE(frame.data, 8, 5, (uint32_t)raw_epb_1_5c0_ep1_spannkraft);
    // EP1_Schalterinfo
    int64_t raw_tmp_epb_1_5c0_ep1_schalterinfo = (int64_t)llround((params::vw_pq.epb_1_5c0_ep1_schalterinfo - 0.0f) / 1.0f);
    uint64_t raw_epb_1_5c0_ep1_schalterinfo = raw_tmp_epb_1_5c0_ep1_schalterinfo < 0 ? 0ULL : (uint64_t)raw_tmp_epb_1_5c0_ep1_schalterinfo;
    if (raw_epb_1_5c0_ep1_schalterinfo > 0x3ULL) raw_epb_1_5c0_ep1_schalterinfo = 0x3ULL;
    writeBitsLE(frame.data, 13, 2, (uint32_t)raw_epb_1_5c0_ep1_schalterinfo);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.epb_1_5c0_ep1_sta_nws ? 1U : 0U);
    // EP1_Neig_winkel
    int64_t raw_tmp_epb_1_5c0_ep1_neig_winkel = (int64_t)llround((params::vw_pq.epb_1_5c0_ep1_neig_winkel - -128.0f) / 1.0f);
    uint64_t raw_epb_1_5c0_ep1_neig_winkel = raw_tmp_epb_1_5c0_ep1_neig_winkel < 0 ? 0ULL : (uint64_t)raw_tmp_epb_1_5c0_ep1_neig_winkel;
    if (raw_epb_1_5c0_ep1_neig_winkel > 0xFFULL) raw_epb_1_5c0_ep1_neig_winkel = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_epb_1_5c0_ep1_neig_winkel);
    // EP1_Verzoegerung
    int64_t raw_tmp_epb_1_5c0_ep1_verzoegerung = (int64_t)llround((params::vw_pq.epb_1_5c0_ep1_verzoegerung - -7.968f) / 0.048f);
    uint64_t raw_epb_1_5c0_ep1_verzoegerung = raw_tmp_epb_1_5c0_ep1_verzoegerung < 0 ? 0ULL : (uint64_t)raw_tmp_epb_1_5c0_ep1_verzoegerung;
    if (raw_epb_1_5c0_ep1_verzoegerung > 0xFFULL) raw_epb_1_5c0_ep1_verzoegerung = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_epb_1_5c0_ep1_verzoegerung);
    writeBitsLE(frame.data, 32, 1, params::vw_pq.epb_1_5c0_ep1_fehlereintr ? 1U : 0U);
    writeBitsLE(frame.data, 33, 1, params::vw_pq.epb_1_5c0_ep1_freigabe_ver ? 1U : 0U);
    writeBitsLE(frame.data, 34, 1, params::vw_pq.epb_1_5c0_ep1_autohold_zul ? 1U : 0U);
    writeBitsLE(frame.data, 35, 1, params::vw_pq.epb_1_5c0_ep1_autohold_aktiv ? 1U : 0U);
    writeBitsLE(frame.data, 36, 1, params::vw_pq.epb_1_5c0_ep1_sleepind ? 1U : 0U);
    writeBitsLE(frame.data, 37, 1, params::vw_pq.epb_1_5c0_ep1_status_kl_15 ? 1U : 0U);
    writeBitsLE(frame.data, 38, 1, params::vw_pq.epb_1_5c0_ep1_lampe_autop ? 1U : 0U);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.epb_1_5c0_ep1_bremslicht ? 1U : 0U);
    writeBitsLE(frame.data, 40, 1, params::vw_pq.epb_1_5c0_ep1_warnton1 ? 1U : 0U);
    writeBitsLE(frame.data, 41, 1, params::vw_pq.epb_1_5c0_ep1_warnton2 ? 1U : 0U);
    writeBitsLE(frame.data, 42, 1, params::vw_pq.epb_1_5c0_ep1_anfshlock ? 1U : 0U);
    writeBitsLE(frame.data, 43, 1, params::vw_pq.epb_1_5c0_epb_autoholdlampe ? 1U : 0U);
    writeBitsLE(frame.data, 44, 1, params::vw_pq.epb_1_5c0_ep1_qualneigwi ? 1U : 0U);
    // EP1_KuppModBer
    int64_t raw_tmp_epb_1_5c0_ep1_kuppmodber = (int64_t)llround((params::vw_pq.epb_1_5c0_ep1_kuppmodber - 0.0f) / 1.0f);
    uint64_t raw_epb_1_5c0_ep1_kuppmodber = raw_tmp_epb_1_5c0_ep1_kuppmodber < 0 ? 0ULL : (uint64_t)raw_tmp_epb_1_5c0_ep1_kuppmodber;
    if (raw_epb_1_5c0_ep1_kuppmodber > 0x3ULL) raw_epb_1_5c0_ep1_kuppmodber = 0x3ULL;
    writeBitsLE(frame.data, 45, 2, (uint32_t)raw_epb_1_5c0_ep1_kuppmodber);
    writeBitsLE(frame.data, 47, 1, params::vw_pq.epb_1_5c0_ep1_hydrhalten ? 1U : 0U);
    writeBitsLE(frame.data, 48, 1, params::vw_pq.epb_1_5c0_ep1_fkt_lampe ? 1U : 0U);
    writeBitsLE(frame.data, 49, 1, params::vw_pq.epb_1_5c0_ep1_warnton ? 1U : 0U);
    writeBitsLE(frame.data, 50, 1, params::vw_pq.epb_1_5c0_ep1_fehler_bkl ? 1U : 0U);
    writeBitsLE(frame.data, 51, 1, params::vw_pq.epb_1_5c0_ep1_fehler_gelb ? 1U : 0U);
    // EP1__Text
    int64_t raw_tmp_epb_1_5c0_ep1_text = (int64_t)llround((params::vw_pq.epb_1_5c0_ep1_text - 0.0f) / 1.0f);
    uint64_t raw_epb_1_5c0_ep1_text = raw_tmp_epb_1_5c0_ep1_text < 0 ? 0ULL : (uint64_t)raw_tmp_epb_1_5c0_ep1_text;
    if (raw_epb_1_5c0_ep1_text > 0xFULL) raw_epb_1_5c0_ep1_text = 0xFULL;
    writeBitsLE(frame.data, 52, 4, (uint32_t)raw_epb_1_5c0_ep1_text);
    // CHECKSUM
    int64_t raw_tmp_epb_1_5c0_checksum = (int64_t)llround((params::vw_pq.epb_1_5c0_checksum - 0.0f) / 1.0f);
    uint64_t raw_epb_1_5c0_checksum = raw_tmp_epb_1_5c0_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_epb_1_5c0_checksum;
    if (raw_epb_1_5c0_checksum > 0xFFULL) raw_epb_1_5c0_checksum = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_epb_1_5c0_checksum);
}

// Message 0x52E (1326) - Diag_Lenkhilfe
// DLC: 3, TX: XXX
bool decodeVwPq0x52E(const CANMessage &frame) {
    if (frame.id != 0x52E || frame.len < 3) {
        return false;
    }

    // Werkstattcode__Diag_: start=16 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_diag_lenkhilfe_52e_werkstattcode_diag = readBitsLE(frame.data, 16, 8);
    params::vw_pq.diag_lenkhilfe_52e_werkstattcode_diag = raw_diag_lenkhilfe_52e_werkstattcode_diag * 1.0f + 0.0f;
    // Multiplex_Signal__Diag_: start=13 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_diag_lenkhilfe_52e_multiplex_signal_diag = readBitsLE(frame.data, 13, 3);
    params::vw_pq.diag_lenkhilfe_52e_multiplex_signal_diag = raw_diag_lenkhilfe_52e_multiplex_signal_diag * 1.0f + 0.0f;
    // Befehl_Sensorcodierung_Lenkhilf: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.diag_lenkhilfe_52e_befehl_sensorcodierung_lenkhilf = readBitsLE(frame.data, 12, 1) != 0;
    // Befehl_Kennliniencodierung_Lenk: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_diag_lenkhilfe_52e_befehl_kennliniencodierung_lenk = readBitsLE(frame.data, 8, 4);
    params::vw_pq.diag_lenkhilfe_52e_befehl_kennliniencodierung_lenk = raw_diag_lenkhilfe_52e_befehl_kennliniencodierung_lenk * 1.0f + 0.0f;
    // Befehl_Fehlerspeicher_loeschen: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_diag_lenkhilfe_52e_befehl_fehlerspeicher_loeschen = readBitsLE(frame.data, 0, 8);
    params::vw_pq.diag_lenkhilfe_52e_befehl_fehlerspeicher_loeschen = raw_diag_lenkhilfe_52e_befehl_fehlerspeicher_loeschen * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x52E(CANMessage &frame) {
    frame.id = 0x52E;
    frame.len = 3;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Werkstattcode__Diag_
    int64_t raw_tmp_diag_lenkhilfe_52e_werkstattcode_diag = (int64_t)llround((params::vw_pq.diag_lenkhilfe_52e_werkstattcode_diag - 0.0f) / 1.0f);
    uint64_t raw_diag_lenkhilfe_52e_werkstattcode_diag = raw_tmp_diag_lenkhilfe_52e_werkstattcode_diag < 0 ? 0ULL : (uint64_t)raw_tmp_diag_lenkhilfe_52e_werkstattcode_diag;
    if (raw_diag_lenkhilfe_52e_werkstattcode_diag > 0xFFULL) raw_diag_lenkhilfe_52e_werkstattcode_diag = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_diag_lenkhilfe_52e_werkstattcode_diag);
    // Multiplex_Signal__Diag_
    int64_t raw_tmp_diag_lenkhilfe_52e_multiplex_signal_diag = (int64_t)llround((params::vw_pq.diag_lenkhilfe_52e_multiplex_signal_diag - 0.0f) / 1.0f);
    uint64_t raw_diag_lenkhilfe_52e_multiplex_signal_diag = raw_tmp_diag_lenkhilfe_52e_multiplex_signal_diag < 0 ? 0ULL : (uint64_t)raw_tmp_diag_lenkhilfe_52e_multiplex_signal_diag;
    if (raw_diag_lenkhilfe_52e_multiplex_signal_diag > 0x7ULL) raw_diag_lenkhilfe_52e_multiplex_signal_diag = 0x7ULL;
    writeBitsLE(frame.data, 13, 3, (uint32_t)raw_diag_lenkhilfe_52e_multiplex_signal_diag);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.diag_lenkhilfe_52e_befehl_sensorcodierung_lenkhilf ? 1U : 0U);
    // Befehl_Kennliniencodierung_Lenk
    int64_t raw_tmp_diag_lenkhilfe_52e_befehl_kennliniencodierung_lenk = (int64_t)llround((params::vw_pq.diag_lenkhilfe_52e_befehl_kennliniencodierung_lenk - 0.0f) / 1.0f);
    uint64_t raw_diag_lenkhilfe_52e_befehl_kennliniencodierung_lenk = raw_tmp_diag_lenkhilfe_52e_befehl_kennliniencodierung_lenk < 0 ? 0ULL : (uint64_t)raw_tmp_diag_lenkhilfe_52e_befehl_kennliniencodierung_lenk;
    if (raw_diag_lenkhilfe_52e_befehl_kennliniencodierung_lenk > 0xFULL) raw_diag_lenkhilfe_52e_befehl_kennliniencodierung_lenk = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_diag_lenkhilfe_52e_befehl_kennliniencodierung_lenk);
    // Befehl_Fehlerspeicher_loeschen
    int64_t raw_tmp_diag_lenkhilfe_52e_befehl_fehlerspeicher_loeschen = (int64_t)llround((params::vw_pq.diag_lenkhilfe_52e_befehl_fehlerspeicher_loeschen - 0.0f) / 1.0f);
    uint64_t raw_diag_lenkhilfe_52e_befehl_fehlerspeicher_loeschen = raw_tmp_diag_lenkhilfe_52e_befehl_fehlerspeicher_loeschen < 0 ? 0ULL : (uint64_t)raw_tmp_diag_lenkhilfe_52e_befehl_fehlerspeicher_loeschen;
    if (raw_diag_lenkhilfe_52e_befehl_fehlerspeicher_loeschen > 0xFFULL) raw_diag_lenkhilfe_52e_befehl_fehlerspeicher_loeschen = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_diag_lenkhilfe_52e_befehl_fehlerspeicher_loeschen);
}

// Message 0x598 (1432) - Daempfer_1
// DLC: 2, TX: XXX
bool decodeVwPq0x598(const CANMessage &frame) {
    if (frame.id != 0x598 || frame.len < 2) {
        return false;
    }

    // Frei_Daempfer_1_4: start=12 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_daempfer_1_598_frei_daempfer_1_4 = readBitsLE(frame.data, 12, 4);
    params::vw_pq.daempfer_1_598_frei_daempfer_1_4 = raw_daempfer_1_598_frei_daempfer_1_4 * 1.0f + 0.0f;
    // Textbits_Daempfer: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_daempfer_1_598_textbits_daempfer = readBitsLE(frame.data, 8, 4);
    params::vw_pq.daempfer_1_598_textbits_daempfer = raw_daempfer_1_598_textbits_daempfer * 1.0f + 0.0f;
    // Fehlerspeicherbit__Daempfer_1_: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.daempfer_1_598_fehlerspeicherbit_daempfer_1 = readBitsLE(frame.data, 7, 1) != 0;
    // Systemstatus__Daempfer_1_: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.daempfer_1_598_systemstatus_daempfer_1 = readBitsLE(frame.data, 6, 1) != 0;
    // Frei_Daempfer_1_3: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.daempfer_1_598_frei_daempfer_1_3 = readBitsLE(frame.data, 5, 1) != 0;
    // Status_CDC_Taster: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.daempfer_1_598_status_cdc_taster = readBitsLE(frame.data, 4, 1) != 0;
    // Frei_Daempfer_1_2: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.daempfer_1_598_frei_daempfer_1_2 = readBitsLE(frame.data, 3, 1) != 0;
    // Status_Daempferregelung_4_1: start=0 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_daempfer_1_598_status_daempferregelung_4_1 = readBitsLE(frame.data, 0, 3);
    params::vw_pq.daempfer_1_598_status_daempferregelung_4_1 = raw_daempfer_1_598_status_daempferregelung_4_1 * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x598(CANMessage &frame) {
    frame.id = 0x598;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Frei_Daempfer_1_4
    int64_t raw_tmp_daempfer_1_598_frei_daempfer_1_4 = (int64_t)llround((params::vw_pq.daempfer_1_598_frei_daempfer_1_4 - 0.0f) / 1.0f);
    uint64_t raw_daempfer_1_598_frei_daempfer_1_4 = raw_tmp_daempfer_1_598_frei_daempfer_1_4 < 0 ? 0ULL : (uint64_t)raw_tmp_daempfer_1_598_frei_daempfer_1_4;
    if (raw_daempfer_1_598_frei_daempfer_1_4 > 0xFULL) raw_daempfer_1_598_frei_daempfer_1_4 = 0xFULL;
    writeBitsLE(frame.data, 12, 4, (uint32_t)raw_daempfer_1_598_frei_daempfer_1_4);
    // Textbits_Daempfer
    int64_t raw_tmp_daempfer_1_598_textbits_daempfer = (int64_t)llround((params::vw_pq.daempfer_1_598_textbits_daempfer - 0.0f) / 1.0f);
    uint64_t raw_daempfer_1_598_textbits_daempfer = raw_tmp_daempfer_1_598_textbits_daempfer < 0 ? 0ULL : (uint64_t)raw_tmp_daempfer_1_598_textbits_daempfer;
    if (raw_daempfer_1_598_textbits_daempfer > 0xFULL) raw_daempfer_1_598_textbits_daempfer = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_daempfer_1_598_textbits_daempfer);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.daempfer_1_598_fehlerspeicherbit_daempfer_1 ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.daempfer_1_598_systemstatus_daempfer_1 ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.daempfer_1_598_frei_daempfer_1_3 ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.daempfer_1_598_status_cdc_taster ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.daempfer_1_598_frei_daempfer_1_2 ? 1U : 0U);
    // Status_Daempferregelung_4_1
    int64_t raw_tmp_daempfer_1_598_status_daempferregelung_4_1 = (int64_t)llround((params::vw_pq.daempfer_1_598_status_daempferregelung_4_1 - 0.0f) / 1.0f);
    uint64_t raw_daempfer_1_598_status_daempferregelung_4_1 = raw_tmp_daempfer_1_598_status_daempferregelung_4_1 < 0 ? 0ULL : (uint64_t)raw_tmp_daempfer_1_598_status_daempferregelung_4_1;
    if (raw_daempfer_1_598_status_daempferregelung_4_1 > 0x7ULL) raw_daempfer_1_598_status_daempferregelung_4_1 = 0x7ULL;
    writeBitsLE(frame.data, 0, 3, (uint32_t)raw_daempfer_1_598_status_daempferregelung_4_1);
}

// Message 0x570 (1392) - BSG_Last
// DLC: 4, TX: XXX
bool decodeVwPq0x570(const CANMessage &frame) {
    if (frame.id != 0x570 || frame.len < 4) {
        return false;
    }

    // Klimaanlage_abschalten: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_klimaanlage_abschalten = readBitsLE(frame.data, 31, 1) != 0;
    // Sitzbelueftung_abschalten: start=30 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_sitzbelueftung_abschalten = readBitsLE(frame.data, 30, 1) != 0;
    // Wischwasserheizung_abschalten: start=29 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_wischwasserheizung_abschalten = readBitsLE(frame.data, 29, 1) != 0;
    // Lenkradheizung_abschalten: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_lenkradheizung_abschalten = readBitsLE(frame.data, 28, 1) != 0;
    // Heizbare_Sitze_abschalten: start=27 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_heizbare_sitze_abschalten = readBitsLE(frame.data, 27, 1) != 0;
    // Heizbare_Aussenspiegel_abschalt: start=26 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_heizbare_aussenspiegel_abschalt = readBitsLE(frame.data, 26, 1) != 0;
    // Heizbare_Frontscheibe_abschalte: start=25 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_heizbare_frontscheibe_abschalte = readBitsLE(frame.data, 25, 1) != 0;
    // Heizbare_Heckscheibe_abschalten: start=24 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_heizbare_heckscheibe_abschalten = readBitsLE(frame.data, 24, 1) != 0;
    // Batteriespannung_Bordnetzbatter: start=16 len=8 endian=1 sign=+ factor=0.05 offset=5 unit="V"
    const uint32_t raw_bsg_last_570_batteriespannung_bordnetzbatter = readBitsLE(frame.data, 16, 8);
    params::vw_pq.bsg_last_570_batteriespannung_bordnetzbatter = raw_bsg_last_570_batteriespannung_bordnetzbatter * 0.05f + 5.0f;
    // Motorhaubenkontakt: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_motorhaubenkontakt = readBitsLE(frame.data, 15, 1) != 0;
    // Leuchtweitenregulierung: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_leuchtweitenregulierung = readBitsLE(frame.data, 14, 1) != 0;
    // Fehlerspeichereintrag__BSG_Last: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_fehlerspeichereintrag_bsg_last = readBitsLE(frame.data, 13, 1) != 0;
    // Zustand_der_Starterbatterie: start=11 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bsg_last_570_zustand_der_starterbatterie = readBitsLE(frame.data, 11, 2);
    params::vw_pq.bsg_last_570_zustand_der_starterbatterie = raw_bsg_last_570_zustand_der_starterbatterie * 1.0f + 0.0f;
    // Zustand_der_Bordnetzbatterie: start=9 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bsg_last_570_zustand_der_bordnetzbatterie = readBitsLE(frame.data, 9, 2);
    params::vw_pq.bsg_last_570_zustand_der_bordnetzbatterie = raw_bsg_last_570_zustand_der_bordnetzbatterie * 1.0f + 0.0f;
    // LL_Drehzahlanhebung: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_ll_drehzahlanhebung = readBitsLE(frame.data, 8, 1) != 0;
    // Klemme_L: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_klemme_l = readBitsLE(frame.data, 7, 1) != 0;
    // Frei_BSG_Last_1_1: start=4 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bsg_last_570_frei_bsg_last_1_1 = readBitsLE(frame.data, 4, 3);
    params::vw_pq.bsg_last_570_frei_bsg_last_1_1 = raw_bsg_last_570_frei_bsg_last_1_1 * 1.0f + 0.0f;
    // ZAS_Klemme_50: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_zas_klemme_50 = readBitsLE(frame.data, 3, 1) != 0;
    // ZAS_Klemme_X: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_zas_klemme_x = readBitsLE(frame.data, 2, 1) != 0;
    // ZAS_Klemme_15: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_zas_klemme_15 = readBitsLE(frame.data, 1, 1) != 0;
    // ZAS_Klemme_S: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_last_570_zas_klemme_s = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x570(CANMessage &frame) {
    frame.id = 0x570;
    frame.len = 4;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 31, 1, params::vw_pq.bsg_last_570_klimaanlage_abschalten ? 1U : 0U);
    writeBitsLE(frame.data, 30, 1, params::vw_pq.bsg_last_570_sitzbelueftung_abschalten ? 1U : 0U);
    writeBitsLE(frame.data, 29, 1, params::vw_pq.bsg_last_570_wischwasserheizung_abschalten ? 1U : 0U);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.bsg_last_570_lenkradheizung_abschalten ? 1U : 0U);
    writeBitsLE(frame.data, 27, 1, params::vw_pq.bsg_last_570_heizbare_sitze_abschalten ? 1U : 0U);
    writeBitsLE(frame.data, 26, 1, params::vw_pq.bsg_last_570_heizbare_aussenspiegel_abschalt ? 1U : 0U);
    writeBitsLE(frame.data, 25, 1, params::vw_pq.bsg_last_570_heizbare_frontscheibe_abschalte ? 1U : 0U);
    writeBitsLE(frame.data, 24, 1, params::vw_pq.bsg_last_570_heizbare_heckscheibe_abschalten ? 1U : 0U);
    // Batteriespannung_Bordnetzbatter
    int64_t raw_tmp_bsg_last_570_batteriespannung_bordnetzbatter = (int64_t)llround((params::vw_pq.bsg_last_570_batteriespannung_bordnetzbatter - 5.0f) / 0.05f);
    uint64_t raw_bsg_last_570_batteriespannung_bordnetzbatter = raw_tmp_bsg_last_570_batteriespannung_bordnetzbatter < 0 ? 0ULL : (uint64_t)raw_tmp_bsg_last_570_batteriespannung_bordnetzbatter;
    if (raw_bsg_last_570_batteriespannung_bordnetzbatter > 0xFFULL) raw_bsg_last_570_batteriespannung_bordnetzbatter = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_bsg_last_570_batteriespannung_bordnetzbatter);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.bsg_last_570_motorhaubenkontakt ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.bsg_last_570_leuchtweitenregulierung ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.bsg_last_570_fehlerspeichereintrag_bsg_last ? 1U : 0U);
    // Zustand_der_Starterbatterie
    int64_t raw_tmp_bsg_last_570_zustand_der_starterbatterie = (int64_t)llround((params::vw_pq.bsg_last_570_zustand_der_starterbatterie - 0.0f) / 1.0f);
    uint64_t raw_bsg_last_570_zustand_der_starterbatterie = raw_tmp_bsg_last_570_zustand_der_starterbatterie < 0 ? 0ULL : (uint64_t)raw_tmp_bsg_last_570_zustand_der_starterbatterie;
    if (raw_bsg_last_570_zustand_der_starterbatterie > 0x3ULL) raw_bsg_last_570_zustand_der_starterbatterie = 0x3ULL;
    writeBitsLE(frame.data, 11, 2, (uint32_t)raw_bsg_last_570_zustand_der_starterbatterie);
    // Zustand_der_Bordnetzbatterie
    int64_t raw_tmp_bsg_last_570_zustand_der_bordnetzbatterie = (int64_t)llround((params::vw_pq.bsg_last_570_zustand_der_bordnetzbatterie - 0.0f) / 1.0f);
    uint64_t raw_bsg_last_570_zustand_der_bordnetzbatterie = raw_tmp_bsg_last_570_zustand_der_bordnetzbatterie < 0 ? 0ULL : (uint64_t)raw_tmp_bsg_last_570_zustand_der_bordnetzbatterie;
    if (raw_bsg_last_570_zustand_der_bordnetzbatterie > 0x3ULL) raw_bsg_last_570_zustand_der_bordnetzbatterie = 0x3ULL;
    writeBitsLE(frame.data, 9, 2, (uint32_t)raw_bsg_last_570_zustand_der_bordnetzbatterie);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.bsg_last_570_ll_drehzahlanhebung ? 1U : 0U);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.bsg_last_570_klemme_l ? 1U : 0U);
    // Frei_BSG_Last_1_1
    int64_t raw_tmp_bsg_last_570_frei_bsg_last_1_1 = (int64_t)llround((params::vw_pq.bsg_last_570_frei_bsg_last_1_1 - 0.0f) / 1.0f);
    uint64_t raw_bsg_last_570_frei_bsg_last_1_1 = raw_tmp_bsg_last_570_frei_bsg_last_1_1 < 0 ? 0ULL : (uint64_t)raw_tmp_bsg_last_570_frei_bsg_last_1_1;
    if (raw_bsg_last_570_frei_bsg_last_1_1 > 0x7ULL) raw_bsg_last_570_frei_bsg_last_1_1 = 0x7ULL;
    writeBitsLE(frame.data, 4, 3, (uint32_t)raw_bsg_last_570_frei_bsg_last_1_1);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.bsg_last_570_zas_klemme_50 ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.bsg_last_570_zas_klemme_x ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.bsg_last_570_zas_klemme_15 ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.bsg_last_570_zas_klemme_s ? 1U : 0U);
}

// Message 0x470 (1136) - BSG_Kombi
// DLC: 5, TX: XXX
bool decodeVwPq0x470(const CANMessage &frame) {
    if (frame.id != 0x470 || frame.len < 5) {
        return false;
    }

    // Frei_BSG_Kombi_1_3: start=36 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bsg_kombi_470_frei_bsg_kombi_1_3 = readBitsLE(frame.data, 36, 4);
    params::vw_pq.bsg_kombi_470_frei_bsg_kombi_1_3 = raw_bsg_kombi_470_frei_bsg_kombi_1_3 * 1.0f + 0.0f;
    // Ruecksitzlehne_HR_verr__4_1: start=35 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_ruecksitzlehne_hr_verr_4_1 = readBitsLE(frame.data, 35, 1) != 0;
    // Ruecksitzlehne_HL_verr__4_1: start=34 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_ruecksitzlehne_hl_verr_4_1 = readBitsLE(frame.data, 34, 1) != 0;
    // Fehlerlampe_Lenkhilfe_veraltet: start=33 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_fehlerlampe_lenkhilfe_veraltet = readBitsLE(frame.data, 33, 1) != 0;
    // Fehlerlampe_Lenkhilfe__BSG_Komb: start=32 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_fehlerlampe_lenkhilfe_bsg_komb = readBitsLE(frame.data, 32, 1) != 0;
    // Fehlerstatus_Kl__58s: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_fehlerstatus_kl_58s = readBitsLE(frame.data, 31, 1) != 0;
    // Klemme_58s__BSG_Kombi_: start=24 len=7 endian=1 sign=+ factor=1 offset=0 unit="%"
    const uint32_t raw_bsg_kombi_470_klemme_58s_bsg_kombi = readBitsLE(frame.data, 24, 7);
    params::vw_pq.bsg_kombi_470_klemme_58s_bsg_kombi = raw_bsg_kombi_470_klemme_58s_bsg_kombi * 1.0f + 0.0f;
    // Fehlerstatus_Kl__58d: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_fehlerstatus_kl_58d = readBitsLE(frame.data, 23, 1) != 0;
    // Klemme_58d__BSG_Kombi_: start=16 len=7 endian=1 sign=+ factor=1 offset=0 unit="%"
    const uint32_t raw_bsg_kombi_470_klemme_58d_bsg_kombi = readBitsLE(frame.data, 16, 7);
    params::vw_pq.bsg_kombi_470_klemme_58d_bsg_kombi = raw_bsg_kombi_470_klemme_58d_bsg_kombi * 1.0f + 0.0f;
    // Unterspannung: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_unterspannung = readBitsLE(frame.data, 15, 1) != 0;
    // Frei_BSG_Kombi_1_2: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_frei_bsg_kombi_1_2 = readBitsLE(frame.data, 14, 1) != 0;
    // Heckdeckel_geoeffnet: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_heckdeckel_geoeffnet = readBitsLE(frame.data, 13, 1) != 0;
    // Motorhaube_geoeffnet: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_motorhaube_geoeffnet = readBitsLE(frame.data, 12, 1) != 0;
    // Tuer_hinten_rechts_geoeffnet: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_tuer_hinten_rechts_geoeffnet = readBitsLE(frame.data, 11, 1) != 0;
    // Tuer_hinten_links_geoeffnet: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_tuer_hinten_links_geoeffnet = readBitsLE(frame.data, 10, 1) != 0;
    // Beifahrertuer_geoeffnet: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_beifahrertuer_geoeffnet = readBitsLE(frame.data, 9, 1) != 0;
    // Fahrertuer_geoeffnet: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_fahrertuer_geoeffnet = readBitsLE(frame.data, 8, 1) != 0;
    // Lade_Kontrollampe: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_lade_kontrollampe = readBitsLE(frame.data, 7, 1) != 0;
    // Frei_BSG_Kombi_1_1: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_frei_bsg_kombi_1_1 = readBitsLE(frame.data, 6, 1) != 0;
    // Rueckfahrlicht: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_rueckfahrlicht = readBitsLE(frame.data, 5, 1) != 0;
    // DWA_Akku: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_dwa_akku = readBitsLE(frame.data, 4, 1) != 0;
    // Warnblink_Mode: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_warnblink_mode = readBitsLE(frame.data, 3, 1) != 0;
    // Anhaenger_Kontrollampe: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_anhaenger_kontrollampe = readBitsLE(frame.data, 2, 1) != 0;
    // Blinker_rechts_Kontrollampe: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_blinker_rechts_kontrollampe = readBitsLE(frame.data, 1, 1) != 0;
    // Blinker_links_Kontrollampe: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bsg_kombi_470_blinker_links_kontrollampe = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x470(CANMessage &frame) {
    frame.id = 0x470;
    frame.len = 5;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Frei_BSG_Kombi_1_3
    int64_t raw_tmp_bsg_kombi_470_frei_bsg_kombi_1_3 = (int64_t)llround((params::vw_pq.bsg_kombi_470_frei_bsg_kombi_1_3 - 0.0f) / 1.0f);
    uint64_t raw_bsg_kombi_470_frei_bsg_kombi_1_3 = raw_tmp_bsg_kombi_470_frei_bsg_kombi_1_3 < 0 ? 0ULL : (uint64_t)raw_tmp_bsg_kombi_470_frei_bsg_kombi_1_3;
    if (raw_bsg_kombi_470_frei_bsg_kombi_1_3 > 0xFULL) raw_bsg_kombi_470_frei_bsg_kombi_1_3 = 0xFULL;
    writeBitsLE(frame.data, 36, 4, (uint32_t)raw_bsg_kombi_470_frei_bsg_kombi_1_3);
    writeBitsLE(frame.data, 35, 1, params::vw_pq.bsg_kombi_470_ruecksitzlehne_hr_verr_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 34, 1, params::vw_pq.bsg_kombi_470_ruecksitzlehne_hl_verr_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 33, 1, params::vw_pq.bsg_kombi_470_fehlerlampe_lenkhilfe_veraltet ? 1U : 0U);
    writeBitsLE(frame.data, 32, 1, params::vw_pq.bsg_kombi_470_fehlerlampe_lenkhilfe_bsg_komb ? 1U : 0U);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.bsg_kombi_470_fehlerstatus_kl_58s ? 1U : 0U);
    // Klemme_58s__BSG_Kombi_
    int64_t raw_tmp_bsg_kombi_470_klemme_58s_bsg_kombi = (int64_t)llround((params::vw_pq.bsg_kombi_470_klemme_58s_bsg_kombi - 0.0f) / 1.0f);
    uint64_t raw_bsg_kombi_470_klemme_58s_bsg_kombi = raw_tmp_bsg_kombi_470_klemme_58s_bsg_kombi < 0 ? 0ULL : (uint64_t)raw_tmp_bsg_kombi_470_klemme_58s_bsg_kombi;
    if (raw_bsg_kombi_470_klemme_58s_bsg_kombi > 0x7FULL) raw_bsg_kombi_470_klemme_58s_bsg_kombi = 0x7FULL;
    writeBitsLE(frame.data, 24, 7, (uint32_t)raw_bsg_kombi_470_klemme_58s_bsg_kombi);
    writeBitsLE(frame.data, 23, 1, params::vw_pq.bsg_kombi_470_fehlerstatus_kl_58d ? 1U : 0U);
    // Klemme_58d__BSG_Kombi_
    int64_t raw_tmp_bsg_kombi_470_klemme_58d_bsg_kombi = (int64_t)llround((params::vw_pq.bsg_kombi_470_klemme_58d_bsg_kombi - 0.0f) / 1.0f);
    uint64_t raw_bsg_kombi_470_klemme_58d_bsg_kombi = raw_tmp_bsg_kombi_470_klemme_58d_bsg_kombi < 0 ? 0ULL : (uint64_t)raw_tmp_bsg_kombi_470_klemme_58d_bsg_kombi;
    if (raw_bsg_kombi_470_klemme_58d_bsg_kombi > 0x7FULL) raw_bsg_kombi_470_klemme_58d_bsg_kombi = 0x7FULL;
    writeBitsLE(frame.data, 16, 7, (uint32_t)raw_bsg_kombi_470_klemme_58d_bsg_kombi);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.bsg_kombi_470_unterspannung ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.bsg_kombi_470_frei_bsg_kombi_1_2 ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.bsg_kombi_470_heckdeckel_geoeffnet ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.bsg_kombi_470_motorhaube_geoeffnet ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.bsg_kombi_470_tuer_hinten_rechts_geoeffnet ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.bsg_kombi_470_tuer_hinten_links_geoeffnet ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.bsg_kombi_470_beifahrertuer_geoeffnet ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.bsg_kombi_470_fahrertuer_geoeffnet ? 1U : 0U);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.bsg_kombi_470_lade_kontrollampe ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.bsg_kombi_470_frei_bsg_kombi_1_1 ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.bsg_kombi_470_rueckfahrlicht ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.bsg_kombi_470_dwa_akku ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.bsg_kombi_470_warnblink_mode ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.bsg_kombi_470_anhaenger_kontrollampe ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.bsg_kombi_470_blinker_rechts_kontrollampe ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.bsg_kombi_470_blinker_links_kontrollampe ? 1U : 0U);
}

// Message 0x1A8 (424) - Bremse_6
// DLC: 3, TX: XXX
bool decodeVwPq0x1A8(const CANMessage &frame) {
    if (frame.id != 0x1A8 || frame.len < 3) {
        return false;
    }

    // Checksumme_Bremse_6: start=16 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_6_1a8_checksumme_bremse_6 = readBitsLE(frame.data, 16, 8);
    params::vw_pq.bremse_6_1a8_checksumme_bremse_6 = raw_bremse_6_1a8_checksumme_bremse_6 * 1.0f + 0.0f;
    // Zaehler_Bremse_6: start=12 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_6_1a8_zaehler_bremse_6 = readBitsLE(frame.data, 12, 4);
    params::vw_pq.bremse_6_1a8_zaehler_bremse_6 = raw_bremse_6_1a8_zaehler_bremse_6 * 1.0f + 0.0f;
    // Status_Bremsdruck__Bremse_6__du: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_6_1a8_status_bremsdruck_bremse_6_du = readBitsLE(frame.data, 11, 1) != 0;
    // Frei_Bremse_6_1: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_6_1a8_frei_bremse_6_1 = readBitsLE(frame.data, 10, 1) != 0;
    // Bremsdruck__Bremse_6_: start=0 len=10 endian=1 sign=+ factor=0.3255 offset=-40 unit="bar"
    const uint32_t raw_bremse_6_1a8_bremsdruck_bremse_6 = readBitsLE(frame.data, 0, 10);
    params::vw_pq.bremse_6_1a8_bremsdruck_bremse_6 = raw_bremse_6_1a8_bremsdruck_bremse_6 * 0.3255f + -40.0f;
    return true;
}

void encodeVwPq0x1A8(CANMessage &frame) {
    frame.id = 0x1A8;
    frame.len = 3;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Checksumme_Bremse_6
    int64_t raw_tmp_bremse_6_1a8_checksumme_bremse_6 = (int64_t)llround((params::vw_pq.bremse_6_1a8_checksumme_bremse_6 - 0.0f) / 1.0f);
    uint64_t raw_bremse_6_1a8_checksumme_bremse_6 = raw_tmp_bremse_6_1a8_checksumme_bremse_6 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_6_1a8_checksumme_bremse_6;
    if (raw_bremse_6_1a8_checksumme_bremse_6 > 0xFFULL) raw_bremse_6_1a8_checksumme_bremse_6 = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_bremse_6_1a8_checksumme_bremse_6);
    // Zaehler_Bremse_6
    int64_t raw_tmp_bremse_6_1a8_zaehler_bremse_6 = (int64_t)llround((params::vw_pq.bremse_6_1a8_zaehler_bremse_6 - 0.0f) / 1.0f);
    uint64_t raw_bremse_6_1a8_zaehler_bremse_6 = raw_tmp_bremse_6_1a8_zaehler_bremse_6 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_6_1a8_zaehler_bremse_6;
    if (raw_bremse_6_1a8_zaehler_bremse_6 > 0xFULL) raw_bremse_6_1a8_zaehler_bremse_6 = 0xFULL;
    writeBitsLE(frame.data, 12, 4, (uint32_t)raw_bremse_6_1a8_zaehler_bremse_6);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.bremse_6_1a8_status_bremsdruck_bremse_6_du ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.bremse_6_1a8_frei_bremse_6_1 ? 1U : 0U);
    // Bremsdruck__Bremse_6_
    int64_t raw_tmp_bremse_6_1a8_bremsdruck_bremse_6 = (int64_t)llround((params::vw_pq.bremse_6_1a8_bremsdruck_bremse_6 - -40.0f) / 0.3255f);
    uint64_t raw_bremse_6_1a8_bremsdruck_bremse_6 = raw_tmp_bremse_6_1a8_bremsdruck_bremse_6 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_6_1a8_bremsdruck_bremse_6;
    if (raw_bremse_6_1a8_bremsdruck_bremse_6 > 0x3FFULL) raw_bremse_6_1a8_bremsdruck_bremse_6 = 0x3FFULL;
    writeBitsLE(frame.data, 0, 10, (uint32_t)raw_bremse_6_1a8_bremsdruck_bremse_6);
}

// Message 0x4A8 (1192) - Bremse_5
// DLC: 8, TX: XXX
bool decodeVwPq0x4A8(const CANMessage &frame) {
    if (frame.id != 0x4A8 || frame.len < 8) {
        return false;
    }

    // CHECKSUM: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_5_4a8_checksum = readBitsLE(frame.data, 56, 8);
    params::vw_pq.bremse_5_4a8_checksum = raw_bremse_5_4a8_checksum * 1.0f + 0.0f;
    // COUNTER: start=52 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_5_4a8_counter = readBitsLE(frame.data, 52, 4);
    params::vw_pq.bremse_5_4a8_counter = raw_bremse_5_4a8_counter * 1.0f + 0.0f;
    // BR5_ECD_Lampe: start=51 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_ecd_lampe = readBitsLE(frame.data, 51, 1) != 0;
    // BR5_ZT_Rueckk_Umsetz: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_zt_rueckk_umsetz = readBitsLE(frame.data, 48, 1) != 0;
    // BR5_Anhi_Sta: start=40 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_anhi_sta = readBitsLE(frame.data, 40, 1) != 0;
    // ESP_Rollenmodus_Deactiveieren: start=34 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_esp_rollenmodus_deactiveieren = readBitsLE(frame.data, 34, 1) != 0;
    // BR5_Sign_Druck: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_sign_druck = readBitsLE(frame.data, 31, 1) != 0;
    // BR5_Sta_Druck: start=30 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_sta_druck = readBitsLE(frame.data, 30, 1) != 0;
    // BR5_Druckvalid: start=29 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_druckvalid = readBitsLE(frame.data, 29, 1) != 0;
    // BR5_Stillstand: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_stillstand = readBitsLE(frame.data, 28, 1) != 0;
    // BR5_Bremsdruck: start=16 len=12 endian=1 sign=+ factor=0.1 offset=0 unit="bar"
    const uint32_t raw_bremse_5_4a8_br5_bremsdruck = readBitsLE(frame.data, 16, 12);
    params::vw_pq.bremse_5_4a8_br5_bremsdruck = raw_bremse_5_4a8_br5_bremsdruck * 0.1f + 0.0f;
    // BR5_Vorzeichen: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_vorzeichen = readBitsLE(frame.data, 15, 1) != 0;
    // BR5_Sta_Gierrate: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_sta_gierrate = readBitsLE(frame.data, 14, 1) != 0;
    // BR5_Giergeschw: start=0 len=14 endian=1 sign=+ factor=0.01 offset=0 unit="Grad/sec"
    const uint32_t raw_bremse_5_4a8_br5_giergeschw = readBitsLE(frame.data, 0, 14);
    params::vw_pq.bremse_5_4a8_br5_giergeschw = raw_bremse_5_4a8_br5_giergeschw * 0.01f + 0.0f;
    // BR5_ANB_CM_Rueckk_Umsetz: start=49 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_anb_cm_rueckk_umsetz = readBitsLE(frame.data, 49, 1) != 0;
    // BR5_HDC_bereit: start=50 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_hdc_bereit = readBitsLE(frame.data, 50, 1) != 0;
    // ESP_Stat_FallBack_eBKV: start=35 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_esp_stat_fallback_ebkv = readBitsLE(frame.data, 35, 1) != 0;
    // ESP_Anforderung_EPB: start=36 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_5_4a8_esp_anforderung_epb = readBitsLE(frame.data, 36, 2);
    params::vw_pq.bremse_5_4a8_esp_anforderung_epb = raw_bremse_5_4a8_esp_anforderung_epb * 1.0f + 0.0f;
    // ESP_Autohold_active: start=38 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_esp_autohold_active = readBitsLE(frame.data, 38, 1) != 0;
    // ESP_Autohold_Standby: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_esp_autohold_standby = readBitsLE(frame.data, 39, 1) != 0;
    // BR5_Anhi_akt: start=41 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_anhi_akt = readBitsLE(frame.data, 41, 1) != 0;
    // BR5_v_Ueberw: start=42 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_v_ueberw = readBitsLE(frame.data, 42, 1) != 0;
    // BR5_Bremslicht: start=43 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_bremslicht = readBitsLE(frame.data, 43, 1) != 0;
    // BR5_Notbremsung: start=44 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_notbremsung = readBitsLE(frame.data, 44, 1) != 0;
    // BR5_Fahrer_tritt_ZBR_Schw: start=45 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_fahrer_tritt_zbr_schw = readBitsLE(frame.data, 45, 1) != 0;
    // BR5_AWV2_Bremsruck: start=46 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_awv2_bremsruck = readBitsLE(frame.data, 46, 1) != 0;
    // BR5_AWV2_Fehler: start=47 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_5_4a8_br5_awv2_fehler = readBitsLE(frame.data, 47, 1) != 0;
    return true;
}

void encodeVwPq0x4A8(CANMessage &frame) {
    frame.id = 0x4A8;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // CHECKSUM
    int64_t raw_tmp_bremse_5_4a8_checksum = (int64_t)llround((params::vw_pq.bremse_5_4a8_checksum - 0.0f) / 1.0f);
    uint64_t raw_bremse_5_4a8_checksum = raw_tmp_bremse_5_4a8_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_5_4a8_checksum;
    if (raw_bremse_5_4a8_checksum > 0xFFULL) raw_bremse_5_4a8_checksum = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_bremse_5_4a8_checksum);
    // COUNTER
    int64_t raw_tmp_bremse_5_4a8_counter = (int64_t)llround((params::vw_pq.bremse_5_4a8_counter - 0.0f) / 1.0f);
    uint64_t raw_bremse_5_4a8_counter = raw_tmp_bremse_5_4a8_counter < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_5_4a8_counter;
    if (raw_bremse_5_4a8_counter > 0xFULL) raw_bremse_5_4a8_counter = 0xFULL;
    writeBitsLE(frame.data, 52, 4, (uint32_t)raw_bremse_5_4a8_counter);
    writeBitsLE(frame.data, 51, 1, params::vw_pq.bremse_5_4a8_br5_ecd_lampe ? 1U : 0U);
    writeBitsLE(frame.data, 48, 1, params::vw_pq.bremse_5_4a8_br5_zt_rueckk_umsetz ? 1U : 0U);
    writeBitsLE(frame.data, 40, 1, params::vw_pq.bremse_5_4a8_br5_anhi_sta ? 1U : 0U);
    writeBitsLE(frame.data, 34, 1, params::vw_pq.bremse_5_4a8_esp_rollenmodus_deactiveieren ? 1U : 0U);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.bremse_5_4a8_br5_sign_druck ? 1U : 0U);
    writeBitsLE(frame.data, 30, 1, params::vw_pq.bremse_5_4a8_br5_sta_druck ? 1U : 0U);
    writeBitsLE(frame.data, 29, 1, params::vw_pq.bremse_5_4a8_br5_druckvalid ? 1U : 0U);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.bremse_5_4a8_br5_stillstand ? 1U : 0U);
    // BR5_Bremsdruck
    int64_t raw_tmp_bremse_5_4a8_br5_bremsdruck = (int64_t)llround((params::vw_pq.bremse_5_4a8_br5_bremsdruck - 0.0f) / 0.1f);
    uint64_t raw_bremse_5_4a8_br5_bremsdruck = raw_tmp_bremse_5_4a8_br5_bremsdruck < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_5_4a8_br5_bremsdruck;
    if (raw_bremse_5_4a8_br5_bremsdruck > 0xFFFULL) raw_bremse_5_4a8_br5_bremsdruck = 0xFFFULL;
    writeBitsLE(frame.data, 16, 12, (uint32_t)raw_bremse_5_4a8_br5_bremsdruck);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.bremse_5_4a8_br5_vorzeichen ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.bremse_5_4a8_br5_sta_gierrate ? 1U : 0U);
    // BR5_Giergeschw
    int64_t raw_tmp_bremse_5_4a8_br5_giergeschw = (int64_t)llround((params::vw_pq.bremse_5_4a8_br5_giergeschw - 0.0f) / 0.01f);
    uint64_t raw_bremse_5_4a8_br5_giergeschw = raw_tmp_bremse_5_4a8_br5_giergeschw < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_5_4a8_br5_giergeschw;
    if (raw_bremse_5_4a8_br5_giergeschw > 0x3FFFULL) raw_bremse_5_4a8_br5_giergeschw = 0x3FFFULL;
    writeBitsLE(frame.data, 0, 14, (uint32_t)raw_bremse_5_4a8_br5_giergeschw);
    writeBitsLE(frame.data, 49, 1, params::vw_pq.bremse_5_4a8_br5_anb_cm_rueckk_umsetz ? 1U : 0U);
    writeBitsLE(frame.data, 50, 1, params::vw_pq.bremse_5_4a8_br5_hdc_bereit ? 1U : 0U);
    writeBitsLE(frame.data, 35, 1, params::vw_pq.bremse_5_4a8_esp_stat_fallback_ebkv ? 1U : 0U);
    // ESP_Anforderung_EPB
    int64_t raw_tmp_bremse_5_4a8_esp_anforderung_epb = (int64_t)llround((params::vw_pq.bremse_5_4a8_esp_anforderung_epb - 0.0f) / 1.0f);
    uint64_t raw_bremse_5_4a8_esp_anforderung_epb = raw_tmp_bremse_5_4a8_esp_anforderung_epb < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_5_4a8_esp_anforderung_epb;
    if (raw_bremse_5_4a8_esp_anforderung_epb > 0x3ULL) raw_bremse_5_4a8_esp_anforderung_epb = 0x3ULL;
    writeBitsLE(frame.data, 36, 2, (uint32_t)raw_bremse_5_4a8_esp_anforderung_epb);
    writeBitsLE(frame.data, 38, 1, params::vw_pq.bremse_5_4a8_esp_autohold_active ? 1U : 0U);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.bremse_5_4a8_esp_autohold_standby ? 1U : 0U);
    writeBitsLE(frame.data, 41, 1, params::vw_pq.bremse_5_4a8_br5_anhi_akt ? 1U : 0U);
    writeBitsLE(frame.data, 42, 1, params::vw_pq.bremse_5_4a8_br5_v_ueberw ? 1U : 0U);
    writeBitsLE(frame.data, 43, 1, params::vw_pq.bremse_5_4a8_br5_bremslicht ? 1U : 0U);
    writeBitsLE(frame.data, 44, 1, params::vw_pq.bremse_5_4a8_br5_notbremsung ? 1U : 0U);
    writeBitsLE(frame.data, 45, 1, params::vw_pq.bremse_5_4a8_br5_fahrer_tritt_zbr_schw ? 1U : 0U);
    writeBitsLE(frame.data, 46, 1, params::vw_pq.bremse_5_4a8_br5_awv2_bremsruck ? 1U : 0U);
    writeBitsLE(frame.data, 47, 1, params::vw_pq.bremse_5_4a8_br5_awv2_fehler ? 1U : 0U);
}

// Message 0x2A0 (672) - Bremse_4
// DLC: 3, TX: XXX
bool decodeVwPq0x2A0(const CANMessage &frame) {
    if (frame.id != 0x2A0 || frame.len < 3) {
        return false;
    }

    // Frei_Bremse_4_1: start=17 len=7 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_4_2a0_frei_bremse_4_1 = readBitsLE(frame.data, 17, 7);
    params::vw_pq.bremse_4_2a0_frei_bremse_4_1 = raw_bremse_4_2a0_frei_bremse_4_1 * 1.0f + 0.0f;
    // Einheit_Kupplungssteifigkeit: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_4_2a0_einheit_kupplungssteifigkeit = readBitsLE(frame.data, 16, 1) != 0;
    // ABS_Vorgabewert_hinten_Kupplung: start=8 len=8 endian=1 sign=+ factor=0.7874 offset=0 unit="%"
    const uint32_t raw_bremse_4_2a0_abs_vorgabewert_hinten_kupplung = readBitsLE(frame.data, 8, 8);
    params::vw_pq.bremse_4_2a0_abs_vorgabewert_hinten_kupplung = raw_bremse_4_2a0_abs_vorgabewert_hinten_kupplung * 0.7874f + 0.0f;
    // ABS_Vorgabewert_mitte_Kupplungs: start=0 len=8 endian=1 sign=+ factor=3 offset=-381 unit="Nm/min"
    const uint32_t raw_bremse_4_2a0_abs_vorgabewert_mitte_kupplungs = readBitsLE(frame.data, 0, 8);
    params::vw_pq.bremse_4_2a0_abs_vorgabewert_mitte_kupplungs = raw_bremse_4_2a0_abs_vorgabewert_mitte_kupplungs * 3.0f + -381.0f;
    return true;
}

void encodeVwPq0x2A0(CANMessage &frame) {
    frame.id = 0x2A0;
    frame.len = 3;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Frei_Bremse_4_1
    int64_t raw_tmp_bremse_4_2a0_frei_bremse_4_1 = (int64_t)llround((params::vw_pq.bremse_4_2a0_frei_bremse_4_1 - 0.0f) / 1.0f);
    uint64_t raw_bremse_4_2a0_frei_bremse_4_1 = raw_tmp_bremse_4_2a0_frei_bremse_4_1 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_4_2a0_frei_bremse_4_1;
    if (raw_bremse_4_2a0_frei_bremse_4_1 > 0x7FULL) raw_bremse_4_2a0_frei_bremse_4_1 = 0x7FULL;
    writeBitsLE(frame.data, 17, 7, (uint32_t)raw_bremse_4_2a0_frei_bremse_4_1);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.bremse_4_2a0_einheit_kupplungssteifigkeit ? 1U : 0U);
    // ABS_Vorgabewert_hinten_Kupplung
    int64_t raw_tmp_bremse_4_2a0_abs_vorgabewert_hinten_kupplung = (int64_t)llround((params::vw_pq.bremse_4_2a0_abs_vorgabewert_hinten_kupplung - 0.0f) / 0.7874f);
    uint64_t raw_bremse_4_2a0_abs_vorgabewert_hinten_kupplung = raw_tmp_bremse_4_2a0_abs_vorgabewert_hinten_kupplung < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_4_2a0_abs_vorgabewert_hinten_kupplung;
    if (raw_bremse_4_2a0_abs_vorgabewert_hinten_kupplung > 0xFFULL) raw_bremse_4_2a0_abs_vorgabewert_hinten_kupplung = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_bremse_4_2a0_abs_vorgabewert_hinten_kupplung);
    // ABS_Vorgabewert_mitte_Kupplungs
    int64_t raw_tmp_bremse_4_2a0_abs_vorgabewert_mitte_kupplungs = (int64_t)llround((params::vw_pq.bremse_4_2a0_abs_vorgabewert_mitte_kupplungs - -381.0f) / 3.0f);
    uint64_t raw_bremse_4_2a0_abs_vorgabewert_mitte_kupplungs = raw_tmp_bremse_4_2a0_abs_vorgabewert_mitte_kupplungs < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_4_2a0_abs_vorgabewert_mitte_kupplungs;
    if (raw_bremse_4_2a0_abs_vorgabewert_mitte_kupplungs > 0xFFULL) raw_bremse_4_2a0_abs_vorgabewert_mitte_kupplungs = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_bremse_4_2a0_abs_vorgabewert_mitte_kupplungs);
}

// Message 0x4A0 (1184) - Bremse_3
// DLC: 8, TX: XXX
bool decodeVwPq0x4A0(const CANMessage &frame) {
    if (frame.id != 0x4A0 || frame.len < 8) {
        return false;
    }

    // Radgeschw__HR_4_1: start=49 len=15 endian=1 sign=+ factor=0.01 offset=0 unit="km/h"
    const uint32_t raw_bremse_3_4a0_radgeschw_hr_4_1 = readBitsLE(frame.data, 49, 15);
    params::vw_pq.bremse_3_4a0_radgeschw_hr_4_1 = raw_bremse_3_4a0_radgeschw_hr_4_1 * 0.01f + 0.0f;
    // Frei_Bremse_3_4: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_3_4a0_frei_bremse_3_4 = readBitsLE(frame.data, 48, 1) != 0;
    // Radgeschw__HL_4_1: start=33 len=15 endian=1 sign=+ factor=0.01 offset=0 unit="km/h"
    const uint32_t raw_bremse_3_4a0_radgeschw_hl_4_1 = readBitsLE(frame.data, 33, 15);
    params::vw_pq.bremse_3_4a0_radgeschw_hl_4_1 = raw_bremse_3_4a0_radgeschw_hl_4_1 * 0.01f + 0.0f;
    // Frei_Bremse_3_3: start=32 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_3_4a0_frei_bremse_3_3 = readBitsLE(frame.data, 32, 1) != 0;
    // Radgeschw__VR_4_1: start=17 len=15 endian=1 sign=+ factor=0.01 offset=0 unit="km/h"
    const uint32_t raw_bremse_3_4a0_radgeschw_vr_4_1 = readBitsLE(frame.data, 17, 15);
    params::vw_pq.bremse_3_4a0_radgeschw_vr_4_1 = raw_bremse_3_4a0_radgeschw_vr_4_1 * 0.01f + 0.0f;
    // Frei_Bremse_3_2: start=16 len=1 endian=1 sign=+ factor=0.01 offset=0 unit="km/h"
    params::vw_pq.bremse_3_4a0_frei_bremse_3_2 = readBitsLE(frame.data, 16, 1) != 0;
    // Radgeschw__VL_4_1: start=1 len=15 endian=1 sign=+ factor=0.01 offset=0 unit="km/h"
    const uint32_t raw_bremse_3_4a0_radgeschw_vl_4_1 = readBitsLE(frame.data, 1, 15);
    params::vw_pq.bremse_3_4a0_radgeschw_vl_4_1 = raw_bremse_3_4a0_radgeschw_vl_4_1 * 0.01f + 0.0f;
    // Frei_Bremse_3_1: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_3_4a0_frei_bremse_3_1 = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x4A0(CANMessage &frame) {
    frame.id = 0x4A0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Radgeschw__HR_4_1
    int64_t raw_tmp_bremse_3_4a0_radgeschw_hr_4_1 = (int64_t)llround((params::vw_pq.bremse_3_4a0_radgeschw_hr_4_1 - 0.0f) / 0.01f);
    uint64_t raw_bremse_3_4a0_radgeschw_hr_4_1 = raw_tmp_bremse_3_4a0_radgeschw_hr_4_1 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_3_4a0_radgeschw_hr_4_1;
    if (raw_bremse_3_4a0_radgeschw_hr_4_1 > 0x7FFFULL) raw_bremse_3_4a0_radgeschw_hr_4_1 = 0x7FFFULL;
    writeBitsLE(frame.data, 49, 15, (uint32_t)raw_bremse_3_4a0_radgeschw_hr_4_1);
    writeBitsLE(frame.data, 48, 1, params::vw_pq.bremse_3_4a0_frei_bremse_3_4 ? 1U : 0U);
    // Radgeschw__HL_4_1
    int64_t raw_tmp_bremse_3_4a0_radgeschw_hl_4_1 = (int64_t)llround((params::vw_pq.bremse_3_4a0_radgeschw_hl_4_1 - 0.0f) / 0.01f);
    uint64_t raw_bremse_3_4a0_radgeschw_hl_4_1 = raw_tmp_bremse_3_4a0_radgeschw_hl_4_1 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_3_4a0_radgeschw_hl_4_1;
    if (raw_bremse_3_4a0_radgeschw_hl_4_1 > 0x7FFFULL) raw_bremse_3_4a0_radgeschw_hl_4_1 = 0x7FFFULL;
    writeBitsLE(frame.data, 33, 15, (uint32_t)raw_bremse_3_4a0_radgeschw_hl_4_1);
    writeBitsLE(frame.data, 32, 1, params::vw_pq.bremse_3_4a0_frei_bremse_3_3 ? 1U : 0U);
    // Radgeschw__VR_4_1
    int64_t raw_tmp_bremse_3_4a0_radgeschw_vr_4_1 = (int64_t)llround((params::vw_pq.bremse_3_4a0_radgeschw_vr_4_1 - 0.0f) / 0.01f);
    uint64_t raw_bremse_3_4a0_radgeschw_vr_4_1 = raw_tmp_bremse_3_4a0_radgeschw_vr_4_1 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_3_4a0_radgeschw_vr_4_1;
    if (raw_bremse_3_4a0_radgeschw_vr_4_1 > 0x7FFFULL) raw_bremse_3_4a0_radgeschw_vr_4_1 = 0x7FFFULL;
    writeBitsLE(frame.data, 17, 15, (uint32_t)raw_bremse_3_4a0_radgeschw_vr_4_1);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.bremse_3_4a0_frei_bremse_3_2 ? 1U : 0U);
    // Radgeschw__VL_4_1
    int64_t raw_tmp_bremse_3_4a0_radgeschw_vl_4_1 = (int64_t)llround((params::vw_pq.bremse_3_4a0_radgeschw_vl_4_1 - 0.0f) / 0.01f);
    uint64_t raw_bremse_3_4a0_radgeschw_vl_4_1 = raw_tmp_bremse_3_4a0_radgeschw_vl_4_1 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_3_4a0_radgeschw_vl_4_1;
    if (raw_bremse_3_4a0_radgeschw_vl_4_1 > 0x7FFFULL) raw_bremse_3_4a0_radgeschw_vl_4_1 = 0x7FFFULL;
    writeBitsLE(frame.data, 1, 15, (uint32_t)raw_bremse_3_4a0_radgeschw_vl_4_1);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.bremse_3_4a0_frei_bremse_3_1 ? 1U : 0U);
}

// Message 0x5A0 (1440) - Bremse_2
// DLC: 8, TX: XXX
bool decodeVwPq0x5A0(const CANMessage &frame) {
    if (frame.id != 0x5A0 || frame.len < 8) {
        return false;
    }

    // gemessene_Querbeschleunigung: start=63 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_2_5a0_gemessene_querbeschleunigung = readBitsLE(frame.data, 63, 1) != 0;
    // Frei_Bremse_2_2: start=62 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_2_5a0_frei_bremse_2_2 = readBitsLE(frame.data, 62, 1) != 0;
    // Impulszahl: start=56 len=6 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_2_5a0_impulszahl = readBitsLE(frame.data, 56, 6);
    params::vw_pq.bremse_2_5a0_impulszahl = raw_bremse_2_5a0_impulszahl * 1.0f + 0.0f;
    // Fehlerstatus_Wegimpulse_4_1: start=55 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_2_5a0_fehlerstatus_wegimpulse_4_1 = readBitsLE(frame.data, 55, 1) != 0;
    // Frei_Bremse_2_5: start=54 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_2_5a0_frei_bremse_2_5 = readBitsLE(frame.data, 54, 1) != 0;
    // Warnlampe_DDS: start=53 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_2_5a0_warnlampe_dds = readBitsLE(frame.data, 53, 1) != 0;
    // Fehlerspeichereintrag_Bremse: start=52 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_2_5a0_fehlerspeichereintrag_bremse = readBitsLE(frame.data, 52, 1) != 0;
    // Wegimpulszaehlerstatus: start=51 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_2_5a0_wegimpulszaehlerstatus = readBitsLE(frame.data, 51, 1) != 0;
    // Wegimpulse_Vorderachse: start=40 len=11 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_2_5a0_wegimpulse_vorderachse = readBitsLE(frame.data, 40, 11);
    params::vw_pq.bremse_2_5a0_wegimpulse_vorderachse = raw_bremse_2_5a0_wegimpulse_vorderachse * 1.0f + 0.0f;
    // Zeitstempel: start=24 len=16 endian=1 sign=+ factor=1 offset=0 unit="tics"
    const uint32_t raw_bremse_2_5a0_zeitstempel = readBitsLE(frame.data, 24, 16);
    params::vw_pq.bremse_2_5a0_zeitstempel = raw_bremse_2_5a0_zeitstempel * 1.0f + 0.0f;
    // mittlere_Raddrehzahl__Bremse_2: start=9 len=15 endian=1 sign=+ factor=0.002 offset=0 unit="U/sec"
    const uint32_t raw_bremse_2_5a0_mittlere_raddrehzahl_bremse_2 = readBitsLE(frame.data, 9, 15);
    params::vw_pq.bremse_2_5a0_mittlere_raddrehzahl_bremse_2 = raw_bremse_2_5a0_mittlere_raddrehzahl_bremse_2 * 0.002f + 0.0f;
    // Querbeschl__TimerTic M: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_2_5a0_querbeschl_timertic_m = readBitsLE(frame.data, 8, 1) != 0;
    // Timer m1: start=0 len=8 endian=1 sign=+ factor=0.04 offset=0 unit="usec"
    const uint32_t raw_bremse_2_5a0_timer_m1 = readBitsLE(frame.data, 0, 8);
    params::vw_pq.bremse_2_5a0_timer_m1 = raw_bremse_2_5a0_timer_m1 * 0.04f + 0.0f;
    // Querbeschleunigung m0: start=0 len=8 endian=1 sign=+ factor=0.01 offset=-1.27 unit="g"
    const uint32_t raw_bremse_2_5a0_querbeschleunigung_m0 = readBitsLE(frame.data, 0, 8);
    params::vw_pq.bremse_2_5a0_querbeschleunigung_m0 = raw_bremse_2_5a0_querbeschleunigung_m0 * 0.01f + -1.27f;
    return true;
}

void encodeVwPq0x5A0(CANMessage &frame) {
    frame.id = 0x5A0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 63, 1, params::vw_pq.bremse_2_5a0_gemessene_querbeschleunigung ? 1U : 0U);
    writeBitsLE(frame.data, 62, 1, params::vw_pq.bremse_2_5a0_frei_bremse_2_2 ? 1U : 0U);
    // Impulszahl
    int64_t raw_tmp_bremse_2_5a0_impulszahl = (int64_t)llround((params::vw_pq.bremse_2_5a0_impulszahl - 0.0f) / 1.0f);
    uint64_t raw_bremse_2_5a0_impulszahl = raw_tmp_bremse_2_5a0_impulszahl < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_2_5a0_impulszahl;
    if (raw_bremse_2_5a0_impulszahl > 0x3FULL) raw_bremse_2_5a0_impulszahl = 0x3FULL;
    writeBitsLE(frame.data, 56, 6, (uint32_t)raw_bremse_2_5a0_impulszahl);
    writeBitsLE(frame.data, 55, 1, params::vw_pq.bremse_2_5a0_fehlerstatus_wegimpulse_4_1 ? 1U : 0U);
    writeBitsLE(frame.data, 54, 1, params::vw_pq.bremse_2_5a0_frei_bremse_2_5 ? 1U : 0U);
    writeBitsLE(frame.data, 53, 1, params::vw_pq.bremse_2_5a0_warnlampe_dds ? 1U : 0U);
    writeBitsLE(frame.data, 52, 1, params::vw_pq.bremse_2_5a0_fehlerspeichereintrag_bremse ? 1U : 0U);
    writeBitsLE(frame.data, 51, 1, params::vw_pq.bremse_2_5a0_wegimpulszaehlerstatus ? 1U : 0U);
    // Wegimpulse_Vorderachse
    int64_t raw_tmp_bremse_2_5a0_wegimpulse_vorderachse = (int64_t)llround((params::vw_pq.bremse_2_5a0_wegimpulse_vorderachse - 0.0f) / 1.0f);
    uint64_t raw_bremse_2_5a0_wegimpulse_vorderachse = raw_tmp_bremse_2_5a0_wegimpulse_vorderachse < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_2_5a0_wegimpulse_vorderachse;
    if (raw_bremse_2_5a0_wegimpulse_vorderachse > 0x7FFULL) raw_bremse_2_5a0_wegimpulse_vorderachse = 0x7FFULL;
    writeBitsLE(frame.data, 40, 11, (uint32_t)raw_bremse_2_5a0_wegimpulse_vorderachse);
    // Zeitstempel
    int64_t raw_tmp_bremse_2_5a0_zeitstempel = (int64_t)llround((params::vw_pq.bremse_2_5a0_zeitstempel - 0.0f) / 1.0f);
    uint64_t raw_bremse_2_5a0_zeitstempel = raw_tmp_bremse_2_5a0_zeitstempel < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_2_5a0_zeitstempel;
    if (raw_bremse_2_5a0_zeitstempel > 0xFFFFULL) raw_bremse_2_5a0_zeitstempel = 0xFFFFULL;
    writeBitsLE(frame.data, 24, 16, (uint32_t)raw_bremse_2_5a0_zeitstempel);
    // mittlere_Raddrehzahl__Bremse_2
    int64_t raw_tmp_bremse_2_5a0_mittlere_raddrehzahl_bremse_2 = (int64_t)llround((params::vw_pq.bremse_2_5a0_mittlere_raddrehzahl_bremse_2 - 0.0f) / 0.002f);
    uint64_t raw_bremse_2_5a0_mittlere_raddrehzahl_bremse_2 = raw_tmp_bremse_2_5a0_mittlere_raddrehzahl_bremse_2 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_2_5a0_mittlere_raddrehzahl_bremse_2;
    if (raw_bremse_2_5a0_mittlere_raddrehzahl_bremse_2 > 0x7FFFULL) raw_bremse_2_5a0_mittlere_raddrehzahl_bremse_2 = 0x7FFFULL;
    writeBitsLE(frame.data, 9, 15, (uint32_t)raw_bremse_2_5a0_mittlere_raddrehzahl_bremse_2);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.bremse_2_5a0_querbeschl_timertic_m ? 1U : 0U);
    // Timer m1
    int64_t raw_tmp_bremse_2_5a0_timer_m1 = (int64_t)llround((params::vw_pq.bremse_2_5a0_timer_m1 - 0.0f) / 0.04f);
    uint64_t raw_bremse_2_5a0_timer_m1 = raw_tmp_bremse_2_5a0_timer_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_2_5a0_timer_m1;
    if (raw_bremse_2_5a0_timer_m1 > 0xFFULL) raw_bremse_2_5a0_timer_m1 = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_bremse_2_5a0_timer_m1);
    // Querbeschleunigung m0
    int64_t raw_tmp_bremse_2_5a0_querbeschleunigung_m0 = (int64_t)llround((params::vw_pq.bremse_2_5a0_querbeschleunigung_m0 - -1.27f) / 0.01f);
    uint64_t raw_bremse_2_5a0_querbeschleunigung_m0 = raw_tmp_bremse_2_5a0_querbeschleunigung_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_2_5a0_querbeschleunigung_m0;
    if (raw_bremse_2_5a0_querbeschleunigung_m0 > 0xFFULL) raw_bremse_2_5a0_querbeschleunigung_m0 = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_bremse_2_5a0_querbeschleunigung_m0);
}

// Message 0x1A0 (416) - Bremse_1
// DLC: 8, TX: ABS
bool decodeVwPq0x1A0(const CANMessage &frame) {
    if (frame.id != 0x1A0 || frame.len < 8) {
        return false;
    }

    // BR1_ASR_Anf: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: ASR request to engine - requests in bytes 5 and 6 to be executed
    // Value table: 0 "no_requirement" 1 "ASR_requirement"
    params::vw_pq.bremse_1_1a0_br1_asr_anf = readBitsLE(frame.data, 0, 1) != 0;
    // BR1_MSR_Anf M: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_1_1a0_br1_msr_anf_m = readBitsLE(frame.data, 1, 1) != 0;
    // BR1_ABS_Brems: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: ABS control active (including pressure reduction)
    // Value table: 0 "no_ABS_control" 1 "ABS_control"
    params::vw_pq.bremse_1_1a0_br1_abs_brems = readBitsLE(frame.data, 2, 1) != 0;
    // BR1_EDS_Ongr: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Electronic differential lock intervention
    // Value table: 0 "no_EDS_intervention" 1 "EDS_intervention"
    params::vw_pq.bremse_1_1a0_br1_eds_ongr = readBitsLE(frame.data, 3, 1) != 0;
    // BR1_ESP_Ongr: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: ESP driving dynamics control intervention
    // Value table: 0 "no_ESP_intervention" 1 "ESP_intervention"
    params::vw_pq.bremse_1_1a0_br1_esp_ongr = readBitsLE(frame.data, 4, 1) != 0;
    // BR1_ASR_Ongr: start=5 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: ASR transmission switching influence
    // Value table: 0 "no_requirement" 1 "ASR_shift_map" 2 "Downshift" 3 "Switching_ban"
    const uint32_t raw_bremse_1_1a0_br1_asr_ongr = readBitsLE(frame.data, 5, 2);
    params::vw_pq.bremse_1_1a0_br1_asr_ongr = raw_bremse_1_1a0_br1_asr_ongr * 1.0f + 0.0f;
    // BR1_EBV_Ongr: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Electronic brake force distribution intervention
    // Value table: 0 "no_EBV_intervention" 1 "EBV_intervention"
    params::vw_pq.bremse_1_1a0_br1_ebv_ongr = readBitsLE(frame.data, 7, 1) != 0;
    // BR1_Lampe_ABS: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: ABS safety warning lamp
    // Value table: 0 "Lamp_off" 1 "Lamp_on"
    params::vw_pq.bremse_1_1a0_br1_lampe_abs = readBitsLE(frame.data, 8, 1) != 0;
    // BR1_Lampe_ASR: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: ASR/ESP lamp (including flashing info)
    // Value table: 0 "Lamp_off" 1 "Lamp_on"
    params::vw_pq.bremse_1_1a0_br1_lampe_asr = readBitsLE(frame.data, 9, 1) != 0;
    // BR1_Lampe_BK: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Brake control lamp (red)
    // Value table: 0 "Lamp_off" 1 "Lamp_on"
    params::vw_pq.bremse_1_1a0_br1_lampe_bk = readBitsLE(frame.data, 10, 1) != 0;
    // BR1_Lichtschalt: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Brake light switch / brake pressure threshold exceeded
    // Value table: 0 "no_Bremsen_Schw_unterschr" 1 "Bremse_betaetigt_Schw_ueberschr"
    params::vw_pq.bremse_1_1a0_br1_lichtschalt = readBitsLE(frame.data, 11, 1) != 0;
    // BR1_StaDruckschw: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Brake test switch / pressure threshold status
    // Value table: 0 "Sta_guel_BKV_n_angest_k_Br" 1 "Sta_n_verf_BKV_angest_Br"
    params::vw_pq.bremse_1_1a0_br1_stadruckschw = readBitsLE(frame.data, 12, 1) != 0;
    // BR1_MAD: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: OBD rough road suppression
    // Value table: 0 "no_Offblendung" 1 "Offblendung"
    params::vw_pq.bremse_1_1a0_br1_mad = readBitsLE(frame.data, 13, 1) != 0;
    // BR1_Sta_MAD: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Rough road suppression status
    // Value table: 0 "valid" 1 "invalid"
    params::vw_pq.bremse_1_1a0_br1_sta_mad = readBitsLE(frame.data, 14, 1) != 0;
    // BR1_Diagnose: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: ABS diagnosis mode active
    // Value table: 0 "no" 1 "in_Diagnose"
    params::vw_pq.bremse_1_1a0_br1_diagnose = readBitsLE(frame.data, 15, 1) != 0;
    // BR1_BKV_active: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Active brake booster installed and not faulty
    // Value table: 0 "BKV_not_installed_or_fault" 1 "BKV_installed_and_not_fault"
    params::vw_pq.bremse_1_1a0_br1_bkv_active = readBitsLE(frame.data, 16, 1) != 0;
    // BR1_Rad_kmh: start=17 len=15 endian=1 sign=+ factor=0.01 offset=0 unit="km/h"
    // DBC comment: Vehicle speed (mean of driven wheels or ABS reference)
    const uint32_t raw_bremse_1_1a0_br1_rad_kmh = readBitsLE(frame.data, 17, 15);
    params::vw_pq.bremse_1_1a0_br1_rad_kmh = raw_bremse_1_1a0_br1_rad_kmh * 0.01f + 0.0f;
    // BR1_ASRMo_sl: start=32 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="%"
    // DBC comment: ASR engagement torque slow (0-99.06%)
    const uint32_t raw_bremse_1_1a0_br1_asrmo_sl = readBitsLE(frame.data, 32, 8);
    params::vw_pq.bremse_1_1a0_br1_asrmo_sl = raw_bremse_1_1a0_br1_asrmo_sl * 0.39f + 0.0f;
    // BR1_ASRMo_fa m0: start=40 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="%"
    const uint32_t raw_bremse_1_1a0_br1_asrmo_fa_m0 = readBitsLE(frame.data, 40, 8);
    params::vw_pq.bremse_1_1a0_br1_asrmo_fa_m0 = raw_bremse_1_1a0_br1_asrmo_fa_m0 * 0.39f + 0.0f;
    // BR1_MSR_Mo_inv m1: start=40 len=8 endian=1 sign=+ factor=-0.39 offset=99.45 unit="%"
    const uint32_t raw_bremse_1_1a0_br1_msr_mo_inv_m1 = readBitsLE(frame.data, 40, 8);
    params::vw_pq.bremse_1_1a0_br1_msr_mo_inv_m1 = raw_bremse_1_1a0_br1_msr_mo_inv_m1 * -0.39f + 99.45f;
    // BR1_MSR_Mo: start=48 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="%"
    // DBC comment: MSR engagement torque (0-99.06%)
    const uint32_t raw_bremse_1_1a0_br1_msr_mo = readBitsLE(frame.data, 48, 8);
    params::vw_pq.bremse_1_1a0_br1_msr_mo = raw_bremse_1_1a0_br1_msr_mo * 0.39f + 0.0f;
    // BR1_Zaehler: start=56 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Message counter to mark intervention moment
    const uint32_t raw_bremse_1_1a0_br1_zaehler = readBitsLE(frame.data, 56, 4);
    params::vw_pq.bremse_1_1a0_br1_zaehler = raw_bremse_1_1a0_br1_zaehler * 1.0f + 0.0f;
    // BR1_ASR_ESP: start=60 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Control unit type: 0=ABS/EDS, 1=ASR/ESP/MABS
    // Value table: 0 "ABS_ABS_EDS" 1 "ASR_ESP_MABS"
    params::vw_pq.bremse_1_1a0_br1_asr_esp = readBitsLE(frame.data, 60, 1) != 0;
    // BR1_ESPASR_passive: start=61 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: ESP/ASR passive or deactivated by driver
    // Value table: 0 "ESP_ASR_activated" 1 "ESP_oder_ASR_passive_get_oder_Fktumsch_oder_Rollenmodus"
    params::vw_pq.bremse_1_1a0_br1_espasr_passive = readBitsLE(frame.data, 61, 1) != 0;
    // BR1_Sta_ESP: start=62 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: ESP error status (fault memory entry)
    // Value table: 0 "ok" 1 "Failure"
    params::vw_pq.bremse_1_1a0_br1_sta_esp = readBitsLE(frame.data, 62, 1) != 0;
    // BR1_Ersatz_Kmh: start=63 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Speed is substitute value (sensor defect)
    // Value table: 0 "OK" 1 "Substitute_value"
    params::vw_pq.bremse_1_1a0_br1_ersatz_kmh = readBitsLE(frame.data, 63, 1) != 0;
    return true;
}

void encodeVwPq0x1A0(CANMessage &frame) {
    frame.id = 0x1A0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 0, 1, params::vw_pq.bremse_1_1a0_br1_asr_anf ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.bremse_1_1a0_br1_msr_anf_m ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.bremse_1_1a0_br1_abs_brems ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.bremse_1_1a0_br1_eds_ongr ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.bremse_1_1a0_br1_esp_ongr ? 1U : 0U);
    // BR1_ASR_Ongr
    int64_t raw_tmp_bremse_1_1a0_br1_asr_ongr = (int64_t)llround((params::vw_pq.bremse_1_1a0_br1_asr_ongr - 0.0f) / 1.0f);
    uint64_t raw_bremse_1_1a0_br1_asr_ongr = raw_tmp_bremse_1_1a0_br1_asr_ongr < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_1_1a0_br1_asr_ongr;
    if (raw_bremse_1_1a0_br1_asr_ongr > 0x3ULL) raw_bremse_1_1a0_br1_asr_ongr = 0x3ULL;
    writeBitsLE(frame.data, 5, 2, (uint32_t)raw_bremse_1_1a0_br1_asr_ongr);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.bremse_1_1a0_br1_ebv_ongr ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.bremse_1_1a0_br1_lampe_abs ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.bremse_1_1a0_br1_lampe_asr ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.bremse_1_1a0_br1_lampe_bk ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.bremse_1_1a0_br1_lichtschalt ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.bremse_1_1a0_br1_stadruckschw ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.bremse_1_1a0_br1_mad ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.bremse_1_1a0_br1_sta_mad ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.bremse_1_1a0_br1_diagnose ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.bremse_1_1a0_br1_bkv_active ? 1U : 0U);
    // BR1_Rad_kmh
    int64_t raw_tmp_bremse_1_1a0_br1_rad_kmh = (int64_t)llround((params::vw_pq.bremse_1_1a0_br1_rad_kmh - 0.0f) / 0.01f);
    uint64_t raw_bremse_1_1a0_br1_rad_kmh = raw_tmp_bremse_1_1a0_br1_rad_kmh < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_1_1a0_br1_rad_kmh;
    if (raw_bremse_1_1a0_br1_rad_kmh > 0x7FFFULL) raw_bremse_1_1a0_br1_rad_kmh = 0x7FFFULL;
    writeBitsLE(frame.data, 17, 15, (uint32_t)raw_bremse_1_1a0_br1_rad_kmh);
    // BR1_ASRMo_sl
    int64_t raw_tmp_bremse_1_1a0_br1_asrmo_sl = (int64_t)llround((params::vw_pq.bremse_1_1a0_br1_asrmo_sl - 0.0f) / 0.39f);
    uint64_t raw_bremse_1_1a0_br1_asrmo_sl = raw_tmp_bremse_1_1a0_br1_asrmo_sl < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_1_1a0_br1_asrmo_sl;
    if (raw_bremse_1_1a0_br1_asrmo_sl > 0xFFULL) raw_bremse_1_1a0_br1_asrmo_sl = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_bremse_1_1a0_br1_asrmo_sl);
    // BR1_ASRMo_fa m0
    int64_t raw_tmp_bremse_1_1a0_br1_asrmo_fa_m0 = (int64_t)llround((params::vw_pq.bremse_1_1a0_br1_asrmo_fa_m0 - 0.0f) / 0.39f);
    uint64_t raw_bremse_1_1a0_br1_asrmo_fa_m0 = raw_tmp_bremse_1_1a0_br1_asrmo_fa_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_1_1a0_br1_asrmo_fa_m0;
    if (raw_bremse_1_1a0_br1_asrmo_fa_m0 > 0xFFULL) raw_bremse_1_1a0_br1_asrmo_fa_m0 = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_bremse_1_1a0_br1_asrmo_fa_m0);
    // BR1_MSR_Mo_inv m1
    int64_t raw_tmp_bremse_1_1a0_br1_msr_mo_inv_m1 = (int64_t)llround((params::vw_pq.bremse_1_1a0_br1_msr_mo_inv_m1 - 99.45f) / -0.39f);
    uint64_t raw_bremse_1_1a0_br1_msr_mo_inv_m1 = raw_tmp_bremse_1_1a0_br1_msr_mo_inv_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_1_1a0_br1_msr_mo_inv_m1;
    if (raw_bremse_1_1a0_br1_msr_mo_inv_m1 > 0xFFULL) raw_bremse_1_1a0_br1_msr_mo_inv_m1 = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_bremse_1_1a0_br1_msr_mo_inv_m1);
    // BR1_MSR_Mo
    int64_t raw_tmp_bremse_1_1a0_br1_msr_mo = (int64_t)llround((params::vw_pq.bremse_1_1a0_br1_msr_mo - 0.0f) / 0.39f);
    uint64_t raw_bremse_1_1a0_br1_msr_mo = raw_tmp_bremse_1_1a0_br1_msr_mo < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_1_1a0_br1_msr_mo;
    if (raw_bremse_1_1a0_br1_msr_mo > 0xFFULL) raw_bremse_1_1a0_br1_msr_mo = 0xFFULL;
    writeBitsLE(frame.data, 48, 8, (uint32_t)raw_bremse_1_1a0_br1_msr_mo);
    // BR1_Zaehler
    int64_t raw_tmp_bremse_1_1a0_br1_zaehler = (int64_t)llround((params::vw_pq.bremse_1_1a0_br1_zaehler - 0.0f) / 1.0f);
    uint64_t raw_bremse_1_1a0_br1_zaehler = raw_tmp_bremse_1_1a0_br1_zaehler < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_1_1a0_br1_zaehler;
    if (raw_bremse_1_1a0_br1_zaehler > 0xFULL) raw_bremse_1_1a0_br1_zaehler = 0xFULL;
    writeBitsLE(frame.data, 56, 4, (uint32_t)raw_bremse_1_1a0_br1_zaehler);
    writeBitsLE(frame.data, 60, 1, params::vw_pq.bremse_1_1a0_br1_asr_esp ? 1U : 0U);
    writeBitsLE(frame.data, 61, 1, params::vw_pq.bremse_1_1a0_br1_espasr_passive ? 1U : 0U);
    writeBitsLE(frame.data, 62, 1, params::vw_pq.bremse_1_1a0_br1_sta_esp ? 1U : 0U);
    writeBitsLE(frame.data, 63, 1, params::vw_pq.bremse_1_1a0_br1_ersatz_kmh ? 1U : 0U);
}

// Message 0x2A8 (680) - Bremsbooster_1
// DLC: 3, TX: XXX
bool decodeVwPq0x2A8(const CANMessage &frame) {
    if (frame.id != 0x2A8 || frame.len < 3) {
        return false;
    }

    // Fehlerspeichereintrag_Booster: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremsbooster_1_2a8_fehlerspeichereintrag_booster = readBitsLE(frame.data, 23, 1) != 0;
    // Loseschalter_unplausibel_Boost: start=22 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremsbooster_1_2a8_loseschalter_unplausibel_boost = readBitsLE(frame.data, 22, 1) != 0;
    // Position_Standby: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremsbooster_1_2a8_position_standby = readBitsLE(frame.data, 21, 1) != 0;
    // ADR_Relais_ge_ffnet: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremsbooster_1_2a8_adr_relais_ge_ffnet = readBitsLE(frame.data, 20, 1) != 0;
    // Status_Bremsbooster_Steuerung: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremsbooster_1_2a8_status_bremsbooster_steuerung = readBitsLE(frame.data, 19, 1) != 0;
    // Bremsbooster_verf_gbar: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremsbooster_1_2a8_bremsbooster_verf_gbar = readBitsLE(frame.data, 18, 1) != 0;
    // Eingriff_Bremsbooster: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremsbooster_1_2a8_eingriff_bremsbooster = readBitsLE(frame.data, 17, 1) != 0;
    // Bremseingriff_Fahrer: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremsbooster_1_2a8_bremseingriff_fahrer = readBitsLE(frame.data, 16, 1) != 0;
    // Frei_Bremsbooster_1_1: start=12 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremsbooster_1_2a8_frei_bremsbooster_1_1 = readBitsLE(frame.data, 12, 4);
    params::vw_pq.bremsbooster_1_2a8_frei_bremsbooster_1_1 = raw_bremsbooster_1_2a8_frei_bremsbooster_1_1 * 1.0f + 0.0f;
    // Zaehler_Booster_1: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremsbooster_1_2a8_zaehler_booster_1 = readBitsLE(frame.data, 8, 4);
    params::vw_pq.bremsbooster_1_2a8_zaehler_booster_1 = raw_bremsbooster_1_2a8_zaehler_booster_1 * 1.0f + 0.0f;
    // Checksumme_Booster_1: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremsbooster_1_2a8_checksumme_booster_1 = readBitsLE(frame.data, 0, 8);
    params::vw_pq.bremsbooster_1_2a8_checksumme_booster_1 = raw_bremsbooster_1_2a8_checksumme_booster_1 * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x2A8(CANMessage &frame) {
    frame.id = 0x2A8;
    frame.len = 3;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 23, 1, params::vw_pq.bremsbooster_1_2a8_fehlerspeichereintrag_booster ? 1U : 0U);
    writeBitsLE(frame.data, 22, 1, params::vw_pq.bremsbooster_1_2a8_loseschalter_unplausibel_boost ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.bremsbooster_1_2a8_position_standby ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.bremsbooster_1_2a8_adr_relais_ge_ffnet ? 1U : 0U);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.bremsbooster_1_2a8_status_bremsbooster_steuerung ? 1U : 0U);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.bremsbooster_1_2a8_bremsbooster_verf_gbar ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.bremsbooster_1_2a8_eingriff_bremsbooster ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.bremsbooster_1_2a8_bremseingriff_fahrer ? 1U : 0U);
    // Frei_Bremsbooster_1_1
    int64_t raw_tmp_bremsbooster_1_2a8_frei_bremsbooster_1_1 = (int64_t)llround((params::vw_pq.bremsbooster_1_2a8_frei_bremsbooster_1_1 - 0.0f) / 1.0f);
    uint64_t raw_bremsbooster_1_2a8_frei_bremsbooster_1_1 = raw_tmp_bremsbooster_1_2a8_frei_bremsbooster_1_1 < 0 ? 0ULL : (uint64_t)raw_tmp_bremsbooster_1_2a8_frei_bremsbooster_1_1;
    if (raw_bremsbooster_1_2a8_frei_bremsbooster_1_1 > 0xFULL) raw_bremsbooster_1_2a8_frei_bremsbooster_1_1 = 0xFULL;
    writeBitsLE(frame.data, 12, 4, (uint32_t)raw_bremsbooster_1_2a8_frei_bremsbooster_1_1);
    // Zaehler_Booster_1
    int64_t raw_tmp_bremsbooster_1_2a8_zaehler_booster_1 = (int64_t)llround((params::vw_pq.bremsbooster_1_2a8_zaehler_booster_1 - 0.0f) / 1.0f);
    uint64_t raw_bremsbooster_1_2a8_zaehler_booster_1 = raw_tmp_bremsbooster_1_2a8_zaehler_booster_1 < 0 ? 0ULL : (uint64_t)raw_tmp_bremsbooster_1_2a8_zaehler_booster_1;
    if (raw_bremsbooster_1_2a8_zaehler_booster_1 > 0xFULL) raw_bremsbooster_1_2a8_zaehler_booster_1 = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_bremsbooster_1_2a8_zaehler_booster_1);
    // Checksumme_Booster_1
    int64_t raw_tmp_bremsbooster_1_2a8_checksumme_booster_1 = (int64_t)llround((params::vw_pq.bremsbooster_1_2a8_checksumme_booster_1 - 0.0f) / 1.0f);
    uint64_t raw_bremsbooster_1_2a8_checksumme_booster_1 = raw_tmp_bremsbooster_1_2a8_checksumme_booster_1 < 0 ? 0ULL : (uint64_t)raw_tmp_bremsbooster_1_2a8_checksumme_booster_1;
    if (raw_bremsbooster_1_2a8_checksumme_booster_1 > 0xFFULL) raw_bremsbooster_1_2a8_checksumme_booster_1 = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_bremsbooster_1_2a8_checksumme_booster_1);
}

// Message 0x578 (1400) - BatMan_1
// DLC: 1, TX: XXX
bool decodeVwPq0x578(const CANMessage &frame) {
    if (frame.id != 0x578 || frame.len < 1) {
        return false;
    }

    // Fehlerspeichereintrag__BatMan_: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.batman_1_578_fehlerspeichereintrag_batman = readBitsLE(frame.data, 7, 1) != 0;
    // Leistungsrelais: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.batman_1_578_leistungsrelais = readBitsLE(frame.data, 6, 1) != 0;
    // Messung_Starterleitung: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.batman_1_578_messung_starterleitung = readBitsLE(frame.data, 5, 1) != 0;
    // Zustand_Starterleitung: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.batman_1_578_zustand_starterleitung = readBitsLE(frame.data, 4, 1) != 0;
    // Umschaltrelais_Bordnetzbatterie: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.batman_1_578_umschaltrelais_bordnetzbatterie = readBitsLE(frame.data, 3, 1) != 0;
    // Ladung_Starterbatterie: start=1 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_batman_1_578_ladung_starterbatterie = readBitsLE(frame.data, 1, 2);
    params::vw_pq.batman_1_578_ladung_starterbatterie = raw_batman_1_578_ladung_starterbatterie * 1.0f + 0.0f;
    // Startmodus: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.batman_1_578_startmodus = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x578(CANMessage &frame) {
    frame.id = 0x578;
    frame.len = 1;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 7, 1, params::vw_pq.batman_1_578_fehlerspeichereintrag_batman ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.batman_1_578_leistungsrelais ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.batman_1_578_messung_starterleitung ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.batman_1_578_zustand_starterleitung ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.batman_1_578_umschaltrelais_bordnetzbatterie ? 1U : 0U);
    // Ladung_Starterbatterie
    int64_t raw_tmp_batman_1_578_ladung_starterbatterie = (int64_t)llround((params::vw_pq.batman_1_578_ladung_starterbatterie - 0.0f) / 1.0f);
    uint64_t raw_batman_1_578_ladung_starterbatterie = raw_tmp_batman_1_578_ladung_starterbatterie < 0 ? 0ULL : (uint64_t)raw_tmp_batman_1_578_ladung_starterbatterie;
    if (raw_batman_1_578_ladung_starterbatterie > 0x3ULL) raw_batman_1_578_ladung_starterbatterie = 0x3ULL;
    writeBitsLE(frame.data, 1, 2, (uint32_t)raw_batman_1_578_ladung_starterbatterie);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.batman_1_578_startmodus ? 1U : 0U);
}

// Message 0x2C0 (704) - Allrad_1
// DLC: 5, TX: XXX
bool decodeVwPq0x2C0(const CANMessage &frame) {
    if (frame.id != 0x2C0 || frame.len < 5) {
        return false;
    }

    // Kupplungssteifigkeit_Hinten__Is: start=32 len=8 endian=1 sign=+ factor=0.7874 offset=0 unit="%"
    const uint32_t raw_allrad_1_2c0_kupplungssteifigkeit_hinten_is = readBitsLE(frame.data, 32, 8);
    params::vw_pq.allrad_1_2c0_kupplungssteifigkeit_hinten_is = raw_allrad_1_2c0_kupplungssteifigkeit_hinten_is * 0.7874f + 0.0f;
    // Fehlerspeichereintrag_Allrad_1: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.allrad_1_2c0_fehlerspeichereintrag_allrad_1 = readBitsLE(frame.data, 31, 1) != 0;
    // Frei_Allrad_1_1: start=26 len=5 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_allrad_1_2c0_frei_allrad_1_1 = readBitsLE(frame.data, 26, 5);
    params::vw_pq.allrad_1_2c0_frei_allrad_1_1 = raw_allrad_1_2c0_frei_allrad_1_1 * 1.0f + 0.0f;
    // Schaltung_Vorwarnung: start=25 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.allrad_1_2c0_schaltung_vorwarnung = readBitsLE(frame.data, 25, 1) != 0;
    // Schaltung_aktiv__Allrad_1_: start=24 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.allrad_1_2c0_schaltung_aktiv_allrad_1 = readBitsLE(frame.data, 24, 1) != 0;
    // Ganginfo__PNG_: start=20 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_allrad_1_2c0_ganginfo_png = readBitsLE(frame.data, 20, 4);
    params::vw_pq.allrad_1_2c0_ganginfo_png = raw_allrad_1_2c0_ganginfo_png * 1.0f + 0.0f;
    // PNG_Anzeige_blinkend: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.allrad_1_2c0_png_anzeige_blinkend = readBitsLE(frame.data, 19, 1) != 0;
    // PNG_Status_4_1: start=16 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_allrad_1_2c0_png_status_4_1 = readBitsLE(frame.data, 16, 3);
    params::vw_pq.allrad_1_2c0_png_status_4_1 = raw_allrad_1_2c0_png_status_4_1 * 1.0f + 0.0f;
    // Kupplungssteifigkeit_Mitte__Ist: start=8 len=8 endian=1 sign=+ factor=3 offset=-381 unit="Nm/min"
    const uint32_t raw_allrad_1_2c0_kupplungssteifigkeit_mitte_ist = readBitsLE(frame.data, 8, 8);
    params::vw_pq.allrad_1_2c0_kupplungssteifigkeit_mitte_ist = raw_allrad_1_2c0_kupplungssteifigkeit_mitte_ist * 3.0f + -381.0f;
    // Einheit_der_Kupplungssteifigkei: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.allrad_1_2c0_einheit_der_kupplungssteifigkei = readBitsLE(frame.data, 7, 1) != 0;
    // Geschwindigkeitsbegrenzung: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.allrad_1_2c0_geschwindigkeitsbegrenzung = readBitsLE(frame.data, 6, 1) != 0;
    // Allrad_Warnlampe: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.allrad_1_2c0_allrad_warnlampe = readBitsLE(frame.data, 5, 1) != 0;
    // Notlauf: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.allrad_1_2c0_notlauf = readBitsLE(frame.data, 4, 1) != 0;
    // Kupplung_komplett_offen: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.allrad_1_2c0_kupplung_komplett_offen = readBitsLE(frame.data, 3, 1) != 0;
    // Fehlerstatus_Kupplungssteifigke: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.allrad_1_2c0_fehlerstatus_kupplungssteifigke = readBitsLE(frame.data, 2, 1) != 0;
    // Ubertemperaturschutz__Allrad_1_: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.allrad_1_2c0_ubertemperaturschutz_allrad_1 = readBitsLE(frame.data, 1, 1) != 0;
    // Fehler_Allrad_Kupplung: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.allrad_1_2c0_fehler_allrad_kupplung = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x2C0(CANMessage &frame) {
    frame.id = 0x2C0;
    frame.len = 5;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Kupplungssteifigkeit_Hinten__Is
    int64_t raw_tmp_allrad_1_2c0_kupplungssteifigkeit_hinten_is = (int64_t)llround((params::vw_pq.allrad_1_2c0_kupplungssteifigkeit_hinten_is - 0.0f) / 0.7874f);
    uint64_t raw_allrad_1_2c0_kupplungssteifigkeit_hinten_is = raw_tmp_allrad_1_2c0_kupplungssteifigkeit_hinten_is < 0 ? 0ULL : (uint64_t)raw_tmp_allrad_1_2c0_kupplungssteifigkeit_hinten_is;
    if (raw_allrad_1_2c0_kupplungssteifigkeit_hinten_is > 0xFFULL) raw_allrad_1_2c0_kupplungssteifigkeit_hinten_is = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_allrad_1_2c0_kupplungssteifigkeit_hinten_is);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.allrad_1_2c0_fehlerspeichereintrag_allrad_1 ? 1U : 0U);
    // Frei_Allrad_1_1
    int64_t raw_tmp_allrad_1_2c0_frei_allrad_1_1 = (int64_t)llround((params::vw_pq.allrad_1_2c0_frei_allrad_1_1 - 0.0f) / 1.0f);
    uint64_t raw_allrad_1_2c0_frei_allrad_1_1 = raw_tmp_allrad_1_2c0_frei_allrad_1_1 < 0 ? 0ULL : (uint64_t)raw_tmp_allrad_1_2c0_frei_allrad_1_1;
    if (raw_allrad_1_2c0_frei_allrad_1_1 > 0x1FULL) raw_allrad_1_2c0_frei_allrad_1_1 = 0x1FULL;
    writeBitsLE(frame.data, 26, 5, (uint32_t)raw_allrad_1_2c0_frei_allrad_1_1);
    writeBitsLE(frame.data, 25, 1, params::vw_pq.allrad_1_2c0_schaltung_vorwarnung ? 1U : 0U);
    writeBitsLE(frame.data, 24, 1, params::vw_pq.allrad_1_2c0_schaltung_aktiv_allrad_1 ? 1U : 0U);
    // Ganginfo__PNG_
    int64_t raw_tmp_allrad_1_2c0_ganginfo_png = (int64_t)llround((params::vw_pq.allrad_1_2c0_ganginfo_png - 0.0f) / 1.0f);
    uint64_t raw_allrad_1_2c0_ganginfo_png = raw_tmp_allrad_1_2c0_ganginfo_png < 0 ? 0ULL : (uint64_t)raw_tmp_allrad_1_2c0_ganginfo_png;
    if (raw_allrad_1_2c0_ganginfo_png > 0xFULL) raw_allrad_1_2c0_ganginfo_png = 0xFULL;
    writeBitsLE(frame.data, 20, 4, (uint32_t)raw_allrad_1_2c0_ganginfo_png);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.allrad_1_2c0_png_anzeige_blinkend ? 1U : 0U);
    // PNG_Status_4_1
    int64_t raw_tmp_allrad_1_2c0_png_status_4_1 = (int64_t)llround((params::vw_pq.allrad_1_2c0_png_status_4_1 - 0.0f) / 1.0f);
    uint64_t raw_allrad_1_2c0_png_status_4_1 = raw_tmp_allrad_1_2c0_png_status_4_1 < 0 ? 0ULL : (uint64_t)raw_tmp_allrad_1_2c0_png_status_4_1;
    if (raw_allrad_1_2c0_png_status_4_1 > 0x7ULL) raw_allrad_1_2c0_png_status_4_1 = 0x7ULL;
    writeBitsLE(frame.data, 16, 3, (uint32_t)raw_allrad_1_2c0_png_status_4_1);
    // Kupplungssteifigkeit_Mitte__Ist
    int64_t raw_tmp_allrad_1_2c0_kupplungssteifigkeit_mitte_ist = (int64_t)llround((params::vw_pq.allrad_1_2c0_kupplungssteifigkeit_mitte_ist - -381.0f) / 3.0f);
    uint64_t raw_allrad_1_2c0_kupplungssteifigkeit_mitte_ist = raw_tmp_allrad_1_2c0_kupplungssteifigkeit_mitte_ist < 0 ? 0ULL : (uint64_t)raw_tmp_allrad_1_2c0_kupplungssteifigkeit_mitte_ist;
    if (raw_allrad_1_2c0_kupplungssteifigkeit_mitte_ist > 0xFFULL) raw_allrad_1_2c0_kupplungssteifigkeit_mitte_ist = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_allrad_1_2c0_kupplungssteifigkeit_mitte_ist);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.allrad_1_2c0_einheit_der_kupplungssteifigkei ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.allrad_1_2c0_geschwindigkeitsbegrenzung ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.allrad_1_2c0_allrad_warnlampe ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.allrad_1_2c0_notlauf ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.allrad_1_2c0_kupplung_komplett_offen ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.allrad_1_2c0_fehlerstatus_kupplungssteifigke ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.allrad_1_2c0_ubertemperaturschutz_allrad_1 ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.allrad_1_2c0_fehler_allrad_kupplung ? 1U : 0U);
}

// Message 0x550 (1360) - Airbag_2
// DLC: 2, TX: XXX
bool decodeVwPq0x550(const CANMessage &frame) {
    if (frame.id != 0x550 || frame.len < 2) {
        return false;
    }

    // OOP_Beifahrer: start=14 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_airbag_2_550_oop_beifahrer = readBitsLE(frame.data, 14, 2);
    params::vw_pq.airbag_2_550_oop_beifahrer = raw_airbag_2_550_oop_beifahrer * 1.0f + 0.0f;
    // OOP_Fahrer: start=12 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_airbag_2_550_oop_fahrer = readBitsLE(frame.data, 12, 2);
    params::vw_pq.airbag_2_550_oop_fahrer = raw_airbag_2_550_oop_fahrer * 1.0f + 0.0f;
    // Belegungserkennung_hinten_mitte: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_2_550_belegungserkennung_hinten_mitte = readBitsLE(frame.data, 11, 1) != 0;
    // Belegungserkennung_hinten_recht: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_2_550_belegungserkennung_hinten_recht = readBitsLE(frame.data, 10, 1) != 0;
    // Belegungserkennung_hinten_links: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_2_550_belegungserkennung_hinten_links = readBitsLE(frame.data, 9, 1) != 0;
    // Belegungserkennung_Beifahrersit: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_2_550_belegungserkennung_beifahrersit = readBitsLE(frame.data, 8, 1) != 0;
    // Checksumme_Airbag_2__reserviert: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_airbag_2_550_checksumme_airbag_2_reserviert = readBitsLE(frame.data, 0, 8);
    params::vw_pq.airbag_2_550_checksumme_airbag_2_reserviert = raw_airbag_2_550_checksumme_airbag_2_reserviert * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x550(CANMessage &frame) {
    frame.id = 0x550;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // OOP_Beifahrer
    int64_t raw_tmp_airbag_2_550_oop_beifahrer = (int64_t)llround((params::vw_pq.airbag_2_550_oop_beifahrer - 0.0f) / 1.0f);
    uint64_t raw_airbag_2_550_oop_beifahrer = raw_tmp_airbag_2_550_oop_beifahrer < 0 ? 0ULL : (uint64_t)raw_tmp_airbag_2_550_oop_beifahrer;
    if (raw_airbag_2_550_oop_beifahrer > 0x3ULL) raw_airbag_2_550_oop_beifahrer = 0x3ULL;
    writeBitsLE(frame.data, 14, 2, (uint32_t)raw_airbag_2_550_oop_beifahrer);
    // OOP_Fahrer
    int64_t raw_tmp_airbag_2_550_oop_fahrer = (int64_t)llround((params::vw_pq.airbag_2_550_oop_fahrer - 0.0f) / 1.0f);
    uint64_t raw_airbag_2_550_oop_fahrer = raw_tmp_airbag_2_550_oop_fahrer < 0 ? 0ULL : (uint64_t)raw_tmp_airbag_2_550_oop_fahrer;
    if (raw_airbag_2_550_oop_fahrer > 0x3ULL) raw_airbag_2_550_oop_fahrer = 0x3ULL;
    writeBitsLE(frame.data, 12, 2, (uint32_t)raw_airbag_2_550_oop_fahrer);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.airbag_2_550_belegungserkennung_hinten_mitte ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.airbag_2_550_belegungserkennung_hinten_recht ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.airbag_2_550_belegungserkennung_hinten_links ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.airbag_2_550_belegungserkennung_beifahrersit ? 1U : 0U);
    // Checksumme_Airbag_2__reserviert
    int64_t raw_tmp_airbag_2_550_checksumme_airbag_2_reserviert = (int64_t)llround((params::vw_pq.airbag_2_550_checksumme_airbag_2_reserviert - 0.0f) / 1.0f);
    uint64_t raw_airbag_2_550_checksumme_airbag_2_reserviert = raw_tmp_airbag_2_550_checksumme_airbag_2_reserviert < 0 ? 0ULL : (uint64_t)raw_tmp_airbag_2_550_checksumme_airbag_2_reserviert;
    if (raw_airbag_2_550_checksumme_airbag_2_reserviert > 0xFFULL) raw_airbag_2_550_checksumme_airbag_2_reserviert = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_airbag_2_550_checksumme_airbag_2_reserviert);
}

// Message 0x050 (80) - Airbag_1
// DLC: 4, TX: XXX
bool decodeVwPq0x50(const CANMessage &frame) {
    if (frame.id != 0x50 || frame.len < 4) {
        return false;
    }

    // CHECKSUM: start=24 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_airbag_1_050_checksum = readBitsLE(frame.data, 24, 8);
    params::vw_pq.airbag_1_050_checksum = raw_airbag_1_050_checksum * 1.0f + 0.0f;
    // COUNTER: start=20 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_airbag_1_050_counter = readBitsLE(frame.data, 20, 4);
    params::vw_pq.airbag_1_050_counter = raw_airbag_1_050_counter * 1.0f + 0.0f;
    // Fehlerspeichereintrag: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_fehlerspeichereintrag = readBitsLE(frame.data, 19, 1) != 0;
    // Frei_Airbag_1_2: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_frei_airbag_1_2 = readBitsLE(frame.data, 18, 1) != 0;
    // Airbag_im_Stellgliedtest: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_airbag_im_stellgliedtest = readBitsLE(frame.data, 17, 1) != 0;
    // Airbag_in_Diagnose: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_airbag_in_diagnose = readBitsLE(frame.data, 16, 1) != 0;
    // Gurtwarnung_Beifahrer: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_gurtwarnung_beifahrer = readBitsLE(frame.data, 15, 1) != 0;
    // Gurtschalter_Beifahrer: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_gurtschalter_beifahrer = readBitsLE(frame.data, 14, 1) != 0;
    // Gurtwarnung_Fahrer: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_gurtwarnung_fahrer = readBitsLE(frame.data, 13, 1) != 0;
    // Gurtschalter_Fahrer: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_gurtschalter_fahrer = readBitsLE(frame.data, 12, 1) != 0;
    // Airbag_Systemfehler: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_airbag_systemfehler = readBitsLE(frame.data, 11, 1) != 0;
    // Kindersitzerkennung: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_kindersitzerkennung = readBitsLE(frame.data, 10, 1) != 0;
    // Airbag_deaktiviert: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_airbag_deaktiviert = readBitsLE(frame.data, 9, 1) != 0;
    // Airbag_Lampe: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_airbag_lampe = readBitsLE(frame.data, 8, 1) != 0;
    // Crash_Intensitaet: start=5 len=3 endian=1 sign=+ factor=1 offset=0 unit="B"
    const uint32_t raw_airbag_1_050_crash_intensitaet = readBitsLE(frame.data, 5, 3);
    params::vw_pq.airbag_1_050_crash_intensitaet = raw_airbag_1_050_crash_intensitaet * 1.0f + 0.0f;
    // Rollover: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_rollover = readBitsLE(frame.data, 4, 1) != 0;
    // Seiten_Crash_Beifahrer: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_seiten_crash_beifahrer = readBitsLE(frame.data, 3, 1) != 0;
    // Seiten_Crash_Fahrer: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_seiten_crash_fahrer = readBitsLE(frame.data, 2, 1) != 0;
    // Heck_Crash: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_heck_crash = readBitsLE(frame.data, 1, 1) != 0;
    // Front_Crash: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.airbag_1_050_front_crash = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x50(CANMessage &frame) {
    frame.id = 0x50;
    frame.len = 4;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // CHECKSUM
    int64_t raw_tmp_airbag_1_050_checksum = (int64_t)llround((params::vw_pq.airbag_1_050_checksum - 0.0f) / 1.0f);
    uint64_t raw_airbag_1_050_checksum = raw_tmp_airbag_1_050_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_airbag_1_050_checksum;
    if (raw_airbag_1_050_checksum > 0xFFULL) raw_airbag_1_050_checksum = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_airbag_1_050_checksum);
    // COUNTER
    int64_t raw_tmp_airbag_1_050_counter = (int64_t)llround((params::vw_pq.airbag_1_050_counter - 0.0f) / 1.0f);
    uint64_t raw_airbag_1_050_counter = raw_tmp_airbag_1_050_counter < 0 ? 0ULL : (uint64_t)raw_tmp_airbag_1_050_counter;
    if (raw_airbag_1_050_counter > 0xFULL) raw_airbag_1_050_counter = 0xFULL;
    writeBitsLE(frame.data, 20, 4, (uint32_t)raw_airbag_1_050_counter);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.airbag_1_050_fehlerspeichereintrag ? 1U : 0U);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.airbag_1_050_frei_airbag_1_2 ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.airbag_1_050_airbag_im_stellgliedtest ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.airbag_1_050_airbag_in_diagnose ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.airbag_1_050_gurtwarnung_beifahrer ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.airbag_1_050_gurtschalter_beifahrer ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.airbag_1_050_gurtwarnung_fahrer ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.airbag_1_050_gurtschalter_fahrer ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.airbag_1_050_airbag_systemfehler ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.airbag_1_050_kindersitzerkennung ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.airbag_1_050_airbag_deaktiviert ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.airbag_1_050_airbag_lampe ? 1U : 0U);
    // Crash_Intensitaet
    int64_t raw_tmp_airbag_1_050_crash_intensitaet = (int64_t)llround((params::vw_pq.airbag_1_050_crash_intensitaet - 0.0f) / 1.0f);
    uint64_t raw_airbag_1_050_crash_intensitaet = raw_tmp_airbag_1_050_crash_intensitaet < 0 ? 0ULL : (uint64_t)raw_tmp_airbag_1_050_crash_intensitaet;
    if (raw_airbag_1_050_crash_intensitaet > 0x7ULL) raw_airbag_1_050_crash_intensitaet = 0x7ULL;
    writeBitsLE(frame.data, 5, 3, (uint32_t)raw_airbag_1_050_crash_intensitaet);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.airbag_1_050_rollover ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.airbag_1_050_seiten_crash_beifahrer ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.airbag_1_050_seiten_crash_fahrer ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.airbag_1_050_heck_crash ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vw_pq.airbag_1_050_front_crash ? 1U : 0U);
}

// Message 0x360 (864) - ADR_System
// DLC: 8, TX: XXX
bool decodeVwPq0x360(const CANMessage &frame) {
    if (frame.id != 0x360 || frame.len < 8) {
        return false;
    }

    // S_Checksumme_ADR_1: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_system_360_s_checksumme_adr_1 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.adr_system_360_s_checksumme_adr_1 = raw_adr_system_360_s_checksumme_adr_1 * 1.0f + 0.0f;
    // S_Frei_ADR_1_1: start=52 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_system_360_s_frei_adr_1_1 = readBitsLE(frame.data, 52, 4);
    params::vw_pq.adr_system_360_s_frei_adr_1_1 = raw_adr_system_360_s_frei_adr_1_1 * 1.0f + 0.0f;
    // S_Zeitluecke_gemessen: start=48 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_system_360_s_zeitluecke_gemessen = readBitsLE(frame.data, 48, 4);
    params::vw_pq.adr_system_360_s_zeitluecke_gemessen = raw_adr_system_360_s_zeitluecke_gemessen * 1.0f + 0.0f;
    // S_Fehlerspeichereintrag_ADR: start=47 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_fehlerspeichereintrag_adr = readBitsLE(frame.data, 47, 1) != 0;
    // S_Fehlerspeichereintrag_Bremsbo: start=46 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_fehlerspeichereintrag_bremsbo = readBitsLE(frame.data, 46, 1) != 0;
    // S_ADR_Relais_geoeffnet: start=45 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_adr_relais_geoeffnet = readBitsLE(frame.data, 45, 1) != 0;
    // S_Bremsbooster_Status: start=43 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_system_360_s_bremsbooster_status = readBitsLE(frame.data, 43, 2);
    params::vw_pq.adr_system_360_s_bremsbooster_status = raw_adr_system_360_s_bremsbooster_status * 1.0f + 0.0f;
    // S_Eingriff_Bremsbooster: start=42 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_eingriff_bremsbooster = readBitsLE(frame.data, 42, 1) != 0;
    // S_Loeseschalter_unplausibel: start=41 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_loeseschalter_unplausibel = readBitsLE(frame.data, 41, 1) != 0;
    // S_Bremseingriff_Fahrer: start=40 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_bremseingriff_fahrer = readBitsLE(frame.data, 40, 1) != 0;
    // S_Anzeige_Sensor_blind: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_anzeige_sensor_blind = readBitsLE(frame.data, 39, 1) != 0;
    // S_Ansteuerung_optischer_Fahrerh: start=38 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_ansteuerung_optischer_fahrerh = readBitsLE(frame.data, 38, 1) != 0;
    // S_Ansteuerung_Gong_2: start=37 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_ansteuerung_gong_2 = readBitsLE(frame.data, 37, 1) != 0;
    // S_Ansteuerung_Gong_1: start=36 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_ansteuerung_gong_1 = readBitsLE(frame.data, 36, 1) != 0;
    // S_Schaltaufforderung: start=34 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_system_360_s_schaltaufforderung = readBitsLE(frame.data, 34, 2);
    params::vw_pq.adr_system_360_s_schaltaufforderung = raw_adr_system_360_s_schaltaufforderung * 1.0f + 0.0f;
    // S_Anzeige_Prioritaet: start=33 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_anzeige_prioritaet = readBitsLE(frame.data, 33, 1) != 0;
    // S_Anzeige_Zeitluecke: start=32 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_anzeige_zeitluecke = readBitsLE(frame.data, 32, 1) != 0;
    // S_Wunschgeschwindigkeit: start=24 len=8 endian=1 sign=+ factor=1 offset=0 unit="km/h"
    const uint32_t raw_adr_system_360_s_wunschgeschwindigkeit = readBitsLE(frame.data, 24, 8);
    params::vw_pq.adr_system_360_s_wunschgeschwindigkeit = raw_adr_system_360_s_wunschgeschwindigkeit * 1.0f + 0.0f;
    // S_Objekt_erfasst: start=22 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_system_360_s_objekt_erfasst = readBitsLE(frame.data, 22, 2);
    params::vw_pq.adr_system_360_s_objekt_erfasst = raw_adr_system_360_s_objekt_erfasst * 1.0f + 0.0f;
    // S_Gesetzte_Zeitluecke__ADR_1_: start=18 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_system_360_s_gesetzte_zeitluecke_adr_1 = readBitsLE(frame.data, 18, 4);
    params::vw_pq.adr_system_360_s_gesetzte_zeitluecke_adr_1 = raw_adr_system_360_s_gesetzte_zeitluecke_adr_1 * 1.0f + 0.0f;
    // S_Synchronisation_Bremsbooster: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_synchronisation_bremsbooster = readBitsLE(frame.data, 17, 1) != 0;
    // S_Momentenanforderung_Freigabe: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_momentenanforderung_freigabe = readBitsLE(frame.data, 16, 1) != 0;
    // S_Verhinderung_Schubabschaltung: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_verhinderung_schubabschaltung = readBitsLE(frame.data, 15, 1) != 0;
    // S_Status_ADR_1__S_: start=13 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_system_360_s_status_adr_1_s = readBitsLE(frame.data, 13, 2);
    params::vw_pq.adr_system_360_s_status_adr_1_s = raw_adr_system_360_s_status_adr_1_s * 1.0f + 0.0f;
    // S_Fehler_ADR_1__S_: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_system_360_s_fehler_adr_1_s = readBitsLE(frame.data, 12, 1) != 0;
    // S_Zaehler_ADR_1: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_system_360_s_zaehler_adr_1 = readBitsLE(frame.data, 8, 4);
    params::vw_pq.adr_system_360_s_zaehler_adr_1 = raw_adr_system_360_s_zaehler_adr_1 * 1.0f + 0.0f;
    // S_Momentenanforderung_ADR: start=0 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="MDI"
    const uint32_t raw_adr_system_360_s_momentenanforderung_adr = readBitsLE(frame.data, 0, 8);
    params::vw_pq.adr_system_360_s_momentenanforderung_adr = raw_adr_system_360_s_momentenanforderung_adr * 0.39f + 0.0f;
    return true;
}

void encodeVwPq0x360(CANMessage &frame) {
    frame.id = 0x360;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // S_Checksumme_ADR_1
    int64_t raw_tmp_adr_system_360_s_checksumme_adr_1 = (int64_t)llround((params::vw_pq.adr_system_360_s_checksumme_adr_1 - 0.0f) / 1.0f);
    uint64_t raw_adr_system_360_s_checksumme_adr_1 = raw_tmp_adr_system_360_s_checksumme_adr_1 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_system_360_s_checksumme_adr_1;
    if (raw_adr_system_360_s_checksumme_adr_1 > 0xFFULL) raw_adr_system_360_s_checksumme_adr_1 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_adr_system_360_s_checksumme_adr_1);
    // S_Frei_ADR_1_1
    int64_t raw_tmp_adr_system_360_s_frei_adr_1_1 = (int64_t)llround((params::vw_pq.adr_system_360_s_frei_adr_1_1 - 0.0f) / 1.0f);
    uint64_t raw_adr_system_360_s_frei_adr_1_1 = raw_tmp_adr_system_360_s_frei_adr_1_1 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_system_360_s_frei_adr_1_1;
    if (raw_adr_system_360_s_frei_adr_1_1 > 0xFULL) raw_adr_system_360_s_frei_adr_1_1 = 0xFULL;
    writeBitsLE(frame.data, 52, 4, (uint32_t)raw_adr_system_360_s_frei_adr_1_1);
    // S_Zeitluecke_gemessen
    int64_t raw_tmp_adr_system_360_s_zeitluecke_gemessen = (int64_t)llround((params::vw_pq.adr_system_360_s_zeitluecke_gemessen - 0.0f) / 1.0f);
    uint64_t raw_adr_system_360_s_zeitluecke_gemessen = raw_tmp_adr_system_360_s_zeitluecke_gemessen < 0 ? 0ULL : (uint64_t)raw_tmp_adr_system_360_s_zeitluecke_gemessen;
    if (raw_adr_system_360_s_zeitluecke_gemessen > 0xFULL) raw_adr_system_360_s_zeitluecke_gemessen = 0xFULL;
    writeBitsLE(frame.data, 48, 4, (uint32_t)raw_adr_system_360_s_zeitluecke_gemessen);
    writeBitsLE(frame.data, 47, 1, params::vw_pq.adr_system_360_s_fehlerspeichereintrag_adr ? 1U : 0U);
    writeBitsLE(frame.data, 46, 1, params::vw_pq.adr_system_360_s_fehlerspeichereintrag_bremsbo ? 1U : 0U);
    writeBitsLE(frame.data, 45, 1, params::vw_pq.adr_system_360_s_adr_relais_geoeffnet ? 1U : 0U);
    // S_Bremsbooster_Status
    int64_t raw_tmp_adr_system_360_s_bremsbooster_status = (int64_t)llround((params::vw_pq.adr_system_360_s_bremsbooster_status - 0.0f) / 1.0f);
    uint64_t raw_adr_system_360_s_bremsbooster_status = raw_tmp_adr_system_360_s_bremsbooster_status < 0 ? 0ULL : (uint64_t)raw_tmp_adr_system_360_s_bremsbooster_status;
    if (raw_adr_system_360_s_bremsbooster_status > 0x3ULL) raw_adr_system_360_s_bremsbooster_status = 0x3ULL;
    writeBitsLE(frame.data, 43, 2, (uint32_t)raw_adr_system_360_s_bremsbooster_status);
    writeBitsLE(frame.data, 42, 1, params::vw_pq.adr_system_360_s_eingriff_bremsbooster ? 1U : 0U);
    writeBitsLE(frame.data, 41, 1, params::vw_pq.adr_system_360_s_loeseschalter_unplausibel ? 1U : 0U);
    writeBitsLE(frame.data, 40, 1, params::vw_pq.adr_system_360_s_bremseingriff_fahrer ? 1U : 0U);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.adr_system_360_s_anzeige_sensor_blind ? 1U : 0U);
    writeBitsLE(frame.data, 38, 1, params::vw_pq.adr_system_360_s_ansteuerung_optischer_fahrerh ? 1U : 0U);
    writeBitsLE(frame.data, 37, 1, params::vw_pq.adr_system_360_s_ansteuerung_gong_2 ? 1U : 0U);
    writeBitsLE(frame.data, 36, 1, params::vw_pq.adr_system_360_s_ansteuerung_gong_1 ? 1U : 0U);
    // S_Schaltaufforderung
    int64_t raw_tmp_adr_system_360_s_schaltaufforderung = (int64_t)llround((params::vw_pq.adr_system_360_s_schaltaufforderung - 0.0f) / 1.0f);
    uint64_t raw_adr_system_360_s_schaltaufforderung = raw_tmp_adr_system_360_s_schaltaufforderung < 0 ? 0ULL : (uint64_t)raw_tmp_adr_system_360_s_schaltaufforderung;
    if (raw_adr_system_360_s_schaltaufforderung > 0x3ULL) raw_adr_system_360_s_schaltaufforderung = 0x3ULL;
    writeBitsLE(frame.data, 34, 2, (uint32_t)raw_adr_system_360_s_schaltaufforderung);
    writeBitsLE(frame.data, 33, 1, params::vw_pq.adr_system_360_s_anzeige_prioritaet ? 1U : 0U);
    writeBitsLE(frame.data, 32, 1, params::vw_pq.adr_system_360_s_anzeige_zeitluecke ? 1U : 0U);
    // S_Wunschgeschwindigkeit
    int64_t raw_tmp_adr_system_360_s_wunschgeschwindigkeit = (int64_t)llround((params::vw_pq.adr_system_360_s_wunschgeschwindigkeit - 0.0f) / 1.0f);
    uint64_t raw_adr_system_360_s_wunschgeschwindigkeit = raw_tmp_adr_system_360_s_wunschgeschwindigkeit < 0 ? 0ULL : (uint64_t)raw_tmp_adr_system_360_s_wunschgeschwindigkeit;
    if (raw_adr_system_360_s_wunschgeschwindigkeit > 0xFFULL) raw_adr_system_360_s_wunschgeschwindigkeit = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_adr_system_360_s_wunschgeschwindigkeit);
    // S_Objekt_erfasst
    int64_t raw_tmp_adr_system_360_s_objekt_erfasst = (int64_t)llround((params::vw_pq.adr_system_360_s_objekt_erfasst - 0.0f) / 1.0f);
    uint64_t raw_adr_system_360_s_objekt_erfasst = raw_tmp_adr_system_360_s_objekt_erfasst < 0 ? 0ULL : (uint64_t)raw_tmp_adr_system_360_s_objekt_erfasst;
    if (raw_adr_system_360_s_objekt_erfasst > 0x3ULL) raw_adr_system_360_s_objekt_erfasst = 0x3ULL;
    writeBitsLE(frame.data, 22, 2, (uint32_t)raw_adr_system_360_s_objekt_erfasst);
    // S_Gesetzte_Zeitluecke__ADR_1_
    int64_t raw_tmp_adr_system_360_s_gesetzte_zeitluecke_adr_1 = (int64_t)llround((params::vw_pq.adr_system_360_s_gesetzte_zeitluecke_adr_1 - 0.0f) / 1.0f);
    uint64_t raw_adr_system_360_s_gesetzte_zeitluecke_adr_1 = raw_tmp_adr_system_360_s_gesetzte_zeitluecke_adr_1 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_system_360_s_gesetzte_zeitluecke_adr_1;
    if (raw_adr_system_360_s_gesetzte_zeitluecke_adr_1 > 0xFULL) raw_adr_system_360_s_gesetzte_zeitluecke_adr_1 = 0xFULL;
    writeBitsLE(frame.data, 18, 4, (uint32_t)raw_adr_system_360_s_gesetzte_zeitluecke_adr_1);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.adr_system_360_s_synchronisation_bremsbooster ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.adr_system_360_s_momentenanforderung_freigabe ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.adr_system_360_s_verhinderung_schubabschaltung ? 1U : 0U);
    // S_Status_ADR_1__S_
    int64_t raw_tmp_adr_system_360_s_status_adr_1_s = (int64_t)llround((params::vw_pq.adr_system_360_s_status_adr_1_s - 0.0f) / 1.0f);
    uint64_t raw_adr_system_360_s_status_adr_1_s = raw_tmp_adr_system_360_s_status_adr_1_s < 0 ? 0ULL : (uint64_t)raw_tmp_adr_system_360_s_status_adr_1_s;
    if (raw_adr_system_360_s_status_adr_1_s > 0x3ULL) raw_adr_system_360_s_status_adr_1_s = 0x3ULL;
    writeBitsLE(frame.data, 13, 2, (uint32_t)raw_adr_system_360_s_status_adr_1_s);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.adr_system_360_s_fehler_adr_1_s ? 1U : 0U);
    // S_Zaehler_ADR_1
    int64_t raw_tmp_adr_system_360_s_zaehler_adr_1 = (int64_t)llround((params::vw_pq.adr_system_360_s_zaehler_adr_1 - 0.0f) / 1.0f);
    uint64_t raw_adr_system_360_s_zaehler_adr_1 = raw_tmp_adr_system_360_s_zaehler_adr_1 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_system_360_s_zaehler_adr_1;
    if (raw_adr_system_360_s_zaehler_adr_1 > 0xFULL) raw_adr_system_360_s_zaehler_adr_1 = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_adr_system_360_s_zaehler_adr_1);
    // S_Momentenanforderung_ADR
    int64_t raw_tmp_adr_system_360_s_momentenanforderung_adr = (int64_t)llround((params::vw_pq.adr_system_360_s_momentenanforderung_adr - 0.0f) / 0.39f);
    uint64_t raw_adr_system_360_s_momentenanforderung_adr = raw_tmp_adr_system_360_s_momentenanforderung_adr < 0 ? 0ULL : (uint64_t)raw_tmp_adr_system_360_s_momentenanforderung_adr;
    if (raw_adr_system_360_s_momentenanforderung_adr > 0xFFULL) raw_adr_system_360_s_momentenanforderung_adr = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_adr_system_360_s_momentenanforderung_adr);
}

// Message 0x260 (608) - ADR_2
// DLC: 4, TX: XXX
bool decodeVwPq0x260(const CANMessage &frame) {
    if (frame.id != 0x260 || frame.len < 4) {
        return false;
    }

    // Frei_ADR_2_2: start=27 len=5 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_2_260_frei_adr_2_2 = readBitsLE(frame.data, 27, 5);
    params::vw_pq.adr_2_260_frei_adr_2_2 = raw_adr_2_260_frei_adr_2_2 * 1.0f + 0.0f;
    // Anforderung_Bremsdruck: start=16 len=11 endian=1 sign=+ factor=0.0625 offset=0 unit="bar"
    const uint32_t raw_adr_2_260_anforderung_bremsdruck = readBitsLE(frame.data, 16, 11);
    params::vw_pq.adr_2_260_anforderung_bremsdruck = raw_adr_2_260_anforderung_bremsdruck * 0.0625f + 0.0f;
    // Frei_ADR_2_1: start=14 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_2_260_frei_adr_2_1 = readBitsLE(frame.data, 14, 2);
    params::vw_pq.adr_2_260_frei_adr_2_1 = raw_adr_2_260_frei_adr_2_1 * 1.0f + 0.0f;
    // Relais_Test_Fehler: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_2_260_relais_test_fehler = readBitsLE(frame.data, 13, 1) != 0;
    // Standby: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_2_260_standby = readBitsLE(frame.data, 12, 1) != 0;
    // Zaehler_ADR_2: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_2_260_zaehler_adr_2 = readBitsLE(frame.data, 8, 4);
    params::vw_pq.adr_2_260_zaehler_adr_2 = raw_adr_2_260_zaehler_adr_2 * 1.0f + 0.0f;
    // Checksumme_ADR_2: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_2_260_checksumme_adr_2 = readBitsLE(frame.data, 0, 8);
    params::vw_pq.adr_2_260_checksumme_adr_2 = raw_adr_2_260_checksumme_adr_2 * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x260(CANMessage &frame) {
    frame.id = 0x260;
    frame.len = 4;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Frei_ADR_2_2
    int64_t raw_tmp_adr_2_260_frei_adr_2_2 = (int64_t)llround((params::vw_pq.adr_2_260_frei_adr_2_2 - 0.0f) / 1.0f);
    uint64_t raw_adr_2_260_frei_adr_2_2 = raw_tmp_adr_2_260_frei_adr_2_2 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_2_260_frei_adr_2_2;
    if (raw_adr_2_260_frei_adr_2_2 > 0x1FULL) raw_adr_2_260_frei_adr_2_2 = 0x1FULL;
    writeBitsLE(frame.data, 27, 5, (uint32_t)raw_adr_2_260_frei_adr_2_2);
    // Anforderung_Bremsdruck
    int64_t raw_tmp_adr_2_260_anforderung_bremsdruck = (int64_t)llround((params::vw_pq.adr_2_260_anforderung_bremsdruck - 0.0f) / 0.0625f);
    uint64_t raw_adr_2_260_anforderung_bremsdruck = raw_tmp_adr_2_260_anforderung_bremsdruck < 0 ? 0ULL : (uint64_t)raw_tmp_adr_2_260_anforderung_bremsdruck;
    if (raw_adr_2_260_anforderung_bremsdruck > 0x7FFULL) raw_adr_2_260_anforderung_bremsdruck = 0x7FFULL;
    writeBitsLE(frame.data, 16, 11, (uint32_t)raw_adr_2_260_anforderung_bremsdruck);
    // Frei_ADR_2_1
    int64_t raw_tmp_adr_2_260_frei_adr_2_1 = (int64_t)llround((params::vw_pq.adr_2_260_frei_adr_2_1 - 0.0f) / 1.0f);
    uint64_t raw_adr_2_260_frei_adr_2_1 = raw_tmp_adr_2_260_frei_adr_2_1 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_2_260_frei_adr_2_1;
    if (raw_adr_2_260_frei_adr_2_1 > 0x3ULL) raw_adr_2_260_frei_adr_2_1 = 0x3ULL;
    writeBitsLE(frame.data, 14, 2, (uint32_t)raw_adr_2_260_frei_adr_2_1);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.adr_2_260_relais_test_fehler ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.adr_2_260_standby ? 1U : 0U);
    // Zaehler_ADR_2
    int64_t raw_tmp_adr_2_260_zaehler_adr_2 = (int64_t)llround((params::vw_pq.adr_2_260_zaehler_adr_2 - 0.0f) / 1.0f);
    uint64_t raw_adr_2_260_zaehler_adr_2 = raw_tmp_adr_2_260_zaehler_adr_2 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_2_260_zaehler_adr_2;
    if (raw_adr_2_260_zaehler_adr_2 > 0xFULL) raw_adr_2_260_zaehler_adr_2 = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_adr_2_260_zaehler_adr_2);
    // Checksumme_ADR_2
    int64_t raw_tmp_adr_2_260_checksumme_adr_2 = (int64_t)llround((params::vw_pq.adr_2_260_checksumme_adr_2 - 0.0f) / 1.0f);
    uint64_t raw_adr_2_260_checksumme_adr_2 = raw_tmp_adr_2_260_checksumme_adr_2 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_2_260_checksumme_adr_2;
    if (raw_adr_2_260_checksumme_adr_2 > 0xFFULL) raw_adr_2_260_checksumme_adr_2 = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_adr_2_260_checksumme_adr_2);
}

// Message 0x52C (1324) - ADR_1
// DLC: 8, TX: XXX
bool decodeVwPq0x52C(const CANMessage &frame) {
    if (frame.id != 0x52C || frame.len < 8) {
        return false;
    }

    // Checksumme_ADR_1: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_1_52c_checksumme_adr_1 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.adr_1_52c_checksumme_adr_1 = raw_adr_1_52c_checksumme_adr_1 * 1.0f + 0.0f;
    // Frei_ADR_1_5: start=52 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_1_52c_frei_adr_1_5 = readBitsLE(frame.data, 52, 4);
    params::vw_pq.adr_1_52c_frei_adr_1_5 = raw_adr_1_52c_frei_adr_1_5 * 1.0f + 0.0f;
    // Zeitluecke_gemessen: start=48 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_1_52c_zeitluecke_gemessen = readBitsLE(frame.data, 48, 4);
    params::vw_pq.adr_1_52c_zeitluecke_gemessen = raw_adr_1_52c_zeitluecke_gemessen * 1.0f + 0.0f;
    // Fehlerspeichereintrag_ADR: start=47 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_1_52c_fehlerspeichereintrag_adr = readBitsLE(frame.data, 47, 1) != 0;
    // Reserviert_ADR_1_1: start=40 len=7 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_1_52c_reserviert_adr_1_1 = readBitsLE(frame.data, 40, 7);
    params::vw_pq.adr_1_52c_reserviert_adr_1_1 = raw_adr_1_52c_reserviert_adr_1_1 * 1.0f + 0.0f;
    // Anzeige_Sensor_blind: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_1_52c_anzeige_sensor_blind = readBitsLE(frame.data, 39, 1) != 0;
    // Ansteuerung_optischer_Fahrerhin: start=38 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_1_52c_ansteuerung_optischer_fahrerhin = readBitsLE(frame.data, 38, 1) != 0;
    // Ansteuerung_Gong_2: start=37 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_1_52c_ansteuerung_gong_2 = readBitsLE(frame.data, 37, 1) != 0;
    // Ansteuerung_Gong_1: start=36 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_1_52c_ansteuerung_gong_1 = readBitsLE(frame.data, 36, 1) != 0;
    // Schaltaufforderung: start=34 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_1_52c_schaltaufforderung = readBitsLE(frame.data, 34, 2);
    params::vw_pq.adr_1_52c_schaltaufforderung = raw_adr_1_52c_schaltaufforderung * 1.0f + 0.0f;
    // Anzeige_Prioritaet: start=33 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_1_52c_anzeige_prioritaet = readBitsLE(frame.data, 33, 1) != 0;
    // Anzeige_Zeitluecke: start=32 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_1_52c_anzeige_zeitluecke = readBitsLE(frame.data, 32, 1) != 0;
    // Wunschgeschwindigkeit: start=24 len=8 endian=1 sign=+ factor=1 offset=0 unit="km/h"
    const uint32_t raw_adr_1_52c_wunschgeschwindigkeit = readBitsLE(frame.data, 24, 8);
    params::vw_pq.adr_1_52c_wunschgeschwindigkeit = raw_adr_1_52c_wunschgeschwindigkeit * 1.0f + 0.0f;
    // Objekt_erfasst: start=22 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_1_52c_objekt_erfasst = readBitsLE(frame.data, 22, 2);
    params::vw_pq.adr_1_52c_objekt_erfasst = raw_adr_1_52c_objekt_erfasst * 1.0f + 0.0f;
    // Gesetzte_Zeitluecke__ADR_1_: start=18 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_1_52c_gesetzte_zeitluecke_adr_1 = readBitsLE(frame.data, 18, 4);
    params::vw_pq.adr_1_52c_gesetzte_zeitluecke_adr_1 = raw_adr_1_52c_gesetzte_zeitluecke_adr_1 * 1.0f + 0.0f;
    // Synchronisation_Bremsbooster: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_1_52c_synchronisation_bremsbooster = readBitsLE(frame.data, 17, 1) != 0;
    // Momentenanforderung_Freigabe: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_1_52c_momentenanforderung_freigabe = readBitsLE(frame.data, 16, 1) != 0;
    // Verhinderung_Schubabschaltung: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_1_52c_verhinderung_schubabschaltung = readBitsLE(frame.data, 15, 1) != 0;
    // Status_ADR_1: start=13 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_1_52c_status_adr_1 = readBitsLE(frame.data, 13, 2);
    params::vw_pq.adr_1_52c_status_adr_1 = raw_adr_1_52c_status_adr_1 * 1.0f + 0.0f;
    // Fehler_ADR_1: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.adr_1_52c_fehler_adr_1 = readBitsLE(frame.data, 12, 1) != 0;
    // Zaehler_ADR_1: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_adr_1_52c_zaehler_adr_1 = readBitsLE(frame.data, 8, 4);
    params::vw_pq.adr_1_52c_zaehler_adr_1 = raw_adr_1_52c_zaehler_adr_1 * 1.0f + 0.0f;
    // Momentenanforderung_ADR: start=0 len=8 endian=1 sign=+ factor=0.39 offset=0 unit="MDI"
    const uint32_t raw_adr_1_52c_momentenanforderung_adr = readBitsLE(frame.data, 0, 8);
    params::vw_pq.adr_1_52c_momentenanforderung_adr = raw_adr_1_52c_momentenanforderung_adr * 0.39f + 0.0f;
    return true;
}

void encodeVwPq0x52C(CANMessage &frame) {
    frame.id = 0x52C;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Checksumme_ADR_1
    int64_t raw_tmp_adr_1_52c_checksumme_adr_1 = (int64_t)llround((params::vw_pq.adr_1_52c_checksumme_adr_1 - 0.0f) / 1.0f);
    uint64_t raw_adr_1_52c_checksumme_adr_1 = raw_tmp_adr_1_52c_checksumme_adr_1 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_1_52c_checksumme_adr_1;
    if (raw_adr_1_52c_checksumme_adr_1 > 0xFFULL) raw_adr_1_52c_checksumme_adr_1 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_adr_1_52c_checksumme_adr_1);
    // Frei_ADR_1_5
    int64_t raw_tmp_adr_1_52c_frei_adr_1_5 = (int64_t)llround((params::vw_pq.adr_1_52c_frei_adr_1_5 - 0.0f) / 1.0f);
    uint64_t raw_adr_1_52c_frei_adr_1_5 = raw_tmp_adr_1_52c_frei_adr_1_5 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_1_52c_frei_adr_1_5;
    if (raw_adr_1_52c_frei_adr_1_5 > 0xFULL) raw_adr_1_52c_frei_adr_1_5 = 0xFULL;
    writeBitsLE(frame.data, 52, 4, (uint32_t)raw_adr_1_52c_frei_adr_1_5);
    // Zeitluecke_gemessen
    int64_t raw_tmp_adr_1_52c_zeitluecke_gemessen = (int64_t)llround((params::vw_pq.adr_1_52c_zeitluecke_gemessen - 0.0f) / 1.0f);
    uint64_t raw_adr_1_52c_zeitluecke_gemessen = raw_tmp_adr_1_52c_zeitluecke_gemessen < 0 ? 0ULL : (uint64_t)raw_tmp_adr_1_52c_zeitluecke_gemessen;
    if (raw_adr_1_52c_zeitluecke_gemessen > 0xFULL) raw_adr_1_52c_zeitluecke_gemessen = 0xFULL;
    writeBitsLE(frame.data, 48, 4, (uint32_t)raw_adr_1_52c_zeitluecke_gemessen);
    writeBitsLE(frame.data, 47, 1, params::vw_pq.adr_1_52c_fehlerspeichereintrag_adr ? 1U : 0U);
    // Reserviert_ADR_1_1
    int64_t raw_tmp_adr_1_52c_reserviert_adr_1_1 = (int64_t)llround((params::vw_pq.adr_1_52c_reserviert_adr_1_1 - 0.0f) / 1.0f);
    uint64_t raw_adr_1_52c_reserviert_adr_1_1 = raw_tmp_adr_1_52c_reserviert_adr_1_1 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_1_52c_reserviert_adr_1_1;
    if (raw_adr_1_52c_reserviert_adr_1_1 > 0x7FULL) raw_adr_1_52c_reserviert_adr_1_1 = 0x7FULL;
    writeBitsLE(frame.data, 40, 7, (uint32_t)raw_adr_1_52c_reserviert_adr_1_1);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.adr_1_52c_anzeige_sensor_blind ? 1U : 0U);
    writeBitsLE(frame.data, 38, 1, params::vw_pq.adr_1_52c_ansteuerung_optischer_fahrerhin ? 1U : 0U);
    writeBitsLE(frame.data, 37, 1, params::vw_pq.adr_1_52c_ansteuerung_gong_2 ? 1U : 0U);
    writeBitsLE(frame.data, 36, 1, params::vw_pq.adr_1_52c_ansteuerung_gong_1 ? 1U : 0U);
    // Schaltaufforderung
    int64_t raw_tmp_adr_1_52c_schaltaufforderung = (int64_t)llround((params::vw_pq.adr_1_52c_schaltaufforderung - 0.0f) / 1.0f);
    uint64_t raw_adr_1_52c_schaltaufforderung = raw_tmp_adr_1_52c_schaltaufforderung < 0 ? 0ULL : (uint64_t)raw_tmp_adr_1_52c_schaltaufforderung;
    if (raw_adr_1_52c_schaltaufforderung > 0x3ULL) raw_adr_1_52c_schaltaufforderung = 0x3ULL;
    writeBitsLE(frame.data, 34, 2, (uint32_t)raw_adr_1_52c_schaltaufforderung);
    writeBitsLE(frame.data, 33, 1, params::vw_pq.adr_1_52c_anzeige_prioritaet ? 1U : 0U);
    writeBitsLE(frame.data, 32, 1, params::vw_pq.adr_1_52c_anzeige_zeitluecke ? 1U : 0U);
    // Wunschgeschwindigkeit
    int64_t raw_tmp_adr_1_52c_wunschgeschwindigkeit = (int64_t)llround((params::vw_pq.adr_1_52c_wunschgeschwindigkeit - 0.0f) / 1.0f);
    uint64_t raw_adr_1_52c_wunschgeschwindigkeit = raw_tmp_adr_1_52c_wunschgeschwindigkeit < 0 ? 0ULL : (uint64_t)raw_tmp_adr_1_52c_wunschgeschwindigkeit;
    if (raw_adr_1_52c_wunschgeschwindigkeit > 0xFFULL) raw_adr_1_52c_wunschgeschwindigkeit = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_adr_1_52c_wunschgeschwindigkeit);
    // Objekt_erfasst
    int64_t raw_tmp_adr_1_52c_objekt_erfasst = (int64_t)llround((params::vw_pq.adr_1_52c_objekt_erfasst - 0.0f) / 1.0f);
    uint64_t raw_adr_1_52c_objekt_erfasst = raw_tmp_adr_1_52c_objekt_erfasst < 0 ? 0ULL : (uint64_t)raw_tmp_adr_1_52c_objekt_erfasst;
    if (raw_adr_1_52c_objekt_erfasst > 0x3ULL) raw_adr_1_52c_objekt_erfasst = 0x3ULL;
    writeBitsLE(frame.data, 22, 2, (uint32_t)raw_adr_1_52c_objekt_erfasst);
    // Gesetzte_Zeitluecke__ADR_1_
    int64_t raw_tmp_adr_1_52c_gesetzte_zeitluecke_adr_1 = (int64_t)llround((params::vw_pq.adr_1_52c_gesetzte_zeitluecke_adr_1 - 0.0f) / 1.0f);
    uint64_t raw_adr_1_52c_gesetzte_zeitluecke_adr_1 = raw_tmp_adr_1_52c_gesetzte_zeitluecke_adr_1 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_1_52c_gesetzte_zeitluecke_adr_1;
    if (raw_adr_1_52c_gesetzte_zeitluecke_adr_1 > 0xFULL) raw_adr_1_52c_gesetzte_zeitluecke_adr_1 = 0xFULL;
    writeBitsLE(frame.data, 18, 4, (uint32_t)raw_adr_1_52c_gesetzte_zeitluecke_adr_1);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.adr_1_52c_synchronisation_bremsbooster ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.adr_1_52c_momentenanforderung_freigabe ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.adr_1_52c_verhinderung_schubabschaltung ? 1U : 0U);
    // Status_ADR_1
    int64_t raw_tmp_adr_1_52c_status_adr_1 = (int64_t)llround((params::vw_pq.adr_1_52c_status_adr_1 - 0.0f) / 1.0f);
    uint64_t raw_adr_1_52c_status_adr_1 = raw_tmp_adr_1_52c_status_adr_1 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_1_52c_status_adr_1;
    if (raw_adr_1_52c_status_adr_1 > 0x3ULL) raw_adr_1_52c_status_adr_1 = 0x3ULL;
    writeBitsLE(frame.data, 13, 2, (uint32_t)raw_adr_1_52c_status_adr_1);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.adr_1_52c_fehler_adr_1 ? 1U : 0U);
    // Zaehler_ADR_1
    int64_t raw_tmp_adr_1_52c_zaehler_adr_1 = (int64_t)llround((params::vw_pq.adr_1_52c_zaehler_adr_1 - 0.0f) / 1.0f);
    uint64_t raw_adr_1_52c_zaehler_adr_1 = raw_tmp_adr_1_52c_zaehler_adr_1 < 0 ? 0ULL : (uint64_t)raw_tmp_adr_1_52c_zaehler_adr_1;
    if (raw_adr_1_52c_zaehler_adr_1 > 0xFULL) raw_adr_1_52c_zaehler_adr_1 = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_adr_1_52c_zaehler_adr_1);
    // Momentenanforderung_ADR
    int64_t raw_tmp_adr_1_52c_momentenanforderung_adr = (int64_t)llround((params::vw_pq.adr_1_52c_momentenanforderung_adr - 0.0f) / 0.39f);
    uint64_t raw_adr_1_52c_momentenanforderung_adr = raw_tmp_adr_1_52c_momentenanforderung_adr < 0 ? 0ULL : (uint64_t)raw_tmp_adr_1_52c_momentenanforderung_adr;
    if (raw_adr_1_52c_momentenanforderung_adr > 0xFFULL) raw_adr_1_52c_momentenanforderung_adr = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_adr_1_52c_momentenanforderung_adr);
}

// Message 0x60E (1550) - Einheiten_1
// DLC: 2, TX: XXX
bool decodeVwPq0x60E(const CANMessage &frame) {
    if (frame.id != 0x60E || frame.len < 2) {
        return false;
    }

    // MFA_v_Einheit_02: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.einheiten_1_60e_mfa_v_einheit_02 = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encodeVwPq0x60E(CANMessage &frame) {
    frame.id = 0x60E;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 0, 1, params::vw_pq.einheiten_1_60e_mfa_v_einheit_02 ? 1U : 0U);
}

// Message 0x368 (872) - ACC_System
// DLC: 8, TX: XXX
bool decodeVwPq0x368(const CANMessage &frame) {
    if (frame.id != 0x368 || frame.len < 8) {
        return false;
    }

    // CHECKSUM: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_acc_system_368_checksum = readBitsLE(frame.data, 0, 8);
    params::vw_pq.acc_system_368_checksum = raw_acc_system_368_checksum * 1.0f + 0.0f;
    // COUNTER: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_acc_system_368_counter = readBitsLE(frame.data, 8, 4);
    params::vw_pq.acc_system_368_counter = raw_acc_system_368_counter * 1.0f + 0.0f;
    // ACS_Sta_ADR: start=12 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 2 "ADR_passiv" 0 "ADR_nicht_aktiv" 1 "ADR_aktiv" 3 "irrev_Fehler"
    const uint32_t raw_acc_system_368_acs_sta_adr = readBitsLE(frame.data, 12, 2);
    params::vw_pq.acc_system_368_acs_sta_adr = raw_acc_system_368_acs_sta_adr * 1.0f + 0.0f;
    // ACS_ADR_Schub: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 1 "Verz_begr_auf_Schub" 0 "Verz_nicht_begr_auf_Schub"
    params::vw_pq.acc_system_368_acs_adr_schub = readBitsLE(frame.data, 14, 1) != 0;
    // ACS_Schubabsch: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 1 "SA_nicht_zulaessig" 0 "SA_zulaessig"
    params::vw_pq.acc_system_368_acs_schubabsch = readBitsLE(frame.data, 15, 1) != 0;
    // ACS_StSt_Info: start=16 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 3 "Systemfehler" 0 "Motorlauf_nn" 1 "Stoppverbot_Motoranlauf_nn" 2 "Motoranlauf_notwendig"
    const uint32_t raw_acc_system_368_acs_stst_info = readBitsLE(frame.data, 16, 2);
    params::vw_pq.acc_system_368_acs_stst_info = raw_acc_system_368_acs_stst_info * 1.0f + 0.0f;
    // ACS_MomEingriff: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 1 "MomEingr_verhindern" 0 "keine_Beeinfl_MomEingr_Mot"
    params::vw_pq.acc_system_368_acs_momeingriff = readBitsLE(frame.data, 18, 1) != 0;
    // ACS_Typ_ACC: start=19 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Basis_ACC" 1 "ACC_mit_FollowToStop" 3 "frei" 2 "frei"
    const uint32_t raw_acc_system_368_acs_typ_acc = readBitsLE(frame.data, 19, 2);
    params::vw_pq.acc_system_368_acs_typ_acc = raw_acc_system_368_acs_typ_acc * 1.0f + 0.0f;
    // ACS_FreigSollB: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Sollbeschl_nicht_freigeg" 1 "Sollbeschl_freigeg"
    params::vw_pq.acc_system_368_acs_freigsollb = readBitsLE(frame.data, 23, 1) != 0;
    // ACS_Sollbeschl: start=24 len=11 endian=1 sign=+ factor=0.005 offset=-7.22 unit="Unit_MeterPerSeconSquar"
    // Value table: 2046 "ADR_nicht_aktiv" 2047 "Fehler"
    const uint32_t raw_acc_system_368_acs_sollbeschl = readBitsLE(frame.data, 24, 11);
    params::vw_pq.acc_system_368_acs_sollbeschl = raw_acc_system_368_acs_sollbeschl * 0.005f + -7.22f;
    // ACS_Anhaltewunsch: start=38 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "kein_Haltewunsch" 1 "Fzg_haelt_an"
    params::vw_pq.acc_system_368_acs_anhaltewunsch = readBitsLE(frame.data, 38, 1) != 0;
    // ACS_Fehler: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 1 "Fehlerspeichereintrag" 0 "kein_Fehlerspeichereintrag"
    params::vw_pq.acc_system_368_acs_fehler = readBitsLE(frame.data, 39, 1) != 0;
    // ACS_zul_Regelabw: start=40 len=8 endian=1 sign=+ factor=0.005 offset=0 unit="Unit_MeterPerSeconSquar"
    // Value table: 254 "ADR_nicht_aktiv" 255 "Fehler"
    const uint32_t raw_acc_system_368_acs_zul_regelabw = readBitsLE(frame.data, 40, 8);
    params::vw_pq.acc_system_368_acs_zul_regelabw = raw_acc_system_368_acs_zul_regelabw * 0.005f + 0.0f;
    // ACS_max_AendGrad: start=48 len=8 endian=1 sign=+ factor=0.02 offset=0 unit="Unit_MeterPerSeconSquar"
    // Value table: 254 "Neutralwert" 0 "Neutralwert" 255 "Fehler"
    const uint32_t raw_acc_system_368_acs_max_aendgrad = readBitsLE(frame.data, 48, 8);
    params::vw_pq.acc_system_368_acs_max_aendgrad = raw_acc_system_368_acs_max_aendgrad * 0.02f + 0.0f;
    return true;
}

void encodeVwPq0x368(CANMessage &frame) {
    frame.id = 0x368;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // CHECKSUM
    int64_t raw_tmp_acc_system_368_checksum = (int64_t)llround((params::vw_pq.acc_system_368_checksum - 0.0f) / 1.0f);
    uint64_t raw_acc_system_368_checksum = raw_tmp_acc_system_368_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_acc_system_368_checksum;
    if (raw_acc_system_368_checksum > 0xFFULL) raw_acc_system_368_checksum = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_acc_system_368_checksum);
    // COUNTER
    int64_t raw_tmp_acc_system_368_counter = (int64_t)llround((params::vw_pq.acc_system_368_counter - 0.0f) / 1.0f);
    uint64_t raw_acc_system_368_counter = raw_tmp_acc_system_368_counter < 0 ? 0ULL : (uint64_t)raw_tmp_acc_system_368_counter;
    if (raw_acc_system_368_counter > 0xFULL) raw_acc_system_368_counter = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_acc_system_368_counter);
    // ACS_Sta_ADR
    int64_t raw_tmp_acc_system_368_acs_sta_adr = (int64_t)llround((params::vw_pq.acc_system_368_acs_sta_adr - 0.0f) / 1.0f);
    uint64_t raw_acc_system_368_acs_sta_adr = raw_tmp_acc_system_368_acs_sta_adr < 0 ? 0ULL : (uint64_t)raw_tmp_acc_system_368_acs_sta_adr;
    if (raw_acc_system_368_acs_sta_adr > 0x3ULL) raw_acc_system_368_acs_sta_adr = 0x3ULL;
    writeBitsLE(frame.data, 12, 2, (uint32_t)raw_acc_system_368_acs_sta_adr);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.acc_system_368_acs_adr_schub ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.acc_system_368_acs_schubabsch ? 1U : 0U);
    // ACS_StSt_Info
    int64_t raw_tmp_acc_system_368_acs_stst_info = (int64_t)llround((params::vw_pq.acc_system_368_acs_stst_info - 0.0f) / 1.0f);
    uint64_t raw_acc_system_368_acs_stst_info = raw_tmp_acc_system_368_acs_stst_info < 0 ? 0ULL : (uint64_t)raw_tmp_acc_system_368_acs_stst_info;
    if (raw_acc_system_368_acs_stst_info > 0x3ULL) raw_acc_system_368_acs_stst_info = 0x3ULL;
    writeBitsLE(frame.data, 16, 2, (uint32_t)raw_acc_system_368_acs_stst_info);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.acc_system_368_acs_momeingriff ? 1U : 0U);
    // ACS_Typ_ACC
    int64_t raw_tmp_acc_system_368_acs_typ_acc = (int64_t)llround((params::vw_pq.acc_system_368_acs_typ_acc - 0.0f) / 1.0f);
    uint64_t raw_acc_system_368_acs_typ_acc = raw_tmp_acc_system_368_acs_typ_acc < 0 ? 0ULL : (uint64_t)raw_tmp_acc_system_368_acs_typ_acc;
    if (raw_acc_system_368_acs_typ_acc > 0x3ULL) raw_acc_system_368_acs_typ_acc = 0x3ULL;
    writeBitsLE(frame.data, 19, 2, (uint32_t)raw_acc_system_368_acs_typ_acc);
    writeBitsLE(frame.data, 23, 1, params::vw_pq.acc_system_368_acs_freigsollb ? 1U : 0U);
    // ACS_Sollbeschl
    int64_t raw_tmp_acc_system_368_acs_sollbeschl = (int64_t)llround((params::vw_pq.acc_system_368_acs_sollbeschl - -7.22f) / 0.005f);
    uint64_t raw_acc_system_368_acs_sollbeschl = raw_tmp_acc_system_368_acs_sollbeschl < 0 ? 0ULL : (uint64_t)raw_tmp_acc_system_368_acs_sollbeschl;
    if (raw_acc_system_368_acs_sollbeschl > 0x7FFULL) raw_acc_system_368_acs_sollbeschl = 0x7FFULL;
    writeBitsLE(frame.data, 24, 11, (uint32_t)raw_acc_system_368_acs_sollbeschl);
    writeBitsLE(frame.data, 38, 1, params::vw_pq.acc_system_368_acs_anhaltewunsch ? 1U : 0U);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.acc_system_368_acs_fehler ? 1U : 0U);
    // ACS_zul_Regelabw
    int64_t raw_tmp_acc_system_368_acs_zul_regelabw = (int64_t)llround((params::vw_pq.acc_system_368_acs_zul_regelabw - 0.0f) / 0.005f);
    uint64_t raw_acc_system_368_acs_zul_regelabw = raw_tmp_acc_system_368_acs_zul_regelabw < 0 ? 0ULL : (uint64_t)raw_tmp_acc_system_368_acs_zul_regelabw;
    if (raw_acc_system_368_acs_zul_regelabw > 0xFFULL) raw_acc_system_368_acs_zul_regelabw = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_acc_system_368_acs_zul_regelabw);
    // ACS_max_AendGrad
    int64_t raw_tmp_acc_system_368_acs_max_aendgrad = (int64_t)llround((params::vw_pq.acc_system_368_acs_max_aendgrad - 0.0f) / 0.02f);
    uint64_t raw_acc_system_368_acs_max_aendgrad = raw_tmp_acc_system_368_acs_max_aendgrad < 0 ? 0ULL : (uint64_t)raw_tmp_acc_system_368_acs_max_aendgrad;
    if (raw_acc_system_368_acs_max_aendgrad > 0xFFULL) raw_acc_system_368_acs_max_aendgrad = 0xFFULL;
    writeBitsLE(frame.data, 48, 8, (uint32_t)raw_acc_system_368_acs_max_aendgrad);
}

// Message 0x56A (1386) - ACC_GRA_Anzeige
// DLC: 8, TX: XXX
bool decodeVwPq0x56A(const CANMessage &frame) {
    if (frame.id != 0x56A || frame.len < 8) {
        return false;
    }

    // CHECKSUM: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_acc_gra_anzeige_56a_checksum = readBitsLE(frame.data, 0, 8);
    params::vw_pq.acc_gra_anzeige_56a_checksum = raw_acc_gra_anzeige_56a_checksum * 1.0f + 0.0f;
    // ACA_StaACC: start=8 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 6 "ACC_rev_aus" 0 "Hauptschalter_aus" 4 "ACC_im_Hintergrund" 3 "ACC_aktiv" 1 "Reserve" 2 "ACC_passiv" 7 "ACC_irrev_aus" 5 "frei"
    const uint32_t raw_acc_gra_anzeige_56a_aca_staacc = readBitsLE(frame.data, 8, 3);
    params::vw_pq.acc_gra_anzeige_56a_aca_staacc = raw_acc_gra_anzeige_56a_aca_staacc * 1.0f + 0.0f;
    // ACA_ID_StaACC: start=11 len=5 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "keine_Anzeige"
    const uint32_t raw_acc_gra_anzeige_56a_aca_id_staacc = readBitsLE(frame.data, 11, 5);
    params::vw_pq.acc_gra_anzeige_56a_aca_id_staacc = raw_acc_gra_anzeige_56a_aca_id_staacc * 1.0f + 0.0f;
    // ACA_Fahrerhinw: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 1 "Ein" 0 "Aus"
    params::vw_pq.acc_gra_anzeige_56a_aca_fahrerhinw = readBitsLE(frame.data, 16, 1) != 0;
    // ACA_AnzDisplay: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 1 "Anzeige_erw" 0 "Anzeige_nicht_erw"
    params::vw_pq.acc_gra_anzeige_56a_aca_anzdisplay = readBitsLE(frame.data, 17, 1) != 0;
    // ACA_Zeitluecke: start=18 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 3 "Zeitluecke3" 10 "Zeitluecke10" 4 "Zeitluecke4" 14 "Zeitluecke14" 11 "Zeitluecke11" 2 "Zeitluecke2" 13 "Zeitluecke13" 9 "Zeitluecke9" 1 "Zeitluecke1" 8 "Zeitluecke8" 5 "Zeitluecke5" 15 "Zeitluecke15" 0 "nicht_definiert" 12 "Zeitluecke12" 6 "Zeitluecke6" 7 "Zeitluecke7"
    const uint32_t raw_acc_gra_anzeige_56a_aca_zeitluecke = readBitsLE(frame.data, 18, 4);
    params::vw_pq.acc_gra_anzeige_56a_aca_zeitluecke = raw_acc_gra_anzeige_56a_aca_zeitluecke * 1.0f + 0.0f;
    // ACA_V_Wunsch: start=24 len=8 endian=1 sign=+ factor=1 offset=0 unit="Unit_KiloMeterPerHour"
    // DBC comment: 255=unset
    // Value table: 255 "kein_Wert_im_Speicher"
    const uint32_t raw_acc_gra_anzeige_56a_aca_v_wunsch = readBitsLE(frame.data, 24, 8);
    params::vw_pq.acc_gra_anzeige_56a_aca_v_wunsch = raw_acc_gra_anzeige_56a_aca_v_wunsch * 1.0f + 0.0f;
    // ACA_kmh_mph: start=32 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "km_h" 1 "mph"
    params::vw_pq.acc_gra_anzeige_56a_aca_kmh_mph = readBitsLE(frame.data, 32, 1) != 0;
    // ACA_Akustik1: start=33 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "kein_Gong" 1 "Gong"
    params::vw_pq.acc_gra_anzeige_56a_aca_akustik1 = readBitsLE(frame.data, 33, 1) != 0;
    // ACA_Akustik2: start=34 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "kein_Summer" 1 "Summer"
    params::vw_pq.acc_gra_anzeige_56a_aca_akustik2 = readBitsLE(frame.data, 34, 1) != 0;
    // ACA_PrioDisp: start=35 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 1 "mittlere_Prio" 3 "keine_Anzeige_Anf" 0 "hohe_Prio" 2 "niedrige_Prio"
    const uint32_t raw_acc_gra_anzeige_56a_aca_priodisp = readBitsLE(frame.data, 35, 2);
    params::vw_pq.acc_gra_anzeige_56a_aca_priodisp = raw_acc_gra_anzeige_56a_aca_priodisp * 1.0f + 0.0f;
    // ACA_gemZeitl: start=40 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 6 "Zeitluecke6" 2 "Zeitluecke2" 7 "Zeitluecke7" 13 "Zeitluecke13" 11 "Zeitluecke11" 4 "Zeitluecke4" 8 "Zeitluecke8" 12 "Zeitluecke12" 10 "Zeitluecke10" 0 "Kein_Objekt_erfasst" 1 "Zeitluecke1" 3 "Zeitluecke3" 9 "Zeitluecke9" 15 "Zeitluecke15" 14 "Zeitluecke14" 5 "Zeitluecke5"
    const uint32_t raw_acc_gra_anzeige_56a_aca_gemzeitl = readBitsLE(frame.data, 40, 4);
    params::vw_pq.acc_gra_anzeige_56a_aca_gemzeitl = raw_acc_gra_anzeige_56a_aca_gemzeitl * 1.0f + 0.0f;
    // ACA_ACC_Verz: start=44 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "ACC_verzoegert_nicht" 1 "ACC_verzoegert"
    params::vw_pq.acc_gra_anzeige_56a_aca_acc_verz = readBitsLE(frame.data, 44, 1) != 0;
    // ACA_StaGRA: start=48 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 3 "GRA_aktiv" 4 "GRA_uebertreten" 2 "GRA_passiv" 0 "Hauptschalter_aus" 6 "frei" 7 "GRA_Fehler" 1 "Reserve" 5 "frei"
    const uint32_t raw_acc_gra_anzeige_56a_aca_stagra = readBitsLE(frame.data, 48, 3);
    params::vw_pq.acc_gra_anzeige_56a_aca_stagra = raw_acc_gra_anzeige_56a_aca_stagra * 1.0f + 0.0f;
    // ACA_ID_StaGRA: start=51 len=5 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "keine_Anzeige"
    const uint32_t raw_acc_gra_anzeige_56a_aca_id_stagra = readBitsLE(frame.data, 51, 5);
    params::vw_pq.acc_gra_anzeige_56a_aca_id_stagra = raw_acc_gra_anzeige_56a_aca_id_stagra * 1.0f + 0.0f;
    // ACA_Codierung: start=56 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "ACC" 1 "GRA"
    params::vw_pq.acc_gra_anzeige_56a_aca_codierung = readBitsLE(frame.data, 56, 1) != 0;
    // ACA_Tachokranz: start=57 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "nicht_beleuchtet" 1 "beleuchtet"
    params::vw_pq.acc_gra_anzeige_56a_aca_tachokranz = readBitsLE(frame.data, 57, 1) != 0;
    // ACA_Aend_Zeitluecke: start=58 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 1 "Anzeige_angef" 0 "keine_Anzeige"
    params::vw_pq.acc_gra_anzeige_56a_aca_aend_zeitluecke = readBitsLE(frame.data, 58, 1) != 0;
    // COUNTER: start=60 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_acc_gra_anzeige_56a_counter = readBitsLE(frame.data, 60, 4);
    params::vw_pq.acc_gra_anzeige_56a_counter = raw_acc_gra_anzeige_56a_counter * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x56A(CANMessage &frame) {
    frame.id = 0x56A;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // CHECKSUM
    int64_t raw_tmp_acc_gra_anzeige_56a_checksum = (int64_t)llround((params::vw_pq.acc_gra_anzeige_56a_checksum - 0.0f) / 1.0f);
    uint64_t raw_acc_gra_anzeige_56a_checksum = raw_tmp_acc_gra_anzeige_56a_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_acc_gra_anzeige_56a_checksum;
    if (raw_acc_gra_anzeige_56a_checksum > 0xFFULL) raw_acc_gra_anzeige_56a_checksum = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_acc_gra_anzeige_56a_checksum);
    // ACA_StaACC
    int64_t raw_tmp_acc_gra_anzeige_56a_aca_staacc = (int64_t)llround((params::vw_pq.acc_gra_anzeige_56a_aca_staacc - 0.0f) / 1.0f);
    uint64_t raw_acc_gra_anzeige_56a_aca_staacc = raw_tmp_acc_gra_anzeige_56a_aca_staacc < 0 ? 0ULL : (uint64_t)raw_tmp_acc_gra_anzeige_56a_aca_staacc;
    if (raw_acc_gra_anzeige_56a_aca_staacc > 0x7ULL) raw_acc_gra_anzeige_56a_aca_staacc = 0x7ULL;
    writeBitsLE(frame.data, 8, 3, (uint32_t)raw_acc_gra_anzeige_56a_aca_staacc);
    // ACA_ID_StaACC
    int64_t raw_tmp_acc_gra_anzeige_56a_aca_id_staacc = (int64_t)llround((params::vw_pq.acc_gra_anzeige_56a_aca_id_staacc - 0.0f) / 1.0f);
    uint64_t raw_acc_gra_anzeige_56a_aca_id_staacc = raw_tmp_acc_gra_anzeige_56a_aca_id_staacc < 0 ? 0ULL : (uint64_t)raw_tmp_acc_gra_anzeige_56a_aca_id_staacc;
    if (raw_acc_gra_anzeige_56a_aca_id_staacc > 0x1FULL) raw_acc_gra_anzeige_56a_aca_id_staacc = 0x1FULL;
    writeBitsLE(frame.data, 11, 5, (uint32_t)raw_acc_gra_anzeige_56a_aca_id_staacc);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.acc_gra_anzeige_56a_aca_fahrerhinw ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.acc_gra_anzeige_56a_aca_anzdisplay ? 1U : 0U);
    // ACA_Zeitluecke
    int64_t raw_tmp_acc_gra_anzeige_56a_aca_zeitluecke = (int64_t)llround((params::vw_pq.acc_gra_anzeige_56a_aca_zeitluecke - 0.0f) / 1.0f);
    uint64_t raw_acc_gra_anzeige_56a_aca_zeitluecke = raw_tmp_acc_gra_anzeige_56a_aca_zeitluecke < 0 ? 0ULL : (uint64_t)raw_tmp_acc_gra_anzeige_56a_aca_zeitluecke;
    if (raw_acc_gra_anzeige_56a_aca_zeitluecke > 0xFULL) raw_acc_gra_anzeige_56a_aca_zeitluecke = 0xFULL;
    writeBitsLE(frame.data, 18, 4, (uint32_t)raw_acc_gra_anzeige_56a_aca_zeitluecke);
    // ACA_V_Wunsch
    int64_t raw_tmp_acc_gra_anzeige_56a_aca_v_wunsch = (int64_t)llround((params::vw_pq.acc_gra_anzeige_56a_aca_v_wunsch - 0.0f) / 1.0f);
    uint64_t raw_acc_gra_anzeige_56a_aca_v_wunsch = raw_tmp_acc_gra_anzeige_56a_aca_v_wunsch < 0 ? 0ULL : (uint64_t)raw_tmp_acc_gra_anzeige_56a_aca_v_wunsch;
    if (raw_acc_gra_anzeige_56a_aca_v_wunsch > 0xFFULL) raw_acc_gra_anzeige_56a_aca_v_wunsch = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_acc_gra_anzeige_56a_aca_v_wunsch);
    writeBitsLE(frame.data, 32, 1, params::vw_pq.acc_gra_anzeige_56a_aca_kmh_mph ? 1U : 0U);
    writeBitsLE(frame.data, 33, 1, params::vw_pq.acc_gra_anzeige_56a_aca_akustik1 ? 1U : 0U);
    writeBitsLE(frame.data, 34, 1, params::vw_pq.acc_gra_anzeige_56a_aca_akustik2 ? 1U : 0U);
    // ACA_PrioDisp
    int64_t raw_tmp_acc_gra_anzeige_56a_aca_priodisp = (int64_t)llround((params::vw_pq.acc_gra_anzeige_56a_aca_priodisp - 0.0f) / 1.0f);
    uint64_t raw_acc_gra_anzeige_56a_aca_priodisp = raw_tmp_acc_gra_anzeige_56a_aca_priodisp < 0 ? 0ULL : (uint64_t)raw_tmp_acc_gra_anzeige_56a_aca_priodisp;
    if (raw_acc_gra_anzeige_56a_aca_priodisp > 0x3ULL) raw_acc_gra_anzeige_56a_aca_priodisp = 0x3ULL;
    writeBitsLE(frame.data, 35, 2, (uint32_t)raw_acc_gra_anzeige_56a_aca_priodisp);
    // ACA_gemZeitl
    int64_t raw_tmp_acc_gra_anzeige_56a_aca_gemzeitl = (int64_t)llround((params::vw_pq.acc_gra_anzeige_56a_aca_gemzeitl - 0.0f) / 1.0f);
    uint64_t raw_acc_gra_anzeige_56a_aca_gemzeitl = raw_tmp_acc_gra_anzeige_56a_aca_gemzeitl < 0 ? 0ULL : (uint64_t)raw_tmp_acc_gra_anzeige_56a_aca_gemzeitl;
    if (raw_acc_gra_anzeige_56a_aca_gemzeitl > 0xFULL) raw_acc_gra_anzeige_56a_aca_gemzeitl = 0xFULL;
    writeBitsLE(frame.data, 40, 4, (uint32_t)raw_acc_gra_anzeige_56a_aca_gemzeitl);
    writeBitsLE(frame.data, 44, 1, params::vw_pq.acc_gra_anzeige_56a_aca_acc_verz ? 1U : 0U);
    // ACA_StaGRA
    int64_t raw_tmp_acc_gra_anzeige_56a_aca_stagra = (int64_t)llround((params::vw_pq.acc_gra_anzeige_56a_aca_stagra - 0.0f) / 1.0f);
    uint64_t raw_acc_gra_anzeige_56a_aca_stagra = raw_tmp_acc_gra_anzeige_56a_aca_stagra < 0 ? 0ULL : (uint64_t)raw_tmp_acc_gra_anzeige_56a_aca_stagra;
    if (raw_acc_gra_anzeige_56a_aca_stagra > 0x7ULL) raw_acc_gra_anzeige_56a_aca_stagra = 0x7ULL;
    writeBitsLE(frame.data, 48, 3, (uint32_t)raw_acc_gra_anzeige_56a_aca_stagra);
    // ACA_ID_StaGRA
    int64_t raw_tmp_acc_gra_anzeige_56a_aca_id_stagra = (int64_t)llround((params::vw_pq.acc_gra_anzeige_56a_aca_id_stagra - 0.0f) / 1.0f);
    uint64_t raw_acc_gra_anzeige_56a_aca_id_stagra = raw_tmp_acc_gra_anzeige_56a_aca_id_stagra < 0 ? 0ULL : (uint64_t)raw_tmp_acc_gra_anzeige_56a_aca_id_stagra;
    if (raw_acc_gra_anzeige_56a_aca_id_stagra > 0x1FULL) raw_acc_gra_anzeige_56a_aca_id_stagra = 0x1FULL;
    writeBitsLE(frame.data, 51, 5, (uint32_t)raw_acc_gra_anzeige_56a_aca_id_stagra);
    writeBitsLE(frame.data, 56, 1, params::vw_pq.acc_gra_anzeige_56a_aca_codierung ? 1U : 0U);
    writeBitsLE(frame.data, 57, 1, params::vw_pq.acc_gra_anzeige_56a_aca_tachokranz ? 1U : 0U);
    writeBitsLE(frame.data, 58, 1, params::vw_pq.acc_gra_anzeige_56a_aca_aend_zeitluecke ? 1U : 0U);
    // COUNTER
    int64_t raw_tmp_acc_gra_anzeige_56a_counter = (int64_t)llround((params::vw_pq.acc_gra_anzeige_56a_counter - 0.0f) / 1.0f);
    uint64_t raw_acc_gra_anzeige_56a_counter = raw_tmp_acc_gra_anzeige_56a_counter < 0 ? 0ULL : (uint64_t)raw_tmp_acc_gra_anzeige_56a_counter;
    if (raw_acc_gra_anzeige_56a_counter > 0xFULL) raw_acc_gra_anzeige_56a_counter = 0xFULL;
    writeBitsLE(frame.data, 60, 4, (uint32_t)raw_acc_gra_anzeige_56a_counter);
}

// Message 0x0D0 (208) - Lenkhilfe_3
// DLC: 6, TX: XXX
bool decodeVwPq0xD0(const CANMessage &frame) {
    if (frame.id != 0xD0 || frame.len < 6) {
        return false;
    }

    // CHECKSUM: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_3_0d0_checksum = readBitsLE(frame.data, 0, 8);
    params::vw_pq.lenkhilfe_3_0d0_checksum = raw_lenkhilfe_3_0d0_checksum * 1.0f + 0.0f;
    // LH3_BS_Spiegel: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_3_0d0_lh3_bs_spiegel = readBitsLE(frame.data, 8, 4);
    params::vw_pq.lenkhilfe_3_0d0_lh3_bs_spiegel = raw_lenkhilfe_3_0d0_lh3_bs_spiegel * 1.0f + 0.0f;
    // COUNTER: start=12 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_3_0d0_counter = readBitsLE(frame.data, 12, 4);
    params::vw_pq.lenkhilfe_3_0d0_counter = raw_lenkhilfe_3_0d0_counter * 1.0f + 0.0f;
    // LH3_LM: start=16 len=10 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_3_0d0_lh3_lm = readBitsLE(frame.data, 16, 10);
    params::vw_pq.lenkhilfe_3_0d0_lh3_lm = raw_lenkhilfe_3_0d0_lh3_lm * 1.0f + 0.0f;
    // LH3_LMSign: start=26 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_3_0d0_lh3_lmsign = readBitsLE(frame.data, 26, 1) != 0;
    // LH3_LMValid: start=27 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_3_0d0_lh3_lmvalid = readBitsLE(frame.data, 27, 1) != 0;
    // LH3_Sta_DSR: start=28 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_3_0d0_lh3_sta_dsr = readBitsLE(frame.data, 28, 4);
    params::vw_pq.lenkhilfe_3_0d0_lh3_sta_dsr = raw_lenkhilfe_3_0d0_lh3_sta_dsr * 1.0f + 0.0f;
    // LH3_BLW: start=32 len=12 endian=1 sign=+ factor=0.15 offset=0 unit=""
    const uint32_t raw_lenkhilfe_3_0d0_lh3_blw = readBitsLE(frame.data, 32, 12);
    params::vw_pq.lenkhilfe_3_0d0_lh3_blw = raw_lenkhilfe_3_0d0_lh3_blw * 0.15f + 0.0f;
    // LH3_BLWSign: start=44 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_3_0d0_lh3_blwsign = readBitsLE(frame.data, 44, 1) != 0;
    // LH3_BLWValid: start=45 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_3_0d0_lh3_blwvalid = readBitsLE(frame.data, 45, 1) != 0;
    // LH3_Lenkungstyp: start=46 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_3_0d0_lh3_lenkungstyp = readBitsLE(frame.data, 46, 2);
    params::vw_pq.lenkhilfe_3_0d0_lh3_lenkungstyp = raw_lenkhilfe_3_0d0_lh3_lenkungstyp * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0xD0(CANMessage &frame) {
    frame.id = 0xD0;
    frame.len = 6;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // CHECKSUM
    int64_t raw_tmp_lenkhilfe_3_0d0_checksum = (int64_t)llround((params::vw_pq.lenkhilfe_3_0d0_checksum - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_3_0d0_checksum = raw_tmp_lenkhilfe_3_0d0_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_3_0d0_checksum;
    if (raw_lenkhilfe_3_0d0_checksum > 0xFFULL) raw_lenkhilfe_3_0d0_checksum = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_lenkhilfe_3_0d0_checksum);
    // LH3_BS_Spiegel
    int64_t raw_tmp_lenkhilfe_3_0d0_lh3_bs_spiegel = (int64_t)llround((params::vw_pq.lenkhilfe_3_0d0_lh3_bs_spiegel - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_3_0d0_lh3_bs_spiegel = raw_tmp_lenkhilfe_3_0d0_lh3_bs_spiegel < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_3_0d0_lh3_bs_spiegel;
    if (raw_lenkhilfe_3_0d0_lh3_bs_spiegel > 0xFULL) raw_lenkhilfe_3_0d0_lh3_bs_spiegel = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_lenkhilfe_3_0d0_lh3_bs_spiegel);
    // COUNTER
    int64_t raw_tmp_lenkhilfe_3_0d0_counter = (int64_t)llround((params::vw_pq.lenkhilfe_3_0d0_counter - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_3_0d0_counter = raw_tmp_lenkhilfe_3_0d0_counter < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_3_0d0_counter;
    if (raw_lenkhilfe_3_0d0_counter > 0xFULL) raw_lenkhilfe_3_0d0_counter = 0xFULL;
    writeBitsLE(frame.data, 12, 4, (uint32_t)raw_lenkhilfe_3_0d0_counter);
    // LH3_LM
    int64_t raw_tmp_lenkhilfe_3_0d0_lh3_lm = (int64_t)llround((params::vw_pq.lenkhilfe_3_0d0_lh3_lm - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_3_0d0_lh3_lm = raw_tmp_lenkhilfe_3_0d0_lh3_lm < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_3_0d0_lh3_lm;
    if (raw_lenkhilfe_3_0d0_lh3_lm > 0x3FFULL) raw_lenkhilfe_3_0d0_lh3_lm = 0x3FFULL;
    writeBitsLE(frame.data, 16, 10, (uint32_t)raw_lenkhilfe_3_0d0_lh3_lm);
    writeBitsLE(frame.data, 26, 1, params::vw_pq.lenkhilfe_3_0d0_lh3_lmsign ? 1U : 0U);
    writeBitsLE(frame.data, 27, 1, params::vw_pq.lenkhilfe_3_0d0_lh3_lmvalid ? 1U : 0U);
    // LH3_Sta_DSR
    int64_t raw_tmp_lenkhilfe_3_0d0_lh3_sta_dsr = (int64_t)llround((params::vw_pq.lenkhilfe_3_0d0_lh3_sta_dsr - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_3_0d0_lh3_sta_dsr = raw_tmp_lenkhilfe_3_0d0_lh3_sta_dsr < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_3_0d0_lh3_sta_dsr;
    if (raw_lenkhilfe_3_0d0_lh3_sta_dsr > 0xFULL) raw_lenkhilfe_3_0d0_lh3_sta_dsr = 0xFULL;
    writeBitsLE(frame.data, 28, 4, (uint32_t)raw_lenkhilfe_3_0d0_lh3_sta_dsr);
    // LH3_BLW
    int64_t raw_tmp_lenkhilfe_3_0d0_lh3_blw = (int64_t)llround((params::vw_pq.lenkhilfe_3_0d0_lh3_blw - 0.0f) / 0.15f);
    uint64_t raw_lenkhilfe_3_0d0_lh3_blw = raw_tmp_lenkhilfe_3_0d0_lh3_blw < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_3_0d0_lh3_blw;
    if (raw_lenkhilfe_3_0d0_lh3_blw > 0xFFFULL) raw_lenkhilfe_3_0d0_lh3_blw = 0xFFFULL;
    writeBitsLE(frame.data, 32, 12, (uint32_t)raw_lenkhilfe_3_0d0_lh3_blw);
    writeBitsLE(frame.data, 44, 1, params::vw_pq.lenkhilfe_3_0d0_lh3_blwsign ? 1U : 0U);
    writeBitsLE(frame.data, 45, 1, params::vw_pq.lenkhilfe_3_0d0_lh3_blwvalid ? 1U : 0U);
    // LH3_Lenkungstyp
    int64_t raw_tmp_lenkhilfe_3_0d0_lh3_lenkungstyp = (int64_t)llround((params::vw_pq.lenkhilfe_3_0d0_lh3_lenkungstyp - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_3_0d0_lh3_lenkungstyp = raw_tmp_lenkhilfe_3_0d0_lh3_lenkungstyp < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_3_0d0_lh3_lenkungstyp;
    if (raw_lenkhilfe_3_0d0_lh3_lenkungstyp > 0x3ULL) raw_lenkhilfe_3_0d0_lh3_lenkungstyp = 0x3ULL;
    writeBitsLE(frame.data, 46, 2, (uint32_t)raw_lenkhilfe_3_0d0_lh3_lenkungstyp);
}

// Message 0x3D2 (978) - Lenkhilfe_2
// DLC: 7, TX: XXX
bool decodeVwPq0x3D2(const CANMessage &frame) {
    if (frame.id != 0x3D2 || frame.len < 7) {
        return false;
    }

    // CHECKSUM: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_2_3d2_checksum = readBitsLE(frame.data, 0, 8);
    params::vw_pq.lenkhilfe_2_3d2_checksum = raw_lenkhilfe_2_3d2_checksum * 1.0f + 0.0f;
    // COUNTER: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_2_3d2_counter = readBitsLE(frame.data, 8, 4);
    params::vw_pq.lenkhilfe_2_3d2_counter = raw_lenkhilfe_2_3d2_counter * 1.0f + 0.0f;
    // LH2_Geradeaus: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_2_3d2_lh2_geradeaus = readBitsLE(frame.data, 12, 1) != 0;
    // LH2_Sta_Charisma: start=13 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_2_3d2_lh2_sta_charisma = readBitsLE(frame.data, 13, 3);
    params::vw_pq.lenkhilfe_2_3d2_lh2_sta_charisma = raw_lenkhilfe_2_3d2_lh2_sta_charisma * 1.0f + 0.0f;
    // LH2_Sta_HCA: start=16 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "disabled" 1 "initializing" 2 "fault" 3 "ready" 4 "rejected" 5 "active" 7 "active"
    const uint32_t raw_lenkhilfe_2_3d2_lh2_sta_hca = readBitsLE(frame.data, 16, 4);
    params::vw_pq.lenkhilfe_2_3d2_lh2_sta_hca = raw_lenkhilfe_2_3d2_lh2_sta_hca * 1.0f + 0.0f;
    // LH2_Ausg_LW1: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_2_3d2_lh2_ausg_lw1 = readBitsLE(frame.data, 20, 1) != 0;
    // LH2_Ausg_LW1_gue: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.lenkhilfe_2_3d2_lh2_ausg_lw1_gue = readBitsLE(frame.data, 21, 1) != 0;
    // LH2_StatEPS_PLA: start=24 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_2_3d2_lh2_stateps_pla = readBitsLE(frame.data, 24, 4);
    params::vw_pq.lenkhilfe_2_3d2_lh2_stateps_pla = raw_lenkhilfe_2_3d2_lh2_stateps_pla * 1.0f + 0.0f;
    // LH2_aktLenkeingriff: start=32 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_2_3d2_lh2_aktlenkeingriff = readBitsLE(frame.data, 32, 8);
    params::vw_pq.lenkhilfe_2_3d2_lh2_aktlenkeingriff = raw_lenkhilfe_2_3d2_lh2_aktlenkeingriff * 1.0f + 0.0f;
    // LH2_PLA_Err: start=48 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_2_3d2_lh2_pla_err = readBitsLE(frame.data, 48, 4);
    params::vw_pq.lenkhilfe_2_3d2_lh2_pla_err = raw_lenkhilfe_2_3d2_lh2_pla_err * 1.0f + 0.0f;
    // LH2_PLA_Abbr: start=52 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_lenkhilfe_2_3d2_lh2_pla_abbr = readBitsLE(frame.data, 52, 4);
    params::vw_pq.lenkhilfe_2_3d2_lh2_pla_abbr = raw_lenkhilfe_2_3d2_lh2_pla_abbr * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x3D2(CANMessage &frame) {
    frame.id = 0x3D2;
    frame.len = 7;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // CHECKSUM
    int64_t raw_tmp_lenkhilfe_2_3d2_checksum = (int64_t)llround((params::vw_pq.lenkhilfe_2_3d2_checksum - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_2_3d2_checksum = raw_tmp_lenkhilfe_2_3d2_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_2_3d2_checksum;
    if (raw_lenkhilfe_2_3d2_checksum > 0xFFULL) raw_lenkhilfe_2_3d2_checksum = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_lenkhilfe_2_3d2_checksum);
    // COUNTER
    int64_t raw_tmp_lenkhilfe_2_3d2_counter = (int64_t)llround((params::vw_pq.lenkhilfe_2_3d2_counter - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_2_3d2_counter = raw_tmp_lenkhilfe_2_3d2_counter < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_2_3d2_counter;
    if (raw_lenkhilfe_2_3d2_counter > 0xFULL) raw_lenkhilfe_2_3d2_counter = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_lenkhilfe_2_3d2_counter);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.lenkhilfe_2_3d2_lh2_geradeaus ? 1U : 0U);
    // LH2_Sta_Charisma
    int64_t raw_tmp_lenkhilfe_2_3d2_lh2_sta_charisma = (int64_t)llround((params::vw_pq.lenkhilfe_2_3d2_lh2_sta_charisma - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_2_3d2_lh2_sta_charisma = raw_tmp_lenkhilfe_2_3d2_lh2_sta_charisma < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_2_3d2_lh2_sta_charisma;
    if (raw_lenkhilfe_2_3d2_lh2_sta_charisma > 0x7ULL) raw_lenkhilfe_2_3d2_lh2_sta_charisma = 0x7ULL;
    writeBitsLE(frame.data, 13, 3, (uint32_t)raw_lenkhilfe_2_3d2_lh2_sta_charisma);
    // LH2_Sta_HCA
    int64_t raw_tmp_lenkhilfe_2_3d2_lh2_sta_hca = (int64_t)llround((params::vw_pq.lenkhilfe_2_3d2_lh2_sta_hca - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_2_3d2_lh2_sta_hca = raw_tmp_lenkhilfe_2_3d2_lh2_sta_hca < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_2_3d2_lh2_sta_hca;
    if (raw_lenkhilfe_2_3d2_lh2_sta_hca > 0xFULL) raw_lenkhilfe_2_3d2_lh2_sta_hca = 0xFULL;
    writeBitsLE(frame.data, 16, 4, (uint32_t)raw_lenkhilfe_2_3d2_lh2_sta_hca);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.lenkhilfe_2_3d2_lh2_ausg_lw1 ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.lenkhilfe_2_3d2_lh2_ausg_lw1_gue ? 1U : 0U);
    // LH2_StatEPS_PLA
    int64_t raw_tmp_lenkhilfe_2_3d2_lh2_stateps_pla = (int64_t)llround((params::vw_pq.lenkhilfe_2_3d2_lh2_stateps_pla - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_2_3d2_lh2_stateps_pla = raw_tmp_lenkhilfe_2_3d2_lh2_stateps_pla < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_2_3d2_lh2_stateps_pla;
    if (raw_lenkhilfe_2_3d2_lh2_stateps_pla > 0xFULL) raw_lenkhilfe_2_3d2_lh2_stateps_pla = 0xFULL;
    writeBitsLE(frame.data, 24, 4, (uint32_t)raw_lenkhilfe_2_3d2_lh2_stateps_pla);
    // LH2_aktLenkeingriff
    int64_t raw_tmp_lenkhilfe_2_3d2_lh2_aktlenkeingriff = (int64_t)llround((params::vw_pq.lenkhilfe_2_3d2_lh2_aktlenkeingriff - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_2_3d2_lh2_aktlenkeingriff = raw_tmp_lenkhilfe_2_3d2_lh2_aktlenkeingriff < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_2_3d2_lh2_aktlenkeingriff;
    if (raw_lenkhilfe_2_3d2_lh2_aktlenkeingriff > 0xFFULL) raw_lenkhilfe_2_3d2_lh2_aktlenkeingriff = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_lenkhilfe_2_3d2_lh2_aktlenkeingriff);
    // LH2_PLA_Err
    int64_t raw_tmp_lenkhilfe_2_3d2_lh2_pla_err = (int64_t)llround((params::vw_pq.lenkhilfe_2_3d2_lh2_pla_err - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_2_3d2_lh2_pla_err = raw_tmp_lenkhilfe_2_3d2_lh2_pla_err < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_2_3d2_lh2_pla_err;
    if (raw_lenkhilfe_2_3d2_lh2_pla_err > 0xFULL) raw_lenkhilfe_2_3d2_lh2_pla_err = 0xFULL;
    writeBitsLE(frame.data, 48, 4, (uint32_t)raw_lenkhilfe_2_3d2_lh2_pla_err);
    // LH2_PLA_Abbr
    int64_t raw_tmp_lenkhilfe_2_3d2_lh2_pla_abbr = (int64_t)llround((params::vw_pq.lenkhilfe_2_3d2_lh2_pla_abbr - 0.0f) / 1.0f);
    uint64_t raw_lenkhilfe_2_3d2_lh2_pla_abbr = raw_tmp_lenkhilfe_2_3d2_lh2_pla_abbr < 0 ? 0ULL : (uint64_t)raw_tmp_lenkhilfe_2_3d2_lh2_pla_abbr;
    if (raw_lenkhilfe_2_3d2_lh2_pla_abbr > 0xFULL) raw_lenkhilfe_2_3d2_lh2_pla_abbr = 0xFULL;
    writeBitsLE(frame.data, 52, 4, (uint32_t)raw_lenkhilfe_2_3d2_lh2_pla_abbr);
}

// Message 0x3D4 (980) - PLA_1
// DLC: 8, TX: XXX
bool decodeVwPq0x3D4(const CANMessage &frame) {
    if (frame.id != 0x3D4 || frame.len < 8) {
        return false;
    }

    // CHECKSUM: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_pla_1_3d4_checksum = readBitsLE(frame.data, 0, 8);
    params::vw_pq.pla_1_3d4_checksum = raw_pla_1_3d4_checksum * 1.0f + 0.0f;
    // COUNTER: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_pla_1_3d4_counter = readBitsLE(frame.data, 8, 4);
    params::vw_pq.pla_1_3d4_counter = raw_pla_1_3d4_counter * 1.0f + 0.0f;
    // PL1_Status_EPS: start=12 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_pla_1_3d4_pl1_status_eps = readBitsLE(frame.data, 12, 4);
    params::vw_pq.pla_1_3d4_pl1_status_eps = raw_pla_1_3d4_pl1_status_eps * 1.0f + 0.0f;
    // PL1_ArcAngleReq: start=16 len=15 endian=1 sign=+ factor=0.04375 offset=0 unit="Unit_DegreeOfArc"
    const uint32_t raw_pla_1_3d4_pl1_arcanglereq = readBitsLE(frame.data, 16, 15);
    params::vw_pq.pla_1_3d4_pl1_arcanglereq = raw_pla_1_3d4_pl1_arcanglereq * 0.04375f + 0.0f;
    // PL1_AngleReqSign: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.pla_1_3d4_pl1_anglereqsign = readBitsLE(frame.data, 31, 1) != 0;
    // PL1_Stat_PLA_ESP: start=32 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_pla_1_3d4_pl1_stat_pla_esp = readBitsLE(frame.data, 32, 4);
    params::vw_pq.pla_1_3d4_pl1_stat_pla_esp = raw_pla_1_3d4_pl1_stat_pla_esp * 1.0f + 0.0f;
    // PL1_Bremsmoment: start=40 len=13 endian=1 sign=+ factor=4 offset=0 unit="Unit_NewtoMeter"
    const uint32_t raw_pla_1_3d4_pl1_bremsmoment = readBitsLE(frame.data, 40, 13);
    params::vw_pq.pla_1_3d4_pl1_bremsmoment = raw_pla_1_3d4_pl1_bremsmoment * 4.0f + 0.0f;
    // PL1_void: start=53 len=11 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_pla_1_3d4_pl1_void = readBitsLE(frame.data, 53, 11);
    params::vw_pq.pla_1_3d4_pl1_void = raw_pla_1_3d4_pl1_void * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x3D4(CANMessage &frame) {
    frame.id = 0x3D4;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // CHECKSUM
    int64_t raw_tmp_pla_1_3d4_checksum = (int64_t)llround((params::vw_pq.pla_1_3d4_checksum - 0.0f) / 1.0f);
    uint64_t raw_pla_1_3d4_checksum = raw_tmp_pla_1_3d4_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_pla_1_3d4_checksum;
    if (raw_pla_1_3d4_checksum > 0xFFULL) raw_pla_1_3d4_checksum = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_pla_1_3d4_checksum);
    // COUNTER
    int64_t raw_tmp_pla_1_3d4_counter = (int64_t)llround((params::vw_pq.pla_1_3d4_counter - 0.0f) / 1.0f);
    uint64_t raw_pla_1_3d4_counter = raw_tmp_pla_1_3d4_counter < 0 ? 0ULL : (uint64_t)raw_tmp_pla_1_3d4_counter;
    if (raw_pla_1_3d4_counter > 0xFULL) raw_pla_1_3d4_counter = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_pla_1_3d4_counter);
    // PL1_Status_EPS
    int64_t raw_tmp_pla_1_3d4_pl1_status_eps = (int64_t)llround((params::vw_pq.pla_1_3d4_pl1_status_eps - 0.0f) / 1.0f);
    uint64_t raw_pla_1_3d4_pl1_status_eps = raw_tmp_pla_1_3d4_pl1_status_eps < 0 ? 0ULL : (uint64_t)raw_tmp_pla_1_3d4_pl1_status_eps;
    if (raw_pla_1_3d4_pl1_status_eps > 0xFULL) raw_pla_1_3d4_pl1_status_eps = 0xFULL;
    writeBitsLE(frame.data, 12, 4, (uint32_t)raw_pla_1_3d4_pl1_status_eps);
    // PL1_ArcAngleReq
    int64_t raw_tmp_pla_1_3d4_pl1_arcanglereq = (int64_t)llround((params::vw_pq.pla_1_3d4_pl1_arcanglereq - 0.0f) / 0.04375f);
    uint64_t raw_pla_1_3d4_pl1_arcanglereq = raw_tmp_pla_1_3d4_pl1_arcanglereq < 0 ? 0ULL : (uint64_t)raw_tmp_pla_1_3d4_pl1_arcanglereq;
    if (raw_pla_1_3d4_pl1_arcanglereq > 0x7FFFULL) raw_pla_1_3d4_pl1_arcanglereq = 0x7FFFULL;
    writeBitsLE(frame.data, 16, 15, (uint32_t)raw_pla_1_3d4_pl1_arcanglereq);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.pla_1_3d4_pl1_anglereqsign ? 1U : 0U);
    // PL1_Stat_PLA_ESP
    int64_t raw_tmp_pla_1_3d4_pl1_stat_pla_esp = (int64_t)llround((params::vw_pq.pla_1_3d4_pl1_stat_pla_esp - 0.0f) / 1.0f);
    uint64_t raw_pla_1_3d4_pl1_stat_pla_esp = raw_tmp_pla_1_3d4_pl1_stat_pla_esp < 0 ? 0ULL : (uint64_t)raw_tmp_pla_1_3d4_pl1_stat_pla_esp;
    if (raw_pla_1_3d4_pl1_stat_pla_esp > 0xFULL) raw_pla_1_3d4_pl1_stat_pla_esp = 0xFULL;
    writeBitsLE(frame.data, 32, 4, (uint32_t)raw_pla_1_3d4_pl1_stat_pla_esp);
    // PL1_Bremsmoment
    int64_t raw_tmp_pla_1_3d4_pl1_bremsmoment = (int64_t)llround((params::vw_pq.pla_1_3d4_pl1_bremsmoment - 0.0f) / 4.0f);
    uint64_t raw_pla_1_3d4_pl1_bremsmoment = raw_tmp_pla_1_3d4_pl1_bremsmoment < 0 ? 0ULL : (uint64_t)raw_tmp_pla_1_3d4_pl1_bremsmoment;
    if (raw_pla_1_3d4_pl1_bremsmoment > 0x1FFFULL) raw_pla_1_3d4_pl1_bremsmoment = 0x1FFFULL;
    writeBitsLE(frame.data, 40, 13, (uint32_t)raw_pla_1_3d4_pl1_bremsmoment);
    // PL1_void
    int64_t raw_tmp_pla_1_3d4_pl1_void = (int64_t)llround((params::vw_pq.pla_1_3d4_pl1_void - 0.0f) / 1.0f);
    uint64_t raw_pla_1_3d4_pl1_void = raw_tmp_pla_1_3d4_pl1_void < 0 ? 0ULL : (uint64_t)raw_tmp_pla_1_3d4_pl1_void;
    if (raw_pla_1_3d4_pl1_void > 0x7FFULL) raw_pla_1_3d4_pl1_void = 0x7FFULL;
    writeBitsLE(frame.data, 53, 11, (uint32_t)raw_pla_1_3d4_pl1_void);
}

// Message 0x0D2 (210) - HCA_1
// DLC: 5, TX: XXX
bool decodeVwPq0xD2(const CANMessage &frame) {
    if (frame.id != 0xD2 || frame.len < 5) {
        return false;
    }

    // CHECKSUM: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_hca_1_0d2_checksum = readBitsLE(frame.data, 0, 8);
    params::vw_pq.hca_1_0d2_checksum = raw_hca_1_0d2_checksum * 1.0f + 0.0f;
    // COUNTER: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_hca_1_0d2_counter = readBitsLE(frame.data, 8, 4);
    params::vw_pq.hca_1_0d2_counter = raw_hca_1_0d2_counter * 1.0f + 0.0f;
    // HCA_Status: start=12 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_hca_1_0d2_hca_status = readBitsLE(frame.data, 12, 4);
    params::vw_pq.hca_1_0d2_hca_status = raw_hca_1_0d2_hca_status * 1.0f + 0.0f;
    // LM_Offset: start=16 len=15 endian=1 sign=+ factor=0.03125 offset=0 unit="cNm"
    // DBC comment: centiNewton-meters for ease of calculation without FP math in Panda
    const uint32_t raw_hca_1_0d2_lm_offset = readBitsLE(frame.data, 16, 15);
    params::vw_pq.hca_1_0d2_lm_offset = raw_hca_1_0d2_lm_offset * 0.03125f + 0.0f;
    // LM_OffSign: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.hca_1_0d2_lm_offsign = readBitsLE(frame.data, 31, 1) != 0;
    // Vib_Freq: start=32 len=4 endian=1 sign=+ factor=4 offset=0 unit="Hz"
    // DBC comment: Steering wheel haptic, frequency
    const uint32_t raw_hca_1_0d2_vib_freq = readBitsLE(frame.data, 32, 4);
    params::vw_pq.hca_1_0d2_vib_freq = raw_hca_1_0d2_vib_freq * 4.0f + 0.0f;
    // Vib_Amp: start=36 len=4 endian=1 sign=+ factor=0.5 offset=0 unit="Nm"
    // DBC comment: Steering wheel haptic, amplitude
    const uint32_t raw_hca_1_0d2_vib_amp = readBitsLE(frame.data, 36, 4);
    params::vw_pq.hca_1_0d2_vib_amp = raw_hca_1_0d2_vib_amp * 0.5f + 0.0f;
    return true;
}

void encodeVwPq0xD2(CANMessage &frame) {
    frame.id = 0xD2;
    frame.len = 5;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // CHECKSUM
    int64_t raw_tmp_hca_1_0d2_checksum = (int64_t)llround((params::vw_pq.hca_1_0d2_checksum - 0.0f) / 1.0f);
    uint64_t raw_hca_1_0d2_checksum = raw_tmp_hca_1_0d2_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_hca_1_0d2_checksum;
    if (raw_hca_1_0d2_checksum > 0xFFULL) raw_hca_1_0d2_checksum = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_hca_1_0d2_checksum);
    // COUNTER
    int64_t raw_tmp_hca_1_0d2_counter = (int64_t)llround((params::vw_pq.hca_1_0d2_counter - 0.0f) / 1.0f);
    uint64_t raw_hca_1_0d2_counter = raw_tmp_hca_1_0d2_counter < 0 ? 0ULL : (uint64_t)raw_tmp_hca_1_0d2_counter;
    if (raw_hca_1_0d2_counter > 0xFULL) raw_hca_1_0d2_counter = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_hca_1_0d2_counter);
    // HCA_Status
    int64_t raw_tmp_hca_1_0d2_hca_status = (int64_t)llround((params::vw_pq.hca_1_0d2_hca_status - 0.0f) / 1.0f);
    uint64_t raw_hca_1_0d2_hca_status = raw_tmp_hca_1_0d2_hca_status < 0 ? 0ULL : (uint64_t)raw_tmp_hca_1_0d2_hca_status;
    if (raw_hca_1_0d2_hca_status > 0xFULL) raw_hca_1_0d2_hca_status = 0xFULL;
    writeBitsLE(frame.data, 12, 4, (uint32_t)raw_hca_1_0d2_hca_status);
    // LM_Offset
    int64_t raw_tmp_hca_1_0d2_lm_offset = (int64_t)llround((params::vw_pq.hca_1_0d2_lm_offset - 0.0f) / 0.03125f);
    uint64_t raw_hca_1_0d2_lm_offset = raw_tmp_hca_1_0d2_lm_offset < 0 ? 0ULL : (uint64_t)raw_tmp_hca_1_0d2_lm_offset;
    if (raw_hca_1_0d2_lm_offset > 0x7FFFULL) raw_hca_1_0d2_lm_offset = 0x7FFFULL;
    writeBitsLE(frame.data, 16, 15, (uint32_t)raw_hca_1_0d2_lm_offset);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.hca_1_0d2_lm_offsign ? 1U : 0U);
    // Vib_Freq
    int64_t raw_tmp_hca_1_0d2_vib_freq = (int64_t)llround((params::vw_pq.hca_1_0d2_vib_freq - 0.0f) / 4.0f);
    uint64_t raw_hca_1_0d2_vib_freq = raw_tmp_hca_1_0d2_vib_freq < 0 ? 0ULL : (uint64_t)raw_tmp_hca_1_0d2_vib_freq;
    if (raw_hca_1_0d2_vib_freq > 0xFULL) raw_hca_1_0d2_vib_freq = 0xFULL;
    writeBitsLE(frame.data, 32, 4, (uint32_t)raw_hca_1_0d2_vib_freq);
    // Vib_Amp
    int64_t raw_tmp_hca_1_0d2_vib_amp = (int64_t)llround((params::vw_pq.hca_1_0d2_vib_amp - 0.0f) / 0.5f);
    uint64_t raw_hca_1_0d2_vib_amp = raw_tmp_hca_1_0d2_vib_amp < 0 ? 0ULL : (uint64_t)raw_tmp_hca_1_0d2_vib_amp;
    if (raw_hca_1_0d2_vib_amp > 0xFULL) raw_hca_1_0d2_vib_amp = 0xFULL;
    writeBitsLE(frame.data, 36, 4, (uint32_t)raw_hca_1_0d2_vib_amp);
}

// Message 0x284 (644) - Motor_Bremse
// DLC: 6, TX: XXX
bool decodeVwPq0x284(const CANMessage &frame) {
    if (frame.id != 0x284 || frame.len < 6) {
        return false;
    }

    // MOB_Standby: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_bremse_284_mob_standby = readBitsLE(frame.data, 12, 1) != 0;
    // MOB_Freigabe: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_bremse_284_mob_freigabe = readBitsLE(frame.data, 14, 1) != 0;
    // MOB_Anhaltewunsch: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_bremse_284_mob_anhaltewunsch = readBitsLE(frame.data, 13, 1) != 0;
    // MOB_CHECKSUM: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Checksum MOB
    const uint32_t raw_motor_bremse_284_mob_checksum = readBitsLE(frame.data, 0, 8);
    params::vw_pq.motor_bremse_284_mob_checksum = raw_motor_bremse_284_mob_checksum * 1.0f + 0.0f;
    // MOB_COUNTER: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // DBC comment: Counter MOB
    const uint32_t raw_motor_bremse_284_mob_counter = readBitsLE(frame.data, 8, 4);
    params::vw_pq.motor_bremse_284_mob_counter = raw_motor_bremse_284_mob_counter * 1.0f + 0.0f;
    // TSK_v_Begrenzung_aktiv: start=15 len=1 endian=0 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.motor_bremse_284_tsk_v_begrenzung_aktiv = readBitsLE(frame.data, 15, 1) != 0;
    // TSK_ax_Getriebe_01: start=40 len=8 endian=1 sign=+ factor=0.048 offset=0 unit="m/s2"
    const uint32_t raw_motor_bremse_284_tsk_ax_getriebe_01 = readBitsLE(frame.data, 40, 8);
    params::vw_pq.motor_bremse_284_tsk_ax_getriebe_01 = raw_motor_bremse_284_tsk_ax_getriebe_01 * 0.048f + 0.0f;
    // MOB_Bremsstgr: start=16 len=11 endian=1 sign=+ factor=0.048852 offset=0 unit="Unit_PerCent"
    const uint32_t raw_motor_bremse_284_mob_bremsstgr = readBitsLE(frame.data, 16, 11);
    params::vw_pq.motor_bremse_284_mob_bremsstgr = raw_motor_bremse_284_mob_bremsstgr * 0.048852f + 0.0f;
    // MOB_Bremsmom: start=27 len=13 endian=1 sign=+ factor=4 offset=0 unit="Unit_NewtoMeter"
    const uint32_t raw_motor_bremse_284_mob_bremsmom = readBitsLE(frame.data, 27, 13);
    params::vw_pq.motor_bremse_284_mob_bremsmom = raw_motor_bremse_284_mob_bremsmom * 4.0f + 0.0f;
    return true;
}

void encodeVwPq0x284(CANMessage &frame) {
    frame.id = 0x284;
    frame.len = 6;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 12, 1, params::vw_pq.motor_bremse_284_mob_standby ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.motor_bremse_284_mob_freigabe ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.motor_bremse_284_mob_anhaltewunsch ? 1U : 0U);
    // MOB_CHECKSUM
    int64_t raw_tmp_motor_bremse_284_mob_checksum = (int64_t)llround((params::vw_pq.motor_bremse_284_mob_checksum - 0.0f) / 1.0f);
    uint64_t raw_motor_bremse_284_mob_checksum = raw_tmp_motor_bremse_284_mob_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_motor_bremse_284_mob_checksum;
    if (raw_motor_bremse_284_mob_checksum > 0xFFULL) raw_motor_bremse_284_mob_checksum = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_motor_bremse_284_mob_checksum);
    // MOB_COUNTER
    int64_t raw_tmp_motor_bremse_284_mob_counter = (int64_t)llround((params::vw_pq.motor_bremse_284_mob_counter - 0.0f) / 1.0f);
    uint64_t raw_motor_bremse_284_mob_counter = raw_tmp_motor_bremse_284_mob_counter < 0 ? 0ULL : (uint64_t)raw_tmp_motor_bremse_284_mob_counter;
    if (raw_motor_bremse_284_mob_counter > 0xFULL) raw_motor_bremse_284_mob_counter = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_motor_bremse_284_mob_counter);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.motor_bremse_284_tsk_v_begrenzung_aktiv ? 1U : 0U);
    // TSK_ax_Getriebe_01
    int64_t raw_tmp_motor_bremse_284_tsk_ax_getriebe_01 = (int64_t)llround((params::vw_pq.motor_bremse_284_tsk_ax_getriebe_01 - 0.0f) / 0.048f);
    uint64_t raw_motor_bremse_284_tsk_ax_getriebe_01 = raw_tmp_motor_bremse_284_tsk_ax_getriebe_01 < 0 ? 0ULL : (uint64_t)raw_tmp_motor_bremse_284_tsk_ax_getriebe_01;
    if (raw_motor_bremse_284_tsk_ax_getriebe_01 > 0xFFULL) raw_motor_bremse_284_tsk_ax_getriebe_01 = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_motor_bremse_284_tsk_ax_getriebe_01);
    // MOB_Bremsstgr
    int64_t raw_tmp_motor_bremse_284_mob_bremsstgr = (int64_t)llround((params::vw_pq.motor_bremse_284_mob_bremsstgr - 0.0f) / 0.048852f);
    uint64_t raw_motor_bremse_284_mob_bremsstgr = raw_tmp_motor_bremse_284_mob_bremsstgr < 0 ? 0ULL : (uint64_t)raw_tmp_motor_bremse_284_mob_bremsstgr;
    if (raw_motor_bremse_284_mob_bremsstgr > 0x7FFULL) raw_motor_bremse_284_mob_bremsstgr = 0x7FFULL;
    writeBitsLE(frame.data, 16, 11, (uint32_t)raw_motor_bremse_284_mob_bremsstgr);
    // MOB_Bremsmom
    int64_t raw_tmp_motor_bremse_284_mob_bremsmom = (int64_t)llround((params::vw_pq.motor_bremse_284_mob_bremsmom - 0.0f) / 4.0f);
    uint64_t raw_motor_bremse_284_mob_bremsmom = raw_tmp_motor_bremse_284_mob_bremsmom < 0 ? 0ULL : (uint64_t)raw_tmp_motor_bremse_284_mob_bremsmom;
    if (raw_motor_bremse_284_mob_bremsmom > 0x1FFFULL) raw_motor_bremse_284_mob_bremsmom = 0x1FFFULL;
    writeBitsLE(frame.data, 27, 13, (uint32_t)raw_motor_bremse_284_mob_bremsmom);
}

// Message 0x366 (870) - AWV
// DLC: 8, TX: XXX
bool decodeVwPq0x366(const CANMessage &frame) {
    if (frame.id != 0x366 || frame.len < 8) {
        return false;
    }

    // CHECKSUM: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_awv_366_checksum = readBitsLE(frame.data, 0, 8);
    params::vw_pq.awv_366_checksum = raw_awv_366_checksum * 1.0f + 0.0f;
    // COUNTER: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_awv_366_counter = readBitsLE(frame.data, 8, 4);
    params::vw_pq.awv_366_counter = raw_awv_366_counter * 1.0f + 0.0f;
    // AWV_Text: start=12 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "kein_Text" 1 "FrontAssist_aus" 2 "FrontAssist_startet" 3 "FrontAssist_Warnung" 4 "FrontAssist_Sens_reinig" 5 "FrontAssist_Failure" 6 "FrontAssist_Demo" 7 "Vorhalt" 8 "Bremsung_wird_gerade_durchgefuehrt" 9 "Sensor_not_verfuegbar" 10 "Sensor_reinigen" 11 "Service_notwendig_Failure" 12 "Funktion_vom_Fahrer_deactivated" 13 "Funktion_vom_Fahrer_activated" 14 "FrontAssist_zur_Zeit_not_verfuegbar__rev_Failure"
    const uint32_t raw_awv_366_awv_text = readBitsLE(frame.data, 12, 4);
    params::vw_pq.awv_366_awv_text = raw_awv_366_awv_text * 1.0f + 0.0f;
    // AWV_1_Freigabe: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "nicht_freigegeben" 1 "freigegeben"
    params::vw_pq.awv_366_awv_1_freigabe = readBitsLE(frame.data, 16, 1) != 0;
    // AWV_1_Prefill: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "keine_Prefill_Anf" 1 "Prefill_Anf"
    params::vw_pq.awv_366_awv_1_prefill = readBitsLE(frame.data, 17, 1) != 0;
    // AWV_1_Parameter: start=18 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Defaultparametersatz" 1 "Par_leicht_erh_Empf" 2 "Par_erh_Empf" 3 "Par_hoechster_Empf"
    const uint32_t raw_awv_366_awv_1_parameter = readBitsLE(frame.data, 18, 2);
    params::vw_pq.awv_366_awv_1_parameter = raw_awv_366_awv_1_parameter * 1.0f + 0.0f;
    // AWV_only: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "ACC_und_AWV_verbaut" 1 "AWV_ohne_ACC_verbaut"
    params::vw_pq.awv_366_awv_only = readBitsLE(frame.data, 20, 1) != 0;
    // AWV_CityANB_Auspraegung: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "autom_Bremsung_im_ges_vBereich" 1 "autom_Bremsung_im_def_vBereich"
    params::vw_pq.awv_366_awv_cityanb_auspraegung = readBitsLE(frame.data, 21, 1) != 0;
    // AWV_Halten: start=22 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "keine_Anforderung" 1 "Anforderung_das_Fzg_im_Stillstand_zu_halten"
    params::vw_pq.awv_366_awv_halten = readBitsLE(frame.data, 22, 1) != 0;
    // ANB_Teilbremsung_Freigabe: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Teilbremsung_nicht_freigegeben" 1 "Teilbremsung_freigegeben"
    params::vw_pq.awv_366_anb_teilbremsung_freigabe = readBitsLE(frame.data, 23, 1) != 0;
    // AWV_2_Status: start=24 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Lampe_aus" 1 "Lampe_ein"
    params::vw_pq.awv_366_awv_2_status = readBitsLE(frame.data, 24, 1) != 0;
    // AWV_2_Fehler: start=25 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Lampe_aus" 1 "Lampe_ein"
    params::vw_pq.awv_366_awv_2_fehler = readBitsLE(frame.data, 25, 1) != 0;
    // AWV_2_SU_Warnzeit: start=26 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "frueh" 1 "normal" 2 "spaet" 3 "adaptiv"
    const uint32_t raw_awv_366_awv_2_su_warnzeit = readBitsLE(frame.data, 26, 2);
    params::vw_pq.awv_366_awv_2_su_warnzeit = raw_awv_366_awv_2_su_warnzeit * 1.0f + 0.0f;
    // AWV_2_SU_Bremsruck: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Bremsruck_deaktiviert" 1 "Bremsruck_aktiviert"
    params::vw_pq.awv_366_awv_2_su_bremsruck = readBitsLE(frame.data, 28, 1) != 0;
    // AWV_2_SU_Gong: start=29 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Gong_deaktiviert" 1 "Gong_aktiviert"
    params::vw_pq.awv_366_awv_2_su_gong = readBitsLE(frame.data, 29, 1) != 0;
    // AWV_2_SU_Lampe: start=30 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Lampe_deaktiviert" 1 "Lampe_aktiviert"
    params::vw_pq.awv_366_awv_2_su_lampe = readBitsLE(frame.data, 30, 1) != 0;
    // AWV_2_Umfeldwarn: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "keine_Warnung" 1 "Warnung"
    params::vw_pq.awv_366_awv_2_umfeldwarn = readBitsLE(frame.data, 31, 1) != 0;
    // AWV_2_Freigabe: start=32 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "keine_Ruckfreigabe" 1 "Ruckfreigabe"
    params::vw_pq.awv_366_awv_2_freigabe = readBitsLE(frame.data, 32, 1) != 0;
    // AWV_2_Ruckprofil: start=33 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "kein_Ruck" 1 "Ruckprofil_1" 2 "Ruckprofil_2" 3 "Ruckprofil_3" 4 "Ruckprofil_4" 5 "Ruckprofil_5" 6 "not_erlaubt" 7 "not_erlaubt"
    const uint32_t raw_awv_366_awv_2_ruckprofil = readBitsLE(frame.data, 33, 3);
    params::vw_pq.awv_366_awv_2_ruckprofil = raw_awv_366_awv_2_ruckprofil * 1.0f + 0.0f;
    // AWV_2_Warnton: start=36 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Aus" 1 "Ein"
    params::vw_pq.awv_366_awv_2_warnton = readBitsLE(frame.data, 36, 1) != 0;
    // AWV_2_Warnsymbol: start=37 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Aus" 1 "Ein"
    params::vw_pq.awv_366_awv_2_warnsymbol = readBitsLE(frame.data, 37, 1) != 0;
    // AWV_Infoton: start=38 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Aus" 1 "Ein"
    params::vw_pq.awv_366_awv_infoton = readBitsLE(frame.data, 38, 1) != 0;
    // AWV_2_Gurtstraffer: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Gurt_not_straffen" 1 "Gurt_straffen"
    params::vw_pq.awv_366_awv_2_gurtstraffer = readBitsLE(frame.data, 39, 1) != 0;
    // AWV_Konfiguration_Menueanf: start=40 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Menue_deaktivieren" 1 "Menue_aktivieren"
    params::vw_pq.awv_366_awv_konfiguration_menueanf = readBitsLE(frame.data, 40, 1) != 0;
    // AWV_Konfiguration_Vorw_Menueanf: start=41 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Menue_deaktivieren" 1 "Menue_aktivieren"
    params::vw_pq.awv_366_awv_konfiguration_vorw_menueanf = readBitsLE(frame.data, 41, 1) != 0;
    // AWV_Konfiguration_Status: start=42 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "AWV_inaktiv" 1 "AWV_aktiv"
    params::vw_pq.awv_366_awv_konfiguration_status = readBitsLE(frame.data, 42, 1) != 0;
    // AWV_Konfiguration_Vorw_Status: start=43 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "AWV_Vorwarnung_inaktiv" 1 "AWV_Vorwarnung_aktiv"
    params::vw_pq.awv_366_awv_konfiguration_vorw_status = readBitsLE(frame.data, 43, 1) != 0;
    // AWV_2_Abstandswarnung: start=51 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "kein_Warnhinweis" 1 "Warnhinweis"
    params::vw_pq.awv_366_awv_2_abstandswarnung = readBitsLE(frame.data, 51, 1) != 0;
    // ANB_Zielbremsung_Freigabe: start=52 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "Zielbremsung_nicht_freigegeben" 1 "Zielbremsung_freigegeben"
    params::vw_pq.awv_366_anb_zielbremsung_freigabe = readBitsLE(frame.data, 52, 1) != 0;
    // ANB_CM_Anforderung: start=53 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    // Value table: 0 "keine_Anforderung" 1 "Anforderung_aktiv"
    params::vw_pq.awv_366_anb_cm_anforderung = readBitsLE(frame.data, 53, 1) != 0;
    // ANB_Ziel_Teilbrems_Verz_Anf: start=54 len=10 endian=1 sign=+ factor=0.024 offset=-20.016 unit="Unit_MeterPerSeconSquar"
    const uint32_t raw_awv_366_anb_ziel_teilbrems_verz_anf = readBitsLE(frame.data, 54, 10);
    params::vw_pq.awv_366_anb_ziel_teilbrems_verz_anf = raw_awv_366_anb_ziel_teilbrems_verz_anf * 0.024f + -20.016f;
    return true;
}

void encodeVwPq0x366(CANMessage &frame) {
    frame.id = 0x366;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // CHECKSUM
    int64_t raw_tmp_awv_366_checksum = (int64_t)llround((params::vw_pq.awv_366_checksum - 0.0f) / 1.0f);
    uint64_t raw_awv_366_checksum = raw_tmp_awv_366_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_awv_366_checksum;
    if (raw_awv_366_checksum > 0xFFULL) raw_awv_366_checksum = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_awv_366_checksum);
    // COUNTER
    int64_t raw_tmp_awv_366_counter = (int64_t)llround((params::vw_pq.awv_366_counter - 0.0f) / 1.0f);
    uint64_t raw_awv_366_counter = raw_tmp_awv_366_counter < 0 ? 0ULL : (uint64_t)raw_tmp_awv_366_counter;
    if (raw_awv_366_counter > 0xFULL) raw_awv_366_counter = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_awv_366_counter);
    // AWV_Text
    int64_t raw_tmp_awv_366_awv_text = (int64_t)llround((params::vw_pq.awv_366_awv_text - 0.0f) / 1.0f);
    uint64_t raw_awv_366_awv_text = raw_tmp_awv_366_awv_text < 0 ? 0ULL : (uint64_t)raw_tmp_awv_366_awv_text;
    if (raw_awv_366_awv_text > 0xFULL) raw_awv_366_awv_text = 0xFULL;
    writeBitsLE(frame.data, 12, 4, (uint32_t)raw_awv_366_awv_text);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.awv_366_awv_1_freigabe ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.awv_366_awv_1_prefill ? 1U : 0U);
    // AWV_1_Parameter
    int64_t raw_tmp_awv_366_awv_1_parameter = (int64_t)llround((params::vw_pq.awv_366_awv_1_parameter - 0.0f) / 1.0f);
    uint64_t raw_awv_366_awv_1_parameter = raw_tmp_awv_366_awv_1_parameter < 0 ? 0ULL : (uint64_t)raw_tmp_awv_366_awv_1_parameter;
    if (raw_awv_366_awv_1_parameter > 0x3ULL) raw_awv_366_awv_1_parameter = 0x3ULL;
    writeBitsLE(frame.data, 18, 2, (uint32_t)raw_awv_366_awv_1_parameter);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.awv_366_awv_only ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.awv_366_awv_cityanb_auspraegung ? 1U : 0U);
    writeBitsLE(frame.data, 22, 1, params::vw_pq.awv_366_awv_halten ? 1U : 0U);
    writeBitsLE(frame.data, 23, 1, params::vw_pq.awv_366_anb_teilbremsung_freigabe ? 1U : 0U);
    writeBitsLE(frame.data, 24, 1, params::vw_pq.awv_366_awv_2_status ? 1U : 0U);
    writeBitsLE(frame.data, 25, 1, params::vw_pq.awv_366_awv_2_fehler ? 1U : 0U);
    // AWV_2_SU_Warnzeit
    int64_t raw_tmp_awv_366_awv_2_su_warnzeit = (int64_t)llround((params::vw_pq.awv_366_awv_2_su_warnzeit - 0.0f) / 1.0f);
    uint64_t raw_awv_366_awv_2_su_warnzeit = raw_tmp_awv_366_awv_2_su_warnzeit < 0 ? 0ULL : (uint64_t)raw_tmp_awv_366_awv_2_su_warnzeit;
    if (raw_awv_366_awv_2_su_warnzeit > 0x3ULL) raw_awv_366_awv_2_su_warnzeit = 0x3ULL;
    writeBitsLE(frame.data, 26, 2, (uint32_t)raw_awv_366_awv_2_su_warnzeit);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.awv_366_awv_2_su_bremsruck ? 1U : 0U);
    writeBitsLE(frame.data, 29, 1, params::vw_pq.awv_366_awv_2_su_gong ? 1U : 0U);
    writeBitsLE(frame.data, 30, 1, params::vw_pq.awv_366_awv_2_su_lampe ? 1U : 0U);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.awv_366_awv_2_umfeldwarn ? 1U : 0U);
    writeBitsLE(frame.data, 32, 1, params::vw_pq.awv_366_awv_2_freigabe ? 1U : 0U);
    // AWV_2_Ruckprofil
    int64_t raw_tmp_awv_366_awv_2_ruckprofil = (int64_t)llround((params::vw_pq.awv_366_awv_2_ruckprofil - 0.0f) / 1.0f);
    uint64_t raw_awv_366_awv_2_ruckprofil = raw_tmp_awv_366_awv_2_ruckprofil < 0 ? 0ULL : (uint64_t)raw_tmp_awv_366_awv_2_ruckprofil;
    if (raw_awv_366_awv_2_ruckprofil > 0x7ULL) raw_awv_366_awv_2_ruckprofil = 0x7ULL;
    writeBitsLE(frame.data, 33, 3, (uint32_t)raw_awv_366_awv_2_ruckprofil);
    writeBitsLE(frame.data, 36, 1, params::vw_pq.awv_366_awv_2_warnton ? 1U : 0U);
    writeBitsLE(frame.data, 37, 1, params::vw_pq.awv_366_awv_2_warnsymbol ? 1U : 0U);
    writeBitsLE(frame.data, 38, 1, params::vw_pq.awv_366_awv_infoton ? 1U : 0U);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.awv_366_awv_2_gurtstraffer ? 1U : 0U);
    writeBitsLE(frame.data, 40, 1, params::vw_pq.awv_366_awv_konfiguration_menueanf ? 1U : 0U);
    writeBitsLE(frame.data, 41, 1, params::vw_pq.awv_366_awv_konfiguration_vorw_menueanf ? 1U : 0U);
    writeBitsLE(frame.data, 42, 1, params::vw_pq.awv_366_awv_konfiguration_status ? 1U : 0U);
    writeBitsLE(frame.data, 43, 1, params::vw_pq.awv_366_awv_konfiguration_vorw_status ? 1U : 0U);
    writeBitsLE(frame.data, 51, 1, params::vw_pq.awv_366_awv_2_abstandswarnung ? 1U : 0U);
    writeBitsLE(frame.data, 52, 1, params::vw_pq.awv_366_anb_zielbremsung_freigabe ? 1U : 0U);
    writeBitsLE(frame.data, 53, 1, params::vw_pq.awv_366_anb_cm_anforderung ? 1U : 0U);
    // ANB_Ziel_Teilbrems_Verz_Anf
    int64_t raw_tmp_awv_366_anb_ziel_teilbrems_verz_anf = (int64_t)llround((params::vw_pq.awv_366_anb_ziel_teilbrems_verz_anf - -20.016f) / 0.024f);
    uint64_t raw_awv_366_anb_ziel_teilbrems_verz_anf = raw_tmp_awv_366_anb_ziel_teilbrems_verz_anf < 0 ? 0ULL : (uint64_t)raw_tmp_awv_366_anb_ziel_teilbrems_verz_anf;
    if (raw_awv_366_anb_ziel_teilbrems_verz_anf > 0x3FFULL) raw_awv_366_anb_ziel_teilbrems_verz_anf = 0x3FFULL;
    writeBitsLE(frame.data, 54, 10, (uint32_t)raw_awv_366_anb_ziel_teilbrems_verz_anf);
}

// Message 0x5BE (1470) - LDW_Status
// DLC: 8, TX: XXX
bool decodeVwPq0x5BE(const CANMessage &frame) {
    if (frame.id != 0x5BE || frame.len < 8) {
        return false;
    }

    // LDW_Lernmodus_rechts: start=0 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ldw_status_5be_ldw_lernmodus_rechts = readBitsLE(frame.data, 0, 2);
    params::vw_pq.ldw_status_5be_ldw_lernmodus_rechts = raw_ldw_status_5be_ldw_lernmodus_rechts * 1.0f + 0.0f;
    // LDW_Lernmodus_links: start=2 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ldw_status_5be_ldw_lernmodus_links = readBitsLE(frame.data, 2, 2);
    params::vw_pq.ldw_status_5be_ldw_lernmodus_links = raw_ldw_status_5be_ldw_lernmodus_links * 1.0f + 0.0f;
    // LDW_Lernmodus: start=9 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ldw_status_5be_ldw_lernmodus = readBitsLE(frame.data, 9, 3);
    params::vw_pq.ldw_status_5be_ldw_lernmodus = raw_ldw_status_5be_ldw_lernmodus * 1.0f + 0.0f;
    // LDW_Textbits: start=12 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ldw_status_5be_ldw_textbits = readBitsLE(frame.data, 12, 4);
    params::vw_pq.ldw_status_5be_ldw_textbits = raw_ldw_status_5be_ldw_textbits * 1.0f + 0.0f;
    // LDW_Gong: start=16 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ldw_status_5be_ldw_gong = readBitsLE(frame.data, 16, 2);
    params::vw_pq.ldw_status_5be_ldw_gong = raw_ldw_status_5be_ldw_gong * 1.0f + 0.0f;
    // LDW_Kameratyp: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.ldw_status_5be_ldw_kameratyp = readBitsLE(frame.data, 18, 1) != 0;
    // LDW_Lampe_gelb: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.ldw_status_5be_ldw_lampe_gelb = readBitsLE(frame.data, 19, 1) != 0;
    // LDW_Lampe_gruen: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.ldw_status_5be_ldw_lampe_gruen = readBitsLE(frame.data, 20, 1) != 0;
    // LDW_SW_Warnung_links: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.ldw_status_5be_ldw_sw_warnung_links = readBitsLE(frame.data, 21, 1) != 0;
    // LDW_SW_Warnung_rechts: start=22 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.ldw_status_5be_ldw_sw_warnung_rechts = readBitsLE(frame.data, 22, 1) != 0;
    // LDW_KD_Fehler: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.ldw_status_5be_ldw_kd_fehler = readBitsLE(frame.data, 23, 1) != 0;
    // LDW_DLC: start=24 len=8 endian=1 sign=+ factor=0.01 offset=-1.25 unit=""
    const uint32_t raw_ldw_status_5be_ldw_dlc = readBitsLE(frame.data, 24, 8);
    params::vw_pq.ldw_status_5be_ldw_dlc = raw_ldw_status_5be_ldw_dlc * 0.01f + -1.25f;
    // LDW_TLC: start=32 len=5 endian=1 sign=+ factor=0.1 offset=0 unit=""
    const uint32_t raw_ldw_status_5be_ldw_tlc = readBitsLE(frame.data, 32, 5);
    params::vw_pq.ldw_status_5be_ldw_tlc = raw_ldw_status_5be_ldw_tlc * 0.1f + 0.0f;
    // LDW_Seite_DLCTLC: start=37 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.ldw_status_5be_ldw_seite_dlctlc = readBitsLE(frame.data, 37, 1) != 0;
    // LDW_Frueh_Spaet: start=38 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ldw_status_5be_ldw_frueh_spaet = readBitsLE(frame.data, 38, 2);
    params::vw_pq.ldw_status_5be_ldw_frueh_spaet = raw_ldw_status_5be_ldw_frueh_spaet * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x5BE(CANMessage &frame) {
    frame.id = 0x5BE;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // LDW_Lernmodus_rechts
    int64_t raw_tmp_ldw_status_5be_ldw_lernmodus_rechts = (int64_t)llround((params::vw_pq.ldw_status_5be_ldw_lernmodus_rechts - 0.0f) / 1.0f);
    uint64_t raw_ldw_status_5be_ldw_lernmodus_rechts = raw_tmp_ldw_status_5be_ldw_lernmodus_rechts < 0 ? 0ULL : (uint64_t)raw_tmp_ldw_status_5be_ldw_lernmodus_rechts;
    if (raw_ldw_status_5be_ldw_lernmodus_rechts > 0x3ULL) raw_ldw_status_5be_ldw_lernmodus_rechts = 0x3ULL;
    writeBitsLE(frame.data, 0, 2, (uint32_t)raw_ldw_status_5be_ldw_lernmodus_rechts);
    // LDW_Lernmodus_links
    int64_t raw_tmp_ldw_status_5be_ldw_lernmodus_links = (int64_t)llround((params::vw_pq.ldw_status_5be_ldw_lernmodus_links - 0.0f) / 1.0f);
    uint64_t raw_ldw_status_5be_ldw_lernmodus_links = raw_tmp_ldw_status_5be_ldw_lernmodus_links < 0 ? 0ULL : (uint64_t)raw_tmp_ldw_status_5be_ldw_lernmodus_links;
    if (raw_ldw_status_5be_ldw_lernmodus_links > 0x3ULL) raw_ldw_status_5be_ldw_lernmodus_links = 0x3ULL;
    writeBitsLE(frame.data, 2, 2, (uint32_t)raw_ldw_status_5be_ldw_lernmodus_links);
    // LDW_Lernmodus
    int64_t raw_tmp_ldw_status_5be_ldw_lernmodus = (int64_t)llround((params::vw_pq.ldw_status_5be_ldw_lernmodus - 0.0f) / 1.0f);
    uint64_t raw_ldw_status_5be_ldw_lernmodus = raw_tmp_ldw_status_5be_ldw_lernmodus < 0 ? 0ULL : (uint64_t)raw_tmp_ldw_status_5be_ldw_lernmodus;
    if (raw_ldw_status_5be_ldw_lernmodus > 0x7ULL) raw_ldw_status_5be_ldw_lernmodus = 0x7ULL;
    writeBitsLE(frame.data, 9, 3, (uint32_t)raw_ldw_status_5be_ldw_lernmodus);
    // LDW_Textbits
    int64_t raw_tmp_ldw_status_5be_ldw_textbits = (int64_t)llround((params::vw_pq.ldw_status_5be_ldw_textbits - 0.0f) / 1.0f);
    uint64_t raw_ldw_status_5be_ldw_textbits = raw_tmp_ldw_status_5be_ldw_textbits < 0 ? 0ULL : (uint64_t)raw_tmp_ldw_status_5be_ldw_textbits;
    if (raw_ldw_status_5be_ldw_textbits > 0xFULL) raw_ldw_status_5be_ldw_textbits = 0xFULL;
    writeBitsLE(frame.data, 12, 4, (uint32_t)raw_ldw_status_5be_ldw_textbits);
    // LDW_Gong
    int64_t raw_tmp_ldw_status_5be_ldw_gong = (int64_t)llround((params::vw_pq.ldw_status_5be_ldw_gong - 0.0f) / 1.0f);
    uint64_t raw_ldw_status_5be_ldw_gong = raw_tmp_ldw_status_5be_ldw_gong < 0 ? 0ULL : (uint64_t)raw_tmp_ldw_status_5be_ldw_gong;
    if (raw_ldw_status_5be_ldw_gong > 0x3ULL) raw_ldw_status_5be_ldw_gong = 0x3ULL;
    writeBitsLE(frame.data, 16, 2, (uint32_t)raw_ldw_status_5be_ldw_gong);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.ldw_status_5be_ldw_kameratyp ? 1U : 0U);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.ldw_status_5be_ldw_lampe_gelb ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.ldw_status_5be_ldw_lampe_gruen ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.ldw_status_5be_ldw_sw_warnung_links ? 1U : 0U);
    writeBitsLE(frame.data, 22, 1, params::vw_pq.ldw_status_5be_ldw_sw_warnung_rechts ? 1U : 0U);
    writeBitsLE(frame.data, 23, 1, params::vw_pq.ldw_status_5be_ldw_kd_fehler ? 1U : 0U);
    // LDW_DLC
    int64_t raw_tmp_ldw_status_5be_ldw_dlc = (int64_t)llround((params::vw_pq.ldw_status_5be_ldw_dlc - -1.25f) / 0.01f);
    uint64_t raw_ldw_status_5be_ldw_dlc = raw_tmp_ldw_status_5be_ldw_dlc < 0 ? 0ULL : (uint64_t)raw_tmp_ldw_status_5be_ldw_dlc;
    if (raw_ldw_status_5be_ldw_dlc > 0xFFULL) raw_ldw_status_5be_ldw_dlc = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_ldw_status_5be_ldw_dlc);
    // LDW_TLC
    int64_t raw_tmp_ldw_status_5be_ldw_tlc = (int64_t)llround((params::vw_pq.ldw_status_5be_ldw_tlc - 0.0f) / 0.1f);
    uint64_t raw_ldw_status_5be_ldw_tlc = raw_tmp_ldw_status_5be_ldw_tlc < 0 ? 0ULL : (uint64_t)raw_tmp_ldw_status_5be_ldw_tlc;
    if (raw_ldw_status_5be_ldw_tlc > 0x1FULL) raw_ldw_status_5be_ldw_tlc = 0x1FULL;
    writeBitsLE(frame.data, 32, 5, (uint32_t)raw_ldw_status_5be_ldw_tlc);
    writeBitsLE(frame.data, 37, 1, params::vw_pq.ldw_status_5be_ldw_seite_dlctlc ? 1U : 0U);
    // LDW_Frueh_Spaet
    int64_t raw_tmp_ldw_status_5be_ldw_frueh_spaet = (int64_t)llround((params::vw_pq.ldw_status_5be_ldw_frueh_spaet - 0.0f) / 1.0f);
    uint64_t raw_ldw_status_5be_ldw_frueh_spaet = raw_tmp_ldw_status_5be_ldw_frueh_spaet < 0 ? 0ULL : (uint64_t)raw_tmp_ldw_status_5be_ldw_frueh_spaet;
    if (raw_ldw_status_5be_ldw_frueh_spaet > 0x3ULL) raw_ldw_status_5be_ldw_frueh_spaet = 0x3ULL;
    writeBitsLE(frame.data, 38, 2, (uint32_t)raw_ldw_status_5be_ldw_frueh_spaet);
}

// Message 0x1AC (428) - Bremse_8
// DLC: 8, TX: XXX
bool decodeVwPq0x1AC(const CANMessage &frame) {
    if (frame.id != 0x1AC || frame.len < 8) {
        return false;
    }

    // BR8_Checksumme: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_8_1ac_br8_checksumme = readBitsLE(frame.data, 0, 8);
    params::vw_pq.bremse_8_1ac_br8_checksumme = raw_bremse_8_1ac_br8_checksumme * 1.0f + 0.0f;
    // BR8_Zaehler: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_8_1ac_br8_zaehler = readBitsLE(frame.data, 8, 4);
    params::vw_pq.bremse_8_1ac_br8_zaehler = raw_bremse_8_1ac_br8_zaehler * 1.0f + 0.0f;
    // BR8_Sta_ACC_Anf: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_sta_acc_anf = readBitsLE(frame.data, 12, 1) != 0;
    // BR8_Verz_EPB_akt: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_verz_epb_akt = readBitsLE(frame.data, 13, 1) != 0;
    // BR8_Sta_Br_temp: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_sta_br_temp = readBitsLE(frame.data, 14, 1) != 0;
    // BR8_Sta_Br_Druck: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_sta_br_druck = readBitsLE(frame.data, 15, 1) != 0;
    // BR8_TolAbgl_HL: start=16 len=8 endian=1 sign=+ factor=0.048828125 offset=-6.201171875 unit="Unit_PerCent"
    const uint32_t raw_bremse_8_1ac_br8_tolabgl_hl = readBitsLE(frame.data, 16, 8);
    params::vw_pq.bremse_8_1ac_br8_tolabgl_hl = raw_bremse_8_1ac_br8_tolabgl_hl * 0.048828125f + -6.201171875f;
    // BR8_TolAbgl_HR: start=24 len=8 endian=1 sign=+ factor=0.048828125 offset=-6.201171875 unit="Unit_PerCent"
    const uint32_t raw_bremse_8_1ac_br8_tolabgl_hr = readBitsLE(frame.data, 24, 8);
    params::vw_pq.bremse_8_1ac_br8_tolabgl_hr = raw_bremse_8_1ac_br8_tolabgl_hr * 0.048828125f + -6.201171875f;
    // BR8_Istbeschl: start=32 len=9 endian=1 sign=+ factor=0.02 offset=-7.22 unit="Unit_MeterPerSeconSquar"
    const uint32_t raw_bremse_8_1ac_br8_istbeschl = readBitsLE(frame.data, 32, 9);
    params::vw_pq.bremse_8_1ac_br8_istbeschl = raw_bremse_8_1ac_br8_istbeschl * 0.02f + -7.22f;
    // BR8_Sta_HW_BLS: start=41 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_sta_hw_bls = readBitsLE(frame.data, 41, 1) != 0;
    // BR8_QB_LBeschl: start=42 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_qb_lbeschl = readBitsLE(frame.data, 42, 1) != 0;
    // BR8_ESC_Mode: start=43 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_8_1ac_br8_esc_mode = readBitsLE(frame.data, 43, 2);
    params::vw_pq.bremse_8_1ac_br8_esc_mode = raw_bremse_8_1ac_br8_esc_mode * 1.0f + 0.0f;
    // BR8_aktBrSyst: start=45 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_aktbrsyst = readBitsLE(frame.data, 45, 1) != 0;
    // BR8_Fa_bremst: start=46 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_fa_bremst = readBitsLE(frame.data, 46, 1) != 0;
    // BR8_StaBrSyst: start=47 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_stabrsyst = readBitsLE(frame.data, 47, 1) != 0;
    // BR8_Laengsbeschl: start=48 len=10 endian=1 sign=+ factor=0.03125 offset=-16 unit="Unit_MeterPerSeconSquar"
    const uint32_t raw_bremse_8_1ac_br8_laengsbeschl = readBitsLE(frame.data, 48, 10);
    params::vw_pq.bremse_8_1ac_br8_laengsbeschl = raw_bremse_8_1ac_br8_laengsbeschl * 0.03125f + -16.0f;
    // BR8_Sta_ADR_BR: start=58 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_sta_adr_br = readBitsLE(frame.data, 58, 1) != 0;
    // BR8_Quattro: start=59 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_quattro = readBitsLE(frame.data, 59, 1) != 0;
    // BR8_Sta_VerzReg: start=60 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_sta_verzreg = readBitsLE(frame.data, 60, 1) != 0;
    // BR8_Sta_BLS: start=61 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_sta_bls = readBitsLE(frame.data, 61, 1) != 0;
    // BR8_Verz_EPB: start=62 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_verz_epb = readBitsLE(frame.data, 62, 1) != 0;
    // BR8_Check_EPB: start=63 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_8_1ac_br8_check_epb = readBitsLE(frame.data, 63, 1) != 0;
    return true;
}

void encodeVwPq0x1AC(CANMessage &frame) {
    frame.id = 0x1AC;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // BR8_Checksumme
    int64_t raw_tmp_bremse_8_1ac_br8_checksumme = (int64_t)llround((params::vw_pq.bremse_8_1ac_br8_checksumme - 0.0f) / 1.0f);
    uint64_t raw_bremse_8_1ac_br8_checksumme = raw_tmp_bremse_8_1ac_br8_checksumme < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_8_1ac_br8_checksumme;
    if (raw_bremse_8_1ac_br8_checksumme > 0xFFULL) raw_bremse_8_1ac_br8_checksumme = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_bremse_8_1ac_br8_checksumme);
    // BR8_Zaehler
    int64_t raw_tmp_bremse_8_1ac_br8_zaehler = (int64_t)llround((params::vw_pq.bremse_8_1ac_br8_zaehler - 0.0f) / 1.0f);
    uint64_t raw_bremse_8_1ac_br8_zaehler = raw_tmp_bremse_8_1ac_br8_zaehler < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_8_1ac_br8_zaehler;
    if (raw_bremse_8_1ac_br8_zaehler > 0xFULL) raw_bremse_8_1ac_br8_zaehler = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_bremse_8_1ac_br8_zaehler);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.bremse_8_1ac_br8_sta_acc_anf ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.bremse_8_1ac_br8_verz_epb_akt ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.bremse_8_1ac_br8_sta_br_temp ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.bremse_8_1ac_br8_sta_br_druck ? 1U : 0U);
    // BR8_TolAbgl_HL
    int64_t raw_tmp_bremse_8_1ac_br8_tolabgl_hl = (int64_t)llround((params::vw_pq.bremse_8_1ac_br8_tolabgl_hl - -6.201171875f) / 0.048828125f);
    uint64_t raw_bremse_8_1ac_br8_tolabgl_hl = raw_tmp_bremse_8_1ac_br8_tolabgl_hl < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_8_1ac_br8_tolabgl_hl;
    if (raw_bremse_8_1ac_br8_tolabgl_hl > 0xFFULL) raw_bremse_8_1ac_br8_tolabgl_hl = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_bremse_8_1ac_br8_tolabgl_hl);
    // BR8_TolAbgl_HR
    int64_t raw_tmp_bremse_8_1ac_br8_tolabgl_hr = (int64_t)llround((params::vw_pq.bremse_8_1ac_br8_tolabgl_hr - -6.201171875f) / 0.048828125f);
    uint64_t raw_bremse_8_1ac_br8_tolabgl_hr = raw_tmp_bremse_8_1ac_br8_tolabgl_hr < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_8_1ac_br8_tolabgl_hr;
    if (raw_bremse_8_1ac_br8_tolabgl_hr > 0xFFULL) raw_bremse_8_1ac_br8_tolabgl_hr = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_bremse_8_1ac_br8_tolabgl_hr);
    // BR8_Istbeschl
    int64_t raw_tmp_bremse_8_1ac_br8_istbeschl = (int64_t)llround((params::vw_pq.bremse_8_1ac_br8_istbeschl - -7.22f) / 0.02f);
    uint64_t raw_bremse_8_1ac_br8_istbeschl = raw_tmp_bremse_8_1ac_br8_istbeschl < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_8_1ac_br8_istbeschl;
    if (raw_bremse_8_1ac_br8_istbeschl > 0x1FFULL) raw_bremse_8_1ac_br8_istbeschl = 0x1FFULL;
    writeBitsLE(frame.data, 32, 9, (uint32_t)raw_bremse_8_1ac_br8_istbeschl);
    writeBitsLE(frame.data, 41, 1, params::vw_pq.bremse_8_1ac_br8_sta_hw_bls ? 1U : 0U);
    writeBitsLE(frame.data, 42, 1, params::vw_pq.bremse_8_1ac_br8_qb_lbeschl ? 1U : 0U);
    // BR8_ESC_Mode
    int64_t raw_tmp_bremse_8_1ac_br8_esc_mode = (int64_t)llround((params::vw_pq.bremse_8_1ac_br8_esc_mode - 0.0f) / 1.0f);
    uint64_t raw_bremse_8_1ac_br8_esc_mode = raw_tmp_bremse_8_1ac_br8_esc_mode < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_8_1ac_br8_esc_mode;
    if (raw_bremse_8_1ac_br8_esc_mode > 0x3ULL) raw_bremse_8_1ac_br8_esc_mode = 0x3ULL;
    writeBitsLE(frame.data, 43, 2, (uint32_t)raw_bremse_8_1ac_br8_esc_mode);
    writeBitsLE(frame.data, 45, 1, params::vw_pq.bremse_8_1ac_br8_aktbrsyst ? 1U : 0U);
    writeBitsLE(frame.data, 46, 1, params::vw_pq.bremse_8_1ac_br8_fa_bremst ? 1U : 0U);
    writeBitsLE(frame.data, 47, 1, params::vw_pq.bremse_8_1ac_br8_stabrsyst ? 1U : 0U);
    // BR8_Laengsbeschl
    int64_t raw_tmp_bremse_8_1ac_br8_laengsbeschl = (int64_t)llround((params::vw_pq.bremse_8_1ac_br8_laengsbeschl - -16.0f) / 0.03125f);
    uint64_t raw_bremse_8_1ac_br8_laengsbeschl = raw_tmp_bremse_8_1ac_br8_laengsbeschl < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_8_1ac_br8_laengsbeschl;
    if (raw_bremse_8_1ac_br8_laengsbeschl > 0x3FFULL) raw_bremse_8_1ac_br8_laengsbeschl = 0x3FFULL;
    writeBitsLE(frame.data, 48, 10, (uint32_t)raw_bremse_8_1ac_br8_laengsbeschl);
    writeBitsLE(frame.data, 58, 1, params::vw_pq.bremse_8_1ac_br8_sta_adr_br ? 1U : 0U);
    writeBitsLE(frame.data, 59, 1, params::vw_pq.bremse_8_1ac_br8_quattro ? 1U : 0U);
    writeBitsLE(frame.data, 60, 1, params::vw_pq.bremse_8_1ac_br8_sta_verzreg ? 1U : 0U);
    writeBitsLE(frame.data, 61, 1, params::vw_pq.bremse_8_1ac_br8_sta_bls ? 1U : 0U);
    writeBitsLE(frame.data, 62, 1, params::vw_pq.bremse_8_1ac_br8_verz_epb ? 1U : 0U);
    writeBitsLE(frame.data, 63, 1, params::vw_pq.bremse_8_1ac_br8_check_epb ? 1U : 0U);
}

// Message 0x3A0 (928) - Bremse_10
// DLC: 8, TX: XXX
bool decodeVwPq0x3A0(const CANMessage &frame) {
    if (frame.id != 0x3A0 || frame.len < 8) {
        return false;
    }

    // B10_Checksumme: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_10_3a0_b10_checksumme = readBitsLE(frame.data, 0, 8);
    params::vw_pq.bremse_10_3a0_b10_checksumme = raw_bremse_10_3a0_b10_checksumme * 1.0f + 0.0f;
    // B10_Zaehler: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_10_3a0_b10_zaehler = readBitsLE(frame.data, 8, 4);
    params::vw_pq.bremse_10_3a0_b10_zaehler = raw_bremse_10_3a0_b10_zaehler * 1.0f + 0.0f;
    // B10_QB_Wegimp_VL: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_10_3a0_b10_qb_wegimp_vl = readBitsLE(frame.data, 12, 1) != 0;
    // B10_QB_Wegimp_VR: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_10_3a0_b10_qb_wegimp_vr = readBitsLE(frame.data, 13, 1) != 0;
    // B10_QB_Wegimp_HL: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_10_3a0_b10_qb_wegimp_hl = readBitsLE(frame.data, 14, 1) != 0;
    // B10_QB_Wegimp_HR: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_10_3a0_b10_qb_wegimp_hr = readBitsLE(frame.data, 15, 1) != 0;
    // B10_Wegimp_VL: start=16 len=10 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_10_3a0_b10_wegimp_vl = readBitsLE(frame.data, 16, 10);
    params::vw_pq.bremse_10_3a0_b10_wegimp_vl = raw_bremse_10_3a0_b10_wegimp_vl * 1.0f + 0.0f;
    // B10_Wegimp_VR: start=26 len=10 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_10_3a0_b10_wegimp_vr = readBitsLE(frame.data, 26, 10);
    params::vw_pq.bremse_10_3a0_b10_wegimp_vr = raw_bremse_10_3a0_b10_wegimp_vr * 1.0f + 0.0f;
    // B10_Wegimp_HL: start=36 len=10 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_10_3a0_b10_wegimp_hl = readBitsLE(frame.data, 36, 10);
    params::vw_pq.bremse_10_3a0_b10_wegimp_hl = raw_bremse_10_3a0_b10_wegimp_hl * 1.0f + 0.0f;
    // B10_Wegimp_HR: start=46 len=10 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_10_3a0_b10_wegimp_hr = readBitsLE(frame.data, 46, 10);
    params::vw_pq.bremse_10_3a0_b10_wegimp_hr = raw_bremse_10_3a0_b10_wegimp_hr * 1.0f + 0.0f;
    // B10_QB_Fahrtr_VL: start=56 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_10_3a0_b10_qb_fahrtr_vl = readBitsLE(frame.data, 56, 1) != 0;
    // B10_QB_Fahrtr_VR: start=57 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_10_3a0_b10_qb_fahrtr_vr = readBitsLE(frame.data, 57, 1) != 0;
    // B10_QB_Fahrtr_HL: start=58 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_10_3a0_b10_qb_fahrtr_hl = readBitsLE(frame.data, 58, 1) != 0;
    // B10_QB_Fahrtr_HR: start=59 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_10_3a0_b10_qb_fahrtr_hr = readBitsLE(frame.data, 59, 1) != 0;
    // B10_Fahrtr_VL: start=60 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_10_3a0_b10_fahrtr_vl = readBitsLE(frame.data, 60, 1) != 0;
    // B10_Fahrtr_VR: start=61 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_10_3a0_b10_fahrtr_vr = readBitsLE(frame.data, 61, 1) != 0;
    // B10_Fahrtr_HL: start=62 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_10_3a0_b10_fahrtr_hl = readBitsLE(frame.data, 62, 1) != 0;
    // B10_Fahrtr_HR: start=63 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_10_3a0_b10_fahrtr_hr = readBitsLE(frame.data, 63, 1) != 0;
    return true;
}

void encodeVwPq0x3A0(CANMessage &frame) {
    frame.id = 0x3A0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // B10_Checksumme
    int64_t raw_tmp_bremse_10_3a0_b10_checksumme = (int64_t)llround((params::vw_pq.bremse_10_3a0_b10_checksumme - 0.0f) / 1.0f);
    uint64_t raw_bremse_10_3a0_b10_checksumme = raw_tmp_bremse_10_3a0_b10_checksumme < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_10_3a0_b10_checksumme;
    if (raw_bremse_10_3a0_b10_checksumme > 0xFFULL) raw_bremse_10_3a0_b10_checksumme = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_bremse_10_3a0_b10_checksumme);
    // B10_Zaehler
    int64_t raw_tmp_bremse_10_3a0_b10_zaehler = (int64_t)llround((params::vw_pq.bremse_10_3a0_b10_zaehler - 0.0f) / 1.0f);
    uint64_t raw_bremse_10_3a0_b10_zaehler = raw_tmp_bremse_10_3a0_b10_zaehler < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_10_3a0_b10_zaehler;
    if (raw_bremse_10_3a0_b10_zaehler > 0xFULL) raw_bremse_10_3a0_b10_zaehler = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_bremse_10_3a0_b10_zaehler);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.bremse_10_3a0_b10_qb_wegimp_vl ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.bremse_10_3a0_b10_qb_wegimp_vr ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.bremse_10_3a0_b10_qb_wegimp_hl ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.bremse_10_3a0_b10_qb_wegimp_hr ? 1U : 0U);
    // B10_Wegimp_VL
    int64_t raw_tmp_bremse_10_3a0_b10_wegimp_vl = (int64_t)llround((params::vw_pq.bremse_10_3a0_b10_wegimp_vl - 0.0f) / 1.0f);
    uint64_t raw_bremse_10_3a0_b10_wegimp_vl = raw_tmp_bremse_10_3a0_b10_wegimp_vl < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_10_3a0_b10_wegimp_vl;
    if (raw_bremse_10_3a0_b10_wegimp_vl > 0x3FFULL) raw_bremse_10_3a0_b10_wegimp_vl = 0x3FFULL;
    writeBitsLE(frame.data, 16, 10, (uint32_t)raw_bremse_10_3a0_b10_wegimp_vl);
    // B10_Wegimp_VR
    int64_t raw_tmp_bremse_10_3a0_b10_wegimp_vr = (int64_t)llround((params::vw_pq.bremse_10_3a0_b10_wegimp_vr - 0.0f) / 1.0f);
    uint64_t raw_bremse_10_3a0_b10_wegimp_vr = raw_tmp_bremse_10_3a0_b10_wegimp_vr < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_10_3a0_b10_wegimp_vr;
    if (raw_bremse_10_3a0_b10_wegimp_vr > 0x3FFULL) raw_bremse_10_3a0_b10_wegimp_vr = 0x3FFULL;
    writeBitsLE(frame.data, 26, 10, (uint32_t)raw_bremse_10_3a0_b10_wegimp_vr);
    // B10_Wegimp_HL
    int64_t raw_tmp_bremse_10_3a0_b10_wegimp_hl = (int64_t)llround((params::vw_pq.bremse_10_3a0_b10_wegimp_hl - 0.0f) / 1.0f);
    uint64_t raw_bremse_10_3a0_b10_wegimp_hl = raw_tmp_bremse_10_3a0_b10_wegimp_hl < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_10_3a0_b10_wegimp_hl;
    if (raw_bremse_10_3a0_b10_wegimp_hl > 0x3FFULL) raw_bremse_10_3a0_b10_wegimp_hl = 0x3FFULL;
    writeBitsLE(frame.data, 36, 10, (uint32_t)raw_bremse_10_3a0_b10_wegimp_hl);
    // B10_Wegimp_HR
    int64_t raw_tmp_bremse_10_3a0_b10_wegimp_hr = (int64_t)llround((params::vw_pq.bremse_10_3a0_b10_wegimp_hr - 0.0f) / 1.0f);
    uint64_t raw_bremse_10_3a0_b10_wegimp_hr = raw_tmp_bremse_10_3a0_b10_wegimp_hr < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_10_3a0_b10_wegimp_hr;
    if (raw_bremse_10_3a0_b10_wegimp_hr > 0x3FFULL) raw_bremse_10_3a0_b10_wegimp_hr = 0x3FFULL;
    writeBitsLE(frame.data, 46, 10, (uint32_t)raw_bremse_10_3a0_b10_wegimp_hr);
    writeBitsLE(frame.data, 56, 1, params::vw_pq.bremse_10_3a0_b10_qb_fahrtr_vl ? 1U : 0U);
    writeBitsLE(frame.data, 57, 1, params::vw_pq.bremse_10_3a0_b10_qb_fahrtr_vr ? 1U : 0U);
    writeBitsLE(frame.data, 58, 1, params::vw_pq.bremse_10_3a0_b10_qb_fahrtr_hl ? 1U : 0U);
    writeBitsLE(frame.data, 59, 1, params::vw_pq.bremse_10_3a0_b10_qb_fahrtr_hr ? 1U : 0U);
    writeBitsLE(frame.data, 60, 1, params::vw_pq.bremse_10_3a0_b10_fahrtr_vl ? 1U : 0U);
    writeBitsLE(frame.data, 61, 1, params::vw_pq.bremse_10_3a0_b10_fahrtr_vr ? 1U : 0U);
    writeBitsLE(frame.data, 62, 1, params::vw_pq.bremse_10_3a0_b10_fahrtr_hl ? 1U : 0U);
    writeBitsLE(frame.data, 63, 1, params::vw_pq.bremse_10_3a0_b10_fahrtr_hr ? 1U : 0U);
}

// Message 0x343 (835) - RDK_Status
// DLC: 3, TX: XXX
bool decodeVwPq0x343(const CANMessage &frame) {
    if (frame.id != 0x343 || frame.len < 3) {
        return false;
    }

    // RKS_Reifen_VL: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_reifen_vl = readBitsLE(frame.data, 0, 1) != 0;
    // RKS_Reifen_VR: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_reifen_vr = readBitsLE(frame.data, 1, 1) != 0;
    // RKS_Reifen_HL: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_reifen_hl = readBitsLE(frame.data, 2, 1) != 0;
    // RKS_Reifen_HR: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_reifen_hr = readBitsLE(frame.data, 3, 1) != 0;
    // RKS_Reifen_RR: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_reifen_rr = readBitsLE(frame.data, 4, 1) != 0;
    // RKS_Warnung_2: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_warnung_2 = readBitsLE(frame.data, 5, 1) != 0;
    // RKS_Warnung_1: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_warnung_1 = readBitsLE(frame.data, 6, 1) != 0;
    // RKS_Systemfehler: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_systemfehler = readBitsLE(frame.data, 7, 1) != 0;
    // RKS_Kalibrier_abgew: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_kalibrier_abgew = readBitsLE(frame.data, 8, 1) != 0;
    // RKS_Druckdiff_Vorn: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_druckdiff_vorn = readBitsLE(frame.data, 9, 1) != 0;
    // RKS_Druckdiff_Hinten: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_druckdiff_hinten = readBitsLE(frame.data, 10, 1) != 0;
    // RKS_Befuellung_RR_low: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_befuellung_rr_low = readBitsLE(frame.data, 11, 1) != 0;
    // RKS_Funkstoerung: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_funkstoerung = readBitsLE(frame.data, 12, 1) != 0;
    // RKS_System_Aus: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_system_aus = readBitsLE(frame.data, 13, 1) != 0;
    // RKS_KD_Fehler: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_kd_fehler = readBitsLE(frame.data, 15, 1) != 0;
    // RKS_Lampe: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_lampe = readBitsLE(frame.data, 16, 1) != 0;
    // RKS_Ton: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_ton = readBitsLE(frame.data, 17, 1) != 0;
    // RKS_Gong: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_gong = readBitsLE(frame.data, 18, 1) != 0;
    // RKS_RDK_Blinkbit: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_rdk_blinkbit = readBitsLE(frame.data, 19, 1) != 0;
    // RKS_Teillast: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.rdk_status_343_rks_teillast = readBitsLE(frame.data, 20, 1) != 0;
    return true;
}

void encodeVwPq0x343(CANMessage &frame) {
    frame.id = 0x343;
    frame.len = 3;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 0, 1, params::vw_pq.rdk_status_343_rks_reifen_vl ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.rdk_status_343_rks_reifen_vr ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.rdk_status_343_rks_reifen_hl ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.rdk_status_343_rks_reifen_hr ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.rdk_status_343_rks_reifen_rr ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.rdk_status_343_rks_warnung_2 ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.rdk_status_343_rks_warnung_1 ? 1U : 0U);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.rdk_status_343_rks_systemfehler ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.rdk_status_343_rks_kalibrier_abgew ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.rdk_status_343_rks_druckdiff_vorn ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.rdk_status_343_rks_druckdiff_hinten ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.rdk_status_343_rks_befuellung_rr_low ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.rdk_status_343_rks_funkstoerung ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.rdk_status_343_rks_system_aus ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.rdk_status_343_rks_kd_fehler ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.rdk_status_343_rks_lampe ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.rdk_status_343_rks_ton ? 1U : 0U);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.rdk_status_343_rks_gong ? 1U : 0U);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.rdk_status_343_rks_rdk_blinkbit ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.rdk_status_343_rks_teillast ? 1U : 0U);
}

// Message 0x392 (914) - Gate_Komf_2
// DLC: 8, TX: XXX
bool decodeVwPq0x392(const CANMessage &frame) {
    if (frame.id != 0x392 || frame.len < 8) {
        return false;
    }

    // GK2_Sta_LSM: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_sta_lsm = readBitsLE(frame.data, 0, 1) != 0;
    // GK2_Sta_Lichtsensor: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_sta_lichtsensor = readBitsLE(frame.data, 1, 1) != 0;
    // GK2_Sta_Licht1: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_sta_licht1 = readBitsLE(frame.data, 2, 1) != 0;
    // GK2_Sta_VSG: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_sta_vsg = readBitsLE(frame.data, 3, 1) != 0;
    // GK2_Sta_Schluessel: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_sta_schluessel = readBitsLE(frame.data, 4, 1) != 0;
    // GK2_Sta_Profil: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_sta_profil = readBitsLE(frame.data, 5, 1) != 0;
    // GK2_Sta_Clima2: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_sta_clima2 = readBitsLE(frame.data, 6, 1) != 0;
    // GK2_Sta_BSG4: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_sta_bsg4 = readBitsLE(frame.data, 7, 1) != 0;
    // GK2_Sta_Kessy_4: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_sta_kessy_4 = readBitsLE(frame.data, 8, 1) != 0;
    // BS4_Gleitende_Leuchtw_Anf: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_bs4_gleitende_leuchtw_anf = readBitsLE(frame.data, 9, 1) != 0;
    // BS4_GLW_Fernlicht_Anf: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_bs4_glw_fernlicht_anf = readBitsLE(frame.data, 10, 1) != 0;
    // GK2_Blk_L_Kontrolle: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_blk_l_kontrolle = readBitsLE(frame.data, 11, 1) != 0;
    // GK2_Blk_R_Kontrolle: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_blk_r_kontrolle = readBitsLE(frame.data, 12, 1) != 0;
    // GK2_LS_KomFehler: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_ls_komfehler = readBitsLE(frame.data, 14, 1) != 0;
    // GK2_LS_def: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_ls_def = readBitsLE(frame.data, 15, 1) != 0;
    // GK2_Helligkeit: start=16 len=3 endian=1 sign=+ factor=714.286 offset=0 unit="Unit_Lux"
    const uint32_t raw_gate_komf_2_392_gk2_helligkeit = readBitsLE(frame.data, 16, 3);
    params::vw_pq.gate_komf_2_392_gk2_helligkeit = raw_gate_komf_2_392_gk2_helligkeit * 714.286f + 0.0f;
    // GK2_VD_zu_ver: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_vd_zu_ver = readBitsLE(frame.data, 19, 1) != 0;
    // GK2_VD_entriegelt: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_vd_entriegelt = readBitsLE(frame.data, 20, 1) != 0;
    // GK2_VD_offen_ver: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_vd_offen_ver = readBitsLE(frame.data, 21, 1) != 0;
    // GK2_Verdeck_Anf: start=22 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_verdeck_anf = readBitsLE(frame.data, 22, 1) != 0;
    // GK2_VDKD_auf: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_vdkd_auf = readBitsLE(frame.data, 23, 1) != 0;
    // GK2_Schluessel: start=24 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gate_komf_2_392_gk2_schluessel = readBitsLE(frame.data, 24, 4);
    params::vw_pq.gate_komf_2_392_gk2_schluessel = raw_gate_komf_2_392_gk2_schluessel * 1.0f + 0.0f;
    // GK2_Hardtop: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_hardtop = readBitsLE(frame.data, 28, 1) != 0;
    // GK2_AFL_Schalter: start=29 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_afl_schalter = readBitsLE(frame.data, 29, 1) != 0;
    // GK2_Nebelschluss: start=30 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_nebelschluss = readBitsLE(frame.data, 30, 1) != 0;
    // GK2_EM_LIN_ungueltig: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.gate_komf_2_392_gk2_em_lin_ungueltig = readBitsLE(frame.data, 31, 1) != 0;
    // GK2_Profil: start=32 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gate_komf_2_392_gk2_profil = readBitsLE(frame.data, 32, 4);
    params::vw_pq.gate_komf_2_392_gk2_profil = raw_gate_komf_2_392_gk2_profil * 1.0f + 0.0f;
    // GK2_Kl_StSt_Info: start=36 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gate_komf_2_392_gk2_kl_stst_info = readBitsLE(frame.data, 36, 2);
    params::vw_pq.gate_komf_2_392_gk2_kl_stst_info = raw_gate_komf_2_392_gk2_kl_stst_info * 1.0f + 0.0f;
    // GK2_BSG_StSt_Info: start=38 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gate_komf_2_392_gk2_bsg_stst_info = readBitsLE(frame.data, 38, 2);
    params::vw_pq.gate_komf_2_392_gk2_bsg_stst_info = raw_gate_komf_2_392_gk2_bsg_stst_info * 1.0f + 0.0f;
    // GK2_BEM_P_Generator: start=40 len=8 endian=1 sign=+ factor=50 offset=0 unit="Unit_Watt"
    const uint32_t raw_gate_komf_2_392_gk2_bem_p_generator = readBitsLE(frame.data, 40, 8);
    params::vw_pq.gate_komf_2_392_gk2_bem_p_generator = raw_gate_komf_2_392_gk2_bem_p_generator * 50.0f + 0.0f;
    // GK2_BEM_Abschaltstufen: start=48 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gate_komf_2_392_gk2_bem_abschaltstufen = readBitsLE(frame.data, 48, 3);
    params::vw_pq.gate_komf_2_392_gk2_bem_abschaltstufen = raw_gate_komf_2_392_gk2_bem_abschaltstufen * 1.0f + 0.0f;
    // GK2_BEM_DFM: start=51 len=5 endian=1 sign=+ factor=3.225 offset=0.025 unit="Unit_PerCent"
    const uint32_t raw_gate_komf_2_392_gk2_bem_dfm = readBitsLE(frame.data, 51, 5);
    params::vw_pq.gate_komf_2_392_gk2_bem_dfm = raw_gate_komf_2_392_gk2_bem_dfm * 3.225f + 0.025f;
    // GK2_Kessy_StSt_Info: start=56 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gate_komf_2_392_gk2_kessy_stst_info = readBitsLE(frame.data, 56, 2);
    params::vw_pq.gate_komf_2_392_gk2_kessy_stst_info = raw_gate_komf_2_392_gk2_kessy_stst_info * 1.0f + 0.0f;
    // GK2_BEM_StSt_Info: start=58 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_gate_komf_2_392_gk2_bem_stst_info = readBitsLE(frame.data, 58, 2);
    params::vw_pq.gate_komf_2_392_gk2_bem_stst_info = raw_gate_komf_2_392_gk2_bem_stst_info * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x392(CANMessage &frame) {
    frame.id = 0x392;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 0, 1, params::vw_pq.gate_komf_2_392_gk2_sta_lsm ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.gate_komf_2_392_gk2_sta_lichtsensor ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.gate_komf_2_392_gk2_sta_licht1 ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.gate_komf_2_392_gk2_sta_vsg ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.gate_komf_2_392_gk2_sta_schluessel ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.gate_komf_2_392_gk2_sta_profil ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.gate_komf_2_392_gk2_sta_clima2 ? 1U : 0U);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.gate_komf_2_392_gk2_sta_bsg4 ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.gate_komf_2_392_gk2_sta_kessy_4 ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.gate_komf_2_392_bs4_gleitende_leuchtw_anf ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.gate_komf_2_392_bs4_glw_fernlicht_anf ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.gate_komf_2_392_gk2_blk_l_kontrolle ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.gate_komf_2_392_gk2_blk_r_kontrolle ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.gate_komf_2_392_gk2_ls_komfehler ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.gate_komf_2_392_gk2_ls_def ? 1U : 0U);
    // GK2_Helligkeit
    int64_t raw_tmp_gate_komf_2_392_gk2_helligkeit = (int64_t)llround((params::vw_pq.gate_komf_2_392_gk2_helligkeit - 0.0f) / 714.286f);
    uint64_t raw_gate_komf_2_392_gk2_helligkeit = raw_tmp_gate_komf_2_392_gk2_helligkeit < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_2_392_gk2_helligkeit;
    if (raw_gate_komf_2_392_gk2_helligkeit > 0x7ULL) raw_gate_komf_2_392_gk2_helligkeit = 0x7ULL;
    writeBitsLE(frame.data, 16, 3, (uint32_t)raw_gate_komf_2_392_gk2_helligkeit);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.gate_komf_2_392_gk2_vd_zu_ver ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.gate_komf_2_392_gk2_vd_entriegelt ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.gate_komf_2_392_gk2_vd_offen_ver ? 1U : 0U);
    writeBitsLE(frame.data, 22, 1, params::vw_pq.gate_komf_2_392_gk2_verdeck_anf ? 1U : 0U);
    writeBitsLE(frame.data, 23, 1, params::vw_pq.gate_komf_2_392_gk2_vdkd_auf ? 1U : 0U);
    // GK2_Schluessel
    int64_t raw_tmp_gate_komf_2_392_gk2_schluessel = (int64_t)llround((params::vw_pq.gate_komf_2_392_gk2_schluessel - 0.0f) / 1.0f);
    uint64_t raw_gate_komf_2_392_gk2_schluessel = raw_tmp_gate_komf_2_392_gk2_schluessel < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_2_392_gk2_schluessel;
    if (raw_gate_komf_2_392_gk2_schluessel > 0xFULL) raw_gate_komf_2_392_gk2_schluessel = 0xFULL;
    writeBitsLE(frame.data, 24, 4, (uint32_t)raw_gate_komf_2_392_gk2_schluessel);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.gate_komf_2_392_gk2_hardtop ? 1U : 0U);
    writeBitsLE(frame.data, 29, 1, params::vw_pq.gate_komf_2_392_gk2_afl_schalter ? 1U : 0U);
    writeBitsLE(frame.data, 30, 1, params::vw_pq.gate_komf_2_392_gk2_nebelschluss ? 1U : 0U);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.gate_komf_2_392_gk2_em_lin_ungueltig ? 1U : 0U);
    // GK2_Profil
    int64_t raw_tmp_gate_komf_2_392_gk2_profil = (int64_t)llround((params::vw_pq.gate_komf_2_392_gk2_profil - 0.0f) / 1.0f);
    uint64_t raw_gate_komf_2_392_gk2_profil = raw_tmp_gate_komf_2_392_gk2_profil < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_2_392_gk2_profil;
    if (raw_gate_komf_2_392_gk2_profil > 0xFULL) raw_gate_komf_2_392_gk2_profil = 0xFULL;
    writeBitsLE(frame.data, 32, 4, (uint32_t)raw_gate_komf_2_392_gk2_profil);
    // GK2_Kl_StSt_Info
    int64_t raw_tmp_gate_komf_2_392_gk2_kl_stst_info = (int64_t)llround((params::vw_pq.gate_komf_2_392_gk2_kl_stst_info - 0.0f) / 1.0f);
    uint64_t raw_gate_komf_2_392_gk2_kl_stst_info = raw_tmp_gate_komf_2_392_gk2_kl_stst_info < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_2_392_gk2_kl_stst_info;
    if (raw_gate_komf_2_392_gk2_kl_stst_info > 0x3ULL) raw_gate_komf_2_392_gk2_kl_stst_info = 0x3ULL;
    writeBitsLE(frame.data, 36, 2, (uint32_t)raw_gate_komf_2_392_gk2_kl_stst_info);
    // GK2_BSG_StSt_Info
    int64_t raw_tmp_gate_komf_2_392_gk2_bsg_stst_info = (int64_t)llround((params::vw_pq.gate_komf_2_392_gk2_bsg_stst_info - 0.0f) / 1.0f);
    uint64_t raw_gate_komf_2_392_gk2_bsg_stst_info = raw_tmp_gate_komf_2_392_gk2_bsg_stst_info < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_2_392_gk2_bsg_stst_info;
    if (raw_gate_komf_2_392_gk2_bsg_stst_info > 0x3ULL) raw_gate_komf_2_392_gk2_bsg_stst_info = 0x3ULL;
    writeBitsLE(frame.data, 38, 2, (uint32_t)raw_gate_komf_2_392_gk2_bsg_stst_info);
    // GK2_BEM_P_Generator
    int64_t raw_tmp_gate_komf_2_392_gk2_bem_p_generator = (int64_t)llround((params::vw_pq.gate_komf_2_392_gk2_bem_p_generator - 0.0f) / 50.0f);
    uint64_t raw_gate_komf_2_392_gk2_bem_p_generator = raw_tmp_gate_komf_2_392_gk2_bem_p_generator < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_2_392_gk2_bem_p_generator;
    if (raw_gate_komf_2_392_gk2_bem_p_generator > 0xFFULL) raw_gate_komf_2_392_gk2_bem_p_generator = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_gate_komf_2_392_gk2_bem_p_generator);
    // GK2_BEM_Abschaltstufen
    int64_t raw_tmp_gate_komf_2_392_gk2_bem_abschaltstufen = (int64_t)llround((params::vw_pq.gate_komf_2_392_gk2_bem_abschaltstufen - 0.0f) / 1.0f);
    uint64_t raw_gate_komf_2_392_gk2_bem_abschaltstufen = raw_tmp_gate_komf_2_392_gk2_bem_abschaltstufen < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_2_392_gk2_bem_abschaltstufen;
    if (raw_gate_komf_2_392_gk2_bem_abschaltstufen > 0x7ULL) raw_gate_komf_2_392_gk2_bem_abschaltstufen = 0x7ULL;
    writeBitsLE(frame.data, 48, 3, (uint32_t)raw_gate_komf_2_392_gk2_bem_abschaltstufen);
    // GK2_BEM_DFM
    int64_t raw_tmp_gate_komf_2_392_gk2_bem_dfm = (int64_t)llround((params::vw_pq.gate_komf_2_392_gk2_bem_dfm - 0.025f) / 3.225f);
    uint64_t raw_gate_komf_2_392_gk2_bem_dfm = raw_tmp_gate_komf_2_392_gk2_bem_dfm < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_2_392_gk2_bem_dfm;
    if (raw_gate_komf_2_392_gk2_bem_dfm > 0x1FULL) raw_gate_komf_2_392_gk2_bem_dfm = 0x1FULL;
    writeBitsLE(frame.data, 51, 5, (uint32_t)raw_gate_komf_2_392_gk2_bem_dfm);
    // GK2_Kessy_StSt_Info
    int64_t raw_tmp_gate_komf_2_392_gk2_kessy_stst_info = (int64_t)llround((params::vw_pq.gate_komf_2_392_gk2_kessy_stst_info - 0.0f) / 1.0f);
    uint64_t raw_gate_komf_2_392_gk2_kessy_stst_info = raw_tmp_gate_komf_2_392_gk2_kessy_stst_info < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_2_392_gk2_kessy_stst_info;
    if (raw_gate_komf_2_392_gk2_kessy_stst_info > 0x3ULL) raw_gate_komf_2_392_gk2_kessy_stst_info = 0x3ULL;
    writeBitsLE(frame.data, 56, 2, (uint32_t)raw_gate_komf_2_392_gk2_kessy_stst_info);
    // GK2_BEM_StSt_Info
    int64_t raw_tmp_gate_komf_2_392_gk2_bem_stst_info = (int64_t)llround((params::vw_pq.gate_komf_2_392_gk2_bem_stst_info - 0.0f) / 1.0f);
    uint64_t raw_gate_komf_2_392_gk2_bem_stst_info = raw_tmp_gate_komf_2_392_gk2_bem_stst_info < 0 ? 0ULL : (uint64_t)raw_tmp_gate_komf_2_392_gk2_bem_stst_info;
    if (raw_gate_komf_2_392_gk2_bem_stst_info > 0x3ULL) raw_gate_komf_2_392_gk2_bem_stst_info = 0x3ULL;
    writeBitsLE(frame.data, 58, 2, (uint32_t)raw_gate_komf_2_392_gk2_bem_stst_info);
}

// Message 0x3BA (954) - SWA_1
// DLC: 8, TX: SWA
bool decodeVwPq0x3BA(const CANMessage &frame) {
    if (frame.id != 0x3BA || frame.len < 8) {
        return false;
    }

    // SWA_Textbits: start=12 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_swa_1_3ba_swa_textbits = readBitsLE(frame.data, 12, 4);
    params::vw_pq.swa_1_3ba_swa_textbits = raw_swa_1_3ba_swa_textbits * 1.0f + 0.0f;
    // SWA_Gong: start=16 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_swa_1_3ba_swa_gong = readBitsLE(frame.data, 16, 2);
    params::vw_pq.swa_1_3ba_swa_gong = raw_swa_1_3ba_swa_gong * 1.0f + 0.0f;
    // SWA_Sta_passiv: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.swa_1_3ba_swa_sta_passiv = readBitsLE(frame.data, 19, 1) != 0;
    // SWA_Sta_aktiv: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.swa_1_3ba_swa_sta_aktiv = readBitsLE(frame.data, 20, 1) != 0;
    // SWA_Infostufe_SWA_li: start=26 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.swa_1_3ba_swa_infostufe_swa_li = readBitsLE(frame.data, 26, 1) != 0;
    // SWA_Warnung_SWA_li: start=27 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.swa_1_3ba_swa_warnung_swa_li = readBitsLE(frame.data, 27, 1) != 0;
    // SWA_Infostufe_SWA_re: start=42 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.swa_1_3ba_swa_infostufe_swa_re = readBitsLE(frame.data, 42, 1) != 0;
    // SWA_Warnung_SWA_re: start=43 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.swa_1_3ba_swa_warnung_swa_re = readBitsLE(frame.data, 43, 1) != 0;
    // SWA_KD_Fehler: start=59 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.swa_1_3ba_swa_kd_fehler = readBitsLE(frame.data, 59, 1) != 0;
    return true;
}

void encodeVwPq0x3BA(CANMessage &frame) {
    frame.id = 0x3BA;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // SWA_Textbits
    int64_t raw_tmp_swa_1_3ba_swa_textbits = (int64_t)llround((params::vw_pq.swa_1_3ba_swa_textbits - 0.0f) / 1.0f);
    uint64_t raw_swa_1_3ba_swa_textbits = raw_tmp_swa_1_3ba_swa_textbits < 0 ? 0ULL : (uint64_t)raw_tmp_swa_1_3ba_swa_textbits;
    if (raw_swa_1_3ba_swa_textbits > 0xFULL) raw_swa_1_3ba_swa_textbits = 0xFULL;
    writeBitsLE(frame.data, 12, 4, (uint32_t)raw_swa_1_3ba_swa_textbits);
    // SWA_Gong
    int64_t raw_tmp_swa_1_3ba_swa_gong = (int64_t)llround((params::vw_pq.swa_1_3ba_swa_gong - 0.0f) / 1.0f);
    uint64_t raw_swa_1_3ba_swa_gong = raw_tmp_swa_1_3ba_swa_gong < 0 ? 0ULL : (uint64_t)raw_tmp_swa_1_3ba_swa_gong;
    if (raw_swa_1_3ba_swa_gong > 0x3ULL) raw_swa_1_3ba_swa_gong = 0x3ULL;
    writeBitsLE(frame.data, 16, 2, (uint32_t)raw_swa_1_3ba_swa_gong);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.swa_1_3ba_swa_sta_passiv ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.swa_1_3ba_swa_sta_aktiv ? 1U : 0U);
    writeBitsLE(frame.data, 26, 1, params::vw_pq.swa_1_3ba_swa_infostufe_swa_li ? 1U : 0U);
    writeBitsLE(frame.data, 27, 1, params::vw_pq.swa_1_3ba_swa_warnung_swa_li ? 1U : 0U);
    writeBitsLE(frame.data, 42, 1, params::vw_pq.swa_1_3ba_swa_infostufe_swa_re ? 1U : 0U);
    writeBitsLE(frame.data, 43, 1, params::vw_pq.swa_1_3ba_swa_warnung_swa_re ? 1U : 0U);
    writeBitsLE(frame.data, 59, 1, params::vw_pq.swa_1_3ba_swa_kd_fehler ? 1U : 0U);
}

// Message 0x497 (1175) - Parkhilfe_01
// DLC: 8, TX: XXX
bool decodeVwPq0x497(const CANMessage &frame) {
    if (frame.id != 0x497 || frame.len < 8) {
        return false;
    }

    // PH_Abschaltursache: start=13 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_parkhilfe_01_497_ph_abschaltursache = readBitsLE(frame.data, 13, 3);
    params::vw_pq.parkhilfe_01_497_ph_abschaltursache = raw_parkhilfe_01_497_ph_abschaltursache * 1.0f + 0.0f;
    // PH_Opt_Anzeige_V_ein: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_opt_anzeige_v_ein = readBitsLE(frame.data, 16, 1) != 0;
    // PH_Opt_Anzeige_H_ein: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_opt_anzeige_h_ein = readBitsLE(frame.data, 17, 1) != 0;
    // PH_Opt_Anz_V_Hindernis: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_opt_anz_v_hindernis = readBitsLE(frame.data, 18, 1) != 0;
    // PH_Opt_Anz_H_Hindernis: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_opt_anz_h_hindernis = readBitsLE(frame.data, 19, 1) != 0;
    // PH_Tongeber_V_aktiv: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_tongeber_v_aktiv = readBitsLE(frame.data, 20, 1) != 0;
    // PH_Tongeber_H_aktiv: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_tongeber_h_aktiv = readBitsLE(frame.data, 21, 1) != 0;
    // PH_Tongeber_mute: start=22 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_tongeber_mute = readBitsLE(frame.data, 22, 1) != 0;
    // PH_Anf_Audioabsenkung: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_anf_audioabsenkung = readBitsLE(frame.data, 23, 1) != 0;
    // PH_Frequenz_hinten: start=32 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_parkhilfe_01_497_ph_frequenz_hinten = readBitsLE(frame.data, 32, 4);
    params::vw_pq.parkhilfe_01_497_ph_frequenz_hinten = raw_parkhilfe_01_497_ph_frequenz_hinten * 1.0f + 0.0f;
    // PH_Lautstaerke_hinten: start=36 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_parkhilfe_01_497_ph_lautstaerke_hinten = readBitsLE(frame.data, 36, 4);
    params::vw_pq.parkhilfe_01_497_ph_lautstaerke_hinten = raw_parkhilfe_01_497_ph_lautstaerke_hinten * 1.0f + 0.0f;
    // PH_Frequenz_vorn: start=40 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_parkhilfe_01_497_ph_frequenz_vorn = readBitsLE(frame.data, 40, 4);
    params::vw_pq.parkhilfe_01_497_ph_frequenz_vorn = raw_parkhilfe_01_497_ph_frequenz_vorn * 1.0f + 0.0f;
    // PH_Lautstaerke_vorn: start=44 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_parkhilfe_01_497_ph_lautstaerke_vorn = readBitsLE(frame.data, 44, 4);
    params::vw_pq.parkhilfe_01_497_ph_lautstaerke_vorn = raw_parkhilfe_01_497_ph_lautstaerke_vorn * 1.0f + 0.0f;
    // PH_Trigger_Bildaufschaltung: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_trigger_bildaufschaltung = readBitsLE(frame.data, 48, 1) != 0;
    // PH_StartStopp_Info: start=49 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_parkhilfe_01_497_ph_startstopp_info = readBitsLE(frame.data, 49, 2);
    params::vw_pq.parkhilfe_01_497_ph_startstopp_info = raw_parkhilfe_01_497_ph_startstopp_info * 1.0f + 0.0f;
    // PH_Aufbauten_erk: start=51 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_aufbauten_erk = readBitsLE(frame.data, 51, 1) != 0;
    // PH_BerErk_vorn: start=52 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_parkhilfe_01_497_ph_bererk_vorn = readBitsLE(frame.data, 52, 2);
    params::vw_pq.parkhilfe_01_497_ph_bererk_vorn = raw_parkhilfe_01_497_ph_bererk_vorn * 1.0f + 0.0f;
    // PH_BerErk_hinten: start=54 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_parkhilfe_01_497_ph_bererk_hinten = readBitsLE(frame.data, 54, 2);
    params::vw_pq.parkhilfe_01_497_ph_bererk_hinten = raw_parkhilfe_01_497_ph_bererk_hinten * 1.0f + 0.0f;
    // PH_defekt: start=56 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_defekt = readBitsLE(frame.data, 56, 1) != 0;
    // PH_gestoert: start=57 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_gestoert = readBitsLE(frame.data, 57, 1) != 0;
    // PH_Systemzustand: start=58 len=3 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_parkhilfe_01_497_ph_systemzustand = readBitsLE(frame.data, 58, 3);
    params::vw_pq.parkhilfe_01_497_ph_systemzustand = raw_parkhilfe_01_497_ph_systemzustand * 1.0f + 0.0f;
    // PH_Display_Kundenwunsch: start=61 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_parkhilfe_01_497_ph_display_kundenwunsch = readBitsLE(frame.data, 61, 2);
    params::vw_pq.parkhilfe_01_497_ph_display_kundenwunsch = raw_parkhilfe_01_497_ph_display_kundenwunsch * 1.0f + 0.0f;
    // PH_KD_Fehler: start=63 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.parkhilfe_01_497_ph_kd_fehler = readBitsLE(frame.data, 63, 1) != 0;
    return true;
}

void encodeVwPq0x497(CANMessage &frame) {
    frame.id = 0x497;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // PH_Abschaltursache
    int64_t raw_tmp_parkhilfe_01_497_ph_abschaltursache = (int64_t)llround((params::vw_pq.parkhilfe_01_497_ph_abschaltursache - 0.0f) / 1.0f);
    uint64_t raw_parkhilfe_01_497_ph_abschaltursache = raw_tmp_parkhilfe_01_497_ph_abschaltursache < 0 ? 0ULL : (uint64_t)raw_tmp_parkhilfe_01_497_ph_abschaltursache;
    if (raw_parkhilfe_01_497_ph_abschaltursache > 0x7ULL) raw_parkhilfe_01_497_ph_abschaltursache = 0x7ULL;
    writeBitsLE(frame.data, 13, 3, (uint32_t)raw_parkhilfe_01_497_ph_abschaltursache);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.parkhilfe_01_497_ph_opt_anzeige_v_ein ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.parkhilfe_01_497_ph_opt_anzeige_h_ein ? 1U : 0U);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.parkhilfe_01_497_ph_opt_anz_v_hindernis ? 1U : 0U);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.parkhilfe_01_497_ph_opt_anz_h_hindernis ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.parkhilfe_01_497_ph_tongeber_v_aktiv ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.parkhilfe_01_497_ph_tongeber_h_aktiv ? 1U : 0U);
    writeBitsLE(frame.data, 22, 1, params::vw_pq.parkhilfe_01_497_ph_tongeber_mute ? 1U : 0U);
    writeBitsLE(frame.data, 23, 1, params::vw_pq.parkhilfe_01_497_ph_anf_audioabsenkung ? 1U : 0U);
    // PH_Frequenz_hinten
    int64_t raw_tmp_parkhilfe_01_497_ph_frequenz_hinten = (int64_t)llround((params::vw_pq.parkhilfe_01_497_ph_frequenz_hinten - 0.0f) / 1.0f);
    uint64_t raw_parkhilfe_01_497_ph_frequenz_hinten = raw_tmp_parkhilfe_01_497_ph_frequenz_hinten < 0 ? 0ULL : (uint64_t)raw_tmp_parkhilfe_01_497_ph_frequenz_hinten;
    if (raw_parkhilfe_01_497_ph_frequenz_hinten > 0xFULL) raw_parkhilfe_01_497_ph_frequenz_hinten = 0xFULL;
    writeBitsLE(frame.data, 32, 4, (uint32_t)raw_parkhilfe_01_497_ph_frequenz_hinten);
    // PH_Lautstaerke_hinten
    int64_t raw_tmp_parkhilfe_01_497_ph_lautstaerke_hinten = (int64_t)llround((params::vw_pq.parkhilfe_01_497_ph_lautstaerke_hinten - 0.0f) / 1.0f);
    uint64_t raw_parkhilfe_01_497_ph_lautstaerke_hinten = raw_tmp_parkhilfe_01_497_ph_lautstaerke_hinten < 0 ? 0ULL : (uint64_t)raw_tmp_parkhilfe_01_497_ph_lautstaerke_hinten;
    if (raw_parkhilfe_01_497_ph_lautstaerke_hinten > 0xFULL) raw_parkhilfe_01_497_ph_lautstaerke_hinten = 0xFULL;
    writeBitsLE(frame.data, 36, 4, (uint32_t)raw_parkhilfe_01_497_ph_lautstaerke_hinten);
    // PH_Frequenz_vorn
    int64_t raw_tmp_parkhilfe_01_497_ph_frequenz_vorn = (int64_t)llround((params::vw_pq.parkhilfe_01_497_ph_frequenz_vorn - 0.0f) / 1.0f);
    uint64_t raw_parkhilfe_01_497_ph_frequenz_vorn = raw_tmp_parkhilfe_01_497_ph_frequenz_vorn < 0 ? 0ULL : (uint64_t)raw_tmp_parkhilfe_01_497_ph_frequenz_vorn;
    if (raw_parkhilfe_01_497_ph_frequenz_vorn > 0xFULL) raw_parkhilfe_01_497_ph_frequenz_vorn = 0xFULL;
    writeBitsLE(frame.data, 40, 4, (uint32_t)raw_parkhilfe_01_497_ph_frequenz_vorn);
    // PH_Lautstaerke_vorn
    int64_t raw_tmp_parkhilfe_01_497_ph_lautstaerke_vorn = (int64_t)llround((params::vw_pq.parkhilfe_01_497_ph_lautstaerke_vorn - 0.0f) / 1.0f);
    uint64_t raw_parkhilfe_01_497_ph_lautstaerke_vorn = raw_tmp_parkhilfe_01_497_ph_lautstaerke_vorn < 0 ? 0ULL : (uint64_t)raw_tmp_parkhilfe_01_497_ph_lautstaerke_vorn;
    if (raw_parkhilfe_01_497_ph_lautstaerke_vorn > 0xFULL) raw_parkhilfe_01_497_ph_lautstaerke_vorn = 0xFULL;
    writeBitsLE(frame.data, 44, 4, (uint32_t)raw_parkhilfe_01_497_ph_lautstaerke_vorn);
    writeBitsLE(frame.data, 48, 1, params::vw_pq.parkhilfe_01_497_ph_trigger_bildaufschaltung ? 1U : 0U);
    // PH_StartStopp_Info
    int64_t raw_tmp_parkhilfe_01_497_ph_startstopp_info = (int64_t)llround((params::vw_pq.parkhilfe_01_497_ph_startstopp_info - 0.0f) / 1.0f);
    uint64_t raw_parkhilfe_01_497_ph_startstopp_info = raw_tmp_parkhilfe_01_497_ph_startstopp_info < 0 ? 0ULL : (uint64_t)raw_tmp_parkhilfe_01_497_ph_startstopp_info;
    if (raw_parkhilfe_01_497_ph_startstopp_info > 0x3ULL) raw_parkhilfe_01_497_ph_startstopp_info = 0x3ULL;
    writeBitsLE(frame.data, 49, 2, (uint32_t)raw_parkhilfe_01_497_ph_startstopp_info);
    writeBitsLE(frame.data, 51, 1, params::vw_pq.parkhilfe_01_497_ph_aufbauten_erk ? 1U : 0U);
    // PH_BerErk_vorn
    int64_t raw_tmp_parkhilfe_01_497_ph_bererk_vorn = (int64_t)llround((params::vw_pq.parkhilfe_01_497_ph_bererk_vorn - 0.0f) / 1.0f);
    uint64_t raw_parkhilfe_01_497_ph_bererk_vorn = raw_tmp_parkhilfe_01_497_ph_bererk_vorn < 0 ? 0ULL : (uint64_t)raw_tmp_parkhilfe_01_497_ph_bererk_vorn;
    if (raw_parkhilfe_01_497_ph_bererk_vorn > 0x3ULL) raw_parkhilfe_01_497_ph_bererk_vorn = 0x3ULL;
    writeBitsLE(frame.data, 52, 2, (uint32_t)raw_parkhilfe_01_497_ph_bererk_vorn);
    // PH_BerErk_hinten
    int64_t raw_tmp_parkhilfe_01_497_ph_bererk_hinten = (int64_t)llround((params::vw_pq.parkhilfe_01_497_ph_bererk_hinten - 0.0f) / 1.0f);
    uint64_t raw_parkhilfe_01_497_ph_bererk_hinten = raw_tmp_parkhilfe_01_497_ph_bererk_hinten < 0 ? 0ULL : (uint64_t)raw_tmp_parkhilfe_01_497_ph_bererk_hinten;
    if (raw_parkhilfe_01_497_ph_bererk_hinten > 0x3ULL) raw_parkhilfe_01_497_ph_bererk_hinten = 0x3ULL;
    writeBitsLE(frame.data, 54, 2, (uint32_t)raw_parkhilfe_01_497_ph_bererk_hinten);
    writeBitsLE(frame.data, 56, 1, params::vw_pq.parkhilfe_01_497_ph_defekt ? 1U : 0U);
    writeBitsLE(frame.data, 57, 1, params::vw_pq.parkhilfe_01_497_ph_gestoert ? 1U : 0U);
    // PH_Systemzustand
    int64_t raw_tmp_parkhilfe_01_497_ph_systemzustand = (int64_t)llround((params::vw_pq.parkhilfe_01_497_ph_systemzustand - 0.0f) / 1.0f);
    uint64_t raw_parkhilfe_01_497_ph_systemzustand = raw_tmp_parkhilfe_01_497_ph_systemzustand < 0 ? 0ULL : (uint64_t)raw_tmp_parkhilfe_01_497_ph_systemzustand;
    if (raw_parkhilfe_01_497_ph_systemzustand > 0x7ULL) raw_parkhilfe_01_497_ph_systemzustand = 0x7ULL;
    writeBitsLE(frame.data, 58, 3, (uint32_t)raw_parkhilfe_01_497_ph_systemzustand);
    // PH_Display_Kundenwunsch
    int64_t raw_tmp_parkhilfe_01_497_ph_display_kundenwunsch = (int64_t)llround((params::vw_pq.parkhilfe_01_497_ph_display_kundenwunsch - 0.0f) / 1.0f);
    uint64_t raw_parkhilfe_01_497_ph_display_kundenwunsch = raw_tmp_parkhilfe_01_497_ph_display_kundenwunsch < 0 ? 0ULL : (uint64_t)raw_tmp_parkhilfe_01_497_ph_display_kundenwunsch;
    if (raw_parkhilfe_01_497_ph_display_kundenwunsch > 0x3ULL) raw_parkhilfe_01_497_ph_display_kundenwunsch = 0x3ULL;
    writeBitsLE(frame.data, 61, 2, (uint32_t)raw_parkhilfe_01_497_ph_display_kundenwunsch);
    writeBitsLE(frame.data, 63, 1, params::vw_pq.parkhilfe_01_497_ph_kd_fehler ? 1U : 0U);
}

// Message 0x5B7 (1463) - Bremse_11
// DLC: 8, TX: XXX
bool decodeVwPq0x5B7(const CANMessage &frame) {
    if (frame.id != 0x5B7 || frame.len < 8) {
        return false;
    }

    // CHECKSUM: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_11_5b7_checksum = readBitsLE(frame.data, 0, 8);
    params::vw_pq.bremse_11_5b7_checksum = raw_bremse_11_5b7_checksum * 1.0f + 0.0f;
    // COUNTER: start=8 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_11_5b7_counter = readBitsLE(frame.data, 8, 4);
    params::vw_pq.bremse_11_5b7_counter = raw_bremse_11_5b7_counter * 1.0f + 0.0f;
    // B11_HydHalten: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_11_5b7_b11_hydhalten = readBitsLE(frame.data, 13, 1) != 0;
    // B11_Br_StSt_Info: start=14 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_11_5b7_b11_br_stst_info = readBitsLE(frame.data, 14, 2);
    params::vw_pq.bremse_11_5b7_b11_br_stst_info = raw_bremse_11_5b7_b11_br_stst_info * 1.0f + 0.0f;
    // B11_OBD_Nib_VL: start=16 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_11_5b7_b11_obd_nib_vl = readBitsLE(frame.data, 16, 4);
    params::vw_pq.bremse_11_5b7_b11_obd_nib_vl = raw_bremse_11_5b7_b11_obd_nib_vl * 1.0f + 0.0f;
    // B11_OBD_Nib_VR: start=20 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_11_5b7_b11_obd_nib_vr = readBitsLE(frame.data, 20, 4);
    params::vw_pq.bremse_11_5b7_b11_obd_nib_vr = raw_bremse_11_5b7_b11_obd_nib_vr * 1.0f + 0.0f;
    // B11_OBD_Nib_HL: start=24 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_11_5b7_b11_obd_nib_hl = readBitsLE(frame.data, 24, 4);
    params::vw_pq.bremse_11_5b7_b11_obd_nib_hl = raw_bremse_11_5b7_b11_obd_nib_hl * 1.0f + 0.0f;
    // B11_OBD_Nib_HR: start=28 len=4 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_bremse_11_5b7_b11_obd_nib_hr = readBitsLE(frame.data, 28, 4);
    params::vw_pq.bremse_11_5b7_b11_obd_nib_hr = raw_bremse_11_5b7_b11_obd_nib_hr * 1.0f + 0.0f;
    // B11_EPB_Steller_akt: start=32 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_11_5b7_b11_epb_steller_akt = readBitsLE(frame.data, 32, 1) != 0;
    // B11_EPB_Steller_gue: start=33 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.bremse_11_5b7_b11_epb_steller_gue = readBitsLE(frame.data, 33, 1) != 0;
    return true;
}

void encodeVwPq0x5B7(CANMessage &frame) {
    frame.id = 0x5B7;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // CHECKSUM
    int64_t raw_tmp_bremse_11_5b7_checksum = (int64_t)llround((params::vw_pq.bremse_11_5b7_checksum - 0.0f) / 1.0f);
    uint64_t raw_bremse_11_5b7_checksum = raw_tmp_bremse_11_5b7_checksum < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_11_5b7_checksum;
    if (raw_bremse_11_5b7_checksum > 0xFFULL) raw_bremse_11_5b7_checksum = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_bremse_11_5b7_checksum);
    // COUNTER
    int64_t raw_tmp_bremse_11_5b7_counter = (int64_t)llround((params::vw_pq.bremse_11_5b7_counter - 0.0f) / 1.0f);
    uint64_t raw_bremse_11_5b7_counter = raw_tmp_bremse_11_5b7_counter < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_11_5b7_counter;
    if (raw_bremse_11_5b7_counter > 0xFULL) raw_bremse_11_5b7_counter = 0xFULL;
    writeBitsLE(frame.data, 8, 4, (uint32_t)raw_bremse_11_5b7_counter);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.bremse_11_5b7_b11_hydhalten ? 1U : 0U);
    // B11_Br_StSt_Info
    int64_t raw_tmp_bremse_11_5b7_b11_br_stst_info = (int64_t)llround((params::vw_pq.bremse_11_5b7_b11_br_stst_info - 0.0f) / 1.0f);
    uint64_t raw_bremse_11_5b7_b11_br_stst_info = raw_tmp_bremse_11_5b7_b11_br_stst_info < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_11_5b7_b11_br_stst_info;
    if (raw_bremse_11_5b7_b11_br_stst_info > 0x3ULL) raw_bremse_11_5b7_b11_br_stst_info = 0x3ULL;
    writeBitsLE(frame.data, 14, 2, (uint32_t)raw_bremse_11_5b7_b11_br_stst_info);
    // B11_OBD_Nib_VL
    int64_t raw_tmp_bremse_11_5b7_b11_obd_nib_vl = (int64_t)llround((params::vw_pq.bremse_11_5b7_b11_obd_nib_vl - 0.0f) / 1.0f);
    uint64_t raw_bremse_11_5b7_b11_obd_nib_vl = raw_tmp_bremse_11_5b7_b11_obd_nib_vl < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_11_5b7_b11_obd_nib_vl;
    if (raw_bremse_11_5b7_b11_obd_nib_vl > 0xFULL) raw_bremse_11_5b7_b11_obd_nib_vl = 0xFULL;
    writeBitsLE(frame.data, 16, 4, (uint32_t)raw_bremse_11_5b7_b11_obd_nib_vl);
    // B11_OBD_Nib_VR
    int64_t raw_tmp_bremse_11_5b7_b11_obd_nib_vr = (int64_t)llround((params::vw_pq.bremse_11_5b7_b11_obd_nib_vr - 0.0f) / 1.0f);
    uint64_t raw_bremse_11_5b7_b11_obd_nib_vr = raw_tmp_bremse_11_5b7_b11_obd_nib_vr < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_11_5b7_b11_obd_nib_vr;
    if (raw_bremse_11_5b7_b11_obd_nib_vr > 0xFULL) raw_bremse_11_5b7_b11_obd_nib_vr = 0xFULL;
    writeBitsLE(frame.data, 20, 4, (uint32_t)raw_bremse_11_5b7_b11_obd_nib_vr);
    // B11_OBD_Nib_HL
    int64_t raw_tmp_bremse_11_5b7_b11_obd_nib_hl = (int64_t)llround((params::vw_pq.bremse_11_5b7_b11_obd_nib_hl - 0.0f) / 1.0f);
    uint64_t raw_bremse_11_5b7_b11_obd_nib_hl = raw_tmp_bremse_11_5b7_b11_obd_nib_hl < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_11_5b7_b11_obd_nib_hl;
    if (raw_bremse_11_5b7_b11_obd_nib_hl > 0xFULL) raw_bremse_11_5b7_b11_obd_nib_hl = 0xFULL;
    writeBitsLE(frame.data, 24, 4, (uint32_t)raw_bremse_11_5b7_b11_obd_nib_hl);
    // B11_OBD_Nib_HR
    int64_t raw_tmp_bremse_11_5b7_b11_obd_nib_hr = (int64_t)llround((params::vw_pq.bremse_11_5b7_b11_obd_nib_hr - 0.0f) / 1.0f);
    uint64_t raw_bremse_11_5b7_b11_obd_nib_hr = raw_tmp_bremse_11_5b7_b11_obd_nib_hr < 0 ? 0ULL : (uint64_t)raw_tmp_bremse_11_5b7_b11_obd_nib_hr;
    if (raw_bremse_11_5b7_b11_obd_nib_hr > 0xFULL) raw_bremse_11_5b7_b11_obd_nib_hr = 0xFULL;
    writeBitsLE(frame.data, 28, 4, (uint32_t)raw_bremse_11_5b7_b11_obd_nib_hr);
    writeBitsLE(frame.data, 32, 1, params::vw_pq.bremse_11_5b7_b11_epb_steller_akt ? 1U : 0U);
    writeBitsLE(frame.data, 33, 1, params::vw_pq.bremse_11_5b7_b11_epb_steller_gue ? 1U : 0U);
}

// Message 0x5DC (1500) - Soll_Verbauliste_neu
// DLC: 8, TX: XXX
bool decodeVwPq0x5DC(const CANMessage &frame) {
    if (frame.id != 0x5DC || frame.len < 8) {
        return false;
    }

    // VL1_Motor_SG: start=0 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_motor_sg = readBitsLE(frame.data, 0, 1) != 0;
    // VL1_Getr_SG: start=1 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_getr_sg = readBitsLE(frame.data, 1, 1) != 0;
    // VL1_ABS: start=2 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_abs = readBitsLE(frame.data, 2, 1) != 0;
    // VL1_Kombi: start=3 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_kombi = readBitsLE(frame.data, 3, 1) != 0;
    // VL1_LSM: start=4 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_lsm = readBitsLE(frame.data, 4, 1) != 0;
    // VL1_Airbag: start=5 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_airbag = readBitsLE(frame.data, 5, 1) != 0;
    // VL1_Lenkhilfe: start=6 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_lenkhilfe = readBitsLE(frame.data, 6, 1) != 0;
    // VL1_dyn_LWR: start=7 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_dyn_lwr = readBitsLE(frame.data, 7, 1) != 0;
    // VL1_res_08: start=8 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_res_08 = readBitsLE(frame.data, 8, 1) != 0;
    // VL1_Allrad: start=9 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_allrad = readBitsLE(frame.data, 9, 1) != 0;
    // VL1_ADR: start=10 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_adr = readBitsLE(frame.data, 10, 1) != 0;
    // VL1_ADR_getrennt: start=11 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_adr_getrennt = readBitsLE(frame.data, 11, 1) != 0;
    // VL1_EPB: start=12 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_epb = readBitsLE(frame.data, 12, 1) != 0;
    // VL1_res_13: start=13 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_res_13 = readBitsLE(frame.data, 13, 1) != 0;
    // VL1_Daempfer: start=14 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_daempfer = readBitsLE(frame.data, 14, 1) != 0;
    // VL1_Quersperre: start=15 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_quersperre = readBitsLE(frame.data, 15, 1) != 0;
    // VL1_MotorSlave: start=16 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_motorslave = readBitsLE(frame.data, 16, 1) != 0;
    // VL1_SWA: start=17 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_swa = readBitsLE(frame.data, 17, 1) != 0;
    // VL1_HCA: start=18 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_hca = readBitsLE(frame.data, 18, 1) != 0;
    // VL1_RKA_Plus: start=19 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_rka_plus = readBitsLE(frame.data, 19, 1) != 0;
    // VL1_PLA: start=20 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_pla = readBitsLE(frame.data, 20, 1) != 0;
    // VL1_WFS_KBI: start=21 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_wfs_kbi = readBitsLE(frame.data, 21, 1) != 0;
    // VL1_Kombi_KBI: start=22 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_kombi_kbi = readBitsLE(frame.data, 22, 1) != 0;
    // VL1_Soll_eq_Ist: start=23 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_soll_eq_ist = readBitsLE(frame.data, 23, 1) != 0;
    // VL1_BSG_Komf: start=24 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_bsg_komf = readBitsLE(frame.data, 24, 1) != 0;
    // VL1_ZKE: start=25 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_zke = readBitsLE(frame.data, 25, 1) != 0;
    // VL1_TSG_FT: start=26 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_tsg_ft = readBitsLE(frame.data, 26, 1) != 0;
    // VL1_TSG_BT: start=27 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_tsg_bt = readBitsLE(frame.data, 27, 1) != 0;
    // VL1_TSG_HL: start=28 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_tsg_hl = readBitsLE(frame.data, 28, 1) != 0;
    // VL1_TSG_HR: start=29 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_tsg_hr = readBitsLE(frame.data, 29, 1) != 0;
    // VL1_Memory: start=30 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_memory = readBitsLE(frame.data, 30, 1) != 0;
    // VL1_Dachmodul_K: start=31 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_dachmodul_k = readBitsLE(frame.data, 31, 1) != 0;
    // VL1_Zentralelektrik_II: start=32 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_zentralelektrik_ii = readBitsLE(frame.data, 32, 1) != 0;
    // VL1_RDK: start=33 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_rdk = readBitsLE(frame.data, 33, 1) != 0;
    // VL1_Lenksaeule: start=34 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_lenksaeule = readBitsLE(frame.data, 34, 1) != 0;
    // VL1_Gateway: start=35 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_gateway = readBitsLE(frame.data, 35, 1) != 0;
    // VL1_Clima_Komf: start=36 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_clima_komf = readBitsLE(frame.data, 36, 1) != 0;
    // VL1_Einparkhilfe: start=37 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_einparkhilfe = readBitsLE(frame.data, 37, 1) != 0;
    // VL1_PTC_Heizung: start=38 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_ptc_heizung = readBitsLE(frame.data, 38, 1) != 0;
    // VL1_Standheiz: start=39 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_standheiz = readBitsLE(frame.data, 39, 1) != 0;
    // VL1_Verdeck: start=40 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_verdeck = readBitsLE(frame.data, 40, 1) != 0;
    // VL1_RSE_I: start=41 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_rse_i = readBitsLE(frame.data, 41, 1) != 0;
    // VL1_res_42: start=42 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_res_42 = readBitsLE(frame.data, 42, 1) != 0;
    // VL1_MDI_I: start=43 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_mdi_i = readBitsLE(frame.data, 43, 1) != 0;
    // VL1_Anhaenger: start=44 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_anhaenger = readBitsLE(frame.data, 44, 1) != 0;
    // VL1_Memory_BF: start=45 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_memory_bf = readBitsLE(frame.data, 45, 1) != 0;
    // VL1_Easy_Entry_VF: start=46 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_easy_entry_vf = readBitsLE(frame.data, 46, 1) != 0;
    // VL1_Easy_Entry_VB: start=47 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_easy_entry_vb = readBitsLE(frame.data, 47, 1) != 0;
    // VL1_Heckdeckel: start=48 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_heckdeckel = readBitsLE(frame.data, 48, 1) != 0;
    // VL1_Rearview: start=49 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_rearview = readBitsLE(frame.data, 49, 1) != 0;
    // VL1_Sonderfzg_SG: start=50 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_sonderfzg_sg = readBitsLE(frame.data, 50, 1) != 0;
    // VL1_Tastenmodul: start=51 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_tastenmodul = readBitsLE(frame.data, 51, 1) != 0;
    // VL1_Kompass: start=52 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_kompass = readBitsLE(frame.data, 52, 1) != 0;
    // VL1_WFS_K: start=53 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_wfs_k = readBitsLE(frame.data, 53, 1) != 0;
    // VL1_GSM_Pager: start=54 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_gsm_pager = readBitsLE(frame.data, 54, 1) != 0;
    // VL1_InfoElektronik: start=55 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_infoelektronik = readBitsLE(frame.data, 55, 1) != 0;
    // VL1_DSP: start=56 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_dsp = readBitsLE(frame.data, 56, 1) != 0;
    // VL1_DAB: start=57 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_dab = readBitsLE(frame.data, 57, 1) != 0;
    // VL1_Telematik: start=58 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_telematik = readBitsLE(frame.data, 58, 1) != 0;
    // VL1_Navigation: start=59 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_navigation = readBitsLE(frame.data, 59, 1) != 0;
    // VL1_TV_Tuner: start=60 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_tv_tuner = readBitsLE(frame.data, 60, 1) != 0;
    // VL1_Neigungsmodul_I: start=61 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_neigungsmodul_i = readBitsLE(frame.data, 61, 1) != 0;
    // VL1_Radio: start=62 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_radio = readBitsLE(frame.data, 62, 1) != 0;
    // VL1_Telefon: start=63 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.soll_verbauliste_neu_5dc_vl1_telefon = readBitsLE(frame.data, 63, 1) != 0;
    return true;
}

void encodeVwPq0x5DC(CANMessage &frame) {
    frame.id = 0x5DC;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 0, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_motor_sg ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_getr_sg ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_abs ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_kombi ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_lsm ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_airbag ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_lenkhilfe ? 1U : 0U);
    writeBitsLE(frame.data, 7, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_dyn_lwr ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_res_08 ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_allrad ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_adr ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_adr_getrennt ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_epb ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_res_13 ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_daempfer ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_quersperre ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_motorslave ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_swa ? 1U : 0U);
    writeBitsLE(frame.data, 18, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_hca ? 1U : 0U);
    writeBitsLE(frame.data, 19, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_rka_plus ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_pla ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_wfs_kbi ? 1U : 0U);
    writeBitsLE(frame.data, 22, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_kombi_kbi ? 1U : 0U);
    writeBitsLE(frame.data, 23, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_soll_eq_ist ? 1U : 0U);
    writeBitsLE(frame.data, 24, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_bsg_komf ? 1U : 0U);
    writeBitsLE(frame.data, 25, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_zke ? 1U : 0U);
    writeBitsLE(frame.data, 26, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_tsg_ft ? 1U : 0U);
    writeBitsLE(frame.data, 27, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_tsg_bt ? 1U : 0U);
    writeBitsLE(frame.data, 28, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_tsg_hl ? 1U : 0U);
    writeBitsLE(frame.data, 29, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_tsg_hr ? 1U : 0U);
    writeBitsLE(frame.data, 30, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_memory ? 1U : 0U);
    writeBitsLE(frame.data, 31, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_dachmodul_k ? 1U : 0U);
    writeBitsLE(frame.data, 32, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_zentralelektrik_ii ? 1U : 0U);
    writeBitsLE(frame.data, 33, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_rdk ? 1U : 0U);
    writeBitsLE(frame.data, 34, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_lenksaeule ? 1U : 0U);
    writeBitsLE(frame.data, 35, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_gateway ? 1U : 0U);
    writeBitsLE(frame.data, 36, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_clima_komf ? 1U : 0U);
    writeBitsLE(frame.data, 37, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_einparkhilfe ? 1U : 0U);
    writeBitsLE(frame.data, 38, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_ptc_heizung ? 1U : 0U);
    writeBitsLE(frame.data, 39, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_standheiz ? 1U : 0U);
    writeBitsLE(frame.data, 40, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_verdeck ? 1U : 0U);
    writeBitsLE(frame.data, 41, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_rse_i ? 1U : 0U);
    writeBitsLE(frame.data, 42, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_res_42 ? 1U : 0U);
    writeBitsLE(frame.data, 43, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_mdi_i ? 1U : 0U);
    writeBitsLE(frame.data, 44, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_anhaenger ? 1U : 0U);
    writeBitsLE(frame.data, 45, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_memory_bf ? 1U : 0U);
    writeBitsLE(frame.data, 46, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_easy_entry_vf ? 1U : 0U);
    writeBitsLE(frame.data, 47, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_easy_entry_vb ? 1U : 0U);
    writeBitsLE(frame.data, 48, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_heckdeckel ? 1U : 0U);
    writeBitsLE(frame.data, 49, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_rearview ? 1U : 0U);
    writeBitsLE(frame.data, 50, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_sonderfzg_sg ? 1U : 0U);
    writeBitsLE(frame.data, 51, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_tastenmodul ? 1U : 0U);
    writeBitsLE(frame.data, 52, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_kompass ? 1U : 0U);
    writeBitsLE(frame.data, 53, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_wfs_k ? 1U : 0U);
    writeBitsLE(frame.data, 54, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_gsm_pager ? 1U : 0U);
    writeBitsLE(frame.data, 55, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_infoelektronik ? 1U : 0U);
    writeBitsLE(frame.data, 56, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_dsp ? 1U : 0U);
    writeBitsLE(frame.data, 57, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_dab ? 1U : 0U);
    writeBitsLE(frame.data, 58, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_telematik ? 1U : 0U);
    writeBitsLE(frame.data, 59, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_navigation ? 1U : 0U);
    writeBitsLE(frame.data, 60, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_tv_tuner ? 1U : 0U);
    writeBitsLE(frame.data, 61, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_neigungsmodul_i ? 1U : 0U);
    writeBitsLE(frame.data, 62, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_radio ? 1U : 0U);
    writeBitsLE(frame.data, 63, 1, params::vw_pq.soll_verbauliste_neu_5dc_vl1_telefon ? 1U : 0U);
}

// Message 0x5D2 (1490) - Ident
// DLC: 8, TX: XXX
bool decodeVwPq0x5D2(const CANMessage &frame) {
    if (frame.id != 0x5D2 || frame.len < 8) {
        return false;
    }

    // IDT_Mux M: start=0 len=2 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_mux_m = readBitsLE(frame.data, 0, 2);
    params::vw_pq.ident_5d2_idt_mux_m = raw_ident_5d2_idt_mux_m * 1.0f + 0.0f;
    // IDT_Geheimnis_1 m0: start=8 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_geheimnis_1_m0 = readBitsLE(frame.data, 8, 8);
    params::vw_pq.ident_5d2_idt_geheimnis_1_m0 = raw_ident_5d2_idt_geheimnis_1_m0 * 1.0f + 0.0f;
    // IDT_VIN_4 m1: start=8 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_4_m1 = readBitsLE(frame.data, 8, 8);
    params::vw_pq.ident_5d2_idt_vin_4_m1 = raw_ident_5d2_idt_vin_4_m1 * 1.0f + 0.0f;
    // IDT_VIN_11 m2: start=8 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_11_m2 = readBitsLE(frame.data, 8, 8);
    params::vw_pq.ident_5d2_idt_vin_11_m2 = raw_ident_5d2_idt_vin_11_m2 * 1.0f + 0.0f;
    // IDT_Geheimnis_2 m0: start=16 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_geheimnis_2_m0 = readBitsLE(frame.data, 16, 8);
    params::vw_pq.ident_5d2_idt_geheimnis_2_m0 = raw_ident_5d2_idt_geheimnis_2_m0 * 1.0f + 0.0f;
    // IDT_VIN_5 m1: start=16 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_5_m1 = readBitsLE(frame.data, 16, 8);
    params::vw_pq.ident_5d2_idt_vin_5_m1 = raw_ident_5d2_idt_vin_5_m1 * 1.0f + 0.0f;
    // IDT_VIN_12 m2: start=16 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_12_m2 = readBitsLE(frame.data, 16, 8);
    params::vw_pq.ident_5d2_idt_vin_12_m2 = raw_ident_5d2_idt_vin_12_m2 * 1.0f + 0.0f;
    // IDT_Geheimnis_3 m0: start=24 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_geheimnis_3_m0 = readBitsLE(frame.data, 24, 8);
    params::vw_pq.ident_5d2_idt_geheimnis_3_m0 = raw_ident_5d2_idt_geheimnis_3_m0 * 1.0f + 0.0f;
    // IDT_VIN_6 m1: start=24 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_6_m1 = readBitsLE(frame.data, 24, 8);
    params::vw_pq.ident_5d2_idt_vin_6_m1 = raw_ident_5d2_idt_vin_6_m1 * 1.0f + 0.0f;
    // IDT_VIN_13 m2: start=24 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_13_m2 = readBitsLE(frame.data, 24, 8);
    params::vw_pq.ident_5d2_idt_vin_13_m2 = raw_ident_5d2_idt_vin_13_m2 * 1.0f + 0.0f;
    // IDT_Geheimnis_4 m0: start=32 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_geheimnis_4_m0 = readBitsLE(frame.data, 32, 8);
    params::vw_pq.ident_5d2_idt_geheimnis_4_m0 = raw_ident_5d2_idt_geheimnis_4_m0 * 1.0f + 0.0f;
    // IDT_VIN_7 m1: start=32 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_7_m1 = readBitsLE(frame.data, 32, 8);
    params::vw_pq.ident_5d2_idt_vin_7_m1 = raw_ident_5d2_idt_vin_7_m1 * 1.0f + 0.0f;
    // IDT_VIN_14 m2: start=32 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_14_m2 = readBitsLE(frame.data, 32, 8);
    params::vw_pq.ident_5d2_idt_vin_14_m2 = raw_ident_5d2_idt_vin_14_m2 * 1.0f + 0.0f;
    // IDT_VIN_1 m0: start=40 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_1_m0 = readBitsLE(frame.data, 40, 8);
    params::vw_pq.ident_5d2_idt_vin_1_m0 = raw_ident_5d2_idt_vin_1_m0 * 1.0f + 0.0f;
    // IDT_VIN_8 m1: start=40 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_8_m1 = readBitsLE(frame.data, 40, 8);
    params::vw_pq.ident_5d2_idt_vin_8_m1 = raw_ident_5d2_idt_vin_8_m1 * 1.0f + 0.0f;
    // IDT_VIN_15 m2: start=40 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_15_m2 = readBitsLE(frame.data, 40, 8);
    params::vw_pq.ident_5d2_idt_vin_15_m2 = raw_ident_5d2_idt_vin_15_m2 * 1.0f + 0.0f;
    // IDT_VIN_2 m0: start=48 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_2_m0 = readBitsLE(frame.data, 48, 8);
    params::vw_pq.ident_5d2_idt_vin_2_m0 = raw_ident_5d2_idt_vin_2_m0 * 1.0f + 0.0f;
    // IDT_VIN_9 m1: start=48 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_9_m1 = readBitsLE(frame.data, 48, 8);
    params::vw_pq.ident_5d2_idt_vin_9_m1 = raw_ident_5d2_idt_vin_9_m1 * 1.0f + 0.0f;
    // IDT_VIN_16 m2: start=48 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_16_m2 = readBitsLE(frame.data, 48, 8);
    params::vw_pq.ident_5d2_idt_vin_16_m2 = raw_ident_5d2_idt_vin_16_m2 * 1.0f + 0.0f;
    // IDT_VIN_3 m0: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_3_m0 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.ident_5d2_idt_vin_3_m0 = raw_ident_5d2_idt_vin_3_m0 * 1.0f + 0.0f;
    // IDT_VIN_10 m1: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_10_m1 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.ident_5d2_idt_vin_10_m1 = raw_ident_5d2_idt_vin_10_m1 * 1.0f + 0.0f;
    // IDT_VIN_17 m2: start=56 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_ident_5d2_idt_vin_17_m2 = readBitsLE(frame.data, 56, 8);
    params::vw_pq.ident_5d2_idt_vin_17_m2 = raw_ident_5d2_idt_vin_17_m2 * 1.0f + 0.0f;
    return true;
}

void encodeVwPq0x5D2(CANMessage &frame) {
    frame.id = 0x5D2;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // IDT_Mux M
    int64_t raw_tmp_ident_5d2_idt_mux_m = (int64_t)llround((params::vw_pq.ident_5d2_idt_mux_m - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_mux_m = raw_tmp_ident_5d2_idt_mux_m < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_mux_m;
    if (raw_ident_5d2_idt_mux_m > 0x3ULL) raw_ident_5d2_idt_mux_m = 0x3ULL;
    writeBitsLE(frame.data, 0, 2, (uint32_t)raw_ident_5d2_idt_mux_m);
    // IDT_Geheimnis_1 m0
    int64_t raw_tmp_ident_5d2_idt_geheimnis_1_m0 = (int64_t)llround((params::vw_pq.ident_5d2_idt_geheimnis_1_m0 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_geheimnis_1_m0 = raw_tmp_ident_5d2_idt_geheimnis_1_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_geheimnis_1_m0;
    if (raw_ident_5d2_idt_geheimnis_1_m0 > 0xFFULL) raw_ident_5d2_idt_geheimnis_1_m0 = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_ident_5d2_idt_geheimnis_1_m0);
    // IDT_VIN_4 m1
    int64_t raw_tmp_ident_5d2_idt_vin_4_m1 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_4_m1 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_4_m1 = raw_tmp_ident_5d2_idt_vin_4_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_4_m1;
    if (raw_ident_5d2_idt_vin_4_m1 > 0xFFULL) raw_ident_5d2_idt_vin_4_m1 = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_ident_5d2_idt_vin_4_m1);
    // IDT_VIN_11 m2
    int64_t raw_tmp_ident_5d2_idt_vin_11_m2 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_11_m2 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_11_m2 = raw_tmp_ident_5d2_idt_vin_11_m2 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_11_m2;
    if (raw_ident_5d2_idt_vin_11_m2 > 0xFFULL) raw_ident_5d2_idt_vin_11_m2 = 0xFFULL;
    writeBitsLE(frame.data, 8, 8, (uint32_t)raw_ident_5d2_idt_vin_11_m2);
    // IDT_Geheimnis_2 m0
    int64_t raw_tmp_ident_5d2_idt_geheimnis_2_m0 = (int64_t)llround((params::vw_pq.ident_5d2_idt_geheimnis_2_m0 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_geheimnis_2_m0 = raw_tmp_ident_5d2_idt_geheimnis_2_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_geheimnis_2_m0;
    if (raw_ident_5d2_idt_geheimnis_2_m0 > 0xFFULL) raw_ident_5d2_idt_geheimnis_2_m0 = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_ident_5d2_idt_geheimnis_2_m0);
    // IDT_VIN_5 m1
    int64_t raw_tmp_ident_5d2_idt_vin_5_m1 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_5_m1 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_5_m1 = raw_tmp_ident_5d2_idt_vin_5_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_5_m1;
    if (raw_ident_5d2_idt_vin_5_m1 > 0xFFULL) raw_ident_5d2_idt_vin_5_m1 = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_ident_5d2_idt_vin_5_m1);
    // IDT_VIN_12 m2
    int64_t raw_tmp_ident_5d2_idt_vin_12_m2 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_12_m2 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_12_m2 = raw_tmp_ident_5d2_idt_vin_12_m2 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_12_m2;
    if (raw_ident_5d2_idt_vin_12_m2 > 0xFFULL) raw_ident_5d2_idt_vin_12_m2 = 0xFFULL;
    writeBitsLE(frame.data, 16, 8, (uint32_t)raw_ident_5d2_idt_vin_12_m2);
    // IDT_Geheimnis_3 m0
    int64_t raw_tmp_ident_5d2_idt_geheimnis_3_m0 = (int64_t)llround((params::vw_pq.ident_5d2_idt_geheimnis_3_m0 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_geheimnis_3_m0 = raw_tmp_ident_5d2_idt_geheimnis_3_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_geheimnis_3_m0;
    if (raw_ident_5d2_idt_geheimnis_3_m0 > 0xFFULL) raw_ident_5d2_idt_geheimnis_3_m0 = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_ident_5d2_idt_geheimnis_3_m0);
    // IDT_VIN_6 m1
    int64_t raw_tmp_ident_5d2_idt_vin_6_m1 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_6_m1 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_6_m1 = raw_tmp_ident_5d2_idt_vin_6_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_6_m1;
    if (raw_ident_5d2_idt_vin_6_m1 > 0xFFULL) raw_ident_5d2_idt_vin_6_m1 = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_ident_5d2_idt_vin_6_m1);
    // IDT_VIN_13 m2
    int64_t raw_tmp_ident_5d2_idt_vin_13_m2 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_13_m2 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_13_m2 = raw_tmp_ident_5d2_idt_vin_13_m2 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_13_m2;
    if (raw_ident_5d2_idt_vin_13_m2 > 0xFFULL) raw_ident_5d2_idt_vin_13_m2 = 0xFFULL;
    writeBitsLE(frame.data, 24, 8, (uint32_t)raw_ident_5d2_idt_vin_13_m2);
    // IDT_Geheimnis_4 m0
    int64_t raw_tmp_ident_5d2_idt_geheimnis_4_m0 = (int64_t)llround((params::vw_pq.ident_5d2_idt_geheimnis_4_m0 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_geheimnis_4_m0 = raw_tmp_ident_5d2_idt_geheimnis_4_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_geheimnis_4_m0;
    if (raw_ident_5d2_idt_geheimnis_4_m0 > 0xFFULL) raw_ident_5d2_idt_geheimnis_4_m0 = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_ident_5d2_idt_geheimnis_4_m0);
    // IDT_VIN_7 m1
    int64_t raw_tmp_ident_5d2_idt_vin_7_m1 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_7_m1 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_7_m1 = raw_tmp_ident_5d2_idt_vin_7_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_7_m1;
    if (raw_ident_5d2_idt_vin_7_m1 > 0xFFULL) raw_ident_5d2_idt_vin_7_m1 = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_ident_5d2_idt_vin_7_m1);
    // IDT_VIN_14 m2
    int64_t raw_tmp_ident_5d2_idt_vin_14_m2 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_14_m2 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_14_m2 = raw_tmp_ident_5d2_idt_vin_14_m2 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_14_m2;
    if (raw_ident_5d2_idt_vin_14_m2 > 0xFFULL) raw_ident_5d2_idt_vin_14_m2 = 0xFFULL;
    writeBitsLE(frame.data, 32, 8, (uint32_t)raw_ident_5d2_idt_vin_14_m2);
    // IDT_VIN_1 m0
    int64_t raw_tmp_ident_5d2_idt_vin_1_m0 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_1_m0 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_1_m0 = raw_tmp_ident_5d2_idt_vin_1_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_1_m0;
    if (raw_ident_5d2_idt_vin_1_m0 > 0xFFULL) raw_ident_5d2_idt_vin_1_m0 = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_ident_5d2_idt_vin_1_m0);
    // IDT_VIN_8 m1
    int64_t raw_tmp_ident_5d2_idt_vin_8_m1 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_8_m1 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_8_m1 = raw_tmp_ident_5d2_idt_vin_8_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_8_m1;
    if (raw_ident_5d2_idt_vin_8_m1 > 0xFFULL) raw_ident_5d2_idt_vin_8_m1 = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_ident_5d2_idt_vin_8_m1);
    // IDT_VIN_15 m2
    int64_t raw_tmp_ident_5d2_idt_vin_15_m2 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_15_m2 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_15_m2 = raw_tmp_ident_5d2_idt_vin_15_m2 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_15_m2;
    if (raw_ident_5d2_idt_vin_15_m2 > 0xFFULL) raw_ident_5d2_idt_vin_15_m2 = 0xFFULL;
    writeBitsLE(frame.data, 40, 8, (uint32_t)raw_ident_5d2_idt_vin_15_m2);
    // IDT_VIN_2 m0
    int64_t raw_tmp_ident_5d2_idt_vin_2_m0 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_2_m0 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_2_m0 = raw_tmp_ident_5d2_idt_vin_2_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_2_m0;
    if (raw_ident_5d2_idt_vin_2_m0 > 0xFFULL) raw_ident_5d2_idt_vin_2_m0 = 0xFFULL;
    writeBitsLE(frame.data, 48, 8, (uint32_t)raw_ident_5d2_idt_vin_2_m0);
    // IDT_VIN_9 m1
    int64_t raw_tmp_ident_5d2_idt_vin_9_m1 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_9_m1 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_9_m1 = raw_tmp_ident_5d2_idt_vin_9_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_9_m1;
    if (raw_ident_5d2_idt_vin_9_m1 > 0xFFULL) raw_ident_5d2_idt_vin_9_m1 = 0xFFULL;
    writeBitsLE(frame.data, 48, 8, (uint32_t)raw_ident_5d2_idt_vin_9_m1);
    // IDT_VIN_16 m2
    int64_t raw_tmp_ident_5d2_idt_vin_16_m2 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_16_m2 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_16_m2 = raw_tmp_ident_5d2_idt_vin_16_m2 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_16_m2;
    if (raw_ident_5d2_idt_vin_16_m2 > 0xFFULL) raw_ident_5d2_idt_vin_16_m2 = 0xFFULL;
    writeBitsLE(frame.data, 48, 8, (uint32_t)raw_ident_5d2_idt_vin_16_m2);
    // IDT_VIN_3 m0
    int64_t raw_tmp_ident_5d2_idt_vin_3_m0 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_3_m0 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_3_m0 = raw_tmp_ident_5d2_idt_vin_3_m0 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_3_m0;
    if (raw_ident_5d2_idt_vin_3_m0 > 0xFFULL) raw_ident_5d2_idt_vin_3_m0 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_ident_5d2_idt_vin_3_m0);
    // IDT_VIN_10 m1
    int64_t raw_tmp_ident_5d2_idt_vin_10_m1 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_10_m1 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_10_m1 = raw_tmp_ident_5d2_idt_vin_10_m1 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_10_m1;
    if (raw_ident_5d2_idt_vin_10_m1 > 0xFFULL) raw_ident_5d2_idt_vin_10_m1 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_ident_5d2_idt_vin_10_m1);
    // IDT_VIN_17 m2
    int64_t raw_tmp_ident_5d2_idt_vin_17_m2 = (int64_t)llround((params::vw_pq.ident_5d2_idt_vin_17_m2 - 0.0f) / 1.0f);
    uint64_t raw_ident_5d2_idt_vin_17_m2 = raw_tmp_ident_5d2_idt_vin_17_m2 < 0 ? 0ULL : (uint64_t)raw_tmp_ident_5d2_idt_vin_17_m2;
    if (raw_ident_5d2_idt_vin_17_m2 > 0xFFULL) raw_ident_5d2_idt_vin_17_m2 = 0xFFULL;
    writeBitsLE(frame.data, 56, 8, (uint32_t)raw_ident_5d2_idt_vin_17_m2);
}

// Message 0x7D0 (2000) - Diagnose_1
// DLC: 8, TX: XXX
bool decodeVwPq0x7D0(const CANMessage &frame) {
    if (frame.id != 0x7D0 || frame.len < 8) {
        return false;
    }

    // DI1_VerlernZaehl: start=0 len=8 endian=1 sign=+ factor=1 offset=0 unit=""
    const uint32_t raw_diagnose_1_7d0_di1_verlernzaehl = readBitsLE(frame.data, 0, 8);
    params::vw_pq.diagnose_1_7d0_di1_verlernzaehl = raw_diagnose_1_7d0_di1_verlernzaehl * 1.0f + 0.0f;
    // DI1_km_Stand: start=8 len=20 endian=1 sign=+ factor=1 offset=0 unit="Unit_KiloMeter"
    const uint32_t raw_diagnose_1_7d0_di1_km_stand = readBitsLE(frame.data, 8, 20);
    params::vw_pq.diagnose_1_7d0_di1_km_stand = raw_diagnose_1_7d0_di1_km_stand * 1.0f + 0.0f;
    // DI1_Jahr: start=28 len=7 endian=1 sign=+ factor=1 offset=2000 unit="Unit_Year"
    const uint32_t raw_diagnose_1_7d0_di1_jahr = readBitsLE(frame.data, 28, 7);
    params::vw_pq.diagnose_1_7d0_di1_jahr = raw_diagnose_1_7d0_di1_jahr * 1.0f + 2000.0f;
    // DI1_Monat: start=35 len=4 endian=1 sign=+ factor=1 offset=0 unit="Unit_Month"
    const uint32_t raw_diagnose_1_7d0_di1_monat = readBitsLE(frame.data, 35, 4);
    params::vw_pq.diagnose_1_7d0_di1_monat = raw_diagnose_1_7d0_di1_monat * 1.0f + 0.0f;
    // DI1_Tag: start=39 len=5 endian=1 sign=+ factor=1 offset=0 unit="Unit_Day"
    const uint32_t raw_diagnose_1_7d0_di1_tag = readBitsLE(frame.data, 39, 5);
    params::vw_pq.diagnose_1_7d0_di1_tag = raw_diagnose_1_7d0_di1_tag * 1.0f + 0.0f;
    // DI1_Stunde: start=44 len=5 endian=1 sign=+ factor=1 offset=0 unit="Unit_Hours"
    const uint32_t raw_diagnose_1_7d0_di1_stunde = readBitsLE(frame.data, 44, 5);
    params::vw_pq.diagnose_1_7d0_di1_stunde = raw_diagnose_1_7d0_di1_stunde * 1.0f + 0.0f;
    // DI1_Minute: start=49 len=6 endian=1 sign=+ factor=1 offset=0 unit="Unit_Minut"
    const uint32_t raw_diagnose_1_7d0_di1_minute = readBitsLE(frame.data, 49, 6);
    params::vw_pq.diagnose_1_7d0_di1_minute = raw_diagnose_1_7d0_di1_minute * 1.0f + 0.0f;
    // DI1_Sekunde: start=55 len=6 endian=1 sign=+ factor=1 offset=0 unit="Unit_Secon"
    const uint32_t raw_diagnose_1_7d0_di1_sekunde = readBitsLE(frame.data, 55, 6);
    params::vw_pq.diagnose_1_7d0_di1_sekunde = raw_diagnose_1_7d0_di1_sekunde * 1.0f + 0.0f;
    // DI1_KM_Stand_alt: start=62 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.diagnose_1_7d0_di1_km_stand_alt = readBitsLE(frame.data, 62, 1) != 0;
    // DI1_Zeit_alt: start=63 len=1 endian=1 sign=+ factor=1 offset=0 unit=""
    params::vw_pq.diagnose_1_7d0_di1_zeit_alt = readBitsLE(frame.data, 63, 1) != 0;
    return true;
}

void encodeVwPq0x7D0(CANMessage &frame) {
    frame.id = 0x7D0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // DI1_VerlernZaehl
    int64_t raw_tmp_diagnose_1_7d0_di1_verlernzaehl = (int64_t)llround((params::vw_pq.diagnose_1_7d0_di1_verlernzaehl - 0.0f) / 1.0f);
    uint64_t raw_diagnose_1_7d0_di1_verlernzaehl = raw_tmp_diagnose_1_7d0_di1_verlernzaehl < 0 ? 0ULL : (uint64_t)raw_tmp_diagnose_1_7d0_di1_verlernzaehl;
    if (raw_diagnose_1_7d0_di1_verlernzaehl > 0xFFULL) raw_diagnose_1_7d0_di1_verlernzaehl = 0xFFULL;
    writeBitsLE(frame.data, 0, 8, (uint32_t)raw_diagnose_1_7d0_di1_verlernzaehl);
    // DI1_km_Stand
    int64_t raw_tmp_diagnose_1_7d0_di1_km_stand = (int64_t)llround((params::vw_pq.diagnose_1_7d0_di1_km_stand - 0.0f) / 1.0f);
    uint64_t raw_diagnose_1_7d0_di1_km_stand = raw_tmp_diagnose_1_7d0_di1_km_stand < 0 ? 0ULL : (uint64_t)raw_tmp_diagnose_1_7d0_di1_km_stand;
    if (raw_diagnose_1_7d0_di1_km_stand > 0xFFFFFULL) raw_diagnose_1_7d0_di1_km_stand = 0xFFFFFULL;
    writeBitsLE(frame.data, 8, 20, (uint32_t)raw_diagnose_1_7d0_di1_km_stand);
    // DI1_Jahr
    int64_t raw_tmp_diagnose_1_7d0_di1_jahr = (int64_t)llround((params::vw_pq.diagnose_1_7d0_di1_jahr - 2000.0f) / 1.0f);
    uint64_t raw_diagnose_1_7d0_di1_jahr = raw_tmp_diagnose_1_7d0_di1_jahr < 0 ? 0ULL : (uint64_t)raw_tmp_diagnose_1_7d0_di1_jahr;
    if (raw_diagnose_1_7d0_di1_jahr > 0x7FULL) raw_diagnose_1_7d0_di1_jahr = 0x7FULL;
    writeBitsLE(frame.data, 28, 7, (uint32_t)raw_diagnose_1_7d0_di1_jahr);
    // DI1_Monat
    int64_t raw_tmp_diagnose_1_7d0_di1_monat = (int64_t)llround((params::vw_pq.diagnose_1_7d0_di1_monat - 0.0f) / 1.0f);
    uint64_t raw_diagnose_1_7d0_di1_monat = raw_tmp_diagnose_1_7d0_di1_monat < 0 ? 0ULL : (uint64_t)raw_tmp_diagnose_1_7d0_di1_monat;
    if (raw_diagnose_1_7d0_di1_monat > 0xFULL) raw_diagnose_1_7d0_di1_monat = 0xFULL;
    writeBitsLE(frame.data, 35, 4, (uint32_t)raw_diagnose_1_7d0_di1_monat);
    // DI1_Tag
    int64_t raw_tmp_diagnose_1_7d0_di1_tag = (int64_t)llround((params::vw_pq.diagnose_1_7d0_di1_tag - 0.0f) / 1.0f);
    uint64_t raw_diagnose_1_7d0_di1_tag = raw_tmp_diagnose_1_7d0_di1_tag < 0 ? 0ULL : (uint64_t)raw_tmp_diagnose_1_7d0_di1_tag;
    if (raw_diagnose_1_7d0_di1_tag > 0x1FULL) raw_diagnose_1_7d0_di1_tag = 0x1FULL;
    writeBitsLE(frame.data, 39, 5, (uint32_t)raw_diagnose_1_7d0_di1_tag);
    // DI1_Stunde
    int64_t raw_tmp_diagnose_1_7d0_di1_stunde = (int64_t)llround((params::vw_pq.diagnose_1_7d0_di1_stunde - 0.0f) / 1.0f);
    uint64_t raw_diagnose_1_7d0_di1_stunde = raw_tmp_diagnose_1_7d0_di1_stunde < 0 ? 0ULL : (uint64_t)raw_tmp_diagnose_1_7d0_di1_stunde;
    if (raw_diagnose_1_7d0_di1_stunde > 0x1FULL) raw_diagnose_1_7d0_di1_stunde = 0x1FULL;
    writeBitsLE(frame.data, 44, 5, (uint32_t)raw_diagnose_1_7d0_di1_stunde);
    // DI1_Minute
    int64_t raw_tmp_diagnose_1_7d0_di1_minute = (int64_t)llround((params::vw_pq.diagnose_1_7d0_di1_minute - 0.0f) / 1.0f);
    uint64_t raw_diagnose_1_7d0_di1_minute = raw_tmp_diagnose_1_7d0_di1_minute < 0 ? 0ULL : (uint64_t)raw_tmp_diagnose_1_7d0_di1_minute;
    if (raw_diagnose_1_7d0_di1_minute > 0x3FULL) raw_diagnose_1_7d0_di1_minute = 0x3FULL;
    writeBitsLE(frame.data, 49, 6, (uint32_t)raw_diagnose_1_7d0_di1_minute);
    // DI1_Sekunde
    int64_t raw_tmp_diagnose_1_7d0_di1_sekunde = (int64_t)llround((params::vw_pq.diagnose_1_7d0_di1_sekunde - 0.0f) / 1.0f);
    uint64_t raw_diagnose_1_7d0_di1_sekunde = raw_tmp_diagnose_1_7d0_di1_sekunde < 0 ? 0ULL : (uint64_t)raw_tmp_diagnose_1_7d0_di1_sekunde;
    if (raw_diagnose_1_7d0_di1_sekunde > 0x3FULL) raw_diagnose_1_7d0_di1_sekunde = 0x3FULL;
    writeBitsLE(frame.data, 55, 6, (uint32_t)raw_diagnose_1_7d0_di1_sekunde);
    writeBitsLE(frame.data, 62, 1, params::vw_pq.diagnose_1_7d0_di1_km_stand_alt ? 1U : 0U);
    writeBitsLE(frame.data, 63, 1, params::vw_pq.diagnose_1_7d0_di1_zeit_alt ? 1U : 0U);
}



bool decodeVwPq(const CANMessage &frame) {
    switch (frame.id) {
        case 0x572: return decodeVwPq0x572(frame);
        case 0x538: return decodeVwPq0x538(frame);
        case 0x5B8: return decodeVwPq0x5B8(frame);
        case 0x448: return decodeVwPq0x448(frame);
        case 0x5D8: return decodeVwPq0x5D8(frame);
        case 0x5D0: return decodeVwPq0x5D0(frame);
        case 0x10C: return decodeVwPq0x10C(frame);
        case 0x534: return decodeVwPq0x534(frame);
        case 0x712: return decodeVwPq0x712(frame);
        case 0x512: return decodeVwPq0x512(frame);
        case 0x112: return decodeVwPq0x112(frame);
        case 0x514: return decodeVwPq0x514(frame);
        case 0x590: return decodeVwPq0x590(frame);
        case 0x530: return decodeVwPq0x530(frame);
        case 0x700: return decodeVwPq0x700(frame);
        case 0x500: return decodeVwPq0x500(frame);
        case 0x100: return decodeVwPq0x100(frame);
        case 0x704: return decodeVwPq0x704(frame);
        case 0x384: return decodeVwPq0x384(frame);
        case 0x580: return decodeVwPq0x580(frame);
        case 0x588: return decodeVwPq0x588(frame);
        case 0x488: return decodeVwPq0x488(frame);
        case 0x480: return decodeVwPq0x480(frame);
        case 0x380: return decodeVwPq0x380(frame);
        case 0x288: return decodeVwPq0x288(frame);
        case 0x280: return decodeVwPq0x280(frame);
        case 0x106: return decodeVwPq0x106(frame);
        case 0x104: return decodeVwPq0x104(frame);
        case 0x102: return decodeVwPq0x102(frame);
        case 0x7C0: return decodeVwPq0x7C0(frame);
        case 0x7C2: return decodeVwPq0x7C2(frame);
        case 0xC0: return decodeVwPq0xC0(frame);
        case 0xC4: return decodeVwPq0xC4(frame);
        case 0xC2: return decodeVwPq0xC2(frame);
        case 0x5DE: return decodeVwPq0x5DE(frame);
        case 0x3D0: return decodeVwPq0x3D0(frame);
        case 0x520: return decodeVwPq0x520(frame);
        case 0x420: return decodeVwPq0x420(frame);
        case 0x320: return decodeVwPq0x320(frame);
        case 0x5E0: return decodeVwPq0x5E0(frame);
        case 0x38A: return decodeVwPq0x38A(frame);
        case 0x388: return decodeVwPq0x388(frame);
        case 0x548: return decodeVwPq0x548(frame);
        case 0x540: return decodeVwPq0x540(frame);
        case 0x440: return decodeVwPq0x440(frame);
        case 0x390: return decodeVwPq0x390(frame);
        case 0x53C: return decodeVwPq0x53C(frame);
        case 0x5C0: return decodeVwPq0x5C0(frame);
        case 0x52E: return decodeVwPq0x52E(frame);
        case 0x598: return decodeVwPq0x598(frame);
        case 0x570: return decodeVwPq0x570(frame);
        case 0x470: return decodeVwPq0x470(frame);
        case 0x1A8: return decodeVwPq0x1A8(frame);
        case 0x4A8: return decodeVwPq0x4A8(frame);
        case 0x2A0: return decodeVwPq0x2A0(frame);
        case 0x4A0: return decodeVwPq0x4A0(frame);
        case 0x5A0: return decodeVwPq0x5A0(frame);
        case 0x1A0: return decodeVwPq0x1A0(frame);
        case 0x2A8: return decodeVwPq0x2A8(frame);
        case 0x578: return decodeVwPq0x578(frame);
        case 0x2C0: return decodeVwPq0x2C0(frame);
        case 0x550: return decodeVwPq0x550(frame);
        case 0x50: return decodeVwPq0x50(frame);
        case 0x360: return decodeVwPq0x360(frame);
        case 0x260: return decodeVwPq0x260(frame);
        case 0x52C: return decodeVwPq0x52C(frame);
        case 0x60E: return decodeVwPq0x60E(frame);
        case 0x368: return decodeVwPq0x368(frame);
        case 0x56A: return decodeVwPq0x56A(frame);
        case 0xD0: return decodeVwPq0xD0(frame);
        case 0x3D2: return decodeVwPq0x3D2(frame);
        case 0x3D4: return decodeVwPq0x3D4(frame);
        case 0xD2: return decodeVwPq0xD2(frame);
        case 0x284: return decodeVwPq0x284(frame);
        case 0x366: return decodeVwPq0x366(frame);
        case 0x5BE: return decodeVwPq0x5BE(frame);
        case 0x1AC: return decodeVwPq0x1AC(frame);
        case 0x3A0: return decodeVwPq0x3A0(frame);
        case 0x343: return decodeVwPq0x343(frame);
        case 0x392: return decodeVwPq0x392(frame);
        case 0x3BA: return decodeVwPq0x3BA(frame);
        case 0x497: return decodeVwPq0x497(frame);
        case 0x5B7: return decodeVwPq0x5B7(frame);
        case 0x5DC: return decodeVwPq0x5DC(frame);
        case 0x5D2: return decodeVwPq0x5D2(frame);
        case 0x7D0: return decodeVwPq0x7D0(frame);
        default: return false;
    }
}

