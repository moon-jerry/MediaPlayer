// Author: jerry yan
#ifndef _LABEL_H_
#define _LABEL_H_
#include "Component.h"
#include "ActionListener.h"
#include <vector>

using namespace std;

namespace cs349 {
    
    class Label : public Component {
    private:
        string label;
        Point location;
        bool needFrame;
        Rectangle aR; //test
    protected:
        /** test
         * The method that does the actual painting of the component once
         * the Graphics context object has been set up by the Paint
         * method in Component.
         */
        virtual void PaintComponent(Graphics* g);
        
    public:
        
        /**
         * Returns the text label for this Button//test
         */
        string GetLabel() const;
        
        /**
         * Sets the text label for this button
         */
        void SetLabel(Point aP);
        
        Label(const string & name, const Point aP); //a constructor
        
        void setFrame(bool a, Rectangle aRec);
        // TODO CS349: Add any other necessary methods or overrides here
        // TODO CS349
    };
}

#endif