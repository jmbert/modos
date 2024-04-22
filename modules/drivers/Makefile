
export ROOT_DIR

DRIVERS=serial_com

all_drivers: 
	for driver in $(DRIVERS); do\
		make -C $(ROOT_DIR)/drivers/$$driver $(MAKEFLAGS);\
	done

clean: 
	for driver in $(DRIVERS); do\
		make -C $(ROOT_DIR)/drivers/$$driver $(MAKEFLAGS) clean;\
	done