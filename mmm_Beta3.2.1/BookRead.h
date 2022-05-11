/*
   思路：由于小说的内容包含中英文及符号等，因此凑满一页的char个数并不相同
   但是我们可以通过将每次得到的个数变为统一格式的字符串，通过页码可以反推出
   当前所在位置对应的索引，在索引取出来的时候转换为int即可。得到了位置码，
   可以通过seek()来实现小说内容读取
*/

void select_read_menue();//
void draw_file_dir();//文件列表
void getbooksource();
void draw_read_menue();//菜单操作
void save_story_name();//保存上次阅读小说名.
void draw_read_menue();//菜单界面
int getstorynum(int pagenum, String indexfilename);
String getpagestr(int storynum, String storyfilename);//获取索引中的记录
void draw_story(String string);
String selected_story_name = "";
String index_story_file_name = "";
String story_progress_name = "";
String stories_name[6] = {};
//int menuebackpage=0;

int8_t getCharLength(char zf) //获取ascii字符的长度
{
  if (zf == 0x20) return 4;      //空格
  else if (zf == '!') return 4;
  else if (zf == '"') return 5;
  else if (zf == '#') return 5;
  else if (zf == '$') return 6;
  else if (zf == '%') return 7;
  else if (zf == '&') return 7;
  else if (zf == '\'') return 3;
  else if (zf == '(') return 5;
  else if (zf == ')') return 5;
  else if (zf == '*') return 7;
  else if (zf == '+') return 7;
  else if (zf == ',') return 3;
  else if (zf == '.') return 3;

  else if (zf == '1') return 5;
  else if (zf == ':') return 4;
  else if (zf == ';') return 4;
  else if (zf == '@') return 9;

  else if (zf == 'A') return 8;
  else if (zf == 'D') return 7;
  else if (zf == 'G') return 7;
  else if (zf == 'H') return 7;
  else if (zf == 'I') return 3;
  else if (zf == 'J') return 3;
  else if (zf == 'M') return 8;
  else if (zf == 'N') return 7;
  else if (zf == 'O') return 7;
  else if (zf == 'Q') return 7;
  else if (zf == 'T') return 7;
  else if (zf == 'U') return 7;
  else if (zf == 'V') return 7;
  else if (zf == 'W') return 11;
  else if (zf == 'X') return 7;
  else if (zf == 'Y') return 7;
  else if (zf == 'Z') return 7;

  else if (zf == '[') return 5;
  else if (zf == ']') return 5;
  else if (zf == '`') return 5;

  else if (zf == 'c') return 5;
  else if (zf == 'f') return 5;
  else if (zf == 'i') return 1;
  else if (zf == 'j') return 2;
  else if (zf == 'k') return 5;
  else if (zf == 'l') return 2;
  else if (zf == 'm') return 9;
  else if (zf == 'o') return 7;
  else if (zf == 'r') return 4;
  else if (zf == 's') return 5;
  else if (zf == 't') return 4;
  else if (zf == 'v') return 7;
  else if (zf == 'w') return 9;
  else if (zf == 'x') return 5;
  else if (zf == 'y') return 7;
  else if (zf == 'z') return 5;

  else if (zf == '{') return 5;
  else if (zf == '|') return 4;
  else if (zf == '}') return 5;

  else if ((zf >= 0 && zf <= 31) || zf == 127) return -1; //没有实际显示功能的字符

  else return 6;
}
//字符串转数字
int conversion_str(String number) {
  int page_num = 0;
  for (int k = 0; k < number.length(); k++) {
    page_num = page_num * 10 + int(number[k] - '0');
  }
  return page_num;
}
//小说进度保存功能
void saveprogress(String filename, String page_couunt) {
  File dataFile = LittleFS.open(filename, "w");
  dataFile.print(page_couunt);
  dataFile.close();
}
//小说进度获取
int getprogress(String filename) {
  //  Serial.print("要读取的文件名："); Serial.println(filename);
  File dataFile = LittleFS.open(filename, "r");
  String num = "";
  for (int i = 0; i < dataFile.size(); i++) {
    num += (char)dataFile.read();
  }
  dataFile.close();
  return conversion_str(num);
}
void story_function(int book_count) {
  display.setPartialWindow(0, 0, display.width(), display.height());
  //  Serial.print("book_count："); Serial.println(book_count);
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
        u8g2Fonts.print("想要对文档进行什么操作？");//stories_name[key_count-1];
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
            draw_file_dir();
            break;
          }
          ESP.wdtFeed();//喂狗
          if (digitalRead(key3 ) == LOW) {
            delay(300);
            if (digitalRead(key3 ) == LOW) {
              if (key_file_fun_count % 2 != 1) {
                Serial.println("文档打开");
                selected_story_name = temp_name;
                save_story_name();
                index_story_file_name = selected_story_name.substring(0, selected_story_name.indexOf(".txt")) + ".md";
                story_progress_name = "/progress" + index_story_file_name;
                Serial.println(selected_story_name);
                Serial.println(index_story_file_name);
                display.setPartialWindow(0, 0, display.width(), display.height());
                display.fillScreen(GxEPD_WHITE);
                getbooksource();
              } else {
                Serial.println("文档删除");
                if (LittleFS.remove(temp_name) ) {
                  for (int i = 66; i < 100; i++) {
                    EEPROM.write(i, 0);
                    EEPROM.commit();
                  }
                  Serial.println("文档删除成功！");
                  Serial.print("文档索引文件："); Serial.println(index_story_file_name);
                  if (LittleFS.exists(index_story_file_name)) {
                    LittleFS.remove(index_story_file_name);
                    Serial.println("文档索引删除成功！");
                  }
                  Serial.print("文档进度文件："); Serial.println(story_progress_name);
                  if (LittleFS.exists(story_progress_name)) {
                    LittleFS.remove(story_progress_name);
                    Serial.println("文档进度删除成功！");
                  }
                  display.setPartialWindow(0, 0, display.width(), display.height());
                  display.fillScreen(GxEPD_WHITE);
                  display.nextPage();
                  draw_file_dir();
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
          display.drawRect(2, (i + 1) * 18 + 2, stories_name[i].length() * 7, 18, GxEPD_WHITE);
        }
        temp_name = stories_name[key_count];
        Serial.print("当前书籍："); Serial.println(temp_name);
        display.drawRect(2, (key_count + 1) * 18 + 2, stories_name[key_count].length() * 7, 18, GxEPD_BLACK);
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
      break;
    }
  }
  getbooksource();
}
void draw_file_dir() {
  int y = 18;
  u8g2Fonts.setFont(chinese_gb2312);
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
  //
  u8g2Fonts.setCursor(2, y);
  u8g2Fonts.print("书籍列表:");
  uint8_t txt_i = 0;
  // 显示目录中文件名以及文件大小
  Dir dir = LittleFS.openDir("/");
  int i = 0;
  String fileName = "";
  while (dir.next())                // dir.next()用于检查目录中是否还有“下一个文件”
  {
    if (i < 6) {
      fileName = dir.fileName();
      if (fileName.endsWith(".txt") && fileName.indexOf("study") == -1) // 检测TXT文件，将名字赋值给txtName
      {
        stories_name[i] = fileName;
        Serial.print("文件:"); Serial.println(fileName);
        i++;
      }
    } else {
      break;
    }
  }
  if (stories_name[0].length() > 5) {
    for (int k = 0; k < i; k++ ) {
      y += 18;
      u8g2Fonts.setCursor(4, y);
      u8g2Fonts.print(stories_name[k]);
    }
    display.nextPage();
    story_function(i);
  } else {
    draw_read_menue();
    select_read_menue();
  }
}
//菜单
void draw_read_menue() {
  float batVCC = getBatVolNew();
  uint8_t bat_bfb = getBatVolBfb(batVCC);
  Serial.println("按钮唤出菜单！");
  //电量显示
  display.setPartialWindow(79, 0, 163, 21); //局刷提示字样
  display.fillScreen(GxEPD_WHITE);
  u8g2Fonts.setFont(chinese_gb2312);
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
  u8g2Fonts.setCursor(82, 18);
  Serial.print("剩余电量：  "); Serial.println(bat_bfb);
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
  u8g2Fonts.print("阅读进度");
  u8g2Fonts.setCursor(9, 60);
  u8g2Fonts.print("文件管理");
  u8g2Fonts.setCursor(9, 79);
  u8g2Fonts.print("更新书库");
  u8g2Fonts.setCursor(9, 98);
  if (EEPROM.read(65) == 1) {
    u8g2Fonts.print("单词模式");
  } else {
    u8g2Fonts.print("名著模式");
  }
  display.nextPage();
  display.setPartialWindow(0, 24, 78, 60);

}
void select_read_menue() {
  int rect_count = 1;
  int mode_count = 1;
  while (true) {
    float batVCC = getBatVolNew();
    if(batVCC<3){
      power_sleep();
    }
    ESP.wdtFeed();//喂狗
    //按键2退出菜单
    if (digitalRead(key2) == LOW) {
      delay(300);
      display.setPartialWindow(0, 0, display.width(), display.height());
      display.fillScreen(GxEPD_WHITE);
      display.nextPage();
      //      draw_story(getpagestr(getstorynum(menuebackpage, index_story_file_name), selected_story_name));
      break;
    }
    //长按确定，短按切换功能
    if (digitalRead(key3) == LOW ) {
      delay(500);
      if (digitalRead(key3) == LOW ) {
        if (mode_count == 2) {
          display.setPartialWindow(0, 0, display.width(), display.height());
          display.fillScreen(GxEPD_WHITE);
          draw_file_dir();//文件管理
        } else if (mode_count == 3) {
          wifi_init();//网络功能
        } else if (mode_count == 1) {
          //阅读进度

        } else {
          //单词模式
          EEPROM.write(65, 2);
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
          Serial.println("小说/阅读功能");
        }
        display.nextPage();
        //      }
      }
      ESP.wdtFeed();//喂狗
    }
  }
  getbooksource();
}
//判断标记位的数位以补相应个数的“0”
int digit_count(int n) {
  int c = 0;
  while (n != 0) {
    c++;
    n /= 10;
  }
  return c;
}
//补位函数，比如这一页首个字符位置在11554，一个五位数，需要补3个“0”变成统一的8位数
String digit_Complement(int n) {
  if (n == 7)return "0";
  else if (n == 6)return "00";
  else if (n == 5)return "000";
  else if (n == 4)return "0000";
  else if (n == 3)return "00000";
  else if (n == 2)return "000000";
}
bool creat_index(String indexfilename) {
  int total_Page = 1;//总页数，建立索引时有用
  int count_char = 0;
  String save_count = "";
  String recent_str = "";
  File indexFile = LittleFS.open(indexfilename, "a");
  File storyFile = LittleFS.open(selected_story_name, "r");
  Serial.println(indexfilename + "小说索引创建成功");
  int line = 0, len_recent_str = 0;//len_recent_str 标记文本长度以处理是否拼满一行
  int y = 16;
  //读取文件内容并且通过串口监视器输出文件信息
  char ch ;
  String s = "";//拼接一行用的
  indexFile.print("00000000");//先写入第一个标记位
  for (int i = 0; i < storyFile.size(); i++) {
    ch = (char)storyFile.read();
    count_char++;
    byte a = B11100000;
    byte b = ch & a;
    if (ch != '\n') {
      s += ch;
    }
    if (b == B11100000) {//中文字符
      ch = (char)storyFile.read();
      s += ch;
      ch = (char)storyFile.read();
      s += ch;
      i += 2;
      count_char += 2;
      len_recent_str += 14;
    } else if (ch >= 0 && ch <= 127 && ch != '\n') {
      len_recent_str += getCharLength(ch) + 1;
    } else if (b == B11000000) {
      ch = (char)storyFile.read();
      i++;
      count_char++;
      s += ch;
      len_recent_str += 6;
    }
    if (len_recent_str > 230 || ch == '\n') {
      s += '\n';
      recent_str += s;
      s = "";
      len_recent_str = 0;
      line++;
    }
    if (line == 8) {
      line = 0;
      save_count = digit_Complement(digit_count(count_char)) + String(count_char);
      Serial.print("添加索引："); Serial.println(save_count);
      indexFile.print(save_count);
      total_Page++;
      recent_str = "";
    }
    ESP.wdtFeed();//喂狗
  }
  //完成文件读取后关闭文件
  storyFile.close();
  indexFile.close();
  Serial.println("索引创建成功！");
  Serial.print("总页数："); Serial.println(total_Page);
}
void draw_story(String string  ) {
  //  display.firstPage();
  Serial.println("小说内容:");
  Serial.print(string);
  int y = 15;
  if (dis_count == 15 ) {
    display.setFullWindow();
    dis_count = 0;
  } else {
    display.setPartialWindow(0, 0, display.width(), display.height());
  }
  display.fillScreen(GxEPD_WHITE);
  u8g2Fonts.setFont(chinese_gb2312);
  u8g2Fonts.setCursor(2, y);
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
  for (int i = 0; i < string.length(); i++) {
    if (string[i] == '\n' ) {
      y += 15;
      i++;
      u8g2Fonts.setCursor(1, y);
    }
    u8g2Fonts.print(string[i]);
  }
  display.nextPage();
  //  display.hibernate();

}
//获取实际小说的位置
int getstorynum(int pagenum, String indexfilename) {
  int num = (pagenum - 1) * 8; //比如第三页，就是2*8=16，在第16个位置开始读,前面0~15为前两页
  //  Serial.print("小说标记num:"); Serial.print(num);
  String page_str = "";
  File indexFile = LittleFS.open(indexfilename, "r");
  if (indexFile.seek(num)) {
    for (int i = 0; i < 8; i++) {
      page_str += (char)indexFile.read();
    }
  }
  //  Serial.print("获取的未处理的位置:"); Serial.print(page_str);
  String result = "";
  int f = 0;
  while (page_str[f] == '0') {
    f++;
  }
  for (int i = f; i < page_str.length(); i++) {
    result += page_str[i];
  }

  //  Serial.print("result:"); Serial.print(result);

  int page_num = conversion_str(result);//0;
  indexFile.close();
  Serial.print("标记位:"); Serial.print(page_num);
  return page_num;
}
//获取当前页小说的内容
String getpagestr(int storynum, String storyfilename) {
  char ch;
  int  line = 0, len_recent_str = 0;//len_recent_str 标记文本长度以处理是否拼满一行
  String recent_str = "", s = "";
  File storyFile = LittleFS.open(storyfilename, "r");
  if (storyFile.seek(storynum)) {
    while (true) {
      ch = (char)storyFile.read();
      byte a = B11100000;
      byte b = ch & a;
      if (ch != '\n') {
        s += ch;
      }
      if (b == B11100000) {//中文字符
        ch = (char)storyFile.read();
        s += ch;
        ch = (char)storyFile.read();
        s += ch;
        len_recent_str += 14;

      } else if (ch >= 0 && ch <= 127 && ch != '\n') {
        len_recent_str += getCharLength(ch) + 1;
      } else if (b == B11000000) {
        ch = (char)storyFile.read();
        s += ch;
        len_recent_str += 6;
      }
      if (len_recent_str > 230 || ch == '\n') {
        s += '\n';
        recent_str += s;
        s = "";
        len_recent_str = 0;
        line++;
      }
      if (line == 8) {
        storyFile.close();
        return recent_str;
      }
      ESP.wdtFeed();//喂狗
    }
  }
  return "";
}
//保存最近打开文件名
void save_story_name() {
  Serial.print("小说文件名:"); Serial.println(selected_story_name);
  for (int i = 66; i < selected_story_name.length() + 66; i++) {
    EEPROM.write(i, selected_story_name[i - 66]);
    EEPROM.commit();
  }
  Serial.println("小说名保存成功！");
}
//获取上次打开文件名
String get_story_name() {
  String name_story  = "";
  for (int i = 66; i < 100; i++ ) {
    name_story += char(EEPROM.read(i));
  }
  if (name_story.indexOf(".txt") == -1) {
    Serial.println("首次打开，无文档记录，重置中...");
    for (int i = 66; i < 100; i++) {
      EEPROM.write(i, 0);
      EEPROM.commit();
    }
    Serial.println("重置完成，打开文件管理！");
    draw_file_dir();
    return "";
  } else {
    for (int i = 66; i < 100; ++i) {
      name_story += char(EEPROM.read(i));
    }
  }
  if (LittleFS.exists(name_story)) {
    selected_story_name = name_story;
    index_story_file_name = selected_story_name.substring(0, selected_story_name.indexOf(".txt")) + ".md";
    String ttt = "";
    for (int i = 1; i < index_story_file_name.length(); i++) {
      ttt += index_story_file_name[i];
    }
    story_progress_name = "/progress" + index_story_file_name;
    Serial.print("本次书目：");
    Serial.println(name_story);
    getbooksource();
    return name_story;
  } else {
    Serial.println("书籍不存在，重置中...");
    for (int i = 66; i < 100; i++) {
      EEPROM.write(i, 0);
      EEPROM.commit();
    }
    Serial.println("重置完成，打开文件管理！");
    draw_file_dir();
    return "";
  }
}
void getbooksource() {
  String save_count = "";
  int sleep_count = 0;
  int page = getprogress(story_progress_name);//EEPROM.read(64);//上次记录
  int count_char = 0;
  File indexFile = LittleFS.open(index_story_file_name, "r");
  if (indexFile.size() < 16) {
    page = 1;
    indexFile.close();
    saveprogress(story_progress_name, String(page));
    Serial.println("首次打开需要创建索引！");
    display.drawInvertedBitmap(0, 0, synchro_pic, 250, 122, GxEPD_BLACK);
    display.nextPage();
    creat_index(index_story_file_name);
    dis_count = 15;
  } else {
    Serial.println("无需创建索引");
  }
  Serial.print("上次读到的页数:"); Serial.println(page);
  display.setPartialWindow(0, 0, 250, 122);
  if (page != 1) {
    page--;
  }
  draw_story(getpagestr(getstorynum(page, index_story_file_name), selected_story_name));
  while (true) {
    float batVCC = getBatVolNew();
    if (batVCC < 3) {
      power_sleep();
    }
    ESP.wdtFeed();//喂狗
    if (digitalRead(key3) == LOW) {
      delay(300);
      if (digitalRead(key3) == LOW) {
        if (page > 0)page--;
        draw_read_menue();
        select_read_menue();
      } else {
        sleep_count = 0; //休眠标记重置
        dis_count++;//局刷/全刷
        page++;//进度标记
        draw_story(getpagestr(getstorynum(page, index_story_file_name), selected_story_name));
        saveprogress(story_progress_name, String(page));
      }
    }
    if (digitalRead(key2) == LOW) {
      delay(100);
      sleep_count = 0; //休眠标记重置
      dis_count++;//局刷/全刷
      page--;//进度标记
      if (page < 1) {
        display.setFullWindow();
        display.fillScreen(GxEPD_WHITE);
        u8g2Fonts.setCursor(20, 70);
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
        u8g2Fonts.print("当前为第一页！不可向前翻页！");
        u8g2Fonts.setCursor(34, 100);
        u8g2Fonts.print("设备将在2秒后回到第一页！");
        page = 1;
        display.nextPage();
        delay(2000);
        draw_story(getpagestr(getstorynum(page, index_story_file_name), selected_story_name));
        continue;
      }
      draw_story(getpagestr(getstorynum(page, index_story_file_name), selected_story_name));
    }
    delay(100);
    sleep_count++;
    if (sleep_count * 0.1 > 300) {
      //      display.init();
      display.setFullWindow();
      //      display.firstPage();
      display.fillScreen(GxEPD_BLACK);
      display.drawInvertedBitmap(0, 0, sleep_pic, 250, 122, GxEPD_WHITE);
      display.nextPage();
      delay(500);
      ESP.deepSleep(0);
    }
  }
}
