#define BLYNK_TEMPLATE_ID "TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "AUTHENTICATION_TOKEN"

#include <WiFi.h>
#include <time.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

const char* ssid = "WIFI_SSID/NAME";
const char* password = "WIFI_PASSWORD";

String botToken = "TELEGRAM_BOT_TOKEN";
String chatID = "CHAT_ID"; 

const long gmtOffset_sec = 21600; // GMT +6, 6*3600 = 21,0600 (noted)
const int daylightOffset_sec = 0;

void sendMessage(String message){
  WiFiClientSecure client;
  client.setInsecure(); 

  HTTPClient http;

  message.replace(" ", "%20");
  String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatID + "&text=" + message;
  if (http.begin(client, url)) {
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("Success! Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println("Response: " + payload);
    } else {
      Serial.print("Error sending message. HTTP Code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Unable to connect to Telegram API");
  }
}

void uptime(){
  unsigned long totalSeconds = millis() / 1000;
  
  int minutes = (totalSeconds / 60) % 60;
  int hours = (totalSeconds / 3600) % 24;
  int days = totalSeconds / 86400;

  String uptimeString = String(days) + "d " + String(hours) + "h " + String(minutes) + "m ";
  Blynk.virtualWrite(V0, uptimeString);
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  timer.setInterval(60000, uptime);

  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");
  
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  char timeBuffer[30];
  strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeinfo); // Copy-pasted, tuff lagtese, kokhono sujog hoile bujhe nimunee.

  String message = "ESP32 Online! System has started successfully at " + String(timeBuffer);
  sendMessage(message);
}

void loop() {
  Blynk.run();
  timer.run();
}