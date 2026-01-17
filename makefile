BUILD_DIR := build
INC_DIRS := include
LINK_FLAGS := -lusb-1.0

SUCCESS_MESSAGE = @printf "`tput bold``tput setaf 2`BUILD SUCCESS`tput sgr0`\n"

VERBOSE ?= FALSE
RELEASE ?= FALSE

export
cli: OUTPUT := arctis_cli
cli:
	@$(MAKE) $(BUILD_DIR)/$(OUTPUT) -f ./build/build.mk \
		SRCS="src/cli.cpp src/NewHeadset.cpp" \
		OUTPUT=$(OUTPUT) \
		VERBOSE=$(VERBOSE) \
		RELEASE=$(RELEASE)
	$(SUCCESS_MESSAGE)

daemon: OUTPUT := arctis_daemon
daemon:
	@$(MAKE) $(BUILD_DIR)/$(OUTPUT) -f ./build/build.mk \
		SRCS="src/main.cpp src/NewHeadset.cpp" \
		OUTPUT=$(OUTPUT) \
		VERBOSE=$(VERBOSE) \
		RELEASE=$(RELEASE)
	$(SUCCESS_MESSAGE)

.PHONY: clean
clean:
	@echo "Cleaning directory: $(BUILD_DIR)"
	@find $(BUILD_DIR) -type f -not -name build.mk -delete
