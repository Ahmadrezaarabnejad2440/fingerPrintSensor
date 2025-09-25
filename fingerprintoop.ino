#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(D7, D8);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

class fingerPrint {
public:
  void begin() {
    finger.begin(57600);
    finger.verifyPassword();
  }

  bool runFingerprint(String request = "test 0") {
    // Define a string
    String str = request;

    // Convert the string to a character array
    char charArray[str.length() + 1];  // +1 for null terminator
    str.toCharArray(charArray, str.length() + 1);

    // Define an array to store individual words
    const int maxWords = 2;  // Maximum number of words
    char* req[maxWords];    // Array of pointers to char arrays
    int wordCount = 0;        // Counter for the number of words

    // Split the string into words based on space delimiter
    char* token = strtok(charArray, " ");
    while (token != NULL && wordCount < maxWords) {
      req[wordCount] = token;
      wordCount++;
      token = strtok(NULL, " ");
    }

    String req1 = String(req[0]);
    int req2 = atoi(req[1]);

    if (req1 == "add") {
      this->enrollFingerprint(req2);
    } else if (req1 == "remove") {
      this->deleteFingerprint(req2);
    } else if (req1 == "get") {
      this->getFingerprintID();
    } else if (req1 == "list") {
      this->getFingerprintList();
    } else if (req1 == "removeAll") {
      this->deleteAllFingerprints();
    } else if (req1 == "test") {
      this->recognitionFingerprint();
    } else {
      Serial.println(request);
      Serial.println(req[0]);
      Serial.println("Error");
      return false;
    }
    return true;
  }


private:
  int p = -2;

  uint8_t enrollFingerprint(uint8_t fingerID) {
    uint8_t id = fingerID;
    int p = -1;
    Serial.println("انگشتی را بر روی سنسور قرار دهید");
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
        case FINGERPRINT_OK:
          break;
        case FINGERPRINT_NOFINGER:
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("خطا در ارتباط با سنسور");
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("تصویر نا معتبر است");
          break;
        default:
          Serial.println("خطای نامشخص");
          break;
      }
    }

    // OK success!

    p = finger.image2Tz(1);
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("تبدیل تصویر به قالب قابل استفاده با موفقیت انجام شد");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("تصویر نامشخص است");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("خطا در ارتباط با سنسور");
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("عدم تطابق ویژگی‌های تصویر");
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("تصویر نامعتبر است");
        return p;
      default:
        Serial.println("خطای نامشخص");
        return p;
    }

    Serial.println("انگشت خود را برداشته و دوباره قرار دهید.");
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER) {
      p = finger.getImage();
    }
    p = -1;
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
        case FINGERPRINT_OK:
          break;
        case FINGERPRINT_NOFINGER:
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("خطا در ارتباط با سنسور");
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("تصویر نا معتبر است");
          break;
        default:
          Serial.println("خطای نامشخص");
          break;
      }
    }

    // OK success!

    p = finger.image2Tz(2);
    switch (p) {
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("تصویر نامشخص است");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("خطا در ارتباط با سنسور");
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("عدم تطابق ویژگی‌های تصویر");
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("تصویر نامعتبر است");
        return p;
      default:
        Serial.println("خطای نامشخص");
        return p;
    }


    // OK converted!
    Serial.print("ساخت مدل یرای ذخیره اثر انگشت");

    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
      Serial.println("مدل اثر انگشت با موفقیت ثبت شد");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("خطا در ارتباط با سنسور");
      return p;
    } else if (p == FINGERPRINT_ENROLLMISMATCH) {
      Serial.println("عدم تطابق اثر انگشت");
      return p;
    } else {
      Serial.println("خطای نامشخص");
      return p;
    }

    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK) {
      Serial.println("اثر انگشت با موفقیت ذخیره شد");
      return p;
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("خطا در ارتباط با سنسور");
      return p;
    } else if (p == FINGERPRINT_BADLOCATION) {
      Serial.println("مکان ذخیره سازی نامعتبر است");
      return p;
    } else if (p == FINGERPRINT_FLASHERR) {
      Serial.println("خطا در ذخیره سازی داده‌ها در حافظه");
      return p;
    } else {
      Serial.println("خطای نامشخص");
      return p;
    }
  }
  void deleteFingerprint(uint8_t id) {
    if (!finger.deleteModel(id)) {
      Serial.println("اثر انگشت با موفقیت حذف شد.");
    } else {
      Serial.println("خطا در حذف اثر انگشت");
    }
  }

  void deleteAllFingerprints() {
    for (int i = 0; i <= 10; i++) {
      finger.deleteModel(i);
    }
    if (!finger.deleteModel(1)) {
      Serial.println("اثر انگشت ها با موفقیت حذف شدند.");
    } else {
      Serial.println("خطا در حذف اثر انگشت ها");
    }
  }

  void recognitionFingerprint() {
    Serial.println("انگشتی را بر روی سنسور قرار دهید");
    while (true) {
      int p = finger.getImage();
      if (p != FINGERPRINT_OK) {
      } else {
        break;
      }
    }

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) {
      Serial.println("تصویر نامشخص است");
    }

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK) {
      Serial.println("!!اثر انگشت پیدا نشد");
    } else {
      Serial.println("+اثر انگشت پیدا شد+");
    }
  }

  void getFingerprintID() {
    int p = -1;
    Serial.println("انگشتی را بر روی سنسور قرار دهید");
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
    }

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) {
      Serial.println("تصویر نامشخص است");
    }


    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK) {
      Serial.println("اثر انگشت یافت نشد");
    } else {
      Serial.print("شناسه اثر انگشت : ");
      Serial.println(finger.fingerID);
    }
  }

  void getFingerprintList() {
    uint8_t fingerprintCount = 0;
    Serial.println("اثر انگشت‌های ثبت شده:");
    for (uint8_t i = 0; i <= 11; i++) {
      if (!finger.loadModel(i)) {
        Serial.print("شناسه: ");
        Serial.println(i);
        fingerprintCount += 1;
      }
    }
    Serial.print("تعداد اثر انگشت های ثبت شده : ");
    Serial.println(fingerprintCount);
  }
};

fingerPrint fingerprintSensor;

void setup() {
  finger.begin(57600);
  Serial.begin(9600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
}

void loop() {
  if (Serial.available()) {
    String request = Serial.readString();
    fingerprintSensor.runFingerprint(request);
  }
  /*   
  add ID ثبت اثر انگشت
  remove ID حذف اثر انگشت
  get تشخیص ایدی اثر انگشت
  list لیست اثر انگشت ها
  removeAll حذف همه ی اثر انگشت ها
  test تست اثر انگشت
  */
}
