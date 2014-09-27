#include "TurtleGraphicsOp.h"
#include <sstream>
#include "Logging.h"

namespace cs349 {
  TurtleGraphicsForwardOp::TurtleGraphicsForwardOp(double amount, bool penDown) {
    this->amount   = amount;
    this->penDown  = penDown;
    this->numSteps = max(1L, (long) ((amount >= 0L) ? amount : -amount));
  }

  double TurtleGraphicsForwardOp::GetDistance(long stepNum) const {
    double t = max(0L, min(stepNum, numSteps));
    t /= numSteps;
    return t * amount;
  }

  AffineTransform TurtleGraphicsForwardOp::GetNewTransform(long stepNum) const {
    double distance = GetDistance(stepNum);
    return AffineTransform::MakeTranslation(distance, 0.0);
  }

  long TurtleGraphicsForwardOp::GetNumSteps() const {
    return numSteps;
  }

  void TurtleGraphicsForwardOp::ExecuteOp(Graphics* g, long stepNum) const {
    if (penDown) {
      double distance = GetDistance(stepNum);
      LOG_DEBUG << "Forward Op, drawing line from (" << (g->GetTransform() * Point(0,0)).x << ", " << (g->GetTransform() * Point(0,0)).y << ") to (" << (g->GetTransform() * Point(distance, 0)).x << ", " << (g->GetTransform() * Point(distance, 0)).y << ")";
      g->DrawLine(Point(0,0), Point(distance, 0));
    }
  }

  string TurtleGraphicsForwardOp::ToString() const {
    stringstream sstream;
    sstream << "Turtle Forward Op (amount: " << this->amount << ", penDown: " << this->penDown << " numSteps: " << this->numSteps << ")";
    return sstream.str();
  }

  string TurtleGraphicsForwardOp::AsSexp() const {
    stringstream sstream;
    sstream << "(turtle-forward " << this->amount << " " << ((this->penDown) ? "#t" : "#f") << ")";
    return sstream.str();
  }


  TurtleGraphicsRotateOp::TurtleGraphicsRotateOp(double theta) {
    this->theta = theta;
  }

  AffineTransform TurtleGraphicsRotateOp::GetNewTransform(long stepNum) const {
    return AffineTransform::MakeRotation(theta);
  }

  long TurtleGraphicsRotateOp::GetNumSteps() const {
    return 1;
  }

  void TurtleGraphicsRotateOp::ExecuteOp(Graphics* g, long stepNum) const {
    ; // no-op
  }

  string TurtleGraphicsRotateOp::ToString() const {
    stringstream sstream;
    sstream << "Turtle Rotate Op (theta: " << this->theta << " numSteps: 1)";
    return sstream.str();
  }

  string TurtleGraphicsRotateOp::AsSexp() const {
    stringstream sstream;
    sstream << "(turtle-rotate " << this->theta << ")";
    return sstream.str();
  }


  TurtleGraphicsDrawTextOp::TurtleGraphicsDrawTextOp(const string & text) {
    this->text = text;
  }

  AffineTransform TurtleGraphicsDrawTextOp::GetNewTransform(long stepNum) const {
    return AffineTransform();
  }

  long TurtleGraphicsDrawTextOp::GetNumSteps() const {
    return 1;
  }

  void TurtleGraphicsDrawTextOp::ExecuteOp(Graphics* g, long stepNum) const {
    g->DrawText(Point(0,0), text);
  }

  string TurtleGraphicsDrawTextOp::ToString() const {
    stringstream sstream;
    sstream << "Turtle Draw Text Op (text: " << this->text << " numSteps: 1)";
    return sstream.str();
  }

  string TurtleGraphicsDrawTextOp::AsSexp() const {
    stringstream sstream;
    sstream << "(turtle-draw-text " << this->text << ")";
    return sstream.str();
  }


  AffineTransform TurtleGraphicsDrawPointOp::GetNewTransform(long stepNum) const {
    return AffineTransform();
  }

  long TurtleGraphicsDrawPointOp::GetNumSteps() const {
    return 1;
  }

  void TurtleGraphicsDrawPointOp::ExecuteOp(Graphics* g, long stepNum) const {
    g->DrawPoint(Point(0,0));
  }

  string TurtleGraphicsDrawPointOp::ToString() const {
    return string("Turtle Draw Point Op (numSteps: 1)");
  }

  string TurtleGraphicsDrawPointOp::AsSexp() const {
    return string("(turtle-draw-point)");
  }


}
