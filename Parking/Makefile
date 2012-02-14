LDFLAGS = 
RMFLAGS = -f
INTERFACE = Mere.h
REALISATION = Mere.cpp
OBJ = Mere.o
LIBS = /public/tp/tp-multitache/libtp.a -lncurses -ltcl
EXE = Parking
EFFACE = clean
RM = rm
ECHO = echo
COMP = g++
EDL = g++

$(EXE) : $(OBJ)
	$(ECHO) EDL de $(EXE)
	$(EDL) $(LDFLAGS) $(OBJ) $(LIBS) -o $(EXE)
	
$(EFFACE):
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core
	
%.o : %.cpp
	$(ECHO) Compilation de $<
	$(COMP) $(CCFLAGS) -c  $<
	