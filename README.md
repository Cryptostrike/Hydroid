# Hydroid
Plant monitoring and watering system for Arduino. This project was developed for use with an Arduino Uno. 
I opted for using an ATmega328P-PU on veroboard to minimise the footprint of the board but this should work fine with any full Arduino board.


# Pinouts:

reservoirPin - Water level sensor to see if reservoir is empty

moisturePin - Soil moisture sensor

pumpPin - Pumps water from reservoir to soil


# Variables:

soilDry - This keeps track of whether the soil is below the desired moisture, 0 = it isn't dry, 1 = it's dry and requires water

reservoirEmpty - This checks whether ther is enough water in the reservoir to pump, 0 = there is enough, 1 = it's empty


# Functions:

moistureSensor(desired); - Measures soil moisture, evaluates against desired moisture and outputs soilDry to say if the plant requires water 

reservoirCheck(); - Checks if the reservoir is filled enough to pump, if there is not enough water it outputs reservoirEmpty = 1 which stops the pump
