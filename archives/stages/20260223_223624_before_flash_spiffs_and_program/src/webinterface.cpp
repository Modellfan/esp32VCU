#include "webinterface.h"

#include <FS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <WebServer.h>
#include "tesla_sdu_messages.h"

// Live data signals currently owned by src/main.cpp.
extern float engine_speed_rpm;
extern float throttle_position_280;
extern float pedal_position_1;
extern float pedal_position_2;
extern float coolant_temperature;
extern bool coolant_level_switch;
extern bool cruise_control_active;
extern float inlet_air_temperature;
extern float throttle_position_1_298;
extern float throttle_position_2_298;
extern float barometric_pressure_mbar;
extern uint8_t vehicle_speed_298_raw;
extern float wheel_speed_fl_legacy;
extern float wheel_speed_fr_legacy;
extern float wheel_speed_rl_legacy;
extern float wheel_speed_rr_legacy;
extern float oil_temperature;
extern float vehicle_reference_speed;
extern float wheel_speed_fl;
extern float wheel_speed_fr;
extern float wheel_speed_rl;
extern float wheel_speed_rr;
extern bool check_engine_light;
extern bool tcs_intervention;
extern bool msr_request;
extern bool abs_intervention;
extern bool abd_intervention;
extern bool fdr_intervention;
extern uint8_t asr_control_mode;
extern bool abs_warning_lamp;
extern bool brake_warning_lamp;
extern bool brake_switch;
extern bool brake_switch_inverted;
extern bool handbrake_switch;
extern float tcs_intervention_slow;
extern float tcs_intervention_fast;
extern float intervention_torque;
extern float lateral_acceleration;
extern bool reduced_power;
extern bool fan_error;
extern uint16_t fuel_used_raw;
extern float boost_pressure;
extern float ambient_temperature;
extern float oil_pressure;
extern uint8_t light_dimmer;
extern uint8_t cluster_counter;
extern float steering_angle_old_deg;
extern float steering_angle_deg;

// Override controls in main.cpp
extern bool engine_speed_override_active;
extern float engine_speed_override_rpm;
extern bool engine_temp_override_active;
extern float coolant_temperature_override_c;
extern bool engine_stat_override_active;
extern float oil_temperature_override_c;

static constexpr const char *kApSsid = "eboxster";
static WebServer server(80);

static String getLiveJson() {
    const IPAddress ip = WiFi.softAPIP();
    String payload;
    payload.reserve(2300);
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

    addNum("rpm", engine_speed_rpm, 1);
    addNum("throttle_pct", throttle_position_280, 2);
    addNum("pedal1_pct", pedal_position_1, 2);
    addNum("pedal2_pct", pedal_position_2, 2);
    addNum("coolant_c", coolant_temperature, 2);
    addBool("coolant_level", coolant_level_switch);
    addBool("cruise_active", cruise_control_active);
    addNum("iat_c", inlet_air_temperature, 2);
    addNum("thr1_298_pct", throttle_position_1_298, 2);
    addNum("thr2_298_pct", throttle_position_2_298, 2);
    addNum("baro_mbar", barometric_pressure_mbar, 1);
    addUInt("v298_raw", vehicle_speed_298_raw);

    addNum("wfl_legacy_kmh", wheel_speed_fl_legacy, 2);
    addNum("wfr_legacy_kmh", wheel_speed_fr_legacy, 2);
    addNum("wrl_legacy_kmh", wheel_speed_rl_legacy, 2);
    addNum("wrr_legacy_kmh", wheel_speed_rr_legacy, 2);
    addNum("wfl_kmh", wheel_speed_fl, 2);
    addNum("wfr_kmh", wheel_speed_fr, 2);
    addNum("wrl_kmh", wheel_speed_rl, 2);
    addNum("wrr_kmh", wheel_speed_rr, 2);

    addBool("tcs", tcs_intervention);
    addBool("msr", msr_request);
    addBool("abs", abs_intervention);
    addBool("abd", abd_intervention);
    addBool("fdr", fdr_intervention);
    addUInt("asr_mode", asr_control_mode);
    addBool("abs_lamp", abs_warning_lamp);
    addBool("brake_lamp", brake_warning_lamp);
    addBool("brake_sw", brake_switch);
    addBool("brake_sw_inv", brake_switch_inverted);
    addBool("handbrake", handbrake_switch);
    addNum("vref_kmh", vehicle_reference_speed, 2);
    addNum("tcs_slow_pct", tcs_intervention_slow, 2);
    addNum("tcs_fast_pct", tcs_intervention_fast, 2);
    addNum("itq_pct", intervention_torque, 2);
    addNum("lat_g", lateral_acceleration, 3);

    addBool("mil", check_engine_light);
    addBool("reduced_power", reduced_power);
    addBool("fan_error", fan_error);
    addUInt("fuel_raw", fuel_used_raw);
    addNum("boost", boost_pressure, 1);
    addNum("oil_temp_c", oil_temperature, 2);
    addNum("ambient_c", ambient_temperature, 2);
    addNum("oil_pressure_bar", oil_pressure, 2);
    addUInt("light_dimmer", light_dimmer);
    addUInt("cluster_counter", cluster_counter);
    addNum("steer_old_deg", steering_angle_old_deg, 2);
    addNum("steer_deg", steering_angle_deg, 2);

    addNum("set_rpm", engine_speed_override_rpm, 1);
    addNum("set_coolant", coolant_temperature_override_c, 1);
    addNum("set_oil", oil_temperature_override_c, 1);
    addBool("set_rpm_active", engine_speed_override_active);
    addBool("set_coolant_active", engine_temp_override_active);
    addBool("set_oil_active", engine_stat_override_active);

    addInt("sdu_opmode", params::tesla_sdu.opmode);
    addInt("sdu_lasterr", params::tesla_sdu.lasterr);
    addInt("sdu_status", params::tesla_sdu.status);
    addInt("sdu_din_ocur", params::tesla_sdu.din_ocur);
    addInt("sdu_din_ocur51", params::tesla_sdu.din_ocur51);
    addInt("sdu_din_bms", params::tesla_sdu.din_bms);

    addNum("sdu_udc", params::tesla_sdu.udc, 3);
    addNum("sdu_idc", params::tesla_sdu.idc, 3);

    addInt("sdu_speed", params::tesla_sdu.speed);
    addInt("sdu_cruisespeed", params::tesla_sdu.cruisespeed);
    addInt("sdu_pot", params::tesla_sdu.pot);
    addInt("sdu_pot2", params::tesla_sdu.pot2);

    addNum("sdu_regenpresent", params::tesla_sdu.regenpresent, 2);
    addNum("sdu_regenpresent32", params::tesla_sdu.regenpresent32, 2);
    addInt("sdu_seldir", params::tesla_sdu.seldir);
    addInt("sdu_seldir43", params::tesla_sdu.seldir43);

    addNum("sdu_temp_heatsink", params::tesla_sdu.temperature_heatsink, 2);
    addNum("sdu_temp_heatsink74", params::tesla_sdu.temperature_heatsink74, 2);
    addNum("sdu_uaux", params::tesla_sdu.uaux, 2);

    addStr("ap_ssid", String(kApSsid));
    addStr("ap_ip", String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));

    payload += "}";
    return payload;
}

static void handleRoot() {
    if (!SPIFFS.exists("/index.html")) {
        server.send(500, "text/plain", "index.html missing on SPIFFS");
        return;
    }
    File file = SPIFFS.open("/index.html", FILE_READ);
    server.streamFile(file, "text/html; charset=utf-8");
    file.close();
}

static void handleLive() {
    server.send(200, "application/json", getLiveJson());
}

static void handleSet() {
    if (server.hasArg("rpm")) {
        engine_speed_override_rpm = server.arg("rpm").toFloat();
        engine_speed_override_active = true;
    }
    if (server.hasArg("coolant")) {
        coolant_temperature_override_c = server.arg("coolant").toFloat();
        engine_temp_override_active = true;
    }
    if (server.hasArg("oil")) {
        oil_temperature_override_c = server.arg("oil").toFloat();
        engine_stat_override_active = true;
    }
    server.send(200, "application/json", getLiveJson());
}

static void handleNotFound() {
    server.send(404, "text/plain", "Not found");
}

void webinterfaceBegin() {
    WiFi.mode(WIFI_AP);
    const bool apOk = WiFi.softAP(kApSsid);

    const bool fsOk = SPIFFS.begin(true);
    server.on("/", HTTP_GET, handleRoot);
    server.on("/index.html", HTTP_GET, handleRoot);
    server.on("/api/live", HTTP_GET, handleLive);
    server.on("/api/set", HTTP_GET, handleSet);
    if (fsOk) {
        server.serveStatic("/assets/", SPIFFS, "/assets/");
    }
    server.onNotFound(handleNotFound);
    server.begin();

    const IPAddress ip = WiFi.softAPIP();
    Serial.printf("[WEB986] AP started SSID=%s OPEN=%s IP=%u.%u.%u.%u SPIFFS=%s\n",
                  kApSsid,
                  apOk ? "true" : "false",
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
    return kApSsid;
}

IPAddress webinterfaceApIp() {
    return WiFi.softAPIP();
}
