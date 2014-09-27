#include "EventQueue.h"
#include <queue>
#include "Logging.h"
#include <algorithm>

using namespace std;

namespace cs349 {

  EventQueue::~EventQueue() {
    // Clear up any memory laying around  
    this->ClearEventQueue();
  }

  void EventQueue::AddTimer(Timer* timer) {
    timers.push_back(timer);
  }

  void EventQueue::RemoveTimer(Timer* timer) {
    // Remove a timer from our queue. And yes, it *is* ridiculous that
    // it takes this much to remove items from vectors in C++
    vector<Timer*>::iterator where = remove(this->timers.begin(), this->timers.end(), timer);
    this->timers.erase(where, this->timers.end());
  }
  
  void EventQueue::AddEventToQueue(Event* e) {
    this->eventQueue.push(e);
  }

  void EventQueue::ClearEventQueue() {
    while (this->eventQueue.size() > 0) {
      Event* e = eventQueue.front();
      eventQueue.pop();
      delete(e);
    }
  }

void EventQueue::ProcessNextEvent() {
        /*
         * This is one of the more important parts of an interactive
         * application. In this method, you need to:
         
         * - Service the timers to them a chance to operate.
         *   (already coded this for you, below)
         * - You should then take/remove an event off of the event queue and
         *   dispatch it using the DispatchEvent()
         * - Once done, the Event's memory should be deallocated
         */
        
        /*
         * Give timers a chance to operate.
         * Important: Since timers can remove themselves from the
         * EventQueue, we need to create a separate list when iterating
         * through the timers, since the contents of the list may change
         * as a result of a timer removing itself from the list.
         */
        vector<Timer*> localList = this->timers;
        for (vector<Timer*>::iterator iter=localList.begin(); iter != localList.end(); iter++) {
            Timer* timer = (*iter);
            timer->ServiceTimer();
        }
        
        //LOG_TODO << "TODO CS349: Implement EventQueue logic in ProcessNextEvent()";
        // As described above: take event from head of queue, dispatch it,
        // then free its memory)
        // TODO CS349
        if(eventQueue.size()>0){
            Event* aE;
            aE=eventQueue.front();
            aE->DispatchEvent();
            eventQueue.pop();
            delete aE;
           // cout<< eventQueue.size()<<endl;
            //cout<<eventQueue.front()->ToString()<<endl;
    
            //cout<<"dipatched, now evetQuene has"<<eventQueue.size()<<endl;
            
    }
        
}

  int EventQueue::GetNumEventsInQueue() const {
    return this->eventQueue.size();
  }

  int EventQueue::GetNumTimers() const {
    return this->timers.size();
  }

  queue<Event*> EventQueue::GetEvents() const {
    return eventQueue;
  }
}
