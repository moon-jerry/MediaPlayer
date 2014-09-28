// Author: Jerry
#ifndef _SLIDER_H_
#define _SLIDER_H_

#include "Component.h"
#include "ValueListener.h"
#include <string>
#include <vector>

using namespace std;

namespace cs349 {

  /**
   * The Slider is a widget that allows the user to select a value
   * from a bounded range. This particular Slider provides facilities
   * to paint labels next to the ends of the scale, as well as next to
   * the current value (above the thumb used to change the slider value).
   */
class Slider : public Component {
  public:
  /**
   * Orientation types for Slider components
   */
  enum OrientationType {
    HORIZONTAL_SLIDER,
    VERTICAL_SLIDER
  };
    Point StartPoint;
 private:
  int lowerBound;
  int upperBound;
  int curValue;
  string lowerBoundLabel;
  string upperBoundLabel;
  string thumbLabel;
  OrientationType orientation;
  vector<ValueListener*> listeners;

// TODO CS349

 protected:
  /**
   * The method that does the actual painting of the component once
   * the Graphics context object has been set up by the Paint
   * method in Component.
   */
  virtual void PaintComponent(Graphics* g);

 public:

    /**
     * Creates a new instance of a Slider with the name given. The
     * name argument is used for unit tests when testing the
     * interactor tree. Thus, the name should be unique.
     *
     * @param name A unique name used to identify this Slider. This
     * name should not be printed out when painting
     * @param orientation Whether to make a horizontal or vertical
     * slider
     */
    Slider(const string & name, OrientationType orientation);

    /**
     * Adds a listener to this slider, which will be called when the
     * slider value is changed. Slider is not responsible for the memory
     * management of the ValueListener -- external code is
     * responsible for removing the listener and deleting it when it
     * is no longer needed.
     */
    void AddValueListener(ValueListener* l);

    /**
     * Removes the specified ValueListener from the Slider.
     */
    void RemoveValueListener(ValueListener* l);

    /**
     * Gets the minimum value supported by the slider.
     */
    int GetMinimum() const;

    /**
     * Gets the maximum value supported by the slider.
     */
    int GetMaximum() const;

    /**
     * Gets the current value of the slider.
     */
    int GetCurValue() const;

    /**
     * Sets the minimum value of the slider. If the Slider is visible,
     * it should repaint itself to update the new bounds.
     */
    void SetMinimum(int minValue);

    /**
     * Sets the maximum value of the slider. If the Slider is visible,
     * it should repaint itself to update the new bounds.
     */
    void SetMaximum(int maxValue);

    /**
     * Sets the current value of the slider and calls all
     * ValueListeners associated with the Slider. The Slider should
     * repaint itself after a new value is set.
     */
    void SetCurValue(int value);

    /**
     * Returns the orientation of the slider (whether it is horizontal
     * or vertical).
     */
    OrientationType GetOrientation() const;

    /**
     * Sets the orientation of the slider to be horizontal or
     * vertical.
     */
    void SetOrientation(OrientationType orientation);

    /**
     * Gets the label shown next to the lower bound of the slider
     */
    string GetLowerBoundLabel() const;

    /**
     * Gets the label shown next to the upper bound of the slider
     */
    string GetUpperBoundLabel() const;

    /**
     * Gets the label shown next to the thumb of the slider (where the
     * thumb is the part of the slider the user manipulates)
     */
    string GetThumbLabel() const;

    /**
     * Sets the label shown next to the lower bound of the slider
     */
    void SetLowerBoundLabel(const string & label);

    /**
     * Sets the label shown next to the upper bound of the slider
     */
    void SetUpperBoundLabel(const string & label);

    /**
     * Sets the label shown next to the thumb of the slider, where the
     * thumb is the part that the user manipulates
     */
    void SetThumbLabel(const string & label);
    
    bool HandleMouseEvent(const MouseEvent & e);

    // TODO CS349: Add any other necessary methods or overrides here
// TODO CS349
  };
}


#endif /* _SLIDER_H_ */
