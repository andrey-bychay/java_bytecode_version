#CXXFLAGS=-g

vpath %.cpp src

.PHONY: all clean build dependencies run
all: clean build dependencies run

clean:
	-rm bin/* 2>/dev/null

build: bin/jbc

dependencies: build
	otool -L bin/jbc

run:
	bin/jbc test/*.class

bin/%: src/%.cpp
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

