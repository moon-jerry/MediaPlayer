#include "TurtleGraphics.h"

#include <math.h>
#include <algorithm>
#include "Logging.h"

namespace cs349 {

  const int TurtleGraphics::TURTLE_LENGTH = 10;

  TurtleGraphics::TurtleGraphics(const string & name)
    : SchemeComponent(name)
  {
    this->drawStepNum = 0;
  }

  TurtleGraphics::~TurtleGraphics() {
    DeleteInstructions();
  }

  void TurtleGraphics::DeleteInstructions() {
    for (vector<TurtleGraphicsOp*>::iterator i = instructions.begin(); i != instructions.end(); i++) {
      delete *i;
    }
    instructions.erase(instructions.begin(), instructions.end());
  }

  void TurtleGraphics::ClearInstructions() {
    DeleteInstructions();
    this->Repaint();
  }
  
  vector<const TurtleGraphicsOp*> TurtleGraphics::GetInstructions() const {
    vector<const TurtleGraphicsOp*> return_instructions;
    for (vector<TurtleGraphicsOp*>::const_iterator i = instructions.begin(); i != instructions.end(); i++) {
      return_instructions.push_back(*i);
    }
    return return_instructions;
  }

  long TurtleGraphics::GetNumSteps() const {
    long numSteps = 0;
    for (vector<TurtleGraphicsOp*>::const_iterator i = instructions.begin(); i != instructions.end(); i++) {
      numSteps += (*i)->GetNumSteps();
    }
    return numSteps;
  }
    
  AffineTransform TurtleGraphics::GetAffineTransform(long stepNum) const {
    return this->WalkInstructions(NULL, stepNum, false);
  }

  void TurtleGraphics::Rotate(double theta, bool redrawNow) {
    instructions.push_back(new TurtleGraphicsRotateOp(theta));
    if (redrawNow) {
      //this->GoToStep(this->GetNumSteps());
    }
  }

  void TurtleGraphics::RotateInDegrees(double degrees, bool redrawNow) {
    this->Rotate(degrees / 180.0 * PI, redrawNow);
  }

  void TurtleGraphics::Forward(double amount, bool penDown, bool redrawNow) {
    instructions.push_back(new TurtleGraphicsForwardOp(amount, penDown));
    if (redrawNow) {
     // this->GoToStep(this->GetNumSteps());
    }
  }

  void TurtleGraphics::DrawText(const string & text, bool redrawNow) {
    instructions.push_back(new TurtleGraphicsDrawTextOp(text));
    if (redrawNow) {
      //this->GoToStep(this->GetNumSteps());
    }
  }

  void TurtleGraphics::DrawPoint(bool redrawNow) {
    instructions.push_back(new TurtleGraphicsDrawPointOp());
    if (redrawNow) {
      //this->GoToStep(this->GetNumSteps());
    }
  }

  double TurtleGraphics::GetRotation(const AffineTransform & t) {
    Point origin;
    Point unitX = Point(1, 0);
    Point newOrigin = t * origin;
    Point newUnitX = t * unitX;
    double new_x = newUnitX.x - newOrigin.x;
    double new_y = newUnitX.y - newOrigin.y;
    double vector_cos = new_x / sqrt(new_x * new_x + new_y * new_y);
    double theta = acos(vector_cos);
    if (new_y < 0) {
      theta = 2*PI - theta;
    }
    return theta;
  }

  double TurtleGraphics::GetOrientation(long stepNum) const {
    AffineTransform t = this->GetAffineTransform(stepNum);
    return GetRotation(t);
  }

  Point TurtleGraphics::GetPosition(long stepNum) const {
    LOG_TODO << "TODO CS349: Implement GetPosition";
    AffineTransform t = this->GetAffineTransform(stepNum);
    Point origin;
    return t * origin;
  }

  void TurtleGraphics::GoToStep(long stepNum) {
    this->drawStepNum = stepNum;
    this->Repaint();
  }

  AffineTransform TurtleGraphics::WalkInstructions(Graphics* g, long stepNum, bool executeOps) const {
    long curStep = 0;
    long numSteps = max(0L, min(this->GetNumSteps(), stepNum));
    AffineTransform originalTransform;
    AffineTransform curTransform;

    LOG_DEBUG << "Turtle WalkInstructions";
    LOG_DEBUG << "Step num requested: " << stepNum;
    LOG_DEBUG << "Num steps available: " << this->GetNumSteps();
    LOG_DEBUG << "Num steps to do: " << numSteps;

    if (executeOps) {
      originalTransform = g->GetTransform();
    }
    for (vector<TurtleGraphicsOp*>::const_iterator i = instructions.begin(); i != instructions.end(); i++) {
      if (curStep >= numSteps) {
        break;
      }
      TurtleGraphicsOp* op = *i;
      int numStepsLeft = numSteps - curStep;
      int numStepsForOp = 0;

      if (numStepsLeft > op->GetNumSteps()) {
        numStepsForOp = op->GetNumSteps();
      } else {
        numStepsForOp = numStepsLeft;
      }

      LOG_DEBUG << "Cur step num: " << curStep;
      LOG_DEBUG << "Num steps left: " << numStepsLeft;
      LOG_DEBUG << op->ToString();
      LOG_DEBUG << "Num steps for turtle op: " << numStepsForOp;

      if (executeOps) {
        op->ExecuteOp(g, numStepsForOp);
      }

      Point origin;
      Point curLocation = curTransform * origin;
      AffineTransform newTransform = op->GetNewTransform(numStepsForOp);

      curTransform = curTransform * newTransform;

      Point newLocation = curTransform * origin;
      double newTheta = GetRotation(curTransform);

      LOG_DEBUG << "Current turtle location: (" << curLocation.x << ", " << curLocation.y << ")";
      LOG_DEBUG << "New turtle location: (" << newLocation.x << ", " << newLocation.y << ")";
      LOG_DEBUG << "New rotation: " << newTheta;
      LOG_DEBUG << "";

      if (executeOps) {
        g->SetTransform(originalTransform * curTransform);
      }
      curStep += numStepsForOp;
    }

    if (executeOps) {
      // Draw the turtle
      g->DrawLine(Point(0, TURTLE_LENGTH/2), Point(0, -TURTLE_LENGTH/2));
      g->DrawLine(Point(0, -TURTLE_LENGTH/2), Point(TURTLE_LENGTH, 0));
      g->DrawLine(Point(TURTLE_LENGTH, 0), Point(0, TURTLE_LENGTH/2));

      g->SetTransform(originalTransform);
    }
    return curTransform;
  }

  void TurtleGraphics::PaintComponent(Graphics* graphics) {
    AffineTransform originalTransform = graphics->GetTransform();
      long fC=graphics->GetForegroundColor();
      graphics->SetForegroundColor(graphics->GetBackgroundColor());
      graphics->DrawRect(Rectangle(1, 1, 310, 310));
      graphics->FillRect(Rectangle(1, 1, 310, 310));
      graphics->SetForegroundColor(fC);// 
    SchemeComponent::PaintComponent(graphics);
    graphics->SetTransform(originalTransform);
    this->WalkInstructions(graphics, drawStepNum, true);
  }

  void TurtleGraphics::BroadcastInstructionsChanged() {
    this->NotifyListeners();
  }

  void TurtleGraphics::NotifyListeners() {
    for (vector<ValueListener*>::iterator i = listeners.begin(); i != listeners.end(); i++) {
      (*i)->ValueChanged(this);
    }
  }

  void TurtleGraphics::AddValueListener(ValueListener* l) {
    listeners.push_back(l);
  }

  void TurtleGraphics::RemoveValueListener(ValueListener* l) {
    remove(listeners.begin(), listeners.end(), l);
  }

}

