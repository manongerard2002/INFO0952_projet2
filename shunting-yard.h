#ifndef SY_H
#define SY_H

#include "tokenizer.h"
#include "symboltable.h"


/* Evaluate the expression output by the tokenizer.
 *
 * PARAMETERS:
 * - `tokenizer`: the tokenizer.
 * - `st`: the symbol table.
 * - `solution`: a pointer to a double where the value of the expression will be
 *   stored.
 *
 * RETURNS: 1 if no error occured, 0 otherwise.
 */
int syEvaluate(Tokenizer *tokenizer, SymbolTable *st, double* solution);

#endif
