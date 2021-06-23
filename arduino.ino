
#include <Wire.h>
#include <stdio.h>
#include <MPU6050_light.h>
#include <inttypes.h>
#define TileDistance 30

const unsigned char m1p1 = 11, m1p2=10, m2p1=9,m2p2=8;//pin motor
const unsigned char trigger[] = {7,4,12};//pinnen trigger distance sensor
const unsigned char echo[]    = {2,3,13};//pinnnen echo distance sensor
unsigned char EN1=5;  //Define a 5 Pin PWM
unsigned char EN2=6;  //Define a 6 Pin PWM
unsigned char donePin = 17;

//GRYO
MPU6050 mpu(Wire);


int dataToSend;//variable to store data for i2c 
uint8_t motorSpeed = 140;
unsigned char c = 8;
unsigned char d = c;
bool driveDone = false;
bool distanceDone = false;
bool angleDone = false;


void receiveEvent(int);
void ReadDistanceSensor(unsigned char);
int ReadDistanceSensorArduino(unsigned char c);
void Drive(unsigned char);
void TurnRight90();
void TurnLeft90();
void TurnLeft5Degree();
void TurnRight5Degree();

void Forward();
void Back();
void Stop();
void sendData();
void SetupGyro();
float GetGyro();
void Angle();
void SetupDistanceSensor();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  // Join I2C bus as slave with address 8
  Wire.begin(0x08);
  Serial.begin(9600);
  //inttrupt pin
  pinMode(donePin,OUTPUT);
  //gyro
  SetupGyro();
  SetupDistanceSensor(); 
   // Call receiveEvent when data received                
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendData); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  
  if(!distanceDone && c>=0 && c<=2){
    ReadDistanceSensor(c); 
    distanceDone = true;
  }
  if(!driveDone && (c >= 3 && c <= 10)){
        Drive(c);
        driveDone = true;//moeten doen zodat je twee keer de zelde commando achter elkaar kan zetten!!!
        //d=c;      
  }
  if(!angleDone && c ==11){
      Angle();
      angleDone = true;
  }
  /*
  float start = GetGyro();
  delay(1000);
  Serial.println(start);
  */
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Drive(unsigned char c){
  dataToSend = 0;
  if(c==3){//90 left
    Serial.println("90 left");
    TurnLeft90();
  }else if(c==4){//90 right
    Serial.println("90 right");
    TurnRight90();
  }else if(c == 5){//left
    Serial.println("left ");
    TurnLeft5Degree();
  }else if(c == 6){//right
    Serial.println("right ");
    TurnRight5Degree();
  }else if(c == 7){//forward
    Serial.println("forward ");
    Forward();
  }else if(c == 8){//stop
    Serial.println("stop ");
    Stop();
  }else if(c == 9){//back
    Serial.println("back ");
    Back();
  }else if(c == 10){//360 right
    Serial.println("360 right");
    for(uint8_t i=0;i<4;i++){
      TurnRight90();
    }
  }else{
    Stop();
  }
  Stop();
  digitalWrite(donePin, HIGH);
  delayMicroseconds(100);
  digitalWrite(donePin, LOW);
 
}

void sendData(){
  Wire.write(dataToSend);
    //Serial.print("send:");
    //Serial.println(dataToSend);
  /*if(driveDone == true){
    Wire.write(dataToSend);
    Serial.print("send:");
    Serial.println(dataToSend);
    //dataToSend = 0;//stay in while loop pi code
  }
  }else{
    Wire.write(dataToSend);
  }*/
}
void ReadDistanceSensor(unsigned char c){
 int duration;
 uint8_t distance = 0;
 
  do{ 
   digitalWrite(trigger[c], LOW); 
   delayMicroseconds(2); 
   digitalWrite(trigger[c], HIGH); 
   delayMicroseconds(10); 
   digitalWrite(trigger[c], LOW); 
   duration = pulseIn(echo[c], HIGH);
   distance = (duration*.0343)/2;//distance
  }while(distance >255 || distance < 0);
  dataToSend = distance;
  Serial.print("Distance: ");
  Serial.println(distance);
}
int ReadDistanceSensorArduino(int c){
  int duration;
  int distance;
  
  do{ 
   digitalWrite(trigger[c], LOW); 
   delayMicroseconds(2); 
   digitalWrite(trigger[c], HIGH); 
   delayMicroseconds(10); 
   digitalWrite(trigger[c], LOW); 
   duration = pulseIn(echo[c], HIGH);
   distance = (duration*.0343)/2;//distance
  }while(distance >255);
  return distance;
}
void SetupGyro(){
   byte status = mpu.begin();
  
   while (status != 0) { } // stop everything if could not connect to MPU6050
   Serial.println(F("Calculating offsets, do not move MPU6050"));
   delay(1000);
   mpu.calcOffsets(); // gyro and accelero
   Serial.println("Done!\n");
}
float GetGyro(){
   mpu.update();
   return mpu.getAngleZ();
}
void Angle(){
  float data = GetGyro();
  Serial.print("get gyro is:");
  Serial.println(data);
  dataToSend = data;
  
}
void SetupDistanceSensor(){
    //distance sensor
  for(uint8_t i=0;i<3;i++){
    pinMode(trigger[i], OUTPUT);
    pinMode(echo[i], INPUT);
  }
}
void TurnLeft90(){  
   // motorSpeed=100;
   float startAngle = GetGyro(); 
   float stopAngle = startAngle + 85;
   float currentAngle = GetGyro();
   //float previousAngle = currentAngle;
   Serial.print("start angle");
   Serial.println(startAngle);
   while(stopAngle >=  currentAngle){
    float previousAngle = currentAngle;
    /*if(previousAngle == currentAngle && motorSpeed < 100){
      motorSpeed += 20;
      Serial.println(motorSpeed);
      if((stopAngle - 20) == currentAngle){
        motorSpeed /2;
        Serial.println(motorSpeed);
      }
    }*/
      currentAngle = GetGyro();
      analogWrite(EN1, motorSpeed);      //sets the motors speed
      analogWrite(EN2, motorSpeed);      //sets the motors speed
      digitalWrite(m1p1, LOW);
      digitalWrite(m1p2, HIGH);
      digitalWrite(m2p1, HIGH);
      digitalWrite(m2p2, LOW); 
      //Stop();
      //Serial.print("current angle: ");
      //Serial.println(currentAngle);
   };
   Stop();
   Serial.print("stopAngle: ");
   Serial.println(stopAngle);
   //Serial.print("stopAngle-startAngle: ");
   Serial.println("out loop");
   Serial.println(currentAngle);
}
void TurnRight90(){
    //motorSpeed = 100;
    float startAngle = GetGyro(); 
    float stopAngle = startAngle - 85;
    float currentAngle = GetGyro();
    Serial.print("start angle");
    Serial.println(startAngle);
    while(stopAngle <  currentAngle){
    /*  float previousAngle = currentAngle;
      if(previousAngle == currentAngle && motorSpeed < 100){
        motorSpeed += 20;
        Serial.println(motorSpeed);
        if((stopAngle + 20) == currentAngle){
        motorSpeed /2;
        Serial.println(motorSpeed);
        }
      }*/
    
      currentAngle = GetGyro();
      analogWrite(EN1, motorSpeed);      //sets the motors speed
      analogWrite(EN2, motorSpeed);      //sets the motors speed
      digitalWrite(m1p1, HIGH);
      digitalWrite(m1p2, LOW);
      digitalWrite(m2p1, LOW);
      digitalWrite(m2p2, HIGH);
      Serial.print("current angle: ");
      Serial.println(currentAngle);
      //Stop();  
   };
   Stop();
   Serial.print("stopAngle: ");
   Serial.println(stopAngle);
   //Serial.print("stopAngle-startAngle: ");
   Serial.println("out loop");
   Serial.println(currentAngle);
}
void TurnRight5Degree(){
    float startAngle = GetGyro(); 
    float stopAngle = startAngle - 5;
    float currentAngle = GetGyro();
    Serial.print("start angle");
    Serial.println(startAngle);
    while(stopAngle <=  currentAngle){
      currentAngle = GetGyro();
      analogWrite(EN1, motorSpeed);      //sets the motors speed
      analogWrite(EN2, motorSpeed);      //sets the motors speed
      digitalWrite(m1p1, HIGH);
      digitalWrite(m1p2, LOW);
      digitalWrite(m2p1, LOW);
      digitalWrite(m2p2, HIGH);
    }
   Stop();
   Serial.print("stopAngle: ");
   Serial.println(stopAngle);
   //Serial.print("stopAngle-startAngle: ");
   Serial.println("out loop");
   Serial.println(currentAngle);
}
void TurnLeft5Degree(){
    //motorSpeed = 100;
    float startAngle = GetGyro(); 
    float stopAngle = startAngle + 5;
    float currentAngle = GetGyro();
    Serial.print("start angle");
    Serial.println(startAngle);
    while(stopAngle >=  currentAngle){
      currentAngle = GetGyro();
      Serial.println(currentAngle);
      analogWrite(EN1, motorSpeed);      //sets the motors speed
      analogWrite(EN2, motorSpeed);      //sets the motors speed
      digitalWrite(m1p1, LOW);
      digitalWrite(m1p2, HIGH);
      digitalWrite(m2p1, HIGH);
      digitalWrite(m2p2, LOW);  
    }
     Stop();
     Serial.print("stopAngle: ");
     Serial.println(stopAngle);
     //Serial.print("stopAngle-startAngle: ");
     Serial.println("out loop");
     Serial.println(currentAngle);

}/*
void TurnRight(){
    //motorSpeed = 100;
    analogWrite(EN1, motorSpeed);      //sets the motors speed
    analogWrite(EN2, motorSpeed);      //sets the motors speed
    digitalWrite(m1p1, HIGH);
    digitalWrite(m1p2, LOW);
    digitalWrite(m2p1, LOW);
    digitalWrite(m2p2, HIGH);
    delay(200);
}*/
void Forward(){
    //motorSpeed = 100;
    uint8_t startDistance = ReadDistanceSensorArduino(1);
    uint8_t stopDistance=0;   
    Serial.print("startDistance:");
    Serial.println(startDistance); 
    do{
      analogWrite(EN1, motorSpeed);      //sets the motors speed
      analogWrite(EN2, motorSpeed);      //sets the motors speed
      digitalWrite(m1p1, LOW);
      digitalWrite(m1p2, HIGH);
      digitalWrite(m2p1, LOW);
      digitalWrite(m2p2, HIGH);
      stopDistance = ReadDistanceSensorArduino(1);  
      Serial.print("stopDistance: ");       
      Serial.println(stopDistance);         
    }while(stopDistance > startDistance-TileDistance && stopDistance > 5 );
    
   
}
void Back(){
    //motorSpeed = 100;
    analogWrite(EN1, motorSpeed);      //sets the motors speed
    analogWrite(EN2, motorSpeed);      //sets the motors speed
    digitalWrite(m1p1, HIGH);
    digitalWrite(m1p2, LOW);
    digitalWrite(m2p1, HIGH);
    digitalWrite(m2p2, LOW);
}
void Stop(){
    digitalWrite(m1p1, LOW);
    digitalWrite(m1p2, LOW);
    digitalWrite(m2p1, LOW);
    digitalWrite(m2p2, LOW);
}
// Function that executes whenever data is received from master
void receiveEvent(int howMany) {
  //value to store incoming data; 
 
  for (int i = 0; i < howMany; i++) {
    c = Wire.read();
    Serial.print("incoming data: ");
    Serial.println(c);
    if(c>=0 && c<= 2)distanceDone = false;
    if(c>=3 && c<= 10)driveDone = false;   
    if(c==11)         angleDone = false;
   } 
}
