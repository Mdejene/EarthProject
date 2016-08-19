//=======================================================================================
// GameTimer.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================


#include <windows.h>

#include "GameTimer.h"

#define PERF_COUNT_AFFINITY

GameTimer::GameTimer()
//: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), 
//  mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{

  static LARGE_INTEGER yo;
  LONGLONG countsPerSec = 0LL;
  init();

  DWORD_PTR oldAffinityMask = ::SetThreadAffinityMask( ::GetCurrentThread(), 0 );
  QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
  ::SetThreadAffinityMask( ::GetCurrentThread(), oldAffinityMask );

  mSecondsPerCount = 1.0 / (double)countsPerSec;
}
void GameTimer::init()
{
  mSecondsPerCount = 0.0F;
  mDeltaTime  = -1.0F;
  mBaseTime   = 0LL; 
  mPausedTime = 0LL;
  mPrevTime   = 0LL;
  mCurrTime   = 0LL;
  mStopTime   = 0LL;
  mStopped    = false;
}
// Returns the total time elapsed since reset() was called, NOT counting any
// time when the clock is stopped.
float GameTimer::getGameTime()const
{
  // If we are stopped, do not count the time that has passed since we stopped.
  //
  // ----*---------------*------------------------------*------> time
  //  mBaseTime       mStopTime                      mCurrTime

  if( mStopped )
  {
    return (float)((mStopTime - mBaseTime)*mSecondsPerCount);
  }

  // The distance mCurrTime - mBaseTime includes paused time,
  // which we do not want to count.  To correct this, we can subtract 
  // the paused time from mCurrTime:  
  //
  //  (mCurrTime - mPausedTime) - mBaseTime 
  //
  //                     |<-------d------->|
  // ----*---------------*-----------------*------------*------> time
  //  mBaseTime       mStopTime        startTime     mCurrTime
  
  else
  {
    return (float)(((mCurrTime-mPausedTime)-mBaseTime)*mSecondsPerCount);
  }
}

float GameTimer::getDeltaTime()const
{
  return (float)mDeltaTime;
}

void GameTimer::reset()
{
  LONGLONG currTime;

#ifdef PERF_COUNT_AFFINITY
  DWORD_PTR oldAffinityMask = ::SetThreadAffinityMask( ::GetCurrentThread(), 0 );
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  ::SetThreadAffinityMask( ::GetCurrentThread(), oldAffinityMask );
#else
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
#endif
  mBaseTime = currTime;
  mPrevTime = currTime;
  mStopTime = 0L;
  mStopped  = false;
}

void GameTimer::start()
{
  LONGLONG startTime;

#ifdef PERF_COUNT_AFFINITY
  DWORD_PTR oldAffinityMask = ::SetThreadAffinityMask( ::GetCurrentThread(), 0 );
  QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
  ::SetThreadAffinityMask( ::GetCurrentThread(), oldAffinityMask );
#else
  QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
#endif

  // Accumulate the time elapsed between stop and start pairs.
  //
  //                     |<-------d------->|
  // ----*---------------*-----------------*------------> time
  //  mBaseTime       mStopTime        startTime     

  if( mStopped )
  {
    mPausedTime += (startTime - mStopTime);	
    mPrevTime = startTime;
    mStopTime = 0;
    mStopped  = false;
  }
}

void GameTimer::stop()
{
  if( !mStopped )
  {
    LONGLONG currTime;
#ifdef PERF_COUNT_AFFINITY
  DWORD_PTR oldAffinityMask = ::SetThreadAffinityMask( ::GetCurrentThread(), 0 );
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  ::SetThreadAffinityMask( ::GetCurrentThread(), oldAffinityMask );
#else
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
#endif

    mStopTime = currTime;
    mStopped  = true;
  }
}

void GameTimer::tick()
{
  if( mStopped )
  {
    mDeltaTime = 0.0F;
    return;
  }

  LONGLONG currTime = 0LL;

#ifdef PERF_COUNT_AFFINITY
  DWORD_PTR oldAffinityMask = ::SetThreadAffinityMask( ::GetCurrentThread(), 0 );
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  ::SetThreadAffinityMask( ::GetCurrentThread(), oldAffinityMask );
#else
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
#endif

  mCurrTime = currTime;

  // Time difference between this frame and the previous.
  mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;

  // Prepare for next frame.
  mPrevTime = mCurrTime;

  // Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
  // processor goes into a power save mode or we get shuffled to another
  // processor, then mDeltaTime can be negative.
  if(mDeltaTime < 0.0F)
  {
    mDeltaTime = 0.0F;
  }
}

