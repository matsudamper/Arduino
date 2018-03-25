#include "DoubleGesture.h"
#include "Reader.h"

DoubleGesture::DoubleGesture()
{
}

DoubleGesture::setLevel(Reader::Level level)
{

    switch (level)
    {
    case Reader::Level::Level_NONE:
        firstDownTime = millis();

        if (finishTime == NULL)
        {
            finishTime = millis();
        }
        else
        {
            if (finishTime + 100 < millis())
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
}