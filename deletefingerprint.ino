#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(D7, D8);  // TX/RX

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

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


  for (int i = 0; i <= 127; i++) {
    deleteFingerprint(i);
    Serial.print(i);
    delay(100);
  }
}

void loop() {
  // دستورات عملکردی دیگر در اینجا
}

void deleteFingerprint(uint8_t id) {
  if (!finger.deleteModel(id)) {
    Serial.println("اثر انگشت با موفقیت حذف شد.");
  } else {
    Serial.println("خطا در حذف اثر انگشت!");
  }
}
