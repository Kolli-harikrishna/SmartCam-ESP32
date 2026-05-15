#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <Preferences.h>
#include <ESPmDNS.h>
#include "esp_camera.h"
#include "time.h"

// ---------- Setup Portal ----------
const char* apSSID = "SmartCam_Setup";
const char* apPassword = "12345678";
const char* mdnsName = "smartcam";
const unsigned long setupAPTime = 100000;
unsigned long setupAPStartTime = 0;
bool setupAPActive = false;

WebServer server(80);
Preferences prefs;
WiFiClientSecure client;

// ---------- Saved Data ----------
String wifiSSID = "";
String wifiPassword = "";
String botToken = "";
String chatID = "";
String deviceID = "";
String houseName = "";
String roomName = "";
String userPassword = "";

bool isLoggedIn = false;

// ---------- Time ----------
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

bool timeSynced = false;
bool previousWiFiConnected = false;
unsigned long lastTimeSyncAttempt = 0;
const unsigned long timeSyncInterval = 30000;

// ---------- PIR ----------
#define PIR_PIN 47

const unsigned long pirWarmupTime = 20000;
const unsigned long motionConfirmTime = 600;
const unsigned long alertCooldown = 20000;
const unsigned long lowResetTime = 5000;

unsigned long systemStartTime = 0;
unsigned long highStartTime = 0;
unsigned long lowStartTime = 0;
unsigned long lastTriggerTime = 0;

bool motionArmed = true;
bool securityEnabled = true;
bool isProcessing = false;
bool cameraReady = false;
bool systemStartedMessageSent = false;
int lastUpdateID = 0;
unsigned long lastTelegramCheck = 0;

// ---------- Camera Pins ----------
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     15
#define SIOD_GPIO_NUM     4
#define SIOC_GPIO_NUM     5

#define Y9_GPIO_NUM       16
#define Y8_GPIO_NUM       17
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       12
#define Y5_GPIO_NUM       10
#define Y4_GPIO_NUM       8
#define Y3_GPIO_NUM       9
#define Y2_GPIO_NUM       11

#define VSYNC_GPIO_NUM    6
#define HREF_GPIO_NUM     7
#define PCLK_GPIO_NUM     13

void loadConfig() {
  prefs.begin("smartcam", true);
  wifiSSID = prefs.getString("ssid", "");
  wifiPassword = prefs.getString("password", "");
  botToken = prefs.getString("botToken", "");
  chatID = prefs.getString("chatID", "");
  deviceID = prefs.getString("deviceID", "CAM001");
  houseName = prefs.getString("houseName", "HARISH HOUSE");
  roomName = prefs.getString("roomName", "CH01");
  userPassword = prefs.getString("userPass", "");
  prefs.end();
}

void saveConfig() {
  prefs.begin("smartcam", false);
  prefs.putString("ssid", wifiSSID);
  prefs.putString("password", wifiPassword);
  prefs.putString("botToken", botToken);
  prefs.putString("chatID", chatID);
  prefs.putString("deviceID", deviceID);
  prefs.putString("houseName", houseName);
  prefs.putString("roomName", roomName);
  prefs.end();
}

void saveUserPassword(String pass) {
  prefs.begin("smartcam", false);
  prefs.putString("userPass", pass);
  prefs.end();
}

String pageHeader(String title) {
  String html = "<!DOCTYPE html><html><head>";

  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>" + title + "</title>";

  html += "<style>";

  html += "*{margin:0;padding:0;box-sizing:border-box;font-family:Arial;}";

  html += "body{background:#0f172a;color:white;padding:18px;}";

  html += ".topbar{display:flex;justify-content:space-between;align-items:center;margin-bottom:20px;}";

  html += ".logo{font-size:26px;font-weight:bold;color:#38bdf8;}";

  html += ".user{background:#1e293b;padding:10px 16px;border-radius:14px;font-size:14px;}";

  html += ".card{background:#1e293b;padding:22px;border-radius:22px;margin-bottom:20px;box-shadow:0 8px 25px rgba(0,0,0,0.3);}";

  html += "h2{margin-bottom:15px;color:#38bdf8;}";

  html += "label{display:block;margin-top:15px;margin-bottom:6px;font-weight:bold;}";

  html += "input{width:100%;padding:14px;border:none;border-radius:14px;background:#334155;color:white;font-size:15px;}";

  html += "input::placeholder{color:#cbd5e1;}";

  html += ".btn{width:100%;padding:15px;border:none;border-radius:14px;background:#2563eb;color:white;font-size:16px;font-weight:bold;margin-top:18px;}";

  html += ".btn-red{background:#dc2626;}";

  html += ".status{display:flex;justify-content:space-between;align-items:center;background:#0f172a;padding:15px;border-radius:14px;margin-top:15px;}";

  html += ".green{color:#22c55e;font-weight:bold;}";

  html += ".red{color:#ef4444;font-weight:bold;}";

  html += ".grid{display:grid;grid-template-columns:1fr 1fr;gap:15px;margin-top:20px;}";

  html += ".smallCard{background:#334155;padding:18px;border-radius:16px;text-align:center;}";

  html += ".smallCard h3{font-size:15px;margin-bottom:8px;color:#38bdf8;}";

  html += ".smallCard p{font-size:18px;font-weight:bold;}";

  html += ".msg{background:#0f172a;padding:14px;border-radius:14px;margin-top:15px;color:#38bdf8;font-weight:bold;text-align:center;}";

  html += "</style></head><body>";

  return html;
}

String pageFooter() {
  return "</body></html>";
}

String createPasswordPage(String msg = "") {
  String page = pageHeader("Create Password");
  page += "<div class='card'><h2>SmartCam Setup</h2>";
  page += "<p>Connect to <b>SmartCam_Setup</b> and open <b>192.168.4.1</b></p>";
  if (msg != "") page += "<div class='msg'>" + msg + "</div>";
  page += "<form action='/setpassword' method='POST'>";
  page += "<label>Create Password</label><input type='password' name='p1' required>";
  page += "<label>Confirm Password</label><input type='password' name='p2' required>";
  page += "<button class='btn'>Save Password</button></form></div>";
  return page + pageFooter();
}

String loginPage(String msg = "") {
  String page = pageHeader("Login");
  page += "<div class='card'><h2>SmartCam Login</h2>";
  if (msg != "") page += "<div class='msg'>" + msg + "</div>";
  page += "<form action='/login' method='POST'>";
  page += "<label>Password</label><input type='password' name='p' required>";
  page += "<button class='btn'>Login</button></form></div>";
  return page + pageFooter();
}

String configPage(String msg = "") {

  String page = pageHeader("SmartCam App");

  page += "<div class='topbar'>";
  page += "<div class='logo'>SmartCam</div>";
  page += "<div class='user'>📷 " + deviceID + "</div>";
  page += "</div>";

  page += "<div class='card' style='text-align:center;'>";
  page += "<h2>Security System</h2>";

  if (securityEnabled) {
    page += "<div style='font-size:42px;margin:15px 0;'>🟢</div>";
    page += "<h2 style='color:#22c55e;'>SYSTEM ACTIVE</h2>";
    page += "<p>Motion detection is ON</p>";
  } else {
    page += "<div style='font-size:42px;margin:15px 0;'>🔴</div>";
    page += "<h2 style='color:#ef4444;'>SYSTEM OFF</h2>";
    page += "<p>Motion detection is disabled</p>";
  }

  page += "<form action='/security/on' method='POST'>";
  page += "<button class='btn'>Turn ON Security</button>";
  page += "</form>";

  page += "<form action='/security/off' method='POST'>";
  page += "<button class='btn btn-red'>Turn OFF Security</button>";
  page += "</form>";
  page += "</div>";

  page += "<div class='grid'>";

  page += "<div class='smallCard'>";
  page += "<h3>WiFi</h3>";
  if (WiFi.status() == WL_CONNECTED) {
    page += "<p class='green'>Connected</p>";
  } else {
    page += "<p class='red'>Disconnected</p>";
  }
  page += "</div>";

  page += "<div class='smallCard'>";
  page += "<h3>Device</h3>";
  page += "<p>" + deviceID + "</p>";
  page += "</div>";

  page += "<div class='smallCard'>";
  page += "<h3>Room</h3>";
  page += "<p>" + roomName + "</p>";
  page += "</div>";


  page += "</div>";

  page += "<div class='card'>";
  page += "<h2>Settings</h2>";

  if (msg != "") {
    page += "<div class='msg'>" + msg + "</div>";
  }

  page += "<form action='/save' method='POST'>";

  page += "<label>WiFi SSID</label>";
  page += "<input name='ssid' placeholder='Enter WiFi name' required>";

  page += "<label>WiFi Password</label>";
  page += "<input type='password' name='wpass' placeholder='Enter WiFi password' required>";

  page += "<label>Telegram Bot Token</label>";
  page += "<input name='bot' placeholder='Enter bot token' required>";

  page += "<label>Telegram Chat ID</label>";
  page += "<input name='chat' placeholder='Enter chat ID' required>";

  page += "<label>Device ID</label>";
  page += "<input name='device' placeholder='Example: CAM001' required>";

  page += "<label>House Name</label>";
  page += "<input name='house' placeholder='Enter house name' required>";

  page += "<label>Room Name</label>";
  page += "<input name='room' placeholder='Enter room name' required>";

  page += "<button class='btn'>Save Settings</button>";
  page += "</form>";

  page += "<form action='/logout' method='POST'>";
  page += "<button class='btn btn-red'>Logout</button>";
  page += "</form>";

  page += "</div>";

  return page + pageFooter();
}

String savedRestartPage() {
  return pageHeader("Saved") +
         "<div class='card'><h2>Configuration Saved</h2>"
         "<div class='msg'>New details saved successfully.</div>"
         "<p>Device is restarting automatically. Please wait 10 seconds.</p>"
         "</div>" + pageFooter();
}

void handleSecurityOn() {
  securityEnabled = true;
  motionArmed = true;
  highStartTime = 0;
  lowStartTime = millis();
  lastTriggerTime = 0;

  Serial.println("Security ON from Web App");

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleSecurityOff() {
  securityEnabled = false;
  motionArmed = false;
  highStartTime = 0;
  lowStartTime = 0;

  Serial.println("Security OFF from Web App");

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleRoot() {
  if (userPassword == "") server.send(200, "text/html", createPasswordPage());
  else if (!isLoggedIn) server.send(200, "text/html", loginPage());
  else server.send(200, "text/html", configPage());
}

void handleSetPassword() {
  String p1 = server.arg("p1");
  String p2 = server.arg("p2");

  if (p1 != p2) {
    server.send(200, "text/html", createPasswordPage("Passwords do not match."));
    return;
  }

  userPassword = p1;
  saveUserPassword(userPassword);
  isLoggedIn = true;
  server.send(200, "text/html", configPage("Password created successfully."));
}

void handleLogin() {
  if (server.arg("p") == userPassword) {
    isLoggedIn = true;
    server.send(200, "text/html", configPage("Login successful."));
  } else {
    server.send(200, "text/html", loginPage("Wrong password."));
  }
}

void handleLogout() {
  isLoggedIn = false;
  server.send(200, "text/html", loginPage("Logged out."));
}

void handleSave() {
  wifiSSID = server.arg("ssid");
  wifiPassword = server.arg("wpass");
  botToken = server.arg("bot");
  chatID = server.arg("chat");
  deviceID = server.arg("device");
  houseName = server.arg("house");
  roomName = server.arg("room");

  saveConfig();

  server.send(200, "text/html", savedRestartPage());
  delay(2000);
  ESP.restart();
}

// ---------- Time Sync ----------
bool syncTimeNow() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Time sync failed: WiFi not connected");
    timeSynced = false;
    return false;
  }

  Serial.println("Syncing date and time from NTP...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  struct tm timeinfo;
  int retry = 0;

  while (!getLocalTime(&timeinfo) && retry < 20) {
    Serial.println("Waiting for NTP time...");
    delay(500);
    retry++;
  }

  if (retry >= 20) {
    Serial.println("NTP time sync failed");
    timeSynced = false;
    return false;
  }

  Serial.println("Date and time synced successfully");
  timeSynced = true;
  return true;
}

// ---------- WiFi ----------
void startSetupWiFi() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(apSSID, apPassword);
  WiFi.setSleep(false);

  setupAPStartTime = millis();
  setupAPActive = true;

  Serial.println("Setup WiFi started for 100 seconds");
  Serial.println("Connect to: SmartCam_Setup");
  Serial.println("Password: 12345678");
  Serial.print("Open: ");
  Serial.println(WiFi.softAPIP());
}

void autoStopSetupAP() {
  if (setupAPActive && millis() - setupAPStartTime >= setupAPTime) {
    WiFi.softAPdisconnect(true);
    setupAPActive = false;
    Serial.println("Setup AP stopped automatically");
  }
}

bool connectWiFi() {
  if (wifiSSID == "") return false;

  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);

  Serial.print("Connecting to WiFi");
  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin(mdnsName)) {
  MDNS.addService("http", "tcp", 80);
  Serial.println("mDNS started: smartcam.local");
} else {
  Serial.println("mDNS start failed");
}

    previousWiFiConnected = true;
    syncTimeNow();
    return true;
  }

  Serial.println("WiFi failed");
  previousWiFiConnected = false;
  timeSynced = false;
  return false;
}

void reconnectWiFiIfNeeded() {
  static unsigned long lastReconnectAttempt = 0;

  if (wifiSSID == "") return;

  unsigned long now = millis();

  if (WiFi.status() == WL_CONNECTED) {
    if (!previousWiFiConnected) {
      Serial.println("WiFi reconnected");
      previousWiFiConnected = true;

      if (MDNS.begin(mdnsName)) {
  MDNS.addService("http", "tcp", 80);
  Serial.println("mDNS restarted: smartcam.local");
}

      syncTimeNow();
    }

    if (!timeSynced && now - lastTimeSyncAttempt >= timeSyncInterval) {
      lastTimeSyncAttempt = now;
      syncTimeNow();
    }

    return;
  }

  if (previousWiFiConnected) {
    Serial.println("WiFi disconnected");
  }

  previousWiFiConnected = false;
  timeSynced = false;

  if (now - lastReconnectAttempt < 15000) return;

  lastReconnectAttempt = now;

  Serial.println("WiFi disconnected. Reconnecting...");
  WiFi.disconnect(false);
  delay(100);
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
}

// ---------- Camera ----------
bool initCamera() {
  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;

  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;

  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  config.grab_mode = CAMERA_GRAB_LATEST;

  if (psramFound()) config.fb_location = CAMERA_FB_IN_PSRAM;
  else config.fb_location = CAMERA_FB_IN_DRAM;

  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK) {
    Serial.print("Camera init failed: 0x");
    Serial.println(err, HEX);
    return false;
  }

  sensor_t *s = esp_camera_sensor_get();
  if (s) {
    s->set_vflip(s, 1);
    s->set_brightness(s, 1);
    s->set_saturation(s, -2);
    s->set_framesize(s, FRAMESIZE_VGA);
  }

  Serial.println("Camera init success");
  return true;
}

void startCameraAfterAPStops() {
  if (!setupAPActive && !cameraReady) {
    cameraReady = initCamera();

    if (WiFi.status() == WL_CONNECTED && !systemStartedMessageSent) {
      syncTimeNow();

      String startMsg = "✅ SmartCam system started successfully.\n";
      startMsg += "📷 Device ID: " + deviceID + "\n";
      startMsg += "🏠 House: " + houseName + "\n";
      startMsg += "🚪 Room: " + roomName + "\n";
      startMsg += "⚙️ Setup Page: 192.168.4.1\n";
      startMsg += "⚙️ Local Config: smartcam.local";

      sendTelegramMessage(startMsg);
      sendTelegramMessage("📡 Send /help to view SmartCam commands");
      systemStartedMessageSent = true;
    }
  }
}

// ---------- Telegram ----------
String urlEncode(String str) {
  String encoded = "";
  char c, code0, code1;

  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) encoded += c;
    else if (c == ' ') encoded += '+';
    else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) code1 = (c & 0xf) - 10 + 'A';
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) code0 = c - 10 + 'A';
      encoded += '%';
      encoded += code0;
      encoded += code1;
    }
  }

  return encoded;
}

String readHttpResponse() {
  String response = "";
  unsigned long timeout = millis();

  while ((client.connected() || client.available()) && millis() - timeout < 5000) {
    while (client.available()) {
      response += (char)client.read();
      timeout = millis();
    }
  }

  return response;
}

bool sendTelegramMessage(String text) {
  if (WiFi.status() != WL_CONNECTED || botToken == "" || chatID == "") return false;

  client.setInsecure();
  client.setTimeout(15000);

  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Telegram message connection failed");
    return false;
  }

  String body = "chat_id=" + chatID + "&text=" + urlEncode(text);

  client.println("POST /bot" + botToken + "/sendMessage HTTP/1.1");
  client.println("Host: api.telegram.org");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Content-Length: " + String(body.length()));
  client.println("Connection: close");
  client.println();
  client.print(body);

  readHttpResponse();
  client.flush();
  client.stop();

  Serial.println("Telegram text sent");
  return true;
}

bool sendPhotoToTelegram(String captionText) {
  if (!cameraReady) {
    Serial.println("Photo skipped: camera not ready");
    return false;
  }

  if (WiFi.status() != WL_CONNECTED || botToken == "" || chatID == "") {
    Serial.println("Photo skipped: WiFi/token/chat missing");
    return false;
  }

  camera_fb_t *fb = esp_camera_fb_get();

  if (!fb) {
    Serial.println("Camera capture failed");
    return false;
  }

  client.setInsecure();
  client.setTimeout(20000);

  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Telegram photo connection failed");
    esp_camera_fb_return(fb);
    return false;
  }

  String boundary = "----SmartCamBoundary";

  String head = "--" + boundary + "\r\n";
  head += "Content-Disposition: form-data; name=\"chat_id\"\r\n\r\n";
  head += chatID + "\r\n";

  head += "--" + boundary + "\r\n";
  head += "Content-Disposition: form-data; name=\"caption\"\r\n\r\n";
  head += captionText + "\r\n";

  head += "--" + boundary + "\r\n";
  head += "Content-Disposition: form-data; name=\"photo\"; filename=\"motion.jpg\"\r\n";
  head += "Content-Type: image/jpeg\r\n\r\n";

  String tail = "\r\n--" + boundary + "--\r\n";

  uint32_t totalLen = head.length() + fb->len + tail.length();

  client.println("POST /bot" + botToken + "/sendPhoto HTTP/1.1");
  client.println("Host: api.telegram.org");
  client.println("Content-Type: multipart/form-data; boundary=" + boundary);
  client.println("Content-Length: " + String(totalLen));
  client.println("Connection: close");
  client.println();

  client.print(head);

  for (size_t i = 0; i < fb->len; i += 1024) {
    size_t chunkSize = (i + 1024 < fb->len) ? 1024 : (fb->len - i);
    client.write(fb->buf + i, chunkSize);
  }

  client.print(tail);

  String response = readHttpResponse();
  client.flush();
  client.stop();
  esp_camera_fb_return(fb);

  bool ok = response.indexOf("\"ok\":true") != -1;
  Serial.println(ok ? "Telegram photo sent" : "Telegram photo failed");
  return ok;
}

// ---------- Time ----------
String getDateString() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    syncTimeNow();
    if (!getLocalTime(&timeinfo)) return "Not available";
  }

  char buf[20];
  strftime(buf, sizeof(buf), "%d-%m-%Y", &timeinfo);
  return String(buf);
}

String getTimeString() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    syncTimeNow();
    if (!getLocalTime(&timeinfo)) return "Not available";
  }

  char buf[20];
  strftime(buf, sizeof(buf), "%H:%M:%S", &timeinfo);
  return String(buf);
}

void checkTelegramCommands() {
  if (WiFi.status() != WL_CONNECTED) return;
  if (botToken == "" || chatID == "") return;

  if (millis() - lastTelegramCheck < 5000) return;
  lastTelegramCheck = millis();

  client.setInsecure();
  client.setTimeout(5000);

  String url = "/bot" + botToken + "/getUpdates?offset=" + String(lastUpdateID + 1);

  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Telegram command connection failed");
    return;
  }

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.telegram.org\r\n" +
               "Connection: close\r\n\r\n");

  String response = readHttpResponse();
  client.flush();
  client.stop();

  if (response.indexOf("\"update_id\":") == -1) return;

  int idIndex = response.lastIndexOf("\"update_id\":");
  int idStart = response.indexOf(":", idIndex) + 1;
  int idEnd = response.indexOf(",", idStart);
  lastUpdateID = response.substring(idStart, idEnd).toInt();

  if (response.indexOf(chatID) == -1) return;

  if (response.indexOf("\"text\":\"/on\"") != -1){
    securityEnabled = true;
    motionArmed = true;
    highStartTime = 0;
    lowStartTime = millis();
    lastTriggerTime = 0;
    sendTelegramMessage("🟢 SECURITY SYSTEM ENABLED\n\n📷 Device: " + deviceID);
    Serial.println("Security ON from Telegram");
  }

  if (response.indexOf("\"text\":\"/off\"") != -1) {
    securityEnabled = false;
    motionArmed = false;
    highStartTime = 0;
    lowStartTime = 0;
    sendTelegramMessage("🔴 SECURITY SYSTEM DISABLED\n\n📷 Device: " + deviceID);
    Serial.println("Security OFF from Telegram");
  }
  if (response.indexOf("\"text\":\"/help\"") != -1) {
    String helpMsg = "📡 SMARTCAM COMMANDS\n\n";
    helpMsg += "/on - Enable security\n";
    helpMsg += "/off - Disable security\n";
    helpMsg += "/status - Device status\n";
    helpMsg += "/help - Show commands";

    sendTelegramMessage(helpMsg);
  }
  if (response.indexOf("\"text\":\"/status\"") != -1) {
    String statusMsg = "📡 SMARTCAM STATUS\n\n";
    statusMsg += "📷 Device: " + deviceID + "\n";
    statusMsg += securityEnabled ? "🟢 Security: ON\n" : "🔴 Security: OFF\n";
    statusMsg += WiFi.status() == WL_CONNECTED ? "📶 WiFi: Connected\n" : "❌ WiFi: Disconnected\n";
    statusMsg += "📅 Date: " + getDateString() + "\n";
    statusMsg += "🕒 Time: " + getTimeString();

    sendTelegramMessage(statusMsg);
    Serial.println("Status sent from Telegram");
  }
}
// ---------- Motion ----------
void handleMotion() {
  if (isProcessing) return;
  isProcessing = true;

  if (!timeSynced) {
    syncTimeNow();
  }

  String caption = "🚨 SECURITY ALERT 🚨\n\n";
  caption += "📷 Device ID: " + deviceID + "\n";
  caption += "🏠 House: " + houseName + "\n";
  caption += "🚪 Room: " + roomName + "\n";
  caption += "📅 Date: " + getDateString() + "\n";
  caption += "🕒 Time: " + getTimeString() + "\n";

  Serial.println("REAL MOTION DETECTED");
  Serial.println(caption);

  bool photoSent = sendPhotoToTelegram(caption);

  if (!photoSent) {
    sendTelegramMessage(caption + "\n\n⚠️ Photo sending failed.");
  }

  isProcessing = false;
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  delay(1000);

  systemStartTime = millis();

  pinMode(PIR_PIN, INPUT);

  loadConfig();
  startSetupWiFi();

  server.on("/", handleRoot);
  server.on("/setpassword", HTTP_POST, handleSetPassword);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/logout", HTTP_POST, handleLogout);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/security/on", HTTP_POST, handleSecurityOn);
  server.on("/security/off", HTTP_POST, handleSecurityOff);

  server.begin();
  Serial.println("HTTP server started");

  connectWiFi();

  Serial.println("Setup AP active for 100 seconds...");
  Serial.println("Camera and PIR will start after setup AP stops.");
}
// ---------- Loop ----------
void loop() {
  server.handleClient();
  autoStopSetupAP();
  reconnectWiFiIfNeeded();
  startCameraAfterAPStops();
  checkTelegramCommands();

  if (setupAPActive || !cameraReady) {
    delay(50);
    yield();
    return;
  }

 if (!securityEnabled) {
    delay(50);
    yield();
    return;
}

  int pirState = digitalRead(PIR_PIN);
  unsigned long now = millis();

  if (now - systemStartTime < pirWarmupTime) {
    delay(50);
    yield();
    return;
  }

  if (pirState == LOW) {
    highStartTime = 0;

    if (lowStartTime == 0) lowStartTime = now;

    if (now - lowStartTime >= lowResetTime) {
      motionArmed = true;
    }
  }

  if (pirState == HIGH) {
    lowStartTime = 0;

    if (highStartTime == 0) highStartTime = now;

    if (motionArmed &&
        !isProcessing &&
        (now - highStartTime >= motionConfirmTime) &&
        (now - lastTriggerTime >= alertCooldown)) {

      motionArmed = false;
      lastTriggerTime = now;
      handleMotion();
    }
  }

  delay(50);
  yield();
}
