/*******************************************************************************
 * Troy Squillaci
 * University of New Mexico – CS 241L
 *
 * This program provides an easy way to construct and manage Priority Quene
 * nodes.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "htree.h"
#include "pquene.h"

//Creates a new PQNode from the given HTNode input and returns a pointer to it.
struct PQNode * createPQNode(struct HTNode * node)
{
  struct PQNode * new_list_node = malloc(sizeof(struct PQNode));
  
  new_list_node -> node = node;
  new_list_node -> next = NULL;
  
  return new_list_node;;
}



//Attempts to find a node that contains an HTNode with the provided symbol data.
//If the node exists return a pointer to it, else return a NULL pointer.
struct PQNode * retrievePQNode(struct PQNode * head, unsigned char symbol)
{
  struct PQNode * current = head;
  
  while (current)
  {
    if (current -> node -> symbol == symbol) return current;
    current = current -> next;
  }
  
  return NULL;
}



//Inserts a new PQNode in the Priority Quene. Insertion is in-order and is
//determined by the symbol of the HTNode.
struct PQNode * insertBySymbol(struct PQNode * head, struct HTNode * node)
{
  struct PQNode * new_node = createPQNode(node);
  
  if (!head || new_node -> node -> symbol < head -> node -> symbol)
  {
    new_node -> next = head;
    return new_node;
  }
  
  else
  {
    struct PQNode * access = head;

    while (access -> next)
    {
      if (new_node -> node -> symbol > access -> next -> node -> symbol)
      {
        access = access -> next;
      }

      else if (new_node -> node -> symbol == access -> next -> node -> symbol)
      {
        if (maxValue(new_node -> node) > maxValue(access -> next -> node))
        {
          access = access -> next;
          continue;
        }
        break;
      }
      
      else
      {
        break;
      }
    }

    new_node -> next = access -> next;
    access -> next = new_node;
  }
  
  return head;
}



//Inserts a new PQNode in the Priority Quene. Insertion is in-order and is
//determined by the frequency of the HTNode.
struct PQNode * insertByFrequency(struct PQNode * head, struct HTNode * node)
{
  struct PQNode * new_node = createPQNode(node);
  
  if (!head || new_node -> node -> freq < head -> node -> freq)
  {
    new_node -> next = head;
    return new_node;
  }
  
  else
  {
    struct PQNode * access = head;
    
    while (access -> next)
    {
      if (new_node -> node -> freq > access -> next -> node -> freq)
      {
        access = access -> next;
      }
      
      else if (new_node -> node -> freq == access -> next -> node -> freq)
      {
        if (maxValue(new_node -> node) > maxValue(access -> next -> node))
        {
          access = access -> next;
          continue;
        }
        break;
      }
      
      else
      {
        break;
      }
    }
    
    new_node -> next = access -> next;
    access -> next = new_node;
  }
  
  return head;
}



//Push a new PQNode onto the head of the Priority Quene and return a new head.
struct PQNode * push(struct PQNode * head, struct HTNode * node)
{
  struct PQNode * new_head = createPQNode(node);
  
  new_head -> next = head;
  
  return new_head;
}



//Pop a PQNode off of the head of the Priority Quene and return a new head.
struct PQNode * pop(struct PQNode ** head_ref)
{
  struct PQNode * head = * head_ref;

  * head_ref = head -> next;
  
  head -> next = NULL;
  
  return head;
}



//Helper function for sortList that swaps two nodes in the Priority Quene.
static void swap(struct PQNode * node_one, struct PQNode * node_two)
{
  struct HTNode * temp = node_one -> node;
  node_one->node = node_two->node;
  node_two->node = temp;
}



//Sorts the Priority Quene by frequencies of root HTNodes.
void sortPQ(struct PQNode * head)
{
  if (!head) return;
  
  int swapped;
  struct PQNode * current;
  struct PQNode * end = NULL;
  
  do
  {
    swapped = 0;
    current = head;
    
    while (current -> next != end)
    {
      if (current -> node -> freq > current -> next -> node -> freq)
      {
        swap(current, current -> next);
        swapped = 1;
      }
      else if (current -> node -> freq == current -> next -> node -> freq)
      {
        
        if (maxValue(current -> node) > maxValue(current -> next -> node))
        {
          swap(current, current -> next);
          swapped = 1;
        }
      }
      current = current -> next;
    }
    end = current;
  }
  while (swapped);
}



//Returns the size of the Priority Quene.
int sizePQ(struct PQNode * head)
{
  struct PQNode * current = head;
  int count = 0;
  
  //Traverse the priority quene and count the nodes.
  while (current)
  {
    count ++;
    current = current -> next ;
  }
  
  return count ;
}



//Prints the Priority Quene on a single line seperated by spaces.
void printPQ(struct PQNode * head)
{
  struct PQNode * current = head;
  
  //While not at end of the list traverse the list and print the data in
  //the nodes.
  while (current)
  {
    printf("%c:%lu ", current -> node -> symbol, current -> node -> freq);
    current = current -> next;
  }
  
  printf("\n");
}



//Free the memory used by the Priority Quene.
void freePQ(struct PQNode * head)
{
  struct PQNode * current = head;
  struct PQNode * next    = current -> next;
  
  while (current)
  {
    free(current);
    current = next;
    if (next) next = current -> next;
  }
}