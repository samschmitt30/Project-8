//Project 31 - Creating a Temperature History Monitor

//define pins that LEDs are connected to:
#define HOT 6
#define NORMAL 4
#define COLD 2

float voltage = 0;
float celsius = 0;
float hotTemp = 26;
float coldTemp = 20;
float sensor = 0;

#include <TFT.h>  // Arduino TFT LCD library
#include <SPI.h>  // SPI bus library

TFT TFTscreen = TFT(10, 9, 8); 
// allocate digital pins to LCD

int tcurrent = 0;
int tempArray[120];

char currentString[3];

void getTemp() // function to read temperature from TMP36
{
  float sum = 0;
  float voltage = 0;
  float sensor = 0;
  float celsius;

  // read the temperature sensor and convert the result to degrees C
  sensor   = analogRead(5);
  voltage  = (sensor * 5000) / 1024;
  voltage  = voltage - 500;
  celsius  = voltage / 10;
  tcurrent = int(celsius);

  // insert the new temperature at the start of the array of past temperatures
  for (int a = 119 ; a >= 0 ; --a )
  {
    tempArray[a] = tempArray[a - 1];
  }
  tempArray[0] = tcurrent;
}

void drawScreen() // generate TFT LCD display effects
{
  int q;
  // display current temperature
  TFTscreen.background(0, 0, 0);   // clear screen to black
  TFTscreen.stroke(255, 255, 255); // white text
  TFTscreen.setTextSize(2);
  TFTscreen.text("Current:", 20, 0);
  String tempString = String(tcurrent);
  tempString.toCharArray(currentString, 3);
  TFTscreen.text(currentString, 115, 0);
  // draw scale for graph
  TFTscreen.setTextSize(1);
  TFTscreen.text("50", 0, 20);
  TFTscreen.text("45", 0, 30);
  TFTscreen.text("40", 0, 40);
  TFTscreen.text("35", 0, 50);
  TFTscreen.text("30", 0, 60);
  TFTscreen.text("25", 0, 70);
  TFTscreen.text("20", 0, 80);
  TFTscreen.text("15", 0, 90);
  TFTscreen.text("10", 0, 100);
  TFTscreen.text(" 5", 0, 110);
  TFTscreen.text(" 0", 0, 120);
  TFTscreen.line(20, 20, 20, 127);
  // plot temperature data points
  for (int a = 25 ; a < 145 ; a++)
  {
    // convert the temperature value to a suitable y-axis position on the LCD
    q = (123 - (tempArray[a - 25] * 2));
    TFTscreen.point(a, q);
  }
}
void setup() 
{
pinMode(HOT, OUTPUT);
pinMode(NORMAL, OUTPUT);
pinMode(COLD, OUTPUT);
TFTscreen.begin();               // activate LCD
  TFTscreen.background(0, 0, 0); // set display to black
}


void loop() 
{
  //read the temperature sensor and convert the result to degrees Celsius
  sensor = analogRead(0);
  voltage = ( sensor * 5000) / 1024; //covert raw sensor vaule to millivolts
celsius = voltage / 10;  // convert millivolts to Celsius
   // act on temperature range
    if ( celsius < coldTemp ) 
   {
      digitalWrite(COLD, HIGH);
      delay(1000); 
      digitalWrite(COLD, LOW);
   }
    else if ( celsius > coldTemp && celsius <= hotTemp ) 
      {
         digitalWrite(NORMAL, HIGH);
         delay(1000);
         digitalWrite(NORMAL, LOW);
      }
      else
      {
         // celsius is > hotTemp
         digitalWrite(HOT, HIGH);
         delay(1000);
         digitalWrite(HOT, LOW);
      }

{
  getTemp();
  drawScreen();
  for (int a = 0 ; a < 20 ; a++) // wait 20 minutes until the next reading
  {
    delay(60000);                // wait 1 minute
  }
}
