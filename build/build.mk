# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
OUTPUT ?= app

BUILD_DIR ?= build
SRC_DIRS ?= src
INC_DIRS ?= include
LIB_DIRS ?= lib
LINK_FLAGS ?=
VERSION_FLAG ?=

DEBUG_MACROS ?= DEBUG
RELEASE_MACROS ?= NDEBUG

DEBUG_FLAGS = -g -Wall $(addprefix -D,$(DEBUG_MACROS))
RELEASE_FLAGS = -O3 -Wall -s $(addprefix -D,$(RELEASE_MACROS))

VERBOSE ?= FALSE
ifeq ($(VERBOSE),FALSE)
    HIDE = @
else
    HIDE =  
endif

CXX := g++
INC_DIRS += $(LINUX_INC_DIRS)
LIB_DIRS += $(LINUX_LIB_DIRS)

# Find all the C and C++ files we want to compile
SRCS := $(shell find $(SRC_DIRS) -name *.cpp)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))
LIB_FLAGS := $(addprefix -L,$(LIB_DIRS))

DEBUG_OBJS := $(SRCS:%=$(BUILD_DIR)/debug/%.o)
RELEASE_OBJS := $(SRCS:%=$(BUILD_DIR)/release/%.o)

DEBUG_DEPS := $(DEBUG_OBJS:.o=.d)
RELEASE_DEPS := $(RELEASE_OBJS:.o=.d)

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS)  -MMD -MP 
LDFLAGS += $(LINK_FLAGS) $(LIB_FLAGS) $(VERSION_FLAG)


.PHONY: all verbose clean debug release
all: help

clean:
	@echo "Cleaning directory: $(BUILD_DIR)"
	$(HIDE)find $(BUILD_DIR) -type f -not -name build.mk -delete

debug: $(BUILD_DIR)/debug/$(OUTPUT)
	@echo "Building Debug complete"

release: $(BUILD_DIR)/release/$(OUTPUT)
	@echo "Building Release complete"

# Link built objects
$(BUILD_DIR)/debug/$(OUTPUT): $(DEBUG_OBJS)
	@echo "Linking $(DEBUG_OBJS)"
	$(HIDE)$(CXX) $(DEBUG_OBJS) -o $@ $(LDFLAGS)
$(BUILD_DIR)/release/$(OUTPUT): $(RELEASE_OBJS)
	@echo "Linking $(RELEASE_OBJS)"
	$(HIDE)$(CXX) $(RELEASE_OBJS) -o $@ $(LDFLAGS)

# Build C++ source
$(BUILD_DIR)/debug/%.cpp.o: %.cpp
	@echo "Building $< -> $@"
	$(HIDE)mkdir -p $(dir $@)
	$(HIDE)$(CXX) $(CPPFLAGS) $(DEBUG_FLAGS) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/release/%.cpp.o: %.cpp
	@echo "Building $< -> $@"
	$(HIDE)mkdir -p $(dir $@)
	$(HIDE)$(CXX) $(CPPFLAGS) $(RELEASE_FLAGS) $(CXXFLAGS) -c $< -o $@



help: 
	@echo "Usage:"
	@echo "    make [VERBOSE=TRUE] [option]"
	@echo ""
	@echo "Options:"
	@echo "    VERBOSE=TRUE    Display verbose command execution"
	@echo "    help            Display this help text"
	@echo "    release         Build release target"
	@echo "    debug           Build debug target"
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
	@echo "        SRC_DIRS        The list of directories that store source (.cpp) files"
	@echo "                        Default value: src"
	@echo "        INC_DIRS        The list of directories that store header (.h) files"
	@echo "                        Default value: include"
	@echo "        LIB_FLAGS       Flags for libraries to be linked. Empty by default"
	@echo "        DEBUG_MACROS    #define macros defined for debug builds"
	@echo "                        Default value: DEBUG"
	@echo "        RELEASE_MACROS  #define macros defined for release builds"
	@echo "                        Default value: NDEBUG"
	@echo "        VERBOSE         Print verbose command execution."
	@echo "                        Default value: FALSE"


# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEBUG_DEPS)
-include $(RELEASE_DEPS)