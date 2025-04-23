//SENSOR PINS ARE 8+ (for four sensors it would be 8-11)
void setup()
{
    Serial.begin(9600);
}

int irRead(){
  int largestValue = 0;
  int largestSensor = 0;
  int value;
  for (int i = 0; i < 10; i++){ //amount of trials. 
    for (int j = 0; j < 16; j++){ //amount of sensors
      value = pulseIn(j+2, LOW, 1500); 
      if (value > largestValue) {largestValue = value; largestSensor = j + 1;} 
    } 
    //Serial.println();
  }
  return largestSensor;
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

int irLogic(){
  int driveValues[16] = \
  {30, 50, 90, 180,\
    180, 180, 180, 180,\
    180, 180, 180, 180,\
    360-30, 360-50, 360-90, 360-180};
  return(driveValues[irRead()]);
}
//NOTE IF JAKEY IS READING THIS: sizeof(arr)/sizeof(arr[0]) just returns the length of arr. I can explain why in person/over text
void loop() {
  //drive(irLogic());
  drive((irRead()-1)*22.5)
}