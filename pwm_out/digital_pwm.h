#include <MsTimer2.h>
/*********************
 * introduction
 * start_Digital_PWMout() : start function
 * 最もはじめに宣言すること 済
 * 
 * attach_motor(unsigned char index,unsigned char pin_1,unsigned char pin_2)
 * setup内で宣言しておくこと
 * 文字通りモーターをarduinoに取り付けるイメージ　初期設定
 * indexは通し番号で0から順に宣言すること　済
 * pin_1,pin_2はそのモータがつながっているピン番号を入力すること
 * 重複は特に弾かないので注意
 * モーターは初期値として出力０　ブレーキモードとなる
 *
 *
 * set_motor(unsigned char index,MOTOR_MODE mode,unsigned char power)
 * モーターへの出力を実際に設定する関数
 * indexはattach_motor関数内で指定したモーター番号
 * MOTOR_MODE mode は以下の値を入力する
 * CW :正転
 * CCW:逆転
 * BRAKE:ブレーキ
 * FREE:フリー
 * 
 * powerは出力の大きさ
 * 0~25
 */

 enum MOTOR_MODE
{
  CW,
  CCW,
  BRAKE,
  FREE
};

struct MOTOR_t
{
  unsigned char pin_1;
  unsigned char pin_2;
  unsigned char mode;
  unsigned char power;
};


/*****************
 * mode:
 * 0:CW
 * 1:CCW
 * 2:BRAKE
 * 3:FREE
 * 
 * power:
 * MIN:0
 * MAX:25
 *********************/
MOTOR_t my_motor[10];
char num_attached_motor=3;

void pwm_int()//called by timer2  every1ms
{
  static char cnt;
  cnt ++;
  if(cnt > 25) cnt = 0;//make 25ms timing
  
  for(int i = 0;i <= num_attached_motor;i ++)
  {
   // Serial.println(my_motor[i].power);
    if(my_motor[i].mode == BRAKE)
    {
    //  Serial.println(my_motor[i].pin_1);
      digitalWrite(my_motor[i].pin_1,HIGH);
      digitalWrite(my_motor[i].pin_2,HIGH);
    }
    else if(cnt < my_motor[i].power)
    {
      //Serial.println(my_motor[i].pin_1);
      switch(my_motor[i].mode)
      {
      case CW:
        digitalWrite(my_motor[i].pin_1,HIGH);
        digitalWrite(my_motor[i].pin_2,LOW);
        break;

      case CCW:
        digitalWrite(my_motor[i].pin_1,LOW);
        digitalWrite(my_motor[i].pin_2,HIGH);
        break;

      case BRAKE:
        digitalWrite(my_motor[i].pin_1,HIGH);
        digitalWrite(my_motor[i].pin_2,HIGH);
        break;

      default:
        digitalWrite(my_motor[i].pin_1,LOW);
        digitalWrite(my_motor[i].pin_2,LOW);
        break;
      }
    }
    else
    {        
    //  Serial.println(my_motor[i].pin_1);
      digitalWrite(my_motor[i].pin_1,LOW);
      digitalWrite(my_motor[i].pin_2,LOW);
    }

  }
}
void attach_motor(unsigned char index,unsigned char pin_1,unsigned char pin_2)
{
  pinMode(pin_1,OUTPUT);
  pinMode(pin_2,OUTPUT);
  my_motor[index].pin_1 = pin_1;
  my_motor[index].pin_2 = pin_2;
  Serial.println(my_motor[index].pin_1);
  my_motor[index].mode = 2;//brake mode
  my_motor[index].power = 0;
 // num_attached_motor = index;
}

void set_motor(unsigned char index,MOTOR_MODE mode,unsigned char power)
{
  my_motor[index].mode = mode;
  my_motor[index].power = power;
}


