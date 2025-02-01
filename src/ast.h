#ifndef AST_H
#define AST_H

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
    Node_Return
} NodeType;

typedef struct {
    NodeType type;
    // Token token; // the token that matches to this node

    union {
        double number;
        char* Ident_or_String;

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
            int param_count;
            struct AstNode** params;
            struct AstNode* body;
        };
    } data;
} AstNode;

void print_ast(AstNode* node, int depth);

AstNode* new_node(NodeType type);
AstNode* node_numeric(double value);
AstNode* node_binary(TokenType op, AstNode* left, AstNode* right);
AstNode* node_unary(TokenType op, AstNode* operand);
#endif //AST_H
