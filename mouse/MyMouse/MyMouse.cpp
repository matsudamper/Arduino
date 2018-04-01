#include <Arduino.h>
#include <stdio.h>
#include "Mouse.h"
#include "Reader.h"
#include "kotlin.h"

// PIN
const int LED_PIN_ID = 2;
const int PIN_X = A2;
const int PIN_Y = A1;
const int PIN_SCROLL = 3;
const int PIN_LEFT = 4;
const int PIN_RIGHT = 5;

Reader *reader;
unsigned long before = 0;

bool beforeLeftState = false;
bool beforeRightState = false;
unsigned long beforeLeftClickTime = NULL;
unsigned long beforeRightClickTime = NULL;

char buf[40];

float getMove(Reader::Level level)
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

void setup()
{
    Serial.begin(9600);
    Mouse.begin();

    reader = new Reader(PIN_X, PIN_Y, PIN_SCROLL, PIN_LEFT, PIN_RIGHT);
    reader->Calibration(LED_PIN_ID);

    before = millis();
}

void loop()
{
    unsigned long diff = millis() - before;
    before = millis();

    int x = reader->readX();
    int y = reader->readY();

    float xPar = reader->getXPar(x);
    float yPar = reader->getYPar(y);

    int xDirection = x < 0 ? -1 : 1;
    int yDirection = y < 0 ? -1 : 1;

    Reader::Level xLevel = reader->getLevel(xPar);
    Reader::Level yLevel = reader->getLevel(yPar);
    float moveX = getMove(xLevel);
    float moveY = getMove(yLevel);

    // Button
    if (reader->isLeftClick())
    {
        if (Mouse.isPressed(MOUSE_LEFT) == false)
            Mouse.press(MOUSE_LEFT);
    }
    else
    {
        if (Mouse.isPressed(MOUSE_LEFT))
            Mouse.release(MOUSE_LEFT);
    }

    if (reader->isRightClick())
    {
        if (beforeRightState == false)
            beforeRightClickTime = millis();
    }
    else
    {
        if (beforeRightClickTime != NULL)
        {
            if (millis() - beforeRightClickTime < 150)
            {
                Mouse.click(MOUSE_RIGHT);
            }
            beforeRightClickTime = NULL;
        }
    }

    beforeLeftState = reader->isLeftClick();
    beforeRightState = reader->isRightClick();

    // Cursor
    if (reader->isScroll())
    {
        long speed = (reader->isRightClick() ? 0.03 : 0.01) * diff;

        if (xPar < yPar)
        {
            Mouse.move(0, 0, yDirection * -1 * speed * moveY, 0);
        }
        else
        {
            Mouse.move(0, 0, 0, xDirection * speed * moveX);
        }
        delay(150);
    }
    else
    {
        long speed = (reader->isRightClick() ? 0.2 : 0.05) * diff;
        Mouse.move(moveX * xDirection * speed,
                   moveY * yDirection * speed,
                   0, 0);

        delay(50);
    }
}
