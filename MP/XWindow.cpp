// Author: Jerry

#include "XWindow.h"

#include <algorithm>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include "XApplication.h"
#include "Logging.h"

namespace cs349
{

  map<long, XWindow*> XWindow::intWindowMap;
  map<Window, XWindow*> XWindow::windowMappings;
  
  XWindow* XWindow::GetXWindowForWindow(Window window)
  {
    return windowMappings[window];
  }

  XWindow::XWindow(const string & title, const Rectangle & bounds) : Component(title)
  {
    this->mouseFocus = NULL;

    this->display = XApplication::GetInstance()->GetXDisplay();

    // Create the window
    this->window = XCreateSimpleWindow(	this->display,
                                        DefaultRootWindow(this->display),
                                        bounds.x,
                                        bounds.y,
                                        bounds.width,
                                        bounds.height,
                                        0, // Window border width. Doesn't include width by window manager
                                        XApplication::GetInstance()->GetWhiteColor(),
                                        XApplication::GetInstance()->GetBlackColor());

    // Add ourselves to the mappings
    windowMappings[this->window] = this;
    windowNum = this->GetComponentNum();
    intWindowMap[windowNum] = this;

    XTextProperty xtext;
    xtext.value = (unsigned char*)title.c_str();
    xtext.encoding = XA_STRING;
    xtext.format = 8; // bits per character
    xtext.nitems = strlen(title.c_str()); // string length
    XSetWMName(this->display, this->window, &xtext);
							
    // Select input events
    XSelectInput(	this->display,
                        this->window,
                        ButtonPressMask
                        | ButtonReleaseMask
                        | KeyPressMask
                        | KeyReleaseMask
                        | ExposureMask
     //                   | PointerMotionMask
                        | ButtonMotionMask
                        | StructureNotifyMask
                        | Button1MotionMask);
					
    this->graphics = new Graphics(this->display, this->window);
	
    // Now set our bounds to that which we were set
    XWindowAttributes attr;
    XGetWindowAttributes(this->display, this->window, &attr);
    Component::SetBounds(Rectangle(attr.x, attr.y, attr.width, attr.height));
  }

  XWindow::~XWindow()
  {
    DestroyWindow();
  }

  void XWindow::DestroyWindow() {
    if (this->display) {
      // Remove ourself from the mappings
      windowMappings.erase(windowMappings.find(this->window));
      intWindowMap.erase(intWindowMap.find(windowNum));

      delete this->graphics;
      this->graphics = NULL;

      XDestroyWindow(this->display, this->window);
      this->display = NULL;
      this->window = 0;
    } else {
      LOG_WARN << "Warning: Attempting to destroy window that has already been destroyed";
    }
  }

  Graphics* XWindow::GetGraphics()
  {
    if (this->display) {
      return this->graphics;
    }
    return NULL;
  }

  XWindow* XWindow::GetParentWindow()
  {
    return this;
  }

  void XWindow::HandlePaintEvent(const PaintEvent & e)
  {
    // Set up Graphics
    this->graphics->SetTransform(AffineTransform::MakeIdentity());
    this->graphics->SetClipRect(e.GetDamagedArea());
    this->Paint(this->graphics); // Calls Component's implementation
  }

  void XWindow::PaintComponent(Graphics* g) {
    ; // no-op -- all painting handled by children
  }

  void XWindow::SetTitle(const string & title)
  {
    if (this->display) {
      XSizeHints hints;
	
      // Set its basic properties
      XSetStandardProperties(	this->display,
                                this->window,
                                title.c_str(),
                                title.c_str(),
                                None, // Pixmap for icon
                                NULL,
                                0,
                                &hints);
    }
  }

  // Overrides
  Rectangle XWindow::GetBounds() const {
    if (this->display) {
      Window root;
      int x, y;
      unsigned int width, height, border_width, depth;
      XGetGeometry(display, window, &root, &x, &y, &width, &height, &border_width, &depth);
      // TODO (Mike): This sometimes crashes nondeterministically, so
      // commented out for now
      // Window parent, *children;
      // unsigned int dummy_w, dummy_h, n_children;
      // children = NULL;
      // if (XQueryTree(display, window, &root, &parent, &children, &n_children) == 0) {
      //   LOG_ERROR << "Error: XQueryTree failed";
      // } else {
      //   XGetGeometry(display, parent, &root, &x, &y, &dummy_w, &dummy_h, &border_width, &depth);
      // }
      // if (children) {
      //   XFree(children);
      // }
      return Rectangle(x, y, width, height);
    }
    return Rectangle();
  }

  void XWindow::Repaint(const Rectangle & r) {
    if (this->display) {
      XApplication::GetInstance()->GetEventQueue()->AddEventToQueue(new PaintEvent(this, r));
    }
  }
  void XWindow::SetBounds(const Rectangle & r)
  {
    if (this->display) {
      int x = (int)r.x;
      int y = (int)r.y;
      unsigned int width = (unsigned int)max(1.0, r.width);
      unsigned int height = (unsigned int)max(1.0, r.height);
      XMoveResizeWindow(display, window, x, y, width, height);
      Component::SetBounds(r);
    }
  }
  void XWindow::SetLocation(const Point & p)
  {
    if (this->display) {
      this->SetBounds(Rectangle(p.x, p.y, this->GetBounds().width, this->GetBounds().height));
    }
  }
  void XWindow::SetParent(Component* parent)
  {
    LOG_ERROR << "Error: Cannot set parent of an XWindow";
  }
  void XWindow::SetSize(int width, int height)
  {
    if (this->display) {
      XResizeWindow(display, window, width, height);
      Component::SetSize(width, height);
    }
  }
  void XWindow::SetVisible(bool visible)
  {
    if (this->display) {
      Component::SetVisible(visible);
      if (visible) {
        XMapRaised(display, window);
      } else {
        XUnmapWindow(display, window);
      }
    }
  }
  
  Component* XWindow::GetMouseFocus() {
    return this->mouseFocus;
  }

  void XWindow::ClearMouseFocus() {
    this->mouseFocus = NULL;
  }

  void XWindow::SetMouseFocus(Component* component) {
    this->mouseFocus = component;
  }

  bool XWindow::HandleMouseEvent(const MouseEvent & e) {
    // TODO CS349: You must properly handle mouse events here, taking
    // into account things like mouse focus (i.e., whether one
    // Component currently has focus or not). If a Component currently
    // has mouse focus, that means it should have the first
    // opportunity to handle an event.

// TODO CS349
    // Default is to call Component's implementation of HandleMouseEvent
    return Component::HandleMouseEvent(e);
  }
  
  long XWindow::GetWindowNum() const {
    return this->windowNum;
  }

  vector<XWindow*> XWindow::GetWindowList() {
    vector<XWindow*> windowList;
    for (map<long, XWindow*>::const_iterator i=intWindowMap.begin(); i != intWindowMap.end(); i++) {
      windowList.push_back(i->second);
    }
    return windowList;
  }

  XWindow* XWindow::GetXWindow(long windowNum) {
    if (intWindowMap.find(windowNum) == intWindowMap.end()) {
      return NULL;
    }
    return intWindowMap[windowNum];
  }
}
