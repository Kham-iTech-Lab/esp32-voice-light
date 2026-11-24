# ESP32 สั่งเปิดปิดไฟด้วยเสียง Google Home / Alexa 🎙️💡

พูด "Hey Google เปิดไฟ" หรือ "Alexa turn on light" แล้วไฟติด ใช้ ESP32 กับรีเลย์ตัวเดิมจากคลิปเปิดปิดไฟจากมือถือ ต่อผ่านบริการฟรี SinricPro

จากคลิป **K iTech Review** (TikTok) · **Kham iTech Lab** (YouTube / Facebook)

## ⚠️ อ่านก่อน

ไฟบ้าน 220V **อันตรายถึงชีวิต**
- ตัดเบรกเกอร์ก่อนต่อสายทุกครั้ง
- เก็บจุดต่อทั้งหมดไว้ในกล่องพักสาย
- ถ้าไม่มั่นใจเรื่องไฟฟ้า ใช้ปลั๊กอัจฉริยะสำเร็จรูป หรือให้ช่างไฟต่อฝั่ง 220V ให้

## ของที่ใช้

| ชิ้น | หมายเหตุ |
|---|---|
| บอร์ด ESP32 | รุ่นไหนก็ได้ |
| โมดูลรีเลย์ 1 ช่อง | ต้องรับได้ 250VAC 10A |
| ปุ่มกด (ไม่ใส่ก็ได้) | ไว้กดเปิด/ปิดเองที่ผนัง |
| อะแดปเตอร์ 5V | เลี้ยงบอร์ดกับรีเลย์ |
| ลำโพง Google Home / Alexa หรือแอปในมือถือ | |

## ต่อสาย

| อุปกรณ์ | ขา | ต่อเข้า |
|---|---|---|
| รีเลย์ | VCC | 5V / VIN |
| | GND | GND |
| | IN | GPIO 26 |
| ปุ่มกด | ขาหนึ่ง | GPIO 27 |
| | อีกขา | GND |

**ฝั่ง 220V** — ตัดเฉพาะสายเส้นไฟ **L** ที่เข้าหลอดไฟ มาต่อที่ขา **COM** กับ **NO** ของรีเลย์ สาย N ไม่ต้องตัด

## ขั้นที่ 1 — สร้างอุปกรณ์ใน SinricPro

1. สมัครฟรีที่ [sinric.pro](https://sinric.pro) (ฟรี 2 อุปกรณ์)
2. เมนู **Devices** → **Add Device** → Device Type เลือก **Switch** ตั้งชื่อเช่น "ไฟห้องนอน" (ชื่อนี้คือคำที่ใช้สั่งด้วยเสียง)
3. จะได้ **Device ID** และในเมนู **Credentials** จะมี **APP KEY** กับ **APP SECRET**

## ขั้นที่ 2 — ใส่รหัสในโค้ด แล้วอัปโหลด

1. Arduino IDE ติดตั้งบอร์ด ESP32 (Boards Manager → esp32 by Espressif)
2. Library Manager ค้น **SinricPro** กด Install ถ้าถามให้ติดตั้ง ArduinoJson กับ WebSockets ด้วย กด **Install All**
3. เปิด `voice_light/voice_light.ino` ใส่ `WIFI_SSID`, `WIFI_PASS`, `APP_KEY`, `APP_SECRET`, `SWITCH_ID`
4. อัปโหลด เปิด Serial Monitor 115200 ต้องเห็น `[SinricPro] Connected`
5. ลองกดสวิตช์ในเว็บ/แอป SinricPro ก่อน ไฟต้องติดดับตาม

## ขั้นที่ 3 — เชื่อม Google Home / Alexa

**Google Home**: แอป Google Home → **+** → ตั้งค่าอุปกรณ์ → **Works with Google** → ค้น **SinricPro** → ล็อกอินบัญชีเดียวกัน → อุปกรณ์จะขึ้นมาเอง

**Alexa**: แอป Alexa → Skills → ค้น **SinricPro** → Enable → ล็อกอิน → Discover Devices

แล้วสั่งได้เลย เช่น "Hey Google เปิดไฟห้องนอน"

## แก้ปัญหา

- **ไฟติดตอนควรดับ** — เปลี่ยน `RELAY_ACTIVE_LOW` เป็น `false`
- **ไม่ขึ้น Connected** — เช็ค APP KEY / SECRET ว่าก๊อปครบ ไม่มีช่องว่าง และ ESP32 ใช้ Wi-Fi 2.4GHz เท่านั้น
- **Google หาอุปกรณ์ไม่เจอ** — พูด "Hey Google sync my devices" หรือ unlink แล้ว link SinricPro ใหม่
- **เน็ตหลุด** — ปุ่มกดที่บอร์ดยังเปิด/ปิดไฟได้ตามปกติ
