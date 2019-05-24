/*******************************************************************************
 * Troy Squillaci
 * University of New Mexico – CS 241L
 *
 * This program provides an easy way to construct and manage Huffman Tree nodes.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "htree.h"

//Creates a new empty HTNode and returns a pointer to it.
struct HTNode * createHTNode()
{
  struct HTNode * new_node = malloc(sizeof(struct HTNode));
  
  new_node -> symbol    = 0;
  new_node -> freq      = 0;
  new_node -> is_leaf   = 0;
  new_node -> code_size = 0;
  new_node -> code      = NULL;
  new_node -> left      = NULL;
  new_node -> right     = NULL;
  
  return new_node;
}



//Returns the largest symbol in the given Huffman Tree.
unsigned char maxValue(struct HTNode * root)
{
  struct HTNode * current = root;
  
  //Traverse to the most right-ward node (to the largest data).
  while (current -> right)
  {
    current = current -> right;
  }
  
  //Return the largest data.
  return current -> symbol;
}



//Helper function for printHTTree. Prints the in-order traversal of the Huffman
//Tree on a single line. This function can be used standalone, if a newline
//character irrelevant.
void printHTHelper(struct HTNode * root)
{
  if (root)
  {
    printHTHelper(root -> left);
    
    if (root -> symbol > 32 && root -> symbol < 127)
    {
      printf("%c:%d ", root -> symbol, root -> is_leaf);
    }
    else
    {
      printf("=%d:%d ", root -> symbol, root -> is_leaf);
    }
    
    printHTHelper(root -> right);
  }
}



//Calls printHTHelper to print the in-order traversal of the Huffman Tree,
//then prints a newline character.
void printHT(struct HTNode * root)
{
  printHTHelper(root);
  printf("\n");
}



//Helper function for printLeaves. Prints the in-order traversal data of the
//Huffman Tree leaves on a single line. This function can be used standalone, if
//a newline character irrelevant.
void printLeavesHelper(struct HTNode * root)
{
  //If the root is null, exit function.
  if (root)
  {
    printLeavesHelper(root -> left);
    
    if (root -> is_leaf)
    {
      if (root -> symbol > 32 && root -> symbol < 127)
      {
        printf("%c:%lu:%d ", root -> symbol, root -> freq, root -> is_leaf);
      }
      else
      {
        printf("=%d:%lu:%d ", root -> symbol, root -> freq, root -> is_leaf);
      }
    }
    
    printLeavesHelper(root -> right);
  }
}


//Calls printLeavesHelper to print the in-order traversal data of the Huffman
//Tree leaves. Then prints a newline character.
void printLeaves(struct HTNode * root)
{
  printLeavesHelper(root);
  printf("\n");
}



//Frees the memory used by the Huffman Tree.
void freeHT(struct HTNode * root)
{
  if (root)
  {
    freeHT(root -> left);
    freeHT(root -> right);
    free(root -> code);
    free(root);
  }
}