LIBSDIR = msim-0.3
TESTSDIR = tests-0.3

all: libs tests

libs:
	cd $(LIBSDIR); make all

tests:
	cd $(TESTSDIR); make all

clean:
	cd $(LIBSDIR); make clean
	cd $(TESTSDIR); make clean

cleanlibs:
	cd $(LIBSDIR); make clean

cleantests:
	cd tests; make clean





