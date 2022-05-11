int dis_count = 0; //判断局刷或全刷
void dis_study();
void draw_word_file_dir();
void draw_menue();
void draw_words(String line, bool f);
//void draw_word_temp(String line, bool f);
String tem_line = "";
int key3_count = 0;
int read_flag = 0;//标记上次词库读到哪里
#include "wifiManageUpdate.h"

String book_name = "";
String wordbook_name = "/study/words.txt";
String wordbook_progress_name = "";
String wordbooks_name[6] = {};
//字符串转数字
int wordbookconversion_str(String number) {
  int page_num = 0;
  for (int k = 0; k < number.length(); k++) {
    page_num = page_num * 10 + int(number[k] - '0');
  }
  return page_num;
}

//获取上次读书进度
int getwordbookprogress(String filename) {
  //  Serial.print("要读取的文件名："); Serial.println(filename);
  Serial.println("进入获取进度函数");
  Serial.print("进度文件名："); Serial.println(filename);
  File dataFile = LittleFS.open(filename, "r");
  String num = "";
  for (int i = 0; i < dataFile.size(); i++) {
    num += (char)dataFile.read();
  }
  dataFile.close();
  Serial.print("获取的数据："); Serial.println(num);
  return wordbookconversion_str(num);
}

//词库进度保存功能
void savewordbookprogress(String filename, String page_couunt) {
  Serial.print("保存的进度："); Serial.println(page_couunt);
  Serial.print("存储进度的文档名字："); Serial.println(filename);
  File dataFile = LittleFS.open(filename, "w");
  dataFile.print(page_couunt);
  dataFile.close();
}
//保存上次阅读记录功能
void savebookname(String name_word) {
  //清空上册阅读记录
  for (int i = 8; i < 64; i++) {
    EEPROM.write(i, 0);
    EEPROM.commit();
  }
  //存储本次书目
  Serial.print("存储的文档名字："); Serial.println(name_word);
  for (int i = 8; i < name_word.length() + 8; i++) {
    //    Serial.print(name_word[i - 8]);
    EEPROM.write(i, name_word[i - 8]);
    EEPROM.commit();
  }
  Serial.println("存储结束");
}
void wordbook_function(int book_count) {
  display.setPartialWindow(0, 0, display.width(), display.height());
  int key_count = 0;//文件选择计数
  int key_file_fun_count = 1;//删除或者打开
  String temp_name = "";
  while (true) {
    float batVCC = getBatVolNew();
    if (batVCC < 3) {
      power_sleep();
    }
    ESP.wdtFeed();//喂狗
    if (digitalRead(key3 ) == LOW) {
      delay(300);
      if (digitalRead(key3 ) == LOW) { //长按进入打开或删除
        display.setPartialWindow(1, 17, 201, 61);
        display.fillScreen(GxEPD_WHITE);
        display.drawRect(2, 18, 200, 60, GxEPD_BLACK);
        u8g2Fonts.setFont(chinese_gb2312);
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
        u8g2Fonts.setCursor(20, 38);
        u8g2Fonts.print("想要对文档进行什么操作？");//wordbooks_name[key_count-1];
        temp_name = "/study/" + temp_name;
        Serial.print("文档名字："); Serial.println(temp_name);
        u8g2Fonts.setCursor(14, 59);
        u8g2Fonts.print("打开");
        u8g2Fonts.setCursor(160, 59);
        u8g2Fonts.print("删除");
        display.nextPage();
        while (true) {
          float batVCC = getBatVolNew();
          if (batVCC < 3) {
            power_sleep();
          }
          //按键2退出菜单
          if (digitalRead(key2) == LOW) {
            delay(300);
            display.setPartialWindow(0, 0, display.width(), display.height());
            display.fillScreen(GxEPD_WHITE);
            display.nextPage();
            draw_word_file_dir();
            return;
          }
          ESP.wdtFeed();//喂狗
          if (digitalRead(key3 ) == LOW) {
            delay(300);
            if (digitalRead(key3 ) == LOW) {
              if (key_file_fun_count % 2 != 1) {
                Serial.println("文档打开");
                book_name = temp_name;
                savebookname(book_name);
                wordbook_progress_name = "/progress" + book_name.substring(0, book_name.indexOf(".txt")) + ".md";
                Serial.println(book_name);
                if (!LittleFS.exists(wordbook_progress_name)) {
                  savewordbookprogress(wordbook_progress_name, String(0));
                }
                display.setPartialWindow(0, 0, display.width(), display.height());
                display.fillScreen(GxEPD_WHITE);
                dis_study();//启用主功能
              } else {
                temp_name = temp_name;
                Serial.print("文档删除:");
                Serial.println(temp_name);
                Serial.println(wordbook_progress_name);
                if (LittleFS.remove(temp_name) ) {
                  for (int i = 8; i < 64; i++) {
                    EEPROM.write(i, 0);
                    EEPROM.commit();
                  }
                  Serial.println("文档删除成功！");
                  Serial.print("文档进度文件："); Serial.println(wordbook_progress_name);
                  if (LittleFS.exists(wordbook_progress_name)) {
                    Serial.println("文档进度删除成功！");
                    LittleFS.remove(wordbook_progress_name);
                  }
                  display.setPartialWindow(0, 0, display.width(), display.height());
                  display.fillScreen(GxEPD_WHITE);
                  display.nextPage();
                  draw_word_file_dir();
                  return ;
                }
              }
            } else {
              if (key_file_fun_count % 2 == 1) {
                display.drawRect(13, 46, 30, 15, GxEPD_BLACK);
                display.drawRect(159, 46, 30, 15, GxEPD_WHITE);
                Serial.println("打开");
              } else {
                display.drawRect(13, 46, 30, 15, GxEPD_WHITE);
                display.drawRect(159, 46, 30, 15, GxEPD_BLACK);
                Serial.println("删除");
              }
              key_file_fun_count++;
              display.nextPage();
            }
          }
        }
      } else { //短按
        for (int i = 0; i < book_count; i++) {
          display.drawRect(2, (i + 1) * 18 + 2, wordbooks_name[i].length() * 7, 18, GxEPD_WHITE);
        }
        temp_name = wordbooks_name[key_count];
        Serial.print("当前书籍："); Serial.println(temp_name);
        display.drawRect(2, (key_count + 1) * 18 + 2, wordbooks_name[key_count].length() * 7, 18, GxEPD_BLACK);
        display.nextPage();
        key_count++;
        if (key_count == book_count) {
          key_count = 0;
        }
      }
    }
    //按键2退出菜单
    if (digitalRead(key2) == LOW) {
      delay(300);
      display.setPartialWindow(0, 0, display.width(), display.height());
      display.fillScreen(GxEPD_WHITE);
      display.nextPage();
      display.setPartialWindow(0, 100, 255, 22);//局刷提示字样
      u8g2Fonts.setFont(chinese_gb2312);
      u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
      u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
      u8g2Fonts.print("正在恢复学习进度，请稍后...");
      display.nextPage();
      break;
    }
  }
  dis_study();
}
void draw_word_file_dir() {
  Serial.println("进入目录");
  int y = 18;
  u8g2Fonts.setFont(chinese_gb2312);
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
  //
  u8g2Fonts.setCursor(2, y);
  u8g2Fonts.print("书籍列表:");
  uint8_t txt_i = 0;
  // 显示目录中文件名以及文件大小
  Dir dir = LittleFS.openDir("/study/");
  int i = 0;
  String fileName = "";
  while (dir.next())                // dir.next()用于检查目录中是否还有“下一个文件”
  {
    if (i < 6) {
      fileName = dir.fileName();
      if (fileName.endsWith(".txt")) // 检测TXT文件，将名字赋值给txtName
      {
        wordbooks_name[i] = fileName;
        Serial.print("文件:"); Serial.println(fileName);
        i++;
      }
    } else {
      break;
    }
  }

  if (wordbooks_name[0].length() > 5) {
    for (int k = 0; k < i; k++ ) {
      y += 18;
      u8g2Fonts.setCursor(4, y);
      u8g2Fonts.print(wordbooks_name[k]);
      if (wordbooks_name[k].indexOf("words.txt") != -1) {
        u8g2Fonts.print("  (生词本)");
      }
    }
    display.nextPage();
    wordbook_function(i);
  } else {
    draw_menue();
  }


  /*  for (int k = 0; k < i; k++ ) {
      y += 18;
      u8g2Fonts.setCursor(4, y);
      u8g2Fonts.print(wordbooks_name[k]);
      if (wordbooks_name[k].indexOf("words.txt") != -1) {
        u8g2Fonts.print("  (生词本)");
      }
    }
    //  display.drawRect(1, 34, wordbooks_name[0].length() * 12, 16, GxEPD_BLACK);
    display.nextPage();
    wordbook_function(i);*/
}
void draw_menue() {
  float batVCC = getBatVolNew();
  uint8_t bat_bfb = getBatVolBfb(batVCC);
  int rect_count = 1;
  int mode_count = 1;
  //菜单
  Serial.println("按钮唤出菜单！");
  display.setPartialWindow(79, 0, 163, 21); //局刷提示字样
  display.fillScreen(GxEPD_WHITE);
  u8g2Fonts.setFont(chinese_gb2312);
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
  u8g2Fonts.setCursor(82, 18);
  u8g2Fonts.print("剩余电量：  ");
  u8g2Fonts.print(bat_bfb);
  u8g2Fonts.print("%");
  display.nextPage();
  display.setPartialWindow(1, 0, 78, 21);//局刷提示字样
  display.fillScreen(GxEPD_BLACK);
  u8g2Fonts.setFont(chinese_gb2312);
  u8g2Fonts.setForegroundColor(GxEPD_WHITE);  // 设置前景色
  u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
  u8g2Fonts.setCursor(24, 18);
  u8g2Fonts.print("菜单");
  display.nextPage();
  display.setPartialWindow(0, 24, 78, 83);
  display.fillScreen(GxEPD_WHITE);
  display.drawRect(1, 25, 77, 82, GxEPD_BLACK);
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
  display.drawRect(3, 22, 73, 22, GxEPD_BLACK);
  u8g2Fonts.setCursor(9, 41);
  u8g2Fonts.print("切换模式");
  u8g2Fonts.setCursor(9, 60);
  u8g2Fonts.print("切换词库");
  u8g2Fonts.setCursor(9, 79);
  u8g2Fonts.print("更新词库");
  u8g2Fonts.setCursor(9, 98);
  if (EEPROM.read(65) == 1) {
    u8g2Fonts.print("单词模式");
  } else {
    u8g2Fonts.print("名著模式");
  }
  display.nextPage();
  display.setPartialWindow(0, 24, 78, 60);
  while (true) {
    float batVCC = getBatVolNew();
    if (batVCC < 3) {
      power_sleep();
    }
    if (digitalRead(key2) == LOW ) {
      delay(300);
      display.setPartialWindow(0, 0, display.width(), display.height());
      display.fillScreen(GxEPD_WHITE);
      display.nextPage();
      break;
    }
    //长按确定，短按切换功能
    if (digitalRead(key3) == LOW ) {
      delay(500);
      if (digitalRead(key3) == LOW ) {
        if (mode_count == 2) {
          display.setPartialWindow(0, 0, display.width(), display.height());
          display.fillScreen(GxEPD_WHITE);
          draw_word_file_dir();
        } else if (mode_count == 3) {
          wifi_init();
        } else if (mode_count == 1) {
          key3_count++;
          (key3_count % 2 == 0) ? flag = true : flag = false;
          display.setPartialWindow(0, 0, display.width(), display.height());
          display.fillScreen(GxEPD_WHITE);
          display.nextPage();
          break;
        } else {//名著模式
          EEPROM.write(65, 1);
          EEPROM.commit();
          ESP.reset();
        }
      } else {
        rect_count++;
        display.setPartialWindow(0, 24, 78, 83);
        display.drawRect(1, 25, 77, 79, GxEPD_BLACK);
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
        if (rect_count % 4 == 1) {
          Serial.println("模式切换功能");
          mode_count = 1;
          display.drawRect(3, 22, 73, 22, GxEPD_BLACK);
          display.drawRect(3, 42, 73, 22, GxEPD_WHITE);
          display.drawRect(3, 62, 73, 22, GxEPD_WHITE);
          display.drawRect(3, 82, 73, 22, GxEPD_WHITE);
        } else if (rect_count % 4 == 2) {
          mode_count = 2;
          display.drawRect(3, 22, 73, 22, GxEPD_WHITE);
          display.drawRect(3, 42, 73, 22, GxEPD_BLACK);
          display.drawRect(3, 62, 73, 22, GxEPD_WHITE);
          display.drawRect(3, 82, 73, 22, GxEPD_WHITE);
          Serial.println("词库切换功能");
        } else if (rect_count % 4 == 3) {
          mode_count = 3;
          display.drawRect(3, 22, 73, 22, GxEPD_WHITE);
          display.drawRect(3, 42, 73, 22, GxEPD_WHITE);
          display.drawRect(3, 62, 73, 22, GxEPD_BLACK);
          display.drawRect(3, 82, 73, 22, GxEPD_WHITE);
          Serial.println("词库更新功能");
        } else {
          mode_count = 4;
          display.drawRect(3, 22, 73, 22, GxEPD_WHITE);
          display.drawRect(3, 42, 73, 22, GxEPD_WHITE);
          display.drawRect(3, 62, 73, 22, GxEPD_WHITE);
          display.drawRect(3, 82, 73, 22, GxEPD_BLACK);
          Serial.println("小说功能");
        }
        display.nextPage();
      }
    }
    ESP.wdtFeed();//喂狗
  }
  draw_words(tem_line, true);
}

//本函数主要实现字符的显示
void draw_words(String line, bool f) {
  Serial.print("dis_count:"); Serial.println(dis_count);
  if (dis_count == 0) {
    Serial.println("全刷");
    display.setFullWindow();
  } else {
    Serial.println("局刷");
    display.setPartialWindow(0, 0, display.width(), display.height());
  }
  display.fillScreen(GxEPD_WHITE);
  //  display.firstPage();
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
  u8g2Fonts.setFont(chinese_gb2312);
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
        if (s4.length() <= 48) {
          Serial.println(s4);
          u8g2Fonts.setCursor(x, y);
          u8g2Fonts.print(s4);
          y += 18;
        } else if (s4.length() > 48) {
          for (int m = 0; m < s4.length(); m++) {
            s5 += s4[m];
            if (m == 48) {
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
  y = 50;
}

//判断书目
String selectbook() {
  String filename = "";
  Serial.print("选书中book_name："); Serial.println(book_name);
  if (book_name.length() > 5) {
    Serial.println(" 直接返回 ！");
    filename = book_name;
  } else {
    for (int i = 8; i < 64; ++i) {
      filename += char(EEPROM.read(i));
    }
  }
  Serial.print("本次书目：");
  Serial.println(filename);
  return filename;
}

File dataFile = LittleFS.open(selectbook(), "r");
File wordsFile ;

//同步上次阅读进度
bool synchro() {
  bool s_flag = false;
  String filename = selectbook();
  int conut_flag = 0 ;
  if (filename != wordbook_name) {
    Serial.println("获取进度中...");
    String temp_progress = "/progress" + filename.substring(0, filename.indexOf(".txt")) + ".md";
    read_flag = getwordbookprogress(temp_progress);
  } else {//生词本
    read_flag = EEPROM.read(4);
  }
  if (read_flag > 0) { //跳到上次阅读的最后一个单词
    read_flag--;
  }
  Serial.print("标记位：");
  Serial.println(read_flag);
  dataFile = LittleFS.open(filename, "r");
  if (filename == book_name) {
    wordsFile = LittleFS.open(book_name, "a");
  }
  while (dataFile.available()) {
    if (conut_flag == read_flag) {
      display.setPartialWindow(0, 100, 255, 22);//局刷提示字样
      display.fillScreen(GxEPD_WHITE);
      u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
      u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
      u8g2Fonts.setCursor(0, 120);
      u8g2Fonts.print("进度同步成功！开始学习吧！");
      display.nextPage();
      s_flag = true;
      break;
    } else {
      //读一条数据
      dataFile.readStringUntil('\n');
      conut_flag++;
    }
  }
  return s_flag;
}
void change_word(String filename, String line) {
  Serial.println("切换单词");
  //如果是短按要判断是阅读模式还是默写模式
  read_flag++;
  Serial.print("read_flag:"); Serial.println(read_flag);
  //同步进度
  if (filename != wordbook_name) {//普通词库进度
    String temp_progress = "/progress" + filename.substring(0, filename.indexOf(".txt")) + ".md";
    savewordbookprogress(temp_progress, String(read_flag));
  } else {//生词本进度
    EEPROM.write(4, read_flag);
    EEPROM.commit();
  }
  //判断是否需要显示完整字符
  if (!f_word) {
    draw_words(line, flag);
    display.fillScreen(GxEPD_WHITE);
    if (!flag) {
      f_word = true;
    }
  } else if (f_word && !flag) {
    draw_words(tem_line, true);
    display.fillScreen(GxEPD_WHITE);
    f_word = false;
  }
  dis_count++;
}
void add_word(String filename) {
  delay(400);
  display.setPartialWindow(0, 100, 255, 22);//局刷提示字样
  display.fillScreen(GxEPD_WHITE);
  u8g2Fonts.setFont(chinese_gb2312);
  u8g2Fonts.setCursor(0, 120);
  if (filename != wordbook_name) {
    wordsFile = LittleFS.open(wordbook_name, "a");
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
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.fillScreen(GxEPD_WHITE);
}
void dis_study() {
  //  String tem_line = "";
  String line = "";
  dis_count = 0;
  bool menu = false;
  String filename = "";
  filename = selectbook();
  savebookname(filename);
  Serial.print("书目名称："); Serial.println(filename);
  int sleep_count = 0;
  if (synchro()) {
    Serial.println("同步成功，开始学习");
    //如果文本未读完
    while (dataFile.available()) {
      //同步成功，进入词库使用功能
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
        float batVCC = getBatVolNew();
        if (batVCC < 3) {
          power_sleep();
        }
        if ( digitalRead(key3) == LOW) {
          sleep_count = 0; //休眠标记重置
          tem_line = line;
          delay(200);
          if (digitalRead(key3) != LOW) {
            /*短按*/
            //切换单词
            //            dis_count++;
            change_word(filename,  line);
            break;
          }
          delay(300);
          //如果是长按，唤出菜单
          if (digitalRead(key3) == LOW) {
            draw_menue();
          }
        }
        if (digitalRead(key2) == LOW) {
          sleep_count = 0; //休眠标记重置
          delay(100);
          if (digitalRead(key2) == LOW) {
            //添加生词
            add_word(filename);
          }
        }
        delay(100);
        sleep_count++;
        if (sleep_count * 0.1 > 300) {
          display.init();
          display.setFullWindow();
          display.firstPage();
          display.fillScreen(GxEPD_BLACK);
          display.drawInvertedBitmap(0, 0, sleep_pic, 250, 122, GxEPD_WHITE);
          display.nextPage();
          delay(500);
          Serial.println("进入休眠...");
          ESP.deepSleep(0);
        }
        ESP.wdtFeed();//喂狗
      }
      if (dis_count == 25) {
        dis_count = 0;
      }
      ESP.wdtFeed();//喂狗
    }
  }
  dataFile.close();
  //如果读完了重置本书标记位
  while (true) {
    float batVCC = getBatVolNew();
    if (batVCC < 3) {
      power_sleep();
    }
    ESP.wdtFeed();//喂狗
    if (digitalRead(key3) == LOW) {
      display.setPartialWindow(0, 100, 255, 22);//局刷提示字样
      display.fillScreen(GxEPD_WHITE);
      u8g2Fonts.setFont(chinese_gb2312);
      u8g2Fonts.setCursor(0, 120);
      Serial.println("本书已读完！请更换词库！");
      u8g2Fonts.print("本书已读完！请更换词库或从头阅读！");
      if (filename != wordbook_name) {//普通词库进度
        String temp_progress = "/progress/" + filename.substring(0, filename.indexOf(".txt")) + ".md";
        savewordbookprogress(temp_progress, String(0));
      } else {
        Serial.println("生词本重置~");
        EEPROM.write(4, 0);
        EEPROM.commit();
      }
      display.nextPage();
      delay(1000);
      draw_menue();
      return;
    }
  }
}
