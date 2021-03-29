#include <ArduinoOTA.h>
#include <WiFiUdp.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

ESP8266WebServer server;
uint8_t pin_led = 2;
uint8_t pin_led1 = 16;
uint8_t pin_led2 = 14;
uint8_t pin_led3 = 15;
char* ssid = "----------";
char* password = "----------";

void setup()
{
  
  lcd.begin(16,2);
  lcd.init();

 
  lcd.backlight();

  
  lcd.setCursor(5, 0);

  
  lcd.print("Welcome!");
  SPIFFS.begin();
  pinMode(pin_led, OUTPUT);
  pinMode(pin_led1, OUTPUT);
  pinMode(pin_led2, OUTPUT);
  pinMode(pin_led3, OUTPUT);
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
 
  Serial.println(WiFi.localIP());
  lcd.setCursor(5, 0);

  
  lcd.print("IP Address");
  lcd.setCursor(5, 1);
  lcd.print(WiFi.localIP());

  server.on("/",serveIndexFile);
  server.on("/ledstate",getLEDState);
  server.on("/ledstate1",getLEDState1);
  server.on("/ledstate2",getLEDState2);
  server.on("/ledstate3",getLEDState3);
  server.begin();
   ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}

void loop()
{
  server.handleClient();
  ArduinoOTA.handle();
}

void serveIndexFile()
{
  File file= SPIFFS.open("/index.html","r");
  server.streamFile(file, "text/html");
  file.close();
}

void toggleLED(int a)
{
  digitalWrite(a,!digitalRead(a));
}

void getLEDState()
{
  toggleLED(pin_led);
  String led_state = digitalRead(pin_led) ? "OFF" : "ON";
  server.send(200,"text/plain", led_state);
}
void getLEDState1()
{
  toggleLED(pin_led1);
  String led_state = digitalRead(pin_led1) ? "OFF" : "ON";
  server.send(200,"text/plain", led_state);
}
void getLEDState2()
{
  toggleLED(pin_led2);
  String led_state = digitalRead(pin_led2) ? "OFF" : "ON";
  server.send(200,"text/plain", led_state);
}
void getLEDState3()
{
  toggleLED(pin_led3);
  String led_state = digitalRead(pin_led3) ? "OFF" : "ON";
  server.send(200,"text/plain", led_state);
}
