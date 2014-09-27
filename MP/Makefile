.PHONY: all docs clean distclean

# See below for how to add your own source files to the build

# Logging and testing libraries
GLOG_SRC  = glog
GTEST_SRC = gtest

GLOG_DIR  = $(GLOG_SRC)/glog
GTEST_DIR = $(GTEST_SRC)/gtest

GLOG_A       = $(GLOG_DIR)/lib/libglog.a
GTEST_A      = $(GTEST_DIR)/lib/gtest.a
GTEST_MAIN_A = $(GTEST_DIR)/lib/gtest_main.a

# Scheme directory
SCHEME_DIR = scheme/tinyscheme

# Where it puts documentation for the code base when you
# run make docs
DOCS_DIR = docs

# Compile flags
CC = gcc -pedantic -pthread
LD = gcc
DEBUG = -g -Wall -Wno-char-subscripts -Wno-long-long -Wno-variadic-macros -O
SYS_LIBS = -lm -lreadline -lstdc++ -lX11 -lrt $(GLOG_A)
RM = -rm -f
INCLUDE_FLAGS = -I. -I$(GLOG_DIR)/include -I$(GTEST_DIR)/include 

SCHEME_FEATURES = -DUSE_DL=0 -DUSE_MATH=1 -DUSE_ASCII_NAMES=0 -DSTANDALONE=0 -DUSE_INTERFACE=1


# Targets. Builds the logging and testing libraries first, then the
# watgui app and the unittests app
all: $(GLOG_A) $(GTEST_A) $(GTEST_MAIN_A) a2 a2_unittests


# We build the documentation with doxygen
docs:
	doxygen doxygen.cfg

# Common objects (everything except files with main defined, to allow linking
# with a different main function (for example, for unit tests)).
# *** If you add new files, add them here, following the same pattern ***
OBJS =		AffineTransform.o \
		Button.o \
		ComponentEvent.o \
		Component.o \
		Event.o \
		EventQueue.o \
		Graphics.o \
		KeyEvent.o \
		MouseEvent.o \
		PaintEvent.o \
		Panel.o \
		Point.o \
		Rectangle.o \
		SchemeAffineTransform.o \
		SchemeComponent.o \
		SchemeGUI.o \
		SchemeInterpreter.o \
		SchemeUtils.o \
		Slider.o \
		Timer.o \
		TimerEvent.o \
		TimerListener.o \
		TurtleGraphics.o \
		TurtleGraphicsOp.o \
		XApplication.o \
		XWindow.o \
        Label.o

A1_OBJS = 	StopWatch.o

# Objects used for testing
# *** If you add new unit tests, add them here ***
A1_UNIT_TEST_OBJS = 	AffineTransformUnitTests.o \
			XApplicationUnitTests.o \
			EventQueueUnitTests.o \
			A1UnitTests.o \
			MarkingUnitTests.o

A2_OBJS = 		MainPanel.o

# Objects used for testing
# *** If you add new unit tests, add them here ***
A2_UNIT_TEST_OBJS = 	AffineTransformUnitTests.o \
			XApplicationUnitTests.o \
			EventQueueUnitTests.o \
			A2UnitTests.o \
			MarkingUnitTests.o

WATGUI_DEMO_OBJS = demo_main.o

# For a coarse-grained dependency graph 
HEADERS =	$(SCHEME_DIR)/scheme.h \
		$(SCHEME_DIR)/scheme-private.h \
		$(SCHEME_DIR)/opdefines.h \
		$(SCHEME_DIR)/scheme.c \
		A2WidgetIdentifiers.h \
		ActionListener.h \
		AffineTransform.h \
		Button.h \
		ComponentEvent.h \
		Component.h \
		Event.h \
		EventQueue.h \
		Graphics.h \
		KeyEvent.h \
		Logging.h \
		MainPanel.h \
		MouseEvent.h \
		PaintEvent.h \
		Panel.h \
		Point.h \
		Rectangle.h \
		SchemeAffineTransform.h \
		SchemeComponent.h \
		SchemeGUI.h \
		SchemeInterpreter.h \
		SchemeUtils.h \
		Slider.h \
		StopWatch.h \
		Timer.h \
		TimerEvent.h \
		TimerListener.h \
		TurtleGraphics.h \
		TurtleGraphicsOp.h \
		ValueListener.h \
		XApplication.h \
		XWindow.h 
       

$(OBJS): $(HEADERS)
$(A1_OBJS): $(HEADERS)
$(A1_UNIT_TEST_OBJS): $(HEADERS)
$(A2_OBJS): $(HEADERS)
$(A2_UNIT_TEST_OBJS): $(HEADERS)
$(WATGUI_DEMO_OBJS): $(HEADERS)

# The Scheme interpreter needs a set of different flags than the other
# files
SchemeInterpreter.o: SchemeInterpreter.c $(HEADERS)
	$(CC) $(INCLUDE_FLAGS) -c $(DEBUG) $(SCHEME_FEATURES) $<

# Rule to compile the .cpp files
%.o: %.cpp
	$(CC) $(INCLUDE_FLAGS) -c $(DEBUG) $<

# The watgui demo app
watgui_demo: $(OBJS) $(WATGUI_DEMO_OBJS)
	$(CC) -o $@ $(DEBUG) $(OBJS) $(WATGUI_DEMO_OBJS) $(LDFLAGS) $(SYS_LIBS)

a1: $(OBJS) $(A1_OBJS) a1_main.o StopWatch.h
	$(CC) -o $@ $(DEBUG) $(OBJS) $(A1_OBJS) a1_main.o $(LDFLAGS) $(SYS_LIBS)

a2: $(OBJS) $(A2_OBJS) a2_main.o
	$(CC) -o $@ $(DEBUG) $(OBJS) $(A2_OBJS) a2_main.o $(LDFLAGS) $(SYS_LIBS)

# The unittests app
a1_unittests: $(OBJS) $(A1_OBJS) $(A1_UNIT_TEST_OBJS)
	$(CC) -o $@ $(DEBUG) $(OBJS) $(A1_OBJS) $(A1_UNIT_TEST_OBJS) $(GTEST_A) $(GTEST_MAIN_A) $(LDFLAGS) $(SYS_LIBS) 

a2_unittests: $(OBJS) $(A2_OBJS) $(A2_UNIT_TEST_OBJS)
	$(CC) -o $@ $(DEBUG) $(OBJS) $(A2_OBJS) $(A2_UNIT_TEST_OBJS) $(GTEST_A) $(GTEST_MAIN_A) $(LDFLAGS) $(SYS_LIBS) 

# Below are rules to build the logging and testing libraries
$(GLOG_A):
	cd $(GLOG_SRC) && $(MAKE)

$(GTEST_A):
	cd $(GTEST_SRC) && $(MAKE)

$(GTEST_MAIN_A):
	cd $(GTEST_SRC) && $(MAKE)

# To clean all of the app's outputs
clean:
	$(RM) $(OBJS) $(A1_OBJS) $(A1_UNIT_TEST_OBJS) $(A2_OBJS) $(A2_UNIT_TEST_OBJS) $(TEST_OBJS) $(WATGUI_DEMO_OBJS) watgui_demo a1 a1_unittests a1_main.o a2 a2_unittests a2_main.o 

# To clean *everything*, including the docs and test and logging libraries
distclean:
	$(MAKE) clean
	cd $(GTEST_SRC) && $(MAKE) clean
	cd $(GLOG_SRC) && $(MAKE) clean
	cd $(DOCS_DIR) && rm -Rf html latex
