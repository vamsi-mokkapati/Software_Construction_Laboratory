randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) randlibhw.c -shared -fPIC -o randlibhw.so
randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) randlibsw.c -shared -fPIC -o randlibsw.so
randmain: randcpuid.o randmain.o
	$(CC) $(CFLAGS) randcpuid.o randmain.o -ldl -Wl,-rpath=$(PWD) -o randmain
