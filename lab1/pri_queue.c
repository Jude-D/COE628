#include <stdlib.h>
#include <stdio.h>
#include "pri_queue.h"
/** @file pri_queue.c */
static Node_ptr_t head = NULL;
/**
 * Insert a Node into a priority queue.
 * @param priority
 * @param data
 * @author YOUR NAME
 */
void PQ_insert(int priority, char *data)
{
    //FIX THIS
    Node_ptr_t New_Node = (Node_ptr_t) malloc(sizeof(Node_t));
    New_Node->priority = priority;
    New_Node->data = data;
    
    if(!head)
    {
        head = New_Node;
        New_Node->next = NULL;
    }
    else
    {
        Node_ptr_t* previous = &head;
        Node_ptr_t current = head;
        while (current)
        {
            if(current->priority > New_Node->priority)
            {
                previous = &current->next;
                current = current->next;
            }
            else
            {
                break;
            }
            
        }
        *previous = New_Node;
         New_Node->next = current;
    }
}
/**
 * Delete and return the node with the highest priority.
 * @return The highest priority Node.
 */
Node_ptr_t PQ_delete()
{
    Node_ptr_t old_head = head;
    head = head->next;
    return old_head;
}

/**
 * Do NOT modify this function.
 * @return A pointer to the head of the list.  (NULL if list is empty.)
 */
Node_ptr_t PQ_get_head()
{
    return head;
}

/**
 * Do NOT modify this function.
 * @return the number of items in the queue
 */
int PQ_get_size()
{
    int size = 0;
    Node_ptr_t tmp;
    for (tmp = head; tmp != NULL; tmp = tmp->next, size++)
        ;
    return size;
}
