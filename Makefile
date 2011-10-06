CC = g++
EXECEP = extractplca
EXECTAA = testaa
EXECTSTFT = teststft
EXECTPLCA = testplca
EXECTNEWPLCA = testnewplca
EXECTCOMP = testcompweight
SRCSSTFT = teststft.cpp STFT.cpp math_util.h matfile.h matfile.cpp
CPPFILESSTFT = teststft.cpp STFT.cpp matfile.cpp
SRCSPLCA = math_util.h LambertWs.h plca2d.cpp LambertWs.cpp plca2d.h testplca.cpp matfile.cpp matfile.h LambertWnew.h LambertWnew.cpp
CPPFILESPLCA = LambertWs.cpp plca2d.cpp testplca.cpp matfile.cpp LambertWnew.cpp
SRCSNEWPLCA = fastmath.h fastmath.cpp LambertWs.h LambertWs.cpp plca.h plca_BLAS.cpp testnewplca.cpp matfile.h matfile.cpp
CPPFILESNEWPLCA = LambertWs.cpp plca_BLAS.cpp testnewplca.cpp matfile.cpp
SRCSCOMP = testcompweight.cpp comp_weight.cpp matfile.cpp math_util.h comp_weight.h matfile.h
CPPFILESCOMP = testcompweight.cpp  comp_weight.cpp matfile.cpp
SRCSAA = STFT.cpp math_util.h LambertWs.h plca2d_wrapper.cpp fastmath.h fastmath.cpp LambertWs.cpp plca2d.h comp_weight.cpp matfile.cpp comp_weight.h matfile.h testaudioanalyzer.cpp STFT.h audio_analyzer.h audio_analyzer.cpp plca.h plca.cpp
CPPFILESAA = STFT.cpp plca2d_wrapper.cpp LambertWs.cpp comp_weight.cpp matfile.cpp testaudioanalyzer.cpp audio_analyzer.cpp plca.cpp
SRCSEP = STFT.cpp math_util.h LambertWs.h plca2d.cpp LambertWs.cpp plca2d.h comp_weight.cpp matfile.cpp comp_weight.h matfile.h STFT.h audio_analyzer.h audio_analyzer.cpp extractplca.cpp
CPPFILESEP = STFT.cpp plca2d.cpp LambertWs.cpp comp_weight.cpp matfile.cpp audio_analyzer.cpp extractplca.cpp
LIBDIR = ./spuc
LIB = $(LIBDIR)/libspuc.a
LIBWAVDIR = ./wavproc
LIBWAV = $(LIBWAVDIR)/libwavproc.a
LIBFLAG = -lspuc -lm
LIBFLAGWAV = -lwavproc
HEADERPATH = -I./spuc -I./spuc/generic
BLASLIB = ./CBLAS/lib/cblas_DARWIN.a ./BLAS/blas_DARWIN.a
BLASHEADER = -ICBLAS/src -ICBLAS/include
ALLHEADER = audio_analyzer.h matfile.h plca2d.h LambertWs.h comp_weight.h math_util.h wav_sep.h STFT.h fastmath.h plca.h
ALLCPP = plca.cpp LambertWs.cpp wav_sep.cpp STFT.cpp plca2d_wrapper.cpp audio_analyzer.cpp comp_weight.cpp matfile.cpp
WAVPROCCPP = exit_msg_stdio.cpp wav_in.cpp wav_out.cpp
all: $(ALLHEADER) $(ALLCPP)
	cd $(LIBDIR); make; cd ..;
	cd $(LIBWAVDIR); $(CC) -O2 -c $(WAVPROCCPP); ar -cvr libwavproc.a *.o; cd ..;
	$(CC) -O2 -c $(ALLCPP) $(HEADERPATH)
	ar -x $(LIB);
	ar -x $(LIBWAV);
	ar -cvr libaudio_analyzer.a *.o

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

$(EXECTNEWPLCA): $(SRCSNEWPLCA)
	$(CC) -O2 -o $(EXECTNEWPLCA) $(CPPFILESNEWPLCA)

$(EXECTNEWPLCA)_BLAS: $(SRCSNEWPLCA)
	$(CC) -O3 -DADD_ -o $(EXECTNEWPLCA)_BLAS $(CPPFILESNEWPLCA) $(BLASLIB) $(BLASHEADER)

$(EXECTCOMP): $(SRCSCOMP)
	$(CC) -O2 -o $(EXECTCOMP) $(CPPFILESCOMP)
$(EXECTCOMP)_debug: $(SRCSCOMP)
	$(CC) -g -ggdb -o $(EXECTCOMP) $(CPPFILESCOMP)

compare_matmult: compare_matmult.cpp
	g++ -O3 -DADD_ -o compare_matmult compare_matmult.cpp matfile.cpp -framework veclib
clean:
	rm -f *~
	rm -f *#
	rm -f ./*.o
	rm -f ./libaudio_analyzer.a
	cd $(LIBDIR); make clean; cd ..;
	cd $(LIBWAVDIR); rm -f *.o; rm -f libwavproc.a; cd ..;