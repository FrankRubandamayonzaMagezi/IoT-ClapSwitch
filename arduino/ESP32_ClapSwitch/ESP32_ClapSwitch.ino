#include <WiFi.h> 
#include <WebServer.h> 

// ==============================  
//Wi-Fi Credentials 
// ============================== 

const char* ssid = "Mak-Unipod"; 
const char* password = "1nNova7ioN5"; 

// ============================== 
// Web Server 
// ============================== 

WebServer server(80); 

// ============================== 
// Built-in LED 
// ============================== 

const int LED_PIN = 2; 
bool ledState = false; 

// ============================== 
// Homepage 
// ============================== 
 

void handleRoot()
{
  String html = "";

  html += "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>IoT ClapSwitch</title>";

  html += "<style>";
  html += "body{font-family:Arial,sans-serif;text-align:center;background:#eef3f8;padding:40px;margin:0;}";
  html += "h1{color:#333;font-size:42px;}";
  html += "h3{color:#666;}";
  html += ".card{background:white;width:90%;max-width:700px;margin:auto;padding:30px;border-radius:15px;box-shadow:0 4px 15px rgba(0,0,0,0.15);}";
  html += ".status{font-size:42px;font-weight:bold;margin:30px;}";
  html += ".on{color:#28a745;}";
  html += ".off{color:#dc3545;}";
  html += "button{padding:18px 40px;font-size:22px;margin:10px;border:none;border-radius:12px;cursor:pointer;transition:0.3s;}";
  html += ".btnOn{background:#28a745;color:white;}";
  html += ".btnOff{background:#dc3545;color:white;}";
  html += "button:hover{transform:scale(1.05);}";
  html += "footer{margin-top:30px;color:#666;font-size:15px;}";
  html += "</style>";

  html += "</head>";
  html += "<body>";

  html += "<div class='card'>";

  html += "<h1>👏 IoT ClapSwitch</h1>";
  html += "<h3>ESP32 Web Dashboard</h3>";

  html += "<p><strong>IP Address:</strong> ";
  html += WiFi.localIP().toString();
  html += "</p>";

  html += "<p><strong>Wi-Fi Status:</strong> Connected</p>";

  html += "<p><strong>Signal Strength:</strong> ";
  html += String(WiFi.RSSI());
  html += " dBm</p>";

  html += "<div id='status' class='status'>";

  if (ledState)
    html += "<span class='on'>🟢 LED ON</span>";
  else
    html += "<span class='off'>🔴 LED OFF</span>";

  html += "</div>";

  html += "<button class='btnOn' onclick=\"fetch('/on').then(updateStatus)\">Turn ON</button>";

  html += "<button class='btnOff' onclick=\"fetch('/off').then(updateStatus)\">Turn OFF</button>";

  html += "<footer>";
  html += "<hr>";
  html += "Created by <b>Frank Rubandamayonza Magezi</b>";
  html += "</footer>";

  html += "</div>";

  html += R"rawliteral(

<script>

function updateStatus()
{
    fetch('/status')
    .then(response => response.text())
    .then(state => {

        let status = document.getElementById("status");

        if(state=="ON")
        {
            status.innerHTML="<span class='on'>🟢 LED ON</span>";
        }
        else
        {
            status.innerHTML="<span class='off'>🔴 LED OFF</span>";
        }

    })
    .catch(error => {
        console.log("Connection lost.");
    });
}

updateStatus();

setInterval(updateStatus,500);

</script>

)rawliteral";

  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html);
}

// ==============================
// Turn LED ON
// ==============================
void handleOn()
{
  ledState = true;
  digitalWrite(LED_PIN, HIGH);

  server.send(200, "text/plain", "LED ON");
}

// ==============================
// Turn LED OFF
// ==============================
void handleOff()
{
  ledState = false;
  digitalWrite(LED_PIN, LOW);

  server.send(200, "text/plain", "LED OFF");
}

// ==============================
// Return LED Status
// ==============================
void handleStatus()
{
  if (ledState)
    server.send(200, "text/plain", "ON");
  else
    server.send(200, "text/plain", "OFF");
}





// ============================== 
// Setup 
// ============================== 

void setup() { 

Serial.begin(115200); 
pinMode(LED_PIN, OUTPUT); 
digitalWrite(LED_PIN, LOW); 
Serial.print("Connecting to Wi-Fi"); 
WiFi.begin(ssid, password); 
while (WiFi.status() != WL_CONNECTED) { delay(500); 
Serial.print("."); 

} 
Serial.println(); 
Serial.println("Wi-Fi Connected!");
Serial.print("IP Address: "); 
Serial.println(WiFi.localIP()); 
server.on("/", handleRoot); 
server.on("/on", handleOn); 
server.on("/off", handleOff); 
server.on("/status", handleStatus); 
server.begin(); 
Serial.println("HTTP Server Started");
 } 

// ============================== 
// Main Loop 
// ============================== 
void loop() { 
server.handleClient(); 
}
