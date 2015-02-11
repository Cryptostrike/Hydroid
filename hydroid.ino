/* 
Plant monitoring and watering system - Cryptostrike (MIT License)
https://github.com/Cryptostrike/Hydroid/

Pinouts:
reservoirPin - Water level sensor to see if reservoir is empty
moisturePin - Soil moisture sensor
pumpPin - Pumps water from reservoir

Variables:
soilDry - This keeps track of whether the soil is below the desired moisture, 0 = it isn't dry, 1 = it's dry and requires water
reservoirEmpty - This checks whether ther is enough water in the reservoir to pump, 0 = there is enough, 1 = it's empty

Functions:
moistureSensor(desired); - Measures soil moisture, evaluates against desired moisture and outputs soilDry to say if the plant requires water 
reservoirCheck(); - Checks if the reservoir is filled enough to pump, if there is not enough water it outputs reservoirEmpty = 1 which stops the pump
*/

// Pins
int moisturePin = 0;
int pumpPin = 2;
int redLED = 3;
int greenLED = 4;
int reservoirPin = 5;

// Variables
int soilDry;
int reservoirEmpty;


void setup(){
  pinMode(reservoirPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  Serial.begin(9600);  
}


int moistureSensor(int desired){
  int moisture = analogRead(moisturePin);
  
  if (moisture < desired){
    soilDry = 0;
  } 
  else {
    soilDry = 1;
  }
  return soilDry;
}


int reservoirCheck(){
  int reservoir = digitalRead(reservoirPin);
  
  if (reservoir == LOW){
    reservoirEmpty = 1;
  } 
  else {
    reservoirEmpty = 0;
  }
  return reservoirEmpty;
}


void loop(){
  /* debugging
  ------------------------
  Serial.print("Raw: ");
  Serial.print(analogRead(moisturePin));
  Serial.print("  ");
  Serial.print("Soil dry? ");
  moistureSensor(700);
  Serial.println(soilDry);
  Serial.print("Reservoir empty? ");
  reservoirCheck();
  Serial.println(reservoirEmpty);
  Serial.print("  ");
  ---------------------------
  */
  
  reservoirCheck();
  moistureSensor(1200);
  
  if ((soilDry == 1) && (reservoirEmpty == 0)){
   Serial.println("* Soil is dry, activating pumping");
   digitalWrite(pumpPin, HIGH);
   digitalWrite(redLED, HIGH);
   //delay(15000);
   delay(5000);
   digitalWrite(pumpPin, LOW);
   digitalWrite(redLED, LOW);
   Serial.println("15 second pump cycle complete, rechecking soil moisture");
  }
  
  
  if ((soilDry == 0) && (reservoirEmpty == 0)){
    Serial.println("+ Soil moisture: Ok | Reservoir level: Ok");
    digitalWrite(greenLED, HIGH);
    //delay(15000);
    delay(5000);
    digitalWrite(greenLED, LOW);
  }
  
  
  if ((soilDry == 1) && (reservoirEmpty == 1)){
    Serial.println("! WARNING: Soil is dry and reservoir is EMPTY");
    digitalWrite(greenLED, HIGH);
    delay(500);
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    delay(500);
    digitalWrite(redLED, LOW);
  }
  
  
   if ((soilDry == 0) && (reservoirEmpty == 1)){
    Serial.println("! WARNING: Reservoir is EMPTY");
    digitalWrite(greenLED, HIGH);
    delay(500);
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    delay(500);
    digitalWrite(redLED, LOW);
  }
  
  delay(5000);
}
