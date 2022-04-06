# 2in13e-ink-word-card
## 新增固件支持无线传书，无线更新系统，动态生成网络二维码，配置更方便
## 1.说明
低成本平替喵喵机单词卡
本作品基于汉朔2.13价签实现了便携单词卡的核心功能，目前可以达到“能用”。本项目代码开放，有能力的同学可以在本项目的基础上实现功能拓展
## 2.操作逻辑
**按钮从左向右依次为1键，2键，3键**

1. 开机默认同步上次读取的位置，同步成功后可按3键实现单词查阅；
2. 长按3键可唤出菜单，3键短按切换选项，长按确认
3. 默写模式下先显示读音及释义，不显示单词，点击3键可查看单词，阅读模式按下3键则直接进入下一个单词
4. 按下2键可将当前词条加入生词本
5. 按下3键后按下2键可重启设备并切换到生词本或词库
6. 词库读完后会重置标记位，可以重新启动实现从头来过或更换词源
7. 切记不可以在屏幕刷新时按2键，这样会导致屏幕卡死，这是因为2键和屏幕共用了一个io口，下个版本将解决

## 3.硬件

硬件地址：https://oshwhub.com/zhushengji/2-13words
本硬件为老款硬件，新版本硬件完成后功能上将有一些调整

## 4.软件

感谢 甘草酸不酸 提供的修改库U8g2_for_Adafruit_GFX及汉字字库

1. 需要将本项目提供的U8g2_for_Adafruit_GFX库替换官方的U8g2_for_Adafruit_GFX库！
2. 第一次烧录需要重置eeprom，因为该位置默认值我255，如果不做处理会直接从255位置开始阅读，所以需要将49~52行取消注释上传一遍!（已修复，无需二次上传）[图片](https://user-images.githubusercontent.com/32239713/159401172-e15e5f80-4c89-4f9a-b4e3-98a89f70f83e.png)

3. 烧录完成后将49~52行注释掉再次上传，这样就可以正常保存进度了
4. 关于词库制作，我提供了一个java脚本，可以将txt文本格式化为单词卡支持的格式，但格式必须是“单词 音标 释义”的形式
5. 上传词库需要在 文档--》Arduino目录下新建文件夹“tools”并将本项目提供的tools文件夹中内容放进去，重启Arduino IDE，在工具下即可看到![图片](https://user-images.githubusercontent.com/32239713/159401123-c598ce18-4d4e-4de0-a9ad-e0986b418ee7.png)

6. 需要将词库进行格式化处理，本人提供了一个脚本，可实现自动加音标+格式化
7. 一切完成后即可好好学习啦~~~

​		

![图片](https://user-images.githubusercontent.com/32239713/159209550-265dac6f-3148-4d76-aae0-9b929021ff41.png)
![图片](https://user-images.githubusercontent.com/32239713/159209559-b1f59d90-5b27-4923-a324-4fccfb4cf89e.png)
![图片](https://user-images.githubusercontent.com/32239713/159209562-58d1b9a1-3805-4f9c-a024-9cd08fbf3864.png)
![图片](https://user-images.githubusercontent.com/32239713/159209565-1072ad2e-00e5-4b69-b224-f51866ade30a.png)
![图片](https://user-images.githubusercontent.com/32239713/159209569-ce56a6ff-7d87-4ed4-b142-da4a573168eb.png)
![图片](https://user-images.githubusercontent.com/32239713/159209574-174a7916-9fc6-4ef4-b975-4992a1e07a98.png)
