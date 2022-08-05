#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "tokenizer.h"

struct Token_t {
  TokenType type;
  int pos;
  void *value;
};

struct Tokenizer_t {
  const char *str;
  int pos;
  TokenType lastType;
};

static int isNumber(const char *str, TokenType lastType);

static int isNumber(const char *str, TokenType lastType) {
  if (str[0] == '\0')
    return 0;

  if (isdigit(str[0]))
    return 1;

  if (str[1] == '\0')
    return 0;

  if (str[0] == '.' && isdigit(str[1]))
    return 1;

  if ((lastType == T_LEFTPAR || lastType == T_OPERATOR || lastType == T_EQUAL) && str[0] == '-' && isdigit(str[1]))
    return 1;

  if (str[2] == '\0')
    return 0;

  if ((lastType == T_LEFTPAR || lastType == T_OPERATOR || lastType == T_EQUAL) && str[0] == '-' && str[1] == '.' && isdigit(str[2]))
    return 1;

  return 0;
}

static char *createString(int length) {
  char *str = malloc((length+1) * sizeof(char));
  if (!str) {
    fprintf(stderr, "tokenizerGetNextToken: error while allocating memory.\n");
    exit(1);
  }
  return str;
}

Tokenizer *tokenizerInit(const char *str) {
  Tokenizer *t = malloc(sizeof(Tokenizer));
  if (!t) {
    fprintf(stderr, "tokenizerInit: error while allocating memory.\n");
    exit(1);
  }

  t->str = str;
  t->pos = 0;
  t->lastType = T_LEFTPAR;

  return t;
}

void tokenizerFree(Tokenizer *tokenizer) {
  free(tokenizer);
}

Token *tokenizerGetNextToken(Tokenizer *tokenizer) {
  int pos = tokenizer->pos;
  const char *str = tokenizer->str;

  while (isspace(str[pos])) {
    pos++;
    continue;
  }

  Token *token = malloc(sizeof(Token));
  if (!token) {
    fprintf(stderr, "tokenizerGetNextToken: error while allocating memory.\n");
    exit(1);
  }

  token->pos = pos;

  char c, *cptr;
  int l;
  double *dptr;
  while (str[pos] != '\0') {
    c = str[pos];

    // Left parenthesis
    if (c == '(') {
      token->type = T_LEFTPAR;
      token->value = NULL;
      tokenizer->pos = pos + 1;
      tokenizer->lastType = T_LEFTPAR;
      return token;
    }

    // Right parenthesis
    if (c == ')') {
      token->type = T_RIGHTPAR;
      token->value = NULL;
      tokenizer->pos = pos + 1;
      tokenizer->lastType = T_RIGHTPAR;
      return token;
    }

    // Equal for an assignment
    if (c == '=') {
      token->type = T_EQUAL;
      token->value = NULL;
      tokenizer->pos = pos + 1;
      tokenizer->lastType = T_EQUAL;
      return token;
    }

    // A number (float or int)
    if (isNumber(str+pos, tokenizer->lastType)) {
      dptr = malloc(sizeof(double));
      if (!dptr) {
        fprintf(stderr, "tokenizerGetNextToken: error while allocating memory.\n");
        exit(1);
      }

      *dptr = strtod(str + pos, &cptr);
      pos += (int) (cptr - (str + pos));

      token->value =  dptr;
      token->type = T_NUMBER;
      tokenizer->pos = pos;
      tokenizer->lastType = T_NUMBER;
      return token;
    }

    // A symbol (function name or variable)
    // ( = a letter followed by any combination of letters and digits and underscore)

    if (isalpha(c)) {
      l = pos+1;
      while (str[l] != '\0' && (isalpha(str[l]) || isdigit(str[l])))
	l++;
      l -= pos;
      cptr = createString(l);
      strncpy(cptr, str + pos, l);
      cptr[l] = '\0';
      token->value = cptr;
      token->type = T_SYMBOL;
      tokenizer->pos = pos + l;
      tokenizer->lastType = T_SYMBOL;
      return token;
    }

    // An operator (any single character, which is neither a digit, '.', '(',')',
    //              or a letter, is considered as an operator)
    cptr = createString(1);
    strncpy(cptr, str + pos, 1);
    cptr[1] = '\0';
    
    token->value = cptr;
    token->type = T_OPERATOR;
    tokenizer->pos = pos + 1;
    tokenizer->lastType = T_OPERATOR;
    return token;
  }

  token->type = T_STOP;
  token->value = NULL;
  tokenizer->pos = pos;
  tokenizer->lastType = T_STOP;
  return token;
}

void tokenizerReset(Tokenizer *t) {
  t->pos = 0;
  t->lastType = T_LEFTPAR;
}

int tokenGetPosition(Token *token) {
  return token->pos;
}

TokenType tokenGetType(Token *token) {
  return token->type;
}

void *tokenGetValue(Token *token) {
  return token->value;
}

void tokenFree(Token *token, int freeValue) {
  if (freeValue && (token->type == T_SYMBOL || token->type == T_NUMBER || token->type == T_OPERATOR))
    free(token->value);

  free(token);
}

// For debugging
void tokenPrint(Token *token) {
  switch(token->type) {
  case T_SYMBOL:
    printf("<SYMBOL,%d,%s>", token->pos, (char *)token->value); break;
  case T_OPERATOR:
    printf("<OPERATOR,%d,%s>", token->pos, (char *)token->value); break;
  case T_EQUAL:
    printf("<EQUAL,%d,NULL>", token->pos); break;
  case T_NUMBER:
    printf("<NUMBER,%d,%f>", token->pos, *((double *)token->value)); break;
  case T_LEFTPAR:
    printf("<LEFTPAR,%d,NULL>", token->pos); break;
  case T_RIGHTPAR:
    printf("<RIGHTPAR,%d,NULL>", token->pos); break;
  case T_STOP:
    printf("<STOP,%d,NULL>", token->pos); break;
  }
}

//AJOUTE
void tokenPrint_2(Token *token);
void tokenPrint_2(Token *token) {
  switch(token->type) {
  case T_SYMBOL:
    fprintf(stderr, "<SYMBOL,%d,%s>", token->pos, (char *)token->value); break;
  case T_OPERATOR:
    fprintf(stderr, "<OPERATOR,%d,%s>", token->pos, (char *)token->value); break;
  case T_EQUAL:
    fprintf(stderr, "<EQUAL,%d,NULL>", token->pos); break;
  case T_NUMBER:
    fprintf(stderr, "<NUMBER,%d,%f>", token->pos, *((double *)token->value)); break;
  case T_LEFTPAR:
    fprintf(stderr, "<LEFTPAR,%d,NULL>", token->pos); break;
  case T_RIGHTPAR:
    fprintf(stderr, "<RIGHTPAR,%d,NULL>", token->pos); break;
  case T_STOP:
    fprintf(stderr, "<STOP,%d,NULL>", token->pos); break;
  }
}
