#include "ast.h"
#include "token.h"

#include <stdio.h>
#include <stdlib.h>

AstNode* new_node(NodeType type) {
    AstNode* node = malloc(sizeof(AstNode));
    node->type = type;
    return node;
}

AstNode* node_root() {
    AstNode* node = new_node(Node_Program_Root);
    node->data.root.size = 10;
    node->data.root.used = 0;
    node->data.root.items = malloc(sizeof(AstNode*) * node->data.root.size);
    return node;
}

AstNode* node_numeric(double value) {
    AstNode* node = new_node(Node_Numeric);
    node->data.number = value;
    return node;
}

AstNode* node_ident(char* name) {
    AstNode* node = new_node(Node_Ident);
    node->data.Ident_or_String = name;
    return node;
}

AstNode* node_unary(TokenType op, AstNode* operand) {
    AstNode* node = new_node(Node_UnOp);
    node->data.unary_operation.op = op;
    node->data.unary_operation.operand = operand;
    return node;
}

AstNode* node_binary(TokenType op, AstNode* left, AstNode* right) {
    AstNode* node = new_node(Node_BinOp);
    node->data.binary_operation.op = op;
    node->data.binary_operation.left = left;
    node->data.binary_operation.right = right;
    return node;
}

AstNode* node_assignment(char* var_name, AstNode* value) {
    AstNode* node = new_node(Node_Assignment);
    node->data.assignment.variable = var_name;
    node->data.assignment.value = value;
    return node;
}

AstNode* node_if_statement(AstNode* condition, AstNode* body, AstNode* else_body) {
    AstNode* node = new_node(Node_If);
    node->data.if_statement.condition = condition;
    node->data.if_statement.body = body;
    node->data.if_statement.else_body = else_body;
    return node;
}

AstNode* node_function(char* name, bool public, int param_count, AstNode** params, AstNode* body) {
    AstNode* node = new_node(Node_Function);
    node->data.function.name = name;
    node->data.function.public = public;
    node->data.function.param_count = param_count;
    // node->data.function.params = malloc(sizeof(AstNode) * param_count);
    node->data.function.params = params;
    node->data.function.body = body;
    return node;
}

char* node_type_to_str(NodeType type) {
    static char* strings[] = {
        "Node_None",
        "Node_Numeric",
        "Node_BinOp",
        "Node_UnOp",
        "Node_Assignment",
        "Node_Ident",
        "Node_If",
        "Node_While",
        "Node_Function",
        "Node_Call",
        "Node_Return"
    };
    return strings[type];
}

void print_ast(AstNode* node, int depth) {
    for (int i = 0; i < depth; i++) { printf("  "); }

    switch (node->type) {
        case Node_Program_Root:
            printf("Program: \n");
            for (size_t i = 0; i < node->data.root.used; ++i) {
                print_ast(node->data.root.items[i], depth + 1);
            }
            break;
        case Node_Numeric:
            printf("Number: %f\n", node->data.number);
            break;
        case Node_Ident:
            printf("Identifier: %s\n", node->data.Ident_or_String);
            break;
        case Node_BinOp:
            printf("Binary Op: %s\n", token_to_string(node->data.binary_operation.op));
            print_ast(node->data.binary_operation.left, depth + 1);
            print_ast(node->data.binary_operation.right, depth + 1);
            break;
        case Node_UnOp:
            printf("Unary Op: %s\n", token_to_string(node->data.unary_operation.op));
            print_ast(node->data.unary_operation.operand, depth + 1);
            break;
        case Node_Assignment:
            printf("Assign: %s\n", node->data.assignment.variable);
            print_ast(node->data.assignment.value, depth + 1);
            break;
        case Node_If:
            printf("If: \n");
            print_ast(node->data.if_statement.condition, depth + 1);
            for (int i = 0; i < depth + 1; i++) { printf("  "); }
            printf("Then: \n");
            print_ast(node->data.if_statement.body, depth + 2);
            if (node->data.if_statement.else_body) {
                for (int i = 0; i < depth + 1; i++) { printf("  "); }
                printf("Else: \n");
                print_ast(node->data.if_statement.else_body, depth + 2);
            }
            break;
        case Node_Function:
                printf("Function: %s\n", node->data.function.name);
                for (int i = 0; i < depth + 1; i++) { printf("  "); }
                printf("Visibility -> ");
                if (node->data.function.public) {
                    printf("Public\n");
                } else {
                    printf("Private\n");
                }
                if (node->data.function.param_count) {
                    for (int i = 0; i < depth + 1; i++) { printf("  "); }
                    printf("Params:\n");
                    for (size_t i = 0; i < node->data.function.param_count; ++i) {
                        print_ast(node->data.function.params[i], depth + 2);
                    }
                }
                for (int i = 0; i < depth + 1; i++) { printf("  "); }
                printf("Body:\n");
                print_ast(node->data.function.body, depth + 2);
            break;
        default:
            printf("Unknown node type: %s\n", node_type_to_str(node->type));
    }
}