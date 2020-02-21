/* Serial device defines for dxl bus */
#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 2  //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 3

/* Dynamixel ID defines */
/* Control table defines */
#define GOAL_SPEED 32
#define CCW_Angle_Limit 8
#define CONTROL_MODE 11

#include <Servo.h> 
Servo myservo;
int pos_servo5 = 190;
int pos_servo6 = 210;//800; //servo 6
int pos_servo9 = 512;
int pos_servo10 = 500;
int pos_servo8 = 230;
int pos_servo7 = 813; //servo 7
int relay = 10;
int sensorvalue = 0;
int speed = 1000;
int speed_brazo = 10;
int servoNum = 1;
char message = '0';         // a string to hold incoming data
boolean inputComplete = false;

HardwareTimer Timer(1);
Dynamixel Dxl(DXL_BUS_SERIAL1);

void setup() {
  // Dynamixel 2.0 Protocol -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps 
  Dxl.begin(3);
  SerialUSB.begin();
  //pinMode(BOARD_LED_PIN, OUTPUT);
  //AX MX RX Series
  Dxl.writeWord(1, CCW_Angle_Limit, 0); 
  Dxl.writeWord(2, CCW_Angle_Limit, 0);
  Dxl.writeWord(3, CCW_Angle_Limit, 0); 
  Dxl.writeWord(4, CCW_Angle_Limit, 0);
  Dxl.goalSpeed(5, 400);
  Dxl.jointMode(5);
  Dxl.goalSpeed(6, 400);
  Dxl.jointMode(6);
  Dxl.goalSpeed(7, 400);
  Dxl.jointMode(7);
  Dxl.goalSpeed(8, 400);
  Dxl.jointMode(8);
  Dxl.goalSpeed(9, 400);
  Dxl.jointMode(9); 
  Dxl.goalSpeed(10, 400);
  Dxl.jointMode(10); 
  Dxl.goalPosition(5, pos_servo5);
  Dxl.goalPosition(6, pos_servo6);
  Dxl.goalPosition(7, pos_servo7);
  Dxl.goalPosition(9, pos_servo9);
  Dxl.goalPosition(10, pos_servo10);
  Dxl.goalPosition(8, pos_servo8);
  pinMode(0, INPUT_ANALOG);
  sensorvalue = analogRead(0);
  SerialUSB.print(analogRead(0));
  //disable CCW Angle Limit(L) to use wheel mode

  //XL-320
  //Dxl.writeByte(ID_NUM, CONTROL_MODE, 1);
}


void loop () {
  //lect();
  if (SerialUSB.available())
  { 
    message = (char)SerialUSB.read();
    if(message !='0')
    {
      SerialUSB.println(message);
      switch (message){
      case 'w': //adelante 
        //inserte instruccion 
        //SerialUSB.println("yay");
        digitalWrite(BOARD_LED_PIN, LOW);
        Dxl.writeWord(4, GOAL_SPEED, speed); 
        Dxl.writeWord(3, GOAL_SPEED, speed | 0x400);
        Dxl.writeWord(2, GOAL_SPEED, speed);
        Dxl.writeWord(1, GOAL_SPEED, speed | 0x400);
        break;
      case 's': //atras
        //inserte instruccion
        Dxl.writeWord(4, GOAL_SPEED, speed | 0x400);
        Dxl.writeWord(3, GOAL_SPEED, speed);
        Dxl.writeWord(2, GOAL_SPEED, speed | 0x400); 
        Dxl.writeWord(1, GOAL_SPEED, speed);
        break;
      case 'd': //derecha
        //inserte instruccion
        Dxl.writeWord(4, GOAL_SPEED, speed); 
        Dxl.writeWord(2, GOAL_SPEED, speed);
        Dxl.writeWord(3, GOAL_SPEED, speed);
        Dxl.writeWord(1, GOAL_SPEED, speed);
        break;
      case 'a': //izquierda
        //inserte instruccion
        Dxl.writeWord(4, GOAL_SPEED, speed | 0x400); 
        Dxl.writeWord(2, GOAL_SPEED, speed | 0x400);
        Dxl.writeWord(3, GOAL_SPEED, speed | 0x400); 
        Dxl.writeWord(1, GOAL_SPEED, speed | 0x400);
        break;
        
        case '4'://Rot
        if (pos_servo5 < 1020){
          pos_servo5 += speed_brazo;
          Dxl.goalPosition(5, pos_servo5);
          delay(10);
        }
        break;
        
      case '6'://Rot
        if (pos_servo5>0){
          pos_servo5 -= speed_brazo;
          Dxl.goalPosition(5, pos_servo5);
          delay(10);
        }
        break;

      case '8'://Brazo
        if (pos_servo6 < 803){
          pos_servo6 = pos_servo6+speed_brazo;
          pos_servo7 = pos_servo7-speed_brazo;
          Dxl.goalPosition(6, pos_servo6);
          Dxl.goalPosition(7, pos_servo7);
          delay(10);
        }
        else{
          pos_servo6 = 813;
          pos_servo7 = 210;
          Dxl.goalPosition(6, pos_servo6);
          Dxl.goalPosition(7, pos_servo7);
          delay(10);
        }
        break;
        
      case '2'://Brazo
        if (pos_servo6>200){
          pos_servo6 = pos_servo6-speed_brazo;
          pos_servo7 += 10;
          Dxl.goalPosition(6, pos_servo6);
          Dxl.goalPosition(7, pos_servo7);
          delay(10);
        }
        else{
          pos_servo6 = 210;
          pos_servo7 = 813;
          Dxl.goalPosition(6, pos_servo6);
          Dxl.goalPosition(7, pos_servo7);
          delay(10);
        }
        break;

      case '7'://muñeca
        if (pos_servo9 < 1022){
          pos_servo9 = pos_servo9+speed_brazo;
          Dxl.goalPosition(9, pos_servo9);
          delay(10);
        }
        break;
      case '9'://muñeca
        if (pos_servo9>2){
          pos_servo9 = pos_servo9-speed_brazo;
          Dxl.goalPosition(9, pos_servo9);
          delay(10);
        }
        break;

      case 'e': //Garra
        if (pos_servo10 < 950){
          pos_servo10 = pos_servo10+50;
          Dxl.goalPosition(10, pos_servo10);
        }
        break;
      case 'q': //Garra
        if (pos_servo10 > 500){
          pos_servo10 = pos_servo10-50;
          Dxl.goalPosition(10, pos_servo10);
        }
        break; 
        
        case '3'://Codo
        if (pos_servo8 < 800){
          pos_servo8 = pos_servo8+speed_brazo;
          Dxl.goalPosition(8, pos_servo8);
          delay(10);
        }
        break;
      case '1'://Codo
        if (pos_servo8>230){
          pos_servo8 = pos_servo8-speed_brazo;
          Dxl.goalPosition(8, pos_servo8);
          delay(10);
        }
        break;


      case 'i'://velocidad más baja
        if(speed + 200 >= 900){
          speed = 1000;
        }
        else{
          speed += 200;
        }
        break;
      case 'u'://velocidad media baja
        if(speed - 200 <= 300){
          speed = 300;
        }
        else{
          speed -= 200;
        }
        break;
      case 'y'://velocidad media alta
        if(speed_brazo + 5 >= 25){
          speed_brazo = 25;
        }
        else{
          speed_brazo += 5;
        }
        break;
      case 't'://velocidad alta
        if(speed_brazo - 5 <= 5){
          speed_brazo = 5;
        }
        else{
          speed_brazo -= 5;
        }
        break;

      case 'p':
         pos_servo5 = 190;
         pos_servo6 = 210;//800; //servo 6
         pos_servo9 = 512;
         pos_servo10 = 500;
         pos_servo8 = 230;
         pos_servo7 = 813; //servo 7
         Dxl.goalPosition(6, pos_servo6);
         Dxl.goalPosition(7, pos_servo7);
         delay(100);
        break;
      case 'j':
         pos_servo5 = 800;
         pos_servo6 = 210;//800; //servo 6
         pos_servo9 = 512;
         pos_servo10 = 500;
         pos_servo8 = 230;
         pos_servo7 = 813; //servo 7
        break;
      case 'o': //pos recto
         pos_servo5 = 190;
         pos_servo7 = 310;//800; //servo 6
         pos_servo9 = 512;
         pos_servo10 = 500;
         pos_servo8 = 800;
         pos_servo6 = 713; //servo 7
       break;
  
/*
      case'j': 
        Dxl.goalSpeed(5, 300);
        Dxl.jointMode(5);
        Dxl.goalSpeed(6, 150);
        Dxl.jointMode(6);
        Dxl.goalSpeed(7, 300);
        Dxl.jointMode(7);
        Dxl.goalSpeed(9, 150);
        Dxl.jointMode(9);
         Dxl.goalSpeed(10, 150);
        Dxl.jointMode(10);
        pos = 510;
        Dxl.goalPosition(5, pos);
        delay(500);
        pos_servo6 = 480;
        Dxl.goalPosition(6, pos_servo6);
        delay(500);
        pos_servo9 = 206;
        Dxl.goalPosition(7, pos_servo9);
        pos_servo8 = 230;
        Dxl.goalPosition(9, pos_servo8);
        delay(1000);
        Dxl.goalSpeed(5, 500);
        Dxl.jointMode(5);
        Dxl.goalSpeed(6, 500);
        Dxl.jointMode(6);
        Dxl.goalSpeed(7, 500);
        Dxl.jointMode(7);
        Dxl.goalSpeed(9, 500);
        Dxl.jointMode(9);
        break;
        
      case'k':
        pos_servo6=206;
        Dxl.goalSpeed(6, 300);
        Dxl.goalPosition(6, pos_servo6);
        Dxl.goalSpeed(6, 400);
        pos_servo9 = 206;
        Dxl.goalSpeed(7, 300);
        Dxl.goalPosition(7, pos_servo9);
        Dxl.goalSpeed(7, 400); 
        pos_servo10= 500;
        Dxl.goalPosition(8, pos_servo10);
        break;
      
      case'l': 
        Dxl.goalSpeed(5, 300);
        Dxl.jointMode(5);
        Dxl.goalSpeed(6, 150);
        Dxl.jointMode(6);
        Dxl.goalSpeed(7, 300);
        Dxl.jointMode(7);
        Dxl.goalSpeed(9, 150);
        Dxl.jointMode(9);
        pos = 510;
        Dxl.goalPosition(5, pos);
        delay(500);
        pos_servo6 = 206;
        Dxl.goalPosition(6, pos_servo6);
        delay(500);
        pos_servo9 = 480;
        Dxl.goalPosition(7, pos_servo9);
        pos_servo8 = 750;
        Dxl.goalPosition(9, pos_servo8);
        delay(1000);
        Dxl.goalSpeed(5, 500);
        Dxl.jointMode(5);
        Dxl.goalSpeed(6, 500);
        Dxl.jointMode(6);
        Dxl.goalSpeed(7, 500);
        Dxl.jointMode(7);
        Dxl.goalSpeed(9, 500);
        Dxl.jointMode(9);
        break;
      
      case'5': 
        Dxl.goalSpeed(5, 300);
        Dxl.jointMode(5);
        Dxl.goalSpeed(6, 150);
        Dxl.jointMode(6);
        Dxl.goalSpeed(7, 300);
        Dxl.jointMode(7);
        Dxl.goalSpeed(9, 150);
        Dxl.jointMode(9);
        pos = 510;
        Dxl.goalPosition(5, pos);
        delay(500);
        pos_servo6 = 206;
        Dxl.goalPosition(6, pos_servo6);
        delay(500);
        pos_servo9 = 480;
        Dxl.goalPosition(7, pos_servo9);
        pos_servo8 = 230;
        Dxl.goalPosition(9, pos_servo8);
        delay(1000);
        Dxl.goalSpeed(5, 500);
        Dxl.jointMode(5);
        Dxl.goalSpeed(6, 500);
        Dxl.jointMode(6);
        Dxl.goalSpeed(7, 500);
        Dxl.jointMode(7);
        Dxl.goalSpeed(9, 500);
        Dxl.jointMode(9);
        break;

*/

      case 'r': //STOP
        Dxl.writeWord(1, GOAL_SPEED, 0); 
        Dxl.writeWord(2, GOAL_SPEED, 0);
        Dxl.writeWord(3, GOAL_SPEED, 0); 
        Dxl.writeWord(4, GOAL_SPEED, 0);
        Dxl.writeWord(5, GOAL_SPEED, 0);
        break;
      }
    }
    message='0';
    delay(10);
  }
  Dxl.goalPosition(5, pos_servo5);
  Dxl.goalPosition(6, pos_servo6);
  Dxl.goalPosition(7, pos_servo7);
  Dxl.goalPosition(9, pos_servo9);
  Dxl.goalPosition(10, pos_servo10);
  Dxl.goalPosition(8, pos_servo8);
  handler_co2();
}

void handler_co2() {
  if(sensorvalue > analogRead(0) + 5 || sensorvalue < analogRead(0) - 5){
    SerialUSB.println(analogRead(0));
    sensorvalue = analogRead(0);
  }
}

void lect(){
  while(SerialUSB.available())
  { //while there is data available on the serial monitor
    message= char(SerialUSB.read());//store string from serial commna
   // SerialUSB.println(message);
  }
  }
