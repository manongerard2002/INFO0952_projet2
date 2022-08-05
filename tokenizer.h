#ifndef TOKENIZER_H
#define TOKENIZER_H

// Different token types
typedef enum {
  T_SYMBOL,
  T_OPERATOR,
  T_EQUAL,
  T_NUMBER,
  T_LEFTPAR,
  T_RIGHTPAR,
  T_STOP,
} TokenType;

// A structure representing a token.
typedef struct Token_t Token;

// A structure representing a tokenizer.
typedef struct Tokenizer_t Tokenizer;


/* Initialize a new tokenizer.
 *
 * PARAMETERS:
 * - `str`: the expression to tokenize.
 *
 * RETURNS: the new tokenizer.
 */
Tokenizer *tokenizerInit(const char *str);

/* Deallocate the memory occupied by a tokenizer.
 *
 * PARAMETERS:
 * - `tokenizer`: the tokenizer.
 */
void tokenizerFree(Tokenizer *tokenizer);

/* Get the next token.
 *
 * PARAMETERS:
 * - `tokenizer`: the tokenizer.
 *
 * RETURNS: the next token.
 */
Token *tokenizerGetNextToken(Tokenizer *tokenizer);

/* Reset a tokenizer to the beginning of the expression.
 *
 * PARAMETERS:
 * - `tokenizer`: the tokenizer.
 */
void tokenizerReset(Tokenizer *tokenizer);


/* Get the position of a token.
 *
 * PARAMETERS:
 * - `token`: the token.
 *
 * RETURNS: the position of the token in the expression.
 */
int tokenGetPosition(Token *token);

/* Get the type of a token.
 *
 * PARAMETERS:
 * - `token`: the token.
 *
 * RETURNS: the type of the token.
 */
TokenType tokenGetType(Token *token);

/* Get the value of a token.
 *
 * PARAMETERS:
 * - `token`: the token.
 *
 * RETURNS: a void pointer to the value of the token (a 'char*' for a
 * symbol/operator, a 'double*' for a number and NULL otherwise).
 */
void *tokenGetValue(Token *token);

/* Free a token.
 *
 * PARAMETERS:
 * - `token`: the token.
 * - `freeValue`: 0 to free only the token, 1 to also free the value of the token.
 */
void tokenFree(Token *token, int freeValue);

/* Print the token on the terminal.
 *
 * PARAMETERS:
 * - `token`: the token.
 */
void tokenPrint(Token *token);

#endif
