#define key2         0   //中间的按键，也是烧录按键
#define key3         5   //右边的按键，进入默写模式的按键
#include <U8g2_for_Adafruit_GFX.h>
#include <GxEPD2_BW.h>
#include "gb2312.c"
#include "img.c"

#include "FS.h"
#include <EEPROM.h>
#define RST_PIN 0 // D3(0)
//GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEM029T94
GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=*/ 16, /*DC=D3*/ 0, /*RST=*/ -1, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS*/ 15, /*DC*/ 0, /*RST*/ 2, /*BUSY*/ 4));
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
int dis_count = 0; //判断局刷或全刷
bool f_word = false;
bool flag = true;
#include "displaycode.h"
File story_dataFile;//小说
extern const uint8_t chinese_city_gb2312[239032] U8G2_FONT_SECTION("chinese_city_gb2312");

/*
   8266接线方式：
   busy--D2 4
   res--D4 2
   dc--D3 0
   cs--D8 15
   clk--D5 14
   din--D7 13
   gnd--g
   vcc--3v3
*/
int x = 10, y = 55;
bool read_state = false; //标记阅读/默写模式

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(key2, INPUT_PULLUP); //INPUT_PULLUP
  pinMode(key3, INPUT_PULLUP); //INPUT_PULLUP
  Serial.println("setup");
  digitalWrite(RST_PIN, HIGH);
  pinMode(RST_PIN, OUTPUT);
  delay(20);
  digitalWrite(RST_PIN, LOW);
  delay(20);
  digitalWrite(RST_PIN, HIGH);
  delay(200);
  display.init();
  EEPROM.begin(64);
  /*
     第一次运行需要取消下面4行已防止EEPROM中的原有数据影响词条位置
     传完之后注释掉下面4行，再次上传就可以正常运行了。
  */
  int f1 = EEPROM.read(0);
  int f2 = EEPROM.read(4);
  if (f1 == f2 && f1 == 255) {
    EEPROM.write(0, 0);
    EEPROM.commit();
    EEPROM.write(4, 0);
    EEPROM.commit();
  }
  u8g2Fonts.begin(display);//将u8g2连接到display
  display.setRotation(3);
  display.firstPage();
  display.fillScreen(GxEPD_WHITE);
  init_study();
  button();
}

void init_study() {
  display.drawInvertedBitmap(0, 0, logo, 250, 122, GxEPD_BLACK);
  //  display.drawInvertedBitmap(0, 0, logo2, 296, 128, GxEPD_BLACK);
  display.nextPage();
  display.setPartialWindow(0, 100, 255, 22);//局刷提示字样
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);

  if (!SPIFFS.begin()) // mount the file system
  {
    Serial.println("文件获取失败，请先上传文件！");
    u8g2Fonts.setCursor(0, 120);
    u8g2Fonts.print("文件获取失败，请先上传文件！");
    return;
  }
  display.fillScreen(GxEPD_WHITE);
  u8g2Fonts.setFont(chinese_city_gb2312);
  u8g2Fonts.setCursor(0, 120);
  Serial.println("文件获取成功!");
  u8g2Fonts.print("文件系统启用成功!");
  display.nextPage();
}
void loop()
{
}
