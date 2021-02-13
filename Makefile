CXX      = g++-6
CXXFLAGS = -Wall -g -std=c++11
LDFLAGS  =
LDLIBS   =
SOURCES  = $(wildcard *.cpp)
HEADERS  = $(wildcard *.h)
OBJS     = $(patsubst %.cpp,%.o,$(SOURCES))
BINARY   = main


# Making $(BINARY) phony enforces recompilation every time.
.PHONY: all run clean test

all: $(BINARY)

run: $(BINARY)
	./$(BINARY)

# Making any object file depend on all header files
# means that everything must be recompiled if a header file
# is updated. It is not that elegant but it is simple.
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BINARY): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(CXXFLAGS) $(LDLIBS) -o $(BINARY)

clean:
	$(RM) $(OBJS) $(BINARY)
