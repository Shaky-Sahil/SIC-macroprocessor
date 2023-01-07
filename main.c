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
char macro_start_pointer[100];
int searchnamtab();

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

       fscanf(defptr,"%s%s%s",label,opcode,operand);
    }
    else{
        fscanf(iptr,"%s%s%s",label,opcode,operand);
    }
}

void processLine()
{
    if(searchnamtab()==1)
    {
        expand();
    }
    else if(strcmp(opcode,"MACRO")==0)
    {
        define();
    }
    else{
    fprintf(exptr,"%s\t%s\t%s\n",label,opcode,operand);
    }
}

void define()
{
    int start,end;
    namptr = fopen("namtab.txt","a");
    defptr = fopen("deftab.txt","a");
    fprintf(namptr,"%s",label);
    fprintf(defptr,"%s\t%s\t%s\n",label,opcode,operand);
    start = ftell(defptr);
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
    end = ftell(defptr);
    fprintf(namptr,"\t%d\t%d\n",start,end);
    fclose(namptr);
    fclose(defptr);

}

void expand()
{
    expanding = 1;
    defptr = fopen("deftab.txt","r");
    fseek(defptr,atoi(macro_start_pointer),SEEK_SET);
    //todo add comment
    getLine();
    while(strcmp(opcode,"MEND")!=0)
    {
        processLine();
        getLine();
    }
    fclose(defptr);
    expanding = 0;
}

void clear_def_and_nam_tab()
{
    namptr = fopen("namtab.txt","w");
    defptr = fopen("deftab.txt","w");
    fclose(namptr);
    fclose(defptr);
}

int searchnamtab()
{
    int found = 0;
    char macro_name[100];
    namptr = fopen("namtab.txt","r");
    while(!feof(namptr))
    {
        fscanf(namptr,"%s",macro_name);
        if(strcmp(opcode,macro_name)==0)
        {
            fscanf(namptr,"%s",macro_start_pointer);
            found = 1;
            return found;
        }
        else{
            fscanf(namptr,"%s",macro_name);
            fscanf(namptr,"%s",macro_name);
        }
    }
    return found;
}
