#include <SPI.h>
#include <LoRa.h>
#include "ds3231.h"
#include <WiFi.h>
//#include <SD.h>
OLED_CLASS_OBJ display(OLED_ADDRESS, OLED_SDA, OLED_SCL);
#define WIFI_SSID       "BeBoX_Home"
#define WIFI_PASSWORD   "8201000501"
int incomingByte = 0; // for serial recieve

void setup()
{
    Serial.begin(115200);
    while (!Serial);
    if (OLED_RST > 0) {
        pinMode(OLED_RST, OUTPUT);
        digitalWrite(OLED_RST, HIGH);
        delay(100);
        digitalWrite(OLED_RST, LOW);
        delay(100);
        digitalWrite(OLED_RST, HIGH);
    }
    display.init();
    display.flipScreenVertically();
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, LORA_SENDER ? "LoRa Sender" : "LoRa Receiver");
    display.display();
    delay(2000);
    String info = ds3231_test();
    if (info != "") {
        display.clear();
        display.setFont(ArialMT_Plain_16);
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, info);
        display.display();
        delay(2000);
    }
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        display.clear();
        Serial.println("WiFi Connect Fail");
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "WiFi Connect Fail");
        display.display();
        delay(2000);
        esp_restart();
    }
    Serial.print("Connected : ");
    Serial.println(WiFi.SSID());
    Serial.print("IP:");
    Serial.println(WiFi.localIP().toString());
    display.clear();
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, "IP:" + WiFi.localIP().toString());
    display.display();
    delay(2000);

    SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
    LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
    if (!LoRa.begin(915E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    if (!LORA_SENDER) {
        display.clear();
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "LoraRecv Ready");
        display.display();
    }
}

int count = 0;

void loop()
{
#if LORA_SENDER
    int32_t rssi;
    String test = "";
    while (Serial.available() > 0) {
      incomingByte = Serial.read();          
      test += String(char(incomingByte));   
    }
    Serial.print("I received: ");
    Serial.println(test);
    if (WiFi.status() == WL_CONNECTED) {
        rssi = WiFi.RSSI();
        display.clear();
        display.setFont(ArialMT_Plain_10);
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.drawStringMaxWidth(0, 0, 128, "Send : \r\n" + test);
        //display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Send RSSI:" + String(rssi));
        //display.drawString(display.getWidth() / 2, 0, "Send:\r\n" + test);
        display.display();
        LoRa.beginPacket();
        //LoRa.print("WiFi RSSI: ");
        //LoRa.print(rssi);
        LoRa.print(test);
        LoRa.endPacket();
    } else {
        Serial.println("WiFi Connect lost ...");
    }
    delay(2500);
#else
    if (LoRa.parsePacket()) {
        String recv = "";
        while (LoRa.available()) {
            recv += (char)LoRa.read();
        }
        count++;
        display.clear();
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, recv);
        String info = "[" + String(count) + "]" + "RSSI " + String(LoRa.packetRssi());
        display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 16, info);
        display.display();
    }
#endif
}
