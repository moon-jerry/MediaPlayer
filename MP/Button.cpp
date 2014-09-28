// Author: Jerry
#include "Button.h"
#include "XWindow.h"
#include "Logging.h"
#include "MouseEvent.h"

#include <algorithm>

using namespace cs349;

Button::Button(const string & name, const string & label)
  : Component(name)
{
  // Perform any initialization needed here
  this->label = label;

// TODO CS349
}

void Button::AddActionListener(ActionListener* l) {
// TODO
    listeners.push_back(l);
}

void Button::RemoveActionListener(ActionListener* l) {
// TODO CS349 test
    remove(listeners.begin(), listeners.end(), l);
}

string Button::GetLabel() const {
  return label;
}

void Button::SetLabel(const string & label) {
  this->label = label;
}


void Button::drawModel(Graphics* g,vector<Point> atemp)
{

    g->DrawRect(Rectangle(0, this->GetBounds().y, 40, 30));
    g->DrawPolygon(atemp);
    

}


bool Button::HandleMouseEvent(const MouseEvent & e)
{
    if(!this->IsVisible())
    {
        return false;
    }
    
    if (e.GetType()==MouseEvent::mouseUp&& this->GetBounds().x<e.GetWhere().x  &&  e.GetWhere().x<this->GetBounds().GetBottomRight().x && this->flag){
        for (vector<ActionListener*>::iterator i = listeners.begin(); i != listeners.end(); i++) {
            (*i)->ActionPerformed(this);
        }
     //cout<<this->GetBounds().x<<endl;
        flag=false;
        return true;
    }
    if (e.GetType()==MouseEvent::mouseDown&& this->GetBounds().x<e.GetWhere().x  &&  e.GetWhere().x<this->GetBounds().GetBottomRight().x){
            //cout<<this->GetBounds().x<<endl;
        flag=true;
        return true;
    }
    
    return false;
}



vector<Point> refill(vector<Point> aV, Point p1, Point p2, Point p3)
{
    aV.clear();
    aV.push_back(p1);
    aV.push_back(p2);
    aV.push_back(p3);
    return aV;
}

void Button::PaintComponent(Graphics* g) {
// TODO CS349
    vector<Point> temp;
   if(label=="go to start button")
   {
    temp = refill(temp,Point(7, 15),Point(32, 5),Point(32, 25));
    drawModel(g,temp);
    g->DrawLine(Point(7, 5), Point(7,25));
    g->FillPolygon(temp);
   }
   
    if(label=="previous frame button")
    {
        temp = refill(temp,Point(7, 15),Point(32, 5),Point(32, 25));
        drawModel(g,temp);
        g->FillPolygon(temp);

    }
    
    if(label=="play button")
    {
        temp = refill(temp,Point(32, 15),Point(7, 5),Point(7, 25));
        drawModel(g,temp);
        g->DrawLine(Point(7, 25), Point(32, 15));
    
    }
    
    if(label=="next frame button")
    {
        temp = refill(temp,Point(32, 15),Point(7, 5),Point(7, 25));
        drawModel(g,temp);
         g->FillPolygon(temp);
    }
    if(label=="go to end button")
    {
        temp = refill(temp,Point(32, 15),Point(7, 5),Point(7, 25));
        drawModel(g,temp);
        g->DrawLine(Point(32, 5), Point(32,25));
        g->FillPolygon(temp);
    }
    
    
    
}
// TODO CS349: Implement any other methods needed here
// TODO CS349
