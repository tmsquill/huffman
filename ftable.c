/*******************************************************************************
 * Troy Squillaci
 * University of New Mexico – CS 241L
 *
 * This program provides an easy way to construct and manage Frequency Tables
 * derived from a Huffman tree.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ftable.h"

//Creates a new FTNode from the given input and returns a pointer to it.
struct FTNode * createFTNode(unsigned char symbol, unsigned long freq,
                             char * code, int code_size)
{
  struct FTNode * new_node = malloc(sizeof(struct FTNode));
  
  new_node -> symbol    = symbol;
  new_node -> freq      = freq;
  new_node -> code      = malloc(sizeof(char) * code_size);
  memcpy(new_node -> code, code, sizeof(char) * code_size);
  new_node -> code_size = code_size;
  new_node -> next   = NULL;

  return new_node;
}



//Inserts a FTNode into the Frequency Table. Insertion is in-order by symbol.
struct FTNode * insertFTNode(struct FTNode * head, struct FTNode * node)
{
  if (!head || node -> symbol < head -> symbol)
  {
    node -> next = head;
    return node;
  }
  
  else
  {
    struct FTNode * current = head;
    
    while (current -> next && current -> next -> symbol < node -> symbol)
    {
      current = current -> next;
    }
    
    node -> next = current -> next;
    current -> next = node;
  }
  
  return head;
}



//Attemps to find a FTNode in the Frequency Table that contains the given
//symbol. If the node is found it is returned, else a NULL pointer is returned.
struct FTNode * getNode(struct FTNode * head, unsigned char symbol)
{
  struct FTNode * temp = head;
  
  while (temp)
  {
    if (symbol == temp -> symbol) return temp;
    temp = temp -> next;
  }
        
  return NULL;
}



//Returns the total number of symbols by adding up the frequencies.
unsigned long getTotalSymbolCount(struct FTNode * head)
{
  struct FTNode * current = head;
  unsigned long count = 0;
  
  while (current)
  {
    count = count + current -> freq;
    current = current -> next ;
  }
  
  return count;
}



//Returns the number of unique symbols.
unsigned short getUniqueSymbolCount(struct FTNode * head)
{
  struct FTNode * current = head;
  unsigned short count = 0;
  
  while (current)
  {
    count ++;
    current = current -> next;
  }
  
  return count;
}



//Writes the Frequency Table to the current location of a file pointed to by the
//FILE pointer.
void writeFTable(struct FTNode * head, FILE * out)
{
  struct FTNode * current = head;
  
  while (current)
  {
    fwrite(&(current -> symbol), sizeof(unsigned char), 1, out);
    fwrite(&(current -> freq), sizeof(unsigned long), 1, out);
    
    current = current -> next;
  }
}



//Prints the list on a single line seperated by spaces.
void printFT(struct FTNode * head)
{
  struct FTNode * current = head;
  
  printf("Symbol\tFreq\tCode\n");
  
  while (current)
  {
    if (current -> symbol > 32 && current -> symbol < 127)
    {
      printf("%c\t%lu\t", current -> symbol, current -> freq);
      
      int i;
      
      for (i = 0; i < current -> code_size; i++)
      {
        printf("%c", current -> code[i]);
      }
      printf("\n");
    }
    else
    {
      printf("=%d\t%lu\t", current -> symbol, current -> freq);
      
      int i;
      
      for (i = 0; i < current -> code_size; i++)
      {
        printf("%c", current -> code[i]);
      }
      printf("\n");
    }
    current = current -> next;
  }
  
  printf("Total chars = %lu\n", getTotalSymbolCount(head));
}



//Free the memory used by the list.
void freeFTable(struct FTNode * head)
{
  struct FTNode * current = head;
  struct FTNode * next    = current -> next;

  while (current)
  {
    free(current -> code);
    free(current);
    current = next;
    if (next) next = current -> next;
  }
}