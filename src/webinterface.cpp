#include "webinterface.h"

#include <FS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <WebServer.h>
#include <stdlib.h>
#include "tesla_sdu_messages.h"
#include "986_vehicle_messages.h"
#include "986_ecu_messages.h"
#include "cluster_calc.h"
#include "digipot_control.h"
#include "coolant_fan_control.h"

static constexpr const char *kStaSsid = "Blacknet@Ueberlingen";
static constexpr const char *kStaPassword = "Ueberlingen2019";
static WebServer server(80);
static float gTargetChargePct = 80.0f;
static float gAcCurrentLimitA = 16.0f;

static String ipToString(const IPAddress &ip) {
    return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

static String getLiveJson() {
    const IPAddress staIp = WiFi.localIP();
    String payload;
    payload.reserve(4200);
    payload += "{";

    auto addSep = [&payload]() {
        if (payload.length() > 1) {
            payload += ",";
        }
    };
    auto addNum = [&payload, &addSep](const char *key, float value, uint8_t decimals) {
        addSep();
        payload += "\"";
        payload += key;
        payload += "\":";
        payload += String((double)value, (unsigned int)decimals);
    };
    auto addUInt = [&payload, &addSep](const char *key, unsigned value) {
        addSep();
        payload += "\"";
        payload += key;
        payload += "\":";
        payload += String(value);
    };
    auto addInt = [&payload, &addSep](const char *key, int value) {
        addSep();
        payload += "\"";
        payload += key;
        payload += "\":";
        payload += String(value);
    };
    auto addBool = [&payload, &addSep](const char *key, bool value) {
        addSep();
        payload += "\"";
        payload += key;
        payload += "\":";
        payload += value ? "true" : "false";
    };
    auto addStr = [&payload, &addSep](const char *key, const String &value) {
        addSep();
        payload += "\"";
        payload += key;
        payload += "\":\"";
        payload += value;
        payload += "\"";
    };

    addNum("rpm", params::ecu.engine_speed_rpm, 1);
    addNum("throttle_pct", params::ecu.throttle_position_280, 2);
    addNum("pedal1_pct", params::ecu.pedal_position_1, 2);
    addNum("pedal2_pct", params::ecu.pedal_position_2, 2);
    addNum("coolant_c", params::ecu.motor2_coolant_temperature, 2);
    addBool("coolant_level", params::ecu.motor2_coolant_temp_not_ok);
    addBool("cruise_active", params::ecu.motor2_cruise_control_regulating);
    addNum("iat_c", params::ecu.inlet_air_temperature, 2);
    addNum("thr1_298_pct", params::ecu.throttle_position_1_298, 2);
    addNum("thr2_298_pct", params::ecu.throttle_position_2_298, 2);
    addNum("baro_mbar", params::ecu.barometric_pressure_mbar, 1);
    addUInt("v298_raw", params::ecu.vehicle_speed_298_raw);

    addNum("wfl_legacy_kmh", params::vehicle.wheel_speed_fl_legacy, 2);
    addNum("wfr_legacy_kmh", params::vehicle.wheel_speed_fr_legacy, 2);
    addNum("wrl_legacy_kmh", params::vehicle.wheel_speed_rl_legacy, 2);
    addNum("wrr_legacy_kmh", params::vehicle.wheel_speed_rr_legacy, 2);

    addBool("tcs", params::vehicle.tcs_intervention);
    addBool("msr", params::vehicle.msr_request);
    addBool("abs", params::vehicle.abs_intervention);
    addBool("abd", params::vehicle.abd_intervention);
    addBool("fdr", params::vehicle.fdr_intervention);
    addUInt("asr_mode", params::vehicle.asr_control_mode);
    addBool("abs_lamp", params::vehicle.abs_warning_lamp);
    addBool("brake_lamp", params::vehicle.brake_warning_lamp);
    addBool("brake_sw", params::vehicle.brake_switch);
    addBool("brake_sw_inv", params::vehicle.brake_switch_inverted);
    addBool("handbrake", params::vehicle.handbrake_switch);
    addNum("vref_kmh", params::vehicle.vehicle_reference_speed, 2);
    addNum("tcs_slow_pct", params::vehicle.tcs_intervention_slow, 2);
    addNum("tcs_fast_pct", params::vehicle.tcs_intervention_fast, 2);
    addNum("msr_inv_pct", params::vehicle.msr_torque_inverse, 2);
    addNum("itq_pct", params::vehicle.intervention_torque, 2);
    addNum("lat_g", params::vehicle.lateral_acceleration, 3);
    addUInt("br1_counter", params::vehicle.br1_counter);
    addBool("br1_asr_esp", params::vehicle.br1_asr_esp);
    addBool("br1_espasr_passive", params::vehicle.br1_espasr_passive);
    addBool("br1_sta_esp", params::vehicle.br1_sta_esp);
    addBool("br1_ersatz_kmh", params::vehicle.br1_ersatz_kmh);

    addBool("mil", params::ecu.mo5_obd_2);
    addBool("check_engine_light", params::ecu.check_engine_light);
    addBool("check_engine_light_bit1", params::ecu.check_engine_light_bit1);
    addBool("engine_stat_bool2", params::ecu.engine_stat_bool2);
    addBool("reduced_power", params::ecu.reduced_power);
    addBool("fan_error", params::ecu.fan_error);
    addBool("engine_stat_bool5", params::ecu.engine_stat_bool5);
    addBool("engine_stat_bool6", params::ecu.engine_stat_bool6);
    addBool("engine_stat_bool7", params::ecu.engine_stat_bool7);
    addBool("mo5_vorgluehen", params::ecu.mo5_vorgluehen);
    addBool("mo5_e_gas", params::ecu.mo5_e_gas);
    addBool("mo5_obd_2", params::ecu.mo5_obd_2);
    addBool("mo5_heissl", params::ecu.mo5_heissl);
    addBool("mo5_klimakompr", params::ecu.mo5_klimakompr);
    addBool("mo5_feld_kuehl", params::ecu.mo5_feld_kuehl);
    addBool("mo5_kliko_red", params::ecu.mo5_kliko_red);
    addUInt("mo5_verbrauch_ul", params::ecu.mo5_verbrauch_ul);
    addBool("mo5_ueberlverb", params::ecu.mo5_ueberlverb);
    addNum("ambient_c", params::vehicle.ambient_temperature, 2);
    addNum("oil_pressure_bar", params::vehicle.oil_pressure, 2);
    addUInt("light_dimmer", params::vehicle.light_dimmer);
    addUInt("cluster_counter", params::vehicle.cluster_counter);
    addBool("vorzeichen_rb", params::vehicle.vorzeichen_rb);
    addNum("lenkwinkel_rb", params::vehicle.lenkwinkel_rb, 2);
    addBool("lws_ok_rb", params::vehicle.lws_ok_rb);
    addBool("lws_abgleich_rb", params::vehicle.lws_abgleich_rb);
    addBool("frei_lenkwinkel_1_1_rb", params::vehicle.frei_lenkwinkel_1_1_rb);
    addBool("fine_check_rb", params::vehicle.fine_check_rb);
    addBool("coarse_check_rb", params::vehicle.coarse_check_rb);
    addBool("fdr_lamp", params::vehicle.fdr_lamp);
    addBool("fdr_lamp2", params::vehicle.fdr_lamp2);
    addBool("rough_road_suppression", params::vehicle.rough_road_suppression);
    addBool("rough_road_suppression_status", params::vehicle.rough_road_suppression_status);
    addBool("psm_button_lamp", params::vehicle.psm_button_lamp);
    addBool("yaw_rate_sign", params::vehicle.yaw_rate_sign);
    addNum("yaw_rate", params::vehicle.yaw_rate, 6);
    addBool("no_actively_built_up_brake_pressure", params::vehicle.no_actively_built_up_brake_pressure);
    addBool("acc_related_error", params::vehicle.acc_related_error);
    addBool("receipt_for_acc_message", params::vehicle.receipt_for_acc_message);
    addBool("driver_braking", params::vehicle.driver_braking);
    addBool("regulator_error_fdr", params::vehicle.regulator_error_fdr);
    addNum("acceleration", params::vehicle.acceleration, 4);
    addBool("error_driver_brake_detection", params::vehicle.error_driver_brake_detection);
    addUInt("multiplex_byte4", params::vehicle.multiplex_byte4);
    addNum("brake_normal_or_offset", params::vehicle.brake_normal_or_offset, 2);
    addNum("vehicle_speed_510", params::vehicle.vehicle_speed_510, 2);
    addBool("indicator_left", params::vehicle.indicator_left);
    addBool("indicator_right", params::vehicle.indicator_right);
    addBool("tempomat_button", params::vehicle.tempomat_button);
    addBool("steeringwheel_button_up", params::vehicle.steeringwheel_button_up);
    addBool("steeringwheel_button_down", params::vehicle.steeringwheel_button_down);
    addUInt("prnd_pos", params::vehicle.prnd_pos);

    addNum("set_rpm", params::ecu.engine_speed_rpm, 1);
    addNum("set_coolant", params::ecu.motor2_coolant_temperature, 1);
    addUInt("set_oil", params::ecu.mo5_verbrauch_ul);
    addBool("set_rpm_active", true);
    addBool("set_coolant_active", true);
    addBool("set_oil_active", true);

    addInt("sdu_opmode", params::tesla_sdu.opmode);
    addInt("sdu_lasterr", params::tesla_sdu.lasterr);
    addUInt("sdu_status", params::tesla_sdu.status);
    addNum("sdu_udc", params::tesla_sdu.udc, 3);
    addNum("sdu_idc", params::tesla_sdu.idc, 3);
    addNum("sdu_il1", params::tesla_sdu.il1, 2);
    addNum("sdu_il2", params::tesla_sdu.il2, 2);
    addNum("sdu_fstat", params::tesla_sdu.fstat, 2);
    addInt("sdu_speed", params::tesla_sdu.speed);
    addInt("sdu_cruisespeed", params::tesla_sdu.cruisespeed);
    addInt("sdu_turns", params::tesla_sdu.turns);
    addNum("sdu_amp", params::tesla_sdu.amp, 2);
    addNum("sdu_angle", params::tesla_sdu.angle, 1);
    addInt("sdu_pot", params::tesla_sdu.pot);
    addInt("sdu_pot2", params::tesla_sdu.pot2);
    addNum("sdu_potnom", params::tesla_sdu.potnom, 2);
    addNum("sdu_regenpreset", params::tesla_sdu.regenpreset, 2);
    addInt("sdu_din_ocur", params::tesla_sdu.din_ocur);
    addInt("sdu_din_ocur51", params::tesla_sdu.din_ocur51);
    addInt("sdu_din_bms", params::tesla_sdu.din_bms);
    addNum("sdu_regenpresent", params::tesla_sdu.regenpresent, 2);
    addNum("sdu_regenpresent32", params::tesla_sdu.regenpresent32, 2);
    addInt("sdu_seldir", params::tesla_sdu.seldir);
    addInt("sdu_rotordir", params::tesla_sdu.rotordir);
    addInt("sdu_seldir43", params::tesla_sdu.seldir43);
    addNum("sdu_temp_heatsink", params::tesla_sdu.temperature_heatsink, 2);
    addNum("sdu_temp_heatsink74", params::tesla_sdu.temperature_heatsink74, 2);
    addNum("sdu_tmphs", params::tesla_sdu.tmphs, 2);
    addNum("sdu_tmpm", params::tesla_sdu.tmpm, 2);
    addNum("sdu_uaux", params::tesla_sdu.uaux, 2);
    addUInt("sdu_pwmio", params::tesla_sdu.pwmio);
    addUInt("sdu_canio", params::tesla_sdu.canio);
    addInt("sdu_din_cruise", params::tesla_sdu.din_cruise);
    addInt("sdu_din_start", params::tesla_sdu.din_start);
    addInt("sdu_din_brake", params::tesla_sdu.din_brake);
    addInt("sdu_din_mprot", params::tesla_sdu.din_mprot);
    addInt("sdu_din_forward", params::tesla_sdu.din_forward);
    addInt("sdu_din_reverse", params::tesla_sdu.din_reverse);
    addInt("sdu_din_emcystop", params::tesla_sdu.din_emcystop);
    addInt("sdu_din_desat", params::tesla_sdu.din_desat);
    addUInt("sdu_uptime", params::tesla_sdu.uptime);
    addNum("sdu_cpuload", params::tesla_sdu.cpuload, 1);
    addNum("sdu_ilmax", params::tesla_sdu.ilmax, 2);
    addNum("sdu_uac", params::tesla_sdu.uac, 2);
    addNum("sdu_il1rms", params::tesla_sdu.il1rms, 2);
    addNum("sdu_il2rms", params::tesla_sdu.il2rms, 2);
    addNum("sdu_boostcalc", params::tesla_sdu.boostcalc, 2);
    addNum("sdu_fweakcalc", params::tesla_sdu.fweakcalc, 2);
    addBool("cluster_activated", params::cluster.activated);
    addNum("cluster_power_percent_max", params::cluster.power_percent_max, 3);
    addNum("cluster_power_percent_dyn", params::cluster.power_percent_dyn, 3);
    addInt("cluster_needle_position", params::cluster.needle_position);
    addNum("cluster_total_fuel_l", params::cluster.total_fuel, 3);
    addBool("cluster_heatsink_temp_critical", params::cluster.heatsink_temp_critical);
    addBool("cluster_motor_temp_critical", params::cluster.motor_temp_critical);
    addBool("cluster_acceleration_run_active", params::cluster.acceleration_run_active);
    addNum("cluster_acceleration_run_time_s", params::cluster.acceleration_run_time_s, 3);
    addNum("cluster_acceleration_0_50_last_s", params::cluster.acceleration_0_50_last_s, 3);
    addNum("cluster_acceleration_0_50_best_s", params::cluster.acceleration_0_50_best_s, 3);
    addNum("cluster_acceleration_0_100_last_s", params::cluster.acceleration_0_100_last_s, 3);
    addNum("cluster_acceleration_0_100_best_s", params::cluster.acceleration_0_100_best_s, 3);
    addBool("cluster_acceleration_0_50_valid", params::cluster.acceleration_0_50_valid);
    addBool("cluster_acceleration_0_100_valid", params::cluster.acceleration_0_100_valid);
    addNum("target_charge_pct", gTargetChargePct, 0);
    addNum("ac_current_limit_a", gAcCurrentLimitA, 0);
    addUInt("digital_pot_pct", digipotPercent());
    addUInt("digital_pot_wiper", digipotWiper());
    addUInt("coolant_fan_level", coolantFanLevel());
    addBool("coolant_fan_low_output", coolantFanLowOutput());
    addBool("coolant_fan_high_output", coolantFanHighOutput());

    // ECU control page fields
    addNum("motor2_coolant_temperature", params::ecu.motor2_coolant_temperature, 2);
    addBool("motor2_brake_light_switch", params::ecu.motor2_brake_light_switch);
    addBool("motor2_brake_test_switch", params::ecu.motor2_brake_test_switch);
    addBool("motor2_coolant_temp_not_ok", params::ecu.motor2_coolant_temp_not_ok);
    addBool("motor2_ac_compressor_active", params::ecu.motor2_ac_compressor_active);
    addBool("motor2_normal_operation_active", params::ecu.motor2_normal_operation_active);
    addBool("motor2_drivetrain_coordinator_active", params::ecu.motor2_drivetrain_coordinator_active);
    addUInt("motor2_cruise_control_status", params::ecu.motor2_cruise_control_status);
    addBool("motor2_speed_limiter_selected", params::ecu.motor2_speed_limiter_selected);
    addBool("motor2_cruise_control_regulating", params::ecu.motor2_cruise_control_regulating);
    addBool("motor2_sport_error", params::ecu.motor2_sport_error);
    addBool("motor2_offroad_mode_active", params::ecu.motor2_offroad_mode_active);
    addNum("motor2_cruise_target_speed_kmh", params::ecu.motor2_cruise_target_speed_kmh, 2);
    addNum("motor2_idle_target_speed_rpm", params::ecu.motor2_idle_target_speed_rpm, 2);
    addNum("motor2_limited_torque_percent", params::ecu.motor2_limited_torque_percent, 2);
    addNum("motor2_min_torque_ignition_retard_percent", params::ecu.motor2_min_torque_ignition_retard_percent, 2);
    addNum("engine_speed_rpm", params::ecu.engine_speed_rpm, 2);
    addNum("throttle_position_280", params::ecu.throttle_position_280, 2);
    addNum("pedal_position_1", params::ecu.pedal_position_1, 2);
    addNum("pedal_position_2", params::ecu.pedal_position_2, 2);
    addNum("inlet_air_temperature", params::ecu.inlet_air_temperature, 2);
    addNum("barometric_pressure_mbar", params::ecu.barometric_pressure_mbar, 2);
    addBool("mo5_vorgluehen", params::ecu.mo5_vorgluehen);
    addBool("mo5_e_gas", params::ecu.mo5_e_gas);
    addBool("mo5_obd_2", params::ecu.mo5_obd_2);
    addBool("mo5_heissl", params::ecu.mo5_heissl);
    addBool("mo5_klimakompr", params::ecu.mo5_klimakompr);
    addBool("mo5_feld_kuehl", params::ecu.mo5_feld_kuehl);
    addBool("mo5_kliko_red", params::ecu.mo5_kliko_red);
    addUInt("mo5_verbrauch_ul", params::ecu.mo5_verbrauch_ul);
    addBool("mo5_ueberlverb", params::ecu.mo5_ueberlverb);
    addBool("check_engine_light", params::ecu.check_engine_light);
    addBool("check_engine_light_bit1", params::ecu.check_engine_light_bit1);
    addBool("engine_stat_bool2", params::ecu.engine_stat_bool2);
    addBool("reduced_power", params::ecu.reduced_power);
    addBool("fan_error", params::ecu.fan_error);
    addBool("engine_stat_bool5", params::ecu.engine_stat_bool5);
    addBool("engine_stat_bool6", params::ecu.engine_stat_bool6);
    addBool("engine_stat_bool7", params::ecu.engine_stat_bool7);

    addStr("ap_ssid", String(kStaSsid));
    addStr("ap_ip", ipToString(staIp));
    addStr("sta_ssid", String(kStaSsid));
    addBool("sta_connected", WiFi.status() == WL_CONNECTED);
    addStr("sta_ip", ipToString(staIp));

    payload += "}";
    return payload;
}

static void streamHtmlFile(const char *path) {
    if (!SPIFFS.exists(path)) {
        server.send(500, "text/plain", String(path) + " missing on SPIFFS");
        return;
    }
    File file = SPIFFS.open(path, FILE_READ);
    server.streamFile(file, "text/html; charset=utf-8");
    file.close();
}

static void handleRoot() {
    streamHtmlFile("/index.html");
}

static void handleTouch() {
    streamHtmlFile("/touch.html");
}

static void handleLive() {
    server.send(200, "application/json", getLiveJson());
}

static void handleSet() {
    auto clampFloat = [](float value, float minimum, float maximum) {
        if (value < minimum) {
            return minimum;
        }
        if (value > maximum) {
            return maximum;
        }
        return value;
    };
    if (server.hasArg("rpm")) {
        params::ecu.engine_speed_rpm = server.arg("rpm").toFloat();
    }
    if (server.hasArg("coolant")) {
        params::ecu.motor2_coolant_temperature = server.arg("coolant").toFloat();
    }
    if (server.hasArg("oil")) {
        params::ecu.mo5_verbrauch_ul = (uint32_t)strtoul(server.arg("oil").c_str(), nullptr, 10);
    }
    if (server.hasArg("target_charge_pct")) {
        gTargetChargePct = clampFloat(server.arg("target_charge_pct").toFloat(), 50.0f, 100.0f);
    }
    if (server.hasArg("ac_current_limit_a")) {
        gAcCurrentLimitA = clampFloat(server.arg("ac_current_limit_a").toFloat(), 6.0f, 32.0f);
    }
    if (server.hasArg("digital_pot_pct")) {
        const float pct = clampFloat(server.arg("digital_pot_pct").toFloat(), 0.0f, 100.0f);
        digipotSetPercent((uint8_t)(pct + 0.5f));
    }
    if (server.hasArg("coolant_fan_level")) {
        coolantFanSetLevel((uint8_t)constrain(server.arg("coolant_fan_level").toInt(), 0, 2));
    }
    server.send(200, "application/json", getLiveJson());
}

static bool parseBoolArg(const String &v) {
    return v == "1" || v == "true" || v == "TRUE" || v == "on" || v == "ON";
}

static void handleSetEcu() {
    auto setFloat = [&](const char *key, float &dst) {
        if (server.hasArg(key)) {
            dst = server.arg(key).toFloat();
        }
    };
    auto setU8 = [&](const char *key, uint8_t &dst) {
        if (server.hasArg(key)) {
            dst = (uint8_t)server.arg(key).toInt();
        }
    };
    auto setU32 = [&](const char *key, uint32_t &dst) {
        if (server.hasArg(key)) {
            dst = (uint32_t)strtoul(server.arg(key).c_str(), nullptr, 10);
        }
    };
    auto setBool = [&](const char *key, bool &dst) {
        if (server.hasArg(key)) {
            dst = parseBoolArg(server.arg(key));
        }
    };

    setFloat("motor2_coolant_temperature", params::ecu.motor2_coolant_temperature);
    setBool("motor2_brake_light_switch", params::ecu.motor2_brake_light_switch);
    setBool("motor2_brake_test_switch", params::ecu.motor2_brake_test_switch);
    setBool("motor2_coolant_temp_not_ok", params::ecu.motor2_coolant_temp_not_ok);
    setBool("motor2_ac_compressor_active", params::ecu.motor2_ac_compressor_active);
    setBool("motor2_normal_operation_active", params::ecu.motor2_normal_operation_active);
    setBool("motor2_drivetrain_coordinator_active", params::ecu.motor2_drivetrain_coordinator_active);
    setU8("motor2_cruise_control_status", params::ecu.motor2_cruise_control_status);
    setBool("motor2_speed_limiter_selected", params::ecu.motor2_speed_limiter_selected);
    setBool("motor2_cruise_control_regulating", params::ecu.motor2_cruise_control_regulating);
    setBool("motor2_sport_error", params::ecu.motor2_sport_error);
    setBool("motor2_offroad_mode_active", params::ecu.motor2_offroad_mode_active);
    setFloat("motor2_cruise_target_speed_kmh", params::ecu.motor2_cruise_target_speed_kmh);
    setFloat("motor2_idle_target_speed_rpm", params::ecu.motor2_idle_target_speed_rpm);
    setFloat("motor2_limited_torque_percent", params::ecu.motor2_limited_torque_percent);
    setFloat("motor2_min_torque_ignition_retard_percent", params::ecu.motor2_min_torque_ignition_retard_percent);

    setFloat("engine_speed_rpm", params::ecu.engine_speed_rpm);
    setFloat("throttle_position_280", params::ecu.throttle_position_280);
    setFloat("pedal_position_1", params::ecu.pedal_position_1);
    setFloat("pedal_position_2", params::ecu.pedal_position_2);
    setFloat("inlet_air_temperature", params::ecu.inlet_air_temperature);
    setFloat("barometric_pressure_mbar", params::ecu.barometric_pressure_mbar);
    setBool("mo5_vorgluehen", params::ecu.mo5_vorgluehen);
    setBool("mo5_e_gas", params::ecu.mo5_e_gas);
    setBool("mo5_obd_2", params::ecu.mo5_obd_2);
    setBool("mo5_heissl", params::ecu.mo5_heissl);
    setBool("mo5_klimakompr", params::ecu.mo5_klimakompr);
    setBool("mo5_feld_kuehl", params::ecu.mo5_feld_kuehl);
    setBool("mo5_kliko_red", params::ecu.mo5_kliko_red);
    setU32("mo5_verbrauch_ul", params::ecu.mo5_verbrauch_ul);
    setBool("mo5_ueberlverb", params::ecu.mo5_ueberlverb);
    setBool("check_engine_light", params::ecu.check_engine_light);
    setBool("check_engine_light_bit1", params::ecu.check_engine_light_bit1);
    setBool("engine_stat_bool2", params::ecu.engine_stat_bool2);
    setBool("reduced_power", params::ecu.reduced_power);
    setBool("fan_error", params::ecu.fan_error);
    setBool("engine_stat_bool5", params::ecu.engine_stat_bool5);
    setBool("engine_stat_bool6", params::ecu.engine_stat_bool6);
    setBool("engine_stat_bool7", params::ecu.engine_stat_bool7);
    if (server.hasArg("digital_pot_pct")) {
        const int value = server.arg("digital_pot_pct").toInt();
        digipotSetPercent((uint8_t)constrain(value, 0, 100));
    }
    if (server.hasArg("coolant_fan_level")) {
        coolantFanSetLevel((uint8_t)constrain(server.arg("coolant_fan_level").toInt(), 0, 2));
    }

    server.send(200, "application/json", getLiveJson());
}

static void handleSetCluster() {
    if (server.hasArg("activated")) {
        params::cluster.activated = parseBoolArg(server.arg("activated"));
    }
    server.send(200, "application/json", getLiveJson());
}

static void handleNotFound() {
    server.send(404, "text/plain", "Not found");
}

void webinterfaceBegin() {
    WiFi.mode(WIFI_STA);
    WiFi.setHostname("eboxster-vcu");
    WiFi.begin(kStaSsid, kStaPassword);

    const uint32_t connectStart = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - connectStart) < 15000UL) {
        delay(250);
    }

    const bool fsOk = SPIFFS.begin(true);
    server.on("/", HTTP_GET, handleRoot);
    server.on("/index.html", HTTP_GET, handleRoot);
    server.on("/touch.html", HTTP_GET, handleTouch);
    server.on("/api/live", HTTP_GET, handleLive);
    server.on("/api/set", HTTP_GET, handleSet);
    server.on("/api/set-ecu", HTTP_GET, handleSetEcu);
    server.on("/api/set-cluster", HTTP_GET, handleSetCluster);
    if (fsOk) {
        server.serveStatic("/assets/", SPIFFS, "/assets/");
        server.serveStatic("/touch/", SPIFFS, "/touch/");
        // Serve PWA and nav icons from short SPIFFS paths to avoid SPIFFS file-name limits.
        server.serveStatic("/favicon-16x16.png", SPIFFS, "/fav16.png");
        server.serveStatic("/favicon-32x32.png", SPIFFS, "/fav32.png");
        server.serveStatic("/favicon.ico", SPIFFS, "/fav.ico");
        server.serveStatic("/apple-touch-icon.png", SPIFFS, "/app.png");
        server.serveStatic("/apple-touch-icon-180x180.png", SPIFFS, "/app.png");
        server.serveStatic("/apple-touch-icon-precomposed.png", SPIFFS, "/app.png");
        server.serveStatic("/android-chrome-192x192.png", SPIFFS, "/a192.png");
        server.serveStatic("/android-chrome-512x512.png", SPIFFS, "/a512.png");
        server.serveStatic("/site.webmanifest", SPIFFS, "/site.webmanifest");
        server.serveStatic("/safari-pinned-tab.svg", SPIFFS, "/mask.svg");
        server.serveStatic("/i_car.svg", SPIFFS, "/i_car.svg");
        server.serveStatic("/i_ecu.svg", SPIFFS, "/i_ecu.svg");
        server.serveStatic("/i_sdu.svg", SPIFFS, "/i_sdu.svg");
        server.serveStatic("/i_cluster.svg", SPIFFS, "/i_cluster.svg");
    }
    server.onNotFound(handleNotFound);
    server.begin();

    const IPAddress ip = WiFi.localIP();
    Serial.printf("[WEB986] STA SSID=%s connected=%s IP=%u.%u.%u.%u SPIFFS=%s\n",
                  kStaSsid,
                  WiFi.status() == WL_CONNECTED ? "true" : "false",
                  ip[0],
                  ip[1],
                  ip[2],
                  ip[3],
                  fsOk ? "OK" : "FAIL");
}

void webinterfaceHandle() {
    server.handleClient();
}

const char *webinterfaceApSsid() {
    return kStaSsid;
}

IPAddress webinterfaceApIp() {
    return WiFi.localIP();
}
