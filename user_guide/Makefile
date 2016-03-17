all:
	clang++ -std=c++11 -O3 \
	    -DVSMC_HAS_MKL \
	    -I ~/Documents/GitHub/vSMC/include \
	    -o pf pf.cpp -lmkl_rt
	clang++ -std=c++11 -O3 \
	    -DVSMC_HAS_MKL \
	    -I ~/Documents/GitHub/vSMC/include \
	    -o program_option program_option.cpp -lmkl_rt
	# ./pf
	# Rscript pf.R
	# latexmk -silent user_guide.tex
