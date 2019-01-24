/*
 Example sketch for the PS3 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS3BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <Servo.h>

Servo myservov;
float posv = 90;

Servo myservoh;
float posh = 90;

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance

bool printTemperature, printAngle;

#include "digital_pwm.h"

void start_Digital_PWMout()
{
  MsTimer2::set(1,pwm_int);//1ms timer
  MsTimer2::start();
}
void setup() {
  Serial.begin(115200);
  attach_motor(0,4,3);
  attach_motor(1,6,7);
  attach_motor(2,8,14);
  attach_motor(3,2,5);
  start_Digital_PWMout();

//Servo setup
  myservov.attach(16);
  myservoh.attach(18);

//Bluetooth setup
  
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
}

void loop() {

Usb.Task();
  
  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    
    //左ジョイスティック：モーター制御
    if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || 
        PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117) {

    //角度計算±180°
    float degree = (180/PI)*atan2((PS3.getAnalogHat(LeftHatY)-127.5),(PS3.getAnalogHat(LeftHatX)-127.5));
      Serial.print(degree);
      Serial.print(F("\r\n"));
      if (degree >= -22.5 && degree < 22.5){
        Serial.print(F("\t\nR"));
        set_motor(0,CW,25);
        set_motor(1,CW,25);
        set_motor(2,CCW,25);
        set_motor(3,CCW,25);
      }
      if (degree >= 22.5 && degree < 67.5){
        Serial.print(F("\t\nRB"));
        set_motor(0,FREE,25);
        set_motor(1,CW,25);
        set_motor(2,FREE,25);
        set_motor(3,CCW,25);
      }
      if (degree >= 67.5 && degree < 112.5){
        Serial.print(F("\t\nB"));
        set_motor(0,CCW,25);
        set_motor(1,CW,25);
        set_motor(2,CW,25);
        set_motor(3,CCW,25);
      }
      if (degree >= 112.5 && degree < 157.5){
        Serial.print(F("\t\nLB"));
        set_motor(0,CCW,25);
        set_motor(1,FREE,25);
        set_motor(2,CW,25);
        set_motor(3,FREE,25);
      }
      if ((degree >= 157.5 && degree < 180.0) || (degree < -157.5 && degree >= -180.0)){
        Serial.print(F("\t\nL"));
        set_motor(0,CCW,25);
        set_motor(1,CCW,25);
        set_motor(2,CW,25);
        set_motor(3,CW,25);
      }
      if (degree < -112.5 && degree >= -157.5){
        Serial.print(F("\t\nLF"));
        set_motor(0,FREE,25);
        set_motor(1,CCW,25);
        set_motor(2,FREE,25);
        set_motor(3,CW,25);
      }
      if (degree < -67.5 && degree >= -112.5){
        Serial.print(F("\t\nF"));
        set_motor(0,CW,25);
        set_motor(1,CCW,25);
        set_motor(2,CCW,25);
        set_motor(3,CW,25);
      }
      if (degree < -22.5 && degree >= -67.5){
        Serial.print(F("\t\nRF"));
        set_motor(0,CW,25);
        set_motor(1,FREE,25);
        set_motor(2,CCW,25);
        set_motor(3,FREE,25);
      }
    } 

    //左ジョイスティック未入力時
    else if (PS3.getAnalogHat(LeftHatX) < 137 || PS3.getAnalogHat(LeftHatX) > 117 || 
        PS3.getAnalogHat(LeftHatY) < 137 || PS3.getAnalogHat(LeftHatY) > 117) {
            set_motor(0,FREE,15);
            set_motor(1,FREE,15);
            set_motor(2,FREE,15);
            set_motor(3,FREE,15);
        }

    //右ジョイスティック：サーボ
    if (PS3.getAnalogHat(RightHatX) > 205 || PS3.getAnalogHat(RightHatX) < 50) {
        if (PS3.getAnalogHat(RightHatX) > 205){
          if (posh > 10.0){
            posh -= 0.01;
            //Serial.print(F("\r\nR"));
          }
        }
        if (PS3.getAnalogHat(RightHatX) < 50){
          if (posh < 170){
            posh += 0.01;
            //Serial.print(F("\r\nL"));
          }
        } 
    }

    if (PS3.getAnalogHat(RightHatY) > 205 || PS3.getAnalogHat(RightHatY) < 50) {
        if (PS3.getAnalogHat(RightHatY) > 205){
          if (posv > 10.0){
            posv -= 0.01;
            //Serial.print(F("\r\nR"));
          }
        }
        if (PS3.getAnalogHat(RightHatY) < 50){
          if (posv < 170){
            posv += 0.01;
            //Serial.print(F("\r\nL"));
          }
        } 
    }

    /*
    //右スイッチ：サーボリセット
    if (PS3.getButtonClick(CROSS)){
        pos = 90;
        Serial.print(F("\r\nS"));
    }
    */
    
    myservov.write(posv);
    myservoh.write(posh);

    //旋回同時押し：停止
    if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
      if (PS3.getAnalogButton(L2) != 0 && PS3.getAnalogButton(R2) != 0){
        Serial.print(F("\r\n\STOP "));
        //モーター止める
      }
      else {
        //L2：左旋回
        if (PS3.getAnalogButton(L2) > 50) {
          Serial.print(F("\r\nL2: "));
          Serial.print(PS3.getAnalogButton(L2));
          float left_power=(float)(PS3.getAnalogButton(L2)/255.0)*25.0;
          set_motor(0,CCW,left_power);
          set_motor(1,CCW,left_power);
          set_motor(2,CCW,left_power);
          set_motor(3,CCW,left_power);
        }
    
        //R2：右旋回
        if (PS3.getAnalogButton(R2) > 50) {
          Serial.print(F("\t\nR2: "));
          Serial.print(PS3.getAnalogButton(R2));
          float right_power=(float)(PS3.getAnalogButton(R2)/255.0)*25.0;
          set_motor(0,CW,right_power);
          set_motor(1,CW,right_power);
          set_motor(2,CW,right_power);
          set_motor(3,CW,right_power);
        }
      }
    }

    //電源OFF
    if (PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    }
  }

  //通信切れ非常停止
  else {
      set_motor(0,BRAKE,15);
      set_motor(1,BRAKE,15);
      set_motor(2,BRAKE,15);
      set_motor(3,BRAKE,15);
  }
}


