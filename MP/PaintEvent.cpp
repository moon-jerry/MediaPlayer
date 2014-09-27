#include "PaintEvent.h"
#include "XWindow.h"
#include "Logging.h"

namespace cs349
{

  PaintEvent::PaintEvent(XWindow* window, const Rectangle & damagedArea) : ComponentEvent(window)
  {
    this->damagedArea = damagedArea;
  }

  PaintEvent::~PaintEvent()
  {
    ; // no-op
  }

  void PaintEvent::DispatchEvent()
  {
    // See MouseEvent's equivalent for a template on how to do this
   this->GetWindow()->HandlePaintEvent(*this);
// TODO CS349
  }

  Rectangle PaintEvent::GetDamagedArea() const
  {
    return this->damagedArea;
  }

  string PaintEvent::ToString() const {
    stringstream ss;
    ss << (*this);
    return ss.str();
  }

  ostream & operator << (ostream &os, const PaintEvent & e) {
    os << "PaintEvent: " << e.GetDamagedArea();
    return os;
  }

}
