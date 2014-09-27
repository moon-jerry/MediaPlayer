// Author: Michael Terry
#ifndef _VALUELISTENER_H_
#define _VALUELISTENER_H_

namespace cs349 {
  /**
   * Listener interface for objects who wish to be informed when the
   * value changes for an adjustable widget (like a Slider).
   */
  class ValueListener {
  public:
    /**
     * This method is called when the value of an adjustable widget is
     * changed.
     * @param source The Component that is the source of this event
     */
    virtual void ValueChanged(Component* source) = 0;
  };
}

#endif /* _VALUELISTENER_H_ */
