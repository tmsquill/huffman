/*******************************************************************************
 * Troy Squillaci
 * University of New Mexico – CS 241L
 *
 * This header file contains various function prototypes for Huffman Coding
 * operations.
 ******************************************************************************/

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "ftable.h"
#include "pquene.h"

struct PQNode * buildPQEncode(FILE * in);

struct PQNode * buildPQDecode(FILE * in);

struct HTNode * buildHuffmanTree(struct PQNode * priority_quene);

void printGraphicalHuffmanTree(struct HTNode * root, int indent);

void buildHuffmanCodes(struct HTNode * root, int * code, int top);

void buildFrequencyTable(struct HTNode * root);

struct FTNode * initEncode(FILE * in);

struct HTNode * initDecode(FILE * in);

void encodeFile(FILE* in, FILE* out, int printTable);

void decodeFile(FILE* in, FILE* out);

#endif