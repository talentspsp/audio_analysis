CC = g++
EXECEP = extractplca
EXECTAA = testaa
EXECTSTFT = teststft
EXECTPLCA = testplca
EXECTCOMP = testcompweight
SRCSSTFT = teststft.cpp STFT.cpp math_util.h matfile.h matfile.cpp
CPPFILESSTFT = teststft.cpp STFT.cpp matfile.cpp
SRCSPLCA = math_util.h LambertW1.h plca2d.cpp LambertW1.cpp plca2d.h testplca.cpp matfile.cpp matfile.h LambertWnew.h LambertWnew.cpp
CPPFILESPLCA = LambertW1.cpp plca2d.cpp testplca.cpp matfile.cpp LambertWnew.cpp
SRCSCOMP = testcompweight.cpp comp_weight.cpp matfile.cpp math_util.h comp_weight.h matfile.h
CPPFILESCOMP = testcompweight.cpp  comp_weight.cpp matfile.cpp
SRCSAA = STFT.cpp math_util.h LambertW1.h plca2d.cpp LambertW1.cpp plca2d.h comp_weight.cpp matfile.cpp comp_weight.h matfile.h testaudioanalyzer.cpp STFT.h audio_analyzer.h audio_analyzer.cpp
CPPFILESAA = STFT.cpp plca2d.cpp LambertW1.cpp comp_weight.cpp matfile.cpp testaudioanalyzer.cpp audio_analyzer.cpp
SRCSEP = STFT.cpp math_util.h LambertW1.h plca2d.cpp LambertW1.cpp plca2d.h comp_weight.cpp matfile.cpp comp_weight.h matfile.h STFT.h audio_analyzer.h audio_analyzer.cpp extractplca.cpp
CPPFILESEP = STFT.cpp plca2d.cpp LambertW1.cpp comp_weight.cpp matfile.cpp audio_analyzer.cpp extractplca.cpp
LIBDIR = ./spuc
LIB = $(LIBDIR)/libspuc.a
LIBWAVDIR = ./wavproc
LIBWAV = $(LIBWAVDIR)/libwavproc.a
LIBFLAG = -lspuc -lm
LIBFLAGWAV = -lwavproc
HEADERPATH = -I./spuc -I./spuc/generic

$(EXECEP): $(SRCSEP) $(LIB) $(LIBWAV)
	  $(CC) -O2 -o $(EXECEP) $(CPPFILESEP) -L$(LIBDIR) $(LIBFLAG) -L$(LIBWAVDIR) $(LIBFLAGWAV) $(HEADERPATH)

$(EXECTAA): $(SRCSAA) $(LIB) $(LIBWAV)
	  $(CC) -O2 -o $(EXECTAA) $(CPPFILESAA) -L$(LIBDIR) $(LIBFLAG) -L$(LIBWAVDIR) $(LIBFLAGWAV) $(HEADERPATH)

$(EXECTAA)_debug: $(SRCSAA) $(LIB)
		$(CC) -g -ggdb -o $(EXECTAA) $(CPPFILESAA) -L$(LIBDIR) $(LIBFLAG) -L$(LIBWAVDIR) $(LIBFLAGWAV) $(HEADERPATH)

$(EXECTSTFT): $(SRCSSTFT) $(LIB)
	  $(CC) -O2 -o $(EXECTSTFT) $(CPPFILESSTFT) -L$(LIBDIR) $(LIBFLAG) $(HEADERPATH)

$(EXECTSTFT)_debug: $(SRCSSTFT) $(LIB)
		$(CC) -g -ggdb -o $(EXECTSTFT) $(CPPFILESSTFT) -L$(LIBDIR) $(LIBFLAG) $(HEADERPATH)

$(EXECTPLCA): $(SRCSPLCA)
	$(CC) -O2 -o $(EXECTPLCA) $(CPPFILESPLCA)
$(EXECTPLCA)_debug: $(SRCSPLCA)
	$(CC) -g -ggdb -DTEST -o $(EXECTPLCA) $(CPPFILESPLCA)

$(EXECTCOMP): $(SRCSCOMP)
	$(CC) -O2 -o $(EXECTCOMP) $(CPPFILESCOMP)
$(EXECTCOMP)_debug: $(SRCSCOMP)
	$(CC) -g -ggdb -o $(EXECTCOMP) $(CPPFILESCOMP)

clean:
	rm -f *~
	rm -f *#
	rm -f ./$(EXECTSTFT)
	rm -f ./$(EXECTPLCA)
	rm -f ./$(EXECTCOMP)
	rm -f ./$(EXECTAA)