//SENSOR PINS ARE 8+ (for four sensors it would be 8-11)
void setup()
{
    Serial.begin(9600);
}

int findLowest(int *arr, int size){ //returns -1 if the lowest number == 0, otherwise returns index of lowest number
  int smallestElement = 9999;// keeps track of the smallest ir values, because the smaller = the closer. starts a arbitrarily high value
  int smallestIndex = -1;// index in array of the the sensor with smallest value. defalt is -1 because if none of them see the ball then later code will know because this will = -1
  for (int i = 0; i < size; i++){// i = index. it goes though each ir value and checks if its the smallest but not zero(because that would mean it sees nothing)
    if ((arr[i] != 0) && arr[i] < smallestElement){ smallestIndex = i; smallestElement = arr[i]; }
  }
  return(smallestIndex);
}

int findGreatest(int *arr, int size){//finds the index of the sensor that gives the lowest vlaue the most
  int greatestValue = 0;//frequency that the sensor showed the lowest value
  int greatestIndex = 0;//index of that sensor
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
  for (int i = 0; i < 11; i++){// (get values, find greatest, add that to sensorAmounts)*10
    for (int j = 0; j < sizeof(sensorValues)/sizeof(sensorValues[0]); j++){
      sensorValues[j] = pulseIn(j+22, LOW, 1201); //reads the sensors - the lower the value, the closer it is
    }
    closestSensor = findGreatest(sensorValues, sizeof(sensorValues)/sizeof(sensorValues[0]));// this gets reset every time and is differnet than the result of findGreatest
//    Serial.println(closestSensor);
    sensorAmounts[closestSensor+1]++;//need +1 since index 0 means no sensor saw ball, sensorAmounts is always 1 ahead of sensorValues. 
  } //If closestSensor = -1, then sensorAmounts[0] (which is the number of times that we saw nothing) will go up so it works out
  return findGreatest(sensorAmounts, sizeof(sensorAmounts)/sizeof(sensorAmounts[0]));// returns the index of the sensor that saw the ball the closest the most
}

//NOTE IF JAKEY IS READING THIS: sizeof(arr)/sizeof(arr[0]) just returns the length of arr. I can explain why in person/over text
void loop() {
  Serial.println(irRead());
  //irRead();
}