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

<h2 onclick="document.getElementById('giantSection').style.display=document.getElementById('giantSection').style.display==='none'?'block':'none'" style="cursor:pointer">&#x1f6b2; Giant E-Bike <span style="font-size:.7em;color:#8b949e">&#x25BC;</span></h2>
<div id="giantSection">
  <div class="status-bar" style="margin-bottom:12px">
    <span><span class="dot off" id="dotGev"></span>GEV: <span id="gevStatus">not initialized</span></span>
  </div>
  <div id="gevDashboard" style="display:none">
    <div style="display:grid;grid-template-columns:1fr 1fr;gap:8px;margin-bottom:12px">
      <div class="device-list" style="text-align:center;padding:16px;margin:0">
        <div style="font-size:2.2em;color:#58a6ff;font-weight:700" id="gevSpeed">0.0</div>
        <div style="color:#8b949e;font-size:.85em">km/h</div>
      </div>
      <div class="device-list" style="text-align:center;padding:16px;margin:0">
        <div style="font-size:2.2em;color:#3fb950;font-weight:700" id="gevBatt">0<span style="font-size:.5em">%</span></div>
        <div style="background:#21262d;border-radius:4px;height:8px;margin-top:4px"><div id="gevBattBar" style="background:#3fb950;height:100%;border-radius:4px;width:0%;transition:width .3s"></div></div>
      </div>
    </div>
    <div style="display:grid;grid-template-columns:repeat(3,1fr);gap:8px;margin-bottom:12px">
      <div class="device-list" style="text-align:center;margin:0;padding:10px">
        <div style="color:#d2a8ff;font-size:1.3em;font-weight:600" id="gevWatt">0</div>
        <div style="color:#8b949e;font-size:.75em">Watts</div>
      </div>
      <div class="device-list" style="text-align:center;margin:0;padding:10px">
        <div style="color:#f0883e;font-size:1.3em;font-weight:600" id="gevCadence">0</div>
        <div style="color:#8b949e;font-size:.75em">RPM</div>
      </div>
      <div class="device-list" style="text-align:center;margin:0;padding:10px">
        <div style="color:#79c0ff;font-size:1.3em;font-weight:600" id="gevTorque">0</div>
        <div style="color:#8b949e;font-size:.75em">Nm</div>
      </div>
    </div>
    <div style="display:grid;grid-template-columns:repeat(3,1fr);gap:8px;margin-bottom:12px">
      <div class="device-list" style="text-align:center;margin:0;padding:10px">
        <div style="color:#c9d1d9;font-size:1.1em" id="gevDist">0.0</div>
        <div style="color:#8b949e;font-size:.75em">km</div>
      </div>
      <div class="device-list" style="text-align:center;margin:0;padding:10px">
        <div style="color:#c9d1d9;font-size:1.1em" id="gevTime">00:00</div>
        <div style="color:#8b949e;font-size:.75em">Ride Time</div>
      </div>
      <div class="device-list" style="text-align:center;margin:0;padding:10px">
        <div style="color:#c9d1d9;font-size:1.1em" id="gevRange">—</div>
        <div style="color:#8b949e;font-size:.75em">Range km</div>
      </div>
    </div>
  </div>
  <div class="controls" style="flex-wrap:wrap">
    <button class="btn primary" onclick="gevCmd('connect')">&#x1f50c; Connect GEV</button>
    <button class="btn" onclick="gevCmd('riding')">&#x1f4ca; Read Data</button>
    <button class="btn" onclick="gevCmd('light')">&#x1f4a1; Light</button>
    <button class="btn" onclick="gevCmd('assist_up')">&#x2b06; Assist</button>
    <button class="btn" onclick="gevCmd('assist_down')">&#x2b07; Assist</button>
    <button class="btn" onclick="gevCmd('power')">&#x26a1; Power</button>
    <button class="btn" onclick="gevCmd('range')">&#x1f4cf; Range</button>
    <button class="btn" onclick="gevCmd('factory')">&#x1f3ed; Factory</button>
    <button class="btn danger" onclick="gevCmd('disconnect')">Disconnect GEV</button>
  </div>
  <div id="gevFactory" class="device-list" style="display:none;margin-top:8px">
    <div style="font-weight:600;color:#d2a8ff;margin-bottom:6px">Factory Info</div>
    <div style="font-size:.85em;color:#8b949e">Frame: <span id="gevFrame" style="color:#c9d1d9">—</span></div>
    <div style="font-size:.85em;color:#8b949e">RC Type: <span id="gevRcType" style="color:#c9d1d9">—</span></div>
    <div style="font-size:.85em;color:#8b949e">RC HW: <span id="gevRcHw" style="color:#c9d1d9">—</span></div>
    <div style="font-size:.85em;color:#8b949e">DU FW: <span id="gevDuFw" style="color:#c9d1d9">—</span></div>
    <div style="font-size:.85em;color:#8b949e">EP FW: <span id="gevEpFw" style="color:#c9d1d9">—</span></div>
    <div style="font-size:.85em;color:#8b949e">ODO: <span id="gevOdo" style="color:#c9d1d9">—</span> km</div>
  </div>
</div>

<h2 onclick="document.getElementById('loggerSection').style.display=document.getElementById('loggerSection').style.display==='none'?'block':'none'" style="cursor:pointer">&#x1f4be; Ride Logger <span style="font-size:.7em;color:#8b949e">&#x25BC;</span></h2>
<div id="loggerSection">
  <div class="status-bar" style="margin-bottom:12px">
    <span><span class="dot off" id="dotRec"></span><span id="recStatus">Idle</span></span>
    <span>Storage: <span id="storageUsed">—</span></span>
  </div>
  <div class="controls">
    <button class="btn primary" id="btnRecStart" onclick="rideLogStart()">&#x23FA; Start Recording</button>
    <button class="btn danger" id="btnRecStop" onclick="rideLogStop()" disabled>&#x23F9; Stop</button>
    <button class="btn" onclick="rideLogRefresh()">&#x1f504; Refresh</button>
  </div>
  <div id="rideList" class="device-list" style="margin-top:8px">
    <span class="log-info">No rides yet.</span>
  </div>
</div>

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
      else if(msg.type==='giant_cmd_result'){log('GEV '+msg.cmd+': '+(msg.success?'OK':'FAIL'),msg.success?'info':'err');setTimeout(gevPoll,500);if(!gevPollTimer)gevPollTimer=setInterval(gevPoll,2000)}
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

function gevCmd(cmd){
  fetch('/api/giant/command',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({cmd:cmd})})
    .then(r=>r.json()).then(d=>log('GEV cmd: '+cmd+' → '+d.status,'info'))
    .catch(e=>log('GEV error: '+e,'err'));
}

let gevPollTimer=null;
function gevUpdateDashboard(d){
  if(!d.initialized){
    document.getElementById('dotGev').className='dot off';
    document.getElementById('gevStatus').textContent='not initialized';
    document.getElementById('gevDashboard').style.display='none';
    if(gevPollTimer){clearInterval(gevPollTimer);gevPollTimer=null}
    return;
  }
  document.getElementById('dotGev').className='dot on';
  document.getElementById('gevStatus').textContent='connected';
  document.getElementById('gevDashboard').style.display='block';
  if(d.ride){
    document.getElementById('gevSpeed').textContent=d.ride.speed.toFixed(1);
    document.getElementById('gevBatt').innerHTML=d.ride.rsoc+'<span style="font-size:.5em">%</span>';
    document.getElementById('gevBattBar').style.width=d.ride.rsoc+'%';
    document.getElementById('gevBattBar').style.background=d.ride.rsoc>20?'#3fb950':'#f85149';
    document.getElementById('gevWatt').textContent=d.ride.watt.toFixed(0);
    document.getElementById('gevCadence').textContent=d.ride.crank.toFixed(0);
    document.getElementById('gevTorque').textContent=d.ride.torque.toFixed(1);
    document.getElementById('gevDist').textContent=d.ride.distance.toFixed(1);
    const m=Math.floor(d.ride.time/60),s=d.ride.time%60;
    document.getElementById('gevTime').textContent=String(m).padStart(2,'0')+':'+String(s).padStart(2,'0');
    document.getElementById('gevRange').textContent=d.ride.range>0?d.ride.range:'—';
  }
  if(d.factory&&d.factory.frameNumber){
    document.getElementById('gevFactory').style.display='block';
    document.getElementById('gevFrame').textContent=d.factory.frameNumber||'—';
    document.getElementById('gevRcType').textContent=d.factory.rcType||'—';
    document.getElementById('gevRcHw').textContent=d.factory.rcHwVersion||'—';
  }
  if(d.bike){
    document.getElementById('gevDuFw').textContent=d.bike.duFw||'—';
    document.getElementById('gevEpFw').textContent=d.bike.epFw||'—';
    document.getElementById('gevOdo').textContent=d.bike.odo>0?d.bike.odo:'—';
  }
}

async function gevPoll(){
  const d=await api('giant/status');
  if(d) gevUpdateDashboard(d);
}

initWS();
updateStatus();
gevPoll();
rideLogRefresh();
setInterval(updateStatus,5000);

async function rideLogRefresh(){
  const d=await api('rides');
  if(!d)return;
  document.getElementById('storageUsed').textContent=(d.usedKB||0)+'/'+(d.totalKB||0)+' KB';
  const logging=d.logging;
  document.getElementById('dotRec').className='dot '+(logging?'on':'off');
  document.getElementById('recStatus').textContent=logging?'Recording: '+d.currentFile:'Idle';
  document.getElementById('btnRecStart').disabled=logging;
  document.getElementById('btnRecStop').disabled=!logging;
  const el=document.getElementById('rideList');
  if(!d.rides||d.rides.length===0){el.innerHTML='<span class="log-info">No rides yet.</span>';return}
  el.innerHTML='';
  d.rides.forEach(r=>{
    const div=document.createElement('div');
    div.className='device';
    const sizeKB=(r.size/1024).toFixed(1);
    div.innerHTML=`<div class="device-info"><div class="device-name">${r.file}</div><div class="device-addr">${r.samples} samples &middot; ${sizeKB} KB</div></div><div style="display:flex;gap:6px"><a class="btn" href="/api/rides/download?file=${encodeURIComponent(r.file)}" download="${r.file}">&#x1f4e5; Download</a><button class="btn danger" onclick="rideLogDelete('${r.file}')">&#x1f5d1;</button></div>`;
    el.appendChild(div);
  });
}

async function rideLogStart(){
  await api('rides/start',{method:'POST'});
  log('Recording started','info');
  rideLogRefresh();
}

async function rideLogStop(){
  await api('rides/stop',{method:'POST'});
  log('Recording stopped','info');
  rideLogRefresh();
}

async function rideLogDelete(file){
  if(!confirm('Delete '+file+'?'))return;
  await fetch('/api/rides/delete?file='+encodeURIComponent(file),{method:'DELETE'});
  rideLogRefresh();
}
</script>
</body>
</html>
)rawhtml";
