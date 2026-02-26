# VW PQ Signal Reference

Source: documentation/vw_pq.dbc

- Total messages: **86**
- Total signals: **1331**

## CAN ID 0x572 - ZAS_1

- DLC: 2  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Fehlerspeichereintrag__ZAS_ | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_ZAS_1_3 | 8 | 7 | Intel | U7 | 1 | 0 | - | - | - |
| Frei_ZAS_1_2 | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Klemme_15_SV | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_ZAS_1_1 | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Klemme_P__Parklichtstellung_ | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Klemme_50__Starten_ | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Klemme_X__Startvorgang_ | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Klemme_15__Z_ndung_ein_ | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Kontakt__Schl_ssel_steckt_ | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x538 - Wischer_1

- DLC: 2  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Blockierung_Heckwischer_erkannt | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Wischer_1_2 | 12 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| Fehlerspeichereintrag__Wischer_ | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ansteuerung_Scheibenwischer_Hec | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ansteuerung_Wascher_Heck | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Scheibenwischer_Heck_eingeschal | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Blockierung_Frontwischer_erkann | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Wischer_1_1 | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Status_Waschduesenheizung | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Parklage_Frontwischer | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ansteuerung_Frontwischer_Schnel | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ansteuerung_Frontwischer_Normal | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ansteuerung_Wascher_Front | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frontwischer__eingeschaltet | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x5B8 - WFS_1

- DLC: 2  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| WFS_Textbits | 8 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Frei_WFS_1_1 | 1 | 7 | Intel | U7 | 1 | 0 | - | - | - |
| WFS_LED | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x448 - Waehlhebel_1

- DLC: 4  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Frei_Waehlhebel_1_1 | 29 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| Test_aktiv_Flag | 28 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Zaehler_Waehlhebel_1 | 24 | 4 | Intel | U4 | 1 | 0 | - | Counter Waehlhebel_1 | - |
| Waehlhebel_Testergebnis | 16 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Fehler_Waehlhebel | 8 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Waehlhebelposition | 4 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Waehlhebel_Initialisierung | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Shiftlock_Position | 0 | 3 | Intel | U3 | 1 | 0 | - | - | - |

## CAN ID 0x5D8 - Verbauliste_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| TV_Tuner | 63 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| DSP | 62 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| CD_Wechsler | 61 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Spracheingabe | 60 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Telematik | 59 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Navigation | 58 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Telefon | 57 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Radio | 56 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| MMI_vorne | 55 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| MMI_hinten | 54 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Verbauliste_1_11 | 53 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Klimabedienteil_HL | 52 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Verbauliste_1_10 | 50 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Tankgeber | 49 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Assistenzfahrlicht | 48 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Memory_hinter_Fahrer | 47 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Sitzmemory_hinten | 46 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Sitzmemory_Beifahrer | 45 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Anh_ngersteuergeraet | 44 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Energiemanagement | 43 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Wischermodul | 42 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| EZS___Kessy__Komfort_ | 41 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Verdecksteuergeraet | 40 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Standheizung | 39 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Verbauliste_1_9 | 38 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Einparkhilfe | 37 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Klimasteuergeraet_Komfort | 36 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Gateway | 35 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenksaeulenmodul | 34 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Reifendruck | 33 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombiinstrument_Komfort | 32 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Dachmodul | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Memory | 30 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| TSG_HR | 29 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| TSG_HL | 28 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| TSG_BT | 27 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| TSG_FT | 26 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ZKE | 25 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BSG_Komfort | 24 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Verbauliste_1_8 | 19 | 5 | Intel | U5 | 1 | 0 | - | - | - |
| Stabi_Entkopplung | 18 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Waehlhebel | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Batteriemanager | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Daempfer_SG | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Niveauregulierung | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| EZS___Kessy__Antrieb_ | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Bremsbooster | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfe | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombi_Antrieb | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Einspritzpumpe | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkwinkel | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Allrad | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BSG_Antrieb | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ADR | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Airbag | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Climatronic_Antrieb | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ABS | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Getriebesteuergeraet | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Motorsteuergeraet | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x5D0 - Systeminfo_1

- DLC: 6  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| CAN_Stand_4_1_Antrieb_Daten_Hau | 44 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| CAN_Stand_4_1_Antrieb_Daten_Neb | 40 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| CAN_Stand_4_1_Komfort_Daten_Hau | 36 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| CAN_Stand_4_1_Komfort_Daten_Neb | 32 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Frei_Systeminfo_1_6 | 30 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Viertuerer | 29 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Rechtslenker | 28 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fahrzeug_Index | 24 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Fahrzeug_Generation | 20 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Fahrzeug_Derivat | 16 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Fahrzeug_Marke_2 | 12 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Fahrzeug_Klasse | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Verbauinformation_gueltig | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| CAN_Infotainment_verbaut | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| CAN_Infotainment_in_Diagnose | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Sleep_CAN_Infotainment | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| CAN_Komfort_in_Diagnose | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Sleep_CAN_Komfort | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| CAN_Antrieb_in_Diagnose | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| CAN_Extern_zugeschaltet | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x10C - Slave_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Delta_reduziertes_Sollmoment | 48 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Delta_Drosselklappenwinkel | 32 | 16 | Intel | U16 | 1 | 0 | - | - | - |
| Frei_Slave_1_1 | 25 | 7 | Intel | U7 | 1 | 0 | - | - | - |
| Delta_Zuendwinkelausgabe | 24 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ubat_Freigabe_DVE__Slave_ | 23 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Drosselklappe_Sollwertbegrenzun | 22 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Einspritzverbot_lernen__Slave_ | 21 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Master_erkannt__Slave_ | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehler_Momentenausgabe | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehler_Getriebe_Moment | 18 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehler_Bremsenbotschaft | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Indiziertes_Istmoment__Slave_ | 0 | 16 | Intel | U16 | 0.0015259 | 0 | % | - | - |
| Timeout_Bremsenbotschaft | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x534 - Sitz_info

- DLC: 2  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Positionserkennung_Beifahrersit | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Beifahrersitz_im_vorderen_Dritt | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Positionserkennung_Fahrersitz_u | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fahrersitz_im_vorderen_Drittel | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Zaehler_Sitzinfo | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Checksumme_Sitzinfo | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |

## CAN ID 0x712 - PSG_3

- DLC: 2  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Selbsttestergebnis | 0 | 16 | Intel | U16 | 1 | 0 | - | - | - |

## CAN ID 0x512 - PSG_2

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| RAM_Inhalt_4 | 48 | 16 | Intel | U16 | 1 | 0 | - | - | - |
| RAM_Inhalt_3 | 32 | 16 | Intel | U16 | 1 | 0 | - | - | - |
| RAM_Inhalt_2 | 16 | 16 | Intel | U16 | 1 | 0 | - | - | - |
| RAM_Inhalt_1 | 0 | 16 | Intel | U16 | 1 | 0 | - | - | - |

## CAN ID 0x112 - PSG_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Pumpentemperatur__2_1_ | 48 | 16 | Intel | U16 | 0.0625 | 0 | K | - | - |
| Pumpentemperatur__3_2_2_ | 44 | 12 | Intel | U12 | 1 | 0 | - | - | - |
| Zylinderzaehler__3_2_2_ | 43 | 3 | Intel | U3 | 1 | 0 | Zaehler | - | - |
| Ansteuerdauer__3_2_2_ | 32 | 11 | Intel | U11 | 0.0469 | 0 | NW | - | - |
| Nockenwellendrehzahl__3_2_2_ | 20 | 12 | Intel | U12 | 4 | 0 | upm | - | - |
| Pumpen_Statuswort__3_2_2_ | 0 | 20 | Intel | U20 | 1 | 0 | - | - | - |

## CAN ID 0x514 - NOX_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| OBD_fuer_NOX | 56 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Frei_NOX1_4 | 53 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| IP2 | 52 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| IP1 | 51 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| IP0 | 50 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Sondenheizung_NOX | 49 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Offsetkorrektur_NOX | 48 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_NOX1_3 | 43 | 5 | Intel | U5 | 1 | 0 | - | - | - |
| Sauerstoff_binaer | 32 | 11 | Intel | U11 | 1 | -200 | mV | - | - |
| Frei_NOX1_2 | 27 | 5 | Intel | U5 | 1 | 0 | - | - | - |
| Sauerstoff_linear | 16 | 11 | Intel | U11 | 1 | 0 | 1000/Lambd | - | - |
| Frei_NOX1_1 | 11 | 5 | Intel | U5 | 1 | 0 | - | - | - |
| NOX_Signal | 0 | 11 | Intel | U11 | 1 | 0 | ppm | - | - |

## CAN ID 0x590 - Niveau_1

- DLC: 6  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Beladungszustand | 40 | 8 | Intel | U8 | 1 | 0 | Zuladung | - | - |
| Fehlerspeichereintrag__Niveau_1 | 39 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Systemstatus__Niveau_1_ | 38 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Reserve_Fahrzeugart | 37 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fahrzeugart_Niveau | 36 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Textbits__Niveau_1_ | 32 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Verstellung_HL | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Verstellung_HR | 30 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Verstellung_VL | 29 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Verstellung_VR | 28 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Absenkung_Fahrzeug | 27 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Anhebung_Fahrzeug | 26 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Verstellung_aktiv | 25 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kompressorlauf_in_Kuerze | 24 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Niveau_1_5 | 23 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Taster_Niveau | 22 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Parkniveau | 21 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Zwischenniveau | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Niveaustati | 16 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| MSG_Einschraenkung | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ESP_Beeinflussung | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Warnlampe_Niveau_1 | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Niveau_1_1 | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Zaehler_Niveau_1 | 8 | 4 | Intel | U4 | 1 | 0 | Zaehler | - | - |
| Checksumme_Niveau_1 | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |

## CAN ID 0x530 - Navigation_1

- DLC: 7  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Kreuzungstyp | 54 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Entfernung_bis_Kreuzung | 48 | 6 | Intel | U6 | 5 | 0 | m | - | - |
| Entfernung_bis_Kurvenanfang | 40 | 8 | Intel | U8 | 1 | 0 | m | - | - |
| Voarusliegende_Kurvenrichtung | 39 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Vorausliegender_Kurvenverlauf | 32 | 7 | Intel | U7 | 50 | 0 | m | - | - |
| Fehler_Navigation | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Anzahl_Fahrbahnen__0_ist_unguel | 28 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| Strassentyp | 24 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Laenderkennung | 16 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Vorzeichen_Gierrate______ | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Gierratenfehler | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Gierrate | 0 | 14 | Intel | U14 | 0.01 | 0 | deg/sek | - | - |

## CAN ID 0x700 - MSG_3

- DLC: 3  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| MSG_Konfiguration | 16 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Lage_des_OT_Impuls | 0 | 16 | Intel | U16 | 0.01172 | -384 | KW | - | - |

## CAN ID 0x500 - MSG_2

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| RAM_Adresse_4 | 48 | 16 | Intel | U16 | 1 | 0 | - | - | - |
| RAM_Adresse_3 | 32 | 16 | Intel | U16 | 1 | 0 | - | - | - |
| RAM_Adresse_2 | 16 | 16 | Intel | U16 | 1 | 0 | - | - | - |
| Ram_Adresse_1 | 0 | 16 | Intel | U16 | 1 | 0 | - | - | - |

## CAN ID 0x100 - MSG_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Kurbelwellendrehzahl__3_2_2_ | 56 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Soll_Foerderbeginn_KW__3_2_2_ | 40 | 16 | Intel | U16 | 0.01172 | -384 | KW | - | - |
| Soll_Foerderbeginn_NW__3_2_2_ | 28 | 12 | Intel | U12 | 0.01172 | 0 | degNW | - | - |
| Soll_Voreinspritzung | 16 | 12 | Intel | U12 | 1 | 0 | - | - | - |
| Soll_Einspritzmenge | 0 | 16 | Intel | U16 | 0.03125 | 0 | mg/H | - | - |

## CAN ID 0x704 - Motor_NOX

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Frei_Motor_NOX_1_2 | 24 | 40 | Intel | U40 | 1 | 0 | - | - | - |
| Frei_Motor_NOX_1_1 | 19 | 5 | Intel | U5 | 1 | 0 | - | - | - |
| Heizleistungsanforderung | 18 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Offsetkorrektur_moeglich | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Betriebsbereich | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Abgastemperatur_NOX | 8 | 8 | Intel | U8 | 5 | -40 | C | - | - |
| Abgasdruck_NOX | 0 | 8 | Intel | U8 | 5 | 600 | mbar | - | - |

## CAN ID 0x384 - Motor_Momente

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Momentenangaben_ungenau__Moment | 35 | 2 | Intel | U2 | 1 | 0 | - | - | - |

## CAN ID 0x580 - Motor_Flexia

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Ansaugsystem m0 | 63 | 1 | Intel | Bool | 1 | 0 | - | Induction System | - |
| Hubraum m0 | 56 | 7 | Intel | U7 | 0.1 | 0 | l | Displacement | - |
| Steigung_der_Befuellungskennlin m1 | 56 | 8 | Intel | U8 | 0.001 | 0 | l/mm | - | - |
| Anzahl_Zylinder m0 | 52 | 4 | Intel | U4 | 1 | 0 | Vent./Zyl. | Number of cylinders | - |
| Bewertungsfaktor_Russindex_Turb m1 | 50 | 6 | Intel | U6 | 0.1 | 0 | - | - | - |
| Anzahl_Ventile m0 | 49 | 3 | Intel | U3 | 1 | 0 | Vent./Zyl. | Number of valves | - |
| Bewertungsfaktor_Verschleissind m1 | 44 | 6 | Intel | U6 | 0.1 | 0 | - | - | - |
| Hersteller_Code m1 | 40 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Motorleistung m0 | 40 | 9 | Intel | U9 | 1 | 0 | KW | Maximum engine power | - |
| Max_Drehmoment m0 | 32 | 8 | Intel | U8 | 10 | 0 | Nm | Maximum torque | - |
| Normierter_Verbrauch m1 | 32 | 8 | Intel | U8 | 10 | 0 | l/Zyl. | - | - |
| Oelniveauschwelle m1 | 24 | 8 | Intel | U8 | 0.25 | 0 | cm | - | - |
| Drehzahl_MaxNorm m0 | 24 | 8 | Intel | U8 | 100 | 0 | U/min | RPM of maximum torque | - |
| Verschleissindex | 16 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Russindex | 8 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Verbrennungsart | 7 | 1 | Intel | Bool | 1 | 0 | - | Type of combustion | - |
| Frei_Motor_Flexia_1 | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Warm_Up_Cycle | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Driving_Cycle | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Zaehler_Motor_Flexia | 1 | 3 | Intel | U3 | 1 | 0 | - | Counter Motor_Flexia | - |
| Multiplex_Schalter_Motor_Flexia M | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x588 - Motor_7

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Oltemperatur | 56 | 8 | Intel | U8 | 1 | 0 | - | Oil temperature | - |
| Frei_Motor_7_3 | 40 | 16 | Intel | U16 | 1 | 0 | - | - | - |
| Ladedruck | 32 | 8 | Intel | U8 | 0.01 | 0 | bar | Boost Pressure | - |
| Vorzeichen_Motordrehzahlgradien | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Motordrehzahlgradient | 24 | 7 | Intel | U7 | 1 | 0 | U/min | Engine speed gradient | - |
| Hoeheninfo__Motor_7_ | 16 | 8 | Intel | U8 | 0.00787 | 0 | - | Altitude correction factor | - |
| Klemme_DFM | 8 | 8 | Intel | U8 | 0.4 | 0 | % | - | - |
| PTC___Gluehstifte_ausgeschaltet | 5 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| Frei_Motor_7_1 | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehlerspeichereintrag__Motor_7_ | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Statusbit_Geschwindikeitsbegren | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Geschwindigkegrenzung_aktivierb | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Leerlauf_Solldrehzahl_auf_Max_W | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x488 - Motor_6

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Zaehler_Motor_6 | 60 | 4 | Intel | U4 | 1 | 0 | - | Counter Motor_6 | - |
| Frei_Motor_6_4 | 58 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| ltemperaturschutz | 57 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Bremseingriff_Freigabe | 56 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Motor_6_3 | 48 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Ruckmeldung_Momenten | 40 | 8 | Intel | U8 | 0.39 | 0 | - | Feedback torque-integral gear intervention | - |
| GRA_Sollbeschleunigung | 32 | 8 | Intel | U8 | 0.024 | -3.984 | m/s2 | GRA target acceleration | - |
| Hoeheninfo__Motor_6_ | 24 | 8 | Intel | U8 | 0.00787 | 0 | - | Altitude Correction | - |
| Istmoment_f_r_Getriebe | 16 | 8 | Intel | U8 | 0.39 | 0 | MDI | Actual torque for gear | - |
| Sollmoment_f_r_Getriebe | 8 | 8 | Intel | U8 | 0.39 | 0 | MDI | Target torque for gearbox | - |
| Checksumme_Motor_6 | 0 | 8 | Intel | U8 | 1 | 0 | - | Checksum Motor_6 | - |

## CAN ID 0x480 - Motor_5

- DLC: 8  TX: Motor  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| MO5_Mp_Code M | 6 | 2 | Intel | U2 | 1 | 0 | - | Multiplex selector - switches every 4 transmissions | 0="Max_Moment", 1="Drehzahl", 2="Motor_Typ", 3="Abgastyp_od_norm_Verbrauch" |
| MO5_max_Moment m0 | 0 | 6 | Intel | U6 | 10 | 0 | Nm | Maximum torque when Mp_Code=0 (0-630 Nm) | - |
| MO5_Drehzahl m1 | 0 | 6 | Intel | U6 | 100 | 0 | U/min | Engine speed at max torque when Mp_Code=1 (0-6300 RPM) | - |
| MO5_Motortyp m2 | 0 | 6 | Intel | U6 | 1 | 0 | - | Engine type info when Mp_Code=2 (cylinder count, turbo, fuel type) | - |
| MO5_Abgastyp m3 | 0 | 1 | Intel | Bool | 1 | 0 | - | Exhaust type EOBD when Mp_Code=3 | 0="no_EOBD", 1="EOBD" |
| MO5_Abgastyp2 m3 | 1 | 1 | Intel | Bool | 1 | 0 | - | Exhaust type OBD when Mp_Code=3 | 0="no_OBD", 1="OBD" |
| MO5_Abgastyp3 m3 | 2 | 1 | Intel | Bool | 1 | 0 | - | DPF installed when Mp_Code=3 | 0="no_DPF_verbaut", 1="DPF_verbaut" |
| OBD_Kaltstart_Denominator m3 | 3 | 1 | Intel | Bool | 1 | 0 | - | Cold start denominator increment when Mp_Code=3 | 0="Denominator_not_hochzaehlen", 1="Bedingungen_zum_Inkr_des_Kaltstart_Denom_erf" |
| OBD_Minimum_Trip m3 | 4 | 1 | Intel | Bool | 1 | 0 | - | Minimum normalized trip when Mp_Code=3 | 0="no_Min_Normed_Trip", 1="Min_Normed_Trip" |
| MO5_DPF_reg m3 | 5 | 1 | Intel | Bool | 1 | 0 | - | DPF regenerating when Mp_Code=3 | 0="DPF_regeneriert_nicht", 1="DPF_regeneriert" |
| MO5_Vorgluehen | 9 | 1 | Intel | Bool | 1 | 0 | - | Diesel pre-glow lamp | 0="Lamp_off", 1="Lamp_on" |
| MO5_E_Gas | 10 | 1 | Intel | Bool | 1 | 0 | - | E-Gas system lamp (petrol only) | 0="Lamp_off", 1="Lamp_on" |
| MO5_OBD_2 | 11 | 1 | Intel | Bool | 1 | 0 | - | OBD/MIL lamp control | 0="Lamp_off", 1="Lamp_on" |
| MO5_Heissl | 12 | 1 | Intel | Bool | 1 | 0 | - | Coolant overheat warning lamp | 0="Lamp_off", 1="Lamp_on" |
| MO5_KlimaKompr | 13 | 1 | Intel | Bool | 1 | 0 | - | AC compressor off request | 0="no_Anforderung", 1="Klimakompr_aus" |
| MO5_Feld_kuehl | 14 | 1 | Intel | Bool | 1 | 0 | - | Map cooling installed and OK | 0="no", 1="yes" |
| MO5_KliKo_Red | 15 | 1 | Intel | Bool | 1 | 0 | - | AC compressor power reduction | 0="no", 1="yes" |
| MO5_Verbrauch | 16 | 15 | Intel | U15 | 1 | 0 | µl | Fuel consumption counter (15-bit, µl) | - |
| MO5_UeberlVerb | 31 | 1 | Intel | Bool | 1 | 0 | - | Consumption counter overflow | 0="no_Ueberlauf", 1="mindestens_einmal_uebergelaufen" |
| MO5_Luefter | 32 | 8 | Intel | U8 | 0.4 | 0 | % | Cooling fan PWM control (0-101.6%) | - |
| MO5_HLeuchte | 40 | 1 | Intel | Bool | 1 | 0 | - | Overheat pre-warning | 0="no_Warnung", 1="Vorwarnung" |
| MO5_PartikelLamp | 41 | 1 | Intel | Bool | 1 | 0 | - | Particle filter lamp | 0="Lamp_off", 1="Lamp_on" |
| MO5_Sta_BKU | 42 | 1 | Intel | Bool | 1 | 0 | - | Brake vacuum pump status (Porsche only) | 0="iO_oder_not_verbaut", 1="n_iO" |
| MO5_TypStartSteu | 43 | 1 | Intel | Bool | 1 | 0 | - | Starter control type: 0=BSG/BCM, 1=MSG | 0="Startersteuerung_BSG_BCM_KessyD1_ZAS", 1="Startersteuerung_durch_MSG" |
| MO5_TDE_Lampe | 44 | 1 | Intel | Bool | 1 | 0 | - | Fuel cap warning lamp (petrol only) | 0="Lamp_off", 1="Lamp_on" |
| MO5_TDE_Text | 45 | 1 | Intel | Bool | 1 | 0 | - | Fuel cap text display (petrol only) | 0="no_Text", 1="Textanzeige_nach_ISO" |
| MO5_DZM_Daempf | 46 | 1 | Intel | Bool | 1 | 0 | - | RPM gauge damping control | 0="normale_Daempfung", 1="dynamische_Daempfung" |
| MO5_Interlock | 47 | 1 | Intel | Bool | 1 | 0 | - | Interlock switch for manual transmission start | 0="Interlock_not_betaetigt", 1="Interlock_betaetigt" |
| MO5_Start | 48 | 1 | Intel | Bool | 1 | 0 | - | Automatic start permission | 0="Start_not_zulaessig", 1="Startfreigabe" |
| MO5_Anlasser | 49 | 1 | Intel | Bool | 1 | 0 | - | Starter control permission | 0="Anlasser_darf_angesteuert_werden", 1="Anlasser_ausspuren_Ansteuerung_not_moeglich" |
| MO5_GRA_Hauptsch | 50 | 1 | Intel | Bool | 1 | 0 | - | GRA main switch / readiness | 0="Off", 1="On" |
| MO5_Momente | 51 | 1 | Intel | Bool | 1 | 0 | - | MDI torque values doubled | 0="Einfach", 1="Doppelt" |
| MO5_Motortext1 | 52 | 1 | Intel | Bool | 1 | 0 | - | Text: Engine workshop | 0="no_Text", 1="Text_1" |
| MO5_Motortext2 | 53 | 1 | Intel | Bool | 1 | 0 | - | Text: Exhaust workshop (deprecated) | 0="no_Text", 1="Text_2" |
| MO5_Motortext3 | 54 | 1 | Intel | Bool | 1 | 0 | - | Text: Fuel system fault | 0="no_Text", 1="Text_3" |
| MO5_Motortext4 | 55 | 1 | Intel | Bool | 1 | 0 | - | Text: Particle filter needs regeneration | 0="no_Text", 1="Text_4" |
| CHECKSUM | 56 | 8 | Intel | U8 | 1 | 0 | - | Checksum with final value 0x0 | - |

## CAN ID 0x380 - Motor_3

- DLC: 8  TX: Motor  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| MO3_Vorgluehen | 0 | 1 | Intel | Bool | 1 | 0 | - | Pre-glow active (diesel engines) | 0="no_Vorgluehen", 1="Vorgluehen_active" |
| MO3_Prio_Dz | 1 | 1 | Intel | Bool | 1 | 0 | - | Engine speed priority: 0=request, 1=mandatory | 0="Wunsch", 1="Zwang" |
| MO_Schalter_StartStopp | 2 | 1 | Intel | Bool | 1 | 0 | - | Start/stop deactivated by driver (LED on when 0) | 0="StartStopp_ueber_Hauptschalter_deactivated_LED_an", 1="StartStopp_ueber_Hauptschalter_activated_LED_aus" |
| MO3_Winterprg | 3 | 1 | Intel | Bool | 1 | 0 | - | Winter driving program active | 0="not_active", 1="active" |
| MO3_Sta_Pedal | 4 | 1 | Intel | Bool | 1 | 0 | - | Accelerator pedal status: 0=OK, 1=substitute value | 0="Fahrpedal_iO", 1="Ersatzwert" |
| MO3_Sta_FPG M | 5 | 1 | Intel | Bool | 1 | 0 | - | Accelerator pedal gradient control bit (multiplexer for byte 8) | 0="Gradient_not_bed", 1="Gradient_bed" |
| MO3_Sign_FPG m1 | 6 | 1 | Intel | Bool | 1 | 0 | - | Accelerator pedal gradient sign (only when MO3_Sta_FPG=1) | 0="positives_Vorzeichen", 1="negatives_Vorzeichen" |
| MO3_Sta_Temp | 7 | 1 | Intel | Bool | 1 | 0 | - | Intake air temperature status: 0=OK, 1=substitute value | 0="Geber_iO", 1="Ersatzwert" |
| MO3_Offsentemp | 8 | 8 | Intel | U8 | 0.75 | -48 | °C | Intake air temperature | - |
| MO3_Pedalwert | 16 | 8 | Intel | U8 | 0.4 | 0 | % | Accelerator pedal raw signal (0-101.6%) | - |
| MO3_Rad_Wu_Mo | 24 | 12 | Intel | U12 | 0.39 | 0 | % | Wheel torque request (12-bit value, 0-1596.66% MDI) | - |
| MO3_Vorz_RadWu | 36 | 1 | Intel | Bool | 1 | 0 | - | Wheel torque sign: 0=positive, 1=negative | 0="positives_Vorzeichen", 1="negatives_Vorzeichen" |
| MO3_Freigabe_Segeln | 37 | 1 | Intel | Bool | 1 | 0 | - | Sailing operation enable for transmission | 0="Segelbetrieb_not_freigegeben", 1="Segelbetrieb_freigegeben" |
| MO_StartStopp_StoppVorbereitung | 38 | 1 | Intel | Bool | 1 | 0 | - | Engine stop in preparation | 0="Motorstopp_not_in_Vorbereitung", 1="Motorstopp_in_Vorbereitung" |
| MO3_Dz_Beeinfl | 40 | 8 | Intel | U8 | 0.39 | 0 | % | Engine speed influence factor (0-100% interpolation) | - |
| MO3_WunschDz | 48 | 8 | Intel | U8 | 25 | 0 | U/min | Desired engine speed (when MO3_Prio_Dz=1) | - |
| MO3_DKW m0 | 56 | 8 | Intel | U8 | 0.4 | 0 | % | Throttle valve position when MO3_Sta_FPG=0 (0-101.6%) | - |
| MO3_FPGradient m1 | 56 | 8 | Intel | U8 | 25 | 0 | %/s | Accelerator pedal gradient when MO3_Sta_FPG=1 (0-6350%/s) | - |

## CAN ID 0x288 - Motor_2

- DLC: 8  TX: Motor  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| MO2_Mp_Code m | 6 | 2 | Intel | U2 | 1 | 0 | - | Multiplex code - switches after 4 transmissions | 0="CAN_Stand", 1="Motor_Kod", 2="Getriebe_Kod", 3="MDI_Max" |
| MO2_Getr_Code m2 | 0 | 6 | Intel | U6 | 1 | 0 | - | Transmission code (multiplex value when Mp_Code=2) | 0="_5HP19", 1="_5HP24", 2="AG4", 3="VL300", 4="VQ250", 5="VQ35", 6="AG4_yestco", 7="AG5_yestco", 8="autom_Kupplung", 9="autom_Kupplung_mit_ASG", 10="AG6_yestco_ZF_6HP_AISIN", 11="DQ500_DQ250_DQ200", 12="SQ100_SQ200", 14="AL1000_AL551_AL951_ZF8HPx", 15="Handschalter_konv", 16="DL800", 17="E_Fahrzeug__Single_Gear" |
| MO2_max_Mo m3 | 0 | 6 | Intel | U6 | 10 | 0 | Nm | Maximum torque MDI (multiplex value when Mp_Code=3) | - |
| MO2_CAN_Vers m0 | 0 | 6 | Intel | U6 | 1 | 0 | - | CAN version (multiplex value when Mp_Code=0) | - |
| MO2_Motor_Code m1 | 0 | 6 | Intel | U6 | 1 | 0 | - | Motor code (multiplex value when Mp_Code=1) | - |
| MO2_Kuehlm_T | 8 | 8 | Intel | U8 | 0.75 | -48 | °C | Engine coolant temperature or substitute value | - |
| MO2_BLS | 16 | 1 | Intel | Bool | 1 | 0 | - | Brake light switch - unfiltered raw signal | 0="no_Bremsen", 1="Bremse_betaetigt" |
| MO2_BTS | 17 | 1 | Intel | Bool | 1 | 0 | - | Brake test switch | 0="no_braking", 1="Brake_activated" |
| MO2_Sta_Kuehlm | 18 | 1 | Intel | Bool | 1 | 0 | - | Coolant temperature status - 0=OK, 1=Not OK | 0="Temperature_ok", 1="Temperature_not_ok" |
| MO2_Sta_Klima | 19 | 1 | Intel | Bool | 1 | 0 | - | Air conditioning compressor status | 0="Climate_off", 1="Climate_on" |
| MO2_Sta_No_Bet | 20 | 1 | Intel | Bool | 1 | 0 | - | Normal operation status (Terminal 15 on, init complete) | 0="no_Normalbetrieb", 1="Normalbetrieb" |
| MO2_Status_TSK | 21 | 1 | Intel | Bool | 1 | 0 | - | Drive train coordinator status | 0="TSK_not_available", 1="TSK_available" |
| MO2_Sta_GRA | 22 | 2 | Intel | U2 | 1 | 0 | - | GRA/ACC status | 0="ADR_GRA_off", 1="gra_activated_lamp_on", 2="gra_overdriven_lamp_on", 3="ADR_gra_faulted" |
| TSK_Limiter_ausgewaehlt | 25 | 1 | Intel | Bool | 1 | 0 | - | Speed limiter selected | 0="no_limiter_selected", 1="Limiter_Selected" |
| MO2_GRAregelt | 26 | 1 | Intel | Bool | 1 | 0 | - | GRA regulating to displayed speed | 0="no", 1="yes" |
| MO2_Sport_Error | 27 | 1 | Intel | Bool | 1 | 0 | - | Sport mode error (Porsche Cayenne) | 0="no_Failure", 1="Failure" |
| MO2_OffRoad | 28 | 1 | Intel | Bool | 1 | 0 | - | OffRoad mode active | 0="not_active", 1="active" |
| MO2_RME_Gehalt | 29 | 3 | Intel | U3 | 12.5 | 0 | % | Rapeseed oil methyl ester content (diesel) | - |
| MO2_GRA_Soll | 32 | 8 | Intel | U8 | 1.28 | 0 | km/h | GRA target speed setpoint | - |
| MO2_LL_Solldz | 40 | 8 | Intel | U8 | 10 | 0 | 1/min | Idle target speed | - |
| MO2_Begr_Mo | 48 | 8 | Intel | U8 | 0.39 | 0 | % | Limiting torque - max possible at speed | - |
| MO2_Mo_ZWR | 56 | 8 | Intel | U8 | 0.39 | 0 | % | Minimum engine torque with ignition angle retardation | - |

## CAN ID 0x280 - Motor_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Fahrerwunschmoment | 56 | 8 | Intel | U8 | 0.39 | 0 | MDI | Driver Requested Torque | - |
| mechanisches_Motor_Verlustmomen | 48 | 8 | Intel | U8 | 0.39 | 0 | MDI | Mechanical Torque Loss | - |
| Fahrpedalwert_oder_Drosselklapp | 40 | 8 | Intel | U8 | 0.4 | 0 | % | Accelerator Pedal or Throttle Position | - |
| inneres_Motor_Moment_ohne_exter | 32 | 8 | Intel | U8 | 0.39 | 0 | MDI | Inner torque without external | - |
| Motordrehzahl | 16 | 16 | Intel | U16 | 0.25 | 0 | U/min | Engine Speed | - |
| inneres_Motor_Moment | 8 | 8 | Intel | U8 | 0.39 | 0 | MDI | Engine Indicated Torque | - |
| Momentenangaben_ungenau | 7 | 1 | Intel | Bool | 1 | 0 | - | Approximate Torque Values | - |
| Fehlerstatus_Getriebe_Momentene | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehlerstatus_Brems_Momenteneing | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Time_Out_Bremsen_Botschaft | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kupplungsschalter | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kickdownschalter | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fahrpedalwert_ungenau__Motor_1_ M | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Leergasinformation | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x106 - Master_3

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Frei_Master_3_1 | 56 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Motortemperatur_linearisiert | 48 | 8 | Intel | U8 | 0.75 | -48 | - | - | - |
| Indiziertes_Sollmoment_f_r_Vmax | 32 | 16 | Intel | U16 | 0.0015259 | 0 | % | - | - |
| Relative_Momentenanforderung_de | 16 | 16 | Intel | U16 | 0.003052 | 0 | % | - | - |
| Delta_Motormoment_aus_Verlustmo | 0 | 16 | Intel | U16 | 0.003052 | -100 | % | - | - |

## CAN ID 0x104 - Master_2

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Ubat_Freigabe_DVE | 45 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| DK_Sollwertbegrenzung | 44 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Einspritzverbot_DV_E_lernen | 43 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Master_erkannt | 42 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Vmax_Begrenzung_aktiv | 41 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| SA_Verbot_von_FGR | 40 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Pedalwert_Komplement | 24 | 16 | Intel | U16 | 1 | 0 | - | - | - |
| Zaehler__Master_2_ | 16 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Normierter_Fahrpedalwinkel | 0 | 16 | Intel | U16 | 0.001526 | 0 | % | - | - |

## CAN ID 0x102 - Master_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Stationaere_Solldrehzahl | 56 | 8 | Intel | U8 | 10 | 0 | Umin | - | - |
| Drehmoment_LLR__I_Anteil_ | 40 | 16 | Intel | U16 | 0.003052 | -100 | % | - | - |
| Drehmoment_LLR__PD_Anteil_ | 24 | 16 | Intel | U16 | 0.003052 | -100 | % | - | - |
| Drehmoment_LLR__Luftpfad_ | 8 | 16 | Intel | U16 | 0.003052 | -100 | % | - | - |
| Frei_Master_1_1 | 4 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| LLR_ist_aktiv | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| I_Anteil_der_LLR_aktiv | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PD_Anteil_der_LLR_aktiv | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LLR_Freigabe_nach_Start | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x7C0 - LWS_Kalibrierung

- DLC: 2  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Frei_LWS_Kalibrierung_1_1 | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LWS_Identifier | 8 | 7 | Intel | U7 | 1 | 0 | - | - | - |
| Frei_LWS_Kalibrierung_1_2 | 4 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Command_Dode_Word | 0 | 4 | Intel | U4 | 1 | 0 | - | - | - |

## CAN ID 0x7C2 - Lenkwinkel_Init

- DLC: 4  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Kodierbytes | 8 | 24 | Intel | U24 | 1 | 0 | - | - | - |
| Identiifier_f_r_LWS_Init | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |

## CAN ID 0x0C0 - Lenkwinkel_1__RB_

- DLC: 2  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Vorzeichen__RB_ | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkwinkel__RB_ | 5 | 10 | Intel | U10 | 2.5 | -720 | - | - | - |
| LWS_OK__RB_ | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LWS_Abgleich__RB_ | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Lenkwinkel_1_1__RB_ | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| FINE_CHECK__RB_ | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| COARSE_CHECK__RB_ | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x0C4 - Lenkwinkel_1__ITT_

- DLC: 2  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Vorzeichen__ITT_ | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkwinkel__ITT_ | 5 | 10 | Intel | U10 | 1.5 | -768 | - | - | - |
| LWS_OK | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LWS_Abgleich__ITT_ | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LWS_Initialisierung__ITT_ | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| FINE_CHECK__ITT_ | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| COARSE_CHECK__ITT_ | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x0C2 - Lenkwinkel_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| LW1_LRW | 0 | 15 | Intel | U15 | 0.04375 | 0 | °Grad | Steering wheel angle (0 = straight ahead, 7FFFh = stop) | - |
| LW1_LRW_Sign | 15 | 1 | Intel | Bool | 1 | 0 | - | Steering angle sign: 0=positive (left), 1=negative (right) | 0="positives_Vorzeichen", 1="negatives_Vorzeichen" |
| LW1_Lenk_Gesch | 16 | 15 | Intel | U15 | 0.04375 | 0 | °Grad/s | Steering wheel angular velocity (0 = no movement, 7FFFh = max speed) | - |
| LW1_Gesch_Sign | 31 | 1 | Intel | Bool | 1 | 0 | - | Angular velocity sign: 0=positive, 1=negative | 0="positives_Vorzeichen", 1="negatives_Vorzeichen" |
| LW1_ID | 32 | 8 | Intel | U8 | 1 | 0 | - | Calibration ID - 0=not calibrated, 128=calibrated (PQ35/46/VW32x/VW411) | 0="noch_not_kalibriert", 128="kalibriert_nur_bei_PQ35_46_VW32x_VW411" |
| LW1_Initquelle | 40 | 1 | Intel | Bool | 1 | 0 | - | Initialization source | 0="Bremse_3", 1="EPS_Bit" |
| LW1_Status | 41 | 2 | Intel | U2 | 1 | 0 | - | Sensor status | 0="OK", 1="no_initial", 2="sporadic_failure", 3="permanent_failure" |
| LW1_Sta_KL30 | 43 | 1 | Intel | Bool | 1 | 0 | - | Terminal 30 status (Kostal sensor) | 0="ok", 1="no_init_s" |
| LW1_Zaehler | 44 | 4 | Intel | U4 | 1 | 0 | - | Free running message counter | - |
| LW1_Kodier | 48 | 8 | Intel | U8 | 1 | 0 | - | Coding data (multiplex output depends on counter) | - |
| LW1_CRC | 56 | 8 | Intel | U8 | 1 | 0 | - | Checksum - One's complement of 8-bit sum of bytes 1,2,3,4,6 | - |

## CAN ID 0x5DE - Lenkhilfe_Fehler

- DLC: 7  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Werkstattcode | 48 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Multiplex_Signal | 45 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| Sensorcodierung_Lenkhilfe | 44 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kennliniencodierung_Lenkhilfe | 40 | 4 | Intel | U4 | 1 | 0 | Kennlinie | - | - |
| Geber_f__Lenkw__Speicher_ausles | 39 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombiinstr__Speicher_auslesen | 38 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Steuerger__Speicher_auslesen | 37 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfe_Steuergeraet_defekt | 36 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Lenkhilfe_1_3 | 32 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Frei_Lenkhilfe_1_4 | 30 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Geber_f__Lenkwi__k__CAN_Komm_sp | 29 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Geber_f__Lenkwinkel_k__CAN_Komm | 28 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombiinstr__k__CAN_Kommunik__sp | 27 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombiinstrument_k__CAN_Kommunik | 26 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Steuergeraet_k__CAN_Kommunik__S | 25 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Steuergeraet_keine_CAN_Kommunik | 24 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfe_Spannung_Ks__nach_Mas | 23 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfe_Spannung_Ks_nach_Mass | 22 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfe_Spannung_Ks__nach___s | 21 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfe_Spannung_Ks__nach__ | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfe_Temperaturschutz_sp_ | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfe_Temperaturschutz | 18 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfe_Betrieb_unpl__Sig__sp | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfe_Betrieb_unpl__Signal | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfesensor_k__Kommunik__sp | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfesensor_keine_Kommunik_ | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfesensor_defekt_sp_ | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfesensor_defekt | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfesensor_Unterbrechung_s | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfesensor_Unterbrechung | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhilfesensor_Ks_nach_Masse_s | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkhifesensor_Ks_nach_Masse | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Spannung_Kl_15_zu_klein_sp_ | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Spannung_Kl_15_zu_klein | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Lenkhilfe_1_5 | 4 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Spannung_Kl_30_zu_klein_sp_ | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Spannung_Kl_30_zu_klein | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Spannung_Kl_30_zu_gro__sp_ | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Spannung_Kl_30_zu_gro_ | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x3D0 - Lenkhilfe_1

- DLC: 2  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| LH1_Lastinfo | 0 | 7 | Intel | U7 | 1 | 0 | A | - | - |
| LH1_Fehler_LI | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_Sicherheitslamp | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_Fehlerlampe | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_Textbits | 10 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| LH1_Akustiksign | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_SleepInd | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_Fehlereintr | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_EPS_Diagmode | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_gue_ECU_Temp | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_Kuehlung | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_Mode_Hybrid | 21 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_NL_Untersp | 24 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_NL_Uebersp | 25 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_NL_Uebertemp | 26 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_NL_Sensor | 27 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_NL_CAN_SS | 28 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_NL_ECU | 29 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_NL_Motor | 30 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_NL_LeistDichte | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH1_ULeistung | 32 | 8 | Intel | U8 | 0.5 | 0 | % | - | - |
| LH1_ECU_Temp | 40 | 8 | Intel | U8 | 1 | -70 | °C | - | - |

## CAN ID 0x520 - Kombi_3

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Frei_Kombi_3_2 | 60 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Kilometerstand | 40 | 20 | Intel | U20 | 1 | 0 | km | - | - |
| Fehlerstatus_Standzeit | 39 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Standzeit | 24 | 15 | Intel | U15 | 4 | 0 | sec | - | - |
| Frei_Kombi_3_1 | 20 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Schluesselinfo | 16 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Kombi_Multiplex_Code M | 14 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Kombi_Verbauliste_Niveauregulie m3 | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombi_Multiplex_Marke m1 | 11 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| Kombi_Verbauliste_Lenkhilfe m3 | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombi_Verbauliste_Dieselpumpe m3 | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombi_Verbauliste_Lenkwinkel m3 | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombi_Multiplex_Baureihe m1 | 8 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| Kombi_Multiplex_Laendervariante m0 | 8 | 6 | Intel | U6 | 1 | 0 | - | - | - |
| Kombi_Verbauliste_Allrad m3 | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombi_Verbauliste_Bordnetz m3 | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombi_Verbauliste_ACC m3 | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombi_Verbauliste_Airbag m3 | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombi_Multiplex_Generation m1 | 4 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Kombi_Verbauliste_Klima m3 | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombi_Verbauliste_ABS m3 | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombi_Verbauliste_Motor m3 | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kombi_Multiplex_Derivat m1 | 0 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Kombi_Multiplex_Sprachvariante m0 | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Kombi_Multiplex_Reifenumfang m2 | 0 | 12 | Intel | U12 | 1 | 0 | mm | - | - |

## CAN ID 0x420 - Kombi_2

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Frei_Kombi_2_2 | 56 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Fehlerstatus_Kl__58_s | 55 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Klemme_58s__Kombi_2_ | 48 | 7 | Intel | U7 | 1 | 0 | % | - | - |
| Fehlerstatus_Kl__58_d | 47 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Klemme_58d__Kombi_2_ | 40 | 7 | Intel | U7 | 1 | 0 | % | - | - |
| Kuehlmitteltemp__4_1__Kombi_2_ | 32 | 8 | Intel | U8 | 0.75 | -48 | C | kombi coolant temperature | - |
| Oeltemperatur_4_1 | 24 | 8 | Intel | U8 | 1 | -60 | C | kombi oil temperature | - |
| Aussentemp__ungefiltert_4_1__Ko | 16 | 8 | Intel | U8 | 0.5 | -50 | C | - | - |
| Aussentemperatur_gefiltert | 8 | 8 | Intel | U8 | 0.5 | -50 | C | outside temp, filtered | - |
| Fehlerspeichereintrag__Kombi_ | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Kombi_2_1 | 4 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| Anhaenger_erkannt | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehlerst__Kuehlmitteltemp__4_1 | 2 | 1 | Intel | Bool | 1 | 0 | - | water temp error | - |
| Fehlerstatus_Oeltemperatur_4_1 | 1 | 1 | Intel | Bool | 1 | 0 | - | oil temp error | - |
| Fehlerstatus_Aussentemp__4_1 | 0 | 1 | Intel | Bool | 1 | 0 | - | ambient temp error | - |

## CAN ID 0x320 - Kombi_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Frei_Kombi_1_3 | 56 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Angezeigte_Geschwindigkeit | 46 | 10 | Intel | U10 | 0.32 | 0 | km/h | - | - |
| Blinker_rechts_4_1 | 45 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Blinker_links_4_1 | 44 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Gesetzte_Zeitluecke__Kombi_1_ | 43 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ADR_Summer_abgeschaltet | 42 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Kombi_1_2 | 40 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Geschwindigkeit__Kombi_1_ | 25 | 15 | Intel | U15 | 0.01 | 0 | km/h | - | - |
| Signalquelle_Geschwindigkeit_4_ | 24 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Tankwarnung | 23 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Tankinhalt | 16 | 7 | Intel | U7 | 1 | 0 | l | - | - |
| Tankstop | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Kombi_1_7 | 12 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| Kombi_im_Stellgliedtest | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ladekontroll_Lampe__Kombi_ | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Bremsinfo | 8 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Vorgluehlampe__Kombi_1_ | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Tankwarnlampe | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Heissleuchten_Vorwarnung | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kuehlmittelmangel | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Dynamische_Oeldruckwarnung | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Oeldruck | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehlerstatus_Tank | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fahrertuer_4_1 | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x5E0 - Klima_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Aussentemp__ungef__Sto_f__4_1 | 56 | 8 | Intel | U8 | 0.5 | -50 | C | - | - |
| Fehlerspeichereintrag__Klima_ | 55 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Klima_1_5 | 50 | 5 | Intel | U5 | 1 | 0 | - | - | - |
| AC_Schalter | 49 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Temperatureinheit | 48 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kuehlerluefteransteuerung__Klim | 40 | 8 | Intel | U8 | 0.4 | 0 | % | - | - |
| Geblaeselast_4_1 | 32 | 8 | Intel | U8 | 0.4 | 0 | % | - | - |
| Kompressorlast | 24 | 8 | Intel | U8 | 0.25 | 0 | Nm | - | - |
| Klimadrucksignal__Klima_1_ | 16 | 8 | Intel | U8 | 0.2 | 0 | bar | - | - |
| Aussentemp__ungef__4_1__Klima_1 | 8 | 8 | Intel | U8 | 0.5 | -50 | C | - | - |
| Kaeltemitteldruck_veraltet | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kompressormoment_veraltet_4_1 | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Keine_Heizleistg_gewuenscht_4_1 | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kompressorzustand__4_1_ | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Heizbare_Frontscheibe | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Heizbare_Heckscheibe | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fahrerwunsch_Zuheizer | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Drehzahlanhebung | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x38A - GRA_Neu

- DLC: 4  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| CHECKSUM | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| GRA_Hauptschalt | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Abbrechen | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Down_kurz | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Up_kurz | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Down_lang | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Up_lang | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Fehler_Bed | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Kodierinfo | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Neu_Setzen | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Recall | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Sender | 18 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| COUNTER | 20 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| GRA_Tip_Down | 24 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Tip_Up | 25 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Zeitluecke | 26 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| GRA_Sta_Limiter | 28 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Typ_Hauptschalt | 29 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Sportschalter | 30 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_Fehler_Tip | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x388 - GRA

- DLC: 3  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Checksumme_GRA_alt | 16 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Frei_GRA_alt | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_alt__ADR_Bedienteil_Fehler | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_alt__ADR_beschleunigen | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_alt__ADR_verzoegern | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_alt__ADR___Tipschalter__Wie | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_alt__ADR___Tipschalter__Set | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_alt__ADR___Tipschalter__Aus | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GRA_alt__ADR___Hauptschalter | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Zaehler_GRA_alt | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |

## CAN ID 0x548 - Getriebe_4

- DLC: 3  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Testparameter_2 | 16 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Testparameter_1 | 8 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Waehlhebelausleuchtung | 4 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Frei_Getriebe_4_1 | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Testfreigabeflag | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Handbremserinnerung_s_Lampe | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Shiftlock_Getriebe_4 | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x540 - Getriebe_2

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| eingelegte_Fahrstufe | 60 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Ganganzeige_Kombi___Getriebe_Va | 56 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Fehlerlampe_f_r_Kupplung_bei_VL | 55 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Anforderung_Kriechadaption | 54 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ECO_Anzeige__4_1_ | 53 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Shift_Lock_Lampe | 52 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Unterdrueckung_von_Warnungen | 51 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Gong | 50 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Starter_wird_angesteuert | 49 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Hochschaltlampe | 48 | 1 | Intel | Bool | 1 | 0 | - | Upshift Flag | - |
| Synchronisationszeit | 40 | 8 | Intel | U8 | 20 | 0 | ms | - | - |
| invertierte_Synchronisations_Wu | 32 | 8 | Intel | U8 | 25 | 0 | U/min | - | - |
| Synchronisations_Wunschdrehzahl | 24 | 8 | Intel | U8 | 25 | 0 | U/min | - | - |
| Gradientenbegrenzung | 16 | 8 | Intel | U8 | 10 | 0 | Nm/s | - | - |
| Leerlaufsolldrehzahl__Getriebe | 8 | 8 | Intel | U8 | 10 | 0 | U/min | - | - |
| Zahler_Getriebe_2 | 4 | 4 | Intel | U4 | 1 | 0 | - | Counter Getriebe_2 | - |
| Zwischengasflag | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ecomatic__4_1_ | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Schubabschaltunterstuetzung | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LFR_Adaption_Freigabeflag | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x440 - Getriebe_1

- DLC: 8  TX: Transmission  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| GE1_Schaltung | 0 | 1 | Intel | Bool | 1 | 0 | - | Gear shift active: 0=no shift, 1=shift in progress | 0="no_circuit", 1="circuit_is_running" |
| GE1_Sta_Schutz | 1 | 1 | Intel | Bool | 1 | 0 | - | Transmission/converter protection request (power reduction or ignition angle) | 0="no_protection", 1="protection_requested" |
| GE1_Klimakompr | 2 | 1 | Intel | Bool | 1 | 0 | - | AC compressor off request | 0="no_Anforderung", 1="Kompressor_aus" |
| GE1_WK | 3 | 2 | Intel | U2 | 1 | 0 | - | Torque converter clutch status | 0="WK_open", 1="WK_regulated", 2="WK_closed", 3="Failure" |
| GE1_StSt_Info | 5 | 2 | Intel | U2 | 1 | 0 | - | Start/stop coordinator info | 0="Engine_running_not_necessary_stop_release", 1="Motor_start_not_mandatory_necessary_stop_prohibition", 2="Motor_start_absolute_necessary_start_request", 3="System_error" |
| GE1_EGS_Anf | 7 | 1 | Intel | Bool | 1 | 0 | - | EGS request - byte 4 becomes valid when set | 0="no_requirement", 1="EGS_requirement" |
| GE1_Zielgang | 8 | 4 | Intel | U4 | 1 | 0 | - | Target gear when shifting or current gear | 0="P (Park) | Disengaged", 1="Gear 1 | Engaged", 2="Gear 2 | Engaged", 3="Gear 3 | Engaged", 4="Gear 4 | Engaged", 5="Gear 5 | Engaged", 6="1m Gear", 7="Gang_R", 8="Gear 6 | Engaged", 9="Gear 7 | Engaged", 10="Gear 8 | Engaged", 14="Speed Not Defined", 15="Failure" |
| GE1_Wahl_Pos | 12 | 4 | Intel | U4 | 1 | 0 | - | Selector lever position / driving program | 8="P", 7="R", 6="N", 5="D", 9="U", 12="S", 14="T", 10="T", 11="T" |
| GE1_Mrad_Mkurb | 16 | 8 | Intel | U8 | 0.1 | 0 | - | Wheel torque to crankshaft torque ratio (Mrad/Mcrank) | - |
| GE1_Soll_Mo | 24 | 8 | Intel | U8 | 0.39 | 0 | % | Internal target engine torque request (0-99.06%) | - |
| GE1_Fahrwistd | 32 | 8 | Intel | U8 | 0.249 | -31.623 | - | Driving resistance index (normalized % slope) | - |
| GE1_Notlauf | 40 | 4 | Intel | U4 | 1 | 0 | - | Emergency mode status | 0="No Emergency", 1="No Switching", 2="Switch to Neutral", 3="50% Shifting Capacity", 4="Driving Without UK", 5="No Emeregency", 6="No Emergency", 7="No Emergency, Target Shifting Reached", 15="Failure" |
| GE1_Kuehlung | 44 | 2 | Intel | U2 | 1 | 0 | - | Cooling request level | 0="No Additional Cooling Requested", 1="20% Additional Fan Cooling Requested", 2="40% Additional Fan Cooling Requested", 3="Maximum Additional Fan Cooling Requested" |
| GE1_Sta_OBD | 47 | 1 | Intel | Bool | 1 | 0 | - | OBD status - MIL lamp control | 0="MIL Off", 1="MIL On" |
| GE1_LaunchControl | 48 | 1 | Intel | Bool | 1 | 0 | - | Launch control active | 0="Launch Control Not Active", 1="Launch Control Program Active" |
| GE1_Infobit | 49 | 1 | Intel | Bool | 1 | 0 | - | Transmission type: 0=stepped automatic, 1=CVT | 0="Standard Automatic", 1="CVT" |
| GE1_SleepInd | 50 | 1 | Intel | Bool | 1 | 0 | - | Sleep readiness after terminal 15 off | 0="CAN is Required", 1="Sleep Ready" |
| COUNTER | 51 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| GE1_Fehlereintr | 55 | 1 | Intel | Bool | 1 | 0 | - | Fault memory entry present | 0="No Failure Memory Entry", 1="Failure Memory Entry" |
| GE1_WaVerl_Mo | 56 | 8 | Intel | U8 | 0.39 | 0 | % | Transmission torque loss / clutch torque (0-99.06%) | - |

## CAN ID 0x390 - Gate_Komf_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| GK1_Sta_RDK_Warn | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Sta_Anhaen | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Sta_Licht1 | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Sta_Licht3 | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Sta_Tuerkont | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Sta_Li_vorn | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_SleepAckn | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_CharismaModus m1 | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| GK1_SamFktNr M | 12 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| GK1_Fa_Tuerkont | 16 | 1 | Intel | Bool | 1 | 0 | - | Status of the driver's door rotary latch | - |
| GK1_RueckfahrSch | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_ELV_verrieg | 18 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Sta_Kessy_2 | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Sta_Stdhzg | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_SH_Verbau | 21 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_ParkFrontWi | 22 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_KW_Warm | 23 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BCM_Remotestart_Betrieb | 24 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BSK_HL_geoeffnet | 26 | 1 | Intel | Bool | 1 | 0 | - | Status of the rear left door rotary latch | - |
| BSK_HR_geoeffnet | 27 | 1 | Intel | Bool | 1 | 0 | - | Status of the rear right door rotary latch | - |
| GK1_Rueckfahr | 28 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_BrLi_links | 29 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_BrLi_rechts | 30 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_BrLi_mitte | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_BLS_ILM | 32 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_EDC_ILM | 33 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Blinker_li | 34 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Blinker_re | 35 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_def_P_verr | 36 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_LS1_Fernlicht | 37 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Sta_Licht2 | 38 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Sta_LSM | 39 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Count_Anhaen | 40 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| BSK_BT_geoeffnet | 41 | 1 | Intel | Bool | 1 | 0 | - | Status of the passenger door rotary latch | - |
| BSK_HD_Hauptraste | 43 | 1 | Intel | Bool | 1 | 0 | - | Status of trunk lid main detent | - |
| GK1_BLS_AAG | 44 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_EDC_AAG | 45 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Anhaenger | 46 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_BrLi_Anhaen | 47 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Abblendlicht | 48 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Fernlicht | 49 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Wischer_vorn | 50 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Sta_ILM_F_1 | 51 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Abbl_VL_def | 52 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Abbl_VR_def | 53 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Blink_Autob | 54 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Warnblk_Status | 55 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_SH_laeuft | 56 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| SH1_ein_Wasserpumpe | 57 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Nebel_ein | 58 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Bremslicht | 59 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_Anh_abgesteckt | 60 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_AnhKonLamp | 61 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LDS_Stellung_AFL | 62 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK1_SH_Zusatzfkt | 63 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x53C - Fahrwerk_1

- DLC: 1  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Frei_Fahrwerk_1_2 | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Fahrwerk_1_1 | 6 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Einstellung_Fahrwerkdaempfung_4 | 4 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| Ansteuererung_Fahrzeugniveau | 0 | 4 | Intel | U4 | 1 | 0 | - | - | - |

## CAN ID 0x5C0 - EPB_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| COUNTER | 0 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| EP1_Fehler_Sta | 4 | 2 | Intel | U2 | 1 | 0 | - | - | 0="volle_Funktion", 1="linke_Seite_fehlerhaft", 2="rechte_Seite_fehlerhaft", 3="beide_Seiten_fehlerhaft" |
| EP1_Sta_EPB | 6 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Bremse_geoeffnet", 1="Bremse_geschlossen" |
| EP1_Sta_Schalter | 7 | 1 | Intel | Bool | 1 | 0 | - | - | 0="volle_Funktion", 1="Schalter_ausser_Funktion" |
| EP1_Spannkraft | 8 | 5 | Intel | U5 | 1 | 0 | Unit_KiloNewto | - | 31="Fehler" |
| EP1_Schalterinfo | 13 | 2 | Intel | U2 | 1 | 0 | - | - | 0="keine_Fahreranforderung", 1="Fahreranforderung_oeffnen", 2="Fahreranforderung_schliessen", 3="Schalterfehler" |
| EP1_Sta_NWS | 15 | 1 | Intel | Bool | 1 | 0 | - | - | 0="volle_Funktion", 1="keine_Funktion" |
| EP1_Neig_winkel | 16 | 8 | Intel | U8 | 1 | -128 | Unit_PerCentOfForceOfGravi | - | - |
| EP1_Verzoegerung | 24 | 8 | Intel | U8 | 0.048 | -7.968 | Unit_MeterPerSeconSquar | - | - |
| EP1_Fehlereintr | 32 | 1 | Intel | Bool | 1 | 0 | - | - | 0="kein_Fehlerspeichereintrag", 1="Fehlerspeichereintrag" |
| EP1_Freigabe_Ver | 33 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Verzoegerungsanf_nicht_freigegeb", 1="Verzoegerungsanf_freigegeben" |
| EP1_AutoHold_zul | 34 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Pers_nicht_zulaessig", 1="Pers_zulaessig" |
| EP1_AutoHold_aktiv | 35 | 1 | Intel | Bool | 1 | 0 | - | - | 0="nein", 1="ja" |
| EP1_SleepInd | 36 | 1 | Intel | Bool | 1 | 0 | - | - | 0="CAN_wird_benoetigt", 1="Sleep_bereit" |
| EP1_Status_Kl_15 | 37 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Kl_15_aus", 1="Kl_15_ein" |
| EP1_Lampe_AutoP | 38 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Lampe_aus", 1="Lampe_ein" |
| EP1_Bremslicht | 39 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Aus", 1="Ein" |
| EP1_Warnton1 | 40 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Aus", 1="Ein" |
| EP1_Warnton2 | 41 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Aus", 1="Ein" |
| EP1_AnfShLock | 42 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Aus", 1="Ein" |
| EPB_Autoholdlampe | 43 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Lampe_aus", 1="Lampe_ein" |
| EP1_QualNeigWi | 44 | 1 | Intel | Bool | 1 | 0 | - | - | 0="gueltiger_Wert", 1="Ersatz_Init_oder_Fehlerwert" |
| EP1_KuppModBer | 45 | 2 | Intel | U2 | 1 | 0 | - | - | 0="Kuppsensor_aus_Modbereich", 1="Kupplsensor_im_Modbereich", 2="Sensorsignal_ungenau", 3="Sensor_defekt" |
| EP1_HydrHalten | 47 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Fzg_nicht_hydr_geh", 1="Fzg_hydr_geh" |
| EP1_Fkt_Lampe | 48 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Lampe_aus", 1="Lampe_ein" |
| EP1_Warnton | 49 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Warnton_aus", 1="Warnton_an" |
| EP1_Fehler_BKL | 50 | 1 | Intel | Bool | 1 | 0 | - | - | 0="BKL_aus", 1="BKL_an" |
| EP1_Fehler_gelb | 51 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Lampe_aus", 1="Lampe_ein" |
| EP1__Text | 52 | 4 | Intel | U4 | 1 | 0 | - | - | 0="kein_Text", 1="Text_1", 2="Text_2", 3="Text_3", 4="Text_4", 5="Text_5", 6="reserviert", 7="reserviert", 8="Text_8" |
| CHECKSUM | 56 | 8 | Intel | U8 | 1 | 0 | - | - | - |

## CAN ID 0x52E - Diag_Lenkhilfe

- DLC: 3  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Werkstattcode__Diag_ | 16 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Multiplex_Signal__Diag_ | 13 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| Befehl_Sensorcodierung_Lenkhilf | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Befehl_Kennliniencodierung_Lenk | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Befehl_Fehlerspeicher_loeschen | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |

## CAN ID 0x598 - Daempfer_1

- DLC: 2  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Frei_Daempfer_1_4 | 12 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Textbits_Daempfer | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Fehlerspeicherbit__Daempfer_1_ | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Systemstatus__Daempfer_1_ | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Daempfer_1_3 | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Status_CDC_Taster | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Daempfer_1_2 | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Status_Daempferregelung_4_1 | 0 | 3 | Intel | U3 | 1 | 0 | - | - | - |

## CAN ID 0x570 - BSG_Last

- DLC: 4  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Klimaanlage_abschalten | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Sitzbelueftung_abschalten | 30 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Wischwasserheizung_abschalten | 29 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lenkradheizung_abschalten | 28 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Heizbare_Sitze_abschalten | 27 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Heizbare_Aussenspiegel_abschalt | 26 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Heizbare_Frontscheibe_abschalte | 25 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Heizbare_Heckscheibe_abschalten | 24 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Batteriespannung_Bordnetzbatter | 16 | 8 | Intel | U8 | 0.05 | 5 | V | - | - |
| Motorhaubenkontakt | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Leuchtweitenregulierung | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehlerspeichereintrag__BSG_Last | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Zustand_der_Starterbatterie | 11 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Zustand_der_Bordnetzbatterie | 9 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| LL_Drehzahlanhebung | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Klemme_L | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_BSG_Last_1_1 | 4 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| ZAS_Klemme_50 | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ZAS_Klemme_X | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ZAS_Klemme_15 | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ZAS_Klemme_S | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x470 - BSG_Kombi

- DLC: 5  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Frei_BSG_Kombi_1_3 | 36 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Ruecksitzlehne_HR_verr__4_1 | 35 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ruecksitzlehne_HL_verr__4_1 | 34 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehlerlampe_Lenkhilfe_veraltet | 33 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehlerlampe_Lenkhilfe__BSG_Komb | 32 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehlerstatus_Kl__58s | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Klemme_58s__BSG_Kombi_ | 24 | 7 | Intel | U7 | 1 | 0 | % | - | - |
| Fehlerstatus_Kl__58d | 23 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Klemme_58d__BSG_Kombi_ | 16 | 7 | Intel | U7 | 1 | 0 | % | - | - |
| Unterspannung | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_BSG_Kombi_1_2 | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Heckdeckel_geoeffnet | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Motorhaube_geoeffnet | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Tuer_hinten_rechts_geoeffnet | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Tuer_hinten_links_geoeffnet | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Beifahrertuer_geoeffnet | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fahrertuer_geoeffnet | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Lade_Kontrollampe | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_BSG_Kombi_1_1 | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Rueckfahrlicht | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| DWA_Akku | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Warnblink_Mode | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Anhaenger_Kontrollampe | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Blinker_rechts_Kontrollampe | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Blinker_links_Kontrollampe | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x1A8 - Bremse_6

- DLC: 3  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Checksumme_Bremse_6 | 16 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Zaehler_Bremse_6 | 12 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Status_Bremsdruck__Bremse_6__du | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Bremse_6_1 | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Bremsdruck__Bremse_6_ | 0 | 10 | Intel | U10 | 0.3255 | -40 | bar | - | - |

## CAN ID 0x4A8 - Bremse_5

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| CHECKSUM | 56 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| COUNTER | 52 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| BR5_ECD_Lampe | 51 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_ZT_Rueckk_Umsetz | 48 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_Anhi_Sta | 40 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ESP_Rollenmodus_Deactiveieren | 34 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_Sign_Druck | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_Sta_Druck | 30 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_Druckvalid | 29 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_Stillstand | 28 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_Bremsdruck | 16 | 12 | Intel | U12 | 0.1 | 0 | bar | - | - |
| BR5_Vorzeichen | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_Sta_Gierrate | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_Giergeschw | 0 | 14 | Intel | U14 | 0.01 | 0 | Grad/sec | - | - |
| BR5_ANB_CM_Rueckk_Umsetz | 49 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_HDC_bereit | 50 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ESP_Stat_FallBack_eBKV | 35 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ESP_Anforderung_EPB | 36 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| ESP_Autohold_active | 38 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ESP_Autohold_Standby | 39 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_Anhi_akt | 41 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_v_Ueberw | 42 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_Bremslicht | 43 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_Notbremsung | 44 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_Fahrer_tritt_ZBR_Schw | 45 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_AWV2_Bremsruck | 46 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR5_AWV2_Fehler | 47 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x2A0 - Bremse_4

- DLC: 3  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Frei_Bremse_4_1 | 17 | 7 | Intel | U7 | 1 | 0 | - | - | - |
| Einheit_Kupplungssteifigkeit | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ABS_Vorgabewert_hinten_Kupplung | 8 | 8 | Intel | U8 | 0.7874 | 0 | % | - | - |
| ABS_Vorgabewert_mitte_Kupplungs | 0 | 8 | Intel | U8 | 3 | -381 | Nm/min | - | - |

## CAN ID 0x4A0 - Bremse_3

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Radgeschw__HR_4_1 | 49 | 15 | Intel | U15 | 0.01 | 0 | km/h | - | - |
| Frei_Bremse_3_4 | 48 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Radgeschw__HL_4_1 | 33 | 15 | Intel | U15 | 0.01 | 0 | km/h | - | - |
| Frei_Bremse_3_3 | 32 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Radgeschw__VR_4_1 | 17 | 15 | Intel | U15 | 0.01 | 0 | km/h | - | - |
| Frei_Bremse_3_2 | 16 | 1 | Intel | Bool | 0.01 | 0 | km/h | - | - |
| Radgeschw__VL_4_1 | 1 | 15 | Intel | U15 | 0.01 | 0 | km/h | - | - |
| Frei_Bremse_3_1 | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x5A0 - Bremse_2

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| gemessene_Querbeschleunigung | 63 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Bremse_2_2 | 62 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Impulszahl | 56 | 6 | Intel | U6 | 1 | 0 | - | - | - |
| Fehlerstatus_Wegimpulse_4_1 | 55 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Bremse_2_5 | 54 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Warnlampe_DDS | 53 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehlerspeichereintrag_Bremse | 52 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Wegimpulszaehlerstatus | 51 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Wegimpulse_Vorderachse | 40 | 11 | Intel | U11 | 1 | 0 | - | - | - |
| Zeitstempel | 24 | 16 | Intel | U16 | 1 | 0 | tics | - | - |
| mittlere_Raddrehzahl__Bremse_2 | 9 | 15 | Intel | U15 | 0.002 | 0 | U/sec | - | - |
| Querbeschl__TimerTic M | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Timer m1 | 0 | 8 | Intel | U8 | 0.04 | 0 | usec | - | - |
| Querbeschleunigung m0 | 0 | 8 | Intel | U8 | 0.01 | -1.27 | g | - | - |

## CAN ID 0x1A0 - Bremse_1

- DLC: 8  TX: ABS  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| BR1_ASR_Anf | 0 | 1 | Intel | Bool | 1 | 0 | - | ASR request to engine - requests in bytes 5 and 6 to be executed | 0="no_requirement", 1="ASR_requirement" |
| BR1_MSR_Anf M | 1 | 1 | Intel | Bool | 1 | 0 | - | MSR request - when set, byte 6 contains inverse MSR torque | 0="no_requirement", 1="MSR_requirement" |
| BR1_ABS_Brems | 2 | 1 | Intel | Bool | 1 | 0 | - | ABS control active (including pressure reduction) | 0="no_ABS_control", 1="ABS_control" |
| BR1_EDS_Ongr | 3 | 1 | Intel | Bool | 1 | 0 | - | Electronic differential lock intervention | 0="no_EDS_intervention", 1="EDS_intervention" |
| BR1_ESP_Ongr | 4 | 1 | Intel | Bool | 1 | 0 | - | ESP driving dynamics control intervention | 0="no_ESP_intervention", 1="ESP_intervention" |
| BR1_ASR_Ongr | 5 | 2 | Intel | U2 | 1 | 0 | - | ASR transmission switching influence | 0="no_requirement", 1="ASR_shift_map", 2="Downshift", 3="Switching_ban" |
| BR1_EBV_Ongr | 7 | 1 | Intel | Bool | 1 | 0 | - | Electronic brake force distribution intervention | 0="no_EBV_intervention", 1="EBV_intervention" |
| BR1_Lampe_ABS | 8 | 1 | Intel | Bool | 1 | 0 | - | ABS safety warning lamp | 0="Lamp_off", 1="Lamp_on" |
| BR1_Lampe_ASR | 9 | 1 | Intel | Bool | 1 | 0 | - | ASR/ESP lamp (including flashing info) | 0="Lamp_off", 1="Lamp_on" |
| BR1_Lampe_BK | 10 | 1 | Intel | Bool | 1 | 0 | - | Brake control lamp (red) | 0="Lamp_off", 1="Lamp_on" |
| BR1_Lichtschalt | 11 | 1 | Intel | Bool | 1 | 0 | - | Brake light switch / brake pressure threshold exceeded | 0="no_Bremsen_Schw_unterschr", 1="Bremse_betaetigt_Schw_ueberschr" |
| BR1_StaDruckschw | 12 | 1 | Intel | Bool | 1 | 0 | - | Brake test switch / pressure threshold status | 0="Sta_guel_BKV_n_angest_k_Br", 1="Sta_n_verf_BKV_angest_Br" |
| BR1_MAD | 13 | 1 | Intel | Bool | 1 | 0 | - | OBD rough road suppression | 0="no_Offblendung", 1="Offblendung" |
| BR1_Sta_MAD | 14 | 1 | Intel | Bool | 1 | 0 | - | Rough road suppression status | 0="valid", 1="invalid" |
| BR1_Diagnose | 15 | 1 | Intel | Bool | 1 | 0 | - | ABS diagnosis mode active | 0="no", 1="in_Diagnose" |
| BR1_BKV_active | 16 | 1 | Intel | Bool | 1 | 0 | - | Active brake booster installed and not faulty | 0="BKV_not_installed_or_fault", 1="BKV_installed_and_not_fault" |
| BR1_Rad_kmh | 17 | 15 | Intel | U15 | 0.01 | 0 | km/h | Vehicle speed (mean of driven wheels or ABS reference) | - |
| BR1_ASRMo_sl | 32 | 8 | Intel | U8 | 0.39 | 0 | % | ASR engagement torque slow (0-99.06%) | - |
| BR1_ASRMo_fa m0 | 40 | 8 | Intel | U8 | 0.39 | 0 | % | ASR engagement torque fast when MSR_Anf=0 (0-99.06%) | - |
| BR1_MSR_Mo_inv m1 | 40 | 8 | Intel | U8 | -0.39 | 99.45 | % | Inverse MSR torque when MSR_Anf=1 (99.06-0%) | - |
| BR1_MSR_Mo | 48 | 8 | Intel | U8 | 0.39 | 0 | % | MSR engagement torque (0-99.06%) | - |
| BR1_Zaehler | 56 | 4 | Intel | U4 | 1 | 0 | - | Message counter to mark intervention moment | - |
| BR1_ASR_ESP | 60 | 1 | Intel | Bool | 1 | 0 | - | Control unit type: 0=ABS/EDS, 1=ASR/ESP/MABS | 0="ABS_ABS_EDS", 1="ASR_ESP_MABS" |
| BR1_ESPASR_passive | 61 | 1 | Intel | Bool | 1 | 0 | - | ESP/ASR passive or deactivated by driver | 0="ESP_ASR_activated", 1="ESP_oder_ASR_passive_get_oder_Fktumsch_oder_Rollenmodus" |
| BR1_Sta_ESP | 62 | 1 | Intel | Bool | 1 | 0 | - | ESP error status (fault memory entry) | 0="ok", 1="Failure" |
| BR1_Ersatz_Kmh | 63 | 1 | Intel | Bool | 1 | 0 | - | Speed is substitute value (sensor defect) | 0="OK", 1="Substitute_value" |

## CAN ID 0x2A8 - Bremsbooster_1

- DLC: 3  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Fehlerspeichereintrag_Booster | 23 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Loseschalter_unplausibel_Boost | 22 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Position_Standby | 21 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| ADR_Relais_ge_ffnet | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Status_Bremsbooster_Steuerung | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Bremsbooster_verf_gbar | 18 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Eingriff_Bremsbooster | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Bremseingriff_Fahrer | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Bremsbooster_1_1 | 12 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Zaehler_Booster_1 | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Checksumme_Booster_1 | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |

## CAN ID 0x578 - BatMan_1

- DLC: 1  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Fehlerspeichereintrag__BatMan_ | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Leistungsrelais | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Messung_Starterleitung | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Zustand_Starterleitung | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Umschaltrelais_Bordnetzbatterie | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ladung_Starterbatterie | 1 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Startmodus | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x2C0 - Allrad_1

- DLC: 5  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Kupplungssteifigkeit_Hinten__Is | 32 | 8 | Intel | U8 | 0.7874 | 0 | % | - | - |
| Fehlerspeichereintrag_Allrad_1 | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Allrad_1_1 | 26 | 5 | Intel | U5 | 1 | 0 | - | - | - |
| Schaltung_Vorwarnung | 25 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Schaltung_aktiv__Allrad_1_ | 24 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ganginfo__PNG_ | 20 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| PNG_Anzeige_blinkend | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PNG_Status_4_1 | 16 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| Kupplungssteifigkeit_Mitte__Ist | 8 | 8 | Intel | U8 | 3 | -381 | Nm/min | - | - |
| Einheit_der_Kupplungssteifigkei | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Geschwindigkeitsbegrenzung | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Allrad_Warnlampe | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Notlauf | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kupplung_komplett_offen | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehlerstatus_Kupplungssteifigke | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ubertemperaturschutz__Allrad_1_ | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Fehler_Allrad_Kupplung | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x550 - Airbag_2

- DLC: 2  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| OOP_Beifahrer | 14 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| OOP_Fahrer | 12 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Belegungserkennung_hinten_mitte | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Belegungserkennung_hinten_recht | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Belegungserkennung_hinten_links | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Belegungserkennung_Beifahrersit | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Checksumme_Airbag_2__reserviert | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |

## CAN ID 0x050 - Airbag_1

- DLC: 4  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| CHECKSUM | 24 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| COUNTER | 20 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Fehlerspeichereintrag | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Frei_Airbag_1_2 | 18 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Airbag_im_Stellgliedtest | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Airbag_in_Diagnose | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Gurtwarnung_Beifahrer | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Gurtschalter_Beifahrer | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Gurtwarnung_Fahrer | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Gurtschalter_Fahrer | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Airbag_Systemfehler | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Kindersitzerkennung | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Airbag_deaktiviert | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Airbag_Lampe | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Crash_Intensitaet | 5 | 3 | Intel | U3 | 1 | 0 | B | - | - |
| Rollover | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Seiten_Crash_Beifahrer | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Seiten_Crash_Fahrer | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Heck_Crash | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Front_Crash | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x360 - ADR_System

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| S_Checksumme_ADR_1 | 56 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| S_Frei_ADR_1_1 | 52 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| S_Zeitluecke_gemessen | 48 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| S_Fehlerspeichereintrag_ADR | 47 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Fehlerspeichereintrag_Bremsbo | 46 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_ADR_Relais_geoeffnet | 45 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Bremsbooster_Status | 43 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| S_Eingriff_Bremsbooster | 42 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Loeseschalter_unplausibel | 41 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Bremseingriff_Fahrer | 40 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Anzeige_Sensor_blind | 39 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Ansteuerung_optischer_Fahrerh | 38 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Ansteuerung_Gong_2 | 37 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Ansteuerung_Gong_1 | 36 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Schaltaufforderung | 34 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| S_Anzeige_Prioritaet | 33 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Anzeige_Zeitluecke | 32 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Wunschgeschwindigkeit | 24 | 8 | Intel | U8 | 1 | 0 | km/h | - | - |
| S_Objekt_erfasst | 22 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| S_Gesetzte_Zeitluecke__ADR_1_ | 18 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| S_Synchronisation_Bremsbooster | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Momentenanforderung_Freigabe | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Verhinderung_Schubabschaltung | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Status_ADR_1__S_ | 13 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| S_Fehler_ADR_1__S_ | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| S_Zaehler_ADR_1 | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| S_Momentenanforderung_ADR | 0 | 8 | Intel | U8 | 0.39 | 0 | MDI | - | - |

## CAN ID 0x260 - ADR_2

- DLC: 4  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Frei_ADR_2_2 | 27 | 5 | Intel | U5 | 1 | 0 | - | - | - |
| Anforderung_Bremsdruck | 16 | 11 | Intel | U11 | 0.0625 | 0 | bar | - | - |
| Frei_ADR_2_1 | 14 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Relais_Test_Fehler | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Standby | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Zaehler_ADR_2 | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Checksumme_ADR_2 | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |

## CAN ID 0x52C - ADR_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| Checksumme_ADR_1 | 56 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| Frei_ADR_1_5 | 52 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Zeitluecke_gemessen | 48 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Fehlerspeichereintrag_ADR | 47 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Reserviert_ADR_1_1 | 40 | 7 | Intel | U7 | 1 | 0 | - | - | - |
| Anzeige_Sensor_blind | 39 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ansteuerung_optischer_Fahrerhin | 38 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ansteuerung_Gong_2 | 37 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Ansteuerung_Gong_1 | 36 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Schaltaufforderung | 34 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Anzeige_Prioritaet | 33 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Anzeige_Zeitluecke | 32 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Wunschgeschwindigkeit | 24 | 8 | Intel | U8 | 1 | 0 | km/h | - | - |
| Objekt_erfasst | 22 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Gesetzte_Zeitluecke__ADR_1_ | 18 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Synchronisation_Bremsbooster | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Momentenanforderung_Freigabe | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Verhinderung_Schubabschaltung | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Status_ADR_1 | 13 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| Fehler_ADR_1 | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Zaehler_ADR_1 | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| Momentenanforderung_ADR | 0 | 8 | Intel | U8 | 0.39 | 0 | MDI | - | - |

## CAN ID 0x60E - Einheiten_1

- DLC: 2  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| MFA_v_Einheit_02 | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x368 - ACC_System

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| CHECKSUM | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| COUNTER | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| ACS_Sta_ADR | 12 | 2 | Intel | U2 | 1 | 0 | - | - | 2="ADR_passiv", 0="ADR_nicht_aktiv", 1="ADR_aktiv", 3="irrev_Fehler" |
| ACS_ADR_Schub | 14 | 1 | Intel | Bool | 1 | 0 | - | - | 1="Verz_begr_auf_Schub", 0="Verz_nicht_begr_auf_Schub" |
| ACS_Schubabsch | 15 | 1 | Intel | Bool | 1 | 0 | - | - | 1="SA_nicht_zulaessig", 0="SA_zulaessig" |
| ACS_StSt_Info | 16 | 2 | Intel | U2 | 1 | 0 | - | - | 3="Systemfehler", 0="Motorlauf_nn", 1="Stoppverbot_Motoranlauf_nn", 2="Motoranlauf_notwendig" |
| ACS_MomEingriff | 18 | 1 | Intel | Bool | 1 | 0 | - | - | 1="MomEingr_verhindern", 0="keine_Beeinfl_MomEingr_Mot" |
| ACS_Typ_ACC | 19 | 2 | Intel | U2 | 1 | 0 | - | - | 0="Basis_ACC", 1="ACC_mit_FollowToStop", 3="frei", 2="frei" |
| ACS_FreigSollB | 23 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Sollbeschl_nicht_freigeg", 1="Sollbeschl_freigeg" |
| ACS_Sollbeschl | 24 | 11 | Intel | U11 | 0.005 | -7.22 | Unit_MeterPerSeconSquar | - | 2046="ADR_nicht_aktiv", 2047="Fehler" |
| ACS_Anhaltewunsch | 38 | 1 | Intel | Bool | 1 | 0 | - | - | 0="kein_Haltewunsch", 1="Fzg_haelt_an" |
| ACS_Fehler | 39 | 1 | Intel | Bool | 1 | 0 | - | - | 1="Fehlerspeichereintrag", 0="kein_Fehlerspeichereintrag" |
| ACS_zul_Regelabw | 40 | 8 | Intel | U8 | 0.005 | 0 | Unit_MeterPerSeconSquar | - | 254="ADR_nicht_aktiv", 255="Fehler" |
| ACS_max_AendGrad | 48 | 8 | Intel | U8 | 0.02 | 0 | Unit_MeterPerSeconSquar | - | 254="Neutralwert", 0="Neutralwert", 255="Fehler" |

## CAN ID 0x56A - ACC_GRA_Anzeige

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| CHECKSUM | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| ACA_StaACC | 8 | 3 | Intel | U3 | 1 | 0 | - | - | 6="ACC_rev_aus", 0="Hauptschalter_aus", 4="ACC_im_Hintergrund", 3="ACC_aktiv", 1="Reserve", 2="ACC_passiv", 7="ACC_irrev_aus", 5="frei" |
| ACA_ID_StaACC | 11 | 5 | Intel | U5 | 1 | 0 | - | - | 0="keine_Anzeige" |
| ACA_Fahrerhinw | 16 | 1 | Intel | Bool | 1 | 0 | - | - | 1="Ein", 0="Aus" |
| ACA_AnzDisplay | 17 | 1 | Intel | Bool | 1 | 0 | - | - | 1="Anzeige_erw", 0="Anzeige_nicht_erw" |
| ACA_Zeitluecke | 18 | 4 | Intel | U4 | 1 | 0 | - | - | 3="Zeitluecke3", 10="Zeitluecke10", 4="Zeitluecke4", 14="Zeitluecke14", 11="Zeitluecke11", 2="Zeitluecke2", 13="Zeitluecke13", 9="Zeitluecke9", 1="Zeitluecke1", 8="Zeitluecke8", 5="Zeitluecke5", 15="Zeitluecke15", 0="nicht_definiert", 12="Zeitluecke12", 6="Zeitluecke6", 7="Zeitluecke7" |
| ACA_V_Wunsch | 24 | 8 | Intel | U8 | 1 | 0 | Unit_KiloMeterPerHour | 255=unset | 255="kein_Wert_im_Speicher" |
| ACA_kmh_mph | 32 | 1 | Intel | Bool | 1 | 0 | - | - | 0="km_h", 1="mph" |
| ACA_Akustik1 | 33 | 1 | Intel | Bool | 1 | 0 | - | - | 0="kein_Gong", 1="Gong" |
| ACA_Akustik2 | 34 | 1 | Intel | Bool | 1 | 0 | - | - | 0="kein_Summer", 1="Summer" |
| ACA_PrioDisp | 35 | 2 | Intel | U2 | 1 | 0 | - | - | 1="mittlere_Prio", 3="keine_Anzeige_Anf", 0="hohe_Prio", 2="niedrige_Prio" |
| ACA_gemZeitl | 40 | 4 | Intel | U4 | 1 | 0 | - | - | 6="Zeitluecke6", 2="Zeitluecke2", 7="Zeitluecke7", 13="Zeitluecke13", 11="Zeitluecke11", 4="Zeitluecke4", 8="Zeitluecke8", 12="Zeitluecke12", 10="Zeitluecke10", 0="Kein_Objekt_erfasst", 1="Zeitluecke1", 3="Zeitluecke3", 9="Zeitluecke9", 15="Zeitluecke15", 14="Zeitluecke14", 5="Zeitluecke5" |
| ACA_ACC_Verz | 44 | 1 | Intel | Bool | 1 | 0 | - | - | 0="ACC_verzoegert_nicht", 1="ACC_verzoegert" |
| ACA_StaGRA | 48 | 3 | Intel | U3 | 1 | 0 | - | - | 3="GRA_aktiv", 4="GRA_uebertreten", 2="GRA_passiv", 0="Hauptschalter_aus", 6="frei", 7="GRA_Fehler", 1="Reserve", 5="frei" |
| ACA_ID_StaGRA | 51 | 5 | Intel | U5 | 1 | 0 | - | - | 0="keine_Anzeige" |
| ACA_Codierung | 56 | 1 | Intel | Bool | 1 | 0 | - | - | 0="ACC", 1="GRA" |
| ACA_Tachokranz | 57 | 1 | Intel | Bool | 1 | 0 | - | - | 0="nicht_beleuchtet", 1="beleuchtet" |
| ACA_Aend_Zeitluecke | 58 | 1 | Intel | Bool | 1 | 0 | - | - | 1="Anzeige_angef", 0="keine_Anzeige" |
| COUNTER | 60 | 4 | Intel | U4 | 1 | 0 | - | - | - |

## CAN ID 0x0D0 - Lenkhilfe_3

- DLC: 6  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| CHECKSUM | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| LH3_BS_Spiegel | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| COUNTER | 12 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| LH3_LM | 16 | 10 | Intel | U10 | 1 | 0 | - | - | - |
| LH3_LMSign | 26 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH3_LMValid | 27 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH3_Sta_DSR | 28 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| LH3_BLW | 32 | 12 | Intel | U12 | 0.15 | 0 | - | - | - |
| LH3_BLWSign | 44 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH3_BLWValid | 45 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH3_Lenkungstyp | 46 | 2 | Intel | U2 | 1 | 0 | - | - | - |

## CAN ID 0x3D2 - Lenkhilfe_2

- DLC: 7  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| CHECKSUM | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| COUNTER | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| LH2_Geradeaus | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH2_Sta_Charisma | 13 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| LH2_Sta_HCA | 16 | 4 | Intel | U4 | 1 | 0 | - | - | 0="disabled", 1="initializing", 2="fault", 3="ready", 4="rejected", 5="active", 7="active" |
| LH2_Ausg_LW1 | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH2_Ausg_LW1_gue | 21 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LH2_StatEPS_PLA | 24 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| LH2_aktLenkeingriff | 32 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| LH2_PLA_Err | 48 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| LH2_PLA_Abbr | 52 | 4 | Intel | U4 | 1 | 0 | - | - | - |

## CAN ID 0x3D4 - PLA_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| CHECKSUM | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| COUNTER | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| PL1_Status_EPS | 12 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| PL1_ArcAngleReq | 16 | 15 | Intel | U15 | 0.04375 | 0 | Unit_DegreeOfArc | - | - |
| PL1_AngleReqSign | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PL1_Stat_PLA_ESP | 32 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| PL1_Bremsmoment | 40 | 13 | Intel | U13 | 4 | 0 | Unit_NewtoMeter | - | - |
| PL1_void | 53 | 11 | Intel | U11 | 1 | 0 | - | - | - |

## CAN ID 0x0D2 - HCA_1

- DLC: 5  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| CHECKSUM | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| COUNTER | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| HCA_Status | 12 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| LM_Offset | 16 | 15 | Intel | U15 | 0.03125 | 0 | cNm | centiNewton-meters for ease of calculation without FP math in Panda | - |
| LM_OffSign | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| Vib_Freq | 32 | 4 | Intel | U4 | 4 | 0 | Hz | Steering wheel haptic, frequency | - |
| Vib_Amp | 36 | 4 | Intel | U4 | 0.5 | 0 | Nm | Steering wheel haptic, amplitude | - |

## CAN ID 0x284 - Motor_Bremse

- DLC: 6  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| MOB_Standby | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| MOB_Freigabe | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| MOB_Anhaltewunsch | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| MOB_CHECKSUM | 0 | 8 | Intel | U8 | 1 | 0 | - | Checksum MOB | - |
| MOB_COUNTER | 8 | 4 | Intel | U4 | 1 | 0 | - | Counter MOB | - |
| TSK_v_Begrenzung_aktiv | 15 | 1 | Motorola | Bool | 1 | 0 | - | - | - |
| TSK_ax_Getriebe_01 | 40 | 8 | Intel | U8 | 0.048 | 0 | m/s2 | - | - |
| MOB_Bremsstgr | 16 | 11 | Intel | U11 | 0.048852 | 0 | Unit_PerCent | - | - |
| MOB_Bremsmom | 27 | 13 | Intel | U13 | 4 | 0 | Unit_NewtoMeter | - | - |

## CAN ID 0x366 - AWV

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| CHECKSUM | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| COUNTER | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| AWV_Text | 12 | 4 | Intel | U4 | 1 | 0 | - | - | 0="kein_Text", 1="FrontAssist_aus", 2="FrontAssist_startet", 3="FrontAssist_Warnung", 4="FrontAssist_Sens_reinig", 5="FrontAssist_Failure", 6="FrontAssist_Demo", 7="Vorhalt", 8="Bremsung_wird_gerade_durchgefuehrt", 9="Sensor_not_verfuegbar", 10="Sensor_reinigen", 11="Service_notwendig_Failure", 12="Funktion_vom_Fahrer_deactivated", 13="Funktion_vom_Fahrer_activated", 14="FrontAssist_zur_Zeit_not_verfuegbar__rev_Failure" |
| AWV_1_Freigabe | 16 | 1 | Intel | Bool | 1 | 0 | - | - | 0="nicht_freigegeben", 1="freigegeben" |
| AWV_1_Prefill | 17 | 1 | Intel | Bool | 1 | 0 | - | - | 0="keine_Prefill_Anf", 1="Prefill_Anf" |
| AWV_1_Parameter | 18 | 2 | Intel | U2 | 1 | 0 | - | - | 0="Defaultparametersatz", 1="Par_leicht_erh_Empf", 2="Par_erh_Empf", 3="Par_hoechster_Empf" |
| AWV_only | 20 | 1 | Intel | Bool | 1 | 0 | - | - | 0="ACC_und_AWV_verbaut", 1="AWV_ohne_ACC_verbaut" |
| AWV_CityANB_Auspraegung | 21 | 1 | Intel | Bool | 1 | 0 | - | - | 0="autom_Bremsung_im_ges_vBereich", 1="autom_Bremsung_im_def_vBereich" |
| AWV_Halten | 22 | 1 | Intel | Bool | 1 | 0 | - | - | 0="keine_Anforderung", 1="Anforderung_das_Fzg_im_Stillstand_zu_halten" |
| ANB_Teilbremsung_Freigabe | 23 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Teilbremsung_nicht_freigegeben", 1="Teilbremsung_freigegeben" |
| AWV_2_Status | 24 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Lampe_aus", 1="Lampe_ein" |
| AWV_2_Fehler | 25 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Lampe_aus", 1="Lampe_ein" |
| AWV_2_SU_Warnzeit | 26 | 2 | Intel | U2 | 1 | 0 | - | - | 0="frueh", 1="normal", 2="spaet", 3="adaptiv" |
| AWV_2_SU_Bremsruck | 28 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Bremsruck_deaktiviert", 1="Bremsruck_aktiviert" |
| AWV_2_SU_Gong | 29 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Gong_deaktiviert", 1="Gong_aktiviert" |
| AWV_2_SU_Lampe | 30 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Lampe_deaktiviert", 1="Lampe_aktiviert" |
| AWV_2_Umfeldwarn | 31 | 1 | Intel | Bool | 1 | 0 | - | - | 0="keine_Warnung", 1="Warnung" |
| AWV_2_Freigabe | 32 | 1 | Intel | Bool | 1 | 0 | - | - | 0="keine_Ruckfreigabe", 1="Ruckfreigabe" |
| AWV_2_Ruckprofil | 33 | 3 | Intel | U3 | 1 | 0 | - | - | 0="kein_Ruck", 1="Ruckprofil_1", 2="Ruckprofil_2", 3="Ruckprofil_3", 4="Ruckprofil_4", 5="Ruckprofil_5", 6="not_erlaubt", 7="not_erlaubt" |
| AWV_2_Warnton | 36 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Aus", 1="Ein" |
| AWV_2_Warnsymbol | 37 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Aus", 1="Ein" |
| AWV_Infoton | 38 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Aus", 1="Ein" |
| AWV_2_Gurtstraffer | 39 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Gurt_not_straffen", 1="Gurt_straffen" |
| AWV_Konfiguration_Menueanf | 40 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Menue_deaktivieren", 1="Menue_aktivieren" |
| AWV_Konfiguration_Vorw_Menueanf | 41 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Menue_deaktivieren", 1="Menue_aktivieren" |
| AWV_Konfiguration_Status | 42 | 1 | Intel | Bool | 1 | 0 | - | - | 0="AWV_inaktiv", 1="AWV_aktiv" |
| AWV_Konfiguration_Vorw_Status | 43 | 1 | Intel | Bool | 1 | 0 | - | - | 0="AWV_Vorwarnung_inaktiv", 1="AWV_Vorwarnung_aktiv" |
| AWV_2_Abstandswarnung | 51 | 1 | Intel | Bool | 1 | 0 | - | - | 0="kein_Warnhinweis", 1="Warnhinweis" |
| ANB_Zielbremsung_Freigabe | 52 | 1 | Intel | Bool | 1 | 0 | - | - | 0="Zielbremsung_nicht_freigegeben", 1="Zielbremsung_freigegeben" |
| ANB_CM_Anforderung | 53 | 1 | Intel | Bool | 1 | 0 | - | - | 0="keine_Anforderung", 1="Anforderung_aktiv" |
| ANB_Ziel_Teilbrems_Verz_Anf | 54 | 10 | Intel | U10 | 0.024 | -20.016 | Unit_MeterPerSeconSquar | - | - |

## CAN ID 0x5BE - LDW_Status

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| LDW_Lernmodus_rechts | 0 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| LDW_Lernmodus_links | 2 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| LDW_Lernmodus | 9 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| LDW_Textbits | 12 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| LDW_Gong | 16 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| LDW_Kameratyp | 18 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LDW_Lampe_gelb | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LDW_Lampe_gruen | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LDW_SW_Warnung_links | 21 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LDW_SW_Warnung_rechts | 22 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LDW_KD_Fehler | 23 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LDW_DLC | 24 | 8 | Intel | U8 | 0.01 | -1.25 | - | - | - |
| LDW_TLC | 32 | 5 | Intel | U5 | 0.1 | 0 | - | - | - |
| LDW_Seite_DLCTLC | 37 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| LDW_Frueh_Spaet | 38 | 2 | Intel | U2 | 1 | 0 | - | - | - |

## CAN ID 0x1AC - Bremse_8

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| BR8_Checksumme | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| BR8_Zaehler | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| BR8_Sta_ACC_Anf | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_Verz_EPB_akt | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_Sta_Br_temp | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_Sta_Br_Druck | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_TolAbgl_HL | 16 | 8 | Intel | U8 | 0.048828125 | -6.201171875 | Unit_PerCent | - | - |
| BR8_TolAbgl_HR | 24 | 8 | Intel | U8 | 0.048828125 | -6.201171875 | Unit_PerCent | - | - |
| BR8_Istbeschl | 32 | 9 | Intel | U9 | 0.02 | -7.22 | Unit_MeterPerSeconSquar | - | - |
| BR8_Sta_HW_BLS | 41 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_QB_LBeschl | 42 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_ESC_Mode | 43 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| BR8_aktBrSyst | 45 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_Fa_bremst | 46 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_StaBrSyst | 47 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_Laengsbeschl | 48 | 10 | Intel | U10 | 0.03125 | -16 | Unit_MeterPerSeconSquar | - | - |
| BR8_Sta_ADR_BR | 58 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_Quattro | 59 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_Sta_VerzReg | 60 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_Sta_BLS | 61 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_Verz_EPB | 62 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BR8_Check_EPB | 63 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x3A0 - Bremse_10

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| B10_Checksumme | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| B10_Zaehler | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| B10_QB_Wegimp_VL | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B10_QB_Wegimp_VR | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B10_QB_Wegimp_HL | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B10_QB_Wegimp_HR | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B10_Wegimp_VL | 16 | 10 | Intel | U10 | 1 | 0 | - | - | - |
| B10_Wegimp_VR | 26 | 10 | Intel | U10 | 1 | 0 | - | - | - |
| B10_Wegimp_HL | 36 | 10 | Intel | U10 | 1 | 0 | - | - | - |
| B10_Wegimp_HR | 46 | 10 | Intel | U10 | 1 | 0 | - | - | - |
| B10_QB_Fahrtr_VL | 56 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B10_QB_Fahrtr_VR | 57 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B10_QB_Fahrtr_HL | 58 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B10_QB_Fahrtr_HR | 59 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B10_Fahrtr_VL | 60 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B10_Fahrtr_VR | 61 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B10_Fahrtr_HL | 62 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B10_Fahrtr_HR | 63 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x343 - RDK_Status

- DLC: 3  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| RKS_Reifen_VL | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Reifen_VR | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Reifen_HL | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Reifen_HR | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Reifen_RR | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Warnung_2 | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Warnung_1 | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Systemfehler | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Kalibrier_abgew | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Druckdiff_Vorn | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Druckdiff_Hinten | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Befuellung_RR_low | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Funkstoerung | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_System_Aus | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_KD_Fehler | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Lampe | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Ton | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Gong | 18 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_RDK_Blinkbit | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| RKS_Teillast | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x392 - Gate_Komf_2

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| GK2_Sta_LSM | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Sta_Lichtsensor | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Sta_Licht1 | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Sta_VSG | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Sta_Schluessel | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Sta_Profil | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Sta_Clima2 | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Sta_BSG4 | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Sta_Kessy_4 | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BS4_Gleitende_Leuchtw_Anf | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| BS4_GLW_Fernlicht_Anf | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Blk_L_Kontrolle | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Blk_R_Kontrolle | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_LS_KomFehler | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_LS_def | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Helligkeit | 16 | 3 | Intel | U3 | 714.286 | 0 | Unit_Lux | - | - |
| GK2_VD_zu_ver | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_VD_entriegelt | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_VD_offen_ver | 21 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Verdeck_Anf | 22 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_VDKD_auf | 23 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Schluessel | 24 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| GK2_Hardtop | 28 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_AFL_Schalter | 29 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Nebelschluss | 30 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_EM_LIN_ungueltig | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| GK2_Profil | 32 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| GK2_Kl_StSt_Info | 36 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| GK2_BSG_StSt_Info | 38 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| GK2_BEM_P_Generator | 40 | 8 | Intel | U8 | 50 | 0 | Unit_Watt | - | - |
| GK2_BEM_Abschaltstufen | 48 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| GK2_BEM_DFM | 51 | 5 | Intel | U5 | 3.225 | 0.025 | Unit_PerCent | - | - |
| GK2_Kessy_StSt_Info | 56 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| GK2_BEM_StSt_Info | 58 | 2 | Intel | U2 | 1 | 0 | - | - | - |

## CAN ID 0x3BA - SWA_1

- DLC: 8  TX: SWA  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| SWA_Textbits | 12 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| SWA_Gong | 16 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| SWA_Sta_passiv | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| SWA_Sta_aktiv | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| SWA_Infostufe_SWA_li | 26 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| SWA_Warnung_SWA_li | 27 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| SWA_Infostufe_SWA_re | 42 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| SWA_Warnung_SWA_re | 43 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| SWA_KD_Fehler | 59 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x497 - Parkhilfe_01

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| PH_Abschaltursache | 13 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| PH_Opt_Anzeige_V_ein | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PH_Opt_Anzeige_H_ein | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PH_Opt_Anz_V_Hindernis | 18 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PH_Opt_Anz_H_Hindernis | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PH_Tongeber_V_aktiv | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PH_Tongeber_H_aktiv | 21 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PH_Tongeber_mute | 22 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PH_Anf_Audioabsenkung | 23 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PH_Frequenz_hinten | 32 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| PH_Lautstaerke_hinten | 36 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| PH_Frequenz_vorn | 40 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| PH_Lautstaerke_vorn | 44 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| PH_Trigger_Bildaufschaltung | 48 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PH_StartStopp_Info | 49 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| PH_Aufbauten_erk | 51 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PH_BerErk_vorn | 52 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| PH_BerErk_hinten | 54 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| PH_defekt | 56 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PH_gestoert | 57 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| PH_Systemzustand | 58 | 3 | Intel | U3 | 1 | 0 | - | - | - |
| PH_Display_Kundenwunsch | 61 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| PH_KD_Fehler | 63 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x5B7 - Bremse_11

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| CHECKSUM | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| COUNTER | 8 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| B11_HydHalten | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B11_Br_StSt_Info | 14 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| B11_OBD_Nib_VL | 16 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| B11_OBD_Nib_VR | 20 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| B11_OBD_Nib_HL | 24 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| B11_OBD_Nib_HR | 28 | 4 | Intel | U4 | 1 | 0 | - | - | - |
| B11_EPB_Steller_akt | 32 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| B11_EPB_Steller_gue | 33 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x5DC - Soll_Verbauliste_neu

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| VL1_Motor_SG | 0 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Getr_SG | 1 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_ABS | 2 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Kombi | 3 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_LSM | 4 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Airbag | 5 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Lenkhilfe | 6 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_dyn_LWR | 7 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_res_08 | 8 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Allrad | 9 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_ADR | 10 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_ADR_getrennt | 11 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_EPB | 12 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_res_13 | 13 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Daempfer | 14 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Quersperre | 15 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_MotorSlave | 16 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_SWA | 17 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_HCA | 18 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_RKA_Plus | 19 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_PLA | 20 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_WFS_KBI | 21 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Kombi_KBI | 22 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Soll_eq_Ist | 23 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_BSG_Komf | 24 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_ZKE | 25 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_TSG_FT | 26 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_TSG_BT | 27 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_TSG_HL | 28 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_TSG_HR | 29 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Memory | 30 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Dachmodul_K | 31 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Zentralelektrik_II | 32 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_RDK | 33 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Lenksaeule | 34 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Gateway | 35 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Clima_Komf | 36 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Einparkhilfe | 37 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_PTC_Heizung | 38 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Standheiz | 39 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Verdeck | 40 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_RSE_I | 41 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_res_42 | 42 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_MDI_I | 43 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Anhaenger | 44 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Memory_BF | 45 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Easy_Entry_VF | 46 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Easy_Entry_VB | 47 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Heckdeckel | 48 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Rearview | 49 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Sonderfzg_SG | 50 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Tastenmodul | 51 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Kompass | 52 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_WFS_K | 53 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_GSM_Pager | 54 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_InfoElektronik | 55 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_DSP | 56 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_DAB | 57 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Telematik | 58 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Navigation | 59 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_TV_Tuner | 60 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Neigungsmodul_I | 61 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Radio | 62 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| VL1_Telefon | 63 | 1 | Intel | Bool | 1 | 0 | - | - | - |

## CAN ID 0x5D2 - Ident

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| IDT_Mux M | 0 | 2 | Intel | U2 | 1 | 0 | - | - | - |
| IDT_Geheimnis_1 m0 | 8 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_4 m1 | 8 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_11 m2 | 8 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_Geheimnis_2 m0 | 16 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_5 m1 | 16 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_12 m2 | 16 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_Geheimnis_3 m0 | 24 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_6 m1 | 24 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_13 m2 | 24 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_Geheimnis_4 m0 | 32 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_7 m1 | 32 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_14 m2 | 32 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_1 m0 | 40 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_8 m1 | 40 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_15 m2 | 40 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_2 m0 | 48 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_9 m1 | 48 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_16 m2 | 48 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_3 m0 | 56 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_10 m1 | 56 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| IDT_VIN_17 m2 | 56 | 8 | Intel | U8 | 1 | 0 | - | - | - |

## CAN ID 0x7D0 - Diagnose_1

- DLC: 8  TX: XXX  Cycle: -

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |
| DI1_VerlernZaehl | 0 | 8 | Intel | U8 | 1 | 0 | - | - | - |
| DI1_km_Stand | 8 | 20 | Intel | U20 | 1 | 0 | Unit_KiloMeter | - | - |
| DI1_Jahr | 28 | 7 | Intel | U7 | 1 | 2000 | Unit_Year | - | - |
| DI1_Monat | 35 | 4 | Intel | U4 | 1 | 0 | Unit_Month | - | - |
| DI1_Tag | 39 | 5 | Intel | U5 | 1 | 0 | Unit_Day | - | - |
| DI1_Stunde | 44 | 5 | Intel | U5 | 1 | 0 | Unit_Hours | - | - |
| DI1_Minute | 49 | 6 | Intel | U6 | 1 | 0 | Unit_Minut | - | - |
| DI1_Sekunde | 55 | 6 | Intel | U6 | 1 | 0 | Unit_Secon | - | - |
| DI1_KM_Stand_alt | 62 | 1 | Intel | Bool | 1 | 0 | - | - | - |
| DI1_Zeit_alt | 63 | 1 | Intel | Bool | 1 | 0 | - | - | - |
