#ifndef ARDUINO
#define ARDUINO 10800
#endif

#define DEBUG

#include <SPI.h>
#include <Arduino.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
// #include <SD.h>
#include <avr/pgmspace.h>
#include "src/portSettings.h"
#include "src/snake.h"
#include "src/define.h"

bool checkPin(byte mask, bool reset = true);
void snakeLoop();

Snake snake;

void setup()
{
  analogWrite(pDisplayLED, 255);
#ifdef DEBUG
  Serial.begin(9600);
#endif
  DEBUG_PRINT("SNAKE");

  pinMode(pButtonA, INPUT_PULLUP);
  pinMode(pButtonB, INPUT_PULLUP);
  pinMode(pJoyStickButton, INPUT_PULLUP);

  display.begin(60); // contrast 50
  delay(1000);       // show hjvvlogo
  if (digitalRead(pButtonA) == LOW)
    snake.ghostMode = true;
  else
    snake.ghostMode = false;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setTextWrap(true);

  startSnake();
}

void loop()
{
  snakeLoop();
}

void snakeLoop()
{
  static bool snakeGrow = false;
  snake.getDirection();

  static unsigned long snakeTimer = millis();
  static int counter = 0;

  if (digitalRead(pButtonA) == LOW)
  {
    startSnake();
  }
  if (digitalRead(pButtonB) == LOW)
  {
    snakeGrow = true;
  }

  if (millis() - snakeTimer > snakeSpeed)
  {
    if (snakeGrow)
    {
      if (snake.isWinner())
      {
        display.setCursor(10, 10);
        display.println(F("WINNER"));
        display.display();
        delay(5000);
        startSnake();
        return;
      }
      snakeGrow = false;
      snakeSpeed -= 12;
      snake.moveAndGrow();
    }
    else
    {
      snake.move();
    }

    if (snake.checkBodyCollision() || snake.checkFieldCollision())
    {
      snake.drawSnake();
      display.setCursor(10, 10);
      display.println(F("GAME OVER"));
      display.setCursor(10, 20);
      display.println(snake.snakeLength);
      display.display();
      delay(2000);
      startSnake();
      return;
    }
    if (snake.eatApple())
    {
      snakeGrow = true;
    }
    snake.drawSnake();

    snakeTimer = millis();
  }
}

void startSnake()
{

  snake.init(5, 1, 1);
  snakeSpeed = snakeInitialSpeed;
  snake.createApple();
}
