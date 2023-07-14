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
LIB_DIRS_LIST  := /usr/lib/x86_64-linux-gnu ./libs
LIBS_LIST      := glut GL GLU gltools math3d
LIB_DIRS       := $(addprefix -L,$(LIB_DIRS_LIST))
LIBS           := $(addprefix -l,$(LIBS_LIST))
LDFLAGS        += $(LIB_DIRS)
LDLIBS         += $(LIBS)
CXXFLAGS       += $(INC_DIRS)

all: libs $(TARGETS)

$(info LIB_DIRS: '$(LIB_DIRS)')
$(info LIBS: '$(LIBS)')


.PHONY:
clean: clean_libs
	rm -f $(TARGETS)
clean_libs:
	rm libs/*.o libs/*.a


libs: libs/libmath3d.a libs/libgltools.a

libs/libmath3d.a: libs/math3d.o
	ar rcs  $@ $<
libs/math3d.o: libs/math3d.cpp libs/math3d.h
	gcc -c $< -o $@

libs/libgltools.a: libs/gltools.o
	ar rcs  $@ $<
libs/gltools.o: libs/gltools.cpp libs/gltools.h
	gcc -c $< -o $@

