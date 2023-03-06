CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++11 -I./src
SRCDIR=./src
BUILDDIR=./build

MAIN:=$(SRCDIR)/Application.cpp $(SRCDIR)/CMD.cpp
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(filter-out $(MAIN),$(SRCS)))

.PHONY: all clean

all: $(BUILDDIR)/rtcapp $(BUILDDIR)/rtccmd

rtcapp: $(BUILDDIR)/rtcapp
$(BUILDDIR)/rtcapp: $(OBJS) $(SRCDIR)/Application.o
	$(CXX) $(CXXFLAGS) $^ -o $@

rtccmd: $(BUILDDIR)/rtccmd
$(BUILDDIR)/rtccmd: $(OBJS) $(SRCDIR)/CMD.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/*.h | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)
