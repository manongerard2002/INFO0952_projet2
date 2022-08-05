#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dict.h"
#include "list.h"
#include "symboltable.h"

struct SymbolTable_t{
    //Dict *table_de_hachage;
    Dict *table_de_hachage_fonction;
    Dict *table_de_hachage_operateur;
    Dict *table_de_hachage_variable;
    List *valeurs_allouees;
};

typedef struct Function_t
{
    double (* FunctionFctType)(double);
}Function;

typedef struct Operator_t
{
    double (* OperatorFctType)(double, double);
    int prec, assoc;
}Operator;

SymbolTable *stCreate(){
    SymbolTable *st = malloc(sizeof(SymbolTable));
    if(!st)
        exit (EXIT_FAILURE);
    
    st->table_de_hachage_fonction = dictCreate(500);
    st->table_de_hachage_operateur = dictCreate(500);
    st->table_de_hachage_variable = dictCreate(500);

    //pour les structures intermédiaires :
    st->valeurs_allouees = listCreate();

    return st;
}

void stFree(SymbolTable *st){
    listFree(st->valeurs_allouees);
    dictFree(st->table_de_hachage_fonction);
    dictFree(st->table_de_hachage_operateur);
    dictFree(st->table_de_hachage_variable);
    free(st);
}

void stInsertOperator(SymbolTable *st, char *symbol, int prec, int assoc, OperatorFctType opf){
    Operator *value = malloc(sizeof(Operator));
    if(!value)
        exit (EXIT_FAILURE);
    value->OperatorFctType = opf;
    value->prec = prec;
    value->assoc = assoc;
    listAdd(st->valeurs_allouees, value);
    dictInsert(st->table_de_hachage_operateur, symbol, value);
}

void stInsertFunction(SymbolTable *st, char *symbol, FunctionFctType f){
    Function *value = malloc(sizeof(Function));
    if(!value)
        exit (EXIT_FAILURE);
    value->FunctionFctType = f;
    listAdd(st->valeurs_allouees, value);
    dictInsert(st->table_de_hachage_fonction, symbol, value);
}

void stInsertVariable(SymbolTable *st, char *symbol, double val){
    double *value = malloc(sizeof(double));
    if(!value)
        exit (EXIT_FAILURE);
    *value = val;
    listAdd(st->valeurs_allouees, value);
    dictInsert(st->table_de_hachage_variable, symbol, value);
}

int stContainsVariable(SymbolTable *st, char *symbol){
    return dictContains(st->table_de_hachage_variable, symbol);
}
int stContainsOperator(SymbolTable *st, char *symbol){
    return dictContains(st->table_de_hachage_operateur, symbol);
}
int stContainsFunction(SymbolTable *st, char *symbol){
    return dictContains(st->table_de_hachage_fonction, symbol);
}

OperatorFctType stGetOperatorFct(SymbolTable *st, char *symbol){
    Operator *op = dictSearch(st->table_de_hachage_operateur, symbol);
    return (op->OperatorFctType);
}

int stGetOperatorPrec(SymbolTable *st, char *symbol){
    Operator *op = dictSearch(st->table_de_hachage_operateur, symbol);
    return (op->prec);
}

int stGetOperatorAssoc(SymbolTable *st, char *symbol){
    Operator *op = dictSearch(st->table_de_hachage_operateur, symbol);
    if(op != NULL)
        return (op->assoc);
    else
        return -1;
}

FunctionFctType stGetFunctionFct(SymbolTable *st, char *symbol){
    Function *fct = dictSearch(st->table_de_hachage_fonction, symbol);
    return (fct->FunctionFctType);
}

int stGetVariableValue(SymbolTable *st, char *symbol, double *result){
    if(dictContains(st->table_de_hachage_variable, symbol))
    {
        *result = *(double *)dictSearch(st->table_de_hachage_variable, symbol);
        return 1;
    }
    else
    {
        return 0;
    }
}
