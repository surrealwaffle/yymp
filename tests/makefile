CXX=g++
CXXFLAGS=-std=c++14 -I../include

OBJ=basic_traits.o join.o filter.o setops.o

all: $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: wclean
wclean:
	del /F /Q *.o

.PHONY: uclean
uclean:
	rm *.o
