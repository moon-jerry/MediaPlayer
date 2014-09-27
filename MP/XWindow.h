// Author: Michael Terry

#ifndef __XWINDOW__
#define __XWINDOW__

#include <X11/Xlib.h>
#include <map>

using namespace std;

// Forward declarations
namespace cs349
{
  class XWindow;
}

// Project includes
#include "Component.h"
#include "Graphics.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "PaintEvent.h"
#include "Rectangle.h"

void xwindow_load_first();

namespace cs349
{

  /**
   * The XWindow class encapsulates an X lib Window object, which
   * represents the actual window users see and interact with.
   * 
   * It is derived from Component, and thus shares much of its
   * functionality, though it must override some of the functionality
   * to make it appropriate to a Window (such as setting the location
   * and bounds).
   * 
   * This class also maintains a mapping of X lib window pointers to
   * our XWindow class pointers. Thus, when X lib creates a new event
   * and specifies a window, you can retrieve the XWindow object
   * corresponding to that window.
   * 
   * Each XWindow object maintains its own Graphics context, which it
   * passes on to its child components to enable drawing into the
   * window.
   * 
   * PaintEvents are passed to the XWindow. Paint events should be
   * analyzed and passed on to children to have them paint themselves.
   * However, only children intersecting the damaged area specified by
   * the PaintEvent should be drawn.
   *
   * *Note that you will need to complete the toolkit before an
   *  XWindow properly maps itself to the screen.*
   *
   * Also note that any Component (including an XWindow) is initially
   * *not visible*.
   *
   * In its constuctor, XWindow sets what X events it wishes to
   * receive. See its constructor for more details.
   */
  class XWindow : public Component
  {
  private:
    Display*  display;
    Window    window;
    Graphics* graphics;
    long      windowNum;        ///< Used for scripting
 
    static map<long, XWindow*>   intWindowMap; ///< Used for scripting interface
    static map<Window, XWindow*> windowMappings;
 
    Component* mouseFocus; ///< The Component that currently has mouse focus
  public:
    /**
     * Creates a new window with the given title and bounds, and
     * selects a subset of X events to receive. See implementation of
     * constructor for more details.
     */
    XWindow(const string & title, const Rectangle & bounds);
    virtual ~XWindow();
 
    /**
     * Returns a pointer to the XWindow object corresponding to the X
     * lib window pointer
     */
    static XWindow* GetXWindowForWindow(Window window);

    /**
     * Closes (destroys) the window so it is no longer visible or usable.
     */
    virtual void DestroyWindow();

    /**
     * Returns the graphics context associated with this window
     */
    virtual Graphics* GetGraphics();
 

    /**
     * Called when there are damaged areas that need to be repainted
     * in the window.
     */
    virtual void HandlePaintEvent(const PaintEvent & e);


    /**
     * Set title of the window
     */
    virtual void SetTitle(const string & title);

    /**
     * Returns the bounds of this window. *Note*: In theory, the
     * *location* of the window should be relative to the screen.
     * However, the location returned should, at this time, be
     * considered *non-deterministic*. You shouldn't need to know (or
     * worry about) the actual location of the window on the screen
     * for any of your code.
     */
    virtual Rectangle GetBounds() const;

    /**
     * Returns a pointer to this window (since an XWindow has no parent).
     */
    virtual XWindow* GetParentWindow();

    /**
     * Does nothing (a no-op). Child Components should instead paint themselves.
     */
    virtual void PaintComponent(Graphics* g);

    /**
     * Causes a PaintEvent to be generated and placed on the
     * XApplication's EventQueue.
     */
    virtual void Repaint(const Rectangle & r);

    /**
     * Sets the bounds of this XWindow, where the location specified
     * is relative to the screen.
     */
    virtual void SetBounds(const Rectangle & r);

    /**
     * Sets the location of this XWindow, where the location specified
     * is relative to the screen.
     */
    virtual void SetLocation(const Point & p);

    /**
       A no-op: It is an error to set an XWindow's parent.
     */
    virtual void SetParent(Component* parent);

    /**
     * Sets the size of this XWindow.
     */
    virtual void SetSize(int width, int height);

    /**
     * Maps (shows/sets visible) or unmaps (hides) the XWindow from
     * the screen.
     */
    virtual void SetVisible(bool visible);

    /**
     * Returns the Component with mouse focus, NULL if no Component
     * currently has mouse focus.
     */
    virtual Component* GetMouseFocus();

    /**
     * Clears mouse focus, so no Component gets MouseEvents first.
     */
    virtual void ClearMouseFocus();

    /**
     * Sets the given Component to have mouse focus (the Component
     * that should have the first opportunity to act on a MouseEvent).
     *
     * @param component The Component to receive mouse focus
     */
    virtual void SetMouseFocus(Component* component);

    /**
     * Called when a MouseEvent has been generated for this XWindow.
     * This overrides the default Component implementation to be able
     * to handle mouse focus issues. Remember that Components must
     * receive events that have been transformed to their coordinate
     * system, including when they have focus...
     *
     * @return true if the event has been handled by this XWindow or one of
     * its children. 
     */
    virtual bool HandleMouseEvent(const MouseEvent & e);

    /**
     * This mapping is used for scripting interfaces. It allows
     * scripts to refer to windows via integers, rather than pointers.
     */
    long GetWindowNum() const;

    /**
     * Gets a list of all XWindow objects that currently exist. Used
     * for scripting interface.
     */
    static vector<XWindow*> GetWindowList();

    /**
     * Retrieves a pointer to the XWindow corresponding to the given
     * windowNum, or NULL if no such window exists.
     */
    static XWindow* GetXWindow(long windowNum);
  };

}

#endif /* __XWINDOW__ */
