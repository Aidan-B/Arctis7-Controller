# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
OUTPUT ?= app

BUILD_DIR ?= build
SRCS ?= src/main.cpp
INC_DIRS ?= include
LIB_DIRS ?=
LINK_FLAGS ?=
CPP_VERSION ?= -std=c++20
CXX ?= g++

VERBOSE ?= FALSE
ifeq ($(VERBOSE),FALSE)
    HIDE = @
else
    HIDE =
endif

INC_FLAGS := $(addprefix -I,$(INC_DIRS))
LIB_FLAGS := $(addprefix -L,$(LIB_DIRS))

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

DEBUG_MACROS ?= DEBUG
RELEASE ?= FALSE
ifeq ($(RELEASE),FALSE)
	CXXFLAGS += -g -Wall $(addprefix -D, $(DEBUG_MACROS))
else
	CXXFLAGS += -O2 -Wall -s
endif

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CXXFLAGS += $(INC_FLAGS) -MMD -MP
LDFLAGS += $(LINK_FLAGS) $(LIB_FLAGS) $(CPP_VERSION)

.PHONY: help
help:
	@echo "Usage:"
	@echo "    make [VERBOSE=TRUE] [RELEASE=TRUE] [target]"
	@echo ""
	@echo "Options:"
	@echo "    VERBOSE=TRUE    Display verbose command execution"
	@echo "    RELEASE=TRUE    Compile a release build"
	@echo "    help            Display this help text"
	@echo ""
	@echo "Makefile Options:"
	@echo ""
	@echo "    The makefile describes options for compiling. They are represented in the"
	@echo "    form ATTRIBUTE = value value."
	@echo ""
	@echo "    Attributes:"
	@echo "        OUTPUT          The name of the executable that is generated."
	@echo "                        Default value: app"
	@echo "        BUILD_DIR       The directory for build objects and files relative to"
	@echo "                        the makefile"
	@echo "                        Default value: build"
	@echo "        SRCS            The list of source files are to be compiled into an application"
	@echo "                        Default value: src/main.cpp"
	@echo "        INC_DIRS        The list of directories that store header (.h) files"
	@echo "                        Default value: include"
	@echo "        LIB_FLAGS       Flags for libraries to be linked. Empty by default"
	@echo "        CPP_VERSION     The C++ version to compile with."
	@echo "                        Default value: -std=c++20"
	@echo "        DEBUG_MACROS    #define macros defined for debug builds"
	@echo "                        Default value: DEBUG"
	@echo "        VERBOSE         Print verbose command execution."
	@echo "                        Default value: FALSE"

# Link built objects
$(BUILD_DIR)/$(OUTPUT): $(OBJS)
	@echo "Linking $(OBJS)"
	$(HIDE)$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo "Building $< -> $@"
	$(HIDE)mkdir -p $(dir $@)
	$(HIDE)$(CXX) $(CPPFLAGS) $(DEBUG_FLAGS) $(CXXFLAGS) -c $< -o $@

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
