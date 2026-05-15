<div align="center">

# 🚨 SmartCam
## Advanced ESP32-S3 Smart Security System

### Real-Time IoT Surveillance & Intrusion Detection System

<img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=for-the-badge">
<img src="https://img.shields.io/badge/Technology-IoT-green?style=for-the-badge">
<img src="https://img.shields.io/badge/Status-Working-success?style=for-the-badge">
<img src="https://img.shields.io/badge/Alerts-Telegram-orange?style=for-the-badge">

</div>

## 📷 Project Preview

<p align="center">
  <img src="images/smartcam.jpg" width="700">
</p>

# 📌 Project Overview

SmartCam is an advanced IoT-based smart security system developed using the ESP32-S3 Camera Module, PIR motion sensor, embedded web technologies, and Telegram cloud alerts.

The system continuously monitors the environment for human motion. When movement is detected, SmartCam instantly captures an image and sends a real-time security alert directly to the user through Telegram.

This project is designed as a product-oriented smart surveillance solution focusing on:

- ✅ Real-time monitoring
- ✅ Remote control
- ✅ Stable long-runtime operation
- ✅ Embedded web dashboard
- ✅ Wireless IoT communication
- ✅ User-friendly configuration


# ✨ Key Features

## 🔍 Smart Motion Detection
- PIR-based human motion sensing
- Real-time intrusion monitoring
- Optimized motion filtering
- Reduced false triggering


## 📷 Automatic Image Capture
- Instant image capture during intrusion
- Real-time Telegram image delivery
- Optimized camera performance


## 📲 Telegram Cloud Alerts
- Instant intrusion notifications
- Remote command control
- Live device status monitoring
- Date & time information with alerts


## 🌐 Embedded Web Dashboard
- Mobile-friendly app-style interface
- Access through:
  
```text
smartcam.local
```

- Secure login authentication
- Device configuration panel


## ⚡ Stable IoT Connectivity
- Wi-Fi auto reconnect
- NTP time synchronization
- Improved network stability
- Better long-runtime performance


## 🔐 Remote Security Control
- Enable/Disable security remotely
- Telegram command integration
- Embedded web control system


# 📡 Telegram Commands

| Command | Function |
|---|---|
| `/on` | Enable security system |
| `/off` | Disable security system |
| `/status` | View current device status |
| `/help` | Show available commands |


# ⚙️ System Workflow

```text
1. System Power ON
        ↓
2. SmartCam Setup Hotspot Starts
        ↓
3. User Opens 192.168.4.1
        ↓
4. Wi-Fi & Telegram Setup Completed
        ↓
5. Device Connects to Wi-Fi
        ↓
6. smartcam.local Dashboard Activated
        ↓
7. PIR Sensor Monitors Environment
        ↓
8. Motion Detected
        ↓
9. Camera Captures Image
        ↓
10. Telegram Alert Sent Instantly
```


# 🛠️ Hardware Components

| Component | Purpose |
|---|---|
| ESP32-S3 Camera Module | Main controller & image processing |
| PIR Motion Sensor | Human motion detection |
| 5V Power Adapter | System power supply |
| Jumper Wires / PCB | Hardware connections |
| Enclosure Box | Product casing |
| Optional MicroSD Card | Local storage support |


# 💻 Software & Technologies Used

| Technology | Purpose |
|---|---|
| Arduino IDE | Firmware development |
| Embedded C/C++ | System programming |
| ESP32 WebServer | Embedded dashboard |
| WiFi Library | Wireless communication |
| ESPmDNS | smartcam.local access |
| Telegram Bot API | Alerts & remote commands |
| NTP Time Sync | Real-time clock |
| HTML & CSS | Dashboard UI |


# 🌐 Smart Web Dashboard

The system includes a responsive embedded dashboard with:

- ✅ Security ON/OFF controls
- ✅ Device monitoring
- ✅ Wi-Fi connection status
- ✅ Login authentication
- ✅ Configuration management
- ✅ Mobile browser support
- ✅ App-style interface


# 🔄 Stability Improvements

The latest SmartCam version includes:

- ✅ Improved Telegram command stability
- ✅ Better Wi-Fi reconnect handling
- ✅ Reduced PIR false triggering
- ✅ Improved smartcam.local stability
- ✅ Better HTTPS communication handling
- ✅ Optimized motion reset logic
- ✅ Reduced CPU & network overload
- ✅ Better long-runtime performance


# 🧪 Testing & Results

| Test | Result |
|---|---|
| Motion Detection | ✅ Successful |
| Telegram Image Alerts | ✅ Working |
| Telegram Remote Commands | ✅ Stable |
| smartcam.local Access | ✅ Working |
| Web Dashboard Control | ✅ Working |
| Wi-Fi Reconnect Testing | ✅ Successful |
| Long Runtime Stability | ✅ Improved |
| Real Environment Testing | ✅ Completed |


# ⚠️ Challenges Faced

## Problems
- PIR false triggering inside enclosure
- Wi-Fi instability during long runtime
- Telegram HTTPS connection failures
- Repeated detections from PIR retriggering
- Heat buildup inside enclosure


## Solutions
- PIR timing optimization
- Telegram polling optimization
- Wi-Fi auto reconnect implementation
- Improved motion reset logic
- Better client connection handling
- Improved enclosure airflow planning


# 📂 Project Structure

```text
SmartCam/
│
├── SmartCam_Final_Code.ino
├── README.md
├── images/
├── user_guide.pdf
└── hardware/
```


# 🔧 Future Improvements

- AI-based human detection
- Face recognition support
- Mobile application integration
- Cloud storage system
- Buzzer & alarm integration
- Battery backup support
- Night vision enhancements


# 👨‍💻 Developer

## Kolli Harikrishna

B.Tech ICT Student  
Embedded Systems & IoT Enthusiast


# 📌 Conclusion

SmartCam is a practical and reliable IoT-based smart surveillance system developed using embedded systems and wireless communication technologies.

The project successfully demonstrates:

- ✅ Real-time intrusion monitoring
- ✅ Embedded web applications
- ✅ Remote security control
- ✅ IoT cloud communication
- ✅ Smart automation concepts

SmartCam combines usability, stability, and real-world implementation into a product-oriented embedded security solution suitable for:

- 🏠 Homes
- 🏢 Offices
- 🏫 Hostels
- 🌐 Smart surveillance applications


<div align="center">

# ⭐ Project Highlights

✅ Real-Time Intrusion Detection  
✅ Telegram-Based Smart Alerts  
✅ Embedded Web Dashboard  
✅ Remote Security Control  
✅ IoT + Embedded Systems Integration  
✅ Product-Oriented Design  
✅ Stable Long-Time Operation  

</div>
