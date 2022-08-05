#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"
#include "symboltable.h"
#include "tokenizer.h"
#include "shunting-yard.h"

void arret_du_programme(Token *token_actuel, Stack *Sop, Stack *Sval);
void arret_du_programme(Token *token_actuel, Stack *Sop, Stack *Sval)
{
    tokenFree(token_actuel, 1);
    stackFree(Sop);
    stackFree(Sval);
    exit(EXIT_FAILURE);
}

int syEvaluate(Tokenizer *tokenizer, SymbolTable *st, double* solution){
    
    Stack *Sop = stackCreate();
    Stack *Sval = stackCreate();

    Token *token_actuel = tokenizerGetNextToken(tokenizer);

    int nombre_parentheses_ouvertes = 0;
    int pos_derniere_parenthese = 0;
    
    while(tokenGetType(token_actuel) != T_STOP)
    {
        TokenType token_actuel_type = tokenGetType(token_actuel);
        Token *token_suivant = tokenizerGetNextToken(tokenizer);

        if(token_actuel_type == T_NUMBER)
        {
            if(tokenGetType(token_suivant) == T_NUMBER)
            {
                fprintf(stderr, "Un nombre est suivi d'un nombre a la position %d\n", tokenGetPosition(token_suivant) + 1);
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            if(tokenGetType(token_suivant) == T_SYMBOL)
            {
                fprintf(stderr, "Un nombre est suivi d'un symbole a la position %d\n", tokenGetPosition(token_suivant) + 1);
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            if(tokenGetType(token_suivant) == T_LEFTPAR)
            {
                fprintf(stderr, "Un nombre est suivi d'une parenthese a gauche a la position %d\n", tokenGetPosition(token_suivant) + 1);
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            stackPush(Sval, (double *)tokenGetValue(token_actuel));
        }

        if(token_actuel_type == T_LEFTPAR)
        {
            if(tokenGetType(token_suivant) == T_RIGHTPAR)
            {
                fprintf(stderr, "Parenthese fermee directement apres en avoir ouverte une a la position : %d\n", tokenGetPosition(token_suivant) + 1);
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            if(tokenGetType(token_suivant) == T_STOP)
            {
                fprintf(stderr, "Parenthese gauche non fermee a la position %d\n", tokenGetPosition(token_suivant));
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            if(tokenGetType(token_suivant) == T_OPERATOR)
            {
                fprintf(stderr, "Parenthese gauche suivie d'un operateur %d\n", tokenGetPosition(token_suivant) + 1);
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            nombre_parentheses_ouvertes++;
            pos_derniere_parenthese = tokenGetPosition(token_actuel);
            stackPush(Sop, token_actuel);
        }

        if(token_actuel_type == T_SYMBOL)
        {            
            if(tokenGetType(token_suivant) == T_LEFTPAR)
            {
                if(stContainsFunction(st, (char *)tokenGetValue(token_actuel)) == 0)
                {
                    fprintf(stderr, "La fonction a la position %d n'existe pas\n", tokenGetPosition(token_actuel) + 1);
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                stackPush(Sop, token_actuel);
            }
            else if(tokenGetType(token_suivant) == T_NUMBER)
            {
                fprintf(stderr, "Une variable est suivie d'un nombre a la position %d\n", tokenGetPosition(token_suivant));
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            else if(tokenGetType(token_suivant) == T_SYMBOL)
            {
                fprintf(stderr, "Une variable est suivie d'une variable a la position %d\n", tokenGetPosition(token_suivant) + 1);
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            else
            {
                double *result = malloc(sizeof(double));
                if(!result)
                {
                    fprintf(stderr, "Un malloc a echoue\n");
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                int recuperation_variable = stGetVariableValue(st, (char *)tokenGetValue(token_actuel), result);
                if(recuperation_variable == 0)
                {
                    fprintf(stderr, "La variable a la position %d n'existe pas\n", tokenGetPosition(token_actuel) + 1);
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                stackPush(Sval, result);
            }
        }

        if(token_actuel_type == T_OPERATOR)
        {
            if(stContainsOperator(st, (char *)tokenGetValue(token_actuel)) == 0)
            {
                fprintf(stderr, "L'operateur a la position %d n'existe pas\n", tokenGetPosition(token_actuel) + 1);
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            if(tokenGetType(token_suivant) == T_RIGHTPAR)
            {
                fprintf(stderr, "Un operateur est suivi d'une parenthese a droite a la position %d\n", tokenGetPosition(token_suivant) + 1);
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            if(tokenGetType(token_suivant) == T_STOP)
            {
                fprintf(stderr, "Un operateur est suivi de rien a la position %d\n", tokenGetPosition(token_suivant));
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            if(tokenGetType(token_suivant) == T_OPERATOR)
            {
                fprintf(stderr, "Un operateur est suivi d'un operateur a la position %d\n", tokenGetPosition(token_suivant) + 1);
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            while(stackIsEmpty(Sop) == 0 && tokenGetType(stackTop(Sop)) == T_OPERATOR && ((stGetOperatorPrec(st, (char *)tokenGetValue(stackTop(Sop))) > stGetOperatorPrec(st, (char *)tokenGetValue(token_actuel))) || (stGetOperatorPrec(st, (char *)tokenGetValue(stackTop(Sop))) == stGetOperatorPrec(st, (char *)tokenGetValue(token_actuel)) && stGetOperatorAssoc(st, (char *)tokenGetValue(token_actuel)) == 0)))
            {
                Token *token = stackPop(Sop);
                if(stContainsOperator(st, (char *)tokenGetValue(token)) == 0)
                {
                    fprintf(stderr, "L'operateur a la position %d n'existe pas\n", tokenGetPosition(token) + 1);
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                OperatorFctType operateur = stGetOperatorFct(st, (char *)tokenGetValue(token));

                if(stackIsEmpty(Sval) != 0)
                {
                    fprintf(stderr, "Sval est vide");
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                double *v1 = stackPop(Sval);
                if(stackIsEmpty(Sval) != 0)
                {
                    fprintf(stderr, "Sval est vide");
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                double *v2 = stackPop(Sval);
                double resultat = operateur(*v2, *v1);
                double *res = malloc(sizeof(double));
                if(!res)
                {
                    fprintf(stderr, "Un malloc a echoue\n");
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                *res = resultat;
                stackPush(Sval, res);
                tokenFree(token, 1);
                free(v1);
                free(v2);
            }
            
            stackPush(Sop, token_actuel);
        }


        if(token_actuel_type == T_RIGHTPAR)
        {
            if(tokenGetType(token_suivant) == T_LEFTPAR)
            {
                fprintf(stderr, "Une parenthese droite est suivie d'une parenthese gauche a la position %d\n", tokenGetPosition(token_suivant) + 1);
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            if(tokenGetType(token_suivant) == T_NUMBER)
            {
                fprintf(stderr, "Une parenthese droite est suivie d'un nombre a la position %d\n", tokenGetPosition(token_suivant) + 1);
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            if(tokenGetType(token_suivant) == T_SYMBOL)
            {
                fprintf(stderr, "Une parenthese droite est suivie d'un symbole a la position %d\n", tokenGetPosition(token_suivant));
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            nombre_parentheses_ouvertes--;

            while(stackIsEmpty(Sop) == 0 && tokenGetType(stackTop(Sop)) != T_LEFTPAR)
            {
                Token *token = stackPop(Sop);
                if(stContainsOperator(st, (char *)tokenGetValue(token)) == 0)
                {
                    fprintf(stderr, "L'operateur a la position %d n'existe pas\n", tokenGetPosition(token) + 1);
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                OperatorFctType operateur = stGetOperatorFct(st, (char *)tokenGetValue(token));

                if(stackIsEmpty(Sval) != 0)
                {
                    fprintf(stderr, "Sval est vide");
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                double *v1 = stackPop(Sval);
                if(stackIsEmpty(Sval) != 0)
                {
                    fprintf(stderr, "Sval est vide");
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                double *v2 = stackPop(Sval);
                double resultat = operateur(*v2, *v1);
                double *res = malloc(sizeof(double));
                if(!res)
                {
                    fprintf(stderr, "Un malloc a echoue\n");
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                *res = resultat;
                stackPush(Sval, res);
                tokenFree(token, 1);
                free(v1);
                free(v2);
            }

            if(stackIsEmpty(Sop) != 0)
            {
                fprintf(stderr, "Il y a pas le meme nombre de parentheses droites et gauches. La derniere parenthese se trouve a la position %d\n", pos_derniere_parenthese + 1);
                tokenFree(token_suivant, 1);
                arret_du_programme(token_actuel, Sop, Sval);
            }
            Token *token = stackPop(Sop);
            tokenFree(token, 0);

            if(!stackIsEmpty(Sop) && tokenGetType(stackTop(Sop)) == T_SYMBOL)
            {
                if(stackIsEmpty(Sval) != 0)
                {
                    fprintf(stderr, "Sval est vide");
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                double *v = stackPop(Sval);
                Token *token = stackPop(Sop);
                if(stContainsFunction(st, (char *)tokenGetValue(token)) == 0)
                {
                    fprintf(stderr, "La fonction a la position %d n'existe pas\n", tokenGetPosition(token) + 1);
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                FunctionFctType function = stGetFunctionFct(st, (char *)tokenGetValue(token));
                double resultat = function(*v);
                double *res = malloc(sizeof(double));
                if(!res)
                {
                    fprintf(stderr, "Un malloc a echoue\n");
                    tokenFree(token_suivant, 1);
                    arret_du_programme(token_actuel, Sop, Sval);
                }
                *res = resultat;
                stackPush(Sval, res);
                tokenFree(token, 1);
                free(v);
            }
        }
        
        //gestion au cas où un symbol serait egal, donc qu'on a eu une expression avec plus d'un egal
        if(token_actuel_type == T_EQUAL)
        {
            fprintf(stderr, "Il y a un probleme avec le signe egal a la position %d\n", tokenGetPosition(token_actuel) + 1);
            tokenFree(token_suivant, 1);
            arret_du_programme(token_actuel, Sop, Sval);
        }
        
        token_actuel = token_suivant;
    }

    if(nombre_parentheses_ouvertes != 0)
    {
        fprintf(stderr, "Il y a pas le meme nombre de parentheses droites et gauches. La derniere perenthese se trouve a la position %d\n", pos_derniere_parenthese + 1);
        arret_du_programme(token_actuel, Sop, Sval);
    }

    while(stackIsEmpty(Sop) == 0 && tokenGetType(stackTop(Sop)) == T_OPERATOR)
    {
        Token *token = stackPop(Sop);
        if(stContainsOperator(st, (char *)tokenGetValue(token)) == 0)
        {
            fprintf(stderr, "L'operateur a la position %d n'existe pas\n", tokenGetPosition(token) + 1);
            arret_du_programme(token_actuel, Sop, Sval);
        }
        OperatorFctType operateur = stGetOperatorFct(st, (char *)tokenGetValue(token));

        if(stackIsEmpty(Sval) != 0)
        {
            fprintf(stderr, "Sval est vide");
            arret_du_programme(token_actuel, Sop, Sval);
        }
        double *v1 = stackPop(Sval);
        if(stackIsEmpty(Sval) != 0)
        {
            fprintf(stderr, "Sval est vide");
            arret_du_programme(token_actuel, Sop, Sval);
        }
        double *v2 = stackPop(Sval);
        double resultat = operateur(*v2, *v1);
        double *res = malloc(sizeof(double));
        if(!res)
        {
            fprintf(stderr, "Un malloc a echoue\n");
            arret_du_programme(token_actuel, Sop, Sval);
        }
        *res = resultat;
        stackPush(Sval, res);
        tokenFree(token, 1);
        free(v1);
        free(v2);
    }

    if(stackIsEmpty(Sval) != 0)
    {
        fprintf(stderr, "A la fin Sval est vide");
        arret_du_programme(token_actuel, Sop, Sval);
    }

    *solution = *(double *)stackPop(Sval);

    tokenFree(token_actuel, 1);
    stackFree(Sop);
    stackFree(Sval);
    
    return 1;
}