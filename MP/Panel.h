#ifndef _PANEL_H_
#define _PANEL_H_

#include "Component.h"

using namespace cs349;

namespace cs349 {
  /**
   * A Panel is simply a Component which contains other Components. It
   * has no visual appearance of its own.
   */
  class Panel : public Component {
  protected:
    /**
     * A no-op method
     */
    virtual void PaintComponent(Graphics* g);
  public:
    /**
     * A no-op constructor
     */
    Panel(const string & name);
  };
}

#endif /* _PANEL_H_ */
