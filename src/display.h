#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "characters.h"

#define inputPin 18
#define led7 19
#define led6 23
#define led5 5
#define led4 13
#define led3 12
#define led2 14
#define led1 27
#define led0 16
#define ASCII_START 48
#define ASCII_END 122
#define MAX_CHARACTER_COUNT 10
#define FONT_WITDH_PIXELS 8

// prototypes
typedef enum
{
    FORWARD,
    BACK
} direction;
direction currdirection = FORWARD;
void checkInput(String input_character);
void printColumn(int column);
void input_character(char c, int column);
void print_character(char c, int column);
void displayString(direction direction);
void inputHandler();
void notDirection();
void clearShit();
// Global variables
String inputString = "__CELESTE__";
pthread_mutex_t mutex;
bool restartProcess = false;
long periodPerStroke = 29862; // in microseconds (1 second)
long timePerColumnMicrosecond = (periodPerStroke / MAX_CHARACTER_COUNT) / FONT_WITDH_PIXELS;
bool continueThread = true;
u_int32_t loopStart = 0;
u_int32_t loopEndTime = 0;


void setupDisplay()
{   
    pinMode(led7, OUTPUT); // set led pin to output
    pinMode(led6, OUTPUT);
    pinMode(led5, OUTPUT);
    pinMode(led4, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(led0, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(inputPin), inputHandler, CHANGE); // attach interrupt to input pin
    Serial.println("Display Setup complete!");
}

void *mainThread(void *arg)
{
    long startTime = micros();
    displayString(currdirection);
    long endTime = micros() - startTime;
    //Serial.println("Time taken: " + String(endTime) + "current direction: " + String(currdirection));
    return NULL;
}

pthread_t displayThread;

void inputHandler()
{   
    continueThread = false;
    pthread_join(displayThread, NULL);
    
    continueThread = true;
    notDirection();
    pthread_create(&displayThread, NULL, mainThread, NULL);
    periodPerStroke = (micros() - loopStart);
    timePerColumnMicrosecond = (periodPerStroke / (inputString.length() + 1)) / FONT_WITDH_PIXELS;
    loopStart = micros();
    restartProcess = true;
}
void notDirection(){
    if(currdirection == FORWARD){
        currdirection = BACK;
    }
    else{
        currdirection = FORWARD;
    }
}

void front()
{
    unsigned long startTime = micros();
    for (int i = 0; i < inputString.length(); i++)
    {
        int j = 0;
        while (j < FONT_WITDH_PIXELS)
        {
            if (micros() - startTime > timePerColumnMicrosecond)
            {
                startTime = micros();
                j++;
            }
            if (continueThread)
            {   
                clearShit();
                print_character(inputString[i], j);
            }
            else
            {
                return;
            }
        }
    }
}
void back()
{
    delayMicroseconds(100);
    unsigned long startTime = micros();
    for (int i = inputString.length()-1; i >= 0; i--)
    {
        int j = FONT_WITDH_PIXELS-1;
        while (j >=0)
        {
            if (micros() - startTime > timePerColumnMicrosecond)
            {
                startTime = micros();
                j--;
            }
            if (continueThread)
            {
                clearShit();
                print_character(inputString[i], j);
            }
            else
            {
                return;
            }
        }
    }
}

void displayString(direction direction)
{
    if (!continueThread)
    {
        return;
    }
    {
        if (!continueThread)
        {
            return;
        }
        if (direction == FORWARD)
        {
            front();
        }
        else
        {
            //back();
        }
    }
}
void printColumn(int column) // print the column for output leds
{
    digitalWrite(led7, characters[0][column]);
    digitalWrite(led6, characters[1][column]);
    digitalWrite(led5, characters[2][column]);
    digitalWrite(led4, characters[3][column]);
    digitalWrite(led3, characters[4][column]);
    digitalWrite(led2, characters[5][column]);
    digitalWrite(led1, characters[6][column]);
    digitalWrite(led0, characters[7][column]);
}

void print_character(char c, int column) //
{
    int id = ((int)c);
    if (id >= ASCII_START && id <= ASCII_END) // check if character is allowed
    {
        int column_id = (id - ASCII_START) * FONT_WITDH_PIXELS + column;
        printColumn(column_id);
    }
    else
    {
        Serial.println("invalid character");
    }
}

void printSerialColumn(int column) // prints the column to the serial monitor
{
    Serial.print(characters[0][column]);
    Serial.print(characters[1][column]);
    Serial.print(characters[2][column]);
    Serial.print(characters[3][column]);
    Serial.print(characters[4][column]);
    Serial.print(characters[5][column]);
    Serial.print(characters[6][column]);
    Serial.println(characters[7][column]);
}

void clearShit(){
    digitalWrite(led7, 0);
    digitalWrite(led6, 0);
    digitalWrite(led5, 0);
    digitalWrite(led4, 0);
    digitalWrite(led3, 0);
    digitalWrite(led2, 0);
    digitalWrite(led1, 0);
    digitalWrite(led0, 0);
}

void checkInput(String input_characters) // input the characters to the display
{
    int string_length = input_characters.length() - 1; // -1 to remove null terminator
    if (string_length > MAX_CHARACTER_COUNT)
    {
        Serial.println("input string too long");
    }
    for (int i = 0; i < string_length; i++)
    {
        inputString[i] = input_characters[i];
    }
    if (string_length < MAX_CHARACTER_COUNT)
    {
        for (int i = string_length; i < MAX_CHARACTER_COUNT; i++)
        {
            inputString[i] = '_';
        }
    }
}