BASE = ezbfcpp
BISON = bison
FLEX = flex
CXX = g++

all: $(BASE)

%.cpp %.hpp: %.yy
	$(BISON) -o $*.cpp $<

%.cpp %.hpp: %.ll
	$(FLEX) -o $*.cpp $<

%.o: %.cpp
	$(CXX) -c -o $@ $<

$(BASE): main.o node.o driver.o parser.o scanner.o
	$(CXX) -o $@ $^

main.o: node.hpp driver.hpp parser.hpp
node.o: node.hpp
driver.o: node.hpp driver.hpp parser.hpp scanner.hpp
parser.o: node.hpp driver.hpp
scanner.o: node.hpp driver.hpp parser.hpp

clean:
	rm -f *.o
	rm -f parser.cpp parser.hpp scanner.cpp scanner.hpp
	rm -f $(BASE)