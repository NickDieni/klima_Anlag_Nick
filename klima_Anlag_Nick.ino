// Include necessary libraries
#include <DHT11.h>        // Library for DHT11 temperature sensor
#include "ADebouncer.h"    // Library for debouncing button inputs
#include <EEPROM.h>        // Library for reading/writing to EEPROM memory

// Initialize the DHT11 sensor on pin 2
DHT11 dht11(2);

// Declare variables and constants
ADebouncer debouncer;     // Initialize debouncer for button input
bool led_mode = HIGH;     // Set LED mode (on/off state)
int BLUE = 13;            // Pin for blue LED
int GREEN = 12;           // Pin for green LED
int RED = 11;             // Pin for red LED
int YELLOW = 4;           // Pin for yellow LED
bool buttonState = LOW;   // State of the button (pressed or not)
int buttonPin = 3;        // Pin for button input
int toggle;               // Variable to track toggle state of the system

void setup() {
  // Initialize serial communication at 9600 baud rate
  Serial.begin(9600);

  // Set LED pins as output
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);

  // Set button pin as input
  pinMode(buttonPin, INPUT);

  // Configure debouncer for instant mode
  debouncer.mode(INSTANT, 0, HIGH);

  // Read the last saved toggle state from EEPROM (position 1)
  toggle = EEPROM.read(1);
}

void loop() {
  // Read temperature threshold value from analog pin A0
  int senVal = analogRead(A0);
  float tempDef = senVal / 10;   // Convert to an appropriate temperature scale
  int temperature = dht11.readTemperature();  // Get current temperature from DHT11 sensor

  // Debounce button input and update toggle state on rising edge
  debouncer.debounce(digitalRead(buttonPin));
  if (debouncer.rising()) toggle = !toggle;  // Toggle state on button press
  
  // Display temperatures to serial monitor
  Serial.print("Klima temp: ");
  Serial.print(tempDef);            // Print threshold temperature
  Serial.print(" Sensor Temp: ");
  Serial.println(temperature);      // Print sensor temperature
  
  // Check toggle state to decide LED behavior
  if(toggle == HIGH){
    // Temperature is lower than threshold, light up blue LED
    if(temperature < tempDef){
      digitalWrite(BLUE, HIGH);
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, LOW);
      digitalWrite(YELLOW, LOW);
    } 
    // Temperature is higher than threshold, light up red LED
    else if(temperature > tempDef){
      digitalWrite(BLUE, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
      digitalWrite(YELLOW, LOW);
    } 
    // Temperature equals threshold, light up green LED
    else {
      digitalWrite(BLUE, LOW);
      digitalWrite(GREEN, HIGH);
      digitalWrite(RED, LOW);
      digitalWrite(YELLOW, LOW);
    }
  } else if(toggle == LOW) {
    // If toggle is off, only light up yellow LED
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, HIGH);
  }
  
  // Save the toggle state to EEPROM to retain it after reset/power-off
  EEPROM.write(1, toggle);
}
