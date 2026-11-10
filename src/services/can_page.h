// CAN test page at /can: CAN manager settings and status from /api/live (params::can),
// refreshed every second. Embedded in the firmware so it works without the SPIFFS image.
#ifndef CAN_PAGE_H
#define CAN_PAGE_H

#include <pgmspace.h>

static const char kCanPageHtml[] PROGMEM = R"HTML(<!doctype html>
<html lang="de"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>VCU CAN</title>
<style>
:root{--bg:#fff;--fg:#111;--mut:#666;--line:#ddd;--ok:#16794a;--bad:#c62828}
@media(prefers-color-scheme:dark){:root{--bg:#121212;--fg:#eee;--mut:#999;--line:#333;--ok:#4caf7d;--bad:#ef5350}}
body{margin:0;padding:16px;background:var(--bg);color:var(--fg);font:14px/1.4 system-ui,sans-serif}
h1{font-size:18px;margin:0 0 4px}h2{font-size:14px;margin:20px 0 6px;color:var(--mut);font-weight:600}
#st{color:var(--mut);font-size:12px}
table{border-collapse:collapse;width:100%;max-width:720px}
td,th{padding:4px 8px;border-bottom:1px solid var(--line);text-align:right;font-variant-numeric:tabular-nums}
td:first-child,th:first-child{text-align:left;color:var(--mut);font-weight:400}
.ok{color:var(--ok)}.bad{color:var(--bad);font-weight:600}
.wrap{overflow-x:auto}
</style></head><body>
<h1>CAN</h1><div id="st">lade …</div>
<h2>Busse</h2><div class="wrap"><table id="bus"></table></div>
<h2>ECU-Task</h2><div class="wrap"><table id="ecu"></table></div>
<h2>Einstellungen</h2><div class="wrap"><table id="cfg"></table></div>
<script>
const B=[0,1,2,3],ROWS=[["up","Status"],["error","Init"],["rx_frames","RX Frames"],["rate","RX/s"],["tec","TEC"],["rec","REC"],["eflg","EFLG/Status"],["bus_off","Bus-off"]];
let last=null;
const hex=v=>"0x"+Number(v).toString(16).toUpperCase().padStart(2,"0");
function cell(k,v,b,d){
 if(k==="up")return v?'<span class="ok">up</span>':'<span class="bad">DOWN</span>';
 if(k==="bus_off")return v?'<span class="bad">JA</span>':"nein";
 if(k==="error")return v?'<span class="bad">'+hex(v)+'</span>':"0";
 if(k==="eflg")return hex(v);
 if(k==="tec"||k==="rec")return v>=128?'<span class="bad">'+v+'</span>':v;
 return v;
}
function render(d){
 const dt=last?(d.uptime_ms-last.uptime_ms)/1000:0;
 let h="<tr><th></th>"+B.map(b=>"<th>CAN"+b+"</th>").join("")+"</tr>";
 for(const[k,l]of ROWS){
  h+="<tr><td>"+l+"</td>"+B.map(b=>{
   if(k==="rate"){const n=d["can"+b+"_rx_frames"],o=last&&last["can"+b+"_rx_frames"];return "<td>"+(dt>0&&o!=null?Math.round((n-o)/dt):"–")+"</td>"}
   return "<td>"+cell(k,d["can"+b+"_"+k])+"</td>"}).join("")+"</tr>";
 }
 document.getElementById("bus").innerHTML=h;
 const row=(l,v)=>"<tr><td>"+l+"</td><td>"+v+"</td></tr>";
 document.getElementById("ecu").innerHTML=
  row("Zyklen",d.ecu_cycles)+row("Überläufe",d.ecu_overruns)+row("max. Zykluszeit",d.ecu_max_cycle_us+" µs")+
  row("TX fehlgeschlagen",d.ecu_tx_failed)+row("CAN0 RX-Puffer Peak",d.can0_rx_peak+" / "+d.can_can0_rx_buffer)+
  row("CAN0 RX-Überlauf",d.can0_rx_overflow?'<span class="bad">JA</span>':"nein")+row("GVRET verworfen",d.gvret_dropped);
 const cfg=Object.keys(d).filter(k=>k.startsWith("can_")).map(k=>row(k.slice(4),d[k]));
 document.getElementById("cfg").innerHTML=cfg.join("");
 document.getElementById("st").textContent="Laufzeit "+(d.uptime_ms/1000).toFixed(0)+" s · Heap "+d.free_heap+" B · "+new Date().toLocaleTimeString();
 last=d;
}
async function tick(){
 try{const r=await fetch("/api/live",{cache:"no-store"});render(await r.json())}
 catch(e){document.getElementById("st").textContent="keine Verbindung: "+e}
 setTimeout(tick,1000);
}
tick();
</script></body></html>)HTML";

#endif
