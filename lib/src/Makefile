AIE-RT := ${AIEBU}/lib/aie-rt

CXXFLAGS := -std=c++11 -I${AIE-RT}/driver/build/driver-src/include
LDFLAGS := -L${AIE-RT}/driver/build/driver-src -lxaiengine

preemption.exe : preemption.o
	g++ $^ ${LDFLAGS} -o $@

preemption.o : preemption.cpp
	g++ -c ${CXXFLAGS} -o $@ $<

clean :
	rm -rf *.o *.exe *.bin
