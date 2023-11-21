//Generated Date: Tue, 21 Nov 2023 08:07:52 GMT

#include "Wire.h"
#include "U8g2lib.h"
#include <SoftwareSerial.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
SoftwareSerial pms5003tSerial(2, 3);
long pmat10_ = 0;
long pmat25_ = 0;
long pmat100_ = 0;
long Temp_ = 0;
long Humid_ = 0;
char buf[50];

void retrievepm25(){
  int count = 0;
  unsigned char c;
  unsigned char high;
  while (pms5003tSerial.available()) {
     c = pms5003tSerial.read();
     if((count==0 && c!=0x42) || (count==1 && c!=0x4d)){
       break;
     }
     if(count > 27){
       break;
     }
      else if(count == 10 || count == 12 || count == 14 || count == 24 || count == 26) {
       high = c;
     }
     else if(count == 11){
       pmat10_ = 256*high + c;
     }
     else if(count == 13){
       pmat25_ = 256*high + c;
     }
     else if(count == 15){
       pmat100_ = 256*high + c;
     }
      else if(count == 25){
          Temp_ = (256*high + c)/10;
     }
     else if(count == 27){
                  Humid_ = (256*high + c)/10;
    }       count++;
  }  while(pms5003tSerial.available()) pms5003tSerial.read();
}

void setup()
{
  Serial.begin(9600);

  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);


  pms5003tSerial.begin(9600);

}

void loop()
{
  retrievepm25();
  u8g2.clearDisplay();
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 0);
    u8g2.print(String(String("PM1.0: ")+String(pmat10_)).c_str());
    u8g2.setCursor(0, 10);
    u8g2.print(String(String("PM2.5: ")+String(pmat25_)).c_str());
    u8g2.setCursor(0, 22);
    u8g2.print(String(String("PM10 : ")+String(pmat100_)).c_str());
    u8g2.setCursor(0, 33);
    u8g2.print(String(String("Temp : ")+String(Temp_)).c_str());
    u8g2.setCursor(0, 44);
    u8g2.print(String(String("Humid: ")+String(Humid_)).c_str());

    u8g2.sendBuffer();
  } while (u8g2.nextPage());
  Serial.println((String("PM1.0: ")+String(pmat10_)));
  Serial.println((String("PM2.5: ")+String(pmat25_)));
  Serial.println((String("PM10 : ")+String(pmat100_)));
  Serial.println((String("Temp : ")+String(Temp_)));
  Serial.println((String("Humid: ")+String(Humid_)));
  delay(1000);
}
