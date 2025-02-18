#include "ast.h"

#include <math.h>

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

AstNode* node_assignment(char* var_name, AstNode* value, TokenType type) {
    AstNode* node = new_node(Node_Assignment);
    node->data.assignment.variable = var_name;
    node->data.assignment.value = value;
    node->data.assignment.type = type;
    return node;
}

AstNode* node_reassignment(char* var_name, AstNode* value, TokenType type) {
    AstNode* node = new_node(Node_Reassignment);
    node->data.assignment.variable = var_name;
    node->data.assignment.value = value;
    node->data.assignment.type = type;
    return node;
}

AstNode* node_if_statement(AstNode* condition, AstNode** body, AstNode** else_body) {
    AstNode* node = new_node(Node_If);
    node->data.if_statement.condition = condition;
    node->data.if_statement.body = body;
    node->data.if_statement.else_body = else_body;
    return node;
}

AstNode* node_while_loop(AstNode* condition, AstNode** body) {
    AstNode* node = new_node(Node_While);
    node->data.while_loop.condition = condition;
    node->data.while_loop.body = body;
    return node;
}

AstNode* node_for_loop(AstNode* iterator, AstNode* iterable, AstNode** body) {
    AstNode* node = new_node(Node_For);
    node->data.for_loop.iterator = iterator;
    node->data.for_loop.iterable = iterable;
    node->data.for_loop.body = body;
    return node;
}

AstNode* node_function(char* name, bool public, int param_count, AstNode** params, TokenType returns, AstNode** body) {
    AstNode* node = new_node(Node_Function);
    node->data.function.name = name;
    node->data.function.public = public;
    node->data.function.param_count = param_count;
    node->data.function.params = params;
    node->data.function.return_type = returns;
    node->data.function.body = body;
    return node;
}

AstNode* node_return(AstNode* expression) {
    AstNode* node = new_node(Node_Return);
    node->data.return_statement.val = expression;
    return node;
}

AstNode* node_call(AstNode* function, AstNode** args, int arg_count) {
    AstNode* node = new_node(Node_Call);
    node->data.function_call.function = function;
    node->data.function_call.args = args;
    node->data.function_call.arg_count = arg_count;
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

void print_scope(AstNode** scope, size_t depth) {
    AstNode** current = scope;
    while (*current) {
        print_ast(*current, depth);
        current++;
    }
}

void print_if(AstNode* node, size_t depth) {
    printf("If: \n");
    print_ast(node->data.if_statement.condition, depth);
    for (int i = 0; i < depth; i++) { printf("  "); }
    printf("Then: \n");
    print_scope(node->data.if_statement.body, depth + 1);

    if (node->data.if_statement.else_body) {
        for (int i = 0; i < depth; i++) { printf("  "); }
        printf("Else: \n");
        print_scope(node->data.if_statement.else_body, depth + 1);
    }
}

void print_while(AstNode* node, size_t depth) {
    printf("While: \n");
    print_ast(node->data.while_loop.condition, depth);
    for (int i = 0; i < depth; i++) { printf("  "); }
    printf("Body: \n");
    print_scope(node->data.while_loop.body, depth + 1);
}

void print_for(AstNode* node, size_t depth) {
    printf("For: \n");

    for (int i = 0; i < depth; i++) { printf("  "); }
    printf("Iterator: \n");
    print_ast(node->data.for_loop.iterator, depth + 1);

    for (int i = 0; i < depth; i++) { printf("  "); }
    printf("Iterable: \n");
    print_ast(node->data.for_loop.iterable, depth + 1);

    for (int i = 0; i < depth; i++) { printf("  "); }
    printf("Body: \n");
    print_scope(node->data.for_loop.body, depth + 1);
}

void print_function(AstNode* node, size_t depth) {
    printf("Function: %s\n", node->data.function.name);
    for (int i = 0; i < depth; i++) { printf("  "); }
    printf("Visibility -> ");
    if (node->data.function.public) {
        printf("Public\n");
    } else {
        printf("Private\n");
    }

    for (int i = 0; i < depth; i++) { printf("  "); }
    printf("Returns -> %s\n", token_to_string(node->data.function.return_type));

    if (node->data.function.param_count) {
        for (int i = 0; i < depth; i++) { printf("  "); }
        printf("Params:\n");
        for (size_t i = 0; i < node->data.function.param_count; ++i) {
            print_ast(node->data.function.params[i], depth + 1);
        }
    }

    for (int i = 0; i < depth; i++) { printf("  "); }
    printf("Body:\n");
    print_scope(node->data.function.body, depth + 1);
}

void print_ast(AstNode* node, size_t depth) {
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
            for (int i = 0; i < depth + 1; i++) { printf("  "); }
            printf("Type: %s\n", token_to_string(node->data.assignment.type));
            print_ast(node->data.assignment.value, depth + 1);
            break;
        case Node_Reassignment:
            printf("Reassign: %s\n", node->data.assignment.variable);
            for (int i = 0; i < depth + 1; i++) { printf("  "); }
            printf("Type: %s\n", token_to_string(node->data.assignment.type));
            print_ast(node->data.assignment.value, depth + 1);
            break;
        case Node_If:
            print_if(node, depth + 1);
            break;
        case Node_While:
            print_while(node, depth + 1);
            break;
        case Node_For:
            print_for(node, depth + 1);
            break;
        case Node_Function:
            print_function(node, depth + 1);
            break;
        case Node_Return:
            printf("Return:\n");
            print_ast(node->data.return_statement.val, depth + 1);
            break;
        case Node_Call:
            printf("Function Call:\n");
            for (int i = 0; i < depth + 1; i++) { printf("  "); }
            printf("Function -> %s\n", node->data.function_call.function->data.function.name);
            for (int i = 0; i < depth + 1; i++) { printf("  "); }

        default:
            printf("Unknown node type: %s\n", node_type_to_str(node->type));
    }
}
