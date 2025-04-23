void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

int irRead(){
  int largestValue = 0;
  int largestSensor = 0;
  int value;
  for (int i = 0; i < 30; i++){ //amount of trials. 
    for (int j = 0; j < 16; j++){ //amount of sensors

      value = pulseIn(j+22, LOW, 1500); 
      if (value > largestValue) {largestValue = value; largestSensor = j + 1;} 
    } 
    //Serial.println();
  }
  return largestSensor;
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 16; i++) {Serial.print(pulseIn(i+22, LOW, 1500)); Serial.print("    ");}
  Serial.println();
  //Serial.println(irRead());
  
}