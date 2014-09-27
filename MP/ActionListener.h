#ifndef _ACTIONLISTENER_H_
#define _ACTIONLISTENER_H_

#include "Component.h"

namespace cs349 {

  /**
   * Listener interface to be notified when the user performs a
   * specific action, like pressing a button.
   */
  class ActionListener {
  public:
    /**
     * This method is called when an action is performed.
     * @param source The Component that is the source of this event
     */
    virtual void ActionPerformed(Component* source) = 0;
  };
}

#endif /* _ACTIONLISTENER_H_ */
