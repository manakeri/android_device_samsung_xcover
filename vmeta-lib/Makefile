#
# Makefile for 2D graphics user library
#

ifeq ($(MAKERULES),)
include ../../Rules.make
else
include $(MAKERULES)
endif

LIBPMEM = -lphycontmem

CFLAGS += -I$(PXA_SRC_PVK_DIR)/phycontmem-lib/phycontmem/ -L$(PXA_SRC_PVK_DIR)/phycontmem-lib/phycontmem
LDLIBS += $(LIBPMEM) -lrt

.PHONY: all compile install-host install-target clean clean-local \
	uninstall-host uninstall-target

all: compile install-host install-target 

compile: vmeta_lib.o
	$(CC) $(CFLAGS) $(LDLIBS) -shared -o libvmeta.so  vmeta_lib.o
	${AR} -rcs libvmeta.a vmeta_lib.o

install-host:
	cp -f libvmeta.so $(PXA_HOST_LIB_DIR)
	cp -f libvmeta.a $(PXA_HOST_LIB_DIR)
	cp -f vmeta_lib.h $(PXA_HOST_INCLUDE_DIR)
install-target:
	$(SUDO) cp -f libvmeta.so $(PXA_TARGET_LIB_DIR)
	$(SUDO) cp -f libvmeta.a $(PXA_TARGET_LIB_DIR)

clean: clean-local uninstall-host uninstall-target

clean-local:
	-rm  -f *.o *.so *.a

uninstall-host:
	-rm -f $(PXA_HOST_LIB_DIR)/libvmeta.so
	-rm -f $(PXA_HOST_LIB_DIR)/libvmeta.a
	-rm -f $(PXA_HOST_INCLUDE_DIR)/vmeta_lib.h
uninstall-target:
	$(SUDO) rm -f $(PXA_TARGET_LIB_DIR)/libvmeta.so
	$(SUDO) rm -f $(PXA_TARGET_LIB_DIR)/libvmeta.a
