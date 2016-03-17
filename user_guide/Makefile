CXX := clang++
CXXFLAGS := $(CXXFLAGS) -std=c++11 -O3
CXXFLAGS := $(CXXFLAGS) -DVSMC_HAS_MKL
CXXFLAGS := $(CXXFLAGS) -DVSMC_HAS_MKL
CXXFLAGS := $(CXXFLAGS) -I ~/Documents/GitHub/vSMC/include
LDFLAGS := -lmkl_rt

all :

pf: pf.cpp
	$(CXX) $(CXXFLAGS) -o pf pf.cpp $(LDFLAGS)
	./pf
	Rscript pf.R

prog : progress.cpp
	$(CXX) $(CXXFLAGS) -o progress progress.cpp $(LDFLAGS)
	./progress

progopt : program_option.cpp
	$(CXX) $(CXXFLAGS) -o program_option program_option.cpp $(LDFLAGS)
	./program_option --vec 1 2 1e-1 --str "abc" --vec 8 9 --str "def hij" --n 2 4

user_guide : user_guide.tex
	latexmk user_guide.tex
