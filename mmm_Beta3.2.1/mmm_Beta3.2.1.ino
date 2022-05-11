#include <LittleFS.h>
//#include <SD.h>
#define key2         0   //中间的按键，也是烧录按键
#define key3         5   //右边的按键，进入默写模式的按键
#define key4         13
#include <U8g2_for_Adafruit_GFX.h>
#include <GxEPD2_BW.h>
#include "gb2312.c"
#include "img.c"
#define SPI_SPEED SD_SCK_MHZ(20)
#define SD_CS           5
#define bat_switch_pin  12          // 电池电压读取开关
#define bat_vcc_pin     A0          // 读取电池电压引脚，不能超过1V
//#include "FS.h"
#include <EEPROM.h>
#define RST_PIN 0 // D3(0)
//GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEM029T94
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=*/ 16, /*DC=D3*/ 0, /*RST=*/ -1, /*BUSY=*/ 4));
GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS*/ 15, /*DC*/ 0, /*RST*/ 2, /*BUSY*/ 4));
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
//int dis_count = 0; //判断局刷或全刷
bool f_word = false;
bool flag = true;
float bat_vcc = 0.0;                // 电池电压
void power_sleep();//低压休眠
#include "bat_data.h"
#include "displaycode.h"
File story_dataFile;//小说

#include "BookRead.h"
extern const uint8_t chinese_gb2312[253023] U8G2_FONT_SECTION("chinese_gb2312");

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
  Serial.println("开始");
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
  EEPROM.begin(256);
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
  float batVCC = getBatVolNew();
    if (batVCC < 3) {
      power_sleep();
    }
  /*判断当前是学习模式还是小说模式 */
  int read_mode = EEPROM.read(65);
  if (read_mode == 1) {
    if (LittleFS.begin()) { //(SD_CS, SPI_SPEED)){
      Serial.println("SD 启动成功");
    } else {
      Serial.println("SD 未能成功启动");
    }
    Serial.println("小说模式");
    display.nextPage();
    get_story_name();
  } else {
    Serial.println("学习正常启动");
    init_study();
    button();
  }
}
void init_read() {
  if (!LittleFS.begin()) // mount the file system
  {
    Serial.println("文件获取失败，请先上传文件！");
    return;
  }
}
void init_study() {
  display.drawInvertedBitmap(0, 0, logo, 250, 122, GxEPD_BLACK);
  //  display.drawInvertedBitmap(0, 0, logo2, 296, 128, GxEPD_BLACK);
  display.nextPage();
  display.setPartialWindow(0, 100, 255, 22);//局刷提示字样
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);

  if (!LittleFS.begin()) // mount the file system
  {
    Serial.println("文件获取失败，请先上传文件！");
    u8g2Fonts.setCursor(0, 120);
    u8g2Fonts.print("文件获取失败，请先上传文件！");
    return;
  }
  display.fillScreen(GxEPD_WHITE);
  u8g2Fonts.setFont(chinese_gb2312);
  u8g2Fonts.setCursor(0, 120);
  Serial.println("文件获取成功!");
  u8g2Fonts.print("文件系统启用成功!");
  String wordbookname_temp = selectbook();
  if (LittleFS.exists(wordbookname_temp)) {
    Serial.println("有");
  } else {
    Serial.println(" 没有.");
    display.fillScreen(GxEPD_WHITE);
    u8g2Fonts.setFont(chinese_gb2312);
    u8g2Fonts.setCursor(0, 120);
    u8g2Fonts.print("上次打开文件不存在！将进入文件管理");
    display.nextPage();
    delay(1000);
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    draw_word_file_dir();
  }
  display.nextPage();
}
void power_sleep() {
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.drawInvertedBitmap(0, 0, low_power, 250, 122, GxEPD_BLACK);
  display.nextPage();
  delay(500);
  ESP.deepSleep(0);
}
void loop()
{
}
