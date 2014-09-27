// Author: Michael Terry

#include "Component.h"
#include "XApplication.h"
#include "XWindow.h"
#include "Logging.h"

namespace cs349
{

  long Component::componentCounter = 1;
  map<long, Component*> Component::intComponentMap;

  Component::Component(const string & name)
  {
    this->name            = name;
    this->parent          = NULL;
    this->visible         = false;
    this->backgroundColor = XApplication::GetInstance()->GetBlackColor();
    this->foregroundColor = XApplication::GetInstance()->GetWhiteColor();
    this->debugDelayMS    = 0;
    componentNum          = componentCounter++;
    intComponentMap[componentNum] = this;
  }

  Component::~Component()
  {
    // Clear up any memory laying around
    for (vector<Component*>::iterator iter=this->children.begin(); iter != this->children.end(); iter++) {
      delete(*iter);
    }
    this->children.clear();
    // Remove ourself from component map
    intComponentMap.erase(intComponentMap.find(componentNum));
  }

  string Component::GetName() const {
    return this->name;
  }

  void Component::AddComponent(Component* child)
  {
    this->children.push_back(child);
    child->SetParent(this);
  }

  void Component::RemoveComponent(Component* child)
  {
    for (vector<Component*>::iterator i = this->children.begin(); i != this->children.end(); i++) {
      if (*i == child) {
        child->SetParent(NULL);
        this->children.erase(i);
        break;
      }
    }
  }

  unsigned long Component::GetBackgroundColor() const
  {
    return this->backgroundColor;
  }

  Rectangle Component::GetBounds() const
  {
    return this->bounds;
  }

  Component* Component::GetComponent(long componentNum) {
    if (intComponentMap.find(componentNum) == intComponentMap.end()) {
      return NULL;
    }
    return intComponentMap[componentNum];
  }

  long Component::GetComponentNum() const {
    return this->componentNum;
  }

  vector<Component*> Component::GetDepthFirstInteractorTree() const {
    vector<Component*> tree;
    tree.push_back(const_cast<Component*>(this));
    for (vector<Component*>::const_iterator child=children.begin(); child != children.end(); child++) {
      vector<Component*> childTree = (*child)->GetDepthFirstInteractorTree();
      tree.insert(tree.end(), childTree.begin(), childTree.end());
    }
    return tree;
  }

  vector<Component*> Component::GetBreadthFirstInteractorTree() const {
    vector<Component*> tree;
    if (const_cast<Component*>(this)->GetParent() == NULL) {
      tree.push_back(const_cast<Component*>(this));
    }
    tree.insert(tree.end(), this->children.begin(), this->children.end());
    for (vector<Component*>::const_iterator child=this->children.begin(); child != this->children.end(); child++) {
      vector<Component*> childTree = (*child)->GetBreadthFirstInteractorTree();
      tree.insert(tree.end(), childTree.begin(), childTree.end());
    }
    return tree;
  }

  unsigned long Component::GetForegroundColor() const
  {
    return this->foregroundColor;
  }

  Component* Component::GetParent()
  {
    return this->parent;
  }

  XWindow* Component::GetParentWindow()
  {
    if (this->parent) {
      return this->parent->GetParentWindow();
    }
    return NULL;
  }

  bool Component::HandleKeyEvent(const KeyEvent & e)
  {
    /*
     * Use this as a basic template for how to handle mouse events.
     * Here, we simply pass key events on to child components to see
     * if they can handle the event. As soon as one component handles
     * the event, the method returns.
     */
    for (vector<Component*>::iterator iter=this->children.begin(); iter != this->children.end(); iter++) {
      Component* c = (*iter);
      if (c->HandleKeyEvent(e)) {
        return true;
      }
    }
    return false;
  }

  bool Component::HandleMouseEvent(const MouseEvent & e)
  {
    /*
     * In this default implementation, you should iterate through the
     * children to see if they can handle the event. As soon as one
     * child handles the event (by returning true), the method should
     * return true. If no child can handle the event, it should return
     * false.
     * 
     * Some caveats and things to keep in mind:
     * - You shouldn't be able to interact with things you can't see
     * - Points are expected to be in the coordinate system of the
     *   Component (see notes in the header file)
      */
       MouseEvent aNew=MouseEvent(e.GetWindow(), e.GetType(), e.GetWhere());
      if (this->IsPointInComponent(Point(e.GetWhere()))&& this->IsVisible()){
          if(this->parent==NULL)
          {
             aNew=MouseEvent(e.GetWindow(), e.GetType(), e.GetWhere());
          }
          else
          {
             aNew=MouseEvent(e.GetWindow(), e.GetType(), Point(e.GetWhere().x-this->GetBounds().x,e.GetWhere().y-this->GetBounds().y));
          }
          //cout<<this->GetBounds().x<<" x "<<this->GetBounds().y<<" y"<<endl;
          
      for (vector<Component*>::iterator iter=this->children.begin(); iter != this->children.end(); iter++)
      {
          Component* c = (*iter);
         /* cout<<c->GetName()<<endl;
          cout<<e.ToString()<<endl;
          cout<<aNew.ToString()<<"new mouse event"<<endl;*/
          if(e.GetType()==MouseEvent::mouseDown)
          {
              for (vector<Component*>::iterator iter=this->children.begin(); iter != this->children.end(); iter++)
              {
                  if(c->flag==true)
                      return true;
              }
          }
          
         if( c->HandleMouseEvent(aNew))
             return true;
          if(e.GetType()==MouseEvent::mouseUp)
          {
              for (vector<Component*>::iterator iter=this->children.begin(); iter != this->children.end(); iter++)
              {
                  c->flag=false;
              }
          }
      }
          return true;
    }
    
    return false;
     
}


  bool Component::IsPointInComponent(const Point & p) const
  {
    return this->GetBounds().IsPointInRectangle(p);
  }

  bool Component::IsVisible() const
  {
    return visible;
  }

  void Component::Paint(Graphics* g)
  {
    /*
     * This is one of the primary methods of the Component class.
     * In this method, you should:
     * - Think about things like visibility of the Component (is it
     *   visible?)
     * - Save any state of the Graphics context that you'll modify
     * - Prepare for drawing (think about things like
     *   background/foreground colors, clip, and clearing the background)
     * - Call PaintComponent
     * - Make sure children paint themselves
     * - Restore anything you changed in the Graphics context
     */
      
      long orB = g->GetBackgroundColor();
      long orF = g->GetForegroundColor();
      AffineTransform oriA=g->GetTransform();
      long WC= XApplication::GetInstance()->GetWhiteColor();
      long BC= XApplication::GetInstance()->GetBlackColor();
      /* cout<<"im here"<<endl;
      cout<<this->GetForegroundColor()<<endl;
       cout<<this->GetBackgroundColor()<<endl;
       */

        
    if (this->GetBackgroundColor() == this->GetForegroundColor()) {
            LOG_PAINT << "Warning: Component's background and foreground colors are identical in Component::Paint()";
    }

      g->SetForegroundColor(WC);
      g->SetBackgroundColor(BC);
        PaintComponent(g);
      
      //Rectangle(0, 0, c->GetBounds().GetBottomRight().x-c->GetBounds().GetTopLeft().x-200,  c->GetBounds().GetBottomRight().y-c->GetBounds().GetTopLeft().y-200)


          //cout<<this->children.size()<<endl;
      for (vector<Component*>::iterator iter=this->children.begin(); iter != this->children.end(); iter++) {
          Component* c = (*iter);
          g->SetTransform(AffineTransform::MakeTranslation(c->GetBounds().x, c->GetBounds().y).operator*(oriA));
          g->SetClipRect(Rectangle(0, 0, c->GetBounds().width, c->GetBounds().height));
          c->Paint(g);
          //cout<<"im here2"<<endl;
          //cout<< this->children.front()->GetName()<<endl;
          //cout<< this->GetName()<<endl;
      }
     
      g->SetBackgroundColor(orB);
      g->SetForegroundColor(orF);
      g->SetTransform(oriA);

// TODO CS349
  }

  void Component::Repaint()
  {
    Rectangle localBounds = this->GetBounds();
    localBounds.x = 0;
    localBounds.y = 0;
    this->Repaint(localBounds);
  }

  void Component::Repaint(const Rectangle & r)
  {
    /*
     * If we have a parent Component, tell them to repaint. This will
     * go up the interactor tree until it reaches the root XWindow.
     * Note that XWindow overrides this method to generate the actual
     * PaintEvent event to place on the EventQueue.
     */
    if (this->GetParent() != NULL) {
      Rectangle newRect = r;
      newRect.x += this->GetBounds().x;
      newRect.y += this->GetBounds().y;
      this->GetParent()->Repaint(newRect);
    }
  }

  void Component::SetBackgroundColor(unsigned long c)
  {
    this->backgroundColor = c;
    this->Repaint();
  }
	
  void Component::SetBounds(const Rectangle & r)
  {
    this->bounds = r;
    this->Repaint();
  }

  void Component::SetDebugDelay(unsigned long delayMS) {
    debugDelayMS = delayMS;
  }

  void Component::SetForegroundColor(unsigned long c)
  {
    this->foregroundColor = c;
    this->Repaint();
  }

  void Component::SetLocation(const Point & p)
  {
    this->bounds.x = p.x;
    this->bounds.y = p.y;
    this->Repaint();
  }

  void Component::SetParent(Component* parent)
  {
    this->parent = parent;
    this->Repaint();
  }

  void Component::SetSize(int width, int height)
  {
    this->bounds.width = width;
    this->bounds.height = height;
    this->Repaint();
  }

  void Component::SetVisible(bool visible)
  {
    this->visible = visible;
    this->Repaint();
  }

}
