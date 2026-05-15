<div align="center">

# 🚨 SmartCam User Guide
## Setup, Configuration & Testing Manual

### ESP32-S3 Smart Security Surveillance System

</div>

---

# 📌 Introduction

Welcome to the SmartCam Security System.

SmartCam is an IoT-based smart surveillance system designed for real-time intrusion monitoring and instant Telegram alerts. The system uses an ESP32-S3 Camera Module and PIR motion sensor to detect movement, capture images, and notify users remotely.

This guide explains:

- Device setup
- Wi-Fi configuration
- Telegram bot creation
- Dashboard access
- Security testing
- Troubleshooting

This manual is intended for end users operating the completed SmartCam device.

---

# 📦 Package Contents

| Component | Description |
|---|---|
| SmartCam Device | Main security system |
| ESP32-S3 Camera | Built-in image capture |
| PIR Motion Sensor | Motion detection |
| Power Adapter | 5V system power |
| USB Cable | Device power connection |

---

# 🔌 1. Powering the Device

## Steps
1. Connect the SmartCam device to a stable **5V power supply**
2. Wait for the system to boot
3. The setup hotspot will start automatically

---

# 📶 2. Setup Mode (Initial Configuration)

After power ON, SmartCam creates its own setup Wi-Fi network.

## Connect to Setup Network

Open Wi-Fi settings on your mobile or laptop and connect to:

```text
Wi-Fi Name : SmartCam_Setup
Password   : 12345678
```

⚠️ Important:
- Setup hotspot remains active for approximately **100 seconds**
- Complete setup before the hotspot closes

---

# 🌐 3. Open Setup Page

After connecting to the SmartCam setup Wi-Fi:

## Open Browser
Enter the following address:

```text
http://192.168.4.1
```

The SmartCam configuration page will open.

---

# 🔐 4. Create Device Login Password

During first setup:

1. Create a secure login password
2. Confirm the password
3. Click **Save Password**

This password protects access to the SmartCam dashboard.

---

# 🤖 5. Create Telegram Bot

SmartCam uses Telegram to send security alerts and captured images.

Follow these steps carefully.

---

## Step 1 — Open Telegram

Install and open the Telegram application on:
- Mobile phone
- Laptop/Desktop

---

## Step 2 — Search for BotFather

In Telegram search:

```text
@BotFather
```

Open the verified BotFather account.

---

## Step 3 — Create New Bot

Send the command:

```text
/newbot
```

BotFather will ask:

### Enter Bot Name
Example:

```text
SmartCam Security Bot
```

---

## Step 4 — Create Bot Username

Choose a unique username ending with:

```text
bot
```

Example:

```text
smartcam_security_bot
```

---

## Step 5 — Copy Bot Token

BotFather will generate a Bot Token.

Example:

```text
123456789:ABCxxxxxxxxxxxxxxxxxxxx
```

⚠️ Save this token carefully.

This token is required in SmartCam setup.

---

# 🆔 6. Get Telegram Chat ID

## Step 1
Search Telegram for:

```text
@userinfobot
```

---

## Step 2
Open the bot and press:

```text
Start
```

---

## Step 3
Copy Your Chat ID

Example:

```text
5646084450
```

⚠️ Save this Chat ID carefully.

---

# ⚙️ 7. Enter Configuration Details

On the SmartCam setup page, enter:

| Field | Description |
|---|---|
| Wi-Fi Name | Your router Wi-Fi SSID |
| Wi-Fi Password | Router password |
| Telegram Bot Token | BotFather token |
| Telegram Chat ID | Your Telegram Chat ID |
| Device ID | Example: CAM001 |
| House Name | Example: Krishna House |
| Room Name | Example: Main Door |

After entering details:

✅ Click **Save Settings**

The device will restart automatically.

---

# 🌍 8. Access SmartCam Dashboard

After restart:

1. Connect your phone/laptop to the SAME Wi-Fi network
2. Open browser
3. Enter:

```text
http://smartcam.local
```

---

## Dashboard Features

The SmartCam dashboard includes:

- Security ON/OFF controls
- Device monitoring
- Wi-Fi status
- Login protection
- Device configuration

---

# 📡 9. Telegram Commands

SmartCam supports remote Telegram commands.

| Command | Function |
|---|---|
| `/on` | Enable security system |
| `/off` | Disable security system |
| `/status` | View current system status |
| `/help` | Show all available commands |

---

# 🧪 10. Testing the System

## Motion Detection Test

### Step 1
Power ON the SmartCam device

### Step 2
Wait until:
- Wi-Fi connects
- Telegram becomes active

### Step 3
Move in front of the PIR sensor

### Step 4
Verify:
- Motion detected
- Image captured
- Telegram alert received

---

# 🚨 11. Expected Telegram Alert

When motion is detected, Telegram will receive:

✅ Intrusion alert  
✅ Captured image  
✅ Device ID  
✅ House name  
✅ Room name  
✅ Date and time  

---

# ⚠️ 12. Important Usage Notes

## Best PIR Placement
- Keep PIR sensor facing open area
- Avoid direct sunlight
- Avoid fans and heat sources
- Keep sensor outside thick enclosure walls

---

## Stable Operation Tips
- Use stable 5V power supply
- Ensure good Wi-Fi signal
- Avoid overheating inside enclosure
- Keep camera lens clean

---

## Network Requirement
`smartcam.local` works only when:
- Device and user are connected to the SAME Wi-Fi network

---

# 🛠️ 13. Troubleshooting

| Problem | Solution |
|---|---|
| smartcam.local not opening | Connect to same Wi-Fi |
| Telegram alerts not working | Check internet & bot token |
| PIR false detection | Reposition sensor |
| Device restarting | Use stable 5V supply |
| Camera image not sending | Check Wi-Fi signal |

---

# 🔄 14. System Workflow

```text
Power ON
    ↓
Setup Wi-Fi Starts
    ↓
User Configuration
    ↓
Wi-Fi Connection
    ↓
Dashboard Activated
    ↓
Motion Monitoring
    ↓
Motion Detected
    ↓
Image Captured
    ↓
Telegram Alert Sent
```

---

# 📌 Conclusion

SmartCam provides a reliable, user-friendly, and real-time smart surveillance solution using IoT and embedded system technologies.

The system combines:
- Motion sensing
- Embedded web applications
- Wireless communication
- Telegram cloud alerts
- Remote monitoring

into a single intelligent security platform suitable for:

- Homes
- Offices
- Hostels
- Smart surveillance applications

---

<div align="center">

# ✅ SmartCam Ready for Use

### Secure • Smart • Reliable

</div>
