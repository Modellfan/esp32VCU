// Output test page at /io: coolant fan stages, fuel gauge potentiometer and the heater request
// to the Zombie-Slave (0x43A on CAN0). Uses /api/set and /api/live. Bench use.
#ifndef IO_PAGE_H
#define IO_PAGE_H

#include <pgmspace.h>

static const char kIoPageHtml[] PROGMEM = R"HTML(<!doctype html>
<html lang="de"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>VCU Ausgänge</title>
<style>
:root{--bg:#fff;--fg:#111;--mut:#666;--line:#ddd;--ok:#16794a;--bad:#c62828;--acc:#1f5fbf}
@media(prefers-color-scheme:dark){:root{--bg:#121212;--fg:#eee;--mut:#999;--line:#333;--ok:#4caf7d;--bad:#ef5350;--acc:#6ea0ff}}
body{margin:0;padding:16px;background:var(--bg);color:var(--fg);font:14px/1.4 system-ui,sans-serif;max-width:560px}
h1{font-size:18px;margin:0 0 4px}h2{font-size:14px;margin:22px 0 8px;color:var(--mut);font-weight:600}
#st{color:var(--mut);font-size:12px}
.row{display:flex;gap:8px;flex-wrap:wrap;align-items:center}
button{font:inherit;padding:8px 14px;border:1px solid var(--line);border-radius:6px;background:transparent;color:var(--fg);cursor:pointer;min-width:56px}
button.on{background:var(--acc);border-color:var(--acc);color:#fff}
input[type=range]{flex:1;min-width:160px}
.kv{display:grid;grid-template-columns:1fr auto;gap:2px 12px;margin-top:8px;font-variant-numeric:tabular-nums}
.kv span:nth-child(odd){color:var(--mut)}
.ok{color:var(--ok)}.bad{color:var(--bad);font-weight:600}.warn{color:var(--bad);font-size:12px;margin-top:6px}
</style></head><body>
<h1>Ausgänge testen</h1><div id="st">lade …</div>

<h2>Kühlmittellüfter</h2>
<div class="row"><button data-fan="0">Aus</button><button data-fan="1">Stufe 1</button><button data-fan="2">Stufe 2</button><button data-fan="auto">Auto</button></div>
<div class="kv" id="fan"></div><div class="warn" id="fanwarn"></div>

<h2>Tankanzeige (Poti)</h2>
<div class="row"><input id="pot" type="range" min="0" max="100" step="1"><b id="potv">–</b><button id="potset">Setzen</button></div>
<div class="kv" id="potkv"></div><div class="warn" id="potwarn"></div>

<h2>Heizung (Zombie-Slave, CAN0 0x43A)</h2>
<div class="row"><button data-heat="0">Aus</button><button data-heat="1">An</button></div>
<div class="kv" id="heat"></div>

<script>
const $=id=>document.getElementById(id);
const yn=(v,okWhenTrue)=>v?'<b class="'+(okWhenTrue?'ok':'bad')+'">ja</b>':'nein';
let potTouched=false;
function kv(el,rows){el.innerHTML=rows.map(r=>"<span>"+r[0]+"</span><span>"+r[1]+"</span>").join("")}
function render(d){
 const stale=new Set(d.stale||[]);
 const auto=d.coolant_fan_mode===1;
 document.querySelectorAll("[data-fan]").forEach(b=>{const v=b.dataset.fan;b.classList.toggle("on",v==="auto"?auto:(!auto&&+v===d.coolant_fan_level_manual))});
 kv($("fan"),[["Modus",auto?"Auto":"Manuell"],["Stufe angewendet",d.coolant_fan_level],["Relais Stufe 1",yn(d.coolant_fan_low_output,true)],["Relais Stufe 2",yn(d.coolant_fan_high_output,true)]]);
 $("fanwarn").textContent=d.coolant_fan_outputs_enabled?"":"Ausgänge deaktiviert: Pin für Stufe 1 fehlt in board.h – die Relais schalten nicht.";
 if(!potTouched){$("pot").value=d.digital_pot_request_pct;$("potv").textContent=d.digital_pot_request_pct+" %"}
 kv($("potkv"),[["Sollwert",d.digital_pot_request_pct+" %"],["angewendet",d.digital_pot_pct+" %"],["Schleifer",d.digital_pot_wiper]]);
 $("potwarn").textContent=d.digital_pot_enabled?"":"Poti deaktiviert: INC/UD-Pins fehlen in board.h.";
 document.querySelectorAll("[data-heat]").forEach(b=>b.classList.toggle("on",+b.dataset.heat===(d.heater_request?1:0)));
 const s=stale.has("heater_active")?' <span class="bad">(kein 0x439)</span>':'';
 kv($("heat"),[["Anforderung VCU",d.heater_request?"An":"Aus"],["Zombie sieht Anforderung",yn(d.heater_can_request,true)+s],["Heizung aktiv",yn(d.heater_active,true)],["Schütz geschlossen",yn(d.heater_contactor_closed,true)],["Thermoschalter ok",yn(d.heater_thermal_ok,true)],["Fehler",d.heater_fault?'<b class="bad">ja</b>':"nein"]]);
 $("st").textContent="Laufzeit "+(d.uptime_ms/1000).toFixed(0)+" s · "+new Date().toLocaleTimeString();
}
async function call(url){
 try{const r=await fetch(url,{cache:"no-store"});render(await r.json())}
 catch(e){$("st").textContent="keine Verbindung: "+e}
}
document.querySelectorAll("[data-fan]").forEach(b=>b.onclick=()=>call(b.dataset.fan==="auto"?"/api/set?coolant_fan_mode=auto":"/api/set?coolant_fan_level="+b.dataset.fan));
document.querySelectorAll("[data-heat]").forEach(b=>b.onclick=()=>call("/api/set?heater_request="+b.dataset.heat));
$("pot").oninput=()=>{potTouched=true;$("potv").textContent=$("pot").value+" %"};
$("potset").onclick=()=>{potTouched=false;call("/api/set?digital_pot_pct="+$("pot").value)};
async function tick(){await call("/api/live");setTimeout(tick,1000)}
tick();
</script></body></html>)HTML";

#endif
