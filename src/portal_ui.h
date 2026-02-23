#pragma once

// Minimal captive portal page for WiFi configuration
const char PORTAL_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Giant ESP32 Setup</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:system-ui,sans-serif;background:#1a1a2e;color:#e0e0e0;display:flex;justify-content:center;align-items:center;min-height:100vh}
.card{background:#16213e;border-radius:12px;padding:2rem;width:90%;max-width:400px;box-shadow:0 4px 24px rgba(0,0,0,.4)}
h1{font-size:1.3rem;margin-bottom:.5rem;color:#4fc3f7}
p{font-size:.85rem;color:#999;margin-bottom:1.5rem}
label{display:block;font-size:.85rem;margin-bottom:.3rem;color:#aaa}
input,select{width:100%;padding:.6rem;margin-bottom:1rem;border:1px solid #333;border-radius:6px;background:#0f3460;color:#e0e0e0;font-size:.95rem}
button{width:100%;padding:.7rem;border:none;border-radius:6px;background:#4fc3f7;color:#1a1a2e;font-weight:bold;font-size:1rem;cursor:pointer}
button:hover{background:#29b6f6}
.status{margin-top:1rem;padding:.6rem;border-radius:6px;font-size:.85rem;display:none}
.ok{background:#1b5e20;display:block}
.err{background:#b71c1c;display:block}
#nets{margin-bottom:1rem}
</style></head><body>
<div class="card">
<h1>🚲 Giant ESP32 Setup</h1>
<p>Connect this device to your WiFi network</p>
<div id="nets"><button type="button" onclick="scan()">Scan for Networks</button></div>
<form onsubmit="return save(event)">
<label>SSID</label><input id="ssid" required>
<label>Password</label><input id="pass" type="password">
<button type="submit">Save & Connect</button>
</form>
<div id="st" class="status"></div>
</div>
<script>
function scan(){
  document.getElementById('nets').innerHTML='Scanning...';
  fetch('/api/wifi/scan').then(r=>r.json()).then(d=>{
    if(!d.networks||!d.networks.length){document.getElementById('nets').innerHTML='<p>No networks found. <button type="button" onclick="scan()">Retry</button></p>';return}
    let h='<label>Available Networks</label><select onchange="document.getElementById(\'ssid\').value=this.value">';
    h+='<option value="">Select a network...</option>';
    d.networks.forEach(n=>{h+='<option value="'+n.ssid+'">'+n.ssid+' ('+n.rssi+' dBm)</option>'});
    h+='</select>';
    document.getElementById('nets').innerHTML=h;
  }).catch(()=>{document.getElementById('nets').innerHTML='<p>Scan failed. <button type="button" onclick="scan()">Retry</button></p>'});
}
function save(e){
  e.preventDefault();
  let st=document.getElementById('st');
  st.className='status';st.style.display='none';
  fetch('/api/wifi/config',{method:'POST',headers:{'Content-Type':'application/json'},
    body:JSON.stringify({ssid:document.getElementById('ssid').value,password:document.getElementById('pass').value})
  }).then(r=>r.json()).then(d=>{
    st.textContent=d.status==='saved'?'Credentials saved! Device will reboot in 3 seconds...':'Error: '+(d.error||'Unknown');
    st.className='status '+(d.status==='saved'?'ok':'err');
  }).catch(()=>{st.textContent='Failed to save';st.className='status err'});
  return false;
}
</script></body></html>
)rawliteral";
