/*
 *******************************************************************************
 *  MapleFreeRTOS Basic Example (CM-900 version)
 *******************************************************************************
 *  The program operates 2 tasks, blinking LED and printing "hello world" via SerialUSB
 *  To operate task, just use 2 API, xTaskCreat() and vTaskStartScheduler()    
 *  This is based on MapleFreeRTOS. you can learn it from www.freertos.org    
 *      
 *  Made by ROBOTIS 2012-12-29 Sangmin lee  
 *    
 *    
 *******************************************************************************
 */

//It must be needed to use FreeRTOS
#include <MapleFreeRTOS.h>

#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04

#define amountOfServos 6
#define min_speed 0

uint16 pos_servo[amountOfServos] = {190, 210, 512, 500, 230, 813};
uint8 motor[4] = {2, 3, 4, 5};
uint16 sensorvalue = 0;
uint8 motor_speed = 200;
uint8 servo_speed = 10;

uint8 selector = 0;
uint8 motorsToMove = 0;

Dynamixel Dxl(DXL_BUS_SERIAL1);

void setup(){
  //configMINIMAL_STACK_SIZE is stack size in which insert your task code
  //so it is expressed in bytes.
  Dxl.begin(3);
  for(uint8 i = 1; i <= amountOfServos; i++)
  {
    Dxl.goalSpeed(i, 400);
    Dxl.jointMode(i);
    Dxl.goalPosition(i, pos_servo[i - 1]);
  }
  
  for(uint8 i = 0; i < 4; i++)
  {
    pinMode(motor[i], OUTPUT);
  }
  //pinMode(0, INPUT_ANALOG);
  xTaskCreate( vUartTask, ( signed char * ) "Uart", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
  xTaskCreate( vMotorTask, ( signed char * ) "Motor", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
  xTaskCreate( vArmTask, ( signed char * ) "Arm", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
  //start run tasks
  vTaskStartScheduler();
}
void loop(){
}

void vArmTask( void *pvParameters )
{
  for( ;; ){
    for(uint8 i = 0; i < amountOfServos; i++)
    {
       Dxl.goalPosition(i + 1, pos_servo[i]);
    }
    delay(100);
  }
}

void vMotorTask( void *pvParameters )
{
  for( ;; )
  {
    if(selector == 3)
    {
      switch(motorsToMove)
      {
        case 0:
          for(uint8 i = 0; i < 4; i++)
          {
            digitalWrite(motor[i], 0);
          }
        break;
        case 1:
          for(uint8 i = 0; i < 4; i++)
          {
            digitalWrite(motor[i], motor_speed);
          }
        break;
        case 2:
          for(uint8 i = 0; i < 2; i++)
          {
            digitalWrite(motor[i], motor_speed / 2);
            digitalWrite(motor[i + 2], -motor_speed / 2);
          }
        break;
        case 3:
          for(uint8 i = 0; i < 2; i++)
          {
            digitalWrite(motor[i], -motor_speed / 2);
            digitalWrite(motor[i + 2], motor_speed / 2);
          }
        break;
      }
    }
    delay(100);
  } 
}

void vUartTask( void *pvParameters )
{
  uint8 message = 0;


  SerialUSB.begin();
  for( ;; )
  {
    if(SerialUSB.available())
    {
      message = SerialUSB.read();
      selector = message >> 5;
      motorsToMove = message & 0x1F;
    }
    
    switch(selector)
    {
      case 1:
        for(uint8 i = 0; i < amountOfServos; i++)
        {
          if(motorsToMove & (2^i))
          {
            pos_servo[i] += servo_speed;
          }
        } 
      break;
      case 2:
        for(uint8 i = 0; i < amountOfServos; i++)
        {
          if(motorsToMove & (2^i))
          {
            pos_servo[i] -= servo_speed;
          }
        } 
      break;
      case 3:
        switch(motorsToMove)
        {
          case 4:
            if(motor_speed < 255)
              motor_speed += 51;
            if(motor_speed > 255)
              motor_speed = 255;
          break;
          case 5:
            if(motor_speed > min_speed)
              motor_speed -= 51;
            if(motor_speed < min_speed)
              motor_speed = min_speed;
          break;
        }
      break;
    }
    
    delay(100);
  }
}
