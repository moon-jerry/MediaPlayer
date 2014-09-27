// Author: Michael Terry

#ifndef __X_APPLICATION__
#define __X_APPLICATION__

#include <X11/Xlib.h>
#include <vector>
#include "EventQueue.h"

using namespace std;

namespace cs349
{

  /**
   * XApplication is the back-bone of your application. XApplication
   * does the following:
   * - Initializes and maintains the connection with X, and provides
   *   access to important X variables (such as the values for black
   *   and white colors).
   * - Implements the event loop via its Run() method. In the event
   *   loop, new events are received from X, packaged up as our own
   *   custom Event objects, and added to the EventQueue object.
   *   Events are then peeled off the event queue in FIFO order, and
   *   asked to deliver themselves.
   * 
   * XApplication is a *singleton*: There is only one instance of it in
   * existence throughout the lifetime of the application. As such,
   * you don't directly instantiate the XApplication object. Instead,
   * you call XApplication::GetInstance().
   * 
   */
  class XApplication
  {
  private:
    static XApplication* s_Instance;

    Display*    display;      ///< A pointer to the display
    int         screen;       ///< Represents the default screen to display on
    bool        quit;         ///< Whether the app should continue to run, or quit
    EventQueue* eventQueue;   ///< The event queue used to manage events
  
    /**
     * Private constructor so only one instance can be created (it's a
     * singleton). Use GetInstance() to construct/get a pointer to the
     * actual object.
     */
    XApplication();
    friend int ErrorHandler(Display* display);
    int        HandleXError(Display* display);

    /**
     * This method checks whether any X events are pending. If so, it
     * packages them up as our own events and adds them to the eventQueue.
     */
    void       CheckForXEvents();
  
  public:
    /**
     * Gets a pointer to the XApplication object (constructing it, if
     * necessary).
     */
    static XApplication* GetInstance();

    /**
     * When this destructor is called, the singleton (global) instance
     * is set to NULL, allowing GetInstance() to be called again (and
     * thus creating a new global XApplication object).
     */
    ~XApplication();
    
    /**
     * Retrieves the X Display for this X server.
     */
    Display* GetXDisplay() const;

    /**
     * Retrieves the default screen for this X server.
     */
    int GetDefaultScreen() const;

    /**
     * Gets the value representing black from this X server.
     */
    unsigned long GetBlackColor() const;

    /**
     * Gets the value representing white from this X server.
     */
    unsigned long GetWhiteColor() const;
  
    /**
     * Returns a pointer to this app's EventQueue object, to be able
     * to add Events to it.
     */
    EventQueue* GetEventQueue();

    /**
     * Replaces the current event queue with the given event queue.
     * When setting the event queue, XApplication takes over
     * management of the memory represented by the passed in pointer.
     * It will also delete the existing event queue, making any
     * existing pointers to it invalid. Any events on the existing
     * event queue will be thrown away when it is deleted.
     *
     * This functionality is provided to make it easier to test event
     * queue event handling in unit tests. *You do not need to replace
     * the event queue for normal operation of the toolkit.*
     *
     * @param eventQueue A pointer to a heap-allocated EventQueue (it
     * is a fatal error to pass in a NULL pointer)
     */
    void SetEventQueue(EventQueue* eventQueue);

    /**
     * Starts the application event loop. This will run until "Quit"
     * is called, the user closes the window, or will only run once if
     * the runOnce flag is set to true. For normal operation, runOnce
     * should be set to false, indicating the application should run
     * until the user signals they wish to quit. However, for unit
     * testing, runOnce can be set to true, allowing unit tests to
     * test single runs through the event loop.
     *
     * If Quit is called, and then Run is called again, Run will reset
     * the quit flag to run again until Quit is called.
     *
     * @param runOnce A flag to indicate whether the event loop should
     * run continuously just once. If runOnce=false, the event loop
     * will run until a) Quit is called or b) the user closes the
     * window. If runOnce=true, it will run just a single pass through
     * the event loop
     */
    void Run(bool runOnce=false);
  
    /**
     * Stops the event loop.
     */
    void Quit();
  };

}

#endif /* __APPLICATION__ */
