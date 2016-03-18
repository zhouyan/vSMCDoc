CXX := clang++
CXXFLAGS := $(CXXFLAGS) -std=c++11 -O3
CXXFLAGS := $(CXXFLAGS) -DVSMC_HAS_MKL
CXXFLAGS := $(CXXFLAGS) -DVSMC_HAS_MKL
CXXFLAGS := $(CXXFLAGS) -I ~/Documents/GitHub/vSMC/include
LDFLAGS := -lmkl_rt

all : pf pf_tbb progress program_option

pf : pf.cpp pf.hpp pf_*.hpp
	$(CXX) $(CXXFLAGS) -o pf pf.cpp $(LDFLAGS)

pf_tbb : pf_tbb.cpp pf_tbb.hpp pf_*_tbb.hpp
	$(CXX) $(CXXFLAGS) -o pf_tbb pf_tbb.cpp $(LDFLAGS)

program_option : program_option.cpp
	$(CXX) $(CXXFLAGS) -o program_option program_option.cpp $(LDFLAGS)

progress : program_option.cpp
	$(CXX) $(CXXFLAGS) -o progress progress.cpp $(LDFLAGS)

clean:
	rm -f pf pf_tbb pf.out pf.pdf pf.rout
	rm -f program_option
	rm -f progress
