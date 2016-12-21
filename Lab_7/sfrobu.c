// VAMSI MOKKAPATI
//
// Takes in an input of words trivially encoded via the memfrob
// function, and sorts the data without decoding it and encoding
// it. Outputs the encoded results.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

long numCmp = 0;

int frobcmp(char const* a, char const* b)
{
  numCmp++;
  int x;
  for (x = 0; a[x] != ' ' && b[x] != ' '; x++) {
    if ((a[x] ^ 42) > (b[x] ^ 42))
      return 1;
    else if ((a[x] ^ 42) < (b[x] ^ 42))
      return -1;
  }
  
  // If nothing is returned after the loop, that means
  // one of the words is a substring of the second word
  
  if (a[x] == ' ' && b[x] != ' ')      // return -1 if a is shorter than b
    return -1;
  else if (a[x] != ' ' && b[x] == ' ') // return 1 if a is longer than b
    return 1;
  else                                 // return 0 if a and b are equal
    return 0;
}

// To be passed into qsort
int comparator(const void* p1, const void* p2)
{
  return frobcmp(*((char const**)p1), *((char const**)p2));
}

int main()
{
  struct stat fileStats;
  fstat(0, &fileStats);
  char* inputStr;
  int fileSize = fileStats.st_size;
  int inputLen = 2054;

  if (fileSize == 0)
    inputStr = (char*)malloc(sizeof(char) * inputLen);
  else {
    inputStr = (char*)malloc(sizeof(char) * fileSize);
    // fprintf(stderr, "One shot allocation!\n");
  }
  if (inputStr == NULL) {
    fprintf(stderr, "Memory allocation error for input string!");
    exit(1);
  }

  // Takes all the input characters and puts it in the inputStr array.
  // Keeps track of the character and word count for later use.
  char c;
  ssize_t pos = read(0, &c, 1);
  int cntChar = 0;
  int cntWord = 0;
  while (pos > 0) {
    inputStr[cntChar++] = c;
    if (c == ' ')
      cntWord++;

    // Reallocates memory if the initial arbitrary length is exceeded.
    if (fileSize == 0 && cntChar == inputLen) {
      inputStr = (char*)realloc(inputStr, 2*inputLen);
      inputLen *= 2; // doubles the input length to reduce number of
	             // possible later memory reallocations.
      if (inputStr == NULL) {
	fprintf(stderr, "Memory allocation error for input string!");
	exit(1);
      }
    }
    else if (fileSize != 0 && cntChar == fileSize) {
      inputStr = (char*)realloc(inputStr, 2*fileSize);
      fileSize *= 2;
      if (inputStr == NULL) {
	fprintf(stderr, "Memory allocation error for input string!");
	exit(1);
      }
    }
    pos = read(0, &c, 1);
  }

  // Adds space at the end of string if not there already.
  if (inputStr[cntChar-1] != ' ') {
    cntWord++;
    cntChar++;
    if (fileSize == 0 && cntChar-1 == inputLen) {
      inputStr = (char*)realloc(inputStr, 1+inputLen);
      if (inputStr == NULL) {
	fprintf(stderr, "Memory allocation error for input string!");
	exit(1);
      }
    }
    else if (fileSize != 0 && cntChar-1 == fileSize) {
      inputStr = (char*)realloc(inputStr, 1+fileSize);
      if (inputStr == NULL) {
	fprintf(stderr, "Memory allocation error for input string!");
	exit(1);
      }
    }
    inputStr[cntChar-1] = ' ';
  }

  // Initializes an auxiliary array containing pointers to the first
  // characters of every word.
  char** aux = (char**)malloc(sizeof(char*)* cntWord);
  if (aux == NULL) {
    fprintf(stderr, "Memory allocation error for auxiliary array!");
    exit(1);
  }
  aux[0] = &inputStr[0];

  int cnt = 1;
  int i;
  for (i = 0; i < cntChar-1; i++)
    if (inputStr[i] == ' ')
      aux[cnt++] = &inputStr[i+1];

  // Sorts all the words according to the comparator function defined
  // above.
  qsort(aux, cntWord, sizeof(char*), comparator);

  int j = 0;
  while (j < cntWord) {
    char* output = aux[j];
    while (*output != ' ') {
      write(1, &(*output), 1);
      output++;
    }
    char space = ' ';
    write(1, &space, 1);
    j++;
  }

  // Free memory of inputStr and aux to prevent memory leaks.
  free(inputStr);
  free(aux);

  // Print out the number of comparisons (aka frobcmp invocations)
  fprintf(stderr, "Comparisons: %li\n", numCmp);
  
  return 0;
}
