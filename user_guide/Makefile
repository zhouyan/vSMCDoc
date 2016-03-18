CXX := clang++
CXXFLAGS := $(CXXFLAGS) -std=c++11 -O3
CXXFLAGS := $(CXXFLAGS) -DVSMC_HAS_MKL
CXXFLAGS := $(CXXFLAGS) -DVSMC_HAS_MKL
CXXFLAGS := $(CXXFLAGS) -I ~/Documents/GitHub/vSMC/include
LDFLAGS := -lmkl_rt

all : user_guide.pdf

user_guide.pdf : user_guide.tex pf.pdf pf.rout
	latexmk user_guide.tex

pf.pdf pf.rout : pf.cpp pf.hpp pf_*.hpp
	$(CXX) $(CXXFLAGS) -o pf pf.cpp $(LDFLAGS)
	./pf
	Rscript --vanilla pf.R

pf_tbb : pf_tbb.cpp pf_tbb.hpp pf_*_tbb.hpp
	$(CXX) $(CXXFLAGS) -o pf_tbb pf_tbb.cpp $(LDFLAGS)
	./pf_tbb

prog : progress.cpp
	$(CXX) $(CXXFLAGS) -o progress progress.cpp $(LDFLAGS)
	./progress

progopt : program_option.cpp
	$(CXX) $(CXXFLAGS) -o program_option program_option.cpp $(LDFLAGS)
	./program_option --vec 1 2 1e-1 --str "abc" --vec 8 9 --str "def hij" --n 2 4

clean:
	rm pf pf.out pf.pdf pf.rout
	latexmk -c user_guide.tex
