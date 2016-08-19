//=======================================================================================
// GameTimer.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef GAMETIMER_H
#define GAMETIMER_H


class GameTimer
{
public:
  GameTimer();

  float getGameTime()const;  // in seconds
  float getDeltaTime()const; // in seconds

  void init(); // Call before message loop.
  void reset(); // Call before message loop.
  void start(); // Call when unpaused.
  void stop();  // Call when paused.
  void tick();  // Call every frame.

private:
  double mSecondsPerCount;
  double mDeltaTime;

  LONGLONG mBaseTime;
  LONGLONG mPausedTime;
  LONGLONG mStopTime;
  LONGLONG mPrevTime;
  LONGLONG mCurrTime;

  bool mStopped;
};

#endif // GAMETIMER_H