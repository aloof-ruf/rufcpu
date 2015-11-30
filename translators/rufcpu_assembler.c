/*
  Assembler for rufcpu code V0.1 (.rpu)
*/
#include <stdio.h>

int main(int argc, char** argv){

  // MUST include ONE .rpu file to assemble and the name of the resulting .ruf
  // file
  if (argc != 3){
    fprintf(stderr, "Incorrect number of arguments. Must have 3 arguments.");
    return 1;
  }

  FILE* rpufile = fopen(argv[1], "r");
  FILE* ruffile = fopen(argv[2], "w");

  // need a table for labels and stuff

  // first pass
  
  // second pass

  fclose(rpufile);
  fclose(ruffile);

  return 0;

}
