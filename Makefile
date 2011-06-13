LIBSDIR = msim
TESTSDIR = tests

all: lib test

lib:
	cd $(LIBSDIR); $(MAKE)  all

test:
	cd $(TESTSDIR); $(MAKE) all

clean:
	cd $(LIBSDIR); $(MAKE) clean
	cd $(TESTSDIR); $(MAKE) clean

printversion: lib print_msim_version.cpp
	$(CXX) print_msim_version.cpp -o print_msim_version -lmsim -Lmsim/bin
	./print_msim_version

cleanlibs:
	cd $(LIBSDIR); $(MAKE) clean

cleantests:
	cd tests; $(MAKE) clean





