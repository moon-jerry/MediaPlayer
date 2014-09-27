#include "MainPanel.h"
#include "TurtleGraphics.h"
#include "Button.h"
#include "Slider.h"
#include "Label.h"
#include "ValueListener.h"
#include "Timer.h"
#include "XApplication.h"
#include "TimerListener.h"
#include "A2WidgetIdentifiers.h"

using namespace cs349;

MainPanel::MainPanel()
  : Panel(MAIN_PANEL_ID)
{
    
  // TODO CS349: Create your entire interface here. You will need to
  // modify the provided code below to lay things out the way you like

  // Note the use of unique identifiers for each widget when they are
  // constructed. See A2WidgetIdentifiers.h and the assignment
  // specification for the identifiers you should use for each of the
  // components you create. These identifiers are used for unit tests
  // to easily identify components in the interactor tree.
  atimer = new Timer(XApplication::GetInstance()->GetEventQueue(),100, true,this);
  atimer->Stop();
  Component* turtleGraphicsPanel = new Panel(TURTLE_GRAPHICS_PANEL_ID);
  Component* buttonPanel = new Panel(BUTTON_PANEL_ID);
  this-> playbackPanel = new Panel (PLAYBACK_PANEL_ID);
  this-> CurrentTurtleStepPanel = new Panel (CURRENT_TURTLE_STEP_PANEL_ID);
  this-> turtle = new TurtleGraphics(TURTLE_GRAPHICS_WIDGET_ID);
  Label* label1 = new Label("Turtle Graphic Output: ",Point(20,20));
  Label* label2 = new Label("Playback Rate: ",Point(0,15));
  Label* label3 = new Label("Current Turtle Step: ",Point(0,15));
    Label* label4 = new Label("1/10x",Point(0,15));
    Label* label5 = new Label("1x",Point(0,15));
    Label* label6 = new Label("10x",Point(0,15));
    
    Label* label7 = new Label("0",Point(0,15));
    Label* label8 = new Label("100",Point(0,15));
    Label* label9 = new Label("200",Point(0,15));
    
  Button* button1= new Button(GO_TO_START_BUTTON_ID,"go to start button");
  Button* button2 =new Button(PREVIOUS_FRAME_BUTTON_ID, "previous frame button");
  Button* button3 =new Button(PLAY_BUTTON_ID, "play button");
  Button* button4 =new Button(NEXT_FRAME_BUTTON_ID, "next frame button");
  Button* button5 =new Button(GO_TO_END_BUTTON_ID, "go to end button");
  this->playback = new Slider(PLAYBACK_SLIDER_ID , Slider:: VERTICAL_SLIDER );
  this->currentRate = new Slider(CURRENT_TURTLE_STEP_SLIDER_ID , Slider:: VERTICAL_SLIDER );
  playback->SetMaximum(100);
  playback->SetCurValue(50);

  
  
  
 //set bounds
  this->SetBounds(Rectangle(0, 0, 750, 400));
  turtleGraphicsPanel->SetBounds(Rectangle(0,20, 400, 330));
  turtle->SetBounds(Rectangle(25,25, 310, 310));
 
    //Button
    buttonPanel->SetBounds(Rectangle(417,300,300,50));
    button1->SetBounds(Rectangle(0,0,41,50));
    button2->SetBounds(Rectangle(45,0,41,50));
    button3->SetBounds(Rectangle(115,0,41,50));
    button4->SetBounds(Rectangle(185,0,41,50));
    button5->SetBounds(Rectangle(230,0,41,50));
  
    //Labels
    label1->SetBounds(Rectangle(0,0, 400, 400));
    label2->SetBounds(Rectangle(0,0, 300, 20)); // playback rate:
    label3->SetBounds(Rectangle(0,0, 300, 20));//
    label4->SetBounds(Rectangle(0,70, 300, 20));
    label5->SetBounds(Rectangle(140,70, 300, 20));
    label6->SetBounds(Rectangle(280,70, 300, 20));
    label7->SetBounds(Rectangle(0,70, 300, 20));
    label8->SetBounds(Rectangle(140,70, 300, 20));
    label9->SetBounds(Rectangle(280,70, 300, 20));

  //Slider
    playbackPanel->SetBounds(Rectangle(417, 40, 290, 80));
    playback->SetBounds(Rectangle(0,30,320,40));
    CurrentTurtleStepPanel->SetBounds(Rectangle(417, 180, 288, 80));
    currentRate->SetBounds(Rectangle(0, 30, 350, 40));

  //adding compounent
  
  
    this->AddComponent(turtleGraphicsPanel);
     turtleGraphicsPanel->AddComponent(turtle);
     turtleGraphicsPanel->AddComponent(label1);//
    this->AddComponent(buttonPanel); //add buttonPanel to xWindows
     buttonPanel->AddComponent(button1);
     buttonPanel->AddComponent(button2);
     buttonPanel->AddComponent(button3);
     buttonPanel->AddComponent(button4);
     buttonPanel->AddComponent(button5);
    
    
  //this->AddComponent(label3);
    
  this->AddComponent(playbackPanel); // play back panel
    playbackPanel->AddComponent(playback);
    playbackPanel->AddComponent(label2);
    playbackPanel->AddComponent(label4);
    playbackPanel->AddComponent(label5);
    playbackPanel->AddComponent(label6);
  this->AddComponent(CurrentTurtleStepPanel);
    CurrentTurtleStepPanel->AddComponent(label3);
    CurrentTurtleStepPanel->AddComponent(currentRate);
    CurrentTurtleStepPanel->AddComponent(label7);
    CurrentTurtleStepPanel->AddComponent(label8);
    CurrentTurtleStepPanel->AddComponent(label9);
 
    
  buttonPanel->SetVisible(true);
  turtle->SetVisible(true);
  label1->Label::setFrame(true, Rectangle(25,25, 320, 320));
  turtleGraphicsPanel->SetVisible(true);
  playbackPanel->SetVisible(true);
  CurrentTurtleStepPanel->SetVisible(true);
     button1->SetVisible(true);
     button2->SetVisible(true);
     button3->SetVisible(true);
     button4->SetVisible(true);
     button5->SetVisible(true);
     playback->SetVisible(true);
     currentRate->SetVisible(true);

    
//add listener
    turtle->AddValueListener(this);
    playback->AddValueListener(this);
     button3->AddActionListener(this);
     button1->AddActionListener(this);
     button2->AddActionListener(this);
     button4->AddActionListener(this);
     button5->AddActionListener(this);
    currentRate->AddValueListener(this);


  // Some simple operations to show the turtle when starting up
  // This is an entirely random set of instructions
  // Once started up, try typing this into the console:
  // (turtle-star (find-turtle-graphics-widget) 300 23)
  // The 300 argument is the length of the edges of the star, in
  // pixels
  // The 23 argument is the number of points for the star
  
    turtle->RotateInDegrees(45);
    turtle->Forward(200, true);
   turtle->RotateInDegrees(135);
    turtle->Forward(65, true);
    turtle->RotateInDegrees(135);
    turtle->Forward(100, true);
    turtle->RotateInDegrees(150);
    turtle->Forward(100, true);
    //turtle->Paint(Application::)
    currentRate->SetMaximum(turtle->GetNumSteps());
  
   
}

double counter=0;
int rate=100;
void MainPanel::ValueChanged(Component* aC)
{
    if(aC->GetName()== CURRENT_TURTLE_STEP_SLIDER_ID )
    {
      counter=currentRate->GetCurValue();
      turtle->Repaint();
      turtle->GoToStep(counter);
      currentRate->Repaint();
    }
    if(aC->GetName()== PLAYBACK_SLIDER_ID )
    {
        rate=this->playback->GetCurValue();
        turtle->Repaint();
        if(atimer!=NULL)
        {
          atimer->Stop();
          //delete atimer;
          atimer=NULL;
        }
        playback->Repaint();
        atimer = new Timer(XApplication::GetInstance()->GetEventQueue(),100-rate, true,this);
        
    }
    if(aC->GetName() == TURTLE_GRAPHICS_WIDGET_ID)
    {
        turtle->Repaint();
        currentRate->SetMaximum(turtle->GetNumSteps());
    }
    
}

void MainPanel::ActionPerformed(Component* s)
{
    
    
    if(s->GetName()== PLAY_BUTTON_ID )
    {
        if(atimer->IsRunning())
            atimer->Stop();
        else
            atimer->Start();
        
    }
    if(s->GetName()==PREVIOUS_FRAME_BUTTON_ID)
    {
        turtle->GoToStep(--counter);
    }
    if(s->GetName()==GO_TO_START_BUTTON_ID)
    {
        this->atimer->Stop();
        counter=0;
        turtle->GoToStep(--counter);
        currentRate->SetCurValue(counter);
        CurrentTurtleStepPanel->Repaint();
        //test
    }
    if(s->GetName()==NEXT_FRAME_BUTTON_ID)
    {
        turtle->GoToStep(++counter);
    }
    if(s->GetName()==GO_TO_END_BUTTON_ID)
    {
        counter=turtle->GetNumSteps();
        turtle->GoToStep(counter);
        currentRate->SetCurValue(counter);
        CurrentTurtleStepPanel->Repaint();
    }
    //cout<<s->GetBounds().x<<endl;
}




void MainPanel::HandleTimerEvent(const TimerEvent & e) {
    if(turtle->GetNumSteps()>=counter)
    {
      currentRate->SetCurValue(++counter);
     //cout<<counter<<endl;
     turtle->GoToStep(counter);
    }
    else
        atimer->Stop();
}




