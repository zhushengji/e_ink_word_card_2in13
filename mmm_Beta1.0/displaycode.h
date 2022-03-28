int dis_count = 0; //判断局刷或全刷
String tem_line = "";
bool flag = true;
int read_flag = 0;//标记上次词库读到哪里

//本函数主要实现字符的显示
void draw_words(String line, bool f) {
  String words = "";
  words = line.substring(0, line.indexOf("["));
  u8g2Fonts.setFont(u8g2_font_logisoso18_tr);
  u8g2Fonts.setCursor(0, 30);
  if (f) {
    u8g2Fonts.print(words);
    Serial.println(words);
  } else {
    int w_index = random(0, words.length());
    for (int k = 0; k < words.length(); k++) {
      if (k == w_index) {
        Serial.print(words.charAt(w_index));
        u8g2Fonts.print(words.charAt(w_index));
      } else {
        Serial.print("*");
        u8g2Fonts.print("*");
      }
    }
    Serial.println();
  }
  Serial.println((line.substring(line.indexOf("["), line.indexOf("]") + 1)));
  u8g2Fonts.setFont(u8g2_font_unifont_t_extended);
  u8g2Fonts.print(line.substring(line.indexOf("["), line.indexOf("]") + 1));
  display.drawLine(5, 35, 245, 35, 0); //画水平线
  int k = line.indexOf("]") + 1;
  String s4 = "", s5 = "";
  int x = 5, y = 50;
  u8g2Fonts.setFont(chinese_city_gb2312);
  while (k < line.length()) {
    if ((k != 0 && line[k - 1] != ' ') || line[k] != ' ') {
      s4 += line[k];
    }
    if ((k <= line.length() - 2 && (line[k + 1] == ' ') && line[k + 2] == ' ' || k == line.length() - 1)) {
      if (s4.length() != 1) {
        /*
           2.13寸屏：48
           2.66寸屏：60
           2.9寸屏：69
        */
        if (s4.length() <= 69) {
          Serial.println(s4);
          u8g2Fonts.setCursor(x, y);
          u8g2Fonts.print(s4);
          y += 18;
        } else if (s4.length() > 69) {
          for (int m = 0; m < s4.length(); m++) {
            s5 += s4[m];
            if (m == 69) {
              u8g2Fonts.setCursor(x, y);
              u8g2Fonts.print(s5);
              Serial.println(s5);
              y += 18;
              s5 = "";
            }
          }
          u8g2Fonts.setCursor(x, y);
          u8g2Fonts.print(s5);
          Serial.println(s5);
          y += 18;
          s5 = "";
        }

      }
      s4 = "";
    }
    k++;
  }
  display.nextPage();
  y += 18;
  if (dis_count > 9) {
    dis_count = 0;
    y = 50;
  }
}
String filename = "";
//判断书目
String selsectbook() {
  if (EEPROM.read(8) != '/') {
    filename = "/book.txt";
  } else {
    for (int i = 8; i < 64; ++i) {
      filename += char(EEPROM.read(i));
    }
  }
  Serial.print("本次书目：");
  Serial.println(filename);
  return filename;
}
File dataFile = SPIFFS.open(filename, "r");//("/words.txt", "r");
File wordsFile ;
//同步上次阅读进度
bool synchro() {
  bool flag = false;
  String filename = selsectbook();
  int conut_flag = 0 ;
  //  bool continue_flag = false;//标记是否同步成功
  if (filename == "/book.txt") {
    read_flag = EEPROM.read(0);
  } else {
    read_flag = EEPROM.read(4);
  }
  Serial.print("标记位：");
  Serial.println(read_flag);
  dataFile = SPIFFS.open(filename, "r");//("/words.txt", "r");
  wordsFile ;
  if (filename == "/book.txt") {
    wordsFile = SPIFFS.open("/words.txt", "a");
  }

  while (dataFile.available()) {
    if (conut_flag == read_flag) {
      //      continue_flag = true;
      display.setPartialWindow(0, 100, 255, 22);//局刷提示字样
      display.fillScreen(GxEPD_WHITE);
      u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
      u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
      u8g2Fonts.setCursor(0, 120);
      u8g2Fonts.print("进度同步成功！开始学习吧！");
      display.nextPage();
      flag = true;
      break;
    } else {
      //读一条数据
      dataFile.readStringUntil('\n');
      conut_flag++;
    }
  }

  return flag;
}
//主要功能函数
void button() {
  String tem_line = "";
  String line = "";
  int key3_count = 0;
  dis_count = 0;
  bool f_word = false;
  int rect_count = 1;
  int mode_count = 1;
  bool menu = false;
  if (synchro()) {
    //如果文本未读完
    while (dataFile.available()) {
      //同步成功，进入词库使用功能
      Serial.print("dis_count计数：");
      Serial.println(dis_count);
      if (dis_count == 0) {
        display.setFullWindow();
      } else {
        display.setPartialWindow(0, 0, display.width(), display.height());
      }
      display.fillScreen(GxEPD_WHITE);
      if (!f_word) {
        line = dataFile.readStringUntil('\n');
      }
      //下面处理如何显示
      while (true) {
        //
        if ( digitalRead(key3) == LOW) {
          tem_line = line;
          delay(200);
          if (digitalRead(key3) != LOW) {
            Serial.println("切换单词");
            /*短按*/
            //如果是短按要判断是阅读模式还是默写模式
            read_flag++;
            //同步进度
            if (filename == "/book.txt") {
              EEPROM.write(0, read_flag);
              EEPROM.commit();
            } else {
              EEPROM.write(4, read_flag);
              EEPROM.commit();
            }
            //判断是否需要显示完整字符
            if (!f_word) {
              draw_words(line, flag);
              display.fillScreen(GxEPD_WHITE);
              dis_count++;
              if (!flag) {
                f_word = true;
              }
            } else if (f_word && !flag) {
              draw_words(tem_line, true);
              display.fillScreen(GxEPD_WHITE);
              f_word = false;
              dis_count++;
            }
            break;
          }
          delay(300);
          //如果是长按，唤出菜单
          if (digitalRead(key3) == LOW) {
            Serial.println("按钮唤出菜单！");
            display.setPartialWindow(1, 0, 78, 21);//局刷提示字样
            display.fillScreen(GxEPD_BLACK);
            u8g2Fonts.setFont(chinese_city_gb2312);
            u8g2Fonts.setForegroundColor(GxEPD_WHITE);  // 设置前景色
            u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
            u8g2Fonts.setCursor(24, 18);
            u8g2Fonts.print("菜单");
            display.nextPage();
            display.setPartialWindow(0, 24, 78, 60);
            display.fillScreen(GxEPD_WHITE);
            display.drawRect(1, 25, 77, 59, GxEPD_BLACK);
            u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
            u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
            display.drawRect(3, 22, 73, 22, GxEPD_BLACK);
            u8g2Fonts.setCursor(9, 41);
            u8g2Fonts.print("切换模式");
            //****
            u8g2Fonts.setCursor(9, 60);
            u8g2Fonts.print("切换词库");
            display.nextPage();
            display.setPartialWindow(0, 24, 78, 60);
            while (true) {
              //长按确定，短按切换功能
              if (digitalRead(key3) == LOW ) {
                delay(500);
                if (digitalRead(key3) == LOW ) {
                  if (mode_count == 2) {
                    if (filename == "/book.txt") {
                      filename = "/words.txt";
                      for (int i = 8; i < 64; ++i) {
                        EEPROM.write(i, 0);
                        EEPROM.commit();
                      }
                      for (int i = 8; i < filename.length() + 8; ++i) {
                        EEPROM.write(i, filename[i - 8]);
                        EEPROM.commit();
                      }
                    } else {
                      filename = "/book.txt";
                      for (int i = 8; i < 64; ++i) {
                        EEPROM.write(i, 0);
                        EEPROM.commit();
                      }
                      for (int i = 8; i < filename.length() + 8; ++i) {
                        EEPROM.write(i, filename[i - 8]);
                        EEPROM.commit();
                      }
                    }
                    ESP.reset();
                  } else {
                    key3_count++;
                    (key3_count % 2 == 0) ? flag = true : flag = false;
                    display.setPartialWindow(0, 0, display.width(), display.height());
                    display.fillScreen(GxEPD_WHITE);
                    display.nextPage();
                    break;
                  }
                } else {
                  rect_count++;
                  display.setPartialWindow(0, 24, 78, 60);
                  display.drawRect(1, 25, 77, 59, GxEPD_BLACK);
                  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
                  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
                  if (rect_count % 2 != 0) {
                    Serial.println("模式切换功能");
                    mode_count = 1;
                    display.drawRect(3, 22, 73, 22, GxEPD_BLACK);
                    display.drawRect(3, 42, 73, 22, GxEPD_WHITE);
                  } else {
                    mode_count = 2;
                    display.drawRect(3, 22, 73, 22, GxEPD_WHITE);
                    display.drawRect(3, 42, 73, 22, GxEPD_BLACK);
                    Serial.println("词库切换功能");
                  }
                  display.nextPage();
                }
              }
              ESP.wdtFeed();//喂狗
            }
          }
        }
        if (digitalRead(key2) == LOW) {
          delay(100);
          if (digitalRead(key2) == LOW) {
            delay(400);
            display.setPartialWindow(0, 100, 255, 22);//局刷提示字样
            display.fillScreen(GxEPD_WHITE);
            u8g2Fonts.setFont(chinese_city_gb2312);
            u8g2Fonts.setCursor(0, 120);
            if (filename != "/words.txt") {
              wordsFile = SPIFFS.open("/words.txt", "a");
              Serial.println("wordsFile创建成功");
              Serial.print("添加词条：");
              wordsFile.println(tem_line);
              Serial.println(tem_line);
              u8g2Fonts.print("已添加到生词本!");
              wordsFile.close();
            } else {
              Serial.print("不可添加词条：");
              u8g2Fonts.print("无法将生词本内容添加到自己!");
            }
            display.nextPage();
          }
        }
        ESP.wdtFeed();//喂狗
      }
      if (dis_count == 10) {
        dis_count = 0;
      }
      ESP.wdtFeed();//喂狗
    }
  }
  dataFile.close();
  //如果读完了重置本书标记位
  while (true) {
    ESP.wdtFeed();//喂狗
    if (digitalRead(key3) == LOW) {
      display.setPartialWindow(0, 100, 255, 22);//局刷提示字样
      display.fillScreen(GxEPD_WHITE);
      u8g2Fonts.setFont(chinese_city_gb2312);
      u8g2Fonts.setCursor(0, 120);
      Serial.println("本书已读完！请更换词库！");
      u8g2Fonts.print("本书已读完！请更换词库或从头阅读！");
      if (filename == "/book.txt") {
        Serial.println("词库重置~");
        EEPROM.write(0, 0);
        EEPROM.commit();
      } else {
        Serial.println("生词本重置~");
        EEPROM.write(4, 0);
        EEPROM.commit();
      }
      display.nextPage();
      delay(500);
      if (digitalRead(key2) == LOW) {
        if (filename == "/book.txt") {
          filename = "/words.txt";
          for (int i = 8; i < 64; ++i) {
            EEPROM.write(i, 0);
            EEPROM.commit();
          }
          for (int i = 8; i < filename.length() + 8; ++i) {
            EEPROM.write(i, filename[i - 8]);
            EEPROM.commit();
          }
        } else {
          filename = "/book.txt";
          for (int i = 8; i < 64; ++i) {
            EEPROM.write(i, 0);
            EEPROM.commit();
          }
          for (int i = 8; i < filename.length() + 8; ++i) {
            EEPROM.write(i, filename[i - 8]);
            EEPROM.commit();
          }
        }
        ESP.reset();
      }
      return;
    }
  }
}
