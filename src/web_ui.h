#pragma once

const char WEB_UI_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Giant BLE Explorer</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,system-ui,sans-serif;background:#0d1117;color:#c9d1d9;padding:16px;max-width:800px;margin:0 auto}
h1{color:#58a6ff;margin-bottom:8px;font-size:1.4em}
h2{color:#8b949e;font-size:1em;margin:16px 0 8px}
.status-bar{display:flex;gap:12px;flex-wrap:wrap;padding:8px 12px;background:#161b22;border:1px solid #30363d;border-radius:8px;margin-bottom:16px;font-size:.85em}
.status-bar .dot{width:8px;height:8px;border-radius:50%;display:inline-block;margin-right:4px}
.dot.on{background:#3fb950}.dot.off{background:#f85149}
.btn{padding:8px 16px;border:1px solid #30363d;border-radius:6px;background:#21262d;color:#c9d1d9;cursor:pointer;font-size:.85em;transition:background .15s}
.btn:hover{background:#30363d}.btn:active{background:#484f58}
.btn.primary{background:#238636;border-color:#2ea043;color:#fff}
.btn.primary:hover{background:#2ea043}
.btn.danger{background:#da3633;border-color:#f85149;color:#fff}
.btn.danger:hover{background:#f85149}
.btn:disabled{opacity:.4;cursor:not-allowed}
.controls{display:flex;gap:8px;flex-wrap:wrap;margin-bottom:16px}
.device-list,.service-list,.log{background:#161b22;border:1px solid #30363d;border-radius:8px;padding:12px;margin-bottom:16px}
.device{display:flex;justify-content:space-between;align-items:center;padding:8px;border-bottom:1px solid #21262d;gap:8px}
.device:last-child{border-bottom:none}
.device-info{flex:1;min-width:0}
.device-name{color:#58a6ff;font-weight:600;font-size:.9em}
.device-addr{color:#8b949e;font-size:.8em;font-family:monospace}
.device-rssi{color:#8b949e;font-size:.8em;white-space:nowrap}
.svc{margin-bottom:12px}
.svc-uuid{color:#d2a8ff;font-family:monospace;font-size:.85em;font-weight:600}
.chr{padding:6px 8px;margin:4px 0 4px 12px;background:#0d1117;border-radius:4px;font-size:.82em}
.chr-uuid{color:#79c0ff;font-family:monospace}
.chr-props{display:flex;gap:4px;flex-wrap:wrap;margin-top:2px}
.chr-props span{background:#30363d;padding:1px 6px;border-radius:3px;font-size:.75em;color:#8b949e}
.chr-val{color:#3fb950;font-family:monospace;margin-top:2px;word-break:break-all}
.log{max-height:300px;overflow-y:auto;font-family:monospace;font-size:.8em;white-space:pre-wrap;word-break:break-all}
.log-entry{padding:2px 0;border-bottom:1px solid #21262d}
.log-ntf{color:#f0883e}.log-info{color:#8b949e}.log-err{color:#f85149}
.spinner{display:inline-block;width:14px;height:14px;border:2px solid #30363d;border-top-color:#58a6ff;border-radius:50%;animation:spin .8s linear infinite;margin-right:6px;vertical-align:middle}
@keyframes spin{to{transform:rotate(360deg)}}
</style>
</head>
<body>
<h1>&#x1f6b2; Giant BLE Explorer</h1>
<div class="status-bar" id="statusBar">
  <span><span class="dot off" id="dotWifi"></span>WiFi: <span id="wifiIp">—</span></span>
  <span><span class="dot off" id="dotBle"></span>BLE: <span id="bleStatus">disconnected</span></span>
  <span>Heap: <span id="heapFree">—</span></span>
</div>

<div class="controls">
  <button class="btn primary" id="btnScan" onclick="startScan()">&#x1f50d; Scan</button>
  <button class="btn danger" id="btnDisconnect" onclick="doDisconnect()" disabled>Disconnect</button>
  <button class="btn" id="btnSubscribe" onclick="doSubscribe()" disabled>&#x1f514; Subscribe All</button>
  <button class="btn" id="btnRefresh" onclick="loadServices()" disabled>&#x1f504; Refresh</button>
</div>

<h2>Discovered Devices</h2>
<div class="device-list" id="deviceList"><span class="log-info">No devices yet. Hit Scan.</span></div>

<h2>Services &amp; Characteristics</h2>
<div class="service-list" id="serviceList"><span class="log-info">Connect to a device first.</span></div>

<h2>Live Log</h2>
<div class="log" id="log"></div>

<script>
let ws;
function initWS(){
  ws=new WebSocket(`ws://${location.host}/ws`);
  ws.onopen=()=>log('WebSocket connected','info');
  ws.onclose=()=>{log('WebSocket disconnected, reconnecting...','err');setTimeout(initWS,2000)};
  ws.onmessage=(e)=>{
    try{
      const msg=JSON.parse(e.data);
      if(msg.event==='scan_result') addDevice(msg.data);
      else if(msg.event==='scan_end'){log('Scan complete','info');btnScan.disabled=false;btnScan.innerHTML='&#x1f50d; Scan'}
      else if(msg.event==='notification') logNotification(msg.data);
      else if(msg.event==='connected'){log('Connected to '+JSON.stringify(msg.data.address),'info');updateStatus();loadServices()}
      else if(msg.event==='disconnected'){log('Disconnected','info');updateStatus();document.getElementById('serviceList').innerHTML='<span class="log-info">Connect to a device first.</span>'}
      else if(msg.event==='subscribed') log('Subscribed to notifications','info');
      else if(msg.event==='status') log(msg.data.message,'info');
      else if(msg.event==='error') log(msg.data.message,'err');
      else log(JSON.stringify(msg),'info');
    }catch(err){log(e.data,'info')}
  };
}

function log(text,cls='info'){
  const el=document.getElementById('log');
  const d=document.createElement('div');
  d.className='log-entry log-'+cls;
  d.textContent=new Date().toLocaleTimeString()+' '+text;
  el.appendChild(d);
  el.scrollTop=el.scrollHeight;
}

function logNotification(d){
  log(`[${d.type||'NTF'}] ${d.uuid}: ${d.hex}`,'ntf');
}

async function api(path,opts={}){
  try{
    const r=await fetch('/api/'+path,opts);
    return await r.json();
  }catch(e){log('API error: '+e,'err');return null}
}

async function updateStatus(){
  const s=await api('status');
  if(!s)return;
  document.getElementById('wifiIp').textContent=s.wifi_ip;
  document.getElementById('dotWifi').className='dot '+(s.wifi_ip?'on':'off');
  document.getElementById('bleStatus').textContent=s.ble_connected?'connected':'disconnected';
  document.getElementById('dotBle').className='dot '+(s.ble_connected?'on':'off');
  document.getElementById('heapFree').textContent=Math.round(s.heap_free/1024)+'KB';
  document.getElementById('btnDisconnect').disabled=!s.ble_connected;
  document.getElementById('btnSubscribe').disabled=!s.ble_connected;
  document.getElementById('btnRefresh').disabled=!s.ble_connected;
  document.getElementById('btnScan').disabled=s.ble_connected||s.ble_scanning;
}

async function startScan(){
  document.getElementById('deviceList').innerHTML='<span class="spinner"></span> Scanning...';
  btnScan.disabled=true;
  btnScan.innerHTML='<span class="spinner"></span> Scanning';
  await api('scan?duration=10');
}

function addDevice(d){
  const list=document.getElementById('deviceList');
  if(list.querySelector('.spinner')||list.querySelector('.log-info'))list.innerHTML='';
  // Deduplicate
  const existing=list.querySelector(`[data-addr="${d.address}"]`);
  if(existing)existing.remove();

  const div=document.createElement('div');
  div.className='device';
  div.dataset.addr=d.address;
  div.innerHTML=`<div class="device-info">
    <div class="device-name">${d.name||'(unknown)'}</div>
    <div class="device-addr">${d.address}</div>
  </div>
  <div class="device-rssi">${d.rssi} dBm</div>
  <button class="btn" onclick="doConnect('${d.address}')">Connect</button>`;
  list.appendChild(div);
}

async function doConnect(addr){
  log('Connecting to '+addr+'...','info');
  const r=await api('connect',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({address:addr})});
  if(r&&r.status==='connected'){log('Connected!','info');updateStatus();loadServices()}
  else log('Connect failed: '+(r?r.error:'unknown'),'err');
}

async function doDisconnect(){
  await api('disconnect',{method:'POST'});
  log('Disconnected','info');
  document.getElementById('serviceList').innerHTML='<span class="log-info">Connect to a device first.</span>';
  updateStatus();
}

async function doSubscribe(){
  const r=await api('subscribe',{method:'POST'});
  if(r&&r.status==='subscribed')log('Subscribed to notifications','info');
  else log('Subscribe failed','err');
}

async function loadServices(){
  const svcs=await api('services');
  const el=document.getElementById('serviceList');
  if(!svcs||!svcs.length){el.innerHTML='<span class="log-info">No services found.</span>';return}
  el.innerHTML='';
  svcs.forEach(svc=>{
    let html=`<div class="svc"><div class="svc-uuid">${svc.uuid}</div>`;
    (svc.characteristics||[]).forEach(chr=>{
      html+=`<div class="chr"><div class="chr-uuid">${chr.uuid}</div>`;
      html+=`<div class="chr-props">${(chr.properties||[]).map(p=>'<span>'+p+'</span>').join('')}</div>`;
      if(chr.value_hex)html+=`<div class="chr-val">HEX: ${chr.value_hex}</div>`;
      if(chr.value_str)html+=`<div class="chr-val">STR: "${chr.value_str}"</div>`;
      html+=`</div>`;
    });
    html+=`</div>`;
    el.innerHTML+=html;
  });
}

initWS();
updateStatus();
setInterval(updateStatus,5000);
</script>
</body>
</html>
)rawhtml";
