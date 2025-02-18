#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stdlib.h>
#include "../lexing/token.h"

typedef enum {
    Node_None,
    Node_Numeric,
    Node_BinOp,
    Node_UnOp,
    Node_Assignment,
    Node_Reassignment,
    Node_Ident,
    Node_If,
    Node_While,
    Node_For,
    Node_Function,
    Node_Call, // todo
    Node_Return,
    Node_Program_Root
} NodeType;


typedef struct AstNode{
    NodeType type;

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
            TokenType type;
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
            struct AstNode** body;
            struct AstNode** else_body;
        }if_statement;

        struct {
            struct AstNode* condition;
            struct AstNode** body;
        }while_loop;

        struct {
            struct AstNode* iterator;
            struct AstNode* iterable;
            struct AstNode** body;
        }for_loop;

        struct {
            char* name;
            bool public;
            int param_count;
            TokenType return_type;
            struct AstNode** params;
            struct AstNode** body;
        }function;

        struct {
            struct AstNode* val;
        }return_statement;

        struct {
            struct AstNode* function;
            struct AstNode** args;
            int arg_count;
        }function_call;
    } data;
} AstNode;

void print_ast(AstNode* node, size_t depth);

AstNode* new_node(NodeType type);

AstNode* node_root();
AstNode* node_numeric(double value);
AstNode* node_ident(char* name);
AstNode* node_binary(TokenType op, AstNode* left, AstNode* right);
AstNode* node_unary(TokenType op, AstNode* operand);
AstNode* node_assignment(char* var_name, AstNode* value, TokenType type);
AstNode* node_reassignment(char* var_name, AstNode* value, TokenType type);
AstNode* node_if_statement(AstNode* condition, AstNode** body, AstNode** else_body);
AstNode* node_while_loop(AstNode* condition, AstNode** body);
AstNode* node_for_loop(AstNode* iterator, AstNode* iterable, AstNode** body);
AstNode* node_function(char* name, bool public, int param_count, AstNode** params, TokenType returns, AstNode** body);
AstNode* node_return(AstNode* expression);
AstNode* node_call(AstNode* function, AstNode** args, int arg_count);

#endif //AST_H