
SUBDIRS = msim-0.3 tests

all: 
	for D in $(SUBDIRS) ; do \
	  echo "----- Directory "'[ ' $$D/ ' ]' ;\
	  cd $$D ;  make ; \
	  done



