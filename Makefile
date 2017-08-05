LDFLAGS = `-L/usr/X11/lib`
CXXFLAGS = `-I/usr/X11/inclues/X11/extensions` 
CPP = ccache g++

# NOTE: to profile this program compile it special on the command line like this:
# g++ -pthread -lglut -pg *.cpp
# Then run ./a.out to take a profile.
# gmon a.out > profile.txt generates the profile

# these .cpp, .h, and .o filenames were put here by makeIncludesMake.pl. If you edit them by hand they will be overwritten. Just rerun the script.
SRCS = averager.cpp averager.h baseent.cpp baseent.h baserule.cpp baserule.h biome.cpp biome.h biomeholder.cpp biomeholder.h buildhtml.cpp buildhtml.h camera.cpp camera.h colorshiftr.cpp colorshiftr.h defines.h descriminatoryr.cpp descriminatoryr.h drules.cpp drules.h env.cpp env.h events.cpp events.h fland.cpp fland.h fmlparser.cpp fmlparser.h fmlutils.cpp fmlutils.h glut.h glutenv.cpp glutenv.h groupr.cpp groupr.h main.cpp main.h meshseed.cpp meshseed.h rendervertex.cpp rendervertex.h roughr.cpp roughr.h templates.h tyler.cpp tyler.h vertex.cpp vertex.h 

OBJS = averager.o baseent.o baserule.o biome.o biomeholder.o buildhtml.o camera.o colorshiftr.o descriminatoryr.o drules.o env.o events.o fland.o fmlparser.o fmlutils.o glutenv.o groupr.o main.o meshseed.o rendervertex.o roughr.o tyler.o vertex.o 


all: compile

./%.o: src/%.cpp
	$(CPP) $(CXXFLAGS) -c $< -o $@

compile: $(OBJS)
	$(CPP) -v -framework GLUT -framework OpenGL -lpthread -std=c++11 -o fland $(CXXFLAGS) $(OBJS) $(LDFLAGS)
	#$(CPP)  -lglut -o fland $(CXXFLAGS) $(OBJS) $(LDFLAGS)

run: compile
	./fland

depend:	./makeIncludesMake.pl; g++ -E -MM *.cpp > .depend

