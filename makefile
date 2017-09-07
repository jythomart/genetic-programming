TARGET = gp
CDIR=./src
IDIR =./include
ODIR=obj
LDIR =./lib

CC=gcc
CFLAGS=-I$(IDIR) -Wall -Werror

LIBS=-lm -lpthread

_DEPS = edge.h feature_parser.h arg_helper.h node.h operation.h population.h test.h tree_generator.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o edge.o feature_parser.o arg_helper.o node.o operation.o population.o test.o tree_generator.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_SRC = %.c
SRC = $(patsubst %,$(CDIR)/%,$(_SRC))

default: $(TARGET)
all: default

$(ODIR)/%.o: $(SRC) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gp: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 