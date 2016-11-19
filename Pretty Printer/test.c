#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  // Testing how instructions will be inserted into an array
  
  char instructionList[1000][100];
  char *temp = "LAA 1";
  strcpy(instructionList[0], temp);
  double test = 1000.2;
  strcpy(instructionList[1], "LOD");
  sprintf(instructionList[2], "LAA %f", test);
  strcpy(instructionList[3], temp);
  for(int i = 0; i < 1000; i++) {
    if(strlen(instructionList[i]) == 0) {
      break;
    }
    else {
      printf("%s\n", instructionList[i]);
      // printf("Size of element at i: %lu\n", sizeof(instructionList[i]));
    }
  }
  return 0;
}
