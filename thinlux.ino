#include <Wire.h>
#include <BH1750.h>
#include <ThingerESP32.h>

#define USERNAME  "lyaelectronic"

/* #define DEVICE_ID "TEST1_AITANA"
#define DEVICE_CREDENTIAL "MrG&tVjd4q0u"
#define Bucket "TEST0" */

#define DEVICE_ID "TEST2_AITANA"
#define DEVICE_CREDENTIAL "cZkdwqEfHOCt"
#define Bucket "TEST0_2"

 #define SSID "LYA LAB"
#define SSID_PASSWORD "LYA devices"

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
BH1750 lightMeter(0x23);
float lux;
unsigned long report, lect;
int n = 0;
void setup() {
    Serial.begin(115200);
    Wire.begin();
    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        Serial.println(F("BH1750 Advanced begin"));
    }
    else {
        Serial.println(F("Error initialising BH1750"));
  }
    thing.add_wifi(SSID, SSID_PASSWORD);
    thing["sensor"] >> [](pson& out){
        out["Lux"] = lux;
    };
}

void loop() {
    thing.handle();
    if (millis() - lect > 30000){
        lux = lightMeter.readLightLevel();
        Serial.print("Light: ");
        Serial.print(lux);
        Serial.println(" lx");
        lect = millis();
    }
    if(millis() - report > toMillis(n)) {
        if (thing.is_connected()) {
            thing.write_bucket(Bucket,"sensor");
            delay(500);
            Serial.println("Enviado");
            n = 5;
            report = millis();
        } else {
            n = 2;
            report = millis();
        } 
    }
}

unsigned long toMillis(int tosminu){
  return tosminu*60000;
}