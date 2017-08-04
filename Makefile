LDFLAGS = ``
CXXFLAGS = `` 
CPP = ccache g++

# NOTE: to profile this program compile it special on the command line like this:
# g++ -pthread -lglut -pg *.cpp
# Then run ./a.out to take a profile.
# gmon a.out > profile.txt generates the profile

# these .cpp, .h, and .o filenames were put here by makeIncludesMake.pl. If you edit them by hand they will be overwritten. Just rerun the script.
SRCS = baseent.cpp baseent.h baseobj.cpp baseobj.h baserule.cpp baserule.h buildhtml.cpp buildhtml.h camera.cpp camera.h colorclass.cpp colorclass.h defines.h drules.cpp drules.h env.cpp env.h events.cpp events.h fland.cpp fland.h fmlparser.cpp fmlparser.h fmlutils.cpp fmlutils.h glutenv.cpp glutenv.h main.cpp main.h meshseed.cpp meshseed.h monor.cpp monor.h perlinnoise.cpp perlinnoise.h rendervertex.cpp rendervertex.h templates.h test.cpp tyler.cpp tyler.h vertex.cpp vertex.h 

OBJS = baseent.o baseobj.o baserule.o buildhtml.o camera.o colorclass.o drules.o env.o events.o fland.o fmlparser.o fmlutils.o glutenv.o main.o meshseed.o monor.o perlinnoise.o rendervertex.o test.o tyler.o vertex.o 


all: compile

./%.o: src/%.cpp
	$(CPP) $(CXXFLAGS) -c $< -o $@

compile: $(OBJS)
	$(CPP)  -pthread -lglut -o fland $(CXXFLAGS) $(OBJS) $(LDFLAGS)


run: compile
	./fland

depend:
	./makeIncludesMake.pl; g++ -E -MM *.cpp > .depend

include .depend
