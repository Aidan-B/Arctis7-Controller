OUTPUT = app

BUILD_DIR = build
SRC_DIRS = src
INC_DIRS = include
LIB_DIRS = lib

LINK_FLAGS = -lusb-1.0
VERSION_FLAG = -std=c++20

include ./build/build.mk