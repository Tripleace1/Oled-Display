#include <Adafruit_GFX.h>    // Include the Adafruit graphics library
#include <Adafruit_ST7735.h> // Include the Adafruit ST7735 library
int trigPin = 6;    // Trigger
int echoPin = 5;    // Echo
long duration;
float distance_cm, distance_inches;

// Pin definitions for the display
#define TFT_CS    10
#define TFT_RST   9
#define TFT_DC    8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// ... Your existing code ...

void setup() {
  // Initialize OLED display
  tft.initR(INITR_MINI160x80);
  tft.setRotation(1); // Set rotation if needed
  
  // Serial Port begin
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
   digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);

  // Measure the time it takes for the echo to return
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance in centimeters
  distance_cm = duration * 0.0343 / 2.0; // Speed of sound: 343 meters/second

  // Calculate distance in inches
  distance_inches = duration * 0.0135 / 2.0; // Speed of sound: 1350 feet/second

  // Print the distances
  Serial.print(distance_cm, 2); // Print with 2 decimal places
  Serial.print(" cm, ");
  Serial.print(distance_inches, 2); // Print with 2 decimal places
  Serial.println(" inches");

  delay(250);
  
  // Clear the display
  tft.fillScreen(ST7735_BLACK);

  // Print the distances on the display
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(10, 30);
  tft.print(distance_cm, 1); // Display with 1 decimal place
  tft.print(" cm");
  
  tft.setCursor(10, 70);
  tft.print(distance_inches, 1); // Display with 1 decimal place
  tft.print(" inches");

  delay(250);
}
