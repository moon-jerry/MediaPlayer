// Author: Michael Terry

#include "TimerEvent.h"
#include "TimerListener.h"
#include <sstream>

using namespace std;

namespace cs349
{

  TimerEvent::TimerEvent(Timer* timer) : Event()
  {
    this->timer = timer;
  }

  TimerEvent::~TimerEvent()
  {
  }

  void TimerEvent::DispatchEvent()
  {
    for (vector<TimerListener*>::iterator iter = this->timer->timerListeners.begin(); iter != this->timer->timerListeners.end(); iter++) {
      TimerListener* l = (*iter);
      l->HandleTimerEvent(*this);
    }
  }

  Timer* TimerEvent::GetTimer() const
  {
    return this->timer;
  }

  string TimerEvent::ToString() const {
    stringstream ss;
    ss << (*this);
    return ss.str();
  }

  ostream & operator << (ostream &os, const TimerEvent & e) {
    os << "TimerEvent";
    return os;
  }
}
