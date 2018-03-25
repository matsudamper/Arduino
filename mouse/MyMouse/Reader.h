#pragma once

class Reader
{
public:
  Reader(int PIN_ID_X, int PIN_ID_Y);
  enum class Level
  {
    Level_NONE,
    Level_ONE,
    Level_TWO,
    Level_THREE,
    Level_MAX
  };

  void Calibration(int LED_PIN_ID);

  Level getLevel(float par);

  int readX();
  int readY();
  float getXPar(int x);
  float getYPar(int y);

private:
  int PIN_ID_X;
  int PIN_ID_Y;

  int HOME_X = 0;
  int HOME_Y = 0;

  int MAX_X = 0;
  int MAX_Y = 0;

  int MIN_X = 0;
  int MIN_Y = 0;

  int led(int LED_PIN_ID, bool value);
};