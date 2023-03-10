#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int expanding;
FILE * iptr, * exptr, * namptr, * defptr, * argptr;
char label[100], opcode[100], operand[100];
void getLine();
void processLine();
void define();
void clear_def_and_nam_tab();
void expand();
char macro_start_pointer[100];
int searchnamtab();
void create_positional_map();
void convert_to_positional_notation();
void setup_argtab();
void substitute_positional_notation();

typedef struct positional_mapping //stores each macro parameter and it's positional mappping
{
  char parameter_name[100]; //eg: &indev
  char positional_notation[3]; //eg: ?1
  char sub_value[100]; //value to be substituted
}
positional_mapping;

positional_mapping parameter_map[10];

int main() {
  clear_def_and_nam_tab();
  iptr = fopen("inputprogram.txt", "r");
  exptr = fopen("expandedfile.txt", "w");

  while (strcmp(opcode, "END") != 0) {
    getLine();
    processLine();
  }
  fclose(exptr);
  printf("The macroprocessor has executed successfully.\nOpen expandedfile.txt to see the output");

}

void getLine() {
  if (expanding == 1) {

    fscanf(defptr, "%s%s%s", label, opcode, operand);
    if (operand[0] == '?') {
      substitute_positional_notation();
    }
  } else {
    fscanf(iptr, "%s%s%s", label, opcode, operand);
  }
}

void processLine() {
  if (searchnamtab() == 1) {
    expand();
  } else if (strcmp(opcode, "MACRO") == 0) {
    define();
  } else {
    fprintf(exptr, "%s\t%s\t%s\n", label, opcode, operand);
  }
}

void define() {
  int start, end;
  namptr = fopen("namtab.txt", "a");
  defptr = fopen("deftab.txt", "a");
  fprintf(namptr, "%s", label);
  fprintf(defptr, "%s\t%s\t%s\n", label, opcode, operand);
  create_positional_map();
  start = ftell(defptr);
  int level = 1;
  while (level > 0) {
    getLine();
    fprintf(defptr, "%s\t%s\t", label, opcode);
    convert_to_positional_notation();
    if (strcmp(opcode, "MACRO") == 0) {
      level++;
    } else if (strcmp(opcode, "MEND") == 0) {
      level--;
    }
  }
  end = ftell(defptr);
  fprintf(namptr, "\t%d\t%d\n", start, end);
  fclose(namptr);
  fclose(defptr);

}

void expand() {
  expanding = 1;
  fprintf(exptr,".%s\t%s\t%s\n",label,operand,opcode);
  defptr = fopen("deftab.txt", "r");
  fseek(defptr, atoi(macro_start_pointer), SEEK_SET);
  setup_argtab();
  getLine();
  while (strcmp(opcode, "MEND") != 0) {
    processLine();
    getLine();
  }
  fclose(defptr);
  expanding = 0;
}

void clear_def_and_nam_tab() {
  namptr = fopen("namtab.txt", "w");
  defptr = fopen("deftab.txt", "w");
  fclose(namptr);
  fclose(defptr);
}

int searchnamtab() {
  int found = 0;
  char macro_name[100];
  namptr = fopen("namtab.txt", "r");
  while (!feof(namptr)) {
    fscanf(namptr, "%s", macro_name);
    if (strcmp(opcode, macro_name) == 0) {
      fscanf(namptr, "%s", macro_start_pointer);
      found = 1;
      return found;
    } else {
      fscanf(namptr, "%s", macro_name);
      fscanf(namptr, "%s", macro_name);
    }
  }
  return found;
}

void create_positional_map() {
  int i = 0, j = 0, k = 0;
  while (operand[i] != '\0') {
    if (operand[i] == ',') {
      parameter_map[j].positional_notation[0] = '?';
      parameter_map[j].positional_notation[1] = (char) j + 49; // ascii of 1-9 is 49-57
      j++;
      k = 0;
    } else {
      parameter_map[j].parameter_name[k] = operand[i];
      k++;
    }
    i++;
  }
  parameter_map[j].positional_notation[0] = '?';
  parameter_map[j].positional_notation[1] = (char) j + 49;
}

void convert_to_positional_notation() {
  int i = 0;
  int found = 0;
  while (i < (sizeof(parameter_map) / sizeof(positional_mapping))) {

    if (strcmp(operand, parameter_map[i].parameter_name) == 0) {
      found = 1;
      fprintf(defptr, "%s\n", parameter_map[i].positional_notation);
      break;
    } else {
      i++;
    }
  }
  if (found == 0) {
    fprintf(defptr, "%s\n", operand);
  }
}

void setup_argtab() {
  argptr = fopen("argtab.txt", "w");
  int i = 0, j = 0, k = 0;
  while (operand[i] != '\0') {
    if (operand[i] == ',') {
      parameter_map[j].sub_value[k] = '\0';
      j++;
      k = 0;
      fprintf(argptr, "\n");
    } else {
      fprintf(argptr, "%c", operand[i]);
      parameter_map[j].sub_value[k] = operand[i];
      k++;
    }
    i++;
  }
  parameter_map[j].sub_value[k] = '\0';
  fclose(argptr);
}

void substitute_positional_notation() {
  int i = 0;
  while (i < (sizeof(parameter_map) / sizeof(positional_mapping))) {
    if (strcmp(operand, parameter_map[i].positional_notation) == 0) {
      strcpy(operand, parameter_map[i].sub_value);
      break;
    } else {
      i++;
    }

  }
}
