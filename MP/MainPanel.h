#ifndef _MAINPANEL_H_
#define _MAINPANEL_H_

#include "Panel.h"
#include "ValueListener.h"
#include "XApplication.h"
#include "TimerListener.h"
#include "Slider.h"
#include "Component.h"
#include "ActionListener.h"
#include "TurtleGraphics.h"

using namespace cs349;

namespace cs349 {
  /**
   * The MainPanel is simply the Component which will contain all
   * other elements of your interface. In our unit tests, we will
   * instantiate an instance of this object, then programmatically
   * test your individual components.
   *
   * We should be able to create an instance of this object, place it
   * in an arbitrary window, and see the full interface specified for
   * the assignment contained within this panel.
   *
   * In the constructor, you should set the size of this panel to the
   * desired size.
   */
  class MainPanel : public Panel , ValueListener, TimerListener, ActionListener {
 
  private:
      int CurValue;
      Slider* playback;
      Slider* currentRate;
      TurtleGraphics* turtle;
      Component* playbackPanel;
      Component* CurrentTurtleStepPanel;
      Timer* atimer;
      
      
      
  public:
    /**
     * Constructs the main panel for the application. All components
     * should be added to this panel.
     *
     * You should set the size of this panel in the constructor so
     * that information can be used to size the XWindow to the
     * appropriate size.
     */
    MainPanel();
      
      void ValueChanged(Component* source);
      void HandleTimerEvent(const TimerEvent & e);
      void ActionPerformed(Component* s);
   
  };

}


#endif /* _MAINPANEL_H_ */
