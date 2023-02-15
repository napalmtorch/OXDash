CFLAGS += -Iinclude -O0 -Wno-incompatible-pointer-types -DLODEPNG_NO_COMPILE_CPP

XBE_TITLE = OXDash
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(shell find src -type f -iname '*.c')
NXDK_DIR ?= $(CURDIR)/../../nxdk

NXDK_SDL = y
NXDK_DISABLE_AUTOMOUNT_D = y

include $(NXDK_DIR)/Makefile

TARGET += $(OUTPUT_DIR)/config.ini
$(GEN_XISO): $(OUTPUT_DIR)/config.ini
$(OUTPUT_DIR)/config.ini: $(CURDIR)/assets/config.ini $(OUTPUT_DIR)
	$(VE)cp '$<' '$@'

TARGET += $(OUTPUT_DIR)/bg.bmp
$(GEN_XISO): $(OUTPUT_DIR)/bg.bmp
$(OUTPUT_DIR)/bg.bmp: $(CURDIR)/assets/bg.bmp $(OUTPUT_DIR)
	$(VE)cp '$<' '$@'

TARGET += $(OUTPUT_DIR)/test.png
$(GEN_XISO): $(OUTPUT_DIR)/test.png
$(OUTPUT_DIR)/test.png: $(CURDIR)/assets/test.png $(OUTPUT_DIR)
	$(VE)cp '$<' '$@'