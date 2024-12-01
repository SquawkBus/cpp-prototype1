

ROOT_DIR=../..
SRC_DIR=${ROOT_DIR}/src
EXT_DIR=${ROOT_DIR}/externals

CXX = clang++
INCLUDES = -I${SRC_DIR} -I${EXT_DIR} -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib
CXXFLAGS = -g -std=c++23 -Wall ${INCLUDES}
LDLIBS = ${LIBS} -lyaml-cpp -lssl -lcrypto -luuid

# Makefile settings - Can be customized.
APPNAME = squawkbus
OBJDIR = obj
BINDIR = bin

############## Do not change anything from here downwards! #############
SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=$(OBJDIR)/%.d)
APP = $(BINDIR)/$(APPNAME)

$(OBJDIR)/%.d: %.cpp
	$(CPP) $(CXXFLAGS) $< -MM -MT $(@:%.d=%.o) > $@

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: all
all: $(APP)

$(APP): $(OBJ)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(DEP) $(APP)

-include $(DEP)
