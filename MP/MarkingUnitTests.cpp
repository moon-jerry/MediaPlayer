// A2 Unit Tests
// Author: Michael Terry

#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <readline/readline.h>
#include <readline/history.h>
#include <algorithm>
#include <unistd.h> // For usleep
#include "XApplication.h"
#include "XWindow.h"
#include "MainPanel.h"
#include "Button.h"
#include "Slider.h"
#include "TurtleGraphics.h"
#include "SchemeInterpreter.h"
#include "SchemeUtils.h"
#include "A2WidgetIdentifiers.h"
#include "Logging.h"

using namespace cs349;
using namespace std;

// We introduce a new namespace merely to avoid name collisions with
// the classes that are redefined here

namespace marking_namespace {

  class Marks;

  void FlushEventQueue(XApplication* app);
  void SimulateClick(XApplication* app, Component* c);
  void SimulateClick(XApplication* app, XWindow* win, Point where);
  void SimulateDrag(XApplication* app, XWindow* win, Point start, Point middle, Point end);

  Component* getComponentWithName(Component* parent, const string & name);

  void markButtons(ostream & fout);
  void markSliders(ostream & fout);
  void markInteractorTree(ostream & fout, MainPanel* mainPanel);
  void markPanel(ostream & fout, Component* panel, const vector<string> & expectedChildren, const string & markingName);
  void markVisual(XApplication* app, const string & testName, const string & prompt, Marks* marks);
  void markClip(ostream & fout, XApplication* app, XWindow* win, MainPanel* panel);
  void markAppInteraction(ostream & fout, XApplication* app, MainPanel* panel);

  class Marks {
  private:
    stringstream markingRecord;
  public:
    string markLabel;
    int totalPossible;
    int received;
    static int allPossibleMarks;
    static int allReceivedMarks;
    bool marksOutput;
    Marks(const string markLabel) {
      this->markLabel     = markLabel;
      this->totalPossible = 0;
      this->received      = 0;
      this->marksOutput   = false;
    }
    virtual ~Marks() {
      if (!marksOutput) {
        LOG_ERROR << "*** Error: Marks for " << markLabel << " NOT output";
      }
    }
    /**
     * Validates boolean, increments received marks
     */
    void CheckAndAdd(const string & testName, bool result, int weight=2) {
      totalPossible += weight;
      allPossibleMarks += weight;
      if (result) {
        received += weight;
        allReceivedMarks += weight;
      }
      markingRecord << ((result) ? weight : 0) << "/" << weight << " -- " << testName << endl;
    }
    string ToString() const {
      stringstream ss;
      ss << "Marking record for: " << markLabel << endl;
      ss << markingRecord.str();
      ss << received << "/" << totalPossible << " -- total for: " << markLabel << endl;
      return ss.str();
    }
  };

  int Marks::allPossibleMarks = 0;
  int Marks::allReceivedMarks = 0;

  ostream & operator << (ostream &os, Marks & marks);
  ostream & operator << (ostream &os, Marks & marks) {
    marks.marksOutput = true;
    os << marks.ToString();
    return os;
  }



  class ListenerCounter : public ActionListener, public ValueListener {
  public:
    int numActionsPerformed;
    int numValuesChanged;
    string lastComponentName;

    ListenerCounter() {
      numActionsPerformed = 0;
      numValuesChanged = 0;
      lastComponentName = "";
    }
    virtual void ActionPerformed(Component* c) {
      numActionsPerformed++;
      lastComponentName = c->GetName();
    }
    virtual void ValueChanged(Component* c) {
      numValuesChanged++;
      lastComponentName = c->GetName();
    }
  };

  void FlushEventQueue(XApplication* app) {
    const int MAX_ATTEMPTS_TO_CLEAR_QUEUE = 200;
    for (int i = 0; i < MAX_ATTEMPTS_TO_CLEAR_QUEUE; i++) {
      app->Run(true);
    }
  }

  void SimulateClick(XApplication* app, Component* c) {
    Point center;
    if (c) {
      center.x = c->GetBounds().width/2;
      center.y = c->GetBounds().height/2;
    }
    while (c && c->GetParent()) {
      Point location = c->GetBounds().GetTopLeft();
      center.x += location.x;
      center.y += location.y;
      c = c->GetParent();
    }
    XWindow* parentWindow = NULL;
    if (c) {
      parentWindow = c->GetParentWindow();
    }
    SimulateClick(app, parentWindow, center);
  }

  void SimulateClick(XApplication* app, XWindow* win, Point where) {
    if (app && win) {
      app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseDown, where));
      app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseUp, where));
    }
  }

  void SimulateDrag(XApplication* app, XWindow* win, Point start, Point middle, Point end) {
    app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseDown, start));
    app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseDrag, middle));
    app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseUp, end));
  }


  void markVisual(XApplication* app, const string & testName, const string & prompt, Marks* marks) {
    char* markerInput = NULL;
    bool invalidInput = true;
    while (markerInput == NULL || strlen(markerInput) == 0 || invalidInput) {
      // Disconnect the readline callback handler from the Scheme
      // interpreter
      rl_callback_handler_remove();
      rl_unbind_key(7); // CTRL_G
    
      markerInput = readline(prompt.c_str());
      if (markerInput && strlen(markerInput) > 0) {
        if (markerInput[0] == 'y' || markerInput[0] == 'n' || markerInput[0] == 'h') {
          invalidInput = false;
          if (markerInput[0] == 'h') {
            marks->CheckAndAdd(testName, true, 1);
            marks->CheckAndAdd(testName, false, 1);
          } else {
            marks->CheckAndAdd(testName, markerInput[0] == 'y');
          }
        }
      }
    }
    free(markerInput);
    markerInput = NULL;
  }

  void markButtons(ostream & fout) {
    XApplication* app = XApplication::GetInstance();

    const int buttonSize  = 100;
    XWindow*  win         = new XWindow("Button Tests Window", Rectangle(50, 20, 4*buttonSize, 4*buttonSize));
    Panel*    panel       = new Panel("Panel");
    Button*   leftButton  = new Button("LEFT", "Left");
    Button*   rightButton = new Button("RIGHT", "Right");

    panel->SetBounds(Rectangle(2*buttonSize, 2*buttonSize, buttonSize*2, buttonSize));
    leftButton->SetBounds(Rectangle(0, 0, buttonSize, buttonSize));
    rightButton->SetBounds(Rectangle(buttonSize, 0, buttonSize, buttonSize));
    Point leftButtonCenter = Point(panel->GetBounds().x + leftButton->GetBounds().x + buttonSize/2,
                                   panel->GetBounds().y + leftButton->GetBounds().y + buttonSize/2);
    Point rightButtonCenter = Point(leftButtonCenter.x + buttonSize, leftButtonCenter.y);

    panel->AddComponent(leftButton);
    panel->AddComponent(rightButton);
    win->AddComponent(panel);

    ListenerCounter counter;
    leftButton->AddActionListener(&counter);
    rightButton->AddActionListener(&counter);

    // Set the window to visible so it responds to clicks
    win->SetVisible(true);
    // Buttons that are not visible should not respond to clicks...
    panel->SetVisible(false);
    leftButton->SetVisible(false);
    rightButton->SetVisible(false);

    // Flush the repaints
    FlushEventQueue(app);
    SimulateClick(app, leftButton);
    SimulateClick(app, rightButton);
    FlushEventQueue(app);


    Marks marks("Button tests");
    marks.CheckAndAdd("button not responsive when not visible", 0 == counter.numActionsPerformed);

    // Now test with visible buttons, but invisible parent panel
    leftButton->SetVisible(true);
    rightButton->SetVisible(true);
    FlushEventQueue(app);

    int preClickNum = counter.numActionsPerformed;
    SimulateClick(app, leftButton);
    FlushEventQueue(app);
    marks.CheckAndAdd("button not responsive when parent not visible", preClickNum == counter.numActionsPerformed);

    // Now test with visible buttons, AND visible parent panel
    panel->SetVisible(true);
    FlushEventQueue(app);

    preClickNum = counter.numActionsPerformed;
    SimulateClick(app, leftButton);
    FlushEventQueue(app);
    marks.CheckAndAdd("button responsive when it and parent are visible", (preClickNum+1) == counter.numActionsPerformed);

    FlushEventQueue(app);
    preClickNum = counter.numActionsPerformed;
    SimulateDrag(app, win, leftButtonCenter, rightButtonCenter, leftButtonCenter);
    FlushEventQueue(app);
    marks.CheckAndAdd("button responds when mouse down/up within its bounds", (preClickNum+1) == counter.numActionsPerformed);


    // Test mouse down then out with button up on the outside
    FlushEventQueue(app);
    preClickNum = counter.numActionsPerformed;
    SimulateDrag(app, win, leftButtonCenter, rightButtonCenter, rightButtonCenter);
    FlushEventQueue(app);
    marks.CheckAndAdd("button doesn't respond when mouse up outside its bounds", preClickNum == counter.numActionsPerformed);

    // Test clipping
    panel->SetBounds(Rectangle(panel->GetBounds().x, panel->GetBounds().y, 1, 1));
    FlushEventQueue(app);
    preClickNum = counter.numActionsPerformed;
    SimulateClick(app, leftButton);
    marks.CheckAndAdd("button doesn't respond when it is clipped by parent", preClickNum == counter.numActionsPerformed);

    markVisual(app, "buttons visible", "Do you see two buttons with the text LEFT and RIGHT in them? (y/n/h): ", &marks);
    fout << marks << endl;
  }


  void markSliders(ostream & fout) {
    XApplication* app = XApplication::GetInstance();

    const int sliderSize = 231;
    XWindow* win = new XWindow("Dummy", Rectangle(0, 0, 2*sliderSize, sliderSize));
    Slider* slider = new Slider("SLIDER", Slider::HORIZONTAL_SLIDER);
    Slider* rightSlider = new Slider("RIGHT SLIDER", Slider::HORIZONTAL_SLIDER);

    slider->SetBounds(Rectangle(0, 0, sliderSize, sliderSize));
    rightSlider->SetBounds(Rectangle(sliderSize, 0, sliderSize, sliderSize));
    Point sliderLeft(0, sliderSize/2);
    // Set right side to be one less than
    // the bounds to be considered "in" the slider
    Point sliderRight(sliderSize-1, sliderSize/2); 
    Point sliderCenter(sliderSize/2, sliderSize/2);

    win->AddComponent(slider);
    win->AddComponent(rightSlider);

    const int sliderMin = -300;
    const int sliderMax = 300;
    const int sliderStart = -11;
    // Some basic tests before testing interaction
    slider->SetMinimum(sliderMin);
    slider->SetMaximum(sliderMax);
    slider->SetCurValue(sliderStart);

    Marks marks("Slider tests");
    marks.CheckAndAdd("slider minimum correctly set", sliderMin == slider->GetMinimum());
    marks.CheckAndAdd("slider maximum correctly set", sliderMax == slider->GetMaximum());
    marks.CheckAndAdd("slider current value correctly set", sliderStart == slider->GetCurValue());

    slider->SetCurValue(2 * sliderMin);
    marks.CheckAndAdd("slider clamps to minimum value", sliderMin == slider->GetCurValue());

    slider->SetCurValue(2 * sliderMax);
    marks.CheckAndAdd("slider clamps to maximum value", sliderMax == slider->GetCurValue());

    // Now test interaction
    ListenerCounter counter;

    slider->SetCurValue(sliderMax);
    slider->AddValueListener(&counter);

    // Set the window to visible so it responds to clicks
    win->SetVisible(true);
    // Sliders that are not visible should not respond to clicks...
    slider->SetVisible(false);

    // Flush the repaints
    FlushEventQueue(app);
    SimulateClick(app, win, sliderCenter);
    FlushEventQueue(app);

    marks.CheckAndAdd("slider doesn't respond when not visible", sliderMax == slider->GetCurValue());
  
    // Now test with visible slider...
    int preChangeCount = counter.numValuesChanged;
    slider->SetVisible(true);
    rightSlider->SetVisible(true);

    // Flush the repaints
    FlushEventQueue(app);
    SimulateClick(app, win, sliderCenter);
    FlushEventQueue(app);

    marks.CheckAndAdd("slider responds when visible", (preChangeCount+1) == counter.numValuesChanged);
    marks.CheckAndAdd("correct slider responds to event", string("SLIDER") == counter.lastComponentName);
    marks.CheckAndAdd("slider set to approximately correct value on click", abs(((sliderMax-sliderMin)/2+sliderMin) - slider->GetCurValue()) <= 4);

    SimulateClick(app, win, sliderLeft);
    FlushEventQueue(app);
    marks.CheckAndAdd("slider set to minimum when click on left side", sliderMin == slider->GetCurValue());

    SimulateClick(app, win, sliderRight);
    FlushEventQueue(app);
    marks.CheckAndAdd("slider set to maximum when click on right side", sliderMax == slider->GetCurValue());

    SimulateClick(app, win, Point(sliderLeft.x + sliderSize/4, sliderCenter.y));
    FlushEventQueue(app);
    marks.CheckAndAdd("slider set to value > min, < mid point when clicked at 1/4",
                                 slider->GetCurValue() > sliderMin
                                 && slider->GetCurValue() < (sliderMin + (sliderMax-sliderMin)/2));

    SimulateClick(app, win, Point(sliderLeft.x +  sliderSize * 3/4, sliderCenter.y));
    FlushEventQueue(app);
    marks.CheckAndAdd("slider set to value < max, > mid point when clicked at 3/4",
                                 slider->GetCurValue() < sliderMax
                                 && slider->GetCurValue() > (sliderMin + (sliderMax-sliderMin)/2));

    markVisual(app, "two sliders visible", "Do you see two sliders side-by-side? (y/n/h): ", &marks);
    fout << marks << endl;
  }


  void markInteractorTree(ostream & fout, MainPanel* mainPanel) {
    vector<string> mainPanelChildren;
    vector<string> turtleGraphicsChildren;
    vector<string> playbackPanelChildren;
    vector<string> stepPanelChildren;
    vector<string> buttonPanelChildren;

    mainPanelChildren.push_back(string(TURTLE_GRAPHICS_PANEL_ID));
    mainPanelChildren.push_back(string(PLAYBACK_PANEL_ID));
    mainPanelChildren.push_back(string(CURRENT_TURTLE_STEP_PANEL_ID));
    mainPanelChildren.push_back(string(BUTTON_PANEL_ID));

    turtleGraphicsChildren.push_back(string(TURTLE_GRAPHICS_OUTPUT_LABEL_ID));
    turtleGraphicsChildren.push_back(string(TURTLE_GRAPHICS_WIDGET_ID));

    playbackPanelChildren.push_back(string(PLAYBACK_OUTPUT_LABEL_ID));
    playbackPanelChildren.push_back(string(PLAYBACK_SLIDER_ID));

    stepPanelChildren.push_back(string(CURRENT_TURTLE_STEP_LABEL_ID));
    stepPanelChildren.push_back(string(CURRENT_TURTLE_STEP_SLIDER_ID));

    buttonPanelChildren.push_back(string(GO_TO_START_BUTTON_ID));
    buttonPanelChildren.push_back(string(PREVIOUS_FRAME_BUTTON_ID));
    buttonPanelChildren.push_back(string(PLAY_BUTTON_ID));
    buttonPanelChildren.push_back(string(NEXT_FRAME_BUTTON_ID));
    buttonPanelChildren.push_back(string(GO_TO_END_BUTTON_ID));

    markPanel(fout, mainPanel, mainPanelChildren, "Main panel interactor tree verification");
    markPanel(fout, getComponentWithName(mainPanel, string(TURTLE_GRAPHICS_PANEL_ID)),
              turtleGraphicsChildren, "Turtle graphics panel interactor tree verification");
    markPanel(fout, getComponentWithName(mainPanel, string(PLAYBACK_PANEL_ID)),
              playbackPanelChildren, "Playback panel interactor tree verification");
    markPanel(fout, getComponentWithName(mainPanel, string(CURRENT_TURTLE_STEP_PANEL_ID)),
              stepPanelChildren, "Step panel interactor tree verification");
    markPanel(fout, getComponentWithName(mainPanel, string(BUTTON_PANEL_ID)),
              buttonPanelChildren, "Button panel interactor tree verification");

    vector<string> allComponents;
    allComponents.insert(allComponents.end(), mainPanelChildren.begin(), mainPanelChildren.end());
    allComponents.insert(allComponents.end(), turtleGraphicsChildren.begin(), turtleGraphicsChildren.end());
    allComponents.insert(allComponents.end(), playbackPanelChildren.begin(), playbackPanelChildren.end());
    allComponents.insert(allComponents.end(), stepPanelChildren.begin(), stepPanelChildren.end());
    allComponents.insert(allComponents.end(), buttonPanelChildren.begin(), buttonPanelChildren.end());

    Marks marks("Component exists validation");
    for (vector<string>::iterator componentName=allComponents.begin(); componentName!=allComponents.end(); componentName++) {
      stringstream ss;
      ss << "component " << *componentName << " exists";
      marks.CheckAndAdd(ss.str(), getComponentWithName(mainPanel, *componentName) != NULL, 1);
    }
    fout << marks << endl;
  }

  void markPanel(ostream & fout, Component* panel, const vector<string> & expectedChildren, const string & markingName) {
    const int numExpected = expectedChildren.size();
    Marks marks(markingName);
    if (panel) {
      vector<Component*> children = panel->GetBreadthFirstInteractorTree();
      for (vector<Component*>::iterator childIterator=children.begin(); childIterator!=children.end(); childIterator++) {
        Component* child = *childIterator;
        if (!child) continue;
        string childName = child->GetName();
        for (vector<string>::const_iterator nameIterator=expectedChildren.begin(); nameIterator != expectedChildren.end(); nameIterator++) {
          if (childName == *nameIterator) {
            marks.received++;
            marks.allReceivedMarks++;
            break;
          }
        }
      }
    }
    // kludge
    marks.totalPossible = numExpected;
    marks.allPossibleMarks += numExpected;
    fout << marks << endl;
  }

  void markClip(ostream & fout, XApplication* app, XWindow* win, MainPanel* panel) {
    /*
      Rearrange the components, test the clip
     */
    Marks marks("Clip tests");
    Component* testPanel = NULL;
    Component* stepSlider = NULL;
    Component* playbackSlider = NULL;
    Component* turtleGraphics = NULL;
    Component* playButton = NULL;
    double maxWidth = 5;

    if (panel) {
      testPanel = new Panel("Clip Test Panel");
      testPanel->SetVisible(true);

      if (win) {
        win->RemoveComponent(panel);
        win->AddComponent(testPanel);
      }
      panel->SetVisible(false);

      stepSlider     = getComponentWithName(panel, string(CURRENT_TURTLE_STEP_SLIDER_ID));
      playbackSlider = getComponentWithName(panel, string(PLAYBACK_SLIDER_ID));
      turtleGraphics = getComponentWithName(panel, string(TURTLE_GRAPHICS_WIDGET_ID));
      playButton     = getComponentWithName(panel, string(PLAY_BUTTON_ID));

      double curY = 0;
      if (stepSlider && stepSlider->GetParent()) {
        stepSlider->GetParent()->RemoveComponent(stepSlider);
        testPanel->AddComponent(stepSlider);
        stepSlider->SetBounds(Rectangle(0, 0, stepSlider->GetBounds().width, stepSlider->GetBounds().height));
        maxWidth = max(maxWidth, stepSlider->GetBounds().width);
        curY += stepSlider->GetBounds().height;
      }
      if (playbackSlider && playbackSlider->GetParent()) {
        playbackSlider->GetParent()->RemoveComponent(playbackSlider);
        testPanel->AddComponent(playbackSlider);
        playbackSlider->SetBounds(Rectangle(0, curY, playbackSlider->GetBounds().width, playbackSlider->GetBounds().height));
        maxWidth = max(maxWidth, playbackSlider->GetBounds().width);
        curY += playbackSlider->GetBounds().height;
      }
      if (turtleGraphics && turtleGraphics->GetParent()) {
        turtleGraphics->GetParent()->RemoveComponent(turtleGraphics);
        testPanel->AddComponent(turtleGraphics);
        turtleGraphics->SetBounds(Rectangle(0, curY, turtleGraphics->GetBounds().width, turtleGraphics->GetBounds().height));
        maxWidth = max(maxWidth, turtleGraphics->GetBounds().width);
        curY += turtleGraphics->GetBounds().height;
      }
      if (playButton && playButton->GetParent()) {
        playButton->GetParent()->RemoveComponent(playButton);
        testPanel->AddComponent(playButton);
        playButton->SetBounds(Rectangle(0, curY, playButton->GetBounds().width, playButton->GetBounds().height));
        maxWidth = max(maxWidth, playButton->GetBounds().width);
        curY += playButton->GetBounds().height;
      }
      testPanel->SetBounds(Rectangle(0, 0, maxWidth, curY));
      if (win) {
        win->SetBounds(testPanel->GetBounds());
      }
    }
    FlushEventQueue(app);
    markVisual(app, "components vertically lined up", "Do you see the two sliders, turtle graphics component, and play button lined up vertically, one on top of the other? (y/n/h): ", &marks);
    testPanel->SetBounds(Rectangle(1, 1, maxWidth/2, testPanel->GetBounds().height));
    win->Repaint(Rectangle(0, 0, maxWidth, testPanel->GetBounds().height));
    FlushEventQueue(app);
    markVisual(app, "components properly clipped", "Is the right half of the window now blank? (It should be clipped to show only the left half of the window.) (y/n/h): ", &marks);
    fout << marks << endl;
  }

  void markAppInteraction(ostream & fout, XApplication* app, MainPanel* panel) {
    TurtleGraphics* turtle = (TurtleGraphics*)getComponentWithName(panel, string(TURTLE_GRAPHICS_WIDGET_ID));
    Slider* stepSlider = (Slider*)getComponentWithName(panel, string(CURRENT_TURTLE_STEP_SLIDER_ID));
    Marks appMarks("Application interaction tests");
    if (turtle) {
      turtle->ClearInstructions();
      turtle->RotateInDegrees(45);
      turtle->Forward(1000, true, false);
      turtle->GoToStep(turtle->GetNumSteps());
    }
    FlushEventQueue(app);

    appMarks.CheckAndAdd("turtle step slider minimum <= 1", turtle && stepSlider && stepSlider->GetMinimum() <= 1);
    appMarks.CheckAndAdd("turtle step slider maximum ~= num steps", turtle && stepSlider && stepSlider->GetMaximum() >= (turtle->GetNumSteps()-1));

    if (stepSlider) {
      stepSlider->SetCurValue(stepSlider->GetMinimum());
    }

    Component* stepForward = getComponentWithName(panel, string(NEXT_FRAME_BUTTON_ID));
    SimulateClick(app, stepForward);
    FlushEventQueue(app);
    appMarks.CheckAndAdd("turtle step slider forward one step", stepSlider && stepSlider->GetCurValue() >= (stepSlider->GetMinimum()+1));

    Component* stepBackward = getComponentWithName(panel, string(PREVIOUS_FRAME_BUTTON_ID));
    SimulateClick(app, stepBackward);
    FlushEventQueue(app);
    appMarks.CheckAndAdd("turtle step slider backward one step", stepSlider && stepSlider->GetCurValue() == stepSlider->GetMinimum());

    Component* toEnd = getComponentWithName(panel, string(GO_TO_END_BUTTON_ID));
    SimulateClick(app, toEnd);
    FlushEventQueue(app);
    appMarks.CheckAndAdd("turtle step slider to the end", stepSlider && stepSlider->GetCurValue() == stepSlider->GetMaximum());

    Component* toStart = getComponentWithName(panel, string(GO_TO_START_BUTTON_ID));
    SimulateClick(app, toStart);
    FlushEventQueue(app);
    appMarks.CheckAndAdd("turtle step slider to the start", stepSlider && stepSlider->GetCurValue() == stepSlider->GetMinimum());

    Component* play = getComponentWithName(panel, string(PLAY_BUTTON_ID));
    SimulateClick(app, play);
    FlushEventQueue(app);
    usleep(500000);
    FlushEventQueue(app);
    appMarks.CheckAndAdd("turtle animation advanced", stepSlider && stepSlider->GetCurValue() > stepSlider->GetMinimum());

    SimulateClick(app, play);
    FlushEventQueue(app);
    int pausedValue = -1;
    if (stepSlider) {
      pausedValue = stepSlider->GetCurValue();
    }
    usleep(500000);
    FlushEventQueue(app);
    appMarks.CheckAndAdd("turtle animation paused", stepSlider && stepSlider->GetCurValue() == pausedValue);

    fout << appMarks << endl;
  }

  Component* getComponentWithName(Component* parent, const string & name) {
    if (parent == NULL) {
      return NULL;
    }
    vector<Component*> allComponents = parent->GetBreadthFirstInteractorTree();
    for (vector<Component*>::iterator componentIterator=allComponents.begin(); componentIterator != allComponents.end(); componentIterator++) {
      Component* c = *componentIterator;
      if (c && c->GetName() == name) {
        return c;
      }
    }
    return NULL;
  }

  TEST(A2UnitTest, AutomatedMarkingUnitTest) {

    google::InitGoogleLogging("a2_unittests");
    FLAGS_logtostderr = 1;

    // Time code from: http://www.cplusplus.com/reference/ctime/asctime/
    time_t rawtime;
    struct tm * timeinfo;
    time( &rawtime );
    timeinfo = localtime ( &rawtime );

    ofstream fout;
    fout.open("automated_marks.txt");
    fout << "Marking: " << asctime(timeinfo) << endl;

    XApplication* app = XApplication::GetInstance();

    // Init the Scheme interpreter
    const char* scheme_init_fname = "init.scm";
    const char* cs349_init_fname  = "cs349_init.scm";
    scheme_interpreter_init(scheme_init_fname, cs349_init_fname, 0, NULL);

    markButtons(fout);
    markSliders(fout);

    XWindow* win = new XWindow("Clip Tests", Rectangle(0, 0, 100, 100));
    MainPanel* mainPanel = new MainPanel();

    win->AddComponent(mainPanel);
    win->SetVisible(true);
    mainPanel->SetVisible(true);

    markInteractorTree(fout, mainPanel);
    markAppInteraction(fout, app, mainPanel);
    markClip(fout, app, win, mainPanel);

    fout << Marks::allReceivedMarks << "/" << Marks::allPossibleMarks << " ** total for all automated marks" << endl;

    scheme_interpreter_shutdown();
    fout.flush();
  }
}
