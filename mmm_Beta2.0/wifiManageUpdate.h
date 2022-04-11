#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <qrcode.h>
//AP 模式下IP地址
const IPAddress apIP(192, 168, 4, 1);
//AP 模式下 SSID 和 密码
const char* ssid = "墨水屏单词卡";
const char* apPassword = "11111111";
//文件上传
bool uplaod_flag = false;//标记是否需要运行web服务器以上传文件
File fsUploadFile;
String qrApi = "https://api.qrserver.com/v1/create-qr-code/?size=100x100&data=http://192.168.4.1";
String formatApi = "";
//配网/连接模式
boolean settingMode;
//WIFI扫描列表
String ssidList;

//窗口位置
int windowX = 110;
int windowY = 20;

DNSServer dnsServer;
ESP8266WebServer webServer(80);

const char* serverIndex = "<h1>OTA更新 请选择bin结尾的固件上传</h1><form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
void serverRUN();
void otaServer();
void uploadServer();
void handleFileUpload();
/*动态二维码生成*/
void drawQRCode(String content, int x, int y, int w) {
  QRCode qrcode;
  int DrawX = x;
  int DrawY = y;
  int blockSize = 3;//w / qrcode.size - 1;
  Serial.print("blockSize:");
  Serial.println(blockSize);
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  //content = content+ ".";
  char contentArray[content.length() + 1];
  content.toCharArray(contentArray, content.length() + 1);
  qrcode_initText(&qrcode, qrcodeData, 1, 0, contentArray);
  for (uint8_t j = 0; j < qrcode.size; j++) {
    DrawX = x;
    for (uint8_t k = 0; k < qrcode.size; k++) {
      if (qrcode_getModule(&qrcode, j, k)) {
//        Serial.println("画了");
        display.fillRect(DrawX, DrawY, blockSize, blockSize, GxEPD_BLACK);
//        Serial.print("\u2588");
      } else {
        display.fillRect(DrawX, DrawY, blockSize, blockSize, GxEPD_WHITE);
//        Serial.print(" ");
      }
      DrawX += blockSize;
    }
    DrawY += blockSize;
//    Serial.println();
  }
  
  u8g2Fonts.setCursor(x-2, DrawY + 20);
  u8g2Fonts.print(">扫我也行<");
}
/*自动配网*/
bool autoConfig()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  Serial.print("AutoConfig Waiting......");
  for (int i = 0; i < 50; i++)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("AutoConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      WiFi.printDiag(Serial);
      return true;
      //break;
    }
    else
    {
      Serial.print(".");
      delay(500);
    }
  }
  Serial.println("自动配网失败！" );
  return false;
  //WiFi.printDiag(Serial);
}

//绘制提示信息          绘制
void drawUpLoadText(String t) {
  display.setPartialWindow(windowX - 5, windowY + 60, 150, 18);
  display.fillScreen(GxEPD_WHITE);
  u8g2Fonts.setCursor(windowX, windowY + 77);
  u8g2Fonts.print(t);
  display.nextPage();
}
//绘制配网窗口           绘制
void drawConfigWindow() {
  IPAddress myIP = WiFi.softAPIP();
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.fillScreen(GxEPD_WHITE);
  drawQRCode(WiFi.softAPIP().toString(), 18, 15, 70);
  //  display.drawInvertedBitmap(-5, 5, qrCode, 110, 110, GxEPD_BLACK);
  u8g2Fonts.setFont(chinese_city_gb2312);
  display.drawRect(windowX - 10, windowY - 15, 140, 90, GxEPD_BLACK);
  u8g2Fonts.setCursor(windowX, windowY);
  u8g2Fonts.print("SSID: ");
  u8g2Fonts.print(ssid);
  u8g2Fonts.setCursor(windowX, (windowY = windowY + 25));
  u8g2Fonts.print("密码:");
  u8g2Fonts.print(apPassword);
  u8g2Fonts.setCursor(windowX, (windowY = windowY + 25));
  u8g2Fonts.print("IP地址: ");
  u8g2Fonts.print(myIP);
  u8g2Fonts.setCursor(windowX, (windowY = windowY + 25));
  u8g2Fonts.print("<-扫码获取IP");
  Serial.print("接入点SSID: ");
  Serial.println(ssid);
  Serial.print("接入点IP地址: ");
  Serial.println(myIP);
  display.nextPage();
  windowY = 40;
}
//绘制服务窗口           绘制
void drawServiceWindow() {
  Serial.println(WiFi.localIP());
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.fillScreen(GxEPD_WHITE);
  drawQRCode(WiFi.localIP().toString(), 18, 15, 70);
  //  display.drawInvertedBitmap(-5, 11, qrCode, 110, 110, GxEPD_BLACK);
  u8g2Fonts.setFont(chinese_city_gb2312);

  display.drawRect(windowX - 10, windowY - 15, 140, 90, GxEPD_BLACK);
  u8g2Fonts.setCursor(windowX, windowY);
  u8g2Fonts.print("WIFI配置服务");
  u8g2Fonts.setCursor(windowX, (windowY = windowY + 25));
  u8g2Fonts.print("IP地址: ");
  u8g2Fonts.print(WiFi.localIP());
  u8g2Fonts.setCursor(windowX, (windowY = windowY + 25));
  u8g2Fonts.print("请访问本页ip");
  display.nextPage();
  windowY = 40;
}
//展示连接信息           绘制
void drawConnectingWindow(String apssid ) {

  display.setPartialWindow(windowX - 15, windowY - 15, 200, 100);
  u8g2Fonts.setFont(chinese_city_gb2312);
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.fillScreen(GxEPD_WHITE);
  u8g2Fonts.setCursor(10, 80);
  u8g2Fonts.print("正在连接到: ");
  u8g2Fonts.print(apssid);
  u8g2Fonts.print("....");
  display.nextPage();
}
//获取文件类型           工具
String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
//读文件                服务
bool handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path = pathWithGz;
    File file = SPIFFS.open(path, "r");
    size_t sent = webServer.streamFile(file, contentType);
    file.close();
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);
  return false;
}
//生成页面               工具
String makePage(String title, String contents) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8' /> ";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}
//url字符解码            工具
String urlDecode(String input) {
  String s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}
//获取配网信息 连接或配置  服务
boolean restoreConfig() {
  //  bool f_connected=false;
  /*Serial.println("读取网络配置...");
    //  Serial.println("Connecting ...");
    WiFi.mode(WIFI_STA);
    WiFi.begin();*/
  Serial.print("wifi名：");
  Serial.println(WiFi.SSID());
  drawConnectingWindow(WiFi.SSID());
  int count = 0;
  bool flag = autoConfig();
  if (!flag) {
    Serial.println("找不到配网信息");
    settingMode = true;
    return false;
  } else {
    return true;
  }

}
//STA模式下 连接网络     工具
boolean checkConnection() {
  int count = 0;
  Serial.println("正在等待连接");
  while ( count < 20 ) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("成功连接！");
      return (true);
    }
    delay(500);
    Serial.print(".");
    count++;
  }
  Serial.println("连接超时.");
  return false;
}
//开启wifi管理页面所有服务  （基础页面/配网/OTA/文件上传）  服务
void startWebServer() {
  if (settingMode) {
    Serial.print("配网服务器启动 IP:");
    Serial.println(WiFi.softAPIP());
    webServer.on("/settings", []() {
      String s = "<head><meta charset=\"utf-8\"></head><h1>墨水屏单词卡配网</h1><p>请在选择WiFi名称后输入对应的WiFi密码.</p>";
      s += "<form method=\"get\" action=\"setap\"><label>网络:</label><select name=\"ssid\">";
      s += ssidList;
      s += "</select><br>密码:<input name=\"pass\" length=64 type=\"password\"><br>";
      s += "<input name=\"保存并提交\"  type=\"submit\"></form><p><a href=\"/updatePage\">OTA UPDATE</a></p>";
      webServer.send(200, "text/html", makePage("墨水屏单词卡配网", s));
    });
    webServer.on("/setap", []() {
      //清空数据，防止出现WiFi账号密码长度不一致导致的无法联网问题
      //      clearEEPROMtoZero(SSID_POSITION, PASSWORD_POSITION + CONTENT_LENGTH);
      String ssid = urlDecode(webServer.arg("ssid"));
      Serial.print("SSID: ");
      Serial.println(ssid.c_str());
      String pass = urlDecode(webServer.arg("pass"));
      Serial.print("Password: ");
      Serial.println(pass.c_str());
      Serial.println("Writing ssid to EEPROM...");
      WiFi.begin(ssid.c_str(), pass.c_str());
      int i = 0;
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(++i); Serial.print(' ');
      }
      Serial.print("ssid长度");
      Serial.println(WiFi.SSID().length());
      //      WriteEEPROMString(ssid, SSID_POSITION, SSID_POSITION + CONTENT_LENGTH);
      Serial.println("Writing Password to EEPROM...");
      //      WriteEEPROMString(pass, PASSWORD_POSITION, PASSWORD_POSITION + CONTENT_LENGTH);
      Serial.println("Write EEPROM done!");
      String s = "<h1>设置结束！</h1><p>设备即将在重启后接入 \"";
      s += ssid;
      s += "\" ";
      Serial.println("准备输出提交结果");
      webServer.send(200, "text/html", makePage("墨水屏单词卡配网", s));
      Serial.println("输出提交结果成功 重启");
      delay(1000);
      ESP.restart();
    });
    webServer.onNotFound([]() {
      String s = "<h1>配网模式</h1><p><a href=\"/settings\">点击配网</a></p><p><a href=\"/updatePage\">OTA UPDATE</a></p><p><a href=\"/upload\">FILE UPLOAD</a></p>";
      webServer.send(200, "text/html", makePage("配网模式", s));
    });
  }
  //无需配网 直接开启Web服务
  else {
    Serial.print(" ");

    drawServiceWindow();
    //注册ota服务
    otaServer();
    //注册文件上传服务
    uploadServer();
    //开启普通页面服务
    webServer.on("/", []() {
      String s = "<h1>STA mode</h1><p><a href=\"/reset\">重置配网信息</a></p><p><a href=\"/updatePage\">OTA 无线固件更新</a></p><p><a href=\"/upload\">词书上传</a></p>";
      webServer.send(200, "text/html", makePage("STA mode", s));
      drawUpLoadText("有管理设备接入");
    });

    webServer.on("/reset", []() {
      drawUpLoadText("有设备正在重置配网");
      delay(2000);
      drawUpLoadText("按5重置,按0取消重置");
      while (true) {
        if (digitalRead(5) == LOW) {
          //          clearEEPROMtoZero(SSID_POSITION , PASSWORD_POSITION + CONTENT_LENGTH);
          drawUpLoadText("重置配网信息成功");
          String s = "<h1>Wi-Fi RESET SUCCESS</h1><p>请重启设备.</p>";
          webServer.send(200, "text/html", makePage("Reset Wi-Fi Settings", s));
        } else if (digitalRead(0) == LOW) {
          drawUpLoadText("取消重置");
          delay(1000);
          break;
        }
        ESP.wdtFeed();
      }

    });
  }
  //启动所有服务
  webServer.begin();
}
//配网功能服务
void setupMode() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(100);
  Serial.println("wifi列表");
  for (int i = 0; i < n; ++i) {
    ssidList += "<option value=\"";
    ssidList += WiFi.SSID(i);
    ssidList += "\">";
    ssidList += WiFi.SSID(i);
    ssidList += "</option>";
  }
  Serial.println(ssidList);
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.enableAP(true);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, apPassword);
  dnsServer.start(53, "*", apIP);
  drawConfigWindow();
  startWebServer();
  while (digitalRead(key3) != LOW ) {
    serverRUN();
  }
  Serial.println("关闭配网功能");
  WiFi.disconnect();
}
//OTA 更新配置服务                                      服务服务(注册到startWebServer)
void otaServer() {
  Serial.println("OTA服务开启");
  drawUpLoadText("正在等待升级固件");
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    //MDNS.begin(host);
    webServer.on("/updatePage", HTTP_GET, []() {
      webServer.sendHeader("Connection", "close");
      webServer.send(200, "text/html", makePage("OTA更新", serverIndex));
    });
    webServer.on("/update", HTTP_POST, []() {
      webServer.sendHeader("Connection", "close");
      webServer.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, []() {
      HTTPUpload& upload = webServer.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        WiFiUDP::stopAll();
        Serial.printf("当前上传: %s\n", upload.filename.c_str());
        drawUpLoadText("上传:" + upload.filename);
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if (!Update.begin(maxSketchSpace)) { //start with max available size
          drawUpLoadText("空间不足，无法更新！");
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
          drawUpLoadText("上传出错！");
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("上传成功: %u字节\n重启中...\n", upload.totalSize);
          drawUpLoadText("上传成功:" + String(upload.totalSize) + "字节");
          delay(1000);
          drawUpLoadText("等待重启...");
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      }
      yield();
    });
    webServer.begin();
    MDNS.addService("http", "tcp", 80);

    Serial.println("Ready! Open http:");
    Serial.println( WiFi.localIP());
  } else {
    Serial.println("WiFi Failed");
  }
}
//文件上传服务                                          服务(注册到startWebServer)
void uploadServer() {
  SPIFFS.begin();
  webServer.on("/upload", HTTP_GET, []() {
    drawUpLoadText("正在等待上传词书");
    if (!handleFileRead("/upload.html"))
      webServer.send(404, "text/plain", "404: Not Found");
  });

  webServer.on("/upload", HTTP_POST, []() {
    drawUpLoadText("开始上传词书");
  }, handleFileUpload);

  webServer.onNotFound([]() {
    if (!handleFileRead(webServer.uri()))
      webServer.send(404, "text/plain", "404: Not Found");
  });

}
//文件上传处理                                          服务
void handleFileUpload() {
  HTTPUpload& upload = webServer.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    Serial.print("上传文件名: ");
    Serial.println(filename);
    drawUpLoadText("上传:" + filename + "中");
    fsUploadFile = SPIFFS.open(filename, "w");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
      drawUpLoadText("上传成功:" + String(upload.totalSize) + "字节");
      Serial.println("词库标记重置 ");
      EEPROM.write(0, 0);
      if (EEPROM.commit()) {
        Serial.println("词库更换成功！ ");
      }
      Serial.print("handleFileUpload Size: ");
      Serial.println(upload.totalSize);
      webServer.sendHeader("Location", "/success.html");

      webServer.send(303);
      uplaod_flag = false;

      //      ESP.reset();
    } else {
      Serial.println("File upload failed");
      drawUpLoadText("上传失败");
      webServer.send(500, "text/plain", "500: couldn't create file");
    }
  }
}
//运行服务器                                            服务
void serverRUN() {
  webServer.handleClient();
}
//WIFI功能启动                                         外部调用
void wifi_init() {
  delay(10);
  if (restoreConfig()) {
    if (checkConnection()) {
      settingMode = false;
      startWebServer();
      while (digitalRead(key3) != LOW ) {
        serverRUN();
      }
      return;
    }
  }
  settingMode = true;
  setupMode();
}
