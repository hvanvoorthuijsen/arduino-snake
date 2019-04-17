#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>

const byte PROGMEM SnakePartsImage[] = {
    0b11000000, 0b01101010, // DOWN_RIGHT 0
    0b10100110, 0b00001100, // UP_RIGHT 2
    0b00110000, 0b01100101, // DOWN_LEFT 4
    0b01010110, 0b00000011, // UP_LEFT 6
    0b11010000, 0b00001011, // LEFT_RIGHT 8
    0b01000110, 0b01100010, // UP_DOWN 10
    0b11010110, 0b01101011, // APPLE 1 12
    0b10110110, 0b01101101, // APPLE 2 14
    0b01010010, 0b00000111, // HEAD RIGHT 16
    0b10100100, 0b00001110, // HEAD LEFT 18
    0b10100110, 0b00000110, // HEAD DOWN 20
    0b01100000, 0b01100101, // HEAD UP 22
    0b10000000, 0b00001110, // TAIL RIGHT 24
    0b00010000, 0b00000111, // TAIL LEFT 26
    0b01000110, 0b00000100, // TAIL UP 28
    0b00100000, 0b01100010, // TAIL DOWN 30
    0x00, 0x00,             // BLANC 32
    0b00010010, 0b01011111, // mouse 34
    0b01011010, 0b11100111, // HEAD RIGHT 36
    0b10100101, 0b01111110, // HEAD LEFT 38
    0b10110110, 0b10010111, // HEAD DOWN 40
    0b11101001, 0b01101101  // HEAD UP 42
};

const uint8_t SnakePartsTranslationTable[5][5] = {
    {32, 2, 10, 6, 20},  // UP->UP (blanc), UP->RIGHT, UP->DOWN, UP->LEFT, UP->NONE (head down)
    {2, 32, 0, 8, 18},   // RIGHT->UP, RIGHT->RIGHT (blanc), RIGHT->DOWN, RIGHT->LEFT, RIGHT->NONE (head left)
    {10, 0, 32, 4, 22},  // DOWN->UP, DOWN->RIGHT, DOWN->DOWN (blanc), DOWN->LEFT, DOWN->NONE (head up)
    {6, 8, 4, 32, 16},   // LEFT->UP, LEFT->RIGHT, LEFT->DOWN, LEFT->LEFT, LEFT->NONE (head right)
    {28, 24, 30, 26, 32} //tials: NONE->UP , NONE->RIGHT, NONE->DOWN, NONE->LEFT, NONE->NONE (blanc)
};

typedef enum SnakePartDirection
{
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NONE
};

typedef struct SnakePart
{
    enum SnakePartDirection from : 3;
    enum SnakePartDirection to : 3;
    boolean apple : 2;

    uint8_t x, y;

    SnakePart *next;
    SnakePart *previous;
};

typedef struct SnakeApple
{
    uint8_t x, y;
    boolean eaten;
};

class Snake
{
  public:
    static const uint8_t maxParts = 30;
    Snake();
    SnakePart parts[maxParts];
    SnakePart *head;
    SnakePart *tail;
    SnakeApple apple;
    enum Direction
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    } direction;
    int8_t currentPartId;
    uint8_t playingField[4] = {0, 21, 0, 12};
    bool ghostMode = true;
    uint8_t snakeLength;

    void init(uint8_t parts, uint8_t startx = 0, uint8_t starty = 0);
    void drawSnakePart(int _x, int _y, int8_t imageId);
    void drawSnake();
    void moveAndGrow();
    void move();
    void getDirection();
    bool checkBodyCollision();
    bool checkFieldCollision();
    void drawApple();
    void createApple();
    void clearApple();
    bool eatApple();
    void drawScore(uint8_t score);
    bool isWinner();
};

#endif
