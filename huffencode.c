/*******************************************************************************
 * Troy Squillaci
 * University of New Mexico – CS 241L
 *
 * This program encodes a file by using a varient of the Huffman Coding
 * algorithm.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "huffman.h"
#include "ftable.h"
#include "htree.h"

//Turns on the nth least significant bit of the character pointed to.
void setBit(char * c, int n)
{
  * c |= 1 << n;
}



//Uses Huffman Coding to produce a lossless compressed version of the provided
//file.
void encodeFile(FILE * in, FILE * out, int print_table)
{
  //Initialize the frequency table from the file input.
  struct FTNode * frequency_table = initEncode(in);
  
  //Print the frequency table to standard out if the flag is set.
  if (print_table)
  {
    printFT(frequency_table);
  }
  
  //Write an unsigned short to the beginning of the file indicating the number
  //of unique symbols.
  unsigned short unique_count = getUniqueSymbolCount(frequency_table);
  fwrite(&unique_count, sizeof(unsigned short), 1, out);
  
  //Write the frequency table to the file.
  writeFTable(frequency_table, out);
  
  //Write the total number of symbols to the file.
  unsigned long total_count = getTotalSymbolCount(frequency_table);
  fwrite(&total_count, sizeof(unsigned long), 1, out);
  
  //Set a reusable HTNode pointer for encoding.
  struct FTNode * active_node;
  
  //The buffer character used for building the bit codes and writing to
  //the encoded file.
  char buffer = 0;
  
  //The current character read from the file input stream.
  int active_char;
  
  //Denotes the number of encoded bits within the buffer. Once the buffer is
  //full, it gets written to the encoded file and this variable is reset for the
  //next set of data.
  int assigned = 0;
  
  //Used for looping (ANSI C).
  int i;
  
  /* ----- Encoding ----- */
  
  //Encode the entire file until end of file is reached.
  while ((active_char = fgetc(in)) != EOF)
  {
    active_node = getNode(frequency_table, active_char);
    
    if (DEBUG_EN)
    {
      if (active_char > 32 && active_char < 127)
      {
        printf("The active symbol is '%c' and its code is %s\n",
               active_node -> symbol, active_node -> code);
      }
      else
      {
        printf("The active symbol is '=%d' and its code is %s\n",
               active_node -> symbol, active_node -> code);
      }
    }
    
    for (i = 0; i < active_node -> code_size; i++)
    {
      if (DEBUG_EN) printf("Active Bit: %c\n", active_node -> code[i]);
      
      //If space is available in the buffer, set the next bit.
      if (assigned < 8)
      {
        if (active_node -> code[i] == '1')
        {
          setBit(&buffer, assigned);
        }
      }
      else
      {
        putc(buffer, out);
        buffer = 0;
        assigned = 0;
        
        if (active_node -> code[i] == '1')
        {
          setBit(&buffer, assigned);
        }
      }
      
      assigned++;
    }
  }
  
  putc(buffer, out);
  
  freeFTable(frequency_table);
  
  if (DEBUG_EN) printf("Encoding successful!\n");
}



int main(int argc, char ** argv)
{  
  char * in_file;
  char * out_file;
  FILE * in;
  FILE * out;

  //If the incorrect number of arguments is provided, then print error and exit.
  if (argc != 3) 
  {
    printf("Error: Wrong number of arguments provided\n");
    return 1;
  }

  in_file = argv[1];
  out_file = argv[2];

  //Attempt to establish input stream to file.
  in = fopen(in_file, "rb");
  if (in == NULL)
  {
    printf("Error: Couldn't open %s for reading\n", in_file);
    perror("Reason");
    return 2;
  }

  //Attempt to establish output stream to file.
  out = fopen(out_file, "wb");
  if (out == NULL)
  {
    printf("Error: Couldn't open %s for writing\n", out_file);
    perror("Reason");
    return 3;
  }
  
  encodeFile(in, out, 1);

  fclose(in);
  fclose(out);

  return 0;
}
