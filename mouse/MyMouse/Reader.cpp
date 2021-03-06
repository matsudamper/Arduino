#include <Arduino.h>
#include <stdio.h>
#include "Reader.h"
#include "kotlin.h"

Reader::Reader(const int PIN_ID_X, const int PIN_ID_Y, const int PIN_ID_SCROLL, const int PIN_ID_LEFT, const int PIN_ID_RIGHT)
{
    pinMode(PIN_ID_X, INPUT_PULLUP);
    pinMode(PIN_ID_Y, INPUT_PULLUP);
    pinMode(PIN_ID_SCROLL, INPUT_PULLUP);
    pinMode(PIN_ID_LEFT, INPUT_PULLUP);
    pinMode(PIN_ID_RIGHT, INPUT_PULLUP);

    this->PIN_ID_X = PIN_ID_X;
    this->PIN_ID_Y = PIN_ID_Y;
    this->PIN_ID_SCROLL = PIN_ID_SCROLL;
    this->PIN_ID_LEFT = PIN_ID_LEFT;
    this->PIN_ID_RIGHT = PIN_ID_RIGHT;

    HOME_X = readX();
    HOME_Y = readY();
}

int Reader::led(int LED_PIN_ID, boolean value) { digitalWrite(LED_PIN_ID, value ? HIGH : LOW); }
Reader::readX() { return analogRead(PIN_ID_X) - HOME_X; }
Reader::readY() { return analogRead(PIN_ID_Y) - HOME_Y; }
boolean Reader::isScroll() { return digitalRead(PIN_ID_SCROLL) == false; }
boolean Reader::isLeftClick(){ return digitalRead(PIN_ID_LEFT) == false; }
boolean Reader::isRightClick(){ return digitalRead(PIN_ID_RIGHT) == false; }

float Reader::getXPar(int x) { return (float)x / (x < 0 ? MIN_X : MAX_X); }
float Reader::getYPar(int y) { return (float)y / (y < 0 ? MIN_Y : MAX_Y); }

void Reader::Calibration(int LED_PIN_ID)
{
    // LED準備
    pinMode(LED_PIN_ID, OUTPUT);

    int millsCounter = 0;
    while (millsCounter < 3000)
    {
        led(LED_PIN_ID, millsCounter % 1000 < 500);

        MAX_X = max(MAX_X, readX());
        MAX_Y = max(MAX_Y, readY());

        MIN_X = min(MIN_X, readX());
        MIN_Y = min(MIN_Y, readY());

        if (max(abs(readX()), abs(readY())) > 100)
        {
            millsCounter += 5;
        }
        delay(5);
    }
}

Reader::Level Reader::getLevel(float par)
{
    when(par)
    {
        until(0, 0.2)
        {
            return Level::Level_NONE;
        }
        until(0.2, 0.5)
        {
            return Level::Level_ONE;
        }
        until(0.5, 0.7)
        {
            return Level::Level_TWO;
        }
        until(0.7, 0.95)
        {
            return Level::Level_THREE;
        }
        more(0.95)
        {
            return Level::Level_MAX;
        }
    }

    return Level::Level_NONE;
}
