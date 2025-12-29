#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Smart Hub</title>
  <style>
     body { font-family: 'DS-Digital', sans-serif; text-align: center; background-color: #222; color: white; margin: 0; padding: 20px;  }
     h2 { color: #00d2ff; }
     .card { background-color: #333; max-width: 400px; margin: 20px auto; padding: 30px; border-radius: 15px; box-shadow: 0 8px 16px rgba(0,0,0,0.3); }
     .data-box { font-size: 2rem; font-weight: bold; color: #ffeb3b; margin: 20px 0; }
     .status-text { font-size: 1.2rem; color: #ccc; }

     .btn { border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: block; font-size: 16px; margin: 10px auto; width: 80%; border-radius: 50px; cursor: pointer; transition: 0.3s; }
     .btn-auto { background-color: #2196F3; }
     .btn-on { background-color: #4CAF50; }
     .btn-off { background-color: #f44336; }
     .btn:hover { opacity: 0.8; transform: scale(1.02); }
     .active { border: 3px solid white; box-shadow: 0 0 10px white; }
  </style>
</head>
<body>
  <h2>===Smart Home Hub===</h2>

  <div class="card">
     <div class="status-text">Luminozitate Senzor</div>
     <div id="ldrValue" class="data-box">---</div>

     <div class="status-text">Mod Operare: <span id="stateText" style="color:#00d2ff">...</span></div>
     <hr style="border-color:#555; margin:  20px 0;">

     <button id="btnAuto" class="btn btn-auto" onclick="sendCommand('auto')">AUTOMAT (SENZOR)</button>
     <button id="btnOn" class="btn btn-on" onclick="sendCommand('on')">MANUAL: PORNIT</button>
     <button id="btnOff" class="btn btn-off" onclick="sendCommand('off')">MANUAL: OPRIT</button>
  </div>

  <script>
  setInterval(function() {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function(){
      if (this.readyState == 4 && this.status == 200) {
        var json = JSON.parse(this.responseText);
        document.getElementById("ldrValue").innerHTML = json.ldr;
        document.getElementById("stateText").innerHTML = json.stateStr;
        highlightButton(json.stateCode);
      }
    };
    xhr.open("GET", "/status", true);
    xhr.send();
  }, 1000);

  function sendCommand(mode){
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/set?mode=" + mode, true);
    xhr.send();
  }

  function highlightButton(code){
    document.getElementById("btnAuto").classList.remove("active");
    document.getElementById("btnOn").classList.remove("active");
    document.getElementById("btnOff").classList.remove("active");

    if(code == 0) document.getElementById("btnAuto").classList.add("active");
    if(code == 1) document.getElementById("btnOn").classList.add("active");
    if(code == 2) document.getElementById("btnOff").classList.add("active");
  }
  </script>
</body>
</html>
)rawliteral";

#endif