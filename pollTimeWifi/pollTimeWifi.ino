/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
#include <Arduino_JSON.h>
ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("fitz_2G", "tz81299895");

}

bool validateOperationTime(String data);
bool validateOperationTime(String data)
{
  // Serial.print("Inside validate: ");
  // Serial.println(data);
  JSONVar locDataWeb = JSON.parse(data);
  if (JSON.typeof(locDataWeb) == "undefined")
  {
    Serial.println("Parsing input failed!");
  }

  if (locDataWeb.hasOwnProperty("datetime"))
  {
    Serial.print("CurrentDatetime: ");
    Serial.println(locDataWeb["datetime"]);
    String datetime = (const char*)locDataWeb["datetime"];
    uint8_t posT = 0;
    uint8_t posColn = 0;
    uint8_t i = 0;
    for (; i < (uint8_t)(sizeof(datetime)); i++)
    {
      if (datetime[i] == 'T')
      {
        posT = i;
      }
      if (datetime[i] == ':')
      {
        posColn = i;
        break;
      }
    }
    char hrs[2];
    hrs[0] = datetime[posT + 1];
    hrs[1] = datetime[posColn - 1];
    uint8_t hrday = (uint8_t)(atoi(hrs));
    Serial.print("hr:");
    Serial.println(hrday);

  }
}


void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://worldtimeapi.org/api/timezone/Asia/Kolkata")) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          validateOperationTime(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  delay(10000);
}
