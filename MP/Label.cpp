// Author: Michael Terry
#include "Label.h"
#include "XWindow.h"
#include "Logging.h"

#include <algorithm>

using namespace cs349;

Label:: Label(const string & name, const Point aP)
: Component(name)
{
    // Perform any initialization needed here
   this->label = name;
    this->location = aP;
    this->needFrame=false;
 
    // TODO CS349
}

string Label::GetLabel() const {
    return label;
}

void Label::SetLabel(const Point aP) {
    this->location = aP;
}

void Label::PaintComponent(Graphics* g) {
    g->DrawText(location, label);
    //g->DrawLine(Point(0, 0), Point(0, 3000));
    //g->DrawLine(Point(0, 0), Point(3000, 0));
    if (needFrame)
        g->DrawRect(aR);
}
void Label::setFrame(bool a, Rectangle aRec)
{
    if(a)
        needFrame=true;
    aR=aRec;
}

// TODO CS349: Implement any other methods needed here
// TODO CS349
