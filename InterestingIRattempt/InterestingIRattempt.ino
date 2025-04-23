void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
  int sensorValues[40]; //number = amount of trials * amount of sensors
  bool seesBall = false;
  for (int i = 0; i < 10; i++){ //amount of trials. 
    for (int j = 0; j < 4; j++){ //amount of sensors
      sensorValues[i*4 + j] = pulseIn(j+2, LOW, 1500);
      if (sensorValues[i*4 + j] > 0)
      {seesBall = true;}
      //Serial.print(sensorValues[j]);
      //Serial.print("     ");
    } 
    //Serial.println();
  }
  if (seesBall)
  {return findGreatest(sensorValues, sizeof(sensorValues)/sizeof(sensorValues[0]))%4+1;}
  return 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  //for (int i = 0; i < 4; i++) {Serial.print(pulseIn(i+2, LOW, 1500)); Serial.print("    ");}
  Serial.println(irRead());
  //Serial.println();
}
