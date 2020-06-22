.PHONY: clean all

APP_AUX=libaux
LIB_AUX=aux
OBJ_AUX=$(LIB_AUX).o
SRC_AUX=$(LIB_AUX).c
AUX=$(APP_AUX).a
INCLUDE_AUX=../include/aux.h

CC=gcc
MAKELIB=ar rv
RANLIB=ranlib

all: $(AUX) clean

$(AUX): $(OBJ_AUX)
		$(MAKELIB) $(AUX) $(OBJ_AUX) && $(RANLIB) $(AUX)

$(OBJ_AUX): $(SRC_AUX) $(INCLUDE_AUX)
		$(CC) -c $(SRC_AUX)

clean:
		rm  *.o