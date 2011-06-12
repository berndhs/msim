LIBSDIR = msim
TESTSDIR = tests

all: lib test

lib:
	cd $(LIBSDIR); make  all

test:
	cd $(TESTSDIR); make all

clean:
	cd $(LIBSDIR); make clean
	cd $(TESTSDIR); make clean

printversion: lib print_msim_version.cpp
	$(CXX) print_msim_version.cpp -o print_msim_version -lmsim -Lmsim/bin
	./print_msim_version

cleanlibs:
	cd $(LIBSDIR); make clean

cleantests:
	cd tests; make clean





