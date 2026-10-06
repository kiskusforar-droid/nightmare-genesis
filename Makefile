# SGDK project configuration

SGDK ?= /opt/sgdk

# Allow simple Windows override too
ifeq ($(OS),Windows_NT)
    ifeq ($(SGDK),/opt/sgdk)
        ifneq ($(wildcard C:\sgdk),)
            SGDK := C:\sgdk
        else ifneq ($(wildcard C:\Program Files\sgdk),)
            SGDK := C:\Program Files\sgdk
        endif
    endif
endif

# Project settings
TARGET := nightmare_genesis
SRC_DIR := src
OUT_DIR := out

# Standard SGDK build includes
include $(SGDK)/makefile.gen
