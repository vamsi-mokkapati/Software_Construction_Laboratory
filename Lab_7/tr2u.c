#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  if (argc != 3) {
    fprintf(stderr, "Only 2 input arguments allowed!\n");
    exit(1);
  }

  if (strlen(argv[1]) != strlen(argv[2])) {
    fprintf(stderr, "Both arguments must be of the same length!\n");
    exit(1);
  }

  int i;
  int j;
  for(i = 0; i < strlen(argv[1]); i++) {
    for (j = i+1; j < strlen(argv[1]); j++) {
      if (argv[1][i] == argv[1][j]) {
	    fprintf(stderr, "The first argument should not have duplicates!\n");
	    exit(1);
      }
    }
  }

  char c;
  ssize_t pos = read(0, &c, 1);
  int replace;
  while (pos > 0) {
    replace = 0;
    int x;
    for (x = 0; x < strlen(argv[1]); x++) {
      if (c == argv[1][x]) {
	    char put = argv[2][x];
	    write(1, &put, 1);
	    replace = 1;
      }
    }
    if (replace == 0)
      write(1, &c, 1);
    pos = read(0, &c, 1);
  }
  return 0;
}