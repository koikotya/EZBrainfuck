BISON = bison
FLEX = flex
SRCS = parser.cpp parser.hpp scanner.cpp scanner.hpp

all: $(SRCS)

%.cpp %.hpp: %.yy
	$(BISON) -o $*.cpp $<

%.cpp %.hpp: %.ll
	$(FLEX) -o $*.cpp $<

clean:
	rm -f parser.cpp parser.hpp scanner.cpp scanner.hpp