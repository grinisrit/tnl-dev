SUFFIXES = .cu
.cu.$(OBJEXT):
	$(NVCC) -c $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $ (CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -o $@ $<