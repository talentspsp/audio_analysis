CC = g++
EXECEP = extractplca
EXECETC = extract_timbre_channels
EXECTAA = testaa
EXECTSTFT = teststft
EXECTPLCA = testplca
EXECTNEWPLCA = testnewplca
EXECTPLCABLAS = testplcablas
EXECTPLCADIRICHLET = testplcadirichlet
EXECTPLCADIRICHLETBLAS = testplcadirichletblas
EXECTCOMP = testcompweight
SRCSETC = STFT.cpp math_util.h LambertWs.h plca2d_wrapper.cpp fastmath.h fastmath.cpp LambertWs.cpp plca2d.h comp_weight.cpp matfile.cpp comp_weight.h matfile.h STFT.h audio_analyzer.h audio_analyzer.cpp plca.h plca.cpp plcadirichlet.h plcadirichlet.cpp extract_timbre_channels.cpp lambert_compute_with_offset.h lambert_compute_with_offset.cpp
CPPFILESETC = STFT.cpp plca2d_wrapper.cpp LambertWs.cpp comp_weight.cpp matfile.cpp audio_analyzer.cpp plca.cpp plcadirichlet.cpp extract_timbre_channels.cpp lambert_compute_with_offset.cpp
SRCSSTFT = teststft.cpp STFT.cpp math_util.h matfile.h matfile.cpp
CPPFILESSTFT = teststft.cpp STFT.cpp matfile.cpp
SRCSPLCA = math_util.h LambertWs.h plca2d.cpp LambertWs.cpp plca2d.h testplca.cpp matfile.cpp matfile.h LambertWnew.h LambertWnew.cpp
CPPFILESPLCA = LambertWs.cpp plca2d.cpp testplca.cpp matfile.cpp LambertWnew.cpp
SRCSNEWPLCA = fastmath.h fastmath.cpp LambertWs.h LambertWs.cpp plca.h testnewplca.cpp matfile.h matfile.cpp plca.cpp lambert_compute_with_offset.h lambert_compute_with_offset.cpp
CPPFILESNEWPLCA = LambertWs.cpp testnewplca.cpp matfile.cpp plca.cpp lambert_compute_with_offset.cpp
SRCSPLCADIRICHLET = fastmath.h fastmath.cpp LambertWs.h LambertWs.cpp plcadirichlet.h testplcadirichlet.cpp matfile.h matfile.cpp plcadirichlet.cpp lambert_compute_with_offset.h lambert_compute_with_offset.cpp
CPPFILESPLCADIRICHLET = LambertWs.cpp testplcadirichlet.cpp matfile.cpp plcadirichlet.cpp lambert_compute_with_offset.cpp
SRCSPLCADIRICHLETBLAS = fastmath.h fastmath.cpp LambertWs.h LambertWs.cpp plcadirichlet.h testplcadirichletblas.cpp matfile.h matfile.cpp plcadirichlet_BLAS.cpp lambert_compute_with_offset.h lambert_compute_with_offset.cpp
CPPFILESPLCADIRICHLETBLAS = LambertWs.cpp testplcadirichletblas.cpp matfile.cpp plcadirichlet_BLAS.cpp lambert_compute_with_offset.cpp
SRCSPLCABLAS = fastmath.h fastmath.cpp LambertWs.h LambertWs.cpp plca.h testplcablas.cpp matfile.h matfile.cpp plca_BLAS.cpp lambert_compute_with_offset.h lambert_compute_with_offset.cpp
CPPFILESPLCABLAS = LambertWs.cpp testplcablas.cpp matfile.cpp plca_BLAS.cpp lambert_compute_with_offset.cpp
SRCSCOMP = testcompweight.cpp comp_weight.cpp matfile.cpp math_util.h comp_weight.h matfile.h
CPPFILESCOMP = testcompweight.cpp  comp_weight.cpp matfile.cpp
SRCSAA = STFT.cpp math_util.h LambertWs.h plca2d_wrapper.cpp fastmath.h fastmath.cpp LambertWs.cpp plca2d.h comp_weight.cpp matfile.cpp comp_weight.h matfile.h testaudioanalyzer.cpp STFT.h audio_analyzer.h audio_analyzer.cpp plca.h plca.cpp lambert_compute_with_offset.h lambert_compute_with_offset.cpp
CPPFILESAA = STFT.cpp plca2d_wrapper.cpp LambertWs.cpp comp_weight.cpp matfile.cpp testaudioanalyzer.cpp audio_analyzer.cpp plca.cpp lambert_compute_with_offset.cpp
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
ALLHEADER = audio_analyzer.h matfile.h plca2d.h LambertWs.h comp_weight.h math_util.h wav_sep.h STFT.h fastmath.h plca.h plcadirichlet.h lambert_compute_with_offset.h
ALLCPP = plca.cpp LambertWs.cpp wav_sep.cpp STFT.cpp plca2d_wrapper.cpp audio_analyzer.cpp comp_weight.cpp matfile.cpp plcadirichlet.cpp lambert_compute_with_offset.cpp
WAVPROCCPP = exit_msg_stdio.cpp wav_in.cpp wav_out.cpp
all: $(ALLHEADER) $(ALLCPP)
	cd $(LIBDIR); make; cd ..;
	cd $(LIBWAVDIR); $(CC) -O2 -c $(WAVPROCCPP); ar -cvr libwavproc.a *.o; cd ..;
	$(CC) -O2 -c $(ALLCPP) $(HEADERPATH)
	ar -x $(LIB);
	ar -x $(LIBWAV);
	ar -cvr libaudio_analyzer.a *.o
	$(CC) -O2 -o $(EXECTAA) $(CPPFILESAA) -L. -laudio_analyzer $(HEADERPATH)

$(EXECETC): $(SRCSETC) $(LIB) $(LIBWAV)
	 $(CC) -O2 -o $(EXECETC) $(CPPFILESETC) -L$(LIBDIR) $(LIBFLAG) -L$(LIBWAVDIR) $(LIBFLAGWAV) $(HEADERPATH)
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
$(EXECTPLCADIRICHLET): $(SRCSPLCADIRICHLET)
	$(CC) -O2 -o $(EXECTPLCADIRICHLET) $(CPPFILESPLCADIRICHLET)

$(EXECTPLCABLAS): $(SRCSPLCABLAS)
	$(CC) -O3 -DADD_ -o $(EXECTPLCABLAS) $(CPPFILESPLCABLAS) $(BLASLIB) $(BLASHEADER)
$(EXECTPLCADIRICHLETBLAS): $(SRCSPLCADIRICHLETBLAS)
	$(CC) -O3 -DADD_ -o $(EXECTPLCADIRICHLETBLAS) $(CPPFILESPLCADIRICHLETBLAS) $(BLASLIB) $(BLASHEADER)

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
	rm -f ./$(EXECTAA)
	cd $(LIBDIR); make clean; cd ..;
	cd $(LIBWAVDIR); rm -f *.o; rm -f libwavproc.a; cd ..;