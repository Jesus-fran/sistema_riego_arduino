#include <Arduino.h>
// void setup()
// {

// }

// void loop()
// {

// }

#include <ArduinoJson.h>
#include "SoftwareSerial.h"
SoftwareSerial serialEsp(2, 3); // rx tx
char data;
#define pinLED 13

const int pinHumedad = A3;
const int pinTemperatura = A0;

void setup()
{
  Serial.begin(9600);
  serialEsp.begin(115200);
  pinMode(pinTemperatura, INPUT);
  pinMode(pinHumedad, INPUT);
}

void loop()
{

  if (serialEsp.available() > 0)
  {
    String data = serialEsp.readStringUntil('\n');
    serialEsp.flush();
    if (data == "conectado")
    {
      Serial.println("esp dice conectado");
      serialEsp.print("como estas");
    }
    else if (data == "bien")
    {
      Serial.println("OK");
    }
    else if (data == "data")
    {
      StaticJsonDocument<300> doc;
      int lectTemperatura = analogRead(pinTemperatura);
      float conversionTemp = (lectTemperatura / 1024.0) * 5000;
      float temperatura = conversionTemp / 10;
      doc["temp"] = temperatura;
      delay(1000);

      int lectHumedad = analogRead(pinHumedad);
      doc["hum"] = lectHumedad;
      String json;
      serializeJson(doc, json);
      serialEsp.println(json);
    }
    else
    {
      Serial.println("");
      Serial.println(data);
    }
  }

  delay(1000);
}
