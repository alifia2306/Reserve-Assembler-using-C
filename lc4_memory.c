#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_memory.h"
#define getBits15to12(I) ((I>>12) & 0xF)

struct row_of_memory* add_to_list    (struct row_of_memory* head, 
                      short unsigned int address, 
                      short unsigned int contents) 
{   
	/* takes in head pointer to a linked list: head
       creates a new node with address & contents passed in
       adds node to end of linked list pointed to by head
       if list is empty, new node is now head
       always returns head pointer */

    /* add node to the end of linked list pointed to by "head"
       if list doesn't exist, create 1st node */

    struct row_of_memory *temp , *curr;

	 /* allocate memory for a single node */
    temp = malloc(sizeof(struct row_of_memory));
    if (temp == NULL) {
        printf("Failed to allocate memory\n");
        return NULL;
    }
    
    /* populate fields in newly allocated node w/ address&contents */
    temp->address = address;
    temp->label = NULL;
    temp->contents = contents;
    temp->assembly = NULL;
    temp->next = NULL;
    temp->directive = NULL;

     /* if head==NULL, node created is the new head of the list! */
    if(head == NULL){
        head = temp;
        head->next = NULL;
        return head;
    }
   
   /* otherwise, traverse linked list until we reach the end */
    curr = head;
    while(curr->next!= NULL){
        curr = curr->next;
    }

 	 /* add node to the end of list */
    curr->next = temp;
    
    /* always return pointer to head of linked list */
    return head ;   
}

struct row_of_memory* add_directive    (struct row_of_memory* head, 
                      short unsigned int address, int data) 
{   
	 /*adds code or data directive depending upon the value of the field data passed to the function*/

    struct row_of_memory *temp , *curr;
    temp = malloc(sizeof(struct row_of_memory));
    if (temp == NULL) {
        printf("Failed to allocate memory\n");
        return NULL;
    }
    
    temp->address = 0;
    temp->label = NULL;
    temp->contents = 0;
    temp->assembly = NULL;
    temp->next = NULL;
	temp->directive = malloc((sizeof(char) + 1)* (16));
	bzero(temp->directive, (sizeof(char) + 1)* 16);
	
	// add a data directive
	if (data == 1) 
		sprintf(temp->directive, ".DATA\n.ADDR x%x", address);

	// add a code directive
	else 
		sprintf(temp->directive, ".CODE\n.ADDR x%x", address);

	/* if head==NULL, node created is the new head of the list! */
    if(head == NULL){
        head = temp;
        head->next = NULL;
        return head;
    }
   
    /* otherwise, traverse linked list until we reach the end */
    curr = head;
    while(curr->next!= NULL){
        curr = curr->next;
    }

 	/* add node to the end of list */
    curr->next = temp;
    
    /* always return pointer to head of linked list */
    return head ;   
}

struct row_of_memory* search_address (struct row_of_memory* head, 
				      short unsigned int address ) 
{	
	/* traverse linked list, searching each node for "address"  */
	while(head != NULL) {
		if(head->address == address) {

			/* return pointer to node in list that matches */
			return head;
		
		}
	head=head->next;
	} 
	/* return NULL if list is empty or if "address" isn't found */
	return NULL;
}

struct row_of_memory* search_opcode  (struct row_of_memory* head, 
				      short unsigned int opcode  ) 
{
	unsigned int contents = 0;

	/* traverse linked list until node is found with matching opcode
	   AND "assembly" field of node is empty */
	while(head != NULL) {
		contents = getBits15to12(head->contents);
		if(contents == opcode && head->assembly == NULL) {

			/* return pointer to node in list that matches */
			return head;
		
		}
	head=head->next;
	} 
	/* return NULL if list is empty or if no matching nodes */
	return NULL ;
}

void print_list (struct row_of_memory* head ) 
{

	/* make sure head isn't NULL */
  if(head==NULL)
  {
    printf("no elements in the list\n");
    return ;
  }

  /* print out a header */
  printf("             <label>       <address>   <contents>   <assembly>\n");

  
  /* traverse linked list, print contents of each node */	
  while(head != NULL)
  {
  	if(head -> label != NULL){
  		printf("%*s", 20, head -> label);
  	}
  	else{ 
  		printf("%*s", 20, " ");
  	}
  	printf("          %04x",head->address);
  	printf("        %04x",head->contents);
  	if(head -> assembly != NULL){
  		printf("      %s\n", head -> assembly);
  	}
  	else{ 
  		printf("\n");
  	}

    head = head->next;
    
	}
	return ;
}

struct row_of_memory* delete_list    (struct row_of_memory* head ) 
{
	 /* delete entire list node by node */
    if(head == NULL){
        return NULL;
    }
    
struct row_of_memory *temp;
while(head != NULL){

    temp = head->next;
    head->next = NULL;
    if (head->label != NULL) {
        free(head->label);
        head->label = NULL;
    }
    
    if (head->assembly != NULL) {
        free(head->assembly);
        head->assembly = NULL;
    }

    if (head->directive != NULL) {
		free(head->directive);
		head->directive = NULL;
	}
    
    free(head);
    head = temp;
 

  }
  return head ;
}

struct row_of_memory* sort_list(struct row_of_memory* head) 
{

struct row_of_memory *curr, *smallest, *smallestPrev, *prev;
struct row_of_memory *temp;
curr = head;
smallest = head;
prev = head;
smallestPrev = head;

// if zero or one element in the list, the list is sorted.
if(head == NULL || head->next == NULL)
    return head; 

//find the smallest node.
while(curr != NULL) {
        if(curr->address < smallest->address) {
            smallestPrev = prev;
            smallest = curr;
        }
        prev = curr;
        curr = curr->next;
    }

//switching first node and smallest node.
if(smallest != head)
{
    smallestPrev->next = head;
    temp = head->next;
    head->next = smallest->next;
    smallest->next = temp;
}
// calling the function again with the list minus its first node.
smallest->next = sort_list(smallest->next);
return smallest;
}
