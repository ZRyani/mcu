APP_ROOT = $(TOP)/SC7HP75_1
UART_UPGRADER_ROOT = $(TOP)/uart-upgrader
TOOLS = $(TOP)/tools
PACK_ROOT = $(TOOLS)/pack
PACK = $(PACK_ROOT)/pack

$(TARGET): FORCE
	$(Q)rm -rf $(OUTPUT)
	$(Q)mkdir -p $(OUTPUT)
	$(Q)cd $(APP_ROOT) && make clean && make
	$(Q)cd $(PACK_ROOT) && make distclean && make
	$(PACK) sc7hp75_1-layout.xml $@ $(E) && mv $@ $(OUTPUT)/

clean: FORCE
	$(Q)cd $(APP_ROOT) && make clean
	$(Q)cd $(UART_UPGRADER_ROOT) && make clean
	$(Q)cd $(PACK_ROOT) && make distclean

distclean: clean
	$(Q)rm -rf $(OUTPUT)

burn: FORCE
	st-flash write $(OUTPUT)/$(TARGET) 0x08000000