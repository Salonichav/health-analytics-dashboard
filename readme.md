# ❤️ Real-Time Heart Rate & SpO₂ Monitoring Dashboard

An IoT-based healthcare monitoring system built using **ESP32**, **MAX30102**, and an **IR Sensor** for real-time Heart Rate (BPM) and SpO₂ monitoring. The system features intelligent patient detection, reliable measurement workflows, and a Wi-Fi-enabled dashboard hosted directly on the ESP32.

## 🚀 Features

* Real-time Heart Rate (BPM) monitoring
* Real-time SpO₂ measurement
* Intelligent patient detection using IR Sensor
* State-driven measurement workflow
* ESP32-hosted web dashboard
* Wireless monitoring over Wi-Fi
* Live sensor data visualization
* Stable and reliable readings through sensor calibration

---

## 🏗️ System Workflow

```text
No Patient Detected
        ↓
"Waiting for Patient"
        ↓
Patient Detected by IR Sensor
        ↓
"Please Place Your Finger"
        ↓
Finger Placed on MAX30102
        ↓
Measurement Starts
        ↓
Reading Stabilizes
        ↓
Live BPM & SpO₂ Displayed
```

---

## 🛠️ Hardware Components

* ESP32 DevKit V1
* MAX30102 Pulse Oximeter & Heart Rate Sensor
* IR Sensor Module
* Breadboard
* Jumper Wires
* USB Cable

---

## 💻 Software Stack

### Firmware

* Embedded C
* Arduino IDE

### Frontend Dashboard

* HTML
* CSS
* JavaScript

### Communication

* Wi-Fi
* HTTP Server

---

## 📡 System Architecture

```text
             +------------------+
             |    IR Sensor     |
             +--------+---------+
                      |
                      v
              Patient Detection
                      |
                      v
+------------+   +---------+   +----------------+
| MAX30102   |-->| ESP32   |-->| Web Dashboard  |
| Sensor     |   |         |   | (HTML/CSS/JS)  |
+------------+   +---------+   +----------------+
                      |
                      |
                 Wi-Fi Network
```

---

## 🎯 Key Challenges Solved

### 1. Patient Detection Logic

Implemented an IR-based detection mechanism to ensure measurements begin only when a patient is present.

### 2. Sensor Calibration

Fine-tuned sensor readings to improve BPM and SpO₂ measurement stability and reliability.

### 3. Wi-Fi Communication

Developed an ESP32-hosted dashboard for seamless wireless monitoring and real-time data access.

---

## 📊 Dashboard Capabilities

* Displays live Heart Rate (BPM)
* Displays live SpO₂ values
* Shows patient monitoring status
* Accessible through any device connected to the same Wi-Fi network

---

## 📷 Project Demo

### Dashboard Screenshot

Add dashboard screenshot here:

```markdown
![Dashboard Screenshot](<img width="1170" height="638" alt="image" src="https://github.com/user-attachments/assets/a7d99f68-c2d0-45f6-932d-308dd53e19d0" />
)
```

### Demo Video

Add demo video link here:

```markdown
[Watch Demo Video](https://drive.google.com/file/d/1aKiax2_UtdnExWR4m3JKgV4YBN3UU7vP/view?usp=sharing)
```

---

## 🔧 Installation & Setup

### Clone Repository

```bash
git clone https://github.com/Salonichav/health-analytics-dashboard.git
```

### Open Firmware

Open the Arduino source code in Arduino IDE.

### Install Required Libraries

* WiFi
* WebServer
* MAX30105 Library
* heartRate Library

### Upload Firmware

1. Connect ESP32 via USB
2. Select correct COM Port
3. Upload code to ESP32

### Access Dashboard

1. Connect to the same Wi-Fi network
2. Open ESP32 IP Address in browser
3. Monitor BPM and SpO₂ in real time

---

## 📂 Project Structure

```text
health-analytics-dashboard/
│
├── firmware/
│   └── health_dashboard.ino
│
└── README.md
```

---

## 🔮 Future Enhancements

* Patient history storage
* Cloud-based monitoring
* Emergency health alerts
* Mobile application integration
* Multi-patient monitoring support

---

## 👩‍💻 Author

**Saloni Chavan**

GitHub: https://github.com/Salonichav

---

## ⭐ If you found this project interesting, consider giving it a star!
