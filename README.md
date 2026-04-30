# 🚨 SmartCam-ESP32 Security System

## 📌 Project Overview
SmartCam is an IoT-based smart security system developed using ESP32-S3, PIR motion sensor, and camera module.

The system detects human motion and instantly captures an image, sending a real-time alert to the user via Telegram.

This project is designed as a **product-oriented solution**, focusing on usability, reliability, and real-world application.

---

## 🎯 Key Features
- 🔍 Motion detection using PIR sensor
- 📷 Automatic image capture on intrusion
- 📲 Instant Telegram alert with image
- 🌐 Wi-Fi + Access Point (AP) setup system
- 🔐 User login security for configuration
- ⚡ Fast system response and optimized performance
- 🧠 Reduced false triggering using PIR tuning
- 🌍 Local access via `smartcam.local`
- 💾 Supports SD card storage (optional)

---

## ⚙️ System Workflow
1. System powers ON
2. AP mode activates for initial setup (45 seconds)
3. User connects and enters configuration details
4. Device connects to Wi-Fi
5. PIR sensor monitors environment
6. Motion detected → Camera captures image
7. Telegram alert sent with date & time

---

## 🛠️ Hardware Components
- ESP32-S3 Camera Module
- PIR Motion Sensor
- Power Supply (5V Adapter)
- Jumper Wires / PCB
- Optional: SD Card (32GB)

---

## 💻 Software & Tools Used
- Arduino IDE
- Embedded C / C++
- WiFi & WebServer Libraries
- Telegram Bot API
---

## 📂 Project Structure
- `esp32_s3_cam_security_code.ino` → Main embedded code  
- `SmartCam_User_Guide.pdf` → End-user setup instructions  

---

## 🔄 Version Control (GitHub)
The project development was tracked using GitHub with multiple commits including:
- Initial setup of ESP32-S3
- PIR sensor integration
- Wi-Fi and AP configuration
- Camera module integration
- Telegram alert system
- System optimization and testing

---

## 🧪 Testing & Results
- Motion detection working accurately
- Telegram alerts received successfully
- Reduced false triggering after calibration
- Stable Wi-Fi connectivity achieved
- System tested in real environment

---

## ⚠️ Challenges Faced
- False triggering in PIR sensor
- Camera delay and quality issues
- Power stability for ESP32

### ✅ Solutions
- Adjusted PIR sensitivity and delay
- Optimized camera resolution and settings
- Used stable 5V power supply

---

## 👤 Developer
**Kolli Harikrishna**  
B.Tech ICT | Embedded Systems Enthusiast  

---

## 📌 Conclusion
SmartCam is a reliable and efficient smart security system that demonstrates practical implementation of IoT and embedded systems. The project is designed as a real-world product with user-friendly configuration and strong alert mechanisms.
