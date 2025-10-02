CXX := g++
CXXFLAGS := -std=c++11 -O3 -DNDEBUG -I cpp/include
BUILDDIR := build

SRCS := cpp/src/poscode.cpp cpp/src/utils.cpp
OBJS := $(BUILDDIR)/poscode.o $(BUILDDIR)/utils.o

all: dirs test bench

dirs:
	@if not exist $(BUILDDIR) mkdir $(BUILDDIR)

$(BUILDDIR)/poscode.o: cpp/src/poscode.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/utils.o: cpp/src/utils.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(OBJS) cpp/test.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) cpp/test.cpp -o $(BUILDDIR)/test.exe

bench: $(OBJS) cpp/bench.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) cpp/bench.cpp -o $(BUILDDIR)/bench.exe

clean:
	-del /Q $(BUILDDIR)\*.o $(BUILDDIR)\*.exe 2>nul

.PHONY: all dirs clean test bench