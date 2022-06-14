#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "spotifyController.h"
#include <pthread.h>

#define baudRate 115200
void setup()
{
  Serial.println("Starting...");
  setupDisplay();
  //setupController();
  Serial.begin(baudRate); // start serial communication
  Serial.println("Setup complete.");
}

void loop()
{
  //Serial.println("periodPerStroke = " + String(periodPerStroke));
  sentResponse(checkController());
}