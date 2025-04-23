//SENSOR PINS ARE 8+ (for four sensors it would be 8-11)
void setup()
{
    Serial.begin(9600);
    Serial.println("ji");
}

int findLowest(int *arr, int size){ //returns -1 if the lowest number == 0, otherwise returns index of lowest number
  int smallestElement = 9999;
  int smallestIndex = -1;
  for (int i = 0; i < size; i++){
    if ((arr[i] != 0) && arr[i] < smallestElement){ smallestIndex = i; smallestElement = arr[i]; }
  }
  return(smallestIndex);
}

int findGreatest(int *arr, int size){
  int greatestValue = 0;
  int greatestIndex = 0;
  for (int i = 0; i < size; i++){
    if (arr[i] > greatestValue){ greatestIndex = i; greatestValue = arr[i]; }
  }
  return(greatestIndex);
}

int irRead(){
  int sensorValues[16]; //values of sensors
  int sensorAmounts[17] = \
  {0,\
  0,0,0,0,\
  0,0,0,0,\
  0,0,0,0,\
  0,0,0,0}; //amount of times that sensors saw the ball, greatest value after 10 tries is what we're going with
  int closestSensor; //closest sensor to ball when seeing which sensor is closest
  for (int i = 0; i < 10; i++){//do the following 10 times to eliminate outliers
    for (int j = 0; j < sizeof(sensorValues)/sizeof(sensorValues[0]); j++){//have each sensor scan
      sensorValues[15-j] = pulseIn(j+22, LOW, 1201);
    }
    closestSensor = findLowest(sensorValues, sizeof(sensorValues)/sizeof(sensorValues[0]));
    sensorAmounts[closestSensor+1]++;//need +1 since index 0 means no sensor saw ball, sensorAmounts is always 1 ahead of sensorValues. 
  } //If closestSensor = -1, then sensorAmounts[0] will go up so it works out
  return findGreatest(sensorAmounts, sizeof(sensorAmounts)/sizeof(sensorAmounts[0]));
}

void drive(float angle) {
  // if p1 is high it turn clockwise from inside perspective
  //the en stuff is for speed (max is 255)
  // Motor 1 - left on L298N - back left motor
  float originalAngle = angle;
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

  
  int x;
  int y;
  char xa; // these are either HIGH or LOW (if xa is HIGH xb is LOW)
  char xb;
  char ya;
  char yb;
  angle = radians(angle);//turns the direction from degrees to radians
  angle = angle + (PI/4);//since the wheals are of by 45 degrees that translates to .71 or just π/4
  x = cos(angle) * 255;//cos and sin always give a value under or eqaul to 1 so multiply it by 255 to reach motor (maybe the pin)max
  y = sin(angle) * 255;//for a better explanation look up "unit cirlcle "
  Serial.println(x);//for testing ig
  Serial.println(y);
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
  Serial.println(xa);//more testing ig
  Serial.println(xb);
  Serial.println(ya);
  Serial.println(yb);
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

int irLogic(){// old values
  int driveValues[16] = \
  {30, 50, 90, 180,\
    180, 180, 180, 180,\
    180, 180, 180, 180,\
    360-30, 360-50, 360-90, 360-180};
  return(driveValues[irRead()]);
}
//NOTE IF JAKEY IS READING THIS: sizeof(arr)/sizeof(arr[0]) just returns the length of arr. I can explain why in person/over text
void loop() {
  {drive(irLogic());}
  //else {dontDrive()}
}