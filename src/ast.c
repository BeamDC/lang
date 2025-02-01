#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

AstNode* new_node(NodeType type) {
    AstNode* node = malloc(sizeof(AstNode));
    node->type = type;
    return node;
}

AstNode* node_numeric(double value) {
    AstNode* node = new_node(Node_Numeric);
    node->data.number = value;
    return node;
}

AstNode* node_unary(TokenType op, AstNode* operand) {
    AstNode* node = new_node(Node_UnOp);
    node->data.unary_operation.op = op;
    node->data.unary_operation.operand = (struct AstNode*)operand;
    return node;
}

AstNode* node_binary(TokenType op, AstNode* left, AstNode* right) {
    AstNode* node = new_node(Node_BinOp);
    node->data.binary_operation.op = op;
    node->data.binary_operation.left = (struct AstNode*)left;
    node->data.binary_operation.right = (struct AstNode*)right;
    return node;
}

void print_ast(AstNode* node, int depth) {
    for (int i = 0; i < depth; i++) printf("  ");

    switch (node->type) {
        case Node_Numeric:
            printf("Number: %f\n", node->data.number);
            break;
        case Node_BinOp:
            printf("Binary Op: %s\n", token_to_string(node->data.binary_operation.op));
            print_ast((AstNode*)node->data.binary_operation.left, depth + 1);
            print_ast((AstNode*)node->data.binary_operation.right, depth + 1);
            break;
        case Node_UnOp:
            printf("Unary Op: %s\n", token_to_string(node->data.unary_operation.op));
            print_ast((AstNode*)node->data.unary_operation.operand, depth + 1);
            break;
        default:
            printf("Unknown node type\n");
    }
}