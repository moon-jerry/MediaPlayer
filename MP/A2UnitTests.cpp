// A2 Unit Tests

#include "gtest/gtest.h"
#include "XApplication.h"
#include "XWindow.h"
#include "MainPanel.h"
#include "Button.h"
#include "Slider.h"
#include "TurtleGraphics.h"

using namespace cs349;

TEST(A2UnitTest, XApplicationCreateDestroy) {
  // Delete any app that is already lying around
  XApplication* app = XApplication::GetInstance();
  delete app;

  // Get new instance
  app = XApplication::GetInstance();
  EXPECT_EQ(0UL, XWindow::GetWindowList().size());
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  // Create a window and add an event to the event queue
  XWindow* win = new XWindow("Dummy", Rectangle(0, 0, 100, 100));
  EXPECT_EQ(1UL, XWindow::GetWindowList().size());
  app->GetEventQueue()->AddEventToQueue(new PaintEvent(win, Rectangle()));
  EXPECT_LT(0, app->GetEventQueue()->GetNumEventsInQueue());

  delete app;
  EXPECT_EQ(0UL, XWindow::GetWindowList().size());

  // Get a new instance, verify it's a new instance by checking that
  // the event queue is empty
  app = XApplication::GetInstance();
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());
}

TEST(A2UnitTest, XApplicationRunOnceTest) {
  // Delete any app that is already lying around
  XApplication* app = XApplication::GetInstance();
  delete app;

  // Get new instance
  app = XApplication::GetInstance();
  EXPECT_EQ(0UL, XWindow::GetWindowList().size());
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  // Create a window and add an event to the event queue
  XWindow* win = new XWindow("Dummy", Rectangle(0, 0, 100, 100));
  app->GetEventQueue()->AddEventToQueue(new PaintEvent(win, Rectangle()));

  int numEvents = app->GetEventQueue()->GetNumEventsInQueue();
  // Run one iteration of the event loop
  app->Run(true);
  EXPECT_EQ(numEvents-1, app->GetEventQueue()->GetNumEventsInQueue());
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

void FlushEventQueue(XApplication* app);
void FlushEventQueue(XApplication* app) {
  const int MAX_ATTEMPTS_TO_CLEAR_QUEUE = 100;
  for (int i = 0; i < MAX_ATTEMPTS_TO_CLEAR_QUEUE; i++) {
    app->Run(true);
    if (app->GetEventQueue()->GetNumEventsInQueue() < 1) {
      break;
    }
  }
}

void SimulateClick(XApplication* app, XWindow* win, Point where);
void SimulateClick(XApplication* app, XWindow* win, Point where) {
  app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseDown, where));
  app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseUp, where));
}

void SimulateDrag(XApplication* app, XWindow* win, Point start, Point middle, Point end);
void SimulateDrag(XApplication* app, XWindow* win, Point start, Point middle, Point end) {
  app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseDown, start));
  app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseDrag, middle));
  app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseUp, end));
}

TEST(A2UnitTest, ButtonTests) {
  // Delete any app that is already lying around
  XApplication* app = XApplication::GetInstance();
  delete app;

  // Get new instance
  app = XApplication::GetInstance();

  const int buttonSize = 100;
  XWindow* win = new XWindow("Dummy", Rectangle(0, 0, 2*buttonSize, buttonSize));
  Button* leftButton = new Button("LEFT", "Left");
  Button* rightButton = new Button("RIGHT", "Right");

  leftButton->SetBounds(Rectangle(0, 0, buttonSize, buttonSize));
  rightButton->SetBounds(Rectangle(buttonSize, 0, buttonSize, buttonSize));
  Point leftButtonCenter(buttonSize/2, buttonSize/2);
  Point rightButtonCenter(buttonSize + buttonSize/2, buttonSize/2);

  win->AddComponent(leftButton);
  win->AddComponent(rightButton);

  ListenerCounter counter;
  leftButton->AddActionListener(&counter);
  rightButton->AddActionListener(&counter);

  // Set the window to visible so it responds to clicks
  win->SetVisible(true);
  // Buttons that are not visible should not respond to clicks...
  leftButton->SetVisible(false);
  rightButton->SetVisible(false);


  // Flush the repaints
  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());
  
  EXPECT_EQ(0, counter.numActionsPerformed);
  EXPECT_EQ(string(""), counter.lastComponentName);


  SimulateClick(app, win, leftButtonCenter);
  SimulateClick(app, win, rightButtonCenter);
  EXPECT_EQ(4, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(0, counter.numActionsPerformed);
  EXPECT_EQ(string(""), counter.lastComponentName);


  // Now test with visible buttons...
  leftButton->SetVisible(true);
  rightButton->SetVisible(true);

  // Flush the repaints
  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  SimulateClick(app, win, leftButtonCenter);
  EXPECT_EQ(2, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(1, counter.numActionsPerformed);
  EXPECT_EQ(string("LEFT"), counter.lastComponentName);

  SimulateClick(app, win, rightButtonCenter);
  EXPECT_EQ(2, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(2, counter.numActionsPerformed);
  EXPECT_EQ(string("RIGHT"), counter.lastComponentName);


  // Test mouse down then out then in
  SimulateDrag(app, win, leftButtonCenter, rightButtonCenter, leftButtonCenter);
  EXPECT_EQ(3, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(3, counter.numActionsPerformed);
  EXPECT_EQ(string("LEFT"), counter.lastComponentName);


  // Test mouse down then out with button up on the outside
  SimulateDrag(app, win, leftButtonCenter, rightButtonCenter, rightButtonCenter);
  EXPECT_EQ(3, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(3, counter.numActionsPerformed);
  EXPECT_EQ(string("LEFT"), counter.lastComponentName);
}

TEST(A2UnitTest, SliderTests) {
  // Delete any app that is already lying around
  XApplication* app = XApplication::GetInstance();
  delete app;

  // Get new instance
  app = XApplication::GetInstance();

  const int sliderSize = 100;
  XWindow* win = new XWindow("Dummy", Rectangle(0, 0, 2*sliderSize, sliderSize));
  Slider* slider = new Slider("SLIDER", Slider::HORIZONTAL_SLIDER);

  slider->SetBounds(Rectangle(0, 0, sliderSize, sliderSize));
  Point sliderLeft(0, sliderSize/2);
  Point sliderRight(sliderSize-1, sliderSize/2); // Set right side to
                                                 // be one less than
                                                 // the bounds to be
                                                 // considered "in"
                                                 // the slider
  Point sliderCenter(sliderSize/2, sliderSize/2);

  win->AddComponent(slider);


  // Some basic tests before testing interaction
  slider->SetMinimum(0);
  slider->SetMaximum(100);
  slider->SetCurValue(0);

  EXPECT_EQ(0, slider->GetMinimum());
  EXPECT_EQ(100, slider->GetMaximum());
  EXPECT_EQ(0, slider->GetCurValue());

  slider->SetCurValue(97);
  EXPECT_EQ(97, slider->GetCurValue());

  slider->SetCurValue(-1);
  EXPECT_EQ(0, slider->GetCurValue());

  slider->SetCurValue(101);
  EXPECT_EQ(100, slider->GetCurValue());

  slider->SetCurValue(0);


  // Now test interaction
  ListenerCounter counter;
  slider->AddValueListener(&counter);

  EXPECT_EQ(0, counter.numValuesChanged);
  EXPECT_EQ(string(""), counter.lastComponentName);


  // Set the window to visible so it responds to clicks
  win->SetVisible(true);
  // Sliders that are not visible should not respond to clicks...
  slider->SetVisible(false);

  // Flush the repaints
  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());
  
  EXPECT_EQ(0, counter.numValuesChanged);
  EXPECT_EQ(string(""), counter.lastComponentName);


  SimulateClick(app, win, sliderCenter);
  EXPECT_EQ(2, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(0, counter.numValuesChanged);
  EXPECT_EQ(string(""), counter.lastComponentName);
  EXPECT_EQ(0, slider->GetCurValue());
  


  // Now test with visible slider...
  slider->SetVisible(true);

  // Flush the repaints
  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(0, slider->GetCurValue());

  SimulateClick(app, win, sliderCenter);
  EXPECT_EQ(2, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(1, counter.numValuesChanged);
  EXPECT_EQ(string("SLIDER"), counter.lastComponentName);
  EXPECT_EQ(50, slider->GetCurValue());


  SimulateClick(app, win, sliderLeft);
  EXPECT_EQ(2, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(2, counter.numValuesChanged);
  EXPECT_EQ(string("SLIDER"), counter.lastComponentName);
  EXPECT_EQ(0, slider->GetCurValue());


  SimulateClick(app, win, sliderRight);
  EXPECT_EQ(2, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(3, counter.numValuesChanged);
  EXPECT_EQ(string("SLIDER"), counter.lastComponentName);
  EXPECT_EQ(100, slider->GetCurValue());
}

TEST(A2UnitTest, TurtleTests) {
  TurtleGraphics* turtle = new TurtleGraphics("TURTLE");

  ListenerCounter counter;
  turtle->AddValueListener(&counter);

  EXPECT_EQ(0, counter.numValuesChanged);
  EXPECT_EQ(string(""), counter.lastComponentName);

  turtle->BroadcastInstructionsChanged();
  
  EXPECT_EQ(1, counter.numValuesChanged);
  EXPECT_EQ(string("TURTLE"), counter.lastComponentName);
}
