#ifndef H_ASMVARS_H
#define H_ASMVARS_H

void asmvars_add(const char key[], int val);
int asmvars_value(const char key[], int *result);
void asmvars_print();
#endif
