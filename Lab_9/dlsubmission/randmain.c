#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dlfcn.h>
#include "randcpuid.h"

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  // void (*initialize) (void);
  unsigned long long (*rand64) (void);
  // void (*finalize) (void);
  void *dl_handle;
  void *error;
  if (rdrand_supported ())
    {
      dl_handle = dlopen("randlibhw.so", RTLD_NOW);
      if (!dl_handle) {
	fprintf(stderr, "Error opening randlibhw.so - %p\n", dlerror());
	return 1;
      }
      rand64 = dlsym(dl_handle, "hardware_rand64");
      error = dlerror();
      if (error != NULL) {
	fprintf(stderr, "Error finding hardware_rand64 - %p\n", error);
	return 1;
      }
    }
  else
    {
      dl_handle= dlopen("randlibsw.so", RTLD_NOW);
      if (!dl_handle) {
	fprintf(stderr, "Error opening randlibhw.so - %p\n", dlerror());
	return 1;
      }
      rand64 = dlsym(dl_handle, "software_rand64");
      error = dlerror();
      if (error != NULL) {
	fprintf(stderr, "Error finding hardware_rand64 - %p\n", error);
	return 1;
      }
    }

  // initialize ();
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

    do
      {
	unsigned long long x = rand64 ();
	size_t outbytes = nbytes < wordsize ? nbytes : wordsize;
	if (fwrite (&x, 1, outbytes, stdout) != outbytes)
	  {
	    output_errno = errno;
	    break;
	  }
	nbytes -= outbytes;
      }
    while (0 < nbytes);

    if (fclose (stdout) != 0)
      output_errno = errno;

    if (output_errno)
      {
	errno = output_errno;
	perror ("output");
	// finalize ();
	return 1;
      }

    //finalize ();
    dlclose(dl_handle);
    return 0;
}
