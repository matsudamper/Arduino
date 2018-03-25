#include <Arduino.h>
#include <stdio.h>
#include "Mouse.h"
#include "Reader.h"
#include "kotlin.h"

// PIN
const int LED_PIN_ID = 2;
const int PIN_X = A2;
const int PIN_Y = A1;

// Value
Reader *reader;
unsigned long firstDownTime = 0;
unsigned long secondDownTime = 0;
unsigned long finishTime = NULL;

char buf[40];

int getMove(Reader::Level level)
{
    switch (level)
    {
    case Reader::Level::Level_NONE:
        return 0;

    case Reader::Level::Level_ONE:
        return 1;

    case Reader::Level::Level_TWO:
        return 2;

    case Reader::Level::Level_THREE:
        return 3;

    case Reader::Level::Level_MAX:
        return 5;
    }
}

boolean firstFlag = false;
boolean secondFlag = false;
boolean firstMode = false;

void setup()
{
    Serial.begin(9600);
    Mouse.begin();

    reader = new Reader(PIN_X, PIN_Y);
    reader->Calibration(LED_PIN_ID);
}

void loop()
{

    int x = reader->readX();
    int y = reader->readY();

    float xPar = reader->getXPar(x);
    float yPar = reader->getYPar(y);

    int xDirection = x < 0 ? -1 : 1;
    int yDirection = y < 0 ? -1 : 1;

    Reader::Level xLevel = reader->getLevel(xPar);
    Reader::Level ylevel = reader->getLevel(yPar);

    switch (xLevel)
    {
    case Reader::Level::Level_NONE:
        firstDownTime = millis();

        if (finishTime == NULL)
        {
            finishTime = millis();
        }
        else
        {
            if (finishTime + 200 < millis())
            {
                firstMode = false;
            }
        }

        if (firstFlag)
        {
            if (secondFlag)
            {
                firstFlag = false;
                secondFlag = false;
            }
            else
            {
                secondDownTime = millis();
            }
        }
        break;

    default:
        finishTime = NULL;
        if (firstDownTime + 500 > millis())
        {
            firstFlag = true;
        }
        else
        {
            firstFlag = false;
        }
        break;

    case Reader::Level::Level_MAX:
        finishTime = NULL;

        if (firstFlag && secondDownTime + 500 > millis())
        {
            firstMode = true;
        }
        break;
    }

    if (firstMode)
    {
        xDirection *= 5;
        yDirection *= 5;
    }

    Mouse.move(getMove(reader->getLevel(xPar)) * xDirection,
               getMove(reader->getLevel(yPar)) * yDirection,
               0);

    delay(50);
}