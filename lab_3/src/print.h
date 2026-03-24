#ifndef PRINT_H
#define PRINT_H

#include "ast.h"
#include <stdio.h>

void print_tree(const ASTNode *node, int depth);
void print_expression(const ASTNode *node, FILE *out);

#endif