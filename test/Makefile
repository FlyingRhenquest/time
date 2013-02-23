CPPUNIT_CONTRIB=../../cppunit_contrib
OBJS = gmst_test.o jd_test.o run_tests.o string_converts_test.o
EXE = run_tests
CFLAGS += -g --std=c++11 -I.. -I${CPPUNIT_CONTRIB}
LFLAGS = -lcppunit

.cpp.o:
	g++ -c ${CFLAGS} $<

all: ${OBJS}
	g++ -o ${EXE} ${OBJS} ${LFLAGS}

clean:
	rm -f *~ ${EXE} ${OBJS} core
