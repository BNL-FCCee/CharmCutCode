# Makefile
CPP = g++

OBJDIR = ../build/obj
BINDIR = ../build

ifeq (${BUILD_STRUC},MASS)
OBJDIR = ../../build/obj/wscanner/
BINDIR = ../../build
endif

SOURCEDIR = Root
HEADERDIR = WScanner
UTILSDIR  = utils

INC_PARAMS=$(foreach d, $(HEADERDIR) $(SOURCEDIR)/atlasstyle, -I$d)

ROOTCXXFLAGS  = $(shell root-config --cflags)
CPPFLAGS = -Wall -c -std=c++11 -O2 $(ROOTCXXFLAGS) $(INC_PARAMS)
LDFLAGS = 
ROOTLIBS   = $(shell root-config --libs) -lRooFitCore -lRooFit -lRooStats -lCore -lTMVA
LDLIBS =  $(ROOTLIBS)

SOURCES := $(shell find ${SOURCEDIR} -name '*.cxx')
UTILS   := $(shell find ${UTILSDIR} -name '*.cxx')

LIBRARY := $(patsubst ${SOURCEDIR}/%.cxx, $(OBJDIR)/%.o, $(SOURCES))
OBJECTS := $(patsubst ${UTILSDIR}/%.cxx, $(BINDIR)/%, $(UTILS))

default: build ${OBJECTS}

.PRECIOUS: ${OBJDIR}/%.o

build:
	@mkdir -p ${OBJDIR}
	@mkdir -p ${BINDIR}

${BINDIR}/%: ${OBJDIR}/%.o ${LIBRARY}
	${CPP} -o $@ $< ${LIBRARY} ${LDLIBS}

${OBJDIR}/%.o: ${SOURCEDIR}/%.cxx
	@echo "Making $(@F)";
	${CPP} ${CPPFLAGS} -c -o $@ $<

${OBJDIR}/%.o: ${UTILSDIR}/%.cxx
	@echo "Making $(@F)";
	${CPP} ${CPPFLAGS} -c -o $@ $<

clean:
	rm -rf ${OBJDIR}/*.o *.o ${BINDIR}/*
