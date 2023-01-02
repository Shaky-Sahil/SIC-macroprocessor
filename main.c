#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int expanding;
FILE *iptr,*exptr,*namptr,*defptr;
char label[100],opcode[100],operand[100];
void getLine();
void processLine();
void define();
void clear_def_and_nam_tab();
void expand();

int main()
{
    clear_def_and_nam_tab();
    iptr = fopen("inputprogram.txt","r");
    exptr = fopen("expandedfile.txt","w");

    while(strcmp(opcode,"END")!=0)
    {
        getLine();
        processLine();

    }
    fclose(exptr);

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
    if(strcmp(opcode,"MACRO")==0)
    {
        define();
    }
    else{
    fprintf(exptr,"%s\t%s\t%s\n",label,opcode,operand);
    }
}

void define()
{

    namptr = fopen("namtab.txt","a");
    defptr = fopen("deftab.txt","a");
    fprintf(namptr,"%s",label);
    fprintf(defptr,"%s\t%s\t%s\n",label,opcode,operand);
    int level = 1;
    while(level>0){
            getLine();
            //todo sub positional notation
            fprintf(defptr,"%s\t%s\t%s\n",label,opcode,operand);
             if(strcmp(opcode,"MACRO")==0){
                level++;
             }
             else if(strcmp(opcode,"MEND")==0){
                    level--;
             }
    }
    fclose(namptr);
    fclose(defptr);
    //todo store pointers

}

void expand()
{
    expanding = 1;
}

void clear_def_and_nam_tab()
{
    namptr = fopen("namtab.txt","w");
    defptr = fopen("deftab.txt","w");
    fclose(namptr);
    fclose(defptr);
}
