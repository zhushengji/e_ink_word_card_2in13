# 2in13e-ink-word-card
# 未经允许，不可商用！仅限个人学习交流
## 重要更新！
### 1.修改按键操作逻辑：
a.单词模式下双击添加生词（不用很快）
b.单击肩键下一条词条
c.单击正面上为上一条
d.单击正面下为菜单/确认
e.在操作界面，正面上为返回/取消功能
### 2.新增记事本功能
a.无记录显示默认待机画面
b.有笔记记录显示备忘录
c.不提交空内容的情况下默认显示上次提交的内容
d.需要将html文件放到tf卡的根目录下
### 3.采用激活码机制
由于一些稀奇古怪之前未曾预料的原因，最新固件改为5元永久激活制，联系QQ：849059687 夏襄居士

### 历史更新
最新源码、固件与工具已发布，支持多图书进度保存，但是体验版只支持英文文件名且文件名不能太长（超过16个字）。文件目录应注意，词库放到study文件夹下，小说放到根目录下，更新小说的时候注意有个选项需要勾选。阅读进度功能是空的，不要选。
## 1.说明
低成本平替喵喵机单词卡
本作品基于汉朔2.13价签实现了便携单词卡的核心功能，目前可以达到“能用”。本项目代码开放，有能力的同学可以在本项目的基础上实现功能拓展
## 2.操作逻辑
![efde39f7f89afcdd31caf39f79a47b0](https://user-images.githubusercontent.com/32239713/167846110-2a1d1754-c4eb-49b0-b49c-bc68a7e4323c.jpg)


## 3.硬件

老硬件地址：https://oshwhub.com/zhushengji/2-13words

新硬件地址：https://oshwhub.com/zhushengji/213-mo-shui-ping-dan-ci-ka-tf-ban-ben

## 4.软件

感谢 甘草酸不酸 提供的修改库U8g2_for_Adafruit_GFX及汉字字库

1. 需要将本项目提供的U8g2_for_Adafruit_GFX库替换官方的U8g2_for_Adafruit_GFX库！
2. 关于词库制作，我提供了一个java脚本，可以将txt文本格式化为单词卡支持的格式，但格式必须是“单词 音标 释义”的形式
3. 上传词库需要在 文档--》Arduino目录下新建文件夹“tools”并将本项目提供的tools文件夹中内容放进去，重启Arduino IDE，在工具下即可看到
4. 需要将词库进行格式化处理，本人提供了一个脚本，可实现自动加音标+格式化
5. 一切完成后即可好好学习啦~~~
## 实物图
![(18SGC6{DX)%1BHC93V`9YI](https://user-images.githubusercontent.com/32239713/170679970-26f1e835-d82a-45cc-959e-1a2c2614b649.jpg)

![c5c797ab85473ec7171d66e850a8c85](https://user-images.githubusercontent.com/32239713/167846542-5b24ebb7-7e59-4409-a604-877e4896b043.jpg)
![29a285a1a464fb348deb51d04cb75d4](https://user-images.githubusercontent.com/32239713/167846556-ceeea8a0-3d2e-41e6-b085-fbdf9236b299.jpg)
![6f2aa6c022cd8c703c12ba3c948396e](https://user-images.githubusercontent.com/32239713/167846569-ec5ff1a5-61f0-4c39-9721-9e5f0e1fa89b.jpg)
![1bc267e912b1ad8b538dd01ccf67ab5](https://user-images.githubusercontent.com/32239713/167846578-83825b64-21f4-41a9-8b69-19151cf1b281.jpg)

