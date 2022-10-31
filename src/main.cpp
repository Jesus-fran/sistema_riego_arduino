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
#define pinLED 13
const int pinReleValvula = 6;
const int pinHumedad = A3;
const int pinTemperatura = A0;
unsigned long interval = 1000;
unsigned long interval_valvula = 10000;
unsigned long previous_milis;
unsigned long previous_milis_valvula;
bool valvula = false;

void setup()
{
  Serial.begin(9600);
  serialEsp.begin(57600);
  pinMode(pinReleValvula, OUTPUT);
  digitalWrite(pinReleValvula, HIGH);
  pinMode(pinTemperatura, INPUT);
  pinMode(pinHumedad, INPUT);
  previous_milis = millis();
  previous_milis_valvula = previous_milis;
}

void loop()
{
  unsigned long current_millis = millis();

  if ((unsigned long)(current_millis - previous_milis) >= interval)
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
      else if (data == "ONVAL")
      {
        if (valvula == false)
        {
          Serial.print("Regando...");
          digitalWrite(pinReleValvula, LOW);
          valvula = true;
          previous_milis_valvula = millis();
        }
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
    previous_milis = millis();
  }

  // if ((unsigned long)(current_millis - previous_milis_valvula) >= interval_valvula)
  // {
  //   if (valvula)
  //   {
  //     digitalWrite(pinReleValvula, HIGH);
  //     valvula = false;
  //     serialEsp.print("OFFVAL");
  //   }
  // }
}