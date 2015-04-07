// Hydroid - Plant monitoring and watering system - Cryptostrike (MIT License)
// https://github.com/Cryptostrike/Hydroid/


/********************************LED STATUS CODES ***************************************
 * There are 4 possible modes that hydroid can be in and each has their own led sequence*
 *                                                                                      *
 * 1. Soil dry, reservoir level ok: pumping started - Red led for 5 seconds then green  *
 * led for pumping duration.                                                            *
 * 2. Soil moist, reservoir level ok: continue monitoring -  One second green led flash *
 * 3. Soil dry, reservoir empty: max error - Solid 10 second red led                    *
 * 4. Soil moist, revervoir empty - error - One second red led flash                    *
 ***************************************************************************************/

                                                                                        
/******************** USER DEFINED PARAMETERS (CHANGE THESE) ****************************
* dryThreshold - An integer between 0 and 1023, 0 being 100% wet and 1023 being 0% dry. *
* pumpingDuration - Time that the pump will pump each cycle in ms. 1000 ms = 1 s.       *
* monitoringPeriod - Time that hydroid will pause before rechecking the moisture again. *
*****************************************************************************************/

int dryThreshold = 600;
int pumpingDuration = 5000;
int monitoringPeriod = 5000;


/* Pinouts:
 * reservoirPin - Water level sensor to check if reservoir is empty
 * moisturePin - Soil moisture sensor
 * pumpPin - Pumps water from reservoir to soil */
 
int moisturePin = 0; // Analog pin 0
int pumpPin = 2;
int redLED = 3;
int greenLED = 4;
int reservoirPin = 5;


/* Variables:
 * soilDry - This keeps track of whether the soil is below the desired moisture
 * reservoirEmpty - This checks whether ther is enough water in the reservoir to pump*/

int soilDry;
int reservoirEmpty;


// Setup pin modes
void setup(){
  pinMode(reservoirPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  Serial.begin(9600);  
}


/* Functions:
 * moistureSensor(dryThreshold); - Measures soil moisture, evaluates against user defined dryness threshold 
 * reservoirCheck(); - Checks if the reservoir is filled enough to pump, if there is not enough water pumping is stopped */

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
  
  /* Debugging loop
  Outputs serial output like:
  
  Raw: 1023  Soil dry? 0
  Reservoir empty? 1
  
  Where 'raw' is the raw soil moisture.
  ------------------------*/
  
  Serial.print("Raw: ");
  Serial.print(analogRead(moisturePin));
  Serial.print("  ");
  Serial.print("Soil dry? ");
  moistureSensor(dryThreshold);
  Serial.println(soilDry);
  Serial.print("Reservoir empty? ");
  reservoirCheck();
  Serial.println(reservoirEmpty);
  Serial.print("  ");
  
  //---------------------------
  
  
  // Read and evaluate sensors
  reservoirCheck(); // Checks if the reservoir is empty
  moistureSensor(dryThreshold); // Measures the soil moisture: moistureSensor(value from 0 - 1023 which is classed as dry);
  
  
  // Soil is dry so a pump cycle activates, there are no errors to report. 
  //LED: Red led for 5 seconds then green led for pumping duration
  if ((soilDry == 1) && (reservoirEmpty == 0)){
   Serial.println("* Soil is dry, activating pumping");
   
   digitalWrite(redLED, HIGH);
   delay(5000);
   digitalWrite(redLED, LOW);
   delay(10);
   
   digitalWrite(pumpPin, HIGH);
   digitalWrite(greenLED, HIGH);
   delay(pumpingDuration);
   
   digitalWrite(pumpPin, LOW);
   digitalWrite(greenLED, LOW);
   Serial.print(pumpingDuration);
   Serial.println(" second pump cycle complete, rechecking soil moisture");
   Serial.println("  ");
   Serial.println("---------------");
   Serial.println("  ");
  }
  
  
  // Soil is not dry so nothing happens, there are no errors to report 
  // LED: One second green led flash
  if ((soilDry == 0) && (reservoirEmpty == 0)){
    Serial.println("+ Soil moisture: Ok | Reservoir level: Ok");
    Serial.println("  ");
    Serial.println("---------------");
    Serial.println("  ");
    digitalWrite(greenLED, HIGH);
    delay(1000);
    digitalWrite(greenLED, LOW);
  }
  
  
  // The soil is dry but the reservoir is empty so pumping cannot occur, error is reported
  // LED: Solid 10 second red led
  if ((soilDry == 1) && (reservoirEmpty == 1)){
    Serial.println("! WARNING: Soil is dry and reservoir is EMPTY");
    Serial.println("  ");
    Serial.println("---------------");
    Serial.println("  ");
    
    // LED sequence for error report
    digitalWrite(redLED, HIGH);
    delay(10000);
    digitalWrite(redLED, LOW);
  }
  
  
   // The soil is not dru but the reservoir is empty so pumping cannot occur once the soil is dry, error is reported
   // LED: One second red led flash  
   if ((soilDry == 0) && (reservoirEmpty == 1)){
    Serial.println("! WARNING: Reservoir is EMPTY");
    Serial.println("  ");
    Serial.println("---------------");
    Serial.println("  ");
        
    // LED sequence for error report
    digitalWrite(redLED, HIGH);
    delay(1000);
    digitalWrite(redLED, LOW);
  }
  
  delay(monitoringPeriod); // Pause before rechecking the moisture again
}
