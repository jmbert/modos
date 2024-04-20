
export ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

MODTYPES=drivers

build_modules: 
	for type in $(MODTYPES); do\
		make -C $$type $(MAKEFLAGS);\
	done

install_modules:
ifndef INSTALL_DESTINATION
	$(error No module destination specified)
endif
	cp --preserve=all *.ko $(INSTALL_DESTINATION)/