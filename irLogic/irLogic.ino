//SENSOR PINS ARE 8+ (for four sensors it would be 8-11)
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
  
void setup()
{
    Serial.begin(9600);
    Serial.println("ji");
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

/*
int irRead(){
  int largestValue = 0;
  int largestSensor = -1;
  int value;
  for (int i = 0; i < 20; i++){ //amount of trials. 
    for (int j = 0; j < 16; j++){ //amount of sensors

      value = pulseIn(j+22, LOW, 1500); 
      if (value > largestValue) {largestValue = value; largestSensor = j;} 
    } 
    //Serial.println();
  }
  if (largestSensor == -1) return(-1);
  return (24 - (largestSensor))%16;
}*/

void drive(float angle) {
  // if p1 is high it turn clockwise from inside perspective
  //the en stuff is for speed (max is 255)
  // Motor 1 - left on L298N - back left motor
  float originalAngle = angle;
  
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
  if (originalAngle < 0)
  {xa = LOW; xb = LOW; ya = LOW; yb = LOW;}
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

int irLogic(){
  int driveValues[17] = {-1, 0, 40, 70, 100, 180, 180, 180, 180, 180, 180, 180, 180, 180, 360-100, 360-70, 360-40};//180, -90, -50, and -30 deg
  return(driveValues[irRead()+1]);
}
//NOTE IF JAKEY IS READING THIS: sizeof(arr)/sizeof(arr[0]) just returns the length of arr. I can explain why in person/over text
void loop() {
  //int direction = 360;
  int direction = irLogic();
  if (direction != -1) drive(360 - (direction));
  else {dontDrive();}


  /*int direction = irLogic();
  if (direction != -1) drive(direction);
  else {dontDrive();}
  
  
  int direction = 22.5 * irRead();
  if (direction != -1) {
    drive(direction);
  }
  //if (direction != -22.5) drive(360-direction);
  else {dontDrive();}
  //Serial.println((8+(irRead()))%16);
  */
  //Serial.println(direction);
  //TESTING RANDOM CHAOS
  /*for (int i = 0; i < 16; i += 4) {Serial.print(pulseIn(i+22, LOW, 1201)); Serial.print("     ");}
  Serial.println();*/
  /*int direction = irRead();
  if (direction == -1) dontDrive();
  else {drive(direction * 22.5);}*/
  //Serial.println(irRead());
}