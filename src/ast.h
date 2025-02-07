#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stdio.h>
#include "token.h"

typedef enum {
    Node_None,
    Node_Numeric,
    Node_BinOp,
    Node_UnOp,
    Node_Assignment,
    Node_Ident,
    Node_If,
    Node_While,
    Node_Function,
    Node_Call,
    Node_Return,
    Node_Program_Root
} NodeType;


typedef struct AstNode{
    NodeType type;
    // Token token; // the token that matches to this node

    union {
        double number;
        char* Ident_or_String;

        struct {
            struct AstNode** items;
            size_t size;
            size_t used;
        }root;

        struct {
            char* variable;
            struct AstNode* value;
        }assignment;

        struct {
            struct AstNode* operand;
            TokenType op;
        } unary_operation;

        struct {
            struct AstNode* left;
            struct AstNode* right;
            TokenType op;
        }binary_operation;

        struct {
            struct AstNode* condition;
            struct AstNode* body;
            struct AstNode* else_body;
        }if_statement;

        struct {
            char* name;
            bool public;
            int param_count;
            struct AstNode** params;
            struct AstNode* body;
        }function;
    } data;
} AstNode;

void print_ast(AstNode* node, int depth);

AstNode* new_node(NodeType type);
AstNode* node_root();
AstNode* node_numeric(double value);
AstNode* node_ident(char* name);
AstNode* node_binary(TokenType op, AstNode* left, AstNode* right);
AstNode* node_unary(TokenType op, AstNode* operand);
AstNode* node_assignment(char* var_name, AstNode* value);
AstNode* node_if_statement(AstNode* condition, AstNode* body, AstNode* else_body);
AstNode* node_function(char* name, bool public, int param_count, AstNode** params, AstNode* body);
#endif //AST_H