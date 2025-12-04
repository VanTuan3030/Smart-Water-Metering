# 💧 Thiết Bị Giám Sát và Xác Định Chỉ Số Nước Thông Minh (Smart Water Metering)

An embedded system project focused on designing and implementing a **Smart Water Metering** device using a **Rotary Encoder/Water Flow Sensor**. The system provides real-time water consumption data and smart alerts via a mobile application, addressing the need for accurate and efficient water management.

## 🚀 Key Features

* **Accurate Flow Measurement:** Utilizes a highly sensitive **Cảm biến dò chuyển động quay (Rotary Encoder/Flow Sensor)** to precisely determine instantaneous flow rate and cumulative water volume.
* **Real-time IoT Monitoring:** Data is collected by the microcontroller and transmitted to a cloud platform (specifically **Firebase Realtime Database** or **Cloud Firestore**) for instant access.
* **Smart Alert System:**
    * **Cảnh báo cắt nước (Water Cut Notification):** Sends proactive notifications to the user when no water supply is detected.
    * **Cảnh báo rò rỉ (Leakage Alert):** Can be configured to detect and warn users about continuous, abnormal flow, indicating potential leaks.
* **Mobile Application:** A dedicated **Android App** interface for monitoring data history, current usage, and receiving system notifications (using `Notification Channel`).
* **Remote Data Management:** Supports remote data manipulation, including the ability to **Reset Data** (Clear cumulative usage) directly from the mobile application.

## 🛠️ Hardware Requirements

* **Microcontroller:** `<Vi điều khiển có khả năng kết nối Wi-Fi, ví dụ: ESP32/ESP8266>` - Controls sensors and handles cloud communication.
* **Sensor:** Cảm biến dò chuyển động quay (**Rotary Encoder/Water Flow Sensor**).
* **Connectivity:** Wi-Fi Module (tích hợp trong MCU hoặc module rời).
* **Power & Circuitry:** 5V Power Supply, Filtering Capacitors, and supporting circuitry for the sensor interface.

## 💻 Software & Platform

| Component | Technology / Platform | Function |
| :--- | :--- | :--- |
| **Firmware** | `<Ngôn ngữ C++ trên Arduino IDE>` | Lập trình logic điều khiển, đọc cảm biến và giao tiếp IoT. |
| **Cloud Backend** | **Firebase Realtime Database** | Lưu trữ, đồng bộ hóa dữ liệu tiêu thụ nước theo thời gian thực. |
| **Mobile App** | **Android (Java)** | Giao diện người dùng, hiển thị dữ liệu và quản lý cảnh báo. |
| **Notification** | **Android Notification Channel** (API 26+) | Đảm bảo hệ thống cảnh báo hoạt động hiệu quả. |

## ⚙️ Installation and Usage

1.  **Cloud Setup (Firebase):**
    * Set up a Firebase project and obtain the necessary configuration files (e.g., `google-services.json`).
    * Configure security rules for the Realtime Database/Firestore.

2.  **Firmware Deployment:**
    * Open the microcontroller code in the appropriate IDE (e.g., Arduino IDE).
    * Update Wi-Fi credentials (`SSID`, `Password`) and Firebase API keys/URLs.
    * Compile and upload the firmware to the device.

3.  **Mobile App Deployment:**
    * Open the Android project in Android Studio.
    * Place the `google-services.json` file in the correct app directory.
    * Build and install the application on the mobile device.

4.  **Operation:**
    * The device continuously reads the flow sensor and updates data to Firebase.
    * The Mobile App displays the latest data and alerts the user to events like water cuts or attempts to reset the usage count.

## 👤 Author

* **Student:** Vo Van Tuan
* **Student ID:** 6251020094
* **Class:** Electronics and Telecommunications Engineering (Course 62)
* **University:** University of Transport and Communications - HCMC Campus - UTC2

---
*Project for the Student Scientific Research Final Report (Năm 2025).*
