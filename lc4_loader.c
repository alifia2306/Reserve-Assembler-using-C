#include <stdio.h>
#include "lc4_memory.h"
#include <stdlib.h>
#include <string.h>

/* declarations of functions that must defined in lc4_loader.c */

FILE* get_file() 
{
	char filename[256];
	FILE *myFilePtr;

	// To prompt the user to enter file name.
	printf("Please enter the name of the file you would like to convert:\n");

	// To take input from the user to a char array.
	scanf("%s", filename);

	myFilePtr = fopen(filename,"r");

	return myFilePtr;
}

int parse_file (FILE* my_obj_file, struct row_of_memory** memory) 
{	unsigned int header[2];
	unsigned int address[2];
	unsigned int n_bytes[2];
	unsigned int contents[2];
	unsigned int c;
	unsigned int n;
	unsigned int a;
	struct row_of_memory *node_addr = NULL;
	unsigned int node_opcode = 0;
	int i = 0;
	int ret = 0;
	char* labels = NULL;
	short unsigned int optcode = 0;
	while(feof(my_obj_file) == 0) {


	// Get first header byte
        header[0] = fgetc(my_obj_file);

	if(header[0] == EOF){
		break;
	}

	// Get second header byte
        header[1] = fgetc(my_obj_file);
	if(header[1] == EOF){
		break;
	}

	 // Get first address byte
	        address[0] = fgetc(my_obj_file);
			if(address[0] == EOF){
				break;
			}

			// Get second address byte
		        address[1] = fgetc(my_obj_file);
			if(address[1] == EOF){
				break;
			}
			a = address[1] + 256 * (address[0]);

		        // Get most significant byte of n
		        n_bytes[0] = fgetc(my_obj_file);
			if(n_bytes[0] == EOF){
				break;
			}

		        // Get least significant byte of n
		        n_bytes[1] = fgetc(my_obj_file);
			if(n_bytes[1] == EOF){
				break;
			}

		// Calculate number in n field
	    	n = n_bytes[1] + 256 * (n_bytes[0]);

	// Determine header type and populate node
        if(header[0] == 0xCA && header[1] == 0xDE){ // Check for code header

        	//adds code directive to the linked list.
        	*memory = add_directive(*memory, a, 0); 

	    	for(i = 0; i < n; i++){
	    		
	    		contents[0] = fgetc(my_obj_file);
	    		contents[1] = fgetc(my_obj_file);
	    		c =  contents[1] + 256 * contents[0];
	    		node_addr = search_address (*memory, a + i);
	    		if(node_addr == NULL){
	    			*memory = add_to_list (*memory, a + i, c);
	    		}
	    	}
            
        }
        else if(header[0] == 0xDA && header[1] == 0xDA){ // Check for data header

        	//adds data directive to the linked list.
          	*memory = add_directive(*memory, a, 1); 
	    	for(i = 0; i < n; i++){
	    		contents[0] = fgetc(my_obj_file);
	    		contents[1] = fgetc(my_obj_file);
	    		c =  contents[1] + 256 * contents[0];
	    		node_addr = search_address (*memory, a + i);
	    		if(node_addr == NULL){
	    			*memory = add_to_list (*memory, a + i, c);
	    		}
	    	}
            
        }

        else if(header[0] == 0xC3 && header[1] == 0xB7){ //Check for Symbol header

            node_addr = search_address (*memory, a);
            if(node_addr == NULL){
   				
   				add_to_list(*memory, a, c);
   				node_addr = search_address (*memory, a);
        	}
           	labels = malloc((n+ 1) * sizeof(char));
           	*labels = '\0';

            ret = fread(labels, 1, n, my_obj_file);
            if (ret != n) {
                return 2;
            }
            labels[n] = '\0';
            node_addr->label = malloc((strlen(labels) + 1 )* sizeof(char));
            strcpy(node_addr->label,labels);
            free(labels);
        }  
        
    }
    return 0;

}


