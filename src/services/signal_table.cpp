#include "signal_table.h"

#include "params_lock.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <type_traits>

namespace {

// Storage type from the C++ member type (int and int32_t differ on Xtensa, so map by size).
template <typename T> constexpr SigStorage storageOf() {
    static_assert(sizeof(T) <= 4, "signal wider than 32 bit");
    return std::is_same<T, bool>::value ? S_BOOL
         : std::is_floating_point<T>::value ? S_F32
         : std::is_signed<T>::value ? (sizeof(T) == 1 ? S_I8 : sizeof(T) == 2 ? S_I16 : S_I32)
                                    : (sizeof(T) == 1 ? S_U8 : sizeof(T) == 2 ? S_U16 : S_U32);
}

}  // namespace

#define SIG(name, group, Struct, member, format, decimals, rx, flags) \
    { name, group, (uint16_t)offsetof(Struct, member), storageOf<decltype(Struct::member)>(), format, decimals, rx, flags }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
const SignalDef kSignals[] = {
    // Generated 2026-10-28 from the former getLiveJson(); field names unchanged.
    SIG("rpm", G_ECU, params::EcuSignals, engine_speed_rpm, F_NUM, 1, RX_NONE, 0),
    SIG("throttle_pct", G_ECU, params::EcuSignals, throttle_position_280, F_NUM, 2, RX_NONE, 0),
    SIG("pedal1_pct", G_ECU, params::EcuSignals, pedal_position_1, F_NUM, 2, RX_NONE, 0),
    SIG("pedal2_pct", G_ECU, params::EcuSignals, pedal_position_2, F_NUM, 2, RX_NONE, 0),
    SIG("coolant_c", G_ECU, params::EcuSignals, motor2_coolant_temperature, F_NUM, 2, RX_NONE, 0),
    SIG("coolant_level", G_ECU, params::EcuSignals, motor2_coolant_temp_not_ok, F_BOOL, 0, RX_NONE, 0),
    SIG("cruise_active", G_ECU, params::EcuSignals, motor2_cruise_control_regulating, F_BOOL, 0, RX_NONE, 0),
    SIG("iat_c", G_ECU, params::EcuSignals, inlet_air_temperature, F_NUM, 2, RX_NONE, 0),
    SIG("thr1_298_pct", G_ECU, params::EcuSignals, throttle_position_1_298, F_NUM, 2, RX_NONE, 0),
    SIG("thr2_298_pct", G_ECU, params::EcuSignals, throttle_position_2_298, F_NUM, 2, RX_NONE, 0),
    SIG("baro_mbar", G_ECU, params::EcuSignals, barometric_pressure_mbar, F_NUM, 1, RX_NONE, 0),
    SIG("v298_raw", G_ECU, params::EcuSignals, vehicle_speed_298_raw, F_UINT, 0, RX_NONE, 0),
    SIG("wfl_legacy_kmh", G_VEHICLE, params::VehicleSignals, wheel_speed_fl_legacy, F_NUM, 2, RX_VEH_2A8, 0),
    SIG("wfr_legacy_kmh", G_VEHICLE, params::VehicleSignals, wheel_speed_fr_legacy, F_NUM, 2, RX_VEH_2A8, 0),
    SIG("wrl_legacy_kmh", G_VEHICLE, params::VehicleSignals, wheel_speed_rl_legacy, F_NUM, 2, RX_VEH_2A8, 0),
    SIG("wrr_legacy_kmh", G_VEHICLE, params::VehicleSignals, wheel_speed_rr_legacy, F_NUM, 2, RX_VEH_2A8, 0),
    SIG("tcs", G_VEHICLE, params::VehicleSignals, tcs_intervention, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("msr", G_VEHICLE, params::VehicleSignals, msr_request, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("abs", G_VEHICLE, params::VehicleSignals, abs_intervention, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("abd", G_VEHICLE, params::VehicleSignals, abd_intervention, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("fdr", G_VEHICLE, params::VehicleSignals, fdr_intervention, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("asr_mode", G_VEHICLE, params::VehicleSignals, asr_control_mode, F_UINT, 0, RX_VEH_1A0, 0),
    SIG("abs_lamp", G_VEHICLE, params::VehicleSignals, abs_warning_lamp, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("brake_lamp", G_VEHICLE, params::VehicleSignals, brake_warning_lamp, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("brake_sw", G_VEHICLE, params::VehicleSignals, brake_switch, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("brake_sw_inv", G_VEHICLE, params::VehicleSignals, brake_switch_inverted, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("handbrake", G_VEHICLE, params::VehicleSignals, handbrake_switch, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("vref_kmh", G_VEHICLE, params::VehicleSignals, vehicle_reference_speed, F_NUM, 2, RX_VEH_1A0, 0),
    SIG("tcs_slow_pct", G_VEHICLE, params::VehicleSignals, tcs_intervention_slow, F_NUM, 2, RX_VEH_1A0, 0),
    SIG("tcs_fast_pct", G_VEHICLE, params::VehicleSignals, tcs_intervention_fast, F_NUM, 2, RX_VEH_1A0, 0),
    SIG("msr_inv_pct", G_VEHICLE, params::VehicleSignals, msr_torque_inverse, F_NUM, 2, RX_VEH_1A0, 0),
    SIG("itq_pct", G_VEHICLE, params::VehicleSignals, intervention_torque, F_NUM, 2, RX_VEH_1A0, 0),
    SIG("lat_g", G_VEHICLE, params::VehicleSignals, lateral_acceleration, F_NUM, 3, RX_VEH_1A0, 0),
    SIG("br1_counter", G_VEHICLE, params::VehicleSignals, br1_counter, F_UINT, 0, RX_VEH_1A0, 0),
    SIG("br1_asr_esp", G_VEHICLE, params::VehicleSignals, br1_asr_esp, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("br1_espasr_passive", G_VEHICLE, params::VehicleSignals, br1_espasr_passive, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("br1_sta_esp", G_VEHICLE, params::VehicleSignals, br1_sta_esp, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("br1_ersatz_kmh", G_VEHICLE, params::VehicleSignals, br1_ersatz_kmh, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("mil", G_ECU, params::EcuSignals, mo5_obd_2, F_BOOL, 0, RX_NONE, 0),
    SIG("check_engine_light", G_ECU, params::EcuSignals, check_engine_light, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("check_engine_light_bit1", G_ECU, params::EcuSignals, check_engine_light_bit1, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("engine_stat_bool2", G_ECU, params::EcuSignals, engine_stat_bool2, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("reduced_power", G_ECU, params::EcuSignals, reduced_power, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("fan_error", G_ECU, params::EcuSignals, fan_error, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("engine_stat_bool5", G_ECU, params::EcuSignals, engine_stat_bool5, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("engine_stat_bool6", G_ECU, params::EcuSignals, engine_stat_bool6, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("engine_stat_bool7", G_ECU, params::EcuSignals, engine_stat_bool7, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("mo5_vorgluehen", G_ECU, params::EcuSignals, mo5_vorgluehen, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("mo5_e_gas", G_ECU, params::EcuSignals, mo5_e_gas, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("mo5_obd_2", G_ECU, params::EcuSignals, mo5_obd_2, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("mo5_heissl", G_ECU, params::EcuSignals, mo5_heissl, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("mo5_klimakompr", G_ECU, params::EcuSignals, mo5_klimakompr, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("mo5_feld_kuehl", G_ECU, params::EcuSignals, mo5_feld_kuehl, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("mo5_kliko_red", G_ECU, params::EcuSignals, mo5_kliko_red, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("mo5_verbrauch_ul", G_ECU, params::EcuSignals, mo5_verbrauch_ul, F_UINT, 0, RX_NONE, OVR_FUEL),
    SIG("mo5_ueberlverb", G_ECU, params::EcuSignals, mo5_ueberlverb, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("ambient_c", G_VEHICLE, params::VehicleSignals, ambient_temperature, F_NUM, 2, RX_VEH_520, 0),
    SIG("oil_pressure_bar", G_VEHICLE, params::VehicleSignals, oil_pressure, F_NUM, 2, RX_VEH_520, 0),
    SIG("light_dimmer", G_VEHICLE, params::VehicleSignals, light_dimmer, F_UINT, 0, RX_VEH_520, 0),
    SIG("cluster_counter", G_VEHICLE, params::VehicleSignals, cluster_counter, F_UINT, 0, RX_VEH_520, 0),
    SIG("vorzeichen_rb", G_VEHICLE, params::VehicleSignals, vorzeichen_rb, F_BOOL, 0, RX_VEH_0C0, 0),
    SIG("lenkwinkel_rb", G_VEHICLE, params::VehicleSignals, lenkwinkel_rb, F_NUM, 2, RX_VEH_0C0, 0),
    SIG("lws_ok_rb", G_VEHICLE, params::VehicleSignals, lws_ok_rb, F_BOOL, 0, RX_VEH_0C0, 0),
    SIG("lws_abgleich_rb", G_VEHICLE, params::VehicleSignals, lws_abgleich_rb, F_BOOL, 0, RX_VEH_0C0, 0),
    SIG("frei_lenkwinkel_1_1_rb", G_VEHICLE, params::VehicleSignals, frei_lenkwinkel_1_1_rb, F_BOOL, 0, RX_VEH_0C0, 0),
    SIG("fine_check_rb", G_VEHICLE, params::VehicleSignals, fine_check_rb, F_BOOL, 0, RX_VEH_0C0, 0),
    SIG("coarse_check_rb", G_VEHICLE, params::VehicleSignals, coarse_check_rb, F_BOOL, 0, RX_VEH_0C0, 0),
    SIG("fdr_lamp", G_VEHICLE, params::VehicleSignals, fdr_lamp, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("fdr_lamp2", G_VEHICLE, params::VehicleSignals, fdr_lamp2, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("rough_road_suppression", G_VEHICLE, params::VehicleSignals, rough_road_suppression, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("rough_road_suppression_status", G_VEHICLE, params::VehicleSignals, rough_road_suppression_status, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("psm_button_lamp", G_VEHICLE, params::VehicleSignals, psm_button_lamp, F_BOOL, 0, RX_VEH_1A0, 0),
    SIG("yaw_rate_sign", G_VEHICLE, params::VehicleSignals, yaw_rate_sign, F_BOOL, 0, RX_VEH_4A1, 0),
    SIG("yaw_rate", G_VEHICLE, params::VehicleSignals, yaw_rate, F_NUM, 6, RX_VEH_4A1, 0),
    SIG("no_actively_built_up_brake_pressure", G_VEHICLE, params::VehicleSignals, no_actively_built_up_brake_pressure, F_BOOL, 0, RX_VEH_4A1, 0),
    SIG("acc_related_error", G_VEHICLE, params::VehicleSignals, acc_related_error, F_BOOL, 0, RX_VEH_4A1, 0),
    SIG("receipt_for_acc_message", G_VEHICLE, params::VehicleSignals, receipt_for_acc_message, F_BOOL, 0, RX_VEH_4A1, 0),
    SIG("driver_braking", G_VEHICLE, params::VehicleSignals, driver_braking, F_BOOL, 0, RX_VEH_4A1, 0),
    SIG("regulator_error_fdr", G_VEHICLE, params::VehicleSignals, regulator_error_fdr, F_BOOL, 0, RX_VEH_4A1, 0),
    SIG("acceleration", G_VEHICLE, params::VehicleSignals, acceleration, F_NUM, 4, RX_VEH_4A1, 0),
    SIG("error_driver_brake_detection", G_VEHICLE, params::VehicleSignals, error_driver_brake_detection, F_BOOL, 0, RX_VEH_4A1, 0),
    SIG("multiplex_byte4", G_VEHICLE, params::VehicleSignals, multiplex_byte4, F_UINT, 0, RX_VEH_4A1, 0),
    SIG("brake_normal_or_offset", G_VEHICLE, params::VehicleSignals, brake_normal_or_offset, F_NUM, 2, RX_VEH_4A1, 0),
    SIG("vehicle_speed_510", G_VEHICLE, params::VehicleSignals, vehicle_speed_510, F_NUM, 2, RX_VEH_510, 0),
    SIG("indicator_left", G_VEHICLE, params::VehicleSignals, indicator_left, F_BOOL, 0, RX_VEH_510, 0),
    SIG("indicator_right", G_VEHICLE, params::VehicleSignals, indicator_right, F_BOOL, 0, RX_VEH_510, 0),
    SIG("tempomat_button", G_VEHICLE, params::VehicleSignals, tempomat_button, F_BOOL, 0, RX_VEH_510, 0),
    SIG("steeringwheel_button_up", G_VEHICLE, params::VehicleSignals, steeringwheel_button_up, F_BOOL, 0, RX_VEH_510, 0),
    SIG("steeringwheel_button_down", G_VEHICLE, params::VehicleSignals, steeringwheel_button_down, F_BOOL, 0, RX_VEH_510, 0),
    SIG("prnd_pos", G_VEHICLE, params::VehicleSignals, prnd_pos, F_UINT, 0, RX_VEH_440, 0),
    SIG("set_rpm", G_ECU, params::EcuSignals, engine_speed_rpm, F_NUM, 1, RX_NONE, 0),
    SIG("set_coolant", G_ECU, params::EcuSignals, motor2_coolant_temperature, F_NUM, 1, RX_NONE, 0),
    SIG("set_oil", G_ECU, params::EcuSignals, mo5_verbrauch_ul, F_UINT, 0, RX_NONE, 0),
    SIG("sdu_opmode", G_SDU, params::TeslaSduSignals, opmode, F_INT, 0, RX_SDU_505, 0),
    SIG("sdu_lasterr", G_SDU, params::TeslaSduSignals, lasterr, F_INT, 0, RX_SDU_505, 0),
    SIG("sdu_status", G_SDU, params::TeslaSduSignals, status, F_UINT, 0, RX_SDU_505, 0),
    SIG("sdu_udc", G_SDU, params::TeslaSduSignals, udc, F_NUM, 3, RX_SDU_500, 0),
    SIG("sdu_idc", G_SDU, params::TeslaSduSignals, idc, F_NUM, 3, RX_SDU_500, 0),
    SIG("sdu_il1", G_SDU, params::TeslaSduSignals, il1, F_NUM, 2, RX_SDU_500, 0),
    SIG("sdu_il2", G_SDU, params::TeslaSduSignals, il2, F_NUM, 2, RX_SDU_500, 0),
    SIG("sdu_fstat", G_SDU, params::TeslaSduSignals, fstat, F_NUM, 2, RX_SDU_501, 0),
    SIG("sdu_speed", G_SDU, params::TeslaSduSignals, speed, F_INT, 0, RX_SDU_501, 0),
    SIG("sdu_cruisespeed", G_SDU, params::TeslaSduSignals, cruisespeed, F_INT, 0, RX_SDU_507, 0),
    SIG("sdu_turns", G_SDU, params::TeslaSduSignals, turns, F_INT, 0, RX_SDU_508, 0),
    SIG("sdu_amp", G_SDU, params::TeslaSduSignals, amp, F_NUM, 2, RX_SDU_503, 0),
    SIG("sdu_angle", G_SDU, params::TeslaSduSignals, angle, F_NUM, 1, RX_SDU_503, 0),
    SIG("sdu_pot", G_SDU, params::TeslaSduSignals, pot, F_INT, 0, RX_SDU_503, 0),
    SIG("sdu_pot2", G_SDU, params::TeslaSduSignals, pot2, F_INT, 0, RX_SDU_503, 0),
    SIG("sdu_potnom", G_SDU, params::TeslaSduSignals, potnom, F_NUM, 2, RX_SDU_503, 0),
    SIG("sdu_regenpreset", G_SDU, params::TeslaSduSignals, regenpreset, F_NUM, 2, RX_SDU_504, 0),
    SIG("sdu_din_ocur", G_SDU, params::TeslaSduSignals, din_ocur, F_INT, 0, RX_SDU_506, 0),
    SIG("sdu_din_ocur51", G_SDU, params::TeslaSduSignals, din_ocur51, F_INT, 0, RX_SDU_506, 0),
    SIG("sdu_din_bms", G_SDU, params::TeslaSduSignals, din_bms, F_INT, 0, RX_SDU_507, 0),
    SIG("sdu_regenpresent", G_SDU, params::TeslaSduSignals, regenpresent, F_NUM, 2, RX_SDU_504, 0),
    SIG("sdu_regenpresent32", G_SDU, params::TeslaSduSignals, regenpresent32, F_NUM, 2, RX_SDU_504, 0),
    SIG("sdu_seldir", G_SDU, params::TeslaSduSignals, seldir, F_INT, 0, RX_SDU_504, 0),
    SIG("sdu_rotordir", G_SDU, params::TeslaSduSignals, rotordir, F_INT, 0, RX_SDU_504, 0),
    SIG("sdu_seldir43", G_SDU, params::TeslaSduSignals, seldir43, F_INT, 0, RX_SDU_504, 0),
    SIG("sdu_temp_heatsink", G_SDU, params::TeslaSduSignals, temperature_heatsink, F_NUM, 2, RX_SDU_504, 0),
    SIG("sdu_temp_heatsink74", G_SDU, params::TeslaSduSignals, temperature_heatsink74, F_NUM, 2, RX_SDU_504, 0),
    SIG("sdu_tmphs", G_SDU, params::TeslaSduSignals, tmphs, F_NUM, 2, RX_SDU_504, 0),
    SIG("sdu_tmpm", G_SDU, params::TeslaSduSignals, tmpm, F_NUM, 2, RX_SDU_504, 0),
    SIG("sdu_uaux", G_SDU, params::TeslaSduSignals, uaux, F_NUM, 2, RX_SDU_504, 0),
    SIG("sdu_pwmio", G_SDU, params::TeslaSduSignals, pwmio, F_UINT, 0, RX_SDU_505, 0),
    SIG("sdu_canio", G_SDU, params::TeslaSduSignals, canio, F_UINT, 0, RX_SDU_505, 0),
    SIG("sdu_din_cruise", G_SDU, params::TeslaSduSignals, din_cruise, F_INT, 0, RX_SDU_506, 0),
    SIG("sdu_din_start", G_SDU, params::TeslaSduSignals, din_start, F_INT, 0, RX_SDU_506, 0),
    SIG("sdu_din_brake", G_SDU, params::TeslaSduSignals, din_brake, F_INT, 0, RX_SDU_506, 0),
    SIG("sdu_din_mprot", G_SDU, params::TeslaSduSignals, din_mprot, F_INT, 0, RX_SDU_506, 0),
    SIG("sdu_din_forward", G_SDU, params::TeslaSduSignals, din_forward, F_INT, 0, RX_SDU_506, 0),
    SIG("sdu_din_reverse", G_SDU, params::TeslaSduSignals, din_reverse, F_INT, 0, RX_SDU_506, 0),
    SIG("sdu_din_emcystop", G_SDU, params::TeslaSduSignals, din_emcystop, F_INT, 0, RX_SDU_506, 0),
    SIG("sdu_din_desat", G_SDU, params::TeslaSduSignals, din_desat, F_INT, 0, RX_SDU_507, 0),
    SIG("sdu_uptime", G_SDU, params::TeslaSduSignals, uptime, F_UINT, 0, RX_SDU_508, 0),
    SIG("sdu_uptime_s", G_SDU, params::TeslaSduSignals, uptime_s, F_NUM, 2, RX_SDU_508, 0),
    SIG("sdu_cpuload", G_SDU, params::TeslaSduSignals, cpuload, F_NUM, 1, RX_SDU_505, 0),
    SIG("sdu_ilmax", G_SDU, params::TeslaSduSignals, ilmax, F_NUM, 2, RX_SDU_501, 0),
    SIG("sdu_uac", G_SDU, params::TeslaSduSignals, uac, F_NUM, 2, RX_SDU_501, 0),
    SIG("sdu_il1rms", G_SDU, params::TeslaSduSignals, il1rms, F_NUM, 2, RX_SDU_502, 0),
    SIG("sdu_il2rms", G_SDU, params::TeslaSduSignals, il2rms, F_NUM, 2, RX_SDU_502, 0),
    SIG("sdu_boostcalc", G_SDU, params::TeslaSduSignals, boostcalc, F_NUM, 2, RX_SDU_502, 0),
    SIG("sdu_fweakcalc", G_SDU, params::TeslaSduSignals, fweakcalc, F_NUM, 2, RX_SDU_502, 0),
    SIG("cluster_activated", G_CLUSTER, params::ClusterSignals, activated, F_BOOL, 0, RX_NONE, 0),
    SIG("cluster_power_percent_max", G_CLUSTER, params::ClusterSignals, power_percent_max, F_NUM, 3, RX_NONE, 0),
    SIG("cluster_power_percent_dyn", G_CLUSTER, params::ClusterSignals, power_percent_dyn, F_NUM, 3, RX_NONE, 0),
    SIG("cluster_needle_position", G_CLUSTER, params::ClusterSignals, needle_position, F_INT, 0, RX_NONE, 0),
    SIG("cluster_total_fuel_l", G_CLUSTER, params::ClusterSignals, total_fuel, F_NUM, 3, RX_NONE, 0),
    SIG("cluster_heatsink_temp_critical", G_CLUSTER, params::ClusterSignals, heatsink_temp_critical, F_BOOL, 0, RX_NONE, 0),
    SIG("cluster_motor_temp_critical", G_CLUSTER, params::ClusterSignals, motor_temp_critical, F_BOOL, 0, RX_NONE, 0),
    SIG("cluster_acceleration_run_active", G_CLUSTER, params::ClusterSignals, acceleration_run_active, F_BOOL, 0, RX_NONE, 0),
    SIG("cluster_acceleration_run_time_s", G_CLUSTER, params::ClusterSignals, acceleration_run_time_s, F_NUM, 3, RX_NONE, 0),
    SIG("cluster_acceleration_0_50_last_s", G_CLUSTER, params::ClusterSignals, acceleration_0_50_last_s, F_NUM, 3, RX_NONE, 0),
    SIG("cluster_acceleration_0_50_best_s", G_CLUSTER, params::ClusterSignals, acceleration_0_50_best_s, F_NUM, 3, RX_NONE, 0),
    SIG("cluster_acceleration_0_100_last_s", G_CLUSTER, params::ClusterSignals, acceleration_0_100_last_s, F_NUM, 3, RX_NONE, 0),
    SIG("cluster_acceleration_0_100_best_s", G_CLUSTER, params::ClusterSignals, acceleration_0_100_best_s, F_NUM, 3, RX_NONE, 0),
    SIG("cluster_acceleration_0_50_valid", G_CLUSTER, params::ClusterSignals, acceleration_0_50_valid, F_BOOL, 0, RX_NONE, 0),
    SIG("cluster_acceleration_0_100_valid", G_CLUSTER, params::ClusterSignals, acceleration_0_100_valid, F_BOOL, 0, RX_NONE, 0),
    SIG("target_charge_pct", G_CHARGING, params::ChargingSignals, target_charge_pct, F_NUM, 0, RX_NONE, 0),
    SIG("ac_current_limit_a", G_CHARGING, params::ChargingSignals, ac_current_limit_a, F_NUM, 0, RX_NONE, 0),
    SIG("digital_pot_pct", G_DIGIPOT, params::DigipotSignals, digipot_applied_pct, F_UINT, 0, RX_NONE, 0),
    SIG("digital_pot_wiper", G_DIGIPOT, params::DigipotSignals, digipot_wiper, F_UINT, 0, RX_NONE, 0),
    SIG("coolant_fan_level", G_FAN, params::FanSignals, fan_level, F_UINT, 0, RX_NONE, 0),
    SIG("coolant_fan_low_output", G_FAN, params::FanSignals, fan_stage1, F_BOOL, 0, RX_NONE, 0),
    SIG("coolant_fan_high_output", G_FAN, params::FanSignals, fan_stage2, F_BOOL, 0, RX_NONE, 0),
    SIG("motor2_coolant_temperature", G_ECU, params::EcuSignals, motor2_coolant_temperature, F_NUM, 2, RX_NONE, OVR_COOLANT_TEMP),
    SIG("motor2_brake_light_switch", G_ECU, params::EcuSignals, motor2_brake_light_switch, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("motor2_brake_test_switch", G_ECU, params::EcuSignals, motor2_brake_test_switch, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("motor2_coolant_temp_not_ok", G_ECU, params::EcuSignals, motor2_coolant_temp_not_ok, F_BOOL, 0, RX_NONE, OVR_COOLANT_WARNING),
    SIG("motor2_ac_compressor_active", G_ECU, params::EcuSignals, motor2_ac_compressor_active, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("motor2_normal_operation_active", G_ECU, params::EcuSignals, motor2_normal_operation_active, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("motor2_drivetrain_coordinator_active", G_ECU, params::EcuSignals, motor2_drivetrain_coordinator_active, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("motor2_cruise_control_status", G_ECU, params::EcuSignals, motor2_cruise_control_status, F_UINT, 0, RX_NONE, W_ECU),
    SIG("motor2_speed_limiter_selected", G_ECU, params::EcuSignals, motor2_speed_limiter_selected, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("motor2_cruise_control_regulating", G_ECU, params::EcuSignals, motor2_cruise_control_regulating, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("motor2_sport_error", G_ECU, params::EcuSignals, motor2_sport_error, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("motor2_offroad_mode_active", G_ECU, params::EcuSignals, motor2_offroad_mode_active, F_BOOL, 0, RX_NONE, W_ECU),
    SIG("motor2_cruise_target_speed_kmh", G_ECU, params::EcuSignals, motor2_cruise_target_speed_kmh, F_NUM, 2, RX_NONE, W_ECU),
    SIG("motor2_idle_target_speed_rpm", G_ECU, params::EcuSignals, motor2_idle_target_speed_rpm, F_NUM, 2, RX_NONE, W_ECU),
    SIG("motor2_limited_torque_percent", G_ECU, params::EcuSignals, motor2_limited_torque_percent, F_NUM, 2, RX_NONE, W_ECU),
    SIG("motor2_min_torque_ignition_retard_percent", G_ECU, params::EcuSignals, motor2_min_torque_ignition_retard_percent, F_NUM, 2, RX_NONE, W_ECU),
    SIG("engine_speed_rpm", G_ECU, params::EcuSignals, engine_speed_rpm, F_NUM, 2, RX_NONE, OVR_ENGINE_SPEED),
    SIG("throttle_position_280", G_ECU, params::EcuSignals, throttle_position_280, F_NUM, 2, RX_NONE, W_ECU),
    SIG("pedal_position_1", G_ECU, params::EcuSignals, pedal_position_1, F_NUM, 2, RX_NONE, W_ECU),
    SIG("pedal_position_2", G_ECU, params::EcuSignals, pedal_position_2, F_NUM, 2, RX_NONE, W_ECU),
    SIG("inlet_air_temperature", G_ECU, params::EcuSignals, inlet_air_temperature, F_NUM, 2, RX_NONE, W_ECU),
    SIG("barometric_pressure_mbar", G_ECU, params::EcuSignals, barometric_pressure_mbar, F_NUM, 2, RX_NONE, W_ECU),
    // Fan control (VCU-REQ-008)
    SIG("coolant_fan_mode", G_FAN, params::FanSignals, fan_mode, F_UINT, 0, RX_NONE, 0),
    SIG("coolant_fan_level_manual", G_FAN, params::FanSignals, fan_level_manual, F_UINT, 0, RX_NONE, 0),
    SIG("coolant_fan_level_auto", G_FAN, params::FanSignals, fan_level_auto, F_UINT, 0, RX_NONE, 0),
    SIG("coolant_fan_outputs_enabled", G_FAN, params::FanSignals, fan_outputs_enabled, F_BOOL, 0, RX_NONE, 0),
    SIG("digital_pot_request_pct", G_DIGIPOT, params::DigipotSignals, digipot_pct, F_UINT, 0, RX_NONE, 0),
    // BMS / charger on CAN0 (VCU-REQ-018). Message layout open: these values stay stale.
    SIG("battery_soc_pct", G_BATTERY, params::BatterySignals, soc_pct, F_NUM, 1, RX_BATTERY, 0),
    SIG("battery_pack_voltage_v", G_BATTERY, params::BatterySignals, pack_voltage_v, F_NUM, 1, RX_BATTERY, 0),
    SIG("battery_pack_current_a", G_BATTERY, params::BatterySignals, pack_current_a, F_NUM, 1, RX_BATTERY, 0),
    SIG("battery_plug_connected", G_BATTERY, params::BatterySignals, plug_connected, F_BOOL, 0, RX_BATTERY, 0),
    SIG("battery_charging_active", G_BATTERY, params::BatterySignals, charging_active, F_BOOL, 0, RX_BATTERY, 0),
};
#pragma GCC diagnostic pop

const size_t kSignalCount = sizeof(kSignals) / sizeof(kSignals[0]);

const SignalDef *findSignal(const char *name) {
    for (size_t i = 0; i < kSignalCount; i++) {
        if (strcmp(kSignals[i].name, name) == 0) {
            return &kSignals[i];
        }
    }
    return nullptr;
}

void takeSnapshot(ParamsSnapshot &snap) {
    ParamsGuard guard;
#define SIGNAL_GROUP_COPY(id, Type, name) snap.name = params::name;
    SIGNAL_GROUPS(SIGNAL_GROUP_COPY)
#undef SIGNAL_GROUP_COPY
    rxSnapshot(snap.last_rx_ms);
}

const uint8_t *snapshotGroupBase(const ParamsSnapshot &snap, SigGroup group) {
    switch (group) {
#define SIGNAL_GROUP_CASE(id, Type, name) case id: return reinterpret_cast<const uint8_t *>(&snap.name);
        SIGNAL_GROUPS(SIGNAL_GROUP_CASE)
#undef SIGNAL_GROUP_CASE
        default: return nullptr;
    }
}

uint8_t *liveGroupBase(SigGroup group) {
    switch (group) {
#define SIGNAL_GROUP_CASE(id, Type, name) case id: return reinterpret_cast<uint8_t *>(&params::name);
        SIGNAL_GROUPS(SIGNAL_GROUP_CASE)
#undef SIGNAL_GROUP_CASE
        default: return nullptr;
    }
}

double readSignal(const SignalDef &def, const uint8_t *base) {
    const uint8_t *p = base + def.offset;
    switch (def.storage) {
        case S_BOOL: { bool v; memcpy(&v, p, sizeof v); return v ? 1.0 : 0.0; }
        case S_U8: { uint8_t v; memcpy(&v, p, sizeof v); return v; }
        case S_I8: { int8_t v; memcpy(&v, p, sizeof v); return v; }
        case S_U16: { uint16_t v; memcpy(&v, p, sizeof v); return v; }
        case S_I16: { int16_t v; memcpy(&v, p, sizeof v); return v; }
        case S_U32: { uint32_t v; memcpy(&v, p, sizeof v); return v; }
        case S_I32: { int32_t v; memcpy(&v, p, sizeof v); return v; }
        case S_F32: { float v; memcpy(&v, p, sizeof v); return v; }
    }
    return 0.0;
}

bool parseSignalBool(const String &text, bool &out) {
    if (text == "1" || text.equalsIgnoreCase("true") || text.equalsIgnoreCase("on")) {
        out = true;
        return true;
    }
    if (text == "0" || text.equalsIgnoreCase("false") || text.equalsIgnoreCase("off")) {
        out = false;
        return true;
    }
    return false;
}

bool parseSignalNumber(const String &text, double &out) {
    if (text.length() == 0 || text.length() > 24) {
        return false;
    }
    char *end = nullptr;
    out = strtod(text.c_str(), &end);
    return end != nullptr && *end == '\0' && isfinite(out);
}

template <typename T> static bool storeInt(uint8_t *p, double v, double lo, double hi) {
    if (v < lo || v > hi) {
        return false;
    }
    const T value = (T)lround(v);
    memcpy(p, &value, sizeof value);
    return true;
}

bool writeSignal(const SignalDef &def, const String &text) {
    uint8_t *base = liveGroupBase(def.group);
    if (base == nullptr) {
        return false;
    }
    uint8_t *p = base + def.offset;
    if (def.storage == S_BOOL) {
        bool value;
        if (!parseSignalBool(text, value)) {
            return false;
        }
        memcpy(p, &value, sizeof value);
        return true;
    }
    double v;
    if (!parseSignalNumber(text, v)) {
        return false;
    }
    switch (def.storage) {
        case S_U8: return storeInt<uint8_t>(p, v, 0, 255);
        case S_I8: return storeInt<int8_t>(p, v, -128, 127);
        case S_U16: return storeInt<uint16_t>(p, v, 0, 65535);
        case S_I16: return storeInt<int16_t>(p, v, -32768, 32767);
        case S_U32: return storeInt<uint32_t>(p, v, 0, 4294967295.0);
        case S_I32: return storeInt<int32_t>(p, v, -2147483648.0, 2147483647.0);
        case S_F32: {
            if (fabs(v) > 1.0e9) {
                return false;
            }
            const float value = (float)v;
            memcpy(p, &value, sizeof value);
            return true;
        }
        default: return false;
    }
}

const char *sigStorageName(SigStorage storage) {
    switch (storage) {
        case S_BOOL: return "bool";
        case S_U8: return "u8";
        case S_I8: return "i8";
        case S_U16: return "u16";
        case S_I16: return "i16";
        case S_U32: return "u32";
        case S_I32: return "i32";
        case S_F32: return "f32";
    }
    return "?";
}
