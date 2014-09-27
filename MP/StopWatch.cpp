#include "XApplication.h"
#include "StopWatch.h"
#include <sstream>
#include <time.h>

using namespace std;

namespace cs349 {

  StopWatch::StopWatch(EventQueue* eventQueue, const Rectangle & bounds) : XWindow(string("Stop Watch!"), bounds) {
    this->running = false;
    this->timer = new Timer(eventQueue, 100, true, this);
    timer->Start();
    startTimeMS = 0;
    
  }

  StopWatch::~StopWatch() {
    timer->Stop();
    delete timer;
    timer = NULL;
  }

  unsigned long StopWatch::GetCurTime() {
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    return tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
  }
	
  void StopWatch::Paint(Graphics* g) {
// TODO CS349
  }
	
  bool StopWatch::HandleMouseEvent(const MouseEvent & e) {
    // TODO: Remove the following code and add your own event handling
    // code to handle mouse events
    LOG_DEBUG << "Received HandleMouseEvent in StopWatch" << e;

// TODO CS349
    return true;
  }

  void StopWatch::HandleTimerEvent(const TimerEvent & e) {
    if (running) {
      Component::Repaint();
    }
  }

  void StopWatch::Start() {
    this->running = true;
    startTimeMS = GetCurTime();
  }

  void StopWatch::Stop() {
    this->running = false;
  }

  bool StopWatch::IsRunning() const {
    return running;
  }
}
