# ❤️ Real-Time Health & Patient Monitoring System using ESP32 + MAX30102 + FreeRTOS

A lightweight **IoT-based Real-Time Health Monitoring Dashboard** built using **ESP32**, **MAX30102 Pulse Oximeter Sensor**, **IR Patient Detection Sensor**, **FreeRTOS**, and an embedded **Web Dashboard**. The system continuously monitors a patient's presence, finger placement, and heart rate (BPM), and displays the data both on the Serial Monitor and through a browser-based dashboard over Wi-Fi.

---

## 🚀 Features

* 🫀 **Real-time Heart Rate (BPM) Monitoring**
* 👤 **Patient Presence Detection** using an IR sensor
* 👉 **Finger Detection** before BPM calculation
* 📊 **Automatic Health Status Classification**

  * Measuring
  * Low Heart Rate
  * Normal
  * High Heart Rate
* 🌐 **Live Web Dashboard** hosted directly on the ESP32
* ⚡ **Multitasking using FreeRTOS**
* 🔄 **Software Timer for System Health Monitoring**
* 📨 **Inter-task Communication** using Queues and Semaphores

---

## 🛠️ Hardware Components

| Component                      | Quantity    |
| ------------------------------ | ----------- |
| ESP32 Development Board        | 1           |
| MAX30102 Pulse Oximeter Sensor | 1           |
| IR Obstacle/Proximity Sensor   | 1           |
| Breadboard & Jumper Wires      | As Required |
| USB Cable                      | 1           |

---

## 🏗️ System Architecture

```
                    +--------------------+
                    |    ESP32 Board     |
                    +--------------------+
                      |      |        |
          ------------       |        ------------
         |                   |                   |
         |                   |                   |
+----------------+   +----------------+   +------------------+
|  IR Sensor     |   |   MAX30102     |   |  WiFi Module     |
| (Patient Det.) |   | (Heart Rate)   |   |  Web Dashboard   |
+----------------+   +----------------+   +------------------+
         |                   |                   |
         -----------------------------------------
                           |
                    +----------------+
                    | FreeRTOS Tasks |
                    +----------------+
                           |
          ---------------------------------------
          |           |            |            |
          |           |            |            |
   Patient Task  Heart Task  Display Task  Web Task
```

---

## ⚙️ FreeRTOS Task Design

| Task            | Function                                                  | Core   |
| --------------- | --------------------------------------------------------- | ------ |
| **PatientTask** | Detects whether a patient is present using the IR sensor. | Core 0 |
| **HeartTask**   | Reads MAX30102 sensor data and calculates BPM.            | Core 1 |
| **DisplayTask** | Prints monitoring information to the Serial Monitor.      | Core 0 |
| **WebTask**     | Serves the live web dashboard and handles HTTP requests.  | Core 0 |

### RTOS Objects Used

* ✅ Queue (`bpmQueue`) for sharing BPM data.
* ✅ Mutex (`serialMutex`) for synchronized Serial output.
* ✅ Software Timer (`systemTimer`) for periodic system heartbeat messages.

---

## 🌐 Web Dashboard

The ESP32 hosts a responsive web dashboard accessible through any device connected to the same Wi-Fi network.

### Dashboard Displays:

* ❤️ Current Heart Rate (BPM)
* 👤 Patient Detection Status
* 👉 Finger Detection Status
* 📊 Current Health Status

The dashboard fetches live data every **500 ms** using JavaScript `fetch()` API and updates automatically without refreshing the page.

---

## 🧠 Health Status Logic

| BPM Range         | Status          |
| ----------------- | --------------- |
| No Finger / 0 BPM | Measuring       |
| < 60 BPM          | Low Heart Rate  |
| 60 - 100 BPM      | Normal          |
| > 100 BPM         | High Heart Rate |

---

## 🔌 Circuit Connections

### MAX30102 ↔ ESP32

| MAX30102 Pin | ESP32 Pin |
| ------------ | --------- |
| VIN          | 3.3V      |
| GND          | GND       |
| SDA          | GPIO 21   |
| SCL          | GPIO 22   |

### IR Sensor ↔ ESP32

| IR Sensor Pin | ESP32 Pin |
| ------------- | --------- |
| VCC           | 3.3V      |
| GND           | GND       |
| OUT           | GPIO 27   |

---

## 📦 Required Libraries

Install the following libraries from the Arduino Library Manager:

* `MAX3010x Sensor Library`
* `heartRate.h` (included with SparkFun MAX3010x library)
* `WiFi`
* `WebServer`
* `Wire`

The project also uses the built-in **ESP32 FreeRTOS** framework.

---

## 🚀 Getting Started

1. Clone this repository:

   ```bash
   git clone https://github.com/yourusername/your-repository-name.git
   ```

2. Open the project in **Arduino IDE**.

3. Install all required libraries.

4. Update the Wi-Fi credentials in the source code:

   ```cpp
   const char* ssid = "YOUR_WIFI_NAME";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

5. Upload the code to the ESP32.

6. Open the Serial Monitor (115200 baud) and note the IP address assigned by your router.

7. Open a browser and navigate to:

   ```
   http://<ESP32_IP_ADDRESS>/
   ```

8. Enjoy the live monitoring dashboard!

---

## 📸 Demo

### 🎥 Project Demonstration Video

**====LINK HERE====**

*(Replace the above text with your Google Drive / YouTube demo link.)*

---

## 📷 Screenshots

### Web Dashboard

**====SCREENSHOT HERE====**

### Serial Monitor Output

**====SCREENSHOT HERE====**

---

## 📁 Project Structure

```
📦 RealTime-Health-Monitoring
 ┣ 📜 health_monitor.ino
 ┣ 📜 README.md
 ┗ 📂 images
    ┣ 📷 dashboard.png
    ┗ 📷 serial_output.png
```

---

## 🎯 Future Improvements

* Add SpO₂ (Blood Oxygen) measurement.
* Store patient history in a cloud database.
* Integrate Firebase or MQTT for remote monitoring.
* Add emergency SMS/Email alerts.
* Develop a mobile application for monitoring.

---

## 🧑‍💻 Technologies Used

* Embedded C++
* ESP32
* FreeRTOS
* Arduino Framework
* HTML/CSS/JavaScript
* Wi-Fi Networking
* REST-style HTTP API

---

## 📚 Learning Outcomes

This project demonstrates practical implementation of:

* Real-Time Embedded Systems
* IoT-based Healthcare Applications
* Multitasking with FreeRTOS
* Inter-Task Communication (Queues & Semaphores)
* Sensor Data Acquisition
* Web Server Development on ESP32
* Browser-based Real-Time Dashboards

---

## 🤝 Contributing

Contributions, suggestions, and improvements are welcome! Feel free to fork the repository and create a pull request.

---

## 📄 License

This project is released under the **MIT License** and is intended for educational and research purposes.

---

## ⭐ Support

If you found this project useful, consider giving the repository a **⭐ Star** on GitHub. It helps others discover the project and motivates further development.