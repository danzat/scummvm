MODULE := engines/neverhood

MODULE_OBJS := \
	detection.o \
	neverhood.o

MODULE_DIRS += \
	engines/neverhood

# This module can be built as a plugin
ifeq ($(ENABLE_NEVERHOOD), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

