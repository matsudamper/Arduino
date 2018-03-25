#pragma once
#include <Arduino.h>
#include "Reader.h"

class DoubleGesture
{
  public:
    DoubleGesture();

    bool firstMode = false;

    setLevel(Reader::Level level);

  private:
    unsigned long firstDownTime = 0;
    unsigned long secondDownTime = 0;
    unsigned long finishTime = NULL;
    bool firstFlag = false;
    bool secondFlag = false;
};