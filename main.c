#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int expanding;
FILE* iptr;
char label[100],opcode[100],operand[100];
void getLine();
void processLine();


int main()
{

    iptr = fopen("inputprogram.txt","r");
    while(strcmp(opcode,"END")!=0)
    {
        getLine();
        //processline

    }

}

void getLine(){
    if(expanding == 1)
    {
        printf("reading from deftab");
    }
    else{
        fscanf(iptr,"%s%s%s",label,opcode,operand);
        //printf("the label is %s the opcode is %s the operand is %s\n",label,opcode,operand);
    }
}

void processLine()
{

}
