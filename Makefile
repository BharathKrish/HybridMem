CXX= gcc
CXXFLAGS=
LDFLAGS =  
CXXFLAGS_THREAD= -lpthread

MPCC          = /usr/bin/mpiCC
OPTFLAGS    = -L/usr/lib64/openmpi 

all: ocean_multi

ocean_multi : ocean_multi.c hwtimer.h 
	$(CXX)  ocean_multi.c -o ocean_multi $(CXXFLAGS_THREAD)

clean:
	rm -f ocean_multi
