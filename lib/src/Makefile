AIE-RT := ${AIEBU}/lib/aie-rt
AIE_CONTROLLER := ${AIEBU}/lib/aie_controller

CXXFLAGS := -std=c++11 -I${AIE-RT}/driver/build/driver-src/include -I${AIE_CONTROLLER}/include/ps -I${AIE_CONTROLLER}/include
LDFLAGS := -L${AIE-RT}/driver/build/driver-src -lxaiengine

preemption.exe : preemption.o
	g++ $^ ${LDFLAGS} -o $@

preemption.o : preemption.cpp
	g++ -c ${CXXFLAGS} -o $@ $<

clean :
	rm -rf *.o *.exe *.bin
