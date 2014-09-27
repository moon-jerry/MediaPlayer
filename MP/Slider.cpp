#include "Slider.h"
#include "XWindow.h"
#include "Logging.h"
#include "sstream"
#include <algorithm>
#include "A2WidgetIdentifiers.h"

using namespace cs349;
using namespace std;

Slider::Slider(const string & name, OrientationType orientation)
  : Component(name)
{
  lowerBound = 0;
  upperBound = 0;
  curValue = 0;
  lowerBoundLabel = "";
  upperBoundLabel = "";
  thumbLabel = "";
  this->orientation = orientation;

// TODO CS349
}

void Slider::AddValueListener(ValueListener* l) {
    listeners.push_back(l);
}

void Slider::RemoveValueListener(ValueListener* l) {
     remove(listeners.begin(), listeners.end(), l);
}

int Slider::GetMinimum() const {
  return lowerBound;
}

int Slider::GetMaximum() const {
  return upperBound;
}

int Slider::GetCurValue() const {
  return curValue;
}

void Slider::SetMinimum(int minValue) {
// TODO CS349
    lowerBound=minValue;
}

void Slider::SetMaximum(int maxValue) {
// TODO CS349
    upperBound=maxValue;
}

void Slider::SetCurValue(int value) {
    if(value>upperBound)
        curValue=upperBound;
    else if(value<lowerBound)
        curValue=lowerBound;
    else
        curValue=value;
}

Slider::OrientationType Slider::GetOrientation() const {
  return orientation;
}

void Slider::SetOrientation(OrientationType orientation) {
  this->orientation = orientation;
  this->Repaint();
}

void Slider::PaintComponent(Graphics* g) {
// TODO CS349
    stringstream cv;
    
    long fC=g->GetForegroundColor();
    g->SetForegroundColor(g->GetBackgroundColor());
    g->DrawRect(Rectangle(0, 0, 350, 40));
    g->FillRect(Rectangle(0, 0, 350, 40));
    g->SetForegroundColor(fC);// refresh
    
    g->DrawRect(Rectangle(0, 15, 295, 2));
    g->FillRect(Rectangle(0, 15, 295, 2));
    int temp=(curValue*286)/(upperBound+1);
    double loction=temp;
    g->DrawRect(Rectangle(loction, 1, 10, 25));
    temp=(temp*200)/285;
   
    if(this->GetName()== CURRENT_TURTLE_STEP_SLIDER_ID)
    {
         cv<<temp;
        g->DrawText(Point(loction, 40), cv.str());
        
    }
    else
    {
        int temp2;
        //cout<<curValue<<endl;
        if(curValue<50)
        {
            temp2=abs(9-(((50-curValue)*10)/50));
            cv<<temp2;
            g->DrawText(Point(loction, 40), cv.str()+"/10x");
        }
        else if(curValue>50)
        {
            temp2=abs(10-(((100-curValue)*10)/50));
            cv<<temp2;
            g->DrawText(Point(loction, 40), cv.str()+"x");
        }
        else
            g->DrawText(Point(loction, 40), cv.str()+"1x");


    }
    
   // g->DrawRect(Rectangle(curValue+2, 2, 6, 24));
   // g->FillRect(Rectangle(curValue+2, 2, 6, 24));
    //g->FillRect(Rectangle(curValue, 1, 10, 28));
    
    
}
bool Slider::HandleMouseEvent(const MouseEvent & e)
{
    if(!IsVisible())
        return false;   
    this->SetCurValue((e.GetWhere().x*upperBound)/285);
    for (vector<ValueListener*>::iterator i = listeners.begin(); i != listeners.end(); i++) {
        (*i)->ValueChanged(this);
    }
    return true;
    
}

string Slider::GetLowerBoundLabel() const {
  return lowerBoundLabel;
}

string Slider::GetUpperBoundLabel() const {
  return upperBoundLabel;
}

string Slider::GetThumbLabel() const {
  return thumbLabel;
}

void Slider::SetLowerBoundLabel(const string & label) {
  this->lowerBoundLabel = label;
  this->Repaint();
}

void Slider::SetUpperBoundLabel(const string & label) {
  this->upperBoundLabel = label;
  this->Repaint();
}

void Slider::SetThumbLabel(const string & label) {
  this->thumbLabel = label;
  this->Repaint();
}

// TODO CS349: Implement any other methods needed here
// TODO CS349
