/*******************************************************************************
 * Troy Squillaci
 * University of New Mexico – CS 241L
 *
 * This header file contains various function prototypes for Frequency Table
 * operations in addition to the struct definition of a FTNode.
 ******************************************************************************/

#ifndef FTABLE_H
#define FTABLE_H

struct FTNode
{
  unsigned char symbol;
  unsigned long freq;
  char * code;
  int code_size;
  
  struct FTNode * next;
};

struct FTNode * createFTNode(unsigned char symbol, unsigned long freq,
                             char * code, int code_size);

struct FTNode * insertFTNode(struct FTNode * head, struct FTNode * node);

struct FTNode * getNode(struct FTNode * head, unsigned char symbol);

unsigned long getTotalSymbolCount(struct FTNode * head);

unsigned short getUniqueSymbolCount(struct FTNode * head);

void writeFTable(struct FTNode * head, FILE * out);

void printFT(struct FTNode * head);

void freeFTable(struct FTNode * head);

#endif