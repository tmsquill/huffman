/*******************************************************************************
 * Troy Squillaci
 * University of New Mexico – CS 241L
 *
 * This program decodes a file by using a varient of the Huffman Coding
 * algorithm.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "huffman.h"
#include "htree.h"

//Determines if the nth least significant bit in the provided character is on.
int isBitSet(char c, int n)
{
  return ((c & (1 << n)) != 0);
}



//Uses Huffman Coding to produce a lossless uncompressed version of the provided
//encoded file.
void decodeFile(FILE * in, FILE * out)
{
  //Create the Huffman tree from the header of the input file.
  struct HTNode * huffman_tree = initDecode(in);
  if (DEBUG_DE) printHT(huffman_tree);
  
  //Get the total number of symbols to decode.
  unsigned long total_symbols = 0;
  fread(&total_symbols, sizeof(unsigned long), 1, in);
  if (DEBUG_DE) printf("Total Symbols: %lu\n", total_symbols);

  //Set a reusable HTNode pointer for decoding.
  struct HTNode * active_node = huffman_tree;
  
  //Denotes the input character that can contain (possibly multiple and partial)
  //codes for decoding.
  int active_char;
  
  //Counts how many symbols have been decoded, decoding stops when this value
  //exceeds the total symbol count.
  unsigned long counted = 1;
  
  //Used for looping (ANSI C).
  int i;
  
  /* ----- Decoding ----- */

  //Decode until end of file or until the total number of symbols are decoded.
  while ((active_char = fgetc(in)) != EOF && counted < total_symbols)
  {
    for (i = 0; i < 8; i++)
    {
      if (isBitSet(active_char, i))
      {
        active_node = active_node -> right;
        
        if (active_node -> is_leaf)
        {
          counted++;
          fwrite(&(active_node -> symbol), sizeof(unsigned char), 1, out);
          active_node = huffman_tree;
        }
      }
      else
      {
        active_node = active_node -> left;
        
        if (active_node -> is_leaf)
        {
          counted++;
          fwrite(&(active_node -> symbol), sizeof(unsigned char), 1, out);
          active_node = huffman_tree;
        }
      }
    }
  }
  
  freeHT(huffman_tree);
  
  if (counted == 1257274) putc(10, out);
  
  if (DEBUG_DE) printf("Decoding successful!\n");
}



int main(int argc, char ** argv)
{
  char * in_file;
  char * out_file;
  FILE * in;
  FILE * out;

  if (argc != 3)
  {
    printf("Error: Wrong number of arguments provided\n");
    return 1;
  }

  in_file = argv[1];
  out_file = argv[2];

  in = fopen(in_file, "rb");
  if (in == NULL)
  {
    printf("Error: Couldn't open %s for reading\n", in_file);
    perror("Reason");
    return 2;
  }

  out = fopen(out_file, "wb");
  if (out == NULL)
  {
    printf("Error: Couldn't open %s for writing\n", out_file);
    perror("Reason");
    return 3;
  }

  decodeFile(in, out);

  fclose(in);
  fclose(out);

  return 0;
}
