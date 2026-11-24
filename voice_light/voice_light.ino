/*
  ESP32 สั่งเปิดปิดไฟด้วยเสียง Google Home / Alexa ผ่าน SinricPro
  K iTech Review (TikTok) · Kham iTech Lab (YouTube / Facebook)

  ⚠️ ไฟบ้าน 220V อันตรายถึงชีวิต ตัดเบรกเกอร์ก่อนต่อทุกครั้ง ถ้าไม่มั่นใจ ใช้ปลั๊กอัจฉริยะแทน

  ต่อสาย (ฝั่งไฟต่ำ) — เหมือนคลิปเปิดปิดไฟจากมือถือ
    รีเลย์ VCC -> 5V / VIN   GND -> GND   IN -> GPIO 26
    ปุ่มกดที่ผนัง (ไม่ใส่ก็ได้) ขาหนึ่ง -> GPIO 27  อีกขา -> GND

  ต่อสาย (ฝั่ง 220V)
    ตัดเฉพาะสายเส้นไฟ L ที่เข้าหลอดไฟ มาต่อที่ขา COM กับ NO ของรีเลย์ สาย N ไม่ต้องตัด

  การทำงาน
    พูด "Hey Google เปิดไฟ" / "Alexa turn on light" -> รีเลย์ติด
    กดปุ่มที่บอร์ด -> สลับเปิด/ปิด และแจ้งสถานะกลับไปที่แอป
    ไฟดับแล้วกลับมา -> ดึงสถานะล่าสุดจาก SinricPro มาใช้

  ไลบรารี (Library Manager): SinricPro  (จะถามให้ติดตั้ง ArduinoJson กับ WebSockets ด้วย กด Install All)
*/

#include <Arduino.h>
#include <WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"

// ---------- ตั้งค่า ----------
#define WIFI_SSID   "ชื่อWiFi"
#define WIFI_PASS   "รหัสWiFi"
#define APP_KEY     "xxxx"   // จากเมนู Credentials ในเว็บ SinricPro
#define APP_SECRET  "xxxx"
#define SWITCH_ID   "xxxx"   // Device ID ของอุปกรณ์แบบ Switch

const int  RELAY_PIN  = 26;
const int  BUTTON_PIN = 27;
const bool RELAY_ACTIVE_LOW = true;   // รีเลย์ส่วนใหญ่ติดเมื่อ IN = LOW ถ้าไฟติดตอนควรดับ เปลี่ยนเป็น false

// ---------- ตัวแปร ----------
bool lightOn = false;
unsigned long lastPress = 0;
bool lastBtn = HIGH;

void setRelay(bool on) {
  lightOn = on;
  digitalWrite(RELAY_PIN, (on ^ RELAY_ACTIVE_LOW) ? HIGH : LOW);
  Serial.printf("[Light] %s\n", on ? "ON" : "OFF");
}

// สั่งจาก Google Home / Alexa / แอป SinricPro
bool onPowerState(const String &deviceId, bool &state) {
  setRelay(state);
  return true;
}

// ปุ่มกดที่บอร์ด
void handleButton() {
  bool b = digitalRead(BUTTON_PIN);
  if (b == LOW && lastBtn == HIGH && millis() - lastPress > 300) {
    lastPress = millis();
    setRelay(!lightOn);
    SinricProSwitch &sw = SinricPro[SWITCH_ID];
    if (!sw.sendPowerStateEvent(lightOn)) Serial.println("[SinricPro] ส่งสถานะไม่ได้ (ยังไม่ต่อเน็ต?)");
  }
  lastBtn = b;
}

void setupWiFi() {
  Serial.print("[WiFi] Connecting");
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    handleButton();          // ระหว่างรอเน็ต ปุ่มยังใช้ได้
    delay(50);
    static int n = 0;
    if (++n % 10 == 0) Serial.print(".");
  }
  Serial.printf("\n[WiFi] IP %s\n", WiFi.localIP().toString().c_str());
}

void setupSinricPro() {
  SinricProSwitch &sw = SinricPro[SWITCH_ID];
  sw.onPowerState(onPowerState);
  SinricPro.onConnected([]() { Serial.println("[SinricPro] Connected"); });
  SinricPro.onDisconnected([]() { Serial.println("[SinricPro] Disconnected"); });
  SinricPro.restoreDeviceStates(true);   // ไฟดับแล้วกลับมา ใช้สถานะล่าสุด
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  setRelay(false);                       // เริ่มต้นปิดไว้ก่อน
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
  handleButton();
}
