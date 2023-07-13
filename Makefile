SRC_DIR   = .

define find_cppfiles
    $(notdir $(shell find $1 -maxdepth 1 -type f -iname '*.cpp'))
endef

SRC_FILES = $(call find_cppfiles,$(SRC_DIR))
TARGETS   = $(notdir $(subst .cpp,,$(SRC_FILES)))

$(info SRC_FILES: '$(SRC_FILES)')
$(info TARGETS: '$(TARGETS)')

INC_DIRS_LIST  := /usr/include /usr/include/x86_64-linux-gnu
INC_DIRS       := $(addprefix -I,$(INC_DIRS_LIST))
LIB_DIRS_LIST  := /usr/lib/x86_64-linux-gnu
LIBS_LIST      := glut GL GLU
LIB_DIRS       := $(addprefix -L,$(LIB_DIRS_LIST))
LIBS           := $(addprefix -l,$(LIBS_LIST))
LDFLAGS        += $(LIB_DIRS)
LDLIBS         += $(LIBS)
CXXFLAGS       += $(INC_DIRS)

all: $(TARGETS)

$(info LIB_DIRS: '$(LIB_DIRS)')
$(info LIBS: '$(LIBS)')


.PHONY:
clean:
	rm -f $(TARGETS)

