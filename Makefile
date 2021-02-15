// (c) 2005 twak
CC = g++
#LIB_DIRS=-L/lib -L/usr/local/lib 
// unix
#LDLIBS=-lfltk -lfltk_gl -lGL -lGLU -lm -lX11 
// windows
LDLIBS= -lfltk_gl -lfltk -lglut32 -lglu32 -lopengl32 -lgdi32 -lwsock32 
#CFLAGS=-I. -I/usr/include -lglut32 -lglu32 -lopengl32
CFLAGS = -ansi -W -Wall -pedantic

#LDLIBS= -lfltk_gl -lglu32 -lglut32 -lopengl32 -lfltk 
#-lole32 -luuid -lcomct132 -lwsock32 -lsupc++
#-lfltk_gl -lfltk -libglut32 -lglu32 -lopengl32 -lgdi32 -lwsock32 

OBJECTS= RobotMain.o Robot.o RobotUI.o root.o point.o joint.o leg.o rotate.o arm.o body.o head.o wheel.o handleBar.o bike.o sched.o moon.o

all: Robot

#/opt/local/glut-3.7/lib/glut/libglut.a 

Robot: $(OBJECTS)
	${CC} $(OBJECTS) ${LIB_DIRS} ${LDLIBS} -o Robot

RobotMain.o: RobotMain.cxx include.h
	${CC} ${CFLAGS} -c RobotMain.cxx

Robot.o: Robot.cxx Robot.h joint.o point.o root.o body.o handleBar.o bike.o stars.o sched.o moon.o include.h
	${CC} ${CFLAGS} -c Robot.cxx

RobotUI.o: Robot.cxx RobotUI.h include.h
	${CC} ${CFLAGS} -c RobotUI.cxx 

joint.o: joint.cxx joint.h point.h rotate.h include.h
	${CC} ${CFLAGS} -c joint.cxx

point.o: point.cxx point.h include.h
	${CC} ${CFLAGS} -c point.cxx

rotate.o: rotate.cxx rotate.h include.h
	${CC} ${CFLAGS} -c rotate.cxx

root.o: root.cxx root.h  joint.h point.h rotate.h point.o include.h
	${CC} ${CFLAGS} -c root.cxx

leg.o: leg.cxx leg.h joint.h point.h rotate.o piston.o include.h
	${CC} ${CFLAGS} -c leg.cxx

arm.o: arm.cxx arm.h joint.h point.h rotate.o piston.o include.h
	${CC} ${CFLAGS} -c arm.cxx

head.o: head.cxx head.h joint.h point.h rotate.o piston.o include.h
	${CC} ${CFLAGS} -c head.cxx

piston.o: piston.cxx piston.h point.o include.h
	${CC} ${CFLAGS} -c piston.cxx

body.o: body.cxx body.h point.o joint.o point.o root.o leg.o arm.o head.o include.h
	${CC} ${CFLAGS} -c body.cxx 

wheel.o: wheel.cxx wheel.h include.h
	${CC} ${CFLAGS} -c wheel.cxx 

handleBar.o: handleBar.h handleBar.cxx wheel.o include.h
	${CC} ${CFLAGS} -c handleBar.cxx 

bike.o: bike.h bike.cxx wheel.o point.o include.h
	${CC} ${CFLAGS} -c bike.cxx 

stars.o: stars.h stars.cxx point.o include.h
	${CC} ${CFLAGS} -c stars.cxx 

sched.o: sched.h sched.cxx point.o include.h
	${CC} ${CFLAGS} -c sched.cxx 

moon.o: moon.h moon.cxx point.o include.h
	${CC} ${CFLAGS} -c moon.cxx 

clean:
	-rm *.o Robot

