CC = g++
EXECT = test
SRCS = main.cpp STFT.cpp math_util.h LambertW1.h
CPPFILES = main.cpp STFT.cpp LambertW1.cpp
LIBDIR = ./spuc
LIB = $(LIBDIR)/libspuc.a
LIBFLAG = -lspuc -lm
HEADERPATH = -I./spuc -I./spuc/generic

$(EXECT): $(SRCS) $(LIB)
	  $(CC) -O2 -o $(EXECT) $(CPPFILES) -L$(LIBDIR) $(LIBFLAG) $(HEADERPATH)

$(EXECT)_debug: $(SRCS) $(LIB)
		$(CC) -g -ggdb -o $(EXECT) $(CPPFILES) -L$(LIBDIR) $(LIBFLAG)

clean:
	rm -f *~
	rm -f *#
	rm -f ./$(EXECT)