LDFLAGS = 
RMFLAGS = -f
CCFLAGS = 
INTERFACE = Mere.h Clavier.h Sortie.h Porte.h
REALISATION = Mere.cpp Clavier.cpp Sortie.cpp Porte.cpp
OBJ = Mere.o Clavier.o Sortie.o Porte.o
LIBS = -ltp -lncurses -ltcl
DIRHEAD = -I/public/tp/tp-multitache/
DIRLIB = -L/public/tp/tp-multitache/
EXE = Parking
EFFACE = clean
RM = rm
ECHO = echo
COMP = g++
EDL = g++

$(EXE) : $(OBJ)
	$(ECHO) EDL de $(EXE)
	$(EDL) $(LDFLAGS) $(OBJ) $(DIRLIB) $(LIBS) -o $(EXE)
	
$(EFFACE):
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core
	
%.o : %.cpp
	$(ECHO) Compilation de $<
	$(COMP) $(CCFLAGS) $(DIRHEAD) -c  $<
	
