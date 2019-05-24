/*******************************************************************************
 * Troy Squillaci
 * University of New Mexico – CS 241L
 *
 * This program provides various Huffman Coding operations including building
 * the Priority Quene, Huffman Tree, and Frequency Table. This file should not
 * be used stand-alone but rather in sync with encoding and decoding routines.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "debug.h"
#include "huffman.h"
#include "ftable.h"
#include "htree.h"
#include "pquene.h"

struct FTNode * frequency_table;
int * codes;

//Builds a priority quene of HTNodes derived from the provided input file.
struct PQNode * buildPQEncode(FILE * in)
{
  struct PQNode * priority_quene = NULL;
  
  int active_char;
  
  //Loop through the input file stream until end of file is reached.
  while ((active_char = fgetc(in)) != EOF)
  {    
    //Search the list for the character, if it exists a pointer to it is
    //returned. If it isn't found a NULL pointer is returned instead.
    struct PQNode * temp = retrievePQNode(priority_quene, active_char);
    
    //If the character already exists, increment the frequency.
    if (temp)
    {
      temp -> node -> freq++;
    }
    
    //Else make a new node for the character.
    else
    {
      struct HTNode * new_leaf = createHTNode();
      
      new_leaf -> symbol    = active_char;
      new_leaf -> freq      = 1;
      new_leaf -> is_leaf   = 1;
      new_leaf -> code_size = 0;
      new_leaf -> code      = NULL;
      new_leaf -> left      = NULL;
      new_leaf -> right     = NULL;
      
      priority_quene = insertBySymbol(priority_quene, new_leaf);
    }
  }
  
  codes = malloc(sizeof(int) * sizePQ(priority_quene));
  
  sortPQ(priority_quene);
  
  return priority_quene;
}



//Builds a priority quene of HTNodes derived from the provided input file.
struct PQNode * buildPQDecode(FILE * in)
{
  unsigned short unique_symbols = 0;
  
  fread(&unique_symbols, sizeof(unsigned short), 1, in);
  
  if (DEBUG_DE) printf("Unique Count: %hu\n", unique_symbols);
  
  struct PQNode * priority_quene = NULL;
  unsigned char active_symbol = 0;
  unsigned long active_freq = 0;
  int i;
  
  for (i = 0; i < unique_symbols; i++)
  {
    fread(&active_symbol, sizeof(unsigned char), 1, in);
    if (DEBUG_DE) printf("Active Symbol[%d]: %u\n", i, active_symbol);
    fread(&active_freq, sizeof(unsigned long), 1, in);
    if (DEBUG_DE) printf("Active Frequency[%d]: %lu\n", i, active_freq);
    
    struct HTNode * new_leaf = createHTNode();
    
    new_leaf -> symbol    = active_symbol;
    new_leaf -> freq      = active_freq;
    new_leaf -> is_leaf   = 1;
    new_leaf -> code_size = 0;
    new_leaf -> code      = NULL;
    new_leaf -> left      = NULL;
    new_leaf -> right     = NULL;
    
    priority_quene = insertBySymbol(priority_quene, new_leaf);
  }
  
  codes = malloc(sizeof(int) * sizePQ(priority_quene));
  
  sortPQ(priority_quene);
  
  return priority_quene;
}



//Builds the Huffman Tree from the HTNodes contained in the priority quene.
struct HTNode * buildHuffmanTree(struct PQNode * priority_quene)
{
  while (sizePQ(priority_quene) > 1)
  {
    struct PQNode * node_one = pop(&priority_quene);
    struct PQNode * node_two = pop(&priority_quene);
    
    struct HTNode * subtree_one = node_one -> node;
    struct HTNode * subtree_two = node_two -> node;
    
    free(node_one);
    free(node_two);
    
    struct HTNode * new_subtree = createHTNode();
    
    new_subtree -> symbol    = '$';
    new_subtree -> freq      = subtree_one -> freq + subtree_two -> freq;
    new_subtree -> is_leaf   = 0;
    new_subtree -> code      = NULL;
    new_subtree -> code_size = 0;
    
    if (subtree_one -> freq < subtree_two -> freq)
    {
      new_subtree -> left  = subtree_one;
      new_subtree -> right = subtree_two;
    }
    else if (subtree_one -> freq > subtree_two -> freq)
    {
      new_subtree -> left  = subtree_two;
      new_subtree -> right = subtree_one;
    }
    else
    {
      if (subtree_one -> is_leaf && subtree_two -> is_leaf)
      {
        if (subtree_one -> symbol < subtree_two -> symbol)
        {
          new_subtree -> left  = subtree_one;
          new_subtree -> right = subtree_two;
        }
        else
        {
          new_subtree -> left  = subtree_two;
          new_subtree -> right = subtree_one;
        }
      }
      else
      {
        if (maxValue(subtree_one) < maxValue(subtree_two))
        {
          new_subtree -> left  = subtree_one;
          new_subtree -> right = subtree_two;
        }
        else
        {
          new_subtree -> left  = subtree_two;
          new_subtree -> right = subtree_one;
        }
      }
    }
    
    priority_quene = insertByFrequency(priority_quene, new_subtree);
  }
  
  struct HTNode * temp = priority_quene -> node;
  
  freePQ(priority_quene);
  
  return temp;
}



//Prints a simple graphical representation of the Huffman Tree.
void printGraphicalHuffmanTree(struct HTNode * root, int indent)
{
  if (root)
  {
    printGraphicalHuffmanTree(root -> right, indent + 5);
    int tmp = indent;
    
    while (tmp != 0)
    {
      printf(" ");
      tmp--;
    }
    
    if (root -> symbol > 32 && root -> symbol < 127)
    {
      printf("%c\n", root -> symbol);
    }
    else
    {
      printf("=%d\n", root -> symbol);
    }

    printGraphicalHuffmanTree(root -> left, indent + 5);
  }
}



//Helper function for buildHuffmanCodes.
void assignCode(struct HTNode * root, int * code, int size)
{
  root -> code = malloc(sizeof(char) * size + 1);
    
  root -> code_size = size;
  
  int i;
  
  for (i = 0; i < size; ++i)
  {
    sprintf(&root -> code[i], "%d", * (code + i));
  }
}



//Compute the Huffman Codes by recursively traversing the Huffman Tree.
void buildHuffmanCodes(struct HTNode * root, int * code, int top)
{
  if (root -> left)
  {
    * (code + top) = 0;
    buildHuffmanCodes(root -> left, code, top + 1);
  }
  
  if (root -> right)
  {
    * (code + top) = 1;
    buildHuffmanCodes(root -> right, code, top + 1);
  }
  
  if (root -> is_leaf)
  {
    assignCode(root, code, top);
  }
}



//Computes the frequency table by recursively traversing the Huffman Tree.
void buildFrequencyTable(struct HTNode * root)
{
  if (root)
  {
    buildFrequencyTable(root -> left);
    
    if (root -> is_leaf)
    {
      frequency_table = insertFTNode(frequency_table,
                                     createFTNode(root -> symbol,
                                     root -> freq, root -> code,
                                     root -> code_size));
      //free(root -> code);
    }
    
    buildFrequencyTable(root -> right);
  }
}



//Calls a variety of utility functions to build the frequency table of the
//input file to be encoded.
struct FTNode * initEncode(FILE * in)
{
  //Build the Priority Quene.
  if (DEBUG_PQ) printf("Building the Priority Quene...\n");
  struct PQNode * active_pq = buildPQEncode(in);
  
  //Set the FILE pointer to the beginning of the file.
  rewind(in);
  
  //Build the Huffman Tree.
  if (DEBUG_HT) printf("Building the Huffman Tree...\n");
  struct HTNode * active_ht = buildHuffmanTree(active_pq);

  //Print a graphical representation of the Huffman Tree.
  if (DEBUG_HT)
  {
    printf("--- Graphical Version of the Huffman Tree ---\n");
    printGraphicalHuffmanTree(active_ht, 0);
  }
  
  //Build the Huffman Codes.
  if (DEBUG_HT) printf("Building the Huffman Codes...\n");
  buildHuffmanCodes(active_ht, codes, 0);
  
  free(codes);
  
  //Build and return the Frequency Table.
  if (DEBUG_FT) printf("Building the Frequency Table...\n");
  buildFrequencyTable(active_ht);
  
  //Free the memory used by the Huffman Tree.
  freeHT(active_ht);
  
  //Print the Frequency Table.
  if (DEBUG_FT)
  {
    printf("--- Frequency Table ---\n");
  }
  
  return frequency_table;
}



//Calls a variety of utility functions to build the Huffman Tree of the
//input file to be decoded.
struct HTNode * initDecode(FILE * in)
{
  //Build the Priority Quene.
  if (DEBUG_PQ) printf("Building the Priority Quene...\n");
  struct PQNode * active_pq = buildPQDecode(in);
  
  //Build the Huffman Tree.
  if (DEBUG_HT) printf("Building the Huffman Tree...\n");
  struct HTNode * active_ht = buildHuffmanTree(active_pq);
  
  //Build the Huffman Codes.
  if (DEBUG_HT) printf("Building the Huffman Codes...\n");
  buildHuffmanCodes(active_ht, codes, 0);
  
  free(codes);
  
  return active_ht;
}