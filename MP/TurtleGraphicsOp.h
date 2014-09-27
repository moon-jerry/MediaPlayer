#ifndef _TURTLEGRAPHICSOP_H_
#define _TURTLEGRAPHICSOP_H_

#include <string>
#include "Graphics.h"

using namespace std;

namespace cs349 {
  class TurtleGraphicsOp {
  public:
    virtual ~TurtleGraphicsOp() {}
    virtual long GetNumSteps() const = 0;
    virtual AffineTransform GetNewTransform(long stepNum) const = 0;
    virtual void ExecuteOp(Graphics* g, long stepNum) const = 0;
    virtual string ToString() const = 0;
    virtual string AsSexp() const = 0;
  };

  class TurtleGraphicsForwardOp : public TurtleGraphicsOp {
  private:
    double amount;
    long   numSteps;
    bool   penDown;

    double GetDistance(long stepNum) const;
  public:
    TurtleGraphicsForwardOp(double amount, bool penDown);
    virtual AffineTransform GetNewTransform(long stepNum) const;
    virtual long GetNumSteps() const;
    virtual void ExecuteOp(Graphics* g, long stepNum) const;
    virtual string ToString() const;
    virtual string AsSexp() const;
  };

  class TurtleGraphicsRotateOp : public TurtleGraphicsOp {
  private:
    double theta;
    
  public:
    TurtleGraphicsRotateOp(double theta);
    virtual AffineTransform GetNewTransform(long stepNum) const;
    virtual long GetNumSteps() const;
    virtual void ExecuteOp(Graphics* g, long stepNum) const;
    virtual string ToString() const;
    virtual string AsSexp() const;
  };

  class TurtleGraphicsDrawTextOp : public TurtleGraphicsOp {
  private:
    string text;
    
  public:
    TurtleGraphicsDrawTextOp(const string & text);
    virtual AffineTransform GetNewTransform(long stepNum) const;
    virtual long GetNumSteps() const;
    virtual void ExecuteOp(Graphics* g, long stepNum) const;
    virtual string ToString() const;
    virtual string AsSexp() const;
  };

  class TurtleGraphicsDrawPointOp : public TurtleGraphicsOp {
  public:
    virtual AffineTransform GetNewTransform(long stepNum) const;
    virtual long GetNumSteps() const;
    virtual void ExecuteOp(Graphics* g, long stepNum) const;
    virtual string ToString() const;
    virtual string AsSexp() const;
  };
}

#endif /* _TURTLEGRAPHICSOP_H_ */
