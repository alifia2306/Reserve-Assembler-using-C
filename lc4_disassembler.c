#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_memory.h"
#define getBits5to3(I) ((I>>3) & 0x7)
#define getBits11to9(I) ((I>>9) & 0x7)
#define getBits8to6(I) ((I>>6) & 0x7)
#define getBits2to0(I) ((I>>0) & 0x7)
#define getBits4to0(I) ((I>>0) & 0x1F)

void switch_register(unsigned int registers, char* as_instr){
	/* function to choose register based on linked list contents. */

	switch(registers){
            
            case 0:
                strcat(as_instr, "R0");
                break;

            case 1:
                strcat(as_instr, "R1");
                break;

            case 2:
                strcat(as_instr, "R2");
                break;

            case 3:
                strcat(as_instr, "R3");
                break;

            case 4:
                strcat(as_instr, "R4");
                break;

            case 5:
                strcat(as_instr, "R5");
                break;

            case 6:
                strcat(as_instr, "R6");
                break;

            case 7:
                strcat(as_instr, "R7");
                break;
        }
}

int reverse_assemble (struct row_of_memory* memory) 
{
	/*function to add assembly instructions. */
   unsigned int opcode = 01;
   unsigned int subopcode = 0;
   unsigned int first_register = 0;
   unsigned int second_register = 0;
   unsigned int third_register = 0;
   int imm5 = 0;
   struct row_of_memory *node_addr = NULL;
   int i = 0;
   char* instruction = NULL;
   char add_imm[5]; 
   instruction = malloc((15 + 1)* sizeof(char));
   bzero(instruction, (15 + 1) * sizeof(char));
   *instruction = '\0';
   
   while(1){
       node_addr = search_opcode(memory, opcode);
       if(node_addr == NULL){
           break;
       }
       //gets subopcode
       subopcode = getBits5to3(node_addr->contents);

       //gets first_register
       first_register = getBits11to9(node_addr->contents);

       //gets second_register
       second_register = getBits8to6(node_addr->contents);

       //gets third_register
       third_register = getBits2to0(node_addr->contents);

       //gets the value for add immediate instruction.
       imm5 = getBits4to0(node_addr->contents);

       //sign extension
       if(imm5 & (~0x0F)){
           imm5 = (imm5 | (~(0x0F)));
       }

       // chooses assembly instruction on the basis of opcode
        switch(subopcode){
            case 0:
            case 4:
            case 5:
            case 6:
            case 7:

                strcat(instruction, "ADD ");
                break;

            case 1:
                strcat(instruction, "MUL ");
                break;

            case 2:
                strcat(instruction, "SUB ");
                break;

            case 3:
                strcat(instruction, "DIV ");
        }

        //appends first_register to the instruction.
        switch_register(first_register, instruction);
        strcat(instruction, ", ");

        //appends second_register to the instruction.
        switch_register(second_register, instruction);   
        strcat(instruction, ", ");

        //formats int to string
        sprintf(add_imm, "%d", imm5);

        //if add immediate, appends value to the instruction.
        if(subopcode == 4 || subopcode == 5 || subopcode == 6 || subopcode == 7){
            strcat(instruction, add_imm);
        }

        //if not add immediate, appends third_register to the instruction.
        else{
        	 switch_register(third_register, instruction);
   		}

   	//allocates memory for instruction.
    node_addr->assembly = malloc((strlen(instruction)+1) * sizeof(char));
    bzero(node_addr->assembly, ((strlen(instruction)+1)* sizeof(char)));
    strcpy(node_addr->assembly , instruction);
    *instruction = '\0';
   }

   free(instruction);
    instruction = NULL; 
    return 0 ;
}

int complete_reverse_assembler(struct row_of_memory* memory){

	/* function to create output file containing assembly program.*/

	FILE *outputFilePtr = NULL;
	char filename[100];
	printf("Please Enter Output Filename:  ");
	scanf("%s", filename);
	outputFilePtr = fopen(filename,"w");

	// Checks if output file is created successfully.
	if(outputFilePtr == NULL){
		fprintf(stderr, "Failed to create File %s\n", filename);
		return 1;
	}
	
	
  while(memory != NULL)
  {
  	if(memory -> directive != NULL) {
		fputs(memory->directive, outputFilePtr);
  		fputs("\n", outputFilePtr);
	}
  	if(memory -> label != NULL){
  		fputs(memory->label, outputFilePtr);
  		fputs("\n", outputFilePtr);
  	} 	
  	if(memory -> assembly != NULL){
  		fputs(memory->assembly, outputFilePtr);
  		fputs("\n", outputFilePtr);
  	}	

    memory = memory->next; 
	}
	fclose(outputFilePtr);
	return 0;
}