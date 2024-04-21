
export ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

PROGRAMS=init

build_programs: 
	-mkdir $(ROOT_DIR)/bin
	for program in $(PROGRAMS); do\
		make -C $$program $(MAKEFLAGS);\
	done

install_programs:
ifndef INSTALL_DESTINATION
	$(error No module destination specified)
endif
	cp --preserve=all $(ROOT_DIR)/bin/* $(INSTALL_DESTINATION)/

clean:
	-rm -rf $(ROOT_DIR)/bin
	for program in $(PROGRAMS); do\
		make -C $$program $(MAKEFLAGS) clean;\
	done