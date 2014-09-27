#ifndef __TURTLE_GRAPHICS__
#define __TURTLE_GRAPHICS__

#include <vector>
#include "Point.h"
#include "Graphics.h"
#include "SchemeComponent.h"
#include "TurtleGraphicsOp.h"
#include "ValueListener.h"

using namespace std;

namespace cs349 {

  /**
   * A Component that supports drawing via turtle graphics
   * instructions. The instructions to control the turtle can be
   * manipulated via C++ code or within Scheme.
   *
   * For the purposes of creating an application that can play back
   * turtle instructions, the methods you will be most interested in
   * are the GetNumSteps() method (which indicates how many steps the
   * turtle can execute from start to finish, for all the instructions
   * it currently has stored), and GoToStep(), which will cause the
   * TurtleGraphics display to display that particular step.
   *
   * Note that there are no facilities to animate drawing: You must
   * create a Timer object to animate the TurtleGraphics playback.
   */
  class TurtleGraphics : public SchemeComponent {
  private:
    vector<TurtleGraphicsOp*> instructions;
    long drawStepNum; /**< Draw through step num drawStepNum */
    static const int TURTLE_LENGTH;

    void DeleteInstructions();
    AffineTransform WalkInstructions(Graphics* g, long stepNum, bool executeOps) const;
    static double GetRotation(const AffineTransform & t);

    vector<ValueListener*> listeners;
    void NotifyListeners();

  protected:
    virtual void PaintComponent(Graphics* graphics);

  public:
    /**
     * Constructs a TurtleGraphics object with the specified name
     * (used for identification purposes for unit tests).
     */
    TurtleGraphics(const string & name);
    virtual ~TurtleGraphics();

    /**
     * Rotates the turtle by the amount specified, optionally
     * redrawing it now or sometime in the future (the delay is useful
     * if giving a bunch of commands at once).
     */
    void    Rotate(double theta, bool redrawNow=true); 

    /**
     * Rotates the turtle by the amount specified, optionally
     * redrawing it now or sometime in the future (the delay is useful
     * if giving a bunch of commands at once).
     */
    void    RotateInDegrees(double degrees, bool redrawNow=true); 

    /**
     * Moves the turtle forward by the amount specified, optionally
     * redrawing it now or sometime in the future (the delay is useful
     * if giving a bunch of commands at once).
     */
    void    Forward(double amount, bool penDown, bool redrawNow=true);

    /**
     * Draws text at the current location of the turtle, optionally
     * redrawing it now or sometime in the future (the delay is useful
     * if giving a bunch of commands at once).
     */
    void    DrawText(const string & text, bool redrawNow=true);

    /**
     * Draws a point at the current location of the turtle, optionally
     * redrawing it now or sometime in the future (the delay is useful
     * if giving a bunch of commands at once).
     */
    void    DrawPoint(bool redrawNow=true);

    /**
     * Retrieves the list of turtle instructions. Used for the Scheme interpreter.
     */
    vector<const TurtleGraphicsOp*> GetInstructions() const;

    /**
     * Clears all instructions for the turtle.
     */
    void    ClearInstructions();

    /**
     * Returns the number of steps the turtle can take to execute all
     * of its instructions.
     */
    long    GetNumSteps() const;
    
    /**
     * Requests the TurtleGraphics Component to cue up to, and draw
     * the step number specified.
     */
    void    GoToStep(long stepNum);

    /**
     * Called by scripts to indicate that the instructions have been
     * changed, so controls and other dependent code can update itself
     */
    void    BroadcastInstructionsChanged();

    /**
     * Adds a listener to this component, which will be called when
     * the turtle's instructions have changed. This component is not
     * responsible for the memory management of the ValueListener --
     * external code is responsible for removing the listener and
     * deleting it when it is no longer needed.
     */
    void AddValueListener(ValueListener* l);

    /**
     * Removes the specified ValueListener from the TurtleGraphics.
     */
    void RemoveValueListener(ValueListener* l);

    /**
     * Returns the current orientation of the turtle. Used for scripts.
     */
    double  GetOrientation(long stepNum) const;

    /**
     * Returns the current position of the turtle. Used for scripts.
     */
    Point   GetPosition(long stepNum) const;

    /**
     * Returns the affine transform used by the turtle at the step
     * specified. You won't need this.
     */
    AffineTransform GetAffineTransform(long stepNum) const;
  };
}

#endif // __TURTLE_GRAPHICS__
