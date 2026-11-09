#ifndef VW_PQ_MESSAGES_H
#define VW_PQ_MESSAGES_H

#include <Arduino.h>
#include <ACAN2515.h>

namespace params {

struct VwPqSignals {
    // 0x572 ZAS_1
    bool zas_1_572_fehlerspeichereintrag_zas = false;
    float zas_1_572_frei_zas_1_3 = 0.0f;
    bool zas_1_572_frei_zas_1_2 = false;
    bool zas_1_572_klemme_15_sv = false;
    bool zas_1_572_frei_zas_1_1 = false;
    bool zas_1_572_klemme_p_parklichtstellung = false;
    bool zas_1_572_klemme_50_starten = false;
    bool zas_1_572_klemme_x_startvorgang = false;
    bool zas_1_572_klemme_15_z_ndung_ein = false;
    bool zas_1_572_s_kontakt_schl_ssel_steckt = false;

    // 0x538 Wischer_1
    bool wischer_1_538_blockierung_heckwischer_erkannt = false;
    float wischer_1_538_frei_wischer_1_2 = 0.0f;
    bool wischer_1_538_fehlerspeichereintrag_wischer = false;
    bool wischer_1_538_ansteuerung_scheibenwischer_hec = false;
    bool wischer_1_538_ansteuerung_wascher_heck = false;
    bool wischer_1_538_scheibenwischer_heck_eingeschal = false;
    bool wischer_1_538_blockierung_frontwischer_erkann = false;
    bool wischer_1_538_frei_wischer_1_1 = false;
    bool wischer_1_538_status_waschduesenheizung = false;
    bool wischer_1_538_parklage_frontwischer = false;
    bool wischer_1_538_ansteuerung_frontwischer_schnel = false;
    bool wischer_1_538_ansteuerung_frontwischer_normal = false;
    bool wischer_1_538_ansteuerung_wascher_front = false;
    bool wischer_1_538_frontwischer_eingeschaltet = false;

    // 0x5B8 WFS_1
    float wfs_1_5b8_wfs_textbits = 0.0f;
    float wfs_1_5b8_frei_wfs_1_1 = 0.0f;
    bool wfs_1_5b8_wfs_led = false;

    // 0x448 Waehlhebel_1
    float waehlhebel_1_448_frei_waehlhebel_1_1 = 0.0f;
    bool waehlhebel_1_448_test_aktiv_flag = false;
    float waehlhebel_1_448_zaehler_waehlhebel_1 = 0.0f;
    float waehlhebel_1_448_waehlhebel_testergebnis = 0.0f;
    float waehlhebel_1_448_fehler_waehlhebel = 0.0f;
    float waehlhebel_1_448_waehlhebelposition = 0.0f;
    bool waehlhebel_1_448_waehlhebel_initialisierung = false;
    float waehlhebel_1_448_shiftlock_position = 0.0f;

    // 0x5D8 Verbauliste_1
    bool verbauliste_1_5d8_tv_tuner = false;
    bool verbauliste_1_5d8_dsp = false;
    bool verbauliste_1_5d8_cd_wechsler = false;
    bool verbauliste_1_5d8_spracheingabe = false;
    bool verbauliste_1_5d8_telematik = false;
    bool verbauliste_1_5d8_navigation = false;
    bool verbauliste_1_5d8_telefon = false;
    bool verbauliste_1_5d8_radio = false;
    bool verbauliste_1_5d8_mmi_vorne = false;
    bool verbauliste_1_5d8_mmi_hinten = false;
    bool verbauliste_1_5d8_frei_verbauliste_1_11 = false;
    bool verbauliste_1_5d8_klimabedienteil_hl = false;
    float verbauliste_1_5d8_frei_verbauliste_1_10 = 0.0f;
    bool verbauliste_1_5d8_tankgeber = false;
    bool verbauliste_1_5d8_assistenzfahrlicht = false;
    bool verbauliste_1_5d8_memory_hinter_fahrer = false;
    bool verbauliste_1_5d8_sitzmemory_hinten = false;
    bool verbauliste_1_5d8_sitzmemory_beifahrer = false;
    bool verbauliste_1_5d8_anh_ngersteuergeraet = false;
    bool verbauliste_1_5d8_energiemanagement = false;
    bool verbauliste_1_5d8_wischermodul = false;
    bool verbauliste_1_5d8_ezs_kessy_komfort = false;
    bool verbauliste_1_5d8_verdecksteuergeraet = false;
    bool verbauliste_1_5d8_standheizung = false;
    bool verbauliste_1_5d8_frei_verbauliste_1_9 = false;
    bool verbauliste_1_5d8_einparkhilfe = false;
    bool verbauliste_1_5d8_klimasteuergeraet_komfort = false;
    bool verbauliste_1_5d8_gateway = false;
    bool verbauliste_1_5d8_lenksaeulenmodul = false;
    bool verbauliste_1_5d8_reifendruck = false;
    bool verbauliste_1_5d8_kombiinstrument_komfort = false;
    bool verbauliste_1_5d8_dachmodul = false;
    bool verbauliste_1_5d8_memory = false;
    bool verbauliste_1_5d8_tsg_hr = false;
    bool verbauliste_1_5d8_tsg_hl = false;
    bool verbauliste_1_5d8_tsg_bt = false;
    bool verbauliste_1_5d8_tsg_ft = false;
    bool verbauliste_1_5d8_zke = false;
    bool verbauliste_1_5d8_bsg_komfort = false;
    float verbauliste_1_5d8_frei_verbauliste_1_8 = 0.0f;
    bool verbauliste_1_5d8_stabi_entkopplung = false;
    bool verbauliste_1_5d8_waehlhebel = false;
    bool verbauliste_1_5d8_batteriemanager = false;
    bool verbauliste_1_5d8_daempfer_sg = false;
    bool verbauliste_1_5d8_niveauregulierung = false;
    bool verbauliste_1_5d8_ezs_kessy_antrieb = false;
    bool verbauliste_1_5d8_bremsbooster = false;
    bool verbauliste_1_5d8_lenkhilfe = false;
    bool verbauliste_1_5d8_kombi_antrieb = false;
    bool verbauliste_1_5d8_einspritzpumpe = false;
    bool verbauliste_1_5d8_lenkwinkel = false;
    bool verbauliste_1_5d8_allrad = false;
    bool verbauliste_1_5d8_bsg_antrieb = false;
    bool verbauliste_1_5d8_adr = false;
    bool verbauliste_1_5d8_airbag = false;
    bool verbauliste_1_5d8_climatronic_antrieb = false;
    bool verbauliste_1_5d8_abs = false;
    bool verbauliste_1_5d8_getriebesteuergeraet = false;
    bool verbauliste_1_5d8_motorsteuergeraet = false;

    // 0x5D0 Systeminfo_1
    float systeminfo_1_5d0_can_stand_4_1_antrieb_daten_hau = 0.0f;
    float systeminfo_1_5d0_can_stand_4_1_antrieb_daten_neb = 0.0f;
    float systeminfo_1_5d0_can_stand_4_1_komfort_daten_hau = 0.0f;
    float systeminfo_1_5d0_can_stand_4_1_komfort_daten_neb = 0.0f;
    bool systeminfo_1_5d0_frei_systeminfo_1_6 = false;
    bool systeminfo_1_5d0_viertuerer = false;
    bool systeminfo_1_5d0_rechtslenker = false;
    float systeminfo_1_5d0_fahrzeug_index = 0.0f;
    float systeminfo_1_5d0_fahrzeug_generation = 0.0f;
    float systeminfo_1_5d0_fahrzeug_derivat = 0.0f;
    float systeminfo_1_5d0_fahrzeug_marke_2 = 0.0f;
    float systeminfo_1_5d0_fahrzeug_klasse = 0.0f;
    bool systeminfo_1_5d0_verbauinformation_gueltig = false;
    bool systeminfo_1_5d0_can_infotainment_verbaut = false;
    bool systeminfo_1_5d0_can_infotainment_in_diagnose = false;
    bool systeminfo_1_5d0_sleep_can_infotainment = false;
    bool systeminfo_1_5d0_can_komfort_in_diagnose = false;
    bool systeminfo_1_5d0_sleep_can_komfort = false;
    bool systeminfo_1_5d0_can_antrieb_in_diagnose = false;
    bool systeminfo_1_5d0_can_extern_zugeschaltet = false;

    // 0x10C Slave_1
    bool slave_1_10c_delta_reduziertes_sollmoment = false;
    float slave_1_10c_delta_drosselklappenwinkel = 0.0f;
    float slave_1_10c_frei_slave_1_1 = 0.0f;
    bool slave_1_10c_delta_zuendwinkelausgabe = false;
    bool slave_1_10c_ubat_freigabe_dve_slave = false;
    bool slave_1_10c_drosselklappe_sollwertbegrenzun = false;
    bool slave_1_10c_einspritzverbot_lernen_slave = false;
    bool slave_1_10c_master_erkannt_slave = false;
    bool slave_1_10c_fehler_momentenausgabe = false;
    bool slave_1_10c_fehler_getriebe_moment = false;
    bool slave_1_10c_fehler_bremsenbotschaft = false;
    float slave_1_10c_indiziertes_istmoment_slave = 0.0f;
    bool slave_1_10c_timeout_bremsenbotschaft = false;

    // 0x534 Sitz_info
    bool sitz_info_534_positionserkennung_beifahrersit = false;
    bool sitz_info_534_beifahrersitz_im_vorderen_dritt = false;
    bool sitz_info_534_positionserkennung_fahrersitz_u = false;
    bool sitz_info_534_fahrersitz_im_vorderen_drittel = false;
    float sitz_info_534_zaehler_sitzinfo = 0.0f;
    float sitz_info_534_checksumme_sitzinfo = 0.0f;

    // 0x712 PSG_3
    float psg_3_712_selbsttestergebnis = 0.0f;

    // 0x512 PSG_2
    float psg_2_512_ram_inhalt_4 = 0.0f;
    float psg_2_512_ram_inhalt_3 = 0.0f;
    float psg_2_512_ram_inhalt_2 = 0.0f;
    float psg_2_512_ram_inhalt_1 = 0.0f;

    // 0x112 PSG_1
    float psg_1_112_pumpentemperatur_2_1 = 0.0f;
    float psg_1_112_pumpentemperatur_3_2_2 = 0.0f;
    float psg_1_112_zylinderzaehler_3_2_2 = 0.0f;
    float psg_1_112_ansteuerdauer_3_2_2 = 0.0f;
    float psg_1_112_nockenwellendrehzahl_3_2_2 = 0.0f;
    float psg_1_112_pumpen_statuswort_3_2_2 = 0.0f;

    // 0x514 NOX_1
    float nox_1_514_obd_fuer_nox = 0.0f;
    float nox_1_514_frei_nox1_4 = 0.0f;
    bool nox_1_514_ip2 = false;
    bool nox_1_514_ip1 = false;
    bool nox_1_514_ip0 = false;
    bool nox_1_514_sondenheizung_nox = false;
    bool nox_1_514_offsetkorrektur_nox = false;
    float nox_1_514_frei_nox1_3 = 0.0f;
    float nox_1_514_sauerstoff_binaer = 0.0f;
    float nox_1_514_frei_nox1_2 = 0.0f;
    float nox_1_514_sauerstoff_linear = 0.0f;
    float nox_1_514_frei_nox1_1 = 0.0f;
    float nox_1_514_nox_signal = 0.0f;

    // 0x590 Niveau_1
    float niveau_1_590_beladungszustand = 0.0f;
    bool niveau_1_590_fehlerspeichereintrag_niveau_1 = false;
    bool niveau_1_590_systemstatus_niveau_1 = false;
    bool niveau_1_590_reserve_fahrzeugart = false;
    bool niveau_1_590_fahrzeugart_niveau = false;
    float niveau_1_590_textbits_niveau_1 = 0.0f;
    bool niveau_1_590_verstellung_hl = false;
    bool niveau_1_590_verstellung_hr = false;
    bool niveau_1_590_verstellung_vl = false;
    bool niveau_1_590_verstellung_vr = false;
    bool niveau_1_590_absenkung_fahrzeug = false;
    bool niveau_1_590_anhebung_fahrzeug = false;
    bool niveau_1_590_verstellung_aktiv = false;
    bool niveau_1_590_kompressorlauf_in_kuerze = false;
    bool niveau_1_590_frei_niveau_1_5 = false;
    bool niveau_1_590_taster_niveau = false;
    bool niveau_1_590_parkniveau = false;
    bool niveau_1_590_zwischenniveau = false;
    float niveau_1_590_niveaustati = 0.0f;
    bool niveau_1_590_msg_einschraenkung = false;
    bool niveau_1_590_esp_beeinflussung = false;
    bool niveau_1_590_warnlampe_niveau_1 = false;
    bool niveau_1_590_frei_niveau_1_1 = false;
    float niveau_1_590_zaehler_niveau_1 = 0.0f;
    float niveau_1_590_checksumme_niveau_1 = 0.0f;

    // 0x530 Navigation_1
    float navigation_1_530_kreuzungstyp = 0.0f;
    float navigation_1_530_entfernung_bis_kreuzung = 0.0f;
    float navigation_1_530_entfernung_bis_kurvenanfang = 0.0f;
    bool navigation_1_530_voarusliegende_kurvenrichtung = false;
    float navigation_1_530_vorausliegender_kurvenverlauf = 0.0f;
    bool navigation_1_530_fehler_navigation = false;
    float navigation_1_530_anzahl_fahrbahnen_0_ist_unguel = 0.0f;
    float navigation_1_530_strassentyp = 0.0f;
    float navigation_1_530_laenderkennung = 0.0f;
    bool navigation_1_530_vorzeichen_gierrate = false;
    bool navigation_1_530_gierratenfehler = false;
    float navigation_1_530_gierrate = 0.0f;

    // 0x700 MSG_3
    float msg_3_700_msg_konfiguration = 0.0f;
    float msg_3_700_lage_des_ot_impuls = 0.0f;

    // 0x500 MSG_2
    float msg_2_500_ram_adresse_4 = 0.0f;
    float msg_2_500_ram_adresse_3 = 0.0f;
    float msg_2_500_ram_adresse_2 = 0.0f;
    float msg_2_500_ram_adresse_1 = 0.0f;

    // 0x100 MSG_1
    float msg_1_100_kurbelwellendrehzahl_3_2_2 = 0.0f;
    float msg_1_100_soll_foerderbeginn_kw_3_2_2 = 0.0f;
    float msg_1_100_soll_foerderbeginn_nw_3_2_2 = 0.0f;
    float msg_1_100_soll_voreinspritzung = 0.0f;
    float msg_1_100_soll_einspritzmenge = 0.0f;

    // 0x704 Motor_NOX
    float motor_nox_704_frei_motor_nox_1_2 = 0.0f;
    float motor_nox_704_frei_motor_nox_1_1 = 0.0f;
    bool motor_nox_704_heizleistungsanforderung = false;
    bool motor_nox_704_offsetkorrektur_moeglich = false;
    bool motor_nox_704_betriebsbereich = false;
    float motor_nox_704_abgastemperatur_nox = 0.0f;
    float motor_nox_704_abgasdruck_nox = 0.0f;

    // 0x384 Motor_Momente
    float motor_momente_384_momentenangaben_ungenau_moment = 0.0f;

    // 0x580 Motor_Flexia
    bool motor_flexia_580_ansaugsystem_m0 = false;
    float motor_flexia_580_hubraum_m0 = 0.0f;
    float motor_flexia_580_steigung_der_befuellungskennlin_m1 = 0.0f;
    float motor_flexia_580_anzahl_zylinder_m0 = 0.0f;
    float motor_flexia_580_bewertungsfaktor_russindex_turb_m1 = 0.0f;
    float motor_flexia_580_anzahl_ventile_m0 = 0.0f;
    float motor_flexia_580_bewertungsfaktor_verschleissind_m1 = 0.0f;
    float motor_flexia_580_hersteller_code_m1 = 0.0f;
    float motor_flexia_580_motorleistung_m0 = 0.0f;
    float motor_flexia_580_max_drehmoment_m0 = 0.0f;
    float motor_flexia_580_normierter_verbrauch_m1 = 0.0f;
    float motor_flexia_580_oelniveauschwelle_m1 = 0.0f;
    float motor_flexia_580_drehzahl_maxnorm_m0 = 0.0f;
    float motor_flexia_580_verschleissindex = 0.0f;
    float motor_flexia_580_russindex = 0.0f;
    bool motor_flexia_580_verbrennungsart = false;
    bool motor_flexia_580_frei_motor_flexia_1 = false;
    bool motor_flexia_580_warm_up_cycle = false;
    bool motor_flexia_580_driving_cycle = false;
    float motor_flexia_580_zaehler_motor_flexia = 0.0f;
    bool motor_flexia_580_multiplex_schalter_motor_flexia_m = false;

    // 0x588 Motor_7
    float motor_7_588_oltemperatur = 0.0f;
    float motor_7_588_frei_motor_7_3 = 0.0f;
    float motor_7_588_ladedruck = 0.0f;
    bool motor_7_588_vorzeichen_motordrehzahlgradien = false;
    float motor_7_588_motordrehzahlgradient = 0.0f;
    float motor_7_588_hoeheninfo_motor_7 = 0.0f;
    float motor_7_588_klemme_dfm = 0.0f;
    float motor_7_588_ptc_gluehstifte_ausgeschaltet = 0.0f;
    bool motor_7_588_frei_motor_7_1 = false;
    bool motor_7_588_fehlerspeichereintrag_motor_7 = false;
    bool motor_7_588_statusbit_geschwindikeitsbegren = false;
    bool motor_7_588_geschwindigkegrenzung_aktivierb = false;
    bool motor_7_588_leerlauf_solldrehzahl_auf_max_w = false;

    // 0x488 Motor_6
    float motor_6_488_zaehler_motor_6 = 0.0f;
    float motor_6_488_frei_motor_6_4 = 0.0f;
    bool motor_6_488_ltemperaturschutz = false;
    bool motor_6_488_gra_bremseingriff_freigabe = false;
    float motor_6_488_frei_motor_6_3 = 0.0f;
    float motor_6_488_ruckmeldung_momenten = 0.0f;
    float motor_6_488_gra_sollbeschleunigung = 0.0f;
    float motor_6_488_hoeheninfo_motor_6 = 0.0f;
    float motor_6_488_istmoment_f_r_getriebe = 0.0f;
    float motor_6_488_sollmoment_f_r_getriebe = 0.0f;
    float motor_6_488_checksumme_motor_6 = 0.0f;

    // 0x480 Motor_5
    float motor_5_480_mo5_mp_code_m = 0.0f;
    float motor_5_480_mo5_max_moment_m0 = 0.0f;
    float motor_5_480_mo5_drehzahl_m1 = 0.0f;
    float motor_5_480_mo5_motortyp_m2 = 0.0f;
    bool motor_5_480_mo5_abgastyp_m3 = false;
    bool motor_5_480_mo5_abgastyp2_m3 = false;
    bool motor_5_480_mo5_abgastyp3_m3 = false;
    bool motor_5_480_obd_kaltstart_denominator_m3 = false;
    bool motor_5_480_obd_minimum_trip_m3 = false;
    bool motor_5_480_mo5_dpf_reg_m3 = false;
    bool motor_5_480_mo5_vorgluehen = false;
    bool motor_5_480_mo5_e_gas = false;
    bool motor_5_480_mo5_obd_2 = false;
    bool motor_5_480_mo5_heissl = false;
    bool motor_5_480_mo5_klimakompr = false;
    bool motor_5_480_mo5_feld_kuehl = false;
    bool motor_5_480_mo5_kliko_red = false;
    float motor_5_480_mo5_verbrauch = 0.0f;
    bool motor_5_480_mo5_ueberlverb = false;
    float motor_5_480_mo5_luefter = 0.0f;
    bool motor_5_480_mo5_hleuchte = false;
    bool motor_5_480_mo5_partikellamp = false;
    bool motor_5_480_mo5_sta_bku = false;
    bool motor_5_480_mo5_typstartsteu = false;
    bool motor_5_480_mo5_tde_lampe = false;
    bool motor_5_480_mo5_tde_text = false;
    bool motor_5_480_mo5_dzm_daempf = false;
    bool motor_5_480_mo5_interlock = false;
    bool motor_5_480_mo5_start = false;
    bool motor_5_480_mo5_anlasser = false;
    bool motor_5_480_mo5_gra_hauptsch = false;
    bool motor_5_480_mo5_momente = false;
    bool motor_5_480_mo5_motortext1 = false;
    bool motor_5_480_mo5_motortext2 = false;
    bool motor_5_480_mo5_motortext3 = false;
    bool motor_5_480_mo5_motortext4 = false;
    float motor_5_480_checksum = 0.0f;

    // 0x380 Motor_3
    bool motor_3_380_mo3_vorgluehen = false;
    bool motor_3_380_mo3_prio_dz = false;
    bool motor_3_380_mo_schalter_startstopp = false;
    bool motor_3_380_mo3_winterprg = false;
    bool motor_3_380_mo3_sta_pedal = false;
    bool motor_3_380_mo3_sta_fpg_m = false;
    bool motor_3_380_mo3_sign_fpg_m1 = false;
    bool motor_3_380_mo3_sta_temp = false;
    float motor_3_380_mo3_offsentemp = 0.0f;
    float motor_3_380_mo3_pedalwert = 0.0f;
    float motor_3_380_mo3_rad_wu_mo = 0.0f;
    bool motor_3_380_mo3_vorz_radwu = false;
    bool motor_3_380_mo3_freigabe_segeln = false;
    bool motor_3_380_mo_startstopp_stoppvorbereitung = false;
    float motor_3_380_mo3_dz_beeinfl = 0.0f;
    float motor_3_380_mo3_wunschdz = 0.0f;
    float motor_3_380_mo3_dkw_m0 = 0.0f;
    float motor_3_380_mo3_fpgradient_m1 = 0.0f;

    // 0x288 Motor_2
    float motor_2_288_mo2_mp_code_m = 0.0f;
    float motor_2_288_mo2_getr_code_m2 = 0.0f;
    float motor_2_288_mo2_max_mo_m3 = 0.0f;
    float motor_2_288_mo2_can_vers_m0 = 0.0f;
    float motor_2_288_mo2_motor_code_m1 = 0.0f;
    float motor_2_288_mo2_kuehlm_t = 0.0f;
    bool motor_2_288_mo2_bls = false;
    bool motor_2_288_mo2_bts = false;
    bool motor_2_288_mo2_sta_kuehlm = false;
    bool motor_2_288_mo2_sta_klima = false;
    bool motor_2_288_mo2_sta_no_bet = false;
    bool motor_2_288_mo2_status_tsk = false;
    float motor_2_288_mo2_sta_gra = 0.0f;
    bool motor_2_288_tsk_limiter_ausgewaehlt = false;
    bool motor_2_288_mo2_graregelt = false;
    bool motor_2_288_mo2_sport_error = false;
    bool motor_2_288_mo2_offroad = false;
    float motor_2_288_mo2_rme_gehalt = 0.0f;
    float motor_2_288_mo2_gra_soll = 0.0f;
    float motor_2_288_mo2_ll_solldz = 0.0f;
    float motor_2_288_mo2_begr_mo = 0.0f;
    float motor_2_288_mo2_mo_zwr = 0.0f;

    // 0x280 Motor_1
    float motor_1_280_fahrerwunschmoment = 0.0f;
    float motor_1_280_mechanisches_motor_verlustmomen = 0.0f;
    float motor_1_280_fahrpedalwert_oder_drosselklapp = 0.0f;
    float motor_1_280_inneres_motor_moment_ohne_exter = 0.0f;
    float motor_1_280_motordrehzahl = 0.0f;
    float motor_1_280_inneres_motor_moment = 0.0f;
    bool motor_1_280_momentenangaben_ungenau = false;
    bool motor_1_280_fehlerstatus_getriebe_momentene = false;
    bool motor_1_280_fehlerstatus_brems_momenteneing = false;
    bool motor_1_280_time_out_bremsen_botschaft = false;
    bool motor_1_280_kupplungsschalter = false;
    bool motor_1_280_kickdownschalter = false;
    bool motor_1_280_fahrpedalwert_ungenau_motor_1_m = false;
    bool motor_1_280_leergasinformation = false;

    // 0x106 Master_3
    float master_3_106_frei_master_3_1 = 0.0f;
    float master_3_106_motortemperatur_linearisiert = 0.0f;
    float master_3_106_indiziertes_sollmoment_f_r_vmax = 0.0f;
    float master_3_106_relative_momentenanforderung_de = 0.0f;
    float master_3_106_delta_motormoment_aus_verlustmo = 0.0f;

    // 0x104 Master_2
    bool master_2_104_ubat_freigabe_dve = false;
    bool master_2_104_dk_sollwertbegrenzung = false;
    bool master_2_104_einspritzverbot_dv_e_lernen = false;
    bool master_2_104_master_erkannt = false;
    bool master_2_104_vmax_begrenzung_aktiv = false;
    bool master_2_104_sa_verbot_von_fgr = false;
    float master_2_104_pedalwert_komplement = 0.0f;
    float master_2_104_zaehler_master_2 = 0.0f;
    float master_2_104_normierter_fahrpedalwinkel = 0.0f;

    // 0x102 Master_1
    float master_1_102_stationaere_solldrehzahl = 0.0f;
    float master_1_102_drehmoment_llr_i_anteil = 0.0f;
    float master_1_102_drehmoment_llr_pd_anteil = 0.0f;
    float master_1_102_drehmoment_llr_luftpfad = 0.0f;
    float master_1_102_frei_master_1_1 = 0.0f;
    bool master_1_102_llr_ist_aktiv = false;
    bool master_1_102_i_anteil_der_llr_aktiv = false;
    bool master_1_102_pd_anteil_der_llr_aktiv = false;
    bool master_1_102_llr_freigabe_nach_start = false;

    // 0x7C0 LWS_Kalibrierung
    bool lws_kalibrierung_7c0_frei_lws_kalibrierung_1_1 = false;
    float lws_kalibrierung_7c0_lws_identifier = 0.0f;
    float lws_kalibrierung_7c0_frei_lws_kalibrierung_1_2 = 0.0f;
    float lws_kalibrierung_7c0_command_dode_word = 0.0f;

    // 0x7C2 Lenkwinkel_Init
    float lenkwinkel_init_7c2_kodierbytes = 0.0f;
    float lenkwinkel_init_7c2_identiifier_f_r_lws_init = 0.0f;

    // 0x0C0 Lenkwinkel_1__RB_
    bool lenkwinkel_1_rb_0c0_vorzeichen_rb = false;
    float lenkwinkel_1_rb_0c0_lenkwinkel_rb = 0.0f;
    bool lenkwinkel_1_rb_0c0_lws_ok_rb = false;
    bool lenkwinkel_1_rb_0c0_lws_abgleich_rb = false;
    bool lenkwinkel_1_rb_0c0_frei_lenkwinkel_1_1_rb = false;
    bool lenkwinkel_1_rb_0c0_fine_check_rb = false;
    bool lenkwinkel_1_rb_0c0_coarse_check_rb = false;

    // 0x0C4 Lenkwinkel_1__ITT_
    bool lenkwinkel_1_itt_0c4_vorzeichen_itt = false;
    float lenkwinkel_1_itt_0c4_lenkwinkel_itt = 0.0f;
    bool lenkwinkel_1_itt_0c4_lws_ok = false;
    bool lenkwinkel_1_itt_0c4_lws_abgleich_itt = false;
    bool lenkwinkel_1_itt_0c4_lws_initialisierung_itt = false;
    bool lenkwinkel_1_itt_0c4_fine_check_itt = false;
    bool lenkwinkel_1_itt_0c4_coarse_check_itt = false;

    // 0x0C2 Lenkwinkel_1
    float lenkwinkel_1_0c2_lw1_lrw = 0.0f;
    bool lenkwinkel_1_0c2_lw1_lrw_sign = false;
    float lenkwinkel_1_0c2_lw1_lenk_gesch = 0.0f;
    bool lenkwinkel_1_0c2_lw1_gesch_sign = false;
    float lenkwinkel_1_0c2_lw1_id = 0.0f;
    bool lenkwinkel_1_0c2_lw1_initquelle = false;
    float lenkwinkel_1_0c2_lw1_status = 0.0f;
    bool lenkwinkel_1_0c2_lw1_sta_kl30 = false;
    float lenkwinkel_1_0c2_lw1_zaehler = 0.0f;
    float lenkwinkel_1_0c2_lw1_kodier = 0.0f;
    float lenkwinkel_1_0c2_lw1_crc = 0.0f;

    // 0x5DE Lenkhilfe_Fehler
    float lenkhilfe_fehler_5de_werkstattcode = 0.0f;
    float lenkhilfe_fehler_5de_multiplex_signal = 0.0f;
    bool lenkhilfe_fehler_5de_sensorcodierung_lenkhilfe = false;
    float lenkhilfe_fehler_5de_kennliniencodierung_lenkhilfe = 0.0f;
    bool lenkhilfe_fehler_5de_geber_f_lenkw_speicher_ausles = false;
    bool lenkhilfe_fehler_5de_kombiinstr_speicher_auslesen = false;
    bool lenkhilfe_fehler_5de_steuerger_speicher_auslesen = false;
    bool lenkhilfe_fehler_5de_lenkhilfe_steuergeraet_defekt = false;
    float lenkhilfe_fehler_5de_frei_lenkhilfe_1_3 = 0.0f;
    float lenkhilfe_fehler_5de_frei_lenkhilfe_1_4 = 0.0f;
    bool lenkhilfe_fehler_5de_geber_f_lenkwi_k_can_komm_sp = false;
    bool lenkhilfe_fehler_5de_geber_f_lenkwinkel_k_can_komm = false;
    bool lenkhilfe_fehler_5de_kombiinstr_k_can_kommunik_sp = false;
    bool lenkhilfe_fehler_5de_kombiinstrument_k_can_kommunik = false;
    bool lenkhilfe_fehler_5de_steuergeraet_k_can_kommunik_s = false;
    bool lenkhilfe_fehler_5de_steuergeraet_keine_can_kommunik = false;
    bool lenkhilfe_fehler_5de_lenkhilfe_spannung_ks_nach_mas = false;
    bool lenkhilfe_fehler_5de_lenkhilfe_spannung_ks_nach_mass = false;
    bool lenkhilfe_fehler_5de_lenkhilfe_spannung_ks_nach_s = false;
    bool lenkhilfe_fehler_5de_lenkhilfe_spannung_ks_nach = false;
    bool lenkhilfe_fehler_5de_lenkhilfe_temperaturschutz_sp = false;
    bool lenkhilfe_fehler_5de_lenkhilfe_temperaturschutz = false;
    bool lenkhilfe_fehler_5de_lenkhilfe_betrieb_unpl_sig_sp = false;
    bool lenkhilfe_fehler_5de_lenkhilfe_betrieb_unpl_signal = false;
    bool lenkhilfe_fehler_5de_lenkhilfesensor_k_kommunik_sp = false;
    bool lenkhilfe_fehler_5de_lenkhilfesensor_keine_kommunik = false;
    bool lenkhilfe_fehler_5de_lenkhilfesensor_defekt_sp = false;
    bool lenkhilfe_fehler_5de_lenkhilfesensor_defekt = false;
    bool lenkhilfe_fehler_5de_lenkhilfesensor_unterbrechung_s = false;
    bool lenkhilfe_fehler_5de_lenkhilfesensor_unterbrechung = false;
    bool lenkhilfe_fehler_5de_lenkhilfesensor_ks_nach_masse_s = false;
    bool lenkhilfe_fehler_5de_lenkhifesensor_ks_nach_masse = false;
    bool lenkhilfe_fehler_5de_spannung_kl_15_zu_klein_sp = false;
    bool lenkhilfe_fehler_5de_spannung_kl_15_zu_klein = false;
    float lenkhilfe_fehler_5de_frei_lenkhilfe_1_5 = 0.0f;
    bool lenkhilfe_fehler_5de_spannung_kl_30_zu_klein_sp = false;
    bool lenkhilfe_fehler_5de_spannung_kl_30_zu_klein = false;
    bool lenkhilfe_fehler_5de_spannung_kl_30_zu_gro_sp = false;
    bool lenkhilfe_fehler_5de_spannung_kl_30_zu_gro = false;

    // 0x3D0 Lenkhilfe_1
    float lenkhilfe_1_3d0_lh1_lastinfo = 0.0f;
    bool lenkhilfe_1_3d0_lh1_fehler_li = false;
    bool lenkhilfe_1_3d0_lh1_sicherheitslamp = false;
    bool lenkhilfe_1_3d0_lh1_fehlerlampe = false;
    float lenkhilfe_1_3d0_lh1_textbits = 0.0f;
    bool lenkhilfe_1_3d0_lh1_akustiksign = false;
    bool lenkhilfe_1_3d0_lh1_sleepind = false;
    bool lenkhilfe_1_3d0_lh1_fehlereintr = false;
    bool lenkhilfe_1_3d0_lh1_eps_diagmode = false;
    bool lenkhilfe_1_3d0_lh1_gue_ecu_temp = false;
    bool lenkhilfe_1_3d0_lh1_kuehlung = false;
    bool lenkhilfe_1_3d0_lh1_mode_hybrid = false;
    bool lenkhilfe_1_3d0_lh1_nl_untersp = false;
    bool lenkhilfe_1_3d0_lh1_nl_uebersp = false;
    bool lenkhilfe_1_3d0_lh1_nl_uebertemp = false;
    bool lenkhilfe_1_3d0_lh1_nl_sensor = false;
    bool lenkhilfe_1_3d0_lh1_nl_can_ss = false;
    bool lenkhilfe_1_3d0_lh1_nl_ecu = false;
    bool lenkhilfe_1_3d0_lh1_nl_motor = false;
    bool lenkhilfe_1_3d0_lh1_nl_leistdichte = false;
    float lenkhilfe_1_3d0_lh1_uleistung = 0.0f;
    float lenkhilfe_1_3d0_lh1_ecu_temp = 0.0f;

    // 0x520 Kombi_3
    float kombi_3_520_frei_kombi_3_2 = 0.0f;
    float kombi_3_520_kilometerstand = 0.0f;
    bool kombi_3_520_fehlerstatus_standzeit = false;
    float kombi_3_520_standzeit = 0.0f;
    float kombi_3_520_frei_kombi_3_1 = 0.0f;
    float kombi_3_520_schluesselinfo = 0.0f;
    float kombi_3_520_kombi_multiplex_code_m = 0.0f;
    bool kombi_3_520_kombi_verbauliste_niveauregulie_m3 = false;
    float kombi_3_520_kombi_multiplex_marke_m1 = 0.0f;
    bool kombi_3_520_kombi_verbauliste_lenkhilfe_m3 = false;
    bool kombi_3_520_kombi_verbauliste_dieselpumpe_m3 = false;
    bool kombi_3_520_kombi_verbauliste_lenkwinkel_m3 = false;
    float kombi_3_520_kombi_multiplex_baureihe_m1 = 0.0f;
    float kombi_3_520_kombi_multiplex_laendervariante_m0 = 0.0f;
    bool kombi_3_520_kombi_verbauliste_allrad_m3 = false;
    bool kombi_3_520_kombi_verbauliste_bordnetz_m3 = false;
    bool kombi_3_520_kombi_verbauliste_acc_m3 = false;
    bool kombi_3_520_kombi_verbauliste_airbag_m3 = false;
    float kombi_3_520_kombi_multiplex_generation_m1 = 0.0f;
    bool kombi_3_520_kombi_verbauliste_klima_m3 = false;
    bool kombi_3_520_kombi_verbauliste_abs_m3 = false;
    bool kombi_3_520_kombi_verbauliste_motor_m3 = false;
    float kombi_3_520_kombi_multiplex_derivat_m1 = 0.0f;
    float kombi_3_520_kombi_multiplex_sprachvariante_m0 = 0.0f;
    float kombi_3_520_kombi_multiplex_reifenumfang_m2 = 0.0f;

    // 0x420 Kombi_2
    float kombi_2_420_frei_kombi_2_2 = 0.0f;
    bool kombi_2_420_fehlerstatus_kl_58_s = false;
    float kombi_2_420_klemme_58s_kombi_2 = 0.0f;
    bool kombi_2_420_fehlerstatus_kl_58_d = false;
    float kombi_2_420_klemme_58d_kombi_2 = 0.0f;
    float kombi_2_420_kuehlmitteltemp_4_1_kombi_2 = 0.0f;
    float kombi_2_420_oeltemperatur_4_1 = 0.0f;
    float kombi_2_420_aussentemp_ungefiltert_4_1_ko = 0.0f;
    float kombi_2_420_aussentemperatur_gefiltert = 0.0f;
    bool kombi_2_420_fehlerspeichereintrag_kombi = false;
    float kombi_2_420_frei_kombi_2_1 = 0.0f;
    bool kombi_2_420_anhaenger_erkannt = false;
    bool kombi_2_420_fehlerst_kuehlmitteltemp_4_1 = false;
    bool kombi_2_420_fehlerstatus_oeltemperatur_4_1 = false;
    bool kombi_2_420_fehlerstatus_aussentemp_4_1 = false;

    // 0x320 Kombi_1
    float kombi_1_320_frei_kombi_1_3 = 0.0f;
    float kombi_1_320_angezeigte_geschwindigkeit = 0.0f;
    bool kombi_1_320_blinker_rechts_4_1 = false;
    bool kombi_1_320_blinker_links_4_1 = false;
    bool kombi_1_320_gesetzte_zeitluecke_kombi_1 = false;
    bool kombi_1_320_adr_summer_abgeschaltet = false;
    float kombi_1_320_frei_kombi_1_2 = 0.0f;
    float kombi_1_320_geschwindigkeit_kombi_1 = 0.0f;
    bool kombi_1_320_signalquelle_geschwindigkeit_4 = false;
    bool kombi_1_320_tankwarnung = false;
    float kombi_1_320_tankinhalt = 0.0f;
    bool kombi_1_320_tankstop = false;
    float kombi_1_320_frei_kombi_1_7 = 0.0f;
    bool kombi_1_320_kombi_im_stellgliedtest = false;
    bool kombi_1_320_ladekontroll_lampe_kombi = false;
    float kombi_1_320_bremsinfo = 0.0f;
    bool kombi_1_320_vorgluehlampe_kombi_1 = false;
    bool kombi_1_320_tankwarnlampe = false;
    bool kombi_1_320_heissleuchten_vorwarnung = false;
    bool kombi_1_320_kuehlmittelmangel = false;
    bool kombi_1_320_dynamische_oeldruckwarnung = false;
    bool kombi_1_320_oeldruck = false;
    bool kombi_1_320_fehlerstatus_tank = false;
    bool kombi_1_320_fahrertuer_4_1 = false;

    // 0x5E0 Klima_1
    float klima_1_5e0_aussentemp_ungef_sto_f_4_1 = 0.0f;
    bool klima_1_5e0_fehlerspeichereintrag_klima = false;
    float klima_1_5e0_frei_klima_1_5 = 0.0f;
    bool klima_1_5e0_ac_schalter = false;
    bool klima_1_5e0_temperatureinheit = false;
    float klima_1_5e0_kuehlerluefteransteuerung_klim = 0.0f;
    float klima_1_5e0_geblaeselast_4_1 = 0.0f;
    float klima_1_5e0_kompressorlast = 0.0f;
    float klima_1_5e0_klimadrucksignal_klima_1 = 0.0f;
    float klima_1_5e0_aussentemp_ungef_4_1_klima_1 = 0.0f;
    bool klima_1_5e0_kaeltemitteldruck_veraltet = false;
    bool klima_1_5e0_kompressormoment_veraltet_4_1 = false;
    bool klima_1_5e0_keine_heizleistg_gewuenscht_4_1 = false;
    bool klima_1_5e0_kompressorzustand_4_1 = false;
    bool klima_1_5e0_heizbare_frontscheibe = false;
    bool klima_1_5e0_heizbare_heckscheibe = false;
    bool klima_1_5e0_fahrerwunsch_zuheizer = false;
    bool klima_1_5e0_drehzahlanhebung = false;

    // 0x38A GRA_Neu
    float gra_neu_38a_checksum = 0.0f;
    bool gra_neu_38a_gra_hauptschalt = false;
    bool gra_neu_38a_gra_abbrechen = false;
    bool gra_neu_38a_gra_down_kurz = false;
    bool gra_neu_38a_gra_up_kurz = false;
    bool gra_neu_38a_gra_down_lang = false;
    bool gra_neu_38a_gra_up_lang = false;
    bool gra_neu_38a_gra_fehler_bed = false;
    bool gra_neu_38a_gra_kodierinfo = false;
    bool gra_neu_38a_gra_neu_setzen = false;
    bool gra_neu_38a_gra_recall = false;
    float gra_neu_38a_gra_sender = 0.0f;
    float gra_neu_38a_counter = 0.0f;
    bool gra_neu_38a_gra_tip_down = false;
    bool gra_neu_38a_gra_tip_up = false;
    float gra_neu_38a_gra_zeitluecke = 0.0f;
    bool gra_neu_38a_gra_sta_limiter = false;
    bool gra_neu_38a_gra_typ_hauptschalt = false;
    bool gra_neu_38a_gra_sportschalter = false;
    bool gra_neu_38a_gra_fehler_tip = false;

    // 0x388 GRA
    float gra_388_checksumme_gra_alt = 0.0f;
    bool gra_388_frei_gra_alt = false;
    bool gra_388_gra_alt_adr_bedienteil_fehler = false;
    bool gra_388_gra_alt_adr_beschleunigen = false;
    bool gra_388_gra_alt_adr_verzoegern = false;
    bool gra_388_gra_alt_adr_tipschalter_wie = false;
    bool gra_388_gra_alt_adr_tipschalter_set = false;
    bool gra_388_gra_alt_adr_tipschalter_aus = false;
    bool gra_388_gra_alt_adr_hauptschalter = false;
    float gra_388_zaehler_gra_alt = 0.0f;

    // 0x548 Getriebe_4
    float getriebe_4_548_testparameter_2 = 0.0f;
    float getriebe_4_548_testparameter_1 = 0.0f;
    float getriebe_4_548_waehlhebelausleuchtung = 0.0f;
    bool getriebe_4_548_frei_getriebe_4_1 = false;
    bool getriebe_4_548_testfreigabeflag = false;
    bool getriebe_4_548_handbremserinnerung_s_lampe = false;
    bool getriebe_4_548_shiftlock_getriebe_4 = false;

    // 0x540 Getriebe_2
    float getriebe_2_540_eingelegte_fahrstufe = 0.0f;
    float getriebe_2_540_ganganzeige_kombi_getriebe_va = 0.0f;
    bool getriebe_2_540_fehlerlampe_f_r_kupplung_bei_vl = false;
    bool getriebe_2_540_anforderung_kriechadaption = false;
    bool getriebe_2_540_eco_anzeige_4_1 = false;
    bool getriebe_2_540_shift_lock_lampe = false;
    bool getriebe_2_540_unterdrueckung_von_warnungen = false;
    bool getriebe_2_540_gong = false;
    bool getriebe_2_540_starter_wird_angesteuert = false;
    bool getriebe_2_540_hochschaltlampe = false;
    float getriebe_2_540_synchronisationszeit = 0.0f;
    float getriebe_2_540_invertierte_synchronisations_wu = 0.0f;
    float getriebe_2_540_synchronisations_wunschdrehzahl = 0.0f;
    float getriebe_2_540_gradientenbegrenzung = 0.0f;
    float getriebe_2_540_leerlaufsolldrehzahl_getriebe = 0.0f;
    float getriebe_2_540_zahler_getriebe_2 = 0.0f;
    bool getriebe_2_540_zwischengasflag = false;
    bool getriebe_2_540_ecomatic_4_1 = false;
    bool getriebe_2_540_schubabschaltunterstuetzung = false;
    bool getriebe_2_540_lfr_adaption_freigabeflag = false;

    // 0x440 Getriebe_1
    bool getriebe_1_440_ge1_schaltung = false;
    bool getriebe_1_440_ge1_sta_schutz = false;
    bool getriebe_1_440_ge1_klimakompr = false;
    float getriebe_1_440_ge1_wk = 0.0f;
    float getriebe_1_440_ge1_stst_info = 0.0f;
    bool getriebe_1_440_ge1_egs_anf = false;
    float getriebe_1_440_ge1_zielgang = 0.0f;
    float getriebe_1_440_ge1_wahl_pos = 0.0f;
    float getriebe_1_440_ge1_mrad_mkurb = 0.0f;
    float getriebe_1_440_ge1_soll_mo = 0.0f;
    float getriebe_1_440_ge1_fahrwistd = 0.0f;
    float getriebe_1_440_ge1_notlauf = 0.0f;
    float getriebe_1_440_ge1_kuehlung = 0.0f;
    bool getriebe_1_440_ge1_sta_obd = false;
    bool getriebe_1_440_ge1_launchcontrol = false;
    bool getriebe_1_440_ge1_infobit = false;
    bool getriebe_1_440_ge1_sleepind = false;
    float getriebe_1_440_counter = 0.0f;
    bool getriebe_1_440_ge1_fehlereintr = false;
    float getriebe_1_440_ge1_waverl_mo = 0.0f;

    // 0x390 Gate_Komf_1
    bool gate_komf_1_390_gk1_sta_rdk_warn = false;
    bool gate_komf_1_390_gk1_sta_anhaen = false;
    bool gate_komf_1_390_gk1_sta_licht1 = false;
    bool gate_komf_1_390_gk1_sta_licht3 = false;
    bool gate_komf_1_390_gk1_sta_tuerkont = false;
    bool gate_komf_1_390_gk1_sta_li_vorn = false;
    bool gate_komf_1_390_gk1_sleepackn = false;
    float gate_komf_1_390_gk1_charismamodus_m1 = 0.0f;
    float gate_komf_1_390_gk1_samfktnr_m = 0.0f;
    bool gate_komf_1_390_gk1_fa_tuerkont = false;
    bool gate_komf_1_390_gk1_rueckfahrsch = false;
    bool gate_komf_1_390_gk1_elv_verrieg = false;
    bool gate_komf_1_390_gk1_sta_kessy_2 = false;
    bool gate_komf_1_390_gk1_sta_stdhzg = false;
    bool gate_komf_1_390_gk1_sh_verbau = false;
    bool gate_komf_1_390_gk1_parkfrontwi = false;
    bool gate_komf_1_390_gk1_kw_warm = false;
    bool gate_komf_1_390_bcm_remotestart_betrieb = false;
    bool gate_komf_1_390_bsk_hl_geoeffnet = false;
    bool gate_komf_1_390_bsk_hr_geoeffnet = false;
    bool gate_komf_1_390_gk1_rueckfahr = false;
    bool gate_komf_1_390_gk1_brli_links = false;
    bool gate_komf_1_390_gk1_brli_rechts = false;
    bool gate_komf_1_390_gk1_brli_mitte = false;
    bool gate_komf_1_390_gk1_bls_ilm = false;
    bool gate_komf_1_390_gk1_edc_ilm = false;
    bool gate_komf_1_390_gk1_blinker_li = false;
    bool gate_komf_1_390_gk1_blinker_re = false;
    bool gate_komf_1_390_gk1_def_p_verr = false;
    bool gate_komf_1_390_gk1_ls1_fernlicht = false;
    bool gate_komf_1_390_gk1_sta_licht2 = false;
    bool gate_komf_1_390_gk1_sta_lsm = false;
    float gate_komf_1_390_gk1_count_anhaen = 0.0f;
    bool gate_komf_1_390_bsk_bt_geoeffnet = false;
    bool gate_komf_1_390_bsk_hd_hauptraste = false;
    bool gate_komf_1_390_gk1_bls_aag = false;
    bool gate_komf_1_390_gk1_edc_aag = false;
    bool gate_komf_1_390_gk1_anhaenger = false;
    bool gate_komf_1_390_gk1_brli_anhaen = false;
    bool gate_komf_1_390_gk1_abblendlicht = false;
    bool gate_komf_1_390_gk1_fernlicht = false;
    bool gate_komf_1_390_gk1_wischer_vorn = false;
    bool gate_komf_1_390_gk1_sta_ilm_f_1 = false;
    bool gate_komf_1_390_gk1_abbl_vl_def = false;
    bool gate_komf_1_390_gk1_abbl_vr_def = false;
    bool gate_komf_1_390_gk1_blink_autob = false;
    bool gate_komf_1_390_gk1_warnblk_status = false;
    bool gate_komf_1_390_gk1_sh_laeuft = false;
    bool gate_komf_1_390_sh1_ein_wasserpumpe = false;
    bool gate_komf_1_390_gk1_nebel_ein = false;
    bool gate_komf_1_390_gk1_bremslicht = false;
    bool gate_komf_1_390_gk1_anh_abgesteckt = false;
    bool gate_komf_1_390_gk1_anhkonlamp = false;
    bool gate_komf_1_390_lds_stellung_afl = false;
    bool gate_komf_1_390_gk1_sh_zusatzfkt = false;

    // 0x53C Fahrwerk_1
    bool fahrwerk_1_53c_frei_fahrwerk_1_2 = false;
    float fahrwerk_1_53c_frei_fahrwerk_1_1 = 0.0f;
    float fahrwerk_1_53c_einstellung_fahrwerkdaempfung_4 = 0.0f;
    float fahrwerk_1_53c_ansteuererung_fahrzeugniveau = 0.0f;

    // 0x5C0 EPB_1
    float epb_1_5c0_counter = 0.0f;
    float epb_1_5c0_ep1_fehler_sta = 0.0f;
    bool epb_1_5c0_ep1_sta_epb = false;
    bool epb_1_5c0_ep1_sta_schalter = false;
    float epb_1_5c0_ep1_spannkraft = 0.0f;
    float epb_1_5c0_ep1_schalterinfo = 0.0f;
    bool epb_1_5c0_ep1_sta_nws = false;
    float epb_1_5c0_ep1_neig_winkel = 0.0f;
    float epb_1_5c0_ep1_verzoegerung = 0.0f;
    bool epb_1_5c0_ep1_fehlereintr = false;
    bool epb_1_5c0_ep1_freigabe_ver = false;
    bool epb_1_5c0_ep1_autohold_zul = false;
    bool epb_1_5c0_ep1_autohold_aktiv = false;
    bool epb_1_5c0_ep1_sleepind = false;
    bool epb_1_5c0_ep1_status_kl_15 = false;
    bool epb_1_5c0_ep1_lampe_autop = false;
    bool epb_1_5c0_ep1_bremslicht = false;
    bool epb_1_5c0_ep1_warnton1 = false;
    bool epb_1_5c0_ep1_warnton2 = false;
    bool epb_1_5c0_ep1_anfshlock = false;
    bool epb_1_5c0_epb_autoholdlampe = false;
    bool epb_1_5c0_ep1_qualneigwi = false;
    float epb_1_5c0_ep1_kuppmodber = 0.0f;
    bool epb_1_5c0_ep1_hydrhalten = false;
    bool epb_1_5c0_ep1_fkt_lampe = false;
    bool epb_1_5c0_ep1_warnton = false;
    bool epb_1_5c0_ep1_fehler_bkl = false;
    bool epb_1_5c0_ep1_fehler_gelb = false;
    float epb_1_5c0_ep1_text = 0.0f;
    float epb_1_5c0_checksum = 0.0f;

    // 0x52E Diag_Lenkhilfe
    float diag_lenkhilfe_52e_werkstattcode_diag = 0.0f;
    float diag_lenkhilfe_52e_multiplex_signal_diag = 0.0f;
    bool diag_lenkhilfe_52e_befehl_sensorcodierung_lenkhilf = false;
    float diag_lenkhilfe_52e_befehl_kennliniencodierung_lenk = 0.0f;
    float diag_lenkhilfe_52e_befehl_fehlerspeicher_loeschen = 0.0f;

    // 0x598 Daempfer_1
    float daempfer_1_598_frei_daempfer_1_4 = 0.0f;
    float daempfer_1_598_textbits_daempfer = 0.0f;
    bool daempfer_1_598_fehlerspeicherbit_daempfer_1 = false;
    bool daempfer_1_598_systemstatus_daempfer_1 = false;
    bool daempfer_1_598_frei_daempfer_1_3 = false;
    bool daempfer_1_598_status_cdc_taster = false;
    bool daempfer_1_598_frei_daempfer_1_2 = false;
    float daempfer_1_598_status_daempferregelung_4_1 = 0.0f;

    // 0x570 BSG_Last
    bool bsg_last_570_klimaanlage_abschalten = false;
    bool bsg_last_570_sitzbelueftung_abschalten = false;
    bool bsg_last_570_wischwasserheizung_abschalten = false;
    bool bsg_last_570_lenkradheizung_abschalten = false;
    bool bsg_last_570_heizbare_sitze_abschalten = false;
    bool bsg_last_570_heizbare_aussenspiegel_abschalt = false;
    bool bsg_last_570_heizbare_frontscheibe_abschalte = false;
    bool bsg_last_570_heizbare_heckscheibe_abschalten = false;
    float bsg_last_570_batteriespannung_bordnetzbatter = 0.0f;
    bool bsg_last_570_motorhaubenkontakt = false;
    bool bsg_last_570_leuchtweitenregulierung = false;
    bool bsg_last_570_fehlerspeichereintrag_bsg_last = false;
    float bsg_last_570_zustand_der_starterbatterie = 0.0f;
    float bsg_last_570_zustand_der_bordnetzbatterie = 0.0f;
    bool bsg_last_570_ll_drehzahlanhebung = false;
    bool bsg_last_570_klemme_l = false;
    float bsg_last_570_frei_bsg_last_1_1 = 0.0f;
    bool bsg_last_570_zas_klemme_50 = false;
    bool bsg_last_570_zas_klemme_x = false;
    bool bsg_last_570_zas_klemme_15 = false;
    bool bsg_last_570_zas_klemme_s = false;

    // 0x470 BSG_Kombi
    float bsg_kombi_470_frei_bsg_kombi_1_3 = 0.0f;
    bool bsg_kombi_470_ruecksitzlehne_hr_verr_4_1 = false;
    bool bsg_kombi_470_ruecksitzlehne_hl_verr_4_1 = false;
    bool bsg_kombi_470_fehlerlampe_lenkhilfe_veraltet = false;
    bool bsg_kombi_470_fehlerlampe_lenkhilfe_bsg_komb = false;
    bool bsg_kombi_470_fehlerstatus_kl_58s = false;
    float bsg_kombi_470_klemme_58s_bsg_kombi = 0.0f;
    bool bsg_kombi_470_fehlerstatus_kl_58d = false;
    float bsg_kombi_470_klemme_58d_bsg_kombi = 0.0f;
    bool bsg_kombi_470_unterspannung = false;
    bool bsg_kombi_470_frei_bsg_kombi_1_2 = false;
    bool bsg_kombi_470_heckdeckel_geoeffnet = false;
    bool bsg_kombi_470_motorhaube_geoeffnet = false;
    bool bsg_kombi_470_tuer_hinten_rechts_geoeffnet = false;
    bool bsg_kombi_470_tuer_hinten_links_geoeffnet = false;
    bool bsg_kombi_470_beifahrertuer_geoeffnet = false;
    bool bsg_kombi_470_fahrertuer_geoeffnet = false;
    bool bsg_kombi_470_lade_kontrollampe = false;
    bool bsg_kombi_470_frei_bsg_kombi_1_1 = false;
    bool bsg_kombi_470_rueckfahrlicht = false;
    bool bsg_kombi_470_dwa_akku = false;
    bool bsg_kombi_470_warnblink_mode = false;
    bool bsg_kombi_470_anhaenger_kontrollampe = false;
    bool bsg_kombi_470_blinker_rechts_kontrollampe = false;
    bool bsg_kombi_470_blinker_links_kontrollampe = false;

    // 0x1A8 Bremse_6
    float bremse_6_1a8_checksumme_bremse_6 = 0.0f;
    float bremse_6_1a8_zaehler_bremse_6 = 0.0f;
    bool bremse_6_1a8_status_bremsdruck_bremse_6_du = false;
    bool bremse_6_1a8_frei_bremse_6_1 = false;
    float bremse_6_1a8_bremsdruck_bremse_6 = 0.0f;

    // 0x4A8 Bremse_5
    float bremse_5_4a8_checksum = 0.0f;
    float bremse_5_4a8_counter = 0.0f;
    bool bremse_5_4a8_br5_ecd_lampe = false;
    bool bremse_5_4a8_br5_zt_rueckk_umsetz = false;
    bool bremse_5_4a8_br5_anhi_sta = false;
    bool bremse_5_4a8_esp_rollenmodus_deactiveieren = false;
    bool bremse_5_4a8_br5_sign_druck = false;
    bool bremse_5_4a8_br5_sta_druck = false;
    bool bremse_5_4a8_br5_druckvalid = false;
    bool bremse_5_4a8_br5_stillstand = false;
    float bremse_5_4a8_br5_bremsdruck = 0.0f;
    bool bremse_5_4a8_br5_vorzeichen = false;
    bool bremse_5_4a8_br5_sta_gierrate = false;
    float bremse_5_4a8_br5_giergeschw = 0.0f;
    bool bremse_5_4a8_br5_anb_cm_rueckk_umsetz = false;
    bool bremse_5_4a8_br5_hdc_bereit = false;
    bool bremse_5_4a8_esp_stat_fallback_ebkv = false;
    float bremse_5_4a8_esp_anforderung_epb = 0.0f;
    bool bremse_5_4a8_esp_autohold_active = false;
    bool bremse_5_4a8_esp_autohold_standby = false;
    bool bremse_5_4a8_br5_anhi_akt = false;
    bool bremse_5_4a8_br5_v_ueberw = false;
    bool bremse_5_4a8_br5_bremslicht = false;
    bool bremse_5_4a8_br5_notbremsung = false;
    bool bremse_5_4a8_br5_fahrer_tritt_zbr_schw = false;
    bool bremse_5_4a8_br5_awv2_bremsruck = false;
    bool bremse_5_4a8_br5_awv2_fehler = false;

    // 0x2A0 Bremse_4
    float bremse_4_2a0_frei_bremse_4_1 = 0.0f;
    bool bremse_4_2a0_einheit_kupplungssteifigkeit = false;
    float bremse_4_2a0_abs_vorgabewert_hinten_kupplung = 0.0f;
    float bremse_4_2a0_abs_vorgabewert_mitte_kupplungs = 0.0f;

    // 0x4A0 Bremse_3
    float bremse_3_4a0_radgeschw_hr_4_1 = 0.0f;
    bool bremse_3_4a0_frei_bremse_3_4 = false;
    float bremse_3_4a0_radgeschw_hl_4_1 = 0.0f;
    bool bremse_3_4a0_frei_bremse_3_3 = false;
    float bremse_3_4a0_radgeschw_vr_4_1 = 0.0f;
    bool bremse_3_4a0_frei_bremse_3_2 = false;
    float bremse_3_4a0_radgeschw_vl_4_1 = 0.0f;
    bool bremse_3_4a0_frei_bremse_3_1 = false;

    // 0x5A0 Bremse_2
    bool bremse_2_5a0_gemessene_querbeschleunigung = false;
    bool bremse_2_5a0_frei_bremse_2_2 = false;
    float bremse_2_5a0_impulszahl = 0.0f;
    bool bremse_2_5a0_fehlerstatus_wegimpulse_4_1 = false;
    bool bremse_2_5a0_frei_bremse_2_5 = false;
    bool bremse_2_5a0_warnlampe_dds = false;
    bool bremse_2_5a0_fehlerspeichereintrag_bremse = false;
    bool bremse_2_5a0_wegimpulszaehlerstatus = false;
    float bremse_2_5a0_wegimpulse_vorderachse = 0.0f;
    float bremse_2_5a0_zeitstempel = 0.0f;
    float bremse_2_5a0_mittlere_raddrehzahl_bremse_2 = 0.0f;
    bool bremse_2_5a0_querbeschl_timertic_m = false;
    float bremse_2_5a0_timer_m1 = 0.0f;
    float bremse_2_5a0_querbeschleunigung_m0 = 0.0f;

    // 0x1A0 Bremse_1
    bool bremse_1_1a0_br1_asr_anf = false;
    bool bremse_1_1a0_br1_msr_anf_m = false;
    bool bremse_1_1a0_br1_abs_brems = false;
    bool bremse_1_1a0_br1_eds_ongr = false;
    bool bremse_1_1a0_br1_esp_ongr = false;
    float bremse_1_1a0_br1_asr_ongr = 0.0f;
    bool bremse_1_1a0_br1_ebv_ongr = false;
    bool bremse_1_1a0_br1_lampe_abs = false;
    bool bremse_1_1a0_br1_lampe_asr = false;
    bool bremse_1_1a0_br1_lampe_bk = false;
    bool bremse_1_1a0_br1_lichtschalt = false;
    bool bremse_1_1a0_br1_stadruckschw = false;
    bool bremse_1_1a0_br1_mad = false;
    bool bremse_1_1a0_br1_sta_mad = false;
    bool bremse_1_1a0_br1_diagnose = false;
    bool bremse_1_1a0_br1_bkv_active = false;
    float bremse_1_1a0_br1_rad_kmh = 0.0f;
    float bremse_1_1a0_br1_asrmo_sl = 0.0f;
    float bremse_1_1a0_br1_asrmo_fa_m0 = 0.0f;
    float bremse_1_1a0_br1_msr_mo_inv_m1 = 0.0f;
    float bremse_1_1a0_br1_msr_mo = 0.0f;
    float bremse_1_1a0_br1_zaehler = 0.0f;
    bool bremse_1_1a0_br1_asr_esp = false;
    bool bremse_1_1a0_br1_espasr_passive = false;
    bool bremse_1_1a0_br1_sta_esp = false;
    bool bremse_1_1a0_br1_ersatz_kmh = false;

    // 0x2A8 Bremsbooster_1
    bool bremsbooster_1_2a8_fehlerspeichereintrag_booster = false;
    bool bremsbooster_1_2a8_loseschalter_unplausibel_boost = false;
    bool bremsbooster_1_2a8_position_standby = false;
    bool bremsbooster_1_2a8_adr_relais_ge_ffnet = false;
    bool bremsbooster_1_2a8_status_bremsbooster_steuerung = false;
    bool bremsbooster_1_2a8_bremsbooster_verf_gbar = false;
    bool bremsbooster_1_2a8_eingriff_bremsbooster = false;
    bool bremsbooster_1_2a8_bremseingriff_fahrer = false;
    float bremsbooster_1_2a8_frei_bremsbooster_1_1 = 0.0f;
    float bremsbooster_1_2a8_zaehler_booster_1 = 0.0f;
    float bremsbooster_1_2a8_checksumme_booster_1 = 0.0f;

    // 0x578 BatMan_1
    bool batman_1_578_fehlerspeichereintrag_batman = false;
    bool batman_1_578_leistungsrelais = false;
    bool batman_1_578_messung_starterleitung = false;
    bool batman_1_578_zustand_starterleitung = false;
    bool batman_1_578_umschaltrelais_bordnetzbatterie = false;
    float batman_1_578_ladung_starterbatterie = 0.0f;
    bool batman_1_578_startmodus = false;

    // 0x2C0 Allrad_1
    float allrad_1_2c0_kupplungssteifigkeit_hinten_is = 0.0f;
    bool allrad_1_2c0_fehlerspeichereintrag_allrad_1 = false;
    float allrad_1_2c0_frei_allrad_1_1 = 0.0f;
    bool allrad_1_2c0_schaltung_vorwarnung = false;
    bool allrad_1_2c0_schaltung_aktiv_allrad_1 = false;
    float allrad_1_2c0_ganginfo_png = 0.0f;
    bool allrad_1_2c0_png_anzeige_blinkend = false;
    float allrad_1_2c0_png_status_4_1 = 0.0f;
    float allrad_1_2c0_kupplungssteifigkeit_mitte_ist = 0.0f;
    bool allrad_1_2c0_einheit_der_kupplungssteifigkei = false;
    bool allrad_1_2c0_geschwindigkeitsbegrenzung = false;
    bool allrad_1_2c0_allrad_warnlampe = false;
    bool allrad_1_2c0_notlauf = false;
    bool allrad_1_2c0_kupplung_komplett_offen = false;
    bool allrad_1_2c0_fehlerstatus_kupplungssteifigke = false;
    bool allrad_1_2c0_ubertemperaturschutz_allrad_1 = false;
    bool allrad_1_2c0_fehler_allrad_kupplung = false;

    // 0x550 Airbag_2
    float airbag_2_550_oop_beifahrer = 0.0f;
    float airbag_2_550_oop_fahrer = 0.0f;
    bool airbag_2_550_belegungserkennung_hinten_mitte = false;
    bool airbag_2_550_belegungserkennung_hinten_recht = false;
    bool airbag_2_550_belegungserkennung_hinten_links = false;
    bool airbag_2_550_belegungserkennung_beifahrersit = false;
    float airbag_2_550_checksumme_airbag_2_reserviert = 0.0f;

    // 0x050 Airbag_1
    float airbag_1_050_checksum = 0.0f;
    float airbag_1_050_counter = 0.0f;
    bool airbag_1_050_fehlerspeichereintrag = false;
    bool airbag_1_050_frei_airbag_1_2 = false;
    bool airbag_1_050_airbag_im_stellgliedtest = false;
    bool airbag_1_050_airbag_in_diagnose = false;
    bool airbag_1_050_gurtwarnung_beifahrer = false;
    bool airbag_1_050_gurtschalter_beifahrer = false;
    bool airbag_1_050_gurtwarnung_fahrer = false;
    bool airbag_1_050_gurtschalter_fahrer = false;
    bool airbag_1_050_airbag_systemfehler = false;
    bool airbag_1_050_kindersitzerkennung = false;
    bool airbag_1_050_airbag_deaktiviert = false;
    bool airbag_1_050_airbag_lampe = false;
    float airbag_1_050_crash_intensitaet = 0.0f;
    bool airbag_1_050_rollover = false;
    bool airbag_1_050_seiten_crash_beifahrer = false;
    bool airbag_1_050_seiten_crash_fahrer = false;
    bool airbag_1_050_heck_crash = false;
    bool airbag_1_050_front_crash = false;

    // 0x360 ADR_System
    float adr_system_360_s_checksumme_adr_1 = 0.0f;
    float adr_system_360_s_frei_adr_1_1 = 0.0f;
    float adr_system_360_s_zeitluecke_gemessen = 0.0f;
    bool adr_system_360_s_fehlerspeichereintrag_adr = false;
    bool adr_system_360_s_fehlerspeichereintrag_bremsbo = false;
    bool adr_system_360_s_adr_relais_geoeffnet = false;
    float adr_system_360_s_bremsbooster_status = 0.0f;
    bool adr_system_360_s_eingriff_bremsbooster = false;
    bool adr_system_360_s_loeseschalter_unplausibel = false;
    bool adr_system_360_s_bremseingriff_fahrer = false;
    bool adr_system_360_s_anzeige_sensor_blind = false;
    bool adr_system_360_s_ansteuerung_optischer_fahrerh = false;
    bool adr_system_360_s_ansteuerung_gong_2 = false;
    bool adr_system_360_s_ansteuerung_gong_1 = false;
    float adr_system_360_s_schaltaufforderung = 0.0f;
    bool adr_system_360_s_anzeige_prioritaet = false;
    bool adr_system_360_s_anzeige_zeitluecke = false;
    float adr_system_360_s_wunschgeschwindigkeit = 0.0f;
    float adr_system_360_s_objekt_erfasst = 0.0f;
    float adr_system_360_s_gesetzte_zeitluecke_adr_1 = 0.0f;
    bool adr_system_360_s_synchronisation_bremsbooster = false;
    bool adr_system_360_s_momentenanforderung_freigabe = false;
    bool adr_system_360_s_verhinderung_schubabschaltung = false;
    float adr_system_360_s_status_adr_1_s = 0.0f;
    bool adr_system_360_s_fehler_adr_1_s = false;
    float adr_system_360_s_zaehler_adr_1 = 0.0f;
    float adr_system_360_s_momentenanforderung_adr = 0.0f;

    // 0x260 ADR_2
    float adr_2_260_frei_adr_2_2 = 0.0f;
    float adr_2_260_anforderung_bremsdruck = 0.0f;
    float adr_2_260_frei_adr_2_1 = 0.0f;
    bool adr_2_260_relais_test_fehler = false;
    bool adr_2_260_standby = false;
    float adr_2_260_zaehler_adr_2 = 0.0f;
    float adr_2_260_checksumme_adr_2 = 0.0f;

    // 0x52C ADR_1
    float adr_1_52c_checksumme_adr_1 = 0.0f;
    float adr_1_52c_frei_adr_1_5 = 0.0f;
    float adr_1_52c_zeitluecke_gemessen = 0.0f;
    bool adr_1_52c_fehlerspeichereintrag_adr = false;
    float adr_1_52c_reserviert_adr_1_1 = 0.0f;
    bool adr_1_52c_anzeige_sensor_blind = false;
    bool adr_1_52c_ansteuerung_optischer_fahrerhin = false;
    bool adr_1_52c_ansteuerung_gong_2 = false;
    bool adr_1_52c_ansteuerung_gong_1 = false;
    float adr_1_52c_schaltaufforderung = 0.0f;
    bool adr_1_52c_anzeige_prioritaet = false;
    bool adr_1_52c_anzeige_zeitluecke = false;
    float adr_1_52c_wunschgeschwindigkeit = 0.0f;
    float adr_1_52c_objekt_erfasst = 0.0f;
    float adr_1_52c_gesetzte_zeitluecke_adr_1 = 0.0f;
    bool adr_1_52c_synchronisation_bremsbooster = false;
    bool adr_1_52c_momentenanforderung_freigabe = false;
    bool adr_1_52c_verhinderung_schubabschaltung = false;
    float adr_1_52c_status_adr_1 = 0.0f;
    bool adr_1_52c_fehler_adr_1 = false;
    float adr_1_52c_zaehler_adr_1 = 0.0f;
    float adr_1_52c_momentenanforderung_adr = 0.0f;

    // 0x60E Einheiten_1
    bool einheiten_1_60e_mfa_v_einheit_02 = false;

    // 0x368 ACC_System
    float acc_system_368_checksum = 0.0f;
    float acc_system_368_counter = 0.0f;
    float acc_system_368_acs_sta_adr = 0.0f;
    bool acc_system_368_acs_adr_schub = false;
    bool acc_system_368_acs_schubabsch = false;
    float acc_system_368_acs_stst_info = 0.0f;
    bool acc_system_368_acs_momeingriff = false;
    float acc_system_368_acs_typ_acc = 0.0f;
    bool acc_system_368_acs_freigsollb = false;
    float acc_system_368_acs_sollbeschl = 0.0f;
    bool acc_system_368_acs_anhaltewunsch = false;
    bool acc_system_368_acs_fehler = false;
    float acc_system_368_acs_zul_regelabw = 0.0f;
    float acc_system_368_acs_max_aendgrad = 0.0f;

    // 0x56A ACC_GRA_Anzeige
    float acc_gra_anzeige_56a_checksum = 0.0f;
    float acc_gra_anzeige_56a_aca_staacc = 0.0f;
    float acc_gra_anzeige_56a_aca_id_staacc = 0.0f;
    bool acc_gra_anzeige_56a_aca_fahrerhinw = false;
    bool acc_gra_anzeige_56a_aca_anzdisplay = false;
    float acc_gra_anzeige_56a_aca_zeitluecke = 0.0f;
    float acc_gra_anzeige_56a_aca_v_wunsch = 0.0f;
    bool acc_gra_anzeige_56a_aca_kmh_mph = false;
    bool acc_gra_anzeige_56a_aca_akustik1 = false;
    bool acc_gra_anzeige_56a_aca_akustik2 = false;
    float acc_gra_anzeige_56a_aca_priodisp = 0.0f;
    float acc_gra_anzeige_56a_aca_gemzeitl = 0.0f;
    bool acc_gra_anzeige_56a_aca_acc_verz = false;
    float acc_gra_anzeige_56a_aca_stagra = 0.0f;
    float acc_gra_anzeige_56a_aca_id_stagra = 0.0f;
    bool acc_gra_anzeige_56a_aca_codierung = false;
    bool acc_gra_anzeige_56a_aca_tachokranz = false;
    bool acc_gra_anzeige_56a_aca_aend_zeitluecke = false;
    float acc_gra_anzeige_56a_counter = 0.0f;

    // 0x0D0 Lenkhilfe_3
    float lenkhilfe_3_0d0_checksum = 0.0f;
    float lenkhilfe_3_0d0_lh3_bs_spiegel = 0.0f;
    float lenkhilfe_3_0d0_counter = 0.0f;
    float lenkhilfe_3_0d0_lh3_lm = 0.0f;
    bool lenkhilfe_3_0d0_lh3_lmsign = false;
    bool lenkhilfe_3_0d0_lh3_lmvalid = false;
    float lenkhilfe_3_0d0_lh3_sta_dsr = 0.0f;
    float lenkhilfe_3_0d0_lh3_blw = 0.0f;
    bool lenkhilfe_3_0d0_lh3_blwsign = false;
    bool lenkhilfe_3_0d0_lh3_blwvalid = false;
    float lenkhilfe_3_0d0_lh3_lenkungstyp = 0.0f;

    // 0x3D2 Lenkhilfe_2
    float lenkhilfe_2_3d2_checksum = 0.0f;
    float lenkhilfe_2_3d2_counter = 0.0f;
    bool lenkhilfe_2_3d2_lh2_geradeaus = false;
    float lenkhilfe_2_3d2_lh2_sta_charisma = 0.0f;
    float lenkhilfe_2_3d2_lh2_sta_hca = 0.0f;
    bool lenkhilfe_2_3d2_lh2_ausg_lw1 = false;
    bool lenkhilfe_2_3d2_lh2_ausg_lw1_gue = false;
    float lenkhilfe_2_3d2_lh2_stateps_pla = 0.0f;
    float lenkhilfe_2_3d2_lh2_aktlenkeingriff = 0.0f;
    float lenkhilfe_2_3d2_lh2_pla_err = 0.0f;
    float lenkhilfe_2_3d2_lh2_pla_abbr = 0.0f;

    // 0x3D4 PLA_1
    float pla_1_3d4_checksum = 0.0f;
    float pla_1_3d4_counter = 0.0f;
    float pla_1_3d4_pl1_status_eps = 0.0f;
    float pla_1_3d4_pl1_arcanglereq = 0.0f;
    bool pla_1_3d4_pl1_anglereqsign = false;
    float pla_1_3d4_pl1_stat_pla_esp = 0.0f;
    float pla_1_3d4_pl1_bremsmoment = 0.0f;
    float pla_1_3d4_pl1_void = 0.0f;

    // 0x0D2 HCA_1
    float hca_1_0d2_checksum = 0.0f;
    float hca_1_0d2_counter = 0.0f;
    float hca_1_0d2_hca_status = 0.0f;
    float hca_1_0d2_lm_offset = 0.0f;
    bool hca_1_0d2_lm_offsign = false;
    float hca_1_0d2_vib_freq = 0.0f;
    float hca_1_0d2_vib_amp = 0.0f;

    // 0x284 Motor_Bremse
    bool motor_bremse_284_mob_standby = false;
    bool motor_bremse_284_mob_freigabe = false;
    bool motor_bremse_284_mob_anhaltewunsch = false;
    float motor_bremse_284_mob_checksum = 0.0f;
    float motor_bremse_284_mob_counter = 0.0f;
    bool motor_bremse_284_tsk_v_begrenzung_aktiv = false;
    float motor_bremse_284_tsk_ax_getriebe_01 = 0.0f;
    float motor_bremse_284_mob_bremsstgr = 0.0f;
    float motor_bremse_284_mob_bremsmom = 0.0f;

    // 0x366 AWV
    float awv_366_checksum = 0.0f;
    float awv_366_counter = 0.0f;
    float awv_366_awv_text = 0.0f;
    bool awv_366_awv_1_freigabe = false;
    bool awv_366_awv_1_prefill = false;
    float awv_366_awv_1_parameter = 0.0f;
    bool awv_366_awv_only = false;
    bool awv_366_awv_cityanb_auspraegung = false;
    bool awv_366_awv_halten = false;
    bool awv_366_anb_teilbremsung_freigabe = false;
    bool awv_366_awv_2_status = false;
    bool awv_366_awv_2_fehler = false;
    float awv_366_awv_2_su_warnzeit = 0.0f;
    bool awv_366_awv_2_su_bremsruck = false;
    bool awv_366_awv_2_su_gong = false;
    bool awv_366_awv_2_su_lampe = false;
    bool awv_366_awv_2_umfeldwarn = false;
    bool awv_366_awv_2_freigabe = false;
    float awv_366_awv_2_ruckprofil = 0.0f;
    bool awv_366_awv_2_warnton = false;
    bool awv_366_awv_2_warnsymbol = false;
    bool awv_366_awv_infoton = false;
    bool awv_366_awv_2_gurtstraffer = false;
    bool awv_366_awv_konfiguration_menueanf = false;
    bool awv_366_awv_konfiguration_vorw_menueanf = false;
    bool awv_366_awv_konfiguration_status = false;
    bool awv_366_awv_konfiguration_vorw_status = false;
    bool awv_366_awv_2_abstandswarnung = false;
    bool awv_366_anb_zielbremsung_freigabe = false;
    bool awv_366_anb_cm_anforderung = false;
    float awv_366_anb_ziel_teilbrems_verz_anf = 0.0f;

    // 0x5BE LDW_Status
    float ldw_status_5be_ldw_lernmodus_rechts = 0.0f;
    float ldw_status_5be_ldw_lernmodus_links = 0.0f;
    float ldw_status_5be_ldw_lernmodus = 0.0f;
    float ldw_status_5be_ldw_textbits = 0.0f;
    float ldw_status_5be_ldw_gong = 0.0f;
    bool ldw_status_5be_ldw_kameratyp = false;
    bool ldw_status_5be_ldw_lampe_gelb = false;
    bool ldw_status_5be_ldw_lampe_gruen = false;
    bool ldw_status_5be_ldw_sw_warnung_links = false;
    bool ldw_status_5be_ldw_sw_warnung_rechts = false;
    bool ldw_status_5be_ldw_kd_fehler = false;
    float ldw_status_5be_ldw_dlc = 0.0f;
    float ldw_status_5be_ldw_tlc = 0.0f;
    bool ldw_status_5be_ldw_seite_dlctlc = false;
    float ldw_status_5be_ldw_frueh_spaet = 0.0f;

    // 0x1AC Bremse_8
    float bremse_8_1ac_br8_checksumme = 0.0f;
    float bremse_8_1ac_br8_zaehler = 0.0f;
    bool bremse_8_1ac_br8_sta_acc_anf = false;
    bool bremse_8_1ac_br8_verz_epb_akt = false;
    bool bremse_8_1ac_br8_sta_br_temp = false;
    bool bremse_8_1ac_br8_sta_br_druck = false;
    float bremse_8_1ac_br8_tolabgl_hl = 0.0f;
    float bremse_8_1ac_br8_tolabgl_hr = 0.0f;
    float bremse_8_1ac_br8_istbeschl = 0.0f;
    bool bremse_8_1ac_br8_sta_hw_bls = false;
    bool bremse_8_1ac_br8_qb_lbeschl = false;
    float bremse_8_1ac_br8_esc_mode = 0.0f;
    bool bremse_8_1ac_br8_aktbrsyst = false;
    bool bremse_8_1ac_br8_fa_bremst = false;
    bool bremse_8_1ac_br8_stabrsyst = false;
    float bremse_8_1ac_br8_laengsbeschl = 0.0f;
    bool bremse_8_1ac_br8_sta_adr_br = false;
    bool bremse_8_1ac_br8_quattro = false;
    bool bremse_8_1ac_br8_sta_verzreg = false;
    bool bremse_8_1ac_br8_sta_bls = false;
    bool bremse_8_1ac_br8_verz_epb = false;
    bool bremse_8_1ac_br8_check_epb = false;

    // 0x3A0 Bremse_10
    float bremse_10_3a0_b10_checksumme = 0.0f;
    float bremse_10_3a0_b10_zaehler = 0.0f;
    bool bremse_10_3a0_b10_qb_wegimp_vl = false;
    bool bremse_10_3a0_b10_qb_wegimp_vr = false;
    bool bremse_10_3a0_b10_qb_wegimp_hl = false;
    bool bremse_10_3a0_b10_qb_wegimp_hr = false;
    float bremse_10_3a0_b10_wegimp_vl = 0.0f;
    float bremse_10_3a0_b10_wegimp_vr = 0.0f;
    float bremse_10_3a0_b10_wegimp_hl = 0.0f;
    float bremse_10_3a0_b10_wegimp_hr = 0.0f;
    bool bremse_10_3a0_b10_qb_fahrtr_vl = false;
    bool bremse_10_3a0_b10_qb_fahrtr_vr = false;
    bool bremse_10_3a0_b10_qb_fahrtr_hl = false;
    bool bremse_10_3a0_b10_qb_fahrtr_hr = false;
    bool bremse_10_3a0_b10_fahrtr_vl = false;
    bool bremse_10_3a0_b10_fahrtr_vr = false;
    bool bremse_10_3a0_b10_fahrtr_hl = false;
    bool bremse_10_3a0_b10_fahrtr_hr = false;

    // 0x343 RDK_Status
    bool rdk_status_343_rks_reifen_vl = false;
    bool rdk_status_343_rks_reifen_vr = false;
    bool rdk_status_343_rks_reifen_hl = false;
    bool rdk_status_343_rks_reifen_hr = false;
    bool rdk_status_343_rks_reifen_rr = false;
    bool rdk_status_343_rks_warnung_2 = false;
    bool rdk_status_343_rks_warnung_1 = false;
    bool rdk_status_343_rks_systemfehler = false;
    bool rdk_status_343_rks_kalibrier_abgew = false;
    bool rdk_status_343_rks_druckdiff_vorn = false;
    bool rdk_status_343_rks_druckdiff_hinten = false;
    bool rdk_status_343_rks_befuellung_rr_low = false;
    bool rdk_status_343_rks_funkstoerung = false;
    bool rdk_status_343_rks_system_aus = false;
    bool rdk_status_343_rks_kd_fehler = false;
    bool rdk_status_343_rks_lampe = false;
    bool rdk_status_343_rks_ton = false;
    bool rdk_status_343_rks_gong = false;
    bool rdk_status_343_rks_rdk_blinkbit = false;
    bool rdk_status_343_rks_teillast = false;

    // 0x392 Gate_Komf_2
    bool gate_komf_2_392_gk2_sta_lsm = false;
    bool gate_komf_2_392_gk2_sta_lichtsensor = false;
    bool gate_komf_2_392_gk2_sta_licht1 = false;
    bool gate_komf_2_392_gk2_sta_vsg = false;
    bool gate_komf_2_392_gk2_sta_schluessel = false;
    bool gate_komf_2_392_gk2_sta_profil = false;
    bool gate_komf_2_392_gk2_sta_clima2 = false;
    bool gate_komf_2_392_gk2_sta_bsg4 = false;
    bool gate_komf_2_392_gk2_sta_kessy_4 = false;
    bool gate_komf_2_392_bs4_gleitende_leuchtw_anf = false;
    bool gate_komf_2_392_bs4_glw_fernlicht_anf = false;
    bool gate_komf_2_392_gk2_blk_l_kontrolle = false;
    bool gate_komf_2_392_gk2_blk_r_kontrolle = false;
    bool gate_komf_2_392_gk2_ls_komfehler = false;
    bool gate_komf_2_392_gk2_ls_def = false;
    float gate_komf_2_392_gk2_helligkeit = 0.0f;
    bool gate_komf_2_392_gk2_vd_zu_ver = false;
    bool gate_komf_2_392_gk2_vd_entriegelt = false;
    bool gate_komf_2_392_gk2_vd_offen_ver = false;
    bool gate_komf_2_392_gk2_verdeck_anf = false;
    bool gate_komf_2_392_gk2_vdkd_auf = false;
    float gate_komf_2_392_gk2_schluessel = 0.0f;
    bool gate_komf_2_392_gk2_hardtop = false;
    bool gate_komf_2_392_gk2_afl_schalter = false;
    bool gate_komf_2_392_gk2_nebelschluss = false;
    bool gate_komf_2_392_gk2_em_lin_ungueltig = false;
    float gate_komf_2_392_gk2_profil = 0.0f;
    float gate_komf_2_392_gk2_kl_stst_info = 0.0f;
    float gate_komf_2_392_gk2_bsg_stst_info = 0.0f;
    float gate_komf_2_392_gk2_bem_p_generator = 0.0f;
    float gate_komf_2_392_gk2_bem_abschaltstufen = 0.0f;
    float gate_komf_2_392_gk2_bem_dfm = 0.0f;
    float gate_komf_2_392_gk2_kessy_stst_info = 0.0f;
    float gate_komf_2_392_gk2_bem_stst_info = 0.0f;

    // 0x3BA SWA_1
    float swa_1_3ba_swa_textbits = 0.0f;
    float swa_1_3ba_swa_gong = 0.0f;
    bool swa_1_3ba_swa_sta_passiv = false;
    bool swa_1_3ba_swa_sta_aktiv = false;
    bool swa_1_3ba_swa_infostufe_swa_li = false;
    bool swa_1_3ba_swa_warnung_swa_li = false;
    bool swa_1_3ba_swa_infostufe_swa_re = false;
    bool swa_1_3ba_swa_warnung_swa_re = false;
    bool swa_1_3ba_swa_kd_fehler = false;

    // 0x497 Parkhilfe_01
    float parkhilfe_01_497_ph_abschaltursache = 0.0f;
    bool parkhilfe_01_497_ph_opt_anzeige_v_ein = false;
    bool parkhilfe_01_497_ph_opt_anzeige_h_ein = false;
    bool parkhilfe_01_497_ph_opt_anz_v_hindernis = false;
    bool parkhilfe_01_497_ph_opt_anz_h_hindernis = false;
    bool parkhilfe_01_497_ph_tongeber_v_aktiv = false;
    bool parkhilfe_01_497_ph_tongeber_h_aktiv = false;
    bool parkhilfe_01_497_ph_tongeber_mute = false;
    bool parkhilfe_01_497_ph_anf_audioabsenkung = false;
    float parkhilfe_01_497_ph_frequenz_hinten = 0.0f;
    float parkhilfe_01_497_ph_lautstaerke_hinten = 0.0f;
    float parkhilfe_01_497_ph_frequenz_vorn = 0.0f;
    float parkhilfe_01_497_ph_lautstaerke_vorn = 0.0f;
    bool parkhilfe_01_497_ph_trigger_bildaufschaltung = false;
    float parkhilfe_01_497_ph_startstopp_info = 0.0f;
    bool parkhilfe_01_497_ph_aufbauten_erk = false;
    float parkhilfe_01_497_ph_bererk_vorn = 0.0f;
    float parkhilfe_01_497_ph_bererk_hinten = 0.0f;
    bool parkhilfe_01_497_ph_defekt = false;
    bool parkhilfe_01_497_ph_gestoert = false;
    float parkhilfe_01_497_ph_systemzustand = 0.0f;
    float parkhilfe_01_497_ph_display_kundenwunsch = 0.0f;
    bool parkhilfe_01_497_ph_kd_fehler = false;

    // 0x5B7 Bremse_11
    float bremse_11_5b7_checksum = 0.0f;
    float bremse_11_5b7_counter = 0.0f;
    bool bremse_11_5b7_b11_hydhalten = false;
    float bremse_11_5b7_b11_br_stst_info = 0.0f;
    float bremse_11_5b7_b11_obd_nib_vl = 0.0f;
    float bremse_11_5b7_b11_obd_nib_vr = 0.0f;
    float bremse_11_5b7_b11_obd_nib_hl = 0.0f;
    float bremse_11_5b7_b11_obd_nib_hr = 0.0f;
    bool bremse_11_5b7_b11_epb_steller_akt = false;
    bool bremse_11_5b7_b11_epb_steller_gue = false;

    // 0x5DC Soll_Verbauliste_neu
    bool soll_verbauliste_neu_5dc_vl1_motor_sg = false;
    bool soll_verbauliste_neu_5dc_vl1_getr_sg = false;
    bool soll_verbauliste_neu_5dc_vl1_abs = false;
    bool soll_verbauliste_neu_5dc_vl1_kombi = false;
    bool soll_verbauliste_neu_5dc_vl1_lsm = false;
    bool soll_verbauliste_neu_5dc_vl1_airbag = false;
    bool soll_verbauliste_neu_5dc_vl1_lenkhilfe = false;
    bool soll_verbauliste_neu_5dc_vl1_dyn_lwr = false;
    bool soll_verbauliste_neu_5dc_vl1_res_08 = false;
    bool soll_verbauliste_neu_5dc_vl1_allrad = false;
    bool soll_verbauliste_neu_5dc_vl1_adr = false;
    bool soll_verbauliste_neu_5dc_vl1_adr_getrennt = false;
    bool soll_verbauliste_neu_5dc_vl1_epb = false;
    bool soll_verbauliste_neu_5dc_vl1_res_13 = false;
    bool soll_verbauliste_neu_5dc_vl1_daempfer = false;
    bool soll_verbauliste_neu_5dc_vl1_quersperre = false;
    bool soll_verbauliste_neu_5dc_vl1_motorslave = false;
    bool soll_verbauliste_neu_5dc_vl1_swa = false;
    bool soll_verbauliste_neu_5dc_vl1_hca = false;
    bool soll_verbauliste_neu_5dc_vl1_rka_plus = false;
    bool soll_verbauliste_neu_5dc_vl1_pla = false;
    bool soll_verbauliste_neu_5dc_vl1_wfs_kbi = false;
    bool soll_verbauliste_neu_5dc_vl1_kombi_kbi = false;
    bool soll_verbauliste_neu_5dc_vl1_soll_eq_ist = false;
    bool soll_verbauliste_neu_5dc_vl1_bsg_komf = false;
    bool soll_verbauliste_neu_5dc_vl1_zke = false;
    bool soll_verbauliste_neu_5dc_vl1_tsg_ft = false;
    bool soll_verbauliste_neu_5dc_vl1_tsg_bt = false;
    bool soll_verbauliste_neu_5dc_vl1_tsg_hl = false;
    bool soll_verbauliste_neu_5dc_vl1_tsg_hr = false;
    bool soll_verbauliste_neu_5dc_vl1_memory = false;
    bool soll_verbauliste_neu_5dc_vl1_dachmodul_k = false;
    bool soll_verbauliste_neu_5dc_vl1_zentralelektrik_ii = false;
    bool soll_verbauliste_neu_5dc_vl1_rdk = false;
    bool soll_verbauliste_neu_5dc_vl1_lenksaeule = false;
    bool soll_verbauliste_neu_5dc_vl1_gateway = false;
    bool soll_verbauliste_neu_5dc_vl1_clima_komf = false;
    bool soll_verbauliste_neu_5dc_vl1_einparkhilfe = false;
    bool soll_verbauliste_neu_5dc_vl1_ptc_heizung = false;
    bool soll_verbauliste_neu_5dc_vl1_standheiz = false;
    bool soll_verbauliste_neu_5dc_vl1_verdeck = false;
    bool soll_verbauliste_neu_5dc_vl1_rse_i = false;
    bool soll_verbauliste_neu_5dc_vl1_res_42 = false;
    bool soll_verbauliste_neu_5dc_vl1_mdi_i = false;
    bool soll_verbauliste_neu_5dc_vl1_anhaenger = false;
    bool soll_verbauliste_neu_5dc_vl1_memory_bf = false;
    bool soll_verbauliste_neu_5dc_vl1_easy_entry_vf = false;
    bool soll_verbauliste_neu_5dc_vl1_easy_entry_vb = false;
    bool soll_verbauliste_neu_5dc_vl1_heckdeckel = false;
    bool soll_verbauliste_neu_5dc_vl1_rearview = false;
    bool soll_verbauliste_neu_5dc_vl1_sonderfzg_sg = false;
    bool soll_verbauliste_neu_5dc_vl1_tastenmodul = false;
    bool soll_verbauliste_neu_5dc_vl1_kompass = false;
    bool soll_verbauliste_neu_5dc_vl1_wfs_k = false;
    bool soll_verbauliste_neu_5dc_vl1_gsm_pager = false;
    bool soll_verbauliste_neu_5dc_vl1_infoelektronik = false;
    bool soll_verbauliste_neu_5dc_vl1_dsp = false;
    bool soll_verbauliste_neu_5dc_vl1_dab = false;
    bool soll_verbauliste_neu_5dc_vl1_telematik = false;
    bool soll_verbauliste_neu_5dc_vl1_navigation = false;
    bool soll_verbauliste_neu_5dc_vl1_tv_tuner = false;
    bool soll_verbauliste_neu_5dc_vl1_neigungsmodul_i = false;
    bool soll_verbauliste_neu_5dc_vl1_radio = false;
    bool soll_verbauliste_neu_5dc_vl1_telefon = false;

    // 0x5D2 Ident
    float ident_5d2_idt_mux_m = 0.0f;
    float ident_5d2_idt_geheimnis_1_m0 = 0.0f;
    float ident_5d2_idt_vin_4_m1 = 0.0f;
    float ident_5d2_idt_vin_11_m2 = 0.0f;
    float ident_5d2_idt_geheimnis_2_m0 = 0.0f;
    float ident_5d2_idt_vin_5_m1 = 0.0f;
    float ident_5d2_idt_vin_12_m2 = 0.0f;
    float ident_5d2_idt_geheimnis_3_m0 = 0.0f;
    float ident_5d2_idt_vin_6_m1 = 0.0f;
    float ident_5d2_idt_vin_13_m2 = 0.0f;
    float ident_5d2_idt_geheimnis_4_m0 = 0.0f;
    float ident_5d2_idt_vin_7_m1 = 0.0f;
    float ident_5d2_idt_vin_14_m2 = 0.0f;
    float ident_5d2_idt_vin_1_m0 = 0.0f;
    float ident_5d2_idt_vin_8_m1 = 0.0f;
    float ident_5d2_idt_vin_15_m2 = 0.0f;
    float ident_5d2_idt_vin_2_m0 = 0.0f;
    float ident_5d2_idt_vin_9_m1 = 0.0f;
    float ident_5d2_idt_vin_16_m2 = 0.0f;
    float ident_5d2_idt_vin_3_m0 = 0.0f;
    float ident_5d2_idt_vin_10_m1 = 0.0f;
    float ident_5d2_idt_vin_17_m2 = 0.0f;

    // 0x7D0 Diagnose_1
    float diagnose_1_7d0_di1_verlernzaehl = 0.0f;
    float diagnose_1_7d0_di1_km_stand = 0.0f;
    float diagnose_1_7d0_di1_jahr = 0.0f;
    float diagnose_1_7d0_di1_monat = 0.0f;
    float diagnose_1_7d0_di1_tag = 0.0f;
    float diagnose_1_7d0_di1_stunde = 0.0f;
    float diagnose_1_7d0_di1_minute = 0.0f;
    float diagnose_1_7d0_di1_sekunde = 0.0f;
    bool diagnose_1_7d0_di1_km_stand_alt = false;
    bool diagnose_1_7d0_di1_zeit_alt = false;

};

extern VwPqSignals vw_pq;

} // namespace params

bool decodeVwPq0x572(const CANMessage &frame);
bool decodeVwPq0x538(const CANMessage &frame);
bool decodeVwPq0x5B8(const CANMessage &frame);
bool decodeVwPq0x448(const CANMessage &frame);
bool decodeVwPq0x5D8(const CANMessage &frame);
bool decodeVwPq0x5D0(const CANMessage &frame);
bool decodeVwPq0x10C(const CANMessage &frame);
bool decodeVwPq0x534(const CANMessage &frame);
bool decodeVwPq0x712(const CANMessage &frame);
bool decodeVwPq0x512(const CANMessage &frame);
bool decodeVwPq0x112(const CANMessage &frame);
bool decodeVwPq0x514(const CANMessage &frame);
bool decodeVwPq0x590(const CANMessage &frame);
bool decodeVwPq0x530(const CANMessage &frame);
bool decodeVwPq0x700(const CANMessage &frame);
bool decodeVwPq0x500(const CANMessage &frame);
bool decodeVwPq0x100(const CANMessage &frame);
bool decodeVwPq0x704(const CANMessage &frame);
bool decodeVwPq0x384(const CANMessage &frame);
bool decodeVwPq0x580(const CANMessage &frame);
bool decodeVwPq0x588(const CANMessage &frame);
bool decodeVwPq0x488(const CANMessage &frame);
bool decodeVwPq0x480(const CANMessage &frame);
bool decodeVwPq0x380(const CANMessage &frame);
bool decodeVwPq0x288(const CANMessage &frame);
bool decodeVwPq0x280(const CANMessage &frame);
bool decodeVwPq0x106(const CANMessage &frame);
bool decodeVwPq0x104(const CANMessage &frame);
bool decodeVwPq0x102(const CANMessage &frame);
bool decodeVwPq0x7C0(const CANMessage &frame);
bool decodeVwPq0x7C2(const CANMessage &frame);
bool decodeVwPq0xC0(const CANMessage &frame);
bool decodeVwPq0xC4(const CANMessage &frame);
bool decodeVwPq0xC2(const CANMessage &frame);
bool decodeVwPq0x5DE(const CANMessage &frame);
bool decodeVwPq0x3D0(const CANMessage &frame);
bool decodeVwPq0x520(const CANMessage &frame);
bool decodeVwPq0x420(const CANMessage &frame);
bool decodeVwPq0x320(const CANMessage &frame);
bool decodeVwPq0x5E0(const CANMessage &frame);
bool decodeVwPq0x38A(const CANMessage &frame);
bool decodeVwPq0x388(const CANMessage &frame);
bool decodeVwPq0x548(const CANMessage &frame);
bool decodeVwPq0x540(const CANMessage &frame);
bool decodeVwPq0x440(const CANMessage &frame);
bool decodeVwPq0x390(const CANMessage &frame);
bool decodeVwPq0x53C(const CANMessage &frame);
bool decodeVwPq0x5C0(const CANMessage &frame);
bool decodeVwPq0x52E(const CANMessage &frame);
bool decodeVwPq0x598(const CANMessage &frame);
bool decodeVwPq0x570(const CANMessage &frame);
bool decodeVwPq0x470(const CANMessage &frame);
bool decodeVwPq0x1A8(const CANMessage &frame);
bool decodeVwPq0x4A8(const CANMessage &frame);
bool decodeVwPq0x2A0(const CANMessage &frame);
bool decodeVwPq0x4A0(const CANMessage &frame);
bool decodeVwPq0x5A0(const CANMessage &frame);
bool decodeVwPq0x1A0(const CANMessage &frame);
bool decodeVwPq0x2A8(const CANMessage &frame);
bool decodeVwPq0x578(const CANMessage &frame);
bool decodeVwPq0x2C0(const CANMessage &frame);
bool decodeVwPq0x550(const CANMessage &frame);
bool decodeVwPq0x50(const CANMessage &frame);
bool decodeVwPq0x360(const CANMessage &frame);
bool decodeVwPq0x260(const CANMessage &frame);
bool decodeVwPq0x52C(const CANMessage &frame);
bool decodeVwPq0x60E(const CANMessage &frame);
bool decodeVwPq0x368(const CANMessage &frame);
bool decodeVwPq0x56A(const CANMessage &frame);
bool decodeVwPq0xD0(const CANMessage &frame);
bool decodeVwPq0x3D2(const CANMessage &frame);
bool decodeVwPq0x3D4(const CANMessage &frame);
bool decodeVwPq0xD2(const CANMessage &frame);
bool decodeVwPq0x284(const CANMessage &frame);
bool decodeVwPq0x366(const CANMessage &frame);
bool decodeVwPq0x5BE(const CANMessage &frame);
bool decodeVwPq0x1AC(const CANMessage &frame);
bool decodeVwPq0x3A0(const CANMessage &frame);
bool decodeVwPq0x343(const CANMessage &frame);
bool decodeVwPq0x392(const CANMessage &frame);
bool decodeVwPq0x3BA(const CANMessage &frame);
bool decodeVwPq0x497(const CANMessage &frame);
bool decodeVwPq0x5B7(const CANMessage &frame);
bool decodeVwPq0x5DC(const CANMessage &frame);
bool decodeVwPq0x5D2(const CANMessage &frame);
bool decodeVwPq0x7D0(const CANMessage &frame);
bool decodeVwPq(const CANMessage &frame);
void encodeVwPq0x572(CANMessage &frame);
void encodeVwPq0x538(CANMessage &frame);
void encodeVwPq0x5B8(CANMessage &frame);
void encodeVwPq0x448(CANMessage &frame);
void encodeVwPq0x5D8(CANMessage &frame);
void encodeVwPq0x5D0(CANMessage &frame);
void encodeVwPq0x10C(CANMessage &frame);
void encodeVwPq0x534(CANMessage &frame);
void encodeVwPq0x712(CANMessage &frame);
void encodeVwPq0x512(CANMessage &frame);
void encodeVwPq0x112(CANMessage &frame);
void encodeVwPq0x514(CANMessage &frame);
void encodeVwPq0x590(CANMessage &frame);
void encodeVwPq0x530(CANMessage &frame);
void encodeVwPq0x700(CANMessage &frame);
void encodeVwPq0x500(CANMessage &frame);
void encodeVwPq0x100(CANMessage &frame);
void encodeVwPq0x704(CANMessage &frame);
void encodeVwPq0x384(CANMessage &frame);
void encodeVwPq0x580(CANMessage &frame);
void encodeVwPq0x588(CANMessage &frame);
void encodeVwPq0x488(CANMessage &frame);
void encodeVwPq0x480(CANMessage &frame);
void encodeVwPq0x380(CANMessage &frame);
void encodeVwPq0x288(CANMessage &frame);
void encodeVwPq0x280(CANMessage &frame);
void encodeVwPq0x106(CANMessage &frame);
void encodeVwPq0x104(CANMessage &frame);
void encodeVwPq0x102(CANMessage &frame);
void encodeVwPq0x7C0(CANMessage &frame);
void encodeVwPq0x7C2(CANMessage &frame);
void encodeVwPq0xC0(CANMessage &frame);
void encodeVwPq0xC4(CANMessage &frame);
void encodeVwPq0xC2(CANMessage &frame);
void encodeVwPq0x5DE(CANMessage &frame);
void encodeVwPq0x3D0(CANMessage &frame);
void encodeVwPq0x520(CANMessage &frame);
void encodeVwPq0x420(CANMessage &frame);
void encodeVwPq0x320(CANMessage &frame);
void encodeVwPq0x5E0(CANMessage &frame);
void encodeVwPq0x38A(CANMessage &frame);
void encodeVwPq0x388(CANMessage &frame);
void encodeVwPq0x548(CANMessage &frame);
void encodeVwPq0x540(CANMessage &frame);
void encodeVwPq0x440(CANMessage &frame);
void encodeVwPq0x390(CANMessage &frame);
void encodeVwPq0x53C(CANMessage &frame);
void encodeVwPq0x5C0(CANMessage &frame);
void encodeVwPq0x52E(CANMessage &frame);
void encodeVwPq0x598(CANMessage &frame);
void encodeVwPq0x570(CANMessage &frame);
void encodeVwPq0x470(CANMessage &frame);
void encodeVwPq0x1A8(CANMessage &frame);
void encodeVwPq0x4A8(CANMessage &frame);
void encodeVwPq0x2A0(CANMessage &frame);
void encodeVwPq0x4A0(CANMessage &frame);
void encodeVwPq0x5A0(CANMessage &frame);
void encodeVwPq0x1A0(CANMessage &frame);
void encodeVwPq0x2A8(CANMessage &frame);
void encodeVwPq0x578(CANMessage &frame);
void encodeVwPq0x2C0(CANMessage &frame);
void encodeVwPq0x550(CANMessage &frame);
void encodeVwPq0x50(CANMessage &frame);
void encodeVwPq0x360(CANMessage &frame);
void encodeVwPq0x260(CANMessage &frame);
void encodeVwPq0x52C(CANMessage &frame);
void encodeVwPq0x60E(CANMessage &frame);
void encodeVwPq0x368(CANMessage &frame);
void encodeVwPq0x56A(CANMessage &frame);
void encodeVwPq0xD0(CANMessage &frame);
void encodeVwPq0x3D2(CANMessage &frame);
void encodeVwPq0x3D4(CANMessage &frame);
void encodeVwPq0xD2(CANMessage &frame);
void encodeVwPq0x284(CANMessage &frame);
void encodeVwPq0x366(CANMessage &frame);
void encodeVwPq0x5BE(CANMessage &frame);
void encodeVwPq0x1AC(CANMessage &frame);
void encodeVwPq0x3A0(CANMessage &frame);
void encodeVwPq0x343(CANMessage &frame);
void encodeVwPq0x392(CANMessage &frame);
void encodeVwPq0x3BA(CANMessage &frame);
void encodeVwPq0x497(CANMessage &frame);
void encodeVwPq0x5B7(CANMessage &frame);
void encodeVwPq0x5DC(CANMessage &frame);
void encodeVwPq0x5D2(CANMessage &frame);
void encodeVwPq0x7D0(CANMessage &frame);

#endif

