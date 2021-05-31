OUTPUT = app

BUILD_DIR = build
SRC_DIRS = src
INC_DIRS = include
LIB_DIRS = lib

WIN_INC_DIRS = target/windows/include
WIN_LIB_DIRS = target/windows/lib

LINUX_INC_DIRS = target/linux/include
LINUX_LIB_DIRS = target/linux/lib

LINK_FLAGS = -lusb-1.0 -pthread
VERSION_FLAG = -std=c++20

TARGET = LINUX

include ./build/build.mk