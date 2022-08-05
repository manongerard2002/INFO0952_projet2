
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tokenizer.h"
#include "calculator.h"
#include "symboltable.h"
#include "shunting-yard.h"

double somme(double a, double b);
double somme(double a, double b)
{
  return (a + b);
}
double difference(double a, double b);
double difference(double a, double b)
{
  return (a - b);
}
double multiplication(double a, double b);
double multiplication(double a, double b)
{
  return (a * b);
}
double division(double a, double b);
double division(double a, double b)
{
  return (a / b);
}
double exposant(double a, double b);
double exposant(double a, double b)
{
  double resultat = a;
  for(int i = 0; i < (b - 1); i++)
  {
    resultat *= a;
  }
  return resultat;
}

SymbolTable *calcInitSymbolTable() {
  SymbolTable *st = stCreate();

  stInsertOperator(st, "+", 1, 0, somme);
  stInsertOperator(st, "-", 1, 0, difference);
  stInsertOperator(st, "*", 2, 0, multiplication);
  stInsertOperator(st, "/", 2, 0, division);
  stInsertOperator(st, "^", 3, 1, exposant);

  stInsertFunction(st, "sin", sin);
  stInsertFunction(st, "cos", cos);
  stInsertFunction(st, "exp", exp);
  stInsertFunction(st, "sqrt", sqrt);
  stInsertFunction(st, "ln", log);
  stInsertFunction(st, "abs", fabs);

  return st;
}

void calcRepl() {

  char *line = NULL;
  size_t len = 0;
  int lineSize;

  SymbolTable *st = calcInitSymbolTable();

  printf("Calculator 0.0.1\n");
  printf("Type \"help\" for more information\n");
  printf(">>> ");

  while ((lineSize = getline(&line, &len, stdin)) > 1) {

    Tokenizer *tokenizer = tokenizerInit(line);
    
    Token *firsttoken = tokenizerGetNextToken(tokenizer);
    
    if (tokenGetType(firsttoken) == T_STOP) {
      tokenFree(firsttoken,1);
      tokenizerFree(tokenizer);
      break;
    }
    
    Token *secondtoken = tokenizerGetNextToken(tokenizer);
    
    if (tokenGetType(firsttoken) == T_SYMBOL
	&& tokenGetType(secondtoken) == T_STOP) {
      char *name = (char *)tokenGetValue(firsttoken);
      if (strcmp(name,"help") == 0) {
	printf("Two types of instructions:\n1) Expr => evaluate the expression.\n2) Var = Expr => evaluate Expr and bind its value to Var.\nTo exit, enter an empty instruction.\n>>> ");
	tokenFree(firsttoken, 1);
	tokenFree(secondtoken, 1);
	tokenizerFree(tokenizer);
	continue;
      }
    }
    
    if (tokenGetType(firsttoken) == T_SYMBOL
	&& tokenGetType(secondtoken) == T_EQUAL) {
      // An assignment
      double *evalresult = malloc(sizeof(double));
      int return_code = syEvaluate(tokenizer, st, evalresult);
      if (return_code==1) {
	// evaluation went well: we store the variable and print the result
	stInsertVariable(st, (char *)tokenGetValue(firsttoken), *evalresult);
	printf("%f\n", *evalresult);
	free(evalresult);
      }
    } else {
      tokenizerReset(tokenizer);
      double *evalresult = malloc(sizeof(double));
      int return_code = syEvaluate(tokenizer, st, evalresult);
      if (return_code == 1) {
	// evaluation went well: we print the value
	printf("%f\n", *evalresult);
      }
      free(evalresult);
    }
    
    tokenFree(firsttoken, 1);
    tokenFree(secondtoken, 1);
    tokenizerFree(tokenizer);
    
    printf(">>> ");
  }
  free(line);
  stFree(st);
}
