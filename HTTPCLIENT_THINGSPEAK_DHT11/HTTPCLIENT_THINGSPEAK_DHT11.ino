/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */


#include "DHT.h"
#define Type DHT11
int sensePin =2;
DHT HT (sensePin,Type);
float humidity;
float temperature;
int setTime=800; 
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
long myChannelNumber =1854874;

WiFiMulti wifiMulti;

String Write_API_Key = "6UL3212PAB7CWPC8";
int Sensor_Reading ;
void setup() {

    Serial.begin(115200);
    HT.begin();
    delay(setTime);
    Serial.println();
    Serial.println();
    Serial.println();

    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    wifiMulti.addAP("SmS_jiofi", "sms123458956");

}

void loop() {
    // wait for WiFi connection
    Sensor_Reading = hallRead();
    humidity =HT.readHumidity();
    temperature =HT.readTemperature();
   Serial.print ("HALL SENSOR VALUE IS: ");
   Serial.println (Sensor_Reading);
   Serial.print ("TEMPERATURE SENSOR VALUE IS: ");
   Serial.println (temperature);
   Serial.print ("HUMIDITY SENSOR VALUE IS: ");
   Serial.println (humidity); 
    if((wifiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        http.begin("https://api.thingspeak.com/update?api_key=" + Write_API_Key + "&field1=" +Sensor_Reading + "&field2=" +temperature + "&field3=" +humidity ); //HTTP

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(5000);
}
