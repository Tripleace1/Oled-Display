// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_GFX.h>      // include Adafruit graphics library
#include <Adafruit_ST7735.h>   // include Adafruit ST7735 TFT library


// Replace with your network credentials
const char* ssid = "projects";
const char* password = "projects";

#define DHTPIN 5     // Digital pin connected to the DHT sensor
// ST7735 TFT module connections
#define TFT_RST   2    // TFT RST pin is connected to NodeMCU pin D4 (GPIO2)
#define TFT_CS    0    // TFT CS  pin is connected to NodeMCU pin D3 (GPIO0)
#define TFT_DC    4     // TFT DC  pin is connected to NodeMCU pin D2 (GPIO4)
// initialize ST7735 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO132
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


float p = 3.1415926;


// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 10000;  


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
      font-family: Arial, sans-serif;
      display: inline-block;
      margin: 0px auto;
      text-align: center;
      background-color: #f2f2f2;
    }
    h2 {
      font-size: 3.5rem;
      color: #333;
      margin-top: 20px;
    }
    p {
      font-size: 2.5rem;
      color: #555;
    }
    .units {
      font-size: 1.2rem;
      color: #777;
    }
    .dht-labels {
      font-size: 1.8rem;
      vertical-align: middle;
      padding-bottom: 15px;
      color: #444;
    }
    #temperature, #humidity {
      font-size: 3.5rem;
      color: #007BFF;
    }
    i {
      font-size: 2.5rem;
    }
  </style>
</head>
<body>
  <h2>Group 6 Project</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#FF5733;"></i>
    <span class="dht-labels">Temperature</span>
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#33AFFF;"></i>
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>
</body>
<script>
  // ... (JavaScript code remains the same)
  setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";
// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  return String();
}

void setup(void){
  // Serial port for debugging purposes
  Serial.begin(115200);
  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
    tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
    tft.fillScreen(ST7735_BLACK);
  testdrawtext("MCE 314 PROJECTS                            MEASUREMENT AND     INSTRUMENTATION                                                                                                                                                                              LECTURER: DR TALHA", ST7735_WHITE);
  delay(1000);
  tft.invertDisplay(true);
  delay(500);
  tft.invertDisplay(false);
  delay(500);

  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;
  delay(500);
  }
 tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
tft.fillScreen(ST7735_BLACK);
testdrawtext("MCE 314 PROJECTS                            MEASUREMENT AND     INSTRUMENTATION                                                                                                                                                                              LECTURER: DR TALHA", ST7735_WHITE);
  delay(1000);
  tft.invertDisplay(true);
  delay(500);
  tft.invertDisplay(false);
  delay(500);
  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });

  // Start server
  server.begin();
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void loop(){  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    // Read temperature as Celsius (the default)
    float newT = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float newT = dht.readTemperature(true);
    // if temperature read failed, don't change t value
    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      t = newT;
      Serial.println(t);
      tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(t);
    }
    // Read Humidity
    float newH = dht.readHumidity();
    // if humidity read failed, don't change h value 
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      h = newH;
      Serial.println(h);
      tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(h);
    }
  }
}
