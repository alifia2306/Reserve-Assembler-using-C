#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"
#include "lc4_loader.h"
#include "lc4_disassembler.h"

/* program to mimic LC4's program & data memory */

int main () {

	/**
	 * main() holds the linked list &
	 * only calls functions in other files 
	 */

	 FILE *filePtr;
	 struct row_of_memory *ret = NULL;
	 int rtrn = 0;

	/* step 1: create head pointer to linked list: memory 	*/
	struct row_of_memory* memory = NULL ;
	
	/* step 2: call function: get_file() in lc4_loader.c 	*/
	filePtr = get_file();
	if(filePtr == NULL){
		fprintf(stderr, "Failed to open File ");
		return 1;
	}

	
	/* step 3: call function: parse_file() in lc4_loader.c 	*/
		rtrn = parse_file(filePtr, &memory);
		if(rtrn != 0){
			fprintf(stderr, "Failed to parse File ");
			return 1;
		}

	/* step 4: call function: reverse_assemble() in lc4_disassembler.c */
		rtrn = reverse_assemble(memory);
		if(rtrn != 0){
			fprintf(stderr, "Failed to assemble File ");
			return 1;
		}

	// sorts list before printing
	sort_list(memory); 
	/* step 5: call function: print_list() in lc4_memory.c 	*/
	print_list(memory);

	// generates output file containing assembly program.
	complete_reverse_assembler(memory);

	/* step 6: call function: delete_list() in lc4_memory.c */
		
		ret = delete_list(memory);
		if(ret != NULL){
			fprintf(stderr, "Failed to delete list ");
			return 1;
		}

	/* only return 0 if everything works properly */
	if(filePtr != NULL){
		if(fclose(filePtr) != 0){
			fprintf(stderr, "Failed to close file ");
			return 2;
		}
	}
	return 0;
}

