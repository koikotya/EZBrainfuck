#ifndef INCLUDED_FUNC
#define INCLUDED_FUNC

#include "ast.h"

int size(Variable *v) ;
int getIndex(Operation op) ;
void allocate(Variable *v,int type) ;
void freeVariable(Variable *v) ;
void setLiteral(Variable *v1,Variable *v2,int index1) ;
Type castType(Type t1,Type t2) ;
void setValue(Variable *v1,Variable *v2,int index1) ;
void printErr(char *s) ;
void output(char *s) ;

extern int list_size;
extern Variable *val_list[100010];

extern int used_memory[2];

#endif