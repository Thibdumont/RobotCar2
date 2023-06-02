#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println("Hello");
  delay(1000);
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}