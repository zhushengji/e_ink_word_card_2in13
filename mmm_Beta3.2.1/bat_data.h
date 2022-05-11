//采集电池电压
//采用时间平均滤波
uint8_t bat_count = 0;      //采样计数
#define bat_vcc_cycs 3          //采样次数
#define bat_vcc_cysj 3000       //采样时间间隔，ms
float bat_adc_new = 0.0;
uint32_t bat_vcc_time_old = 0.0;
void get_bat_vcc() //时间平均滤波的电压
{
  if (millis() - bat_vcc_time_old >= bat_vcc_cysj)
  {
    pinMode(bat_switch_pin, OUTPUT);
    digitalWrite(bat_switch_pin, 1);
    bat_count++;
    bat_adc_new += analogRead(bat_vcc_pin);
    if (bat_count >= bat_vcc_cycs)
    {
      bat_vcc = (bat_adc_new / bat_vcc_cycs) * 0.0009765625 * 5.607; //电池电压系数
      bat_adc_new = 0.0;
      bat_count = 0;
    }
    digitalWrite(bat_switch_pin, 0); //关闭电池测量
    //pinMode(bat_switch_pin, INPUT);  //改为输入状态避免漏电
    bat_vcc_time_old = millis();
  }
}
float getBatVolNew() //即时的电压
{
  pinMode(bat_switch_pin, OUTPUT);
  digitalWrite(bat_switch_pin, 1);
  delay(1);
  float vcc_cache = 0.0;
  for (uint8_t i = 0; i < 30; i++)
  {
    //delay(1);
    vcc_cache += analogRead(bat_vcc_pin) * 0.0009765625 * 5.607;
  }
  digitalWrite(bat_switch_pin, 0); //关闭电池测量
  pinMode(bat_switch_pin, INPUT);  //改为输入状态避免漏电
  return (vcc_cache / 30);
}
double getBatVolBfb(double batVcc) //获取电压的百分比，经过换算并非线性关系
{
  double bfb = 0.0;
  //y = 497.50976 x4 - 7,442.07254 x3 + 41,515.70648 x2 - 102,249.34377 x + 93,770.99821
  bfb = 497.50976 * batVcc * batVcc * batVcc * batVcc
        - 7442.07254 * batVcc * batVcc * batVcc
        + 41515.70648 * batVcc * batVcc
        - 102249.34377 * batVcc
        + 93770.99821;
  if (bfb > 100) bfb = 100.0;
  else if (bfb < 0) bfb = 3.0;
  return bfb;
}
