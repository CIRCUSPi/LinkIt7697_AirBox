//Generated Date: Tue, 21 Nov 2023 08:08:44 GMT

#include "Wire.h"
#include "U8g2lib.h"
#include <LWiFi.h>
#include <PubSubClient.h>
#define MQTT_SERVER_IP "io.adafruit.com"
#define MQTT_SERVER_PORT 1883
#define MQTT_ID "0123"
#define MQTT_USERNAME "username_on_adafruit"
#define MQTT_PASSWORD "pass_key_on_adafruit"
#include <PMS.h>
#include <SoftwareSerial.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
char _lwifi_ssid[] = "YOUR_SSID";
char _lwifi_pass[] = "YOUR_PASSWORD";
String receivedTopic="";
String receivedMsg="";
bool waitForE=true;
bool ended=true;
bool pubCtrl=false;

WiFiClient mqttClient;
PubSubClient myClient(mqttClient);

SoftwareSerial pmsSerial(2, 0);
PMS pms(pmsSerial);
PMS::DATA pmsData;

int pms_read_PM1()
{
  pms.read(pmsData);
  return pmsData.PM_AE_UG_1_0;
}

int pms_read_PM2()
{
  pms.read(pmsData);
  return pmsData.PM_AE_UG_2_5;
}

int pms_read_PM10()
{
  pms.read(pmsData);
  return pmsData.PM_AE_UG_10_0;
}

void connectMQTT(){
  while (!myClient.connected()){
    if (!myClient.connect(MQTT_ID,MQTT_USERNAME,MQTT_PASSWORD))
    {
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length){
  receivedTopic=String(topic);
  receivedMsg="";
  for (unsigned int myIndex = 0; myIndex < length; myIndex++)
  {
    receivedMsg += (char)payload[myIndex];
  }
  receivedMsg.trim();

}

void setup()
{
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);

  myClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  myClient.setCallback(mqttCallback);

  u8g2.clearDisplay();
  while (WiFi.begin(_lwifi_ssid, _lwifi_pass) != WL_CONNECTED) { delay(1000); }
  connectMQTT();
  pmsSerial.begin(9600);

}

void loop()
{
  myClient.loop();
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 0);
    u8g2.print(String(String("PM2.5　：")+String(pms_read_PM2())).c_str());
    u8g2.setCursor(0, 16);
    u8g2.print(String(String("PM1.0 :")+String(pms_read_PM1())).c_str());

    u8g2.sendBuffer();
  } while (u8g2.nextPage());
  myClient.publish(String("ICSHOP/feeds/YOUR_TOPIC").c_str(),String(String(pms_read_PM2())).c_str());
  myClient.publish(String("ICSHOP/feeds/YOUR_TOPIC").c_str(),String(String(pms_read_PM1())).c_str());
  delay(5000);
}
