#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);


int m1p1 = 2;
int m1p2 = 3;
int enA = 13;

// Motor 2 - right on L298N - front right motor
int m2p1 = 4;
int m2p2 = 5;
int enB = 11;

// Motor 3 - left on L298N - back right motor
int m3p1 = 6;
int m3p2 = 7;
int enC = 12;

// Motor 4 - right on L298N - front left motor
int m4p1 = 8;
int m4p2 = 9;
int enD = 10;

int goalAngle;
int angleError = 13;
int rotationSpeed = 35;

void setup(){

  if (!mag.begin()) {
    Serial.println("Could not initialize LSM303DLHC magnetometer!");
    while (1);}
  Serial.begin(9600);
  Serial.println("hi");
  goalAngle = compass();
}

int findGreatest(int *arr, int size){//returns the index of the int with the highest value
  int greatestValue = 0;
  int greatestIndex = 0;
  bool notAllZeroes = false;
  for (int i = 0; i < size; i++){//if highest frequency so far, greatest index = the index of the sensor we are checking
    if (arr[i] > greatestValue){ greatestIndex = i; greatestValue = arr[i]; }
    notAllZeroes |= arr[i] != 0;
  }
  if (notAllZeroes)
  return(greatestIndex);
  return -1;
}

int irRead(){
  int sensorValues[16]; //values of sensors
  int sensorAmounts[17] = {0}; //amount of times that sensors saw the ball, greatest value after 10 tries is what we're going with
  int closestSensor; //index of closest sensor to ball on each of the ten checks
  for (int i = 0; i < 15; i++){// (get values, find greatest, add that to sensorAmounts)*10
    for (int j = 0; j < sizeof(sensorValues)/sizeof(sensorValues[0]); j++){
      sensorValues[j] = pulseIn(22 + ((j+8)%16), LOW, 1201); //reads the sensors - the HIGHER the value, the closer it is //IR ring is backwards (facepalm)
    }
    closestSensor = findGreatest(sensorValues, sizeof(sensorValues)/sizeof(sensorValues[0]));// this gets reset every time and is differnet than the result of findGreatest
//    Serial.println(closestSensor);
    sensorAmounts[closestSensor+1]++;//need +1 since index 0 means no sensor saw ball, sensorAmounts is always 1 ahead of sensorValues. 
  } //If closestSensor = -1, then sensorAmounts[0] (which is the number of times that we saw nothing) will go up so it works out
  return findGreatest(sensorAmounts, sizeof(sensorAmounts)/sizeof(sensorAmounts[0]))-1;// returns the index of the sensor that saw the ball the closest the most

}


int compass(){
  //sensors_event_t accelEvent;
  sensors_event_t magEvent;
  mag.getEvent(&magEvent);    // Get magnetometer data

  // Offset magnetometer readings to center them
  float magX = (magEvent.magnetic.x * (40.0/45.0)) + 10; // Adjust offset as needed
  float magY = (magEvent.magnetic.y * (40.0/45.0)) - 6;

  // Heading calculation
  int heading = atan2(magY, magX) * 180 / PI;

  heading = heading%360;
  if (heading < 0) heading += 360;
  //Serial.println(heading);
  return 360 - heading;
}

void driveDirection(float angle) {
  //the en stuff is for speed (max is 255)
  // Motor 1 - left on L298N - back left motor
  
  int x;
  int y;
  char xa; // these are either HIGH or LOW (if xa is HIGH xb is LOW)
  char xb;
  char ya;
  char yb;
  angle = radians(180+angle);//turns the direction from degrees to radians
  angle = angle + (PI/4);//since the wheals are of by 45 degrees that translates to .71 or just π/4
  x = cos(angle) * 255;//cos and sin always give a value under or eqaul to 1 so multiply it by 255 to reach motor (maybe the pin)max
  y = sin(angle) * 255;//for a better explanation look up "unit cirlcle "
  if (x >= 0){//if the direction is negitive you have to tell the motor controller to spin the motors the other way
    xa = HIGH;
    xb = LOW;
  }else{
    xa = LOW;
    xb = HIGH;
  };
  if (y >= 0){
    ya = HIGH;
    yb = LOW;
  }else{
    ya = LOW;
    yb = HIGH;
  }
  analogWrite(enA, abs(y));
  digitalWrite(m1p1, ya);
  digitalWrite(m1p2, yb);
  analogWrite(enB, abs(y));
  digitalWrite(m2p1, yb);
  digitalWrite(m2p2, ya);
  analogWrite(enC, abs(x));
  digitalWrite(m3p1, xb);
  digitalWrite(m3p2, xa);
  analogWrite(enD, abs(x));
  digitalWrite(m4p1, xa);
  digitalWrite(m4p2, xb);
}

void driveRotation(int rotation) {
  char xa; // these are either HIGH or LOW (if xa is HIGH xb is LOW)
  char xb;
  char ya;
  char yb;
  if (rotation >= 0){//if the direction is negitive you have to tell the motor controller to spin the motors the other way
    xa = HIGH;
    xb = LOW;
  }else{
    xa = LOW;
    xb = HIGH;
  }
  rotation = abs(rotation);

  analogWrite(enA, rotation);
  digitalWrite(m1p1, xb);
  digitalWrite(m1p2, xa);
  analogWrite(enB, rotation);
  digitalWrite(m2p1, xb);
  digitalWrite(m2p2, xa);
  analogWrite(enC, rotation);
  digitalWrite(m3p1, xb);
  digitalWrite(m3p2, xa);
  analogWrite(enD, rotation);
  digitalWrite(m4p1, xb);
  digitalWrite(m4p2, xa);
}

void dontDrive(){
  
  analogWrite(enA, 0);
  digitalWrite(m1p1, LOW);
  digitalWrite(m1p2, LOW);
  analogWrite(enB, 0);
  digitalWrite(m2p1, LOW);
  digitalWrite(m2p2, LOW);
  analogWrite(enC, 0);
  digitalWrite(m3p1, LOW);
  digitalWrite(m3p2, LOW);
  analogWrite(enD, 0);
  digitalWrite(m4p1, LOW);
  digitalWrite(m4p2, LOW);
}

int readlightSensor(){
  int j;
  int thresholdArray[4] = {20, 20, 40, 36};
  //Serial.println(analogRead(1));
  for (int i = 0; i < 4; i++){
    j = analogRead(i);
    if (j > thresholdArray[i]) {
      return i;
    } 
  }
  return 4;
}

int irLogic(){
  int driveValues[17] = {-1, 0, 40, 70, 100, 180, 180, 180, 180, 180, 90, 180, 180, 180, 360-100, 360-70, 360-40};//180, -90, -50, and -30 deg
  return(driveValues[irRead()+1]);
}

void loop() {
  int light = readlightSensor();
  //int light = 4;
  //Serial.println(light);
  if (light != 4){
    driveDirection((90 * light) + 180);
    delay(20);
    }
  else{
    int facing = (compass() - goalAngle)%360;
    if (facing < 0) facing += 360;
    //Serial.println(facing);
    if ((facing) < (360 - angleError) && (facing >= 180)){
      driveRotation(rotationSpeed);
      //Serial.print("LEFT  ");
      }
    else if ((facing > angleError) && (facing < 180)){
      driveRotation(-rotationSpeed);
      //Serial.print("RIGHT  ");
      }
    else {
      //int direction = irRead();
      int direction = irLogic();
      //Serial.println(-direction);
      if (direction != -1)
        driveDirection(-direction);
      else {
        dontDrive();
        }
      }
    }
  }