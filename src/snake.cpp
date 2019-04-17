#ifndef ARDUINO
#define ARDUINO 10800
#endif

#include <stdint.h>
#include <Arduino.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <avr/pgmspace.h>
#include "portSettings.h"
#include "snake.h"
#include "define.h"

void Snake::drawSnakePart(int _x, int _y, int8_t imageId)
{
    _x = _x << 2;
    _y = _y << 2;
    int x = 0;
    int y = 0;

    for (int i = imageId; i < imageId + 2; i++)
    {
        byte data = pgm_read_byte(&SnakePartsImage[i]);

        for (int j = 0; j < 8; j++)
        {
            display.writePixel(_x + x, _y + y, (data & 0x01 << j) ? BLACK : WHITE);
            x++;
            if (x >= 4)
            {
                x = 0;
                y++;
            }
        }
    }
}

Snake::Snake()
{
    this->direction = this->Direction::RIGHT; // default
}

void Snake::init(uint8_t parts, uint8_t startx, uint8_t starty)
{
    this->head = &this->parts[1];
    this->tail = &this->parts[0];

    // create head
    //this->head = &this->parts[0];
    this->head->from = SnakePartDirection::LEFT;
    this->head->to = SnakePartDirection::NONE;
    this->head->apple = false;

    //create tail
    //this->tail = &this->parts[1];
    this->tail->from = SnakePartDirection::NONE;
    this->tail->to = SnakePartDirection::RIGHT;
    this->tail->apple = false;

    // connect head and tail
    this->head->next = this->tail;
    this->head->previous = NULL;
    this->tail->previous = this->head;
    this->tail->next = NULL;

    // start position
    this->tail->x = startx;
    this->tail->y = starty;

    switch (this->direction)
    {
    case UP:
        this->head->x = startx;
        this->head->y = starty - 1;
        break;
    case RIGHT:
        this->head->x = startx + 1;
        this->head->y = starty;
        break;
    case DOWN:
        this->head->x = startx;
        this->head->y = starty + 1;
        break;
    case LEFT:
        this->head->x = startx - 1;
        this->head->y = starty;
        break;
    }

    this->currentPartId = 2;
    this->snakeLength = 2;

    if (this->ghostMode)
    {
        this->playingField[0] = 0;
        this->playingField[1] = snakeXPositions;
        this->playingField[2] = 0;
        this->playingField[3] = snakeYPositions;
    }
    else
    {
        this->playingField[0] = 1;
        this->playingField[1] = snakeXPositions - 1;
        this->playingField[2] = 1;
        this->playingField[3] = snakeYPositions - 1;
    }

    display.clearDisplay();
    if (!this->ghostMode)
    {
        display.drawRect(1, 1, displayWidth - 2, displayHeight - 2, BLACK);
    }
    this->direction = this->Direction::RIGHT;

    for (uint8_t i = 2; i < parts; i++)
    {
        this->moveAndGrow();
    }

    this->drawScore(this->snakeLength);

    this->apple.eaten = true; // no apple set
}

void Snake::drawScore(uint8_t score)
{
    int diff = (displayWidth - this->maxParts * 2) / 2 - 2;
    display.drawLine(1, 0, diff, 0, BLACK);
    display.drawLine(displayWidth - 1 - diff, 0, displayWidth - 2, 0, BLACK);

    for (int i = 1; i <= score; i++)
    {
        display.drawPixel(diff + i * 2, 0, BLACK);
    }
}

void Snake::drawSnake()
{
    SnakePart *currentPart = this->head;

    while (currentPart != NULL)
    {
        uint8_t partId = SnakePartsTranslationTable[currentPart->from][currentPart->to];
        if (currentPart->apple)
        {
            if (partId == 2 || partId == 4 || partId == 24 || partId == 26)
                partId = 14; // special apple
            else if (partId < 12 || partId == 28 || partId == 30)
                partId = 12; // no apple for head or tail
            else if (partId >= 16 && partId <= 22)
                partId = partId += 20;
        }

        this->drawSnakePart(currentPart->x, currentPart->y, partId);
        currentPart = currentPart->next;
    }
    this->drawApple();
    this->drawScore(this->snakeLength);
    display.display();
}

void Snake::moveAndGrow()
{
    this->snakeLength++;
    if (++this->currentPartId >= this->maxParts)
        this->currentPartId = 0;
    SnakePart *newPart = &this->parts[this->currentPartId]; // get a new part from the buffer

    newPart->apple = false;
    newPart->to = SnakePartDirection::NONE;

    newPart->next = this->head; // connect new part before the head
    this->head = newPart;       // make the new part the new head

    newPart->next->previous = newPart;        // connect new part to old head
    newPart->next->to = (int)this->direction; // direction is where the old head is going to

    newPart->x = newPart->next->x; // copy x and y
    newPart->y = newPart->next->y;
    // move head to direction
    switch (this->direction)
    {
    case UP:
        this->head->y--;
        if (this->ghostMode && (this->head->y < this->playingField[2] || this->head->y == 255))
            this->head->y = this->playingField[3] - 1;
        this->head->from = SnakePartDirection::DOWN;
        break;
    case RIGHT:
        this->head->x++;
        if (this->ghostMode && this->head->x >= this->playingField[1])
            this->head->x = this->playingField[0];
        this->head->from = SnakePartDirection::LEFT;
        break;
    case DOWN:
        this->head->y++;
        if (this->ghostMode && this->head->y >= this->playingField[3])
            this->head->y = this->playingField[2];
        this->head->from = SnakePartDirection::UP;
        break;
    case LEFT:
        this->head->x--;
        if (this->ghostMode && (this->head->x < this->playingField[0] || this->head->x == 255))
            this->head->x = this->playingField[1] - 1;
        this->head->from = SnakePartDirection::RIGHT;
        break;
    }
}

void Snake::move()
{
    this->drawSnakePart(this->tail->x, this->tail->y, 32); // overwrite the old tail with nothing
    this->tail = this->tail->previous;                     // make the part previous to the tail the new tail
    this->tail->next = NULL;                               // remove reference to old part
    this->tail->from = NONE;                               // make it a tail
    this->moveAndGrow();
    this->snakeLength--;
}

void Snake::getDirection()
{
    int x = analogRead(pJoyStickX);
    int y = analogRead(pJoyStickY);

    int x_diff = x - 504;
    if (x_diff < 0)
        x_diff = 504 - x;

    int y_diff = y - 504;
    if (y_diff < 0)
        y_diff = 504 - y;

    if (x_diff < directionThreshold && y_diff < directionThreshold)
        return; // direction stays the same

    if (x_diff > y_diff)
    {
        if (this->direction == this->Direction::RIGHT || this->direction == this->Direction::LEFT)
            return;
        if (x > 504)
            this->direction = this->Direction::RIGHT;
        else
            this->direction = this->Direction::LEFT;
    }
    else
    {
        if (this->direction == this->Direction::UP || this->direction == this->Direction::DOWN)
            return;
        if (y > 504)
            this->direction = this->Direction::UP;
        else
            this->direction = this->Direction::DOWN;
    }
}

bool Snake::checkBodyCollision()
{
    SnakePart *current = this->head->next;

    while (current != NULL)
    {
        if (this->head->x == current->x && this->head->y == current->y)
            return true;
        current = current->next;
    }
    return false;
}

bool Snake::checkFieldCollision()
{
    if (this->ghostMode)
        return false;
    if (this->head->x < this->playingField[0] ||
        this->head->x > this->playingField[1] - 1 ||
        this->head->y < this->playingField[2] ||
        this->head->y > this->playingField[3] - 1)
        return true;
    return false;
}

void Snake::createApple()
{
    // check if apple is set
    if (this->apple.eaten)
    {
        DEBUG_PRINT("creating");
        this->apple.eaten = false;
        this->apple.x = random(this->playingField[0], this->playingField[1]);
        this->apple.y = random(this->playingField[2], this->playingField[3]);
    }
    else
        DEBUG_PRINT("no apple created");
}

void Snake::drawApple()
{
    if (this->apple.eaten)
    {
        return;
    }
    this->drawSnakePart(this->apple.x, this->apple.y, 34);
}

void Snake::clearApple()
{
    this->drawSnakePart(this->apple.x, this->apple.y, 32);
}

bool Snake::eatApple()
{
    if (this->apple.eaten)
        return false;
    if (this->head->x == this->apple.x && this->head->y == this->apple.y)
    {
        this->head->apple = true;
        this->apple.eaten = true;
        this->clearApple();
        this->createApple();
        this->drawScore(this->snakeLength + 1);
        DEBUG_PRINT("apple eaten");
        return true;
    }
    return false;
}

bool Snake::isWinner()
{
    return this->snakeLength >= this->maxParts;
}
