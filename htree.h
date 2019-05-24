/*******************************************************************************
 * Troy Squillaci
 * University of New Mexico – CS 241L
 *
 * This header file contains various function prototypes for Huffman Tree
 * operations in addition to the struct definition of a HTNode.
 ******************************************************************************/

#ifndef BINARYTREE_H
#define BINARYTREE_H

struct HTNode
{
  unsigned char symbol;
  unsigned long freq;
  int is_leaf;
  char * code;
  int code_size;
  struct HTNode * left;
  struct HTNode * right;
};

struct HTNode * createHTNode();

unsigned char maxValue(struct HTNode * root);

void printHT(struct HTNode * root);

void printLeaves(struct HTNode * root);

void freeHT(struct HTNode * root);

#endif
