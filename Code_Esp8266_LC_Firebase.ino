#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// --- CẤU HÌNH KẾT NỐI ---
#define FIREBASE_HOST "**************"
#define FIREBASE_AUTH "**************"
#define WIFI_SSID "**************"
#define WIFI_PASSWORD "**************"

// --- KHAI BÁO BIẾN TOÀN CỤC ---
FirebaseData firebaseData;
WiFiUDP ntpUDP;
// Đặt múi giờ cho Việt Nam (GMT+7), 7 * 3600 = 25200
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200);

String dailyCountPath = "/Day";
String totalCountPath = "/Month";
String lastIntervalPath = "/Time";

const byte pin_pulse = 14; // D2 (Dùng cho cảm biến lưu lượng nước)
const byte pin_cap = A0; // (Dùng cho cảm biến LC)
const byte pin_LED = 12; // D6

// --- CẤU HÌNH THAM SỐ KHÁC ---
const byte npulse = 15;
const int DETECTION_THRESHOLD = 300;
const int nmeas = 384;
const unsigned long DETECTION_COOLDOWN = 800; // Thời gian chờ giữa các lần phát hiện (ms)

long int metalDetectedCount = 0;
long lastResetInterval = -1;
bool metalPreviouslyDetected = false;
bool boQuaLanDau = true;
long int sumsum = 0;
long int diff = 0;


// --- HÀM NGẮT (INTERRUPT SERVICE ROUTINE) ---

// Hàm ngắt để đếm xung từ cảm biến lưu lượng nước
void IRAM_ATTR pulseCounter() {
  if (boQuaLanDau) {
    boQuaLanDau = false;
    return;
  }
  metalDetectedCount++;
}


// --- HÀM HỖ TRỢ ---

// Lấy giá trị từ Firebase (cũng là hàm reset logic ban đầu)
void getFirebaseData() {
  // Lấy giá trị đếm ngày
  if (Firebase.getInt(firebaseData, dailyCountPath)) {
    long int fbDay = firebaseData.intData();
    if (fbDay >= 0) {
      metalDetectedCount = fbDay;
    }
  }

  // Lấy thời gian reset cuối cùng
  if (Firebase.getInt(firebaseData, lastIntervalPath)) {
    lastResetInterval = firebaseData.intData();
  }
}

// Xử lý logic và gửi dữ liệu lên Firebase
void sendDataToFirebase(long int count, long int total) {
  // Gửi trạng thái hệ thống
  String status = "Bình thường";
  // Giả định: Tắt nước nếu không có xung sau 1 phút
  if (count == 0 && total > 0 && (millis() - lastResetInterval > 60000)) {
    status = "Tắt nước"; 
  }
  Firebase.setString(firebaseData, "/Statu", status);

  // Cập nhật số lượng đếm và tổng
  Firebase.setInt(firebaseData, dailyCountPath, count);
  Firebase.setInt(firebaseData, totalCountPath, total);

  // Cập nhật thời gian gửi
  Firebase.setString(firebaseData, "/TimeSend", timeClient.getFormattedTime());

  Serial.printf("Gửi dữ liệu: Day=%ld, Month=%ld, Status=%s\n", count, total, status.c_str());
}

// Đo điện dung (cho cảm biến LC)
long int readCapacitance() {
  long int sumCap = 0;
  for (int i = 0; i < nmeas; i++) {
    sumCap += analogRead(pin_cap);
    delay(1);
  }
  return sumCap / nmeas;
}


// --- HÀM SETUP ---

void setup() {
  Serial.begin(115200);
  pinMode(pin_pulse, INPUT_PULLUP);
  pinMode(pin_LED, OUTPUT);
  digitalWrite(pin_LED, LOW);

  // Kết nối WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Khởi tạo Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  // Khởi tạo NTP Client
  timeClient.begin();
  timeClient.update();

  // Đọc dữ liệu ban đầu từ Firebase
  getFirebaseData();

  // Thiết lập ngắt cho chân cảm biến lưu lượng nước
  attachInterrupt(digitalPinToInterrupt(pin_pulse), pulseCounter, FALLING);
}


// --- HÀM LOOP ---

void loop() {
  // Cập nhật thời gian
  timeClient.update();

  // Cập nhật giá trị đếm
  getFirebaseData();

  // Gửi dữ liệu định kỳ (mỗi 10 giây)
  static unsigned long lastSendTime = 0;
  if (millis() - lastSendTime >= 10000) {
    lastSendTime = millis();

    // Đo cảm biến LC (phần này bị chú thích/không sử dụng trong logic gửi data)
    // long int cap = readCapacitance();
    // if (abs(cap - sumsum) > DETECTION_THRESHOLD) {
    //   metalDetectedCount++;
    // }

    // Tính toán tổng số (totalCount)
    // Trong code này, totalCount được gán bằng metalDetectedCount, 
    // có thể hiểu là tổng số lần đếm hiện tại.
    long int totalCount = metalDetectedCount; 

    // Cập nhật trạng thái đèn LED
    digitalWrite(pin_LED, LOW);

    // Gửi dữ liệu
    sendDataToFirebase(metalDetectedCount, totalCount);

    // Cập nhật lại thời gian reset cuối cùng
    Firebase.setInt(firebaseData, lastIntervalPath, millis());
  }

  // Delay ngắn
  delay(100);
}