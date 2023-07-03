SRC_DIR   = .

define find_cppfiles
    $(notdir $(shell find $1 -maxdepth 1 -type f -iname '*.cpp'))
endef

SRC_FILES = $(call find_cppfiles,$(SRC_DIR))
TARGETS   = $(notdir $(subst .cpp,,$(SRC_FILES)))

$(info SRC_FILES: '$(SRC_FILES)')
$(info TARGETS: '$(TARGETS)')

LIB_DIRS_LIST  := /usr/lib/x86_64-linux-gnu
LIBS_LIST      := glut GL
LIB_DIRS       := $(addprefix -L,$(LIB_DIRS_LIST))
LIBS           := $(addprefix -l,$(LIBS_LIST))
LDFLAGS        += $(LIBS_DIR)
LDLIBS         += $(LIBS)

all: $(TARGETS)

$(info LIB_DIRS: '$(LIB_DIRS)')
$(info LIBS: '$(LIBS)')


.PHONY:
clean:
	rm -f $(TARGETS)

