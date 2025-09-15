#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>


SoftwareSerial fingerprintSerial(D7, D8);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerprintSerial);

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  getRegisteredFingerprints();
}

void loop() {
  // دستورات عملکردی دیگر در اینجا
}

void getRegisteredFingerprints() {
  uint8_t fingerprintCount = 0;
  Serial.println("اثر انگشت‌های ثبت شده:");
  for (uint8_t i = 0; i <= 127; i++) {
    if (!finger.loadModel(i)) {
      Serial.print("شناسه: ");
      Serial.println(i);
      fingerprintCount += 1;
    }
  }
  Serial.print("تعداد اثر انگشت های ثبت شده : ");
  Serial.println(fingerprintCount);
}