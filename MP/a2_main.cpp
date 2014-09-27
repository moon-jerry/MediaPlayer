#include <string>
#include "XApplication.h"
#include "XWindow.h"
#include "SchemeInterpreter.h"
#include "Logging.h"
#include "MainPanel.h"

using namespace std;
using namespace cs349;

const char* scheme_init_fname = "init.scm";
const char* cs349_init_fname  = "cs349_init.scm";

int main (int argc, char *argv[])
{
  google::InitGoogleLogging(argv[0]);

  //  FLAGS_log_dir = "./logs";
  FLAGS_logtostderr = 1;
  LOG_DEBUG << "Starting up a2_main.cpp";

  XApplication* app = XApplication::GetInstance();
  scheme_interpreter_init(scheme_init_fname, cs349_init_fname, argc, argv);

  XWindow* window = new XWindow("University-Age Mutant Turtle Interfaces!", Rectangle(500, 500, 500, 500));
  MainPanel* mainPanel = new MainPanel();

  // Get the main panel's preferred size
  Rectangle mainPanelBounds = mainPanel->GetBounds();

  // Set the window size to match the main panel's size
  // Important! Make sure you set the size of MainPanel to be what you
  // want it to be in its constructor!
  window->SetSize(mainPanelBounds.width, mainPanelBounds.height);

  window->AddComponent(mainPanel);

  mainPanel->SetVisible(true);
  window->SetVisible(true);
  
  app->Run();
	
  // The code has been changed so that XApplication's destructor
  // automatically deletes all open windows, so all you need to do is
  // delete the XApplication object
  delete app;
  return scheme_interpreter_shutdown();
}
