/*
  Creep test mechanical switch
*/

int sensorPin = A0;   // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
int pushButtonPin = 2; //digital pin 2 has a push button connected
bool flagNewFile = 0; // this flags whether a new file needs to be created

//LED pin
const int greenLEDPin = 6; 
const int redLEDPin = 7;

#include <SPI.h>
#include <SD.h>
const int chipSelect = 4;
unsigned long time;

// variable to store next available file name
int file = 0;
char fileString[12];

void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);

  //Blink both red and green LEDs
  blinkThreeDualLED(greenLEDPin, redLEDPin);
  
  //configure pin 2 as an input and enable the internal pull-up resistor and interrupt
  pinMode(pushButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pushButtonPin), button_ISR, FALLING);

  // see if the card is present and can be initialized:
  while (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    blinkThreeSingleLED(redLEDPin);
  }

  // cREATE NEW FILE
  createNewFile();
  blinkThreeDualLED(greenLEDPin, redLEDPin);
  digitalWrite(greenLEDPin, HIGH); 
}

void loop() {
  //First check if push button was pressed, so a new file needs to be created
  if (flagNewFile == 1) {
    digitalWrite(greenLEDPin, LOW);
    createNewFile();
    blinkThreeDualLED(greenLEDPin, redLEDPin);
    flagNewFile = 0;
    digitalWrite(greenLEDPin, HIGH); 
  //If not, perform the usual loop
  } else {
    // make a string for assembling the data to log:
    String dataString = "";
    // read the value from the sensor:
    sensorValue = analogRead(sensorPin);
    time = micros();
    // construct the String variable to store in SD card
    dataString += String(sensorValue);
    dataString += ",";
    dataString += String(time);

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open(fileString, FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      blinkThreeSingleLED(redLEDPin);
      Serial.println("error opening datalog.txt");
    }
  }
}

void createNewFile(){
  // Obtain the next available file name
  sprintf(fileString, "%02d.txt", file);
  do {
    file = file + 1;
    sprintf(fileString, "%02d.txt", file);
  } while (SD.exists(fileString));
}

void button_ISR()
{
  flagNewFile = 1;
}

void blinkThreeDualLED(int LED1, int LED2) {
  digitalWrite(LED1, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED2, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED1, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(LED2, LOW);   // turn the LED off by making the voltage LOW
  delay(500);                      // wait for a second
  digitalWrite(LED1, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED2, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED1, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(LED2, LOW);   // turn the LED off by making the voltage LOW
  delay(500);  
  digitalWrite(LED1, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED2, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED1, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(LED2, LOW);   // turn the LED off by making the voltage LOW
  delay(500);  
}

void blinkThreeSingleLED(int LED) {
  digitalWrite(LED, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED, LOW);   // turn the LED off by making the voltage LOW
  delay(500);                      // wait for a second
  digitalWrite(LED, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED, LOW);   // turn the LED off by making the voltage LOW
  delay(500);  
  digitalWrite(LED, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED, LOW);   // turn the LED off by making the voltage LOW
  delay(500);  
}