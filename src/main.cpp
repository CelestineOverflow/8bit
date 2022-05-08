#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "characters.h"
#define led7 18
#define led6 19
#define led5 23
#define led4 5
#define led3 13
#define led2 12
#define led1 14
#define led0 27
#define inputPin 17
#define baudRate 115200
#define DEBUG 0
#define ASCII_START 48
#define ASCII_END 122
#define MAX_CHARACTER_COUNT 10
char inputString[MAX_CHARACTER_COUNT + 1]; // +1 for null terminator
// prototypes
void printColumn(int column);
void input_character(char c, int column);
void print_character(char c, int column);
void displayString(String input_characters, bool forward);
void displayString_front(String input_characters);
void displayString_back(String input_characters);
void *mainThread(void *arg);
void inputHandler();
void front(String input_characters);
void back(String input_characters);
void setup();
void loop();
void input(String input_characters);

void setup()
{
  input("CELESTE");
  Serial.begin(baudRate); // start serial communication
  Serial.println("Starting...");
  pinMode(led7, OUTPUT); // set led pin to output
  pinMode(led6, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led0, OUTPUT);
  pinMode(inputPin, INPUT_PULLDOWN);                                      // set input pin to pulldown
  attachInterrupt(digitalPinToInterrupt(inputPin), inputHandler, CHANGE); // attach interrupt to input pin
  Serial.println("Setup complete.");
}
#define FONT_WITDH_PIXELS 8

unsigned long periodPerStroke = 1000000; // in microseconds (1 second)
unsigned long timePerColumnMicrosecond = (periodPerStroke / MAX_CHARACTER_COUNT) / FONT_WITDH_PIXELS;
unsigned long loopStart = 0;
bool forward = true;

uint32_t loopStartTime = 0;
u_int32_t loopEndTime = 0;

void loop()
{
  if (millis() - loopStartTime > 1000)
  {
    loopStartTime = millis();
    Serial.printf("forward: %d", forward);
    Serial.println("periodPerStroke: " + String(periodPerStroke));
  }
}
void *mainThread(void *arg)
{
  displayString(inputString, forward);
  return NULL;
}

pthread_t aThread;

void inputHandler()
{
  pthread_cancel(aThread);                          // delete the old thread
  pthread_create(&aThread, NULL, mainThread, NULL); // create a new thread to execute the display
  forward = !forward;
  periodPerStroke = (micros() - loopStart) / 2;
  timePerColumnMicrosecond = (periodPerStroke / MAX_CHARACTER_COUNT) / FONT_WITDH_PIXELS;
  loopStart = micros();
}

void front(String input_characters)
{
  unsigned long startTime = micros();
  for (int i = 0; i < MAX_CHARACTER_COUNT; i++)
  {
    int j = 0;
    while (j < FONT_WITDH_PIXELS)
    {
      if (micros() - startTime > timePerColumnMicrosecond)
      {
        startTime = micros();
        j++;
      }
      if(DEBUG) print_character(input_characters[i], j);
    }
  }
}
void back(String input_characters)
{
  unsigned long startTime = micros();
  for (int i = MAX_CHARACTER_COUNT - 1; i >= 0; i--)
  {
    int j = MAX_CHARACTER_COUNT;
    while (j >= 0)
    {
      if (micros() - startTime > timePerColumnMicrosecond)
      {
        startTime = micros();
        j--;
      }
      if(DEBUG) print_character(input_characters[i], j);
    }
  }
}

void displayString(String input_characters, bool forward)
{
  if (forward)
  {
    front(input_characters);
  }
  else
  {
    back(input_characters);
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

void print_character(char c, int column)//
{
  int id = ((int)c);
  if (id >= ASCII_START && id <= ASCII_END) // check if character is allowed
  {
    int column_id = (id - ASCII_START) * FONT_WITDH_PIXELS + column;
    if (DEBUG)
    {
      Serial.printf("c = %c id = %d", c, id);
      Serial.printf(" column_id = %d\n", column_id);
    }
    printColumn(column_id);
  }
  else
  {
    Serial.println("invalid character");
  }
}
void input(String input_characters)// input the characters to the display
{
  int string_length = input_characters.length() - 1;// -1 to remove null terminator
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