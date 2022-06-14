#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define playButton 17
#define pauseButton 26
#define nextButton 36
#define previousButton 39

String newString = "empty";

enum Button
{
    PLAY,
    PAUSE,
    NEXT,
    PREVIOUS,
    NONE
};

void setupController(){
    pinMode(playButton, INPUT_PULLUP);
    pinMode(pauseButton, INPUT_PULLUP);
    pinMode(nextButton, INPUT_PULLUP);
    pinMode(previousButton, INPUT_PULLUP);
}

int checkController(){
    int result = NONE;
    if(digitalRead(playButton) == LOW){
        result = PLAY;
    }
    return result;
}

int previousResult = NONE;

void sentResponse(int response){
    delay(100);
    if(response==previousResult){
        return;
    }
    switch(response){
        case PLAY:
            Serial.println("1");
            break;
        case PAUSE:
            Serial.println("2");
            break;
        case NEXT:
            Serial.println("3");
            break;
        case PREVIOUS:
            Serial.println("4");
            break;
        default:
            Serial.println("0");
            break;
    }
    previousResult = response;
}

void serialEvent()
{   
    newString = ""; // clear the string
    while (Serial.available())
    {
        char inChar = Serial.read();
        if (inChar == '\n')
        {
            Serial.println("");
            return;
        }
        else
        {
            Serial.print(inChar);
            newString += inChar;
        }
    }
    inputString = newString;
}

void printResponse(){
    Serial.println("SONG NAME? =" + newString);
}