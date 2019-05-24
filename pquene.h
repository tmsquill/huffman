/*******************************************************************************
 * Troy Squillaci
 * University of New Mexico – CS 241L
 *
 * This header file contains various function prototypes for Priority Quene
 * operations in addition to the struct definition of a PQNode.
 ******************************************************************************/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "huffman.h"

struct PQNode
{
  struct HTNode * node;
  struct PQNode * next;
};

struct PQNode * createPQNode(struct HTNode * node);

struct PQNode * retrievePQNode(struct PQNode * head, unsigned char symbol);

struct PQNode * insertBySymbol(struct PQNode * head, struct HTNode * node);

struct PQNode * insertByFrequency(struct PQNode * head, struct HTNode * node);

struct PQNode * push(struct PQNode* head, struct HTNode * node);

struct PQNode * pop(struct PQNode ** head_ref);

void sortPQ(struct PQNode * head);

int sizePQ(struct PQNode * head);

void printPQ(struct PQNode * head);

void freePQ(struct PQNode * head);

#endif