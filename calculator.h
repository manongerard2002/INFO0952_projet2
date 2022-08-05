#ifndef CALC_H
#define CALC_H

#include "symboltable.h"

/* Start the Calculator interpreter.
 */
void calcRepl(void);

/* Initialize the symbol table.
 *
 * RETURNS: the symbol table.
 */
SymbolTable *calcInitSymbolTable(void);

#endif
