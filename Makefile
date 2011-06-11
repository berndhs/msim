LIBSDIR = msim
TESTSDIR = tests

all: lib test

lib:
	cd $(LIBSDIR); make all

test:
	cd $(TESTSDIR); make all

clean:
	cd $(LIBSDIR); make clean
	cd $(TESTSDIR); make clean

cleanlibs:
	cd $(LIBSDIR); make clean

cleantests:
	cd tests; make clean





