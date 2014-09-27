// Author: Michael Terry

#ifndef __TIMERLISTENER__
#define __TIMERLISTENER__

namespace cs349
{
  class TimerListener;
}

#include "TimerEvent.h"

namespace cs349
{

  /**
   * This is an abstract class that must be subclassed by
   * any object interested in receiving timer events. The
   * HandleTimerEvent method will be called whenever the
   * elapsed time has passed in a timer.
   */
  class TimerListener
  {

  public:
    /**
     * Default constructor does nothing
     */
    TimerListener();	
    virtual ~TimerListener();

    /**
     * Override this to be notified when a given Timer fires.
     */
    virtual void HandleTimerEvent(const TimerEvent & e) = 0;
  };
}

#endif /* __TIMERLISTENER__ */
