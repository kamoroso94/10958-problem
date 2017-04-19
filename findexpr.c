#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define OPCODES 6
#define DIGITS 9

typedef struct ExprNode {
	char value;
	struct ExprNode* parent;
	struct ExprNode* left;
	struct ExprNode* right;
} ExprNode;

int findExpr(int target, char* result);
int _findExpr(int target, ExprNode* root, int leafCount);
ExprNode* ExprNode_create();
void ExprNode_destroy(ExprNode* root);
void ExprNode_clear(ExprNode* node);
int ExprNode_leaves(ExprNode* root, ExprNode** list, int index);
int ExprNode_isLeaf(ExprNode* node);
double ExprNode_eval(ExprNode* root);
int ExprNode_toString(ExprNode* root, char* expr, int index);
void ExprNode_set(ExprNode* node, char value);
int concat(int a, int b);
int isdigit(char c);

int main(int argc, char* argv[]) {
	int target, success;
	char expr[128];
	
	if(argc != 2) {
		printf("Missing argument\n");
		return 0;
	}
	
	target = atoi(argv[1]);
	if(target == 0 && argv[1][0] != '0') {
		printf("Invalid argument\n");
	}
	
	success = findExpr(target, expr);
	
	printf("Success? %c\n", success ? 'y' : 'n');
	printf("%i = %s\n", target, expr);
	
	return 0;
}

int findExpr(int target, char* result) {
	int success, len;
	ExprNode* root = ExprNode_create();
	
	success = _findExpr(target, root, 1);
	
	if(success) {
		len = ExprNode_toString(root, result, 0);
		result[len] = '\0';
	} else {
		strcpy(result, "No solution");
	}
	
	ExprNode_destroy(root);
	
	return success;
}

int _findExpr(int target, ExprNode* root, int leafCount) {
	int i, j, len;
	char opcodes[OPCODES] = {'|', '+', '-', '*', '/', '^'};
	ExprNode* leaves[DIGITS];
	ExprNode* leaf;
	
	if(leafCount == DIGITS) {
		len = ExprNode_leaves(root, leaves, 0);
		
		for(i = 0; i < len; i++) {
			leaves[i]->value = '1' + i;
		}
		
		return ExprNode_eval(root) == target;
	}
	
	for(i = 0; i < OPCODES; i++) {
		char op = opcodes[i];
		len = ExprNode_leaves(root, leaves, 0);
		
		for(j = 0; j < len; j++) {
			leaf = leaves[j];
			
			if(leaf->parent != NULL && leaf->parent->value == '|' && op != '|') {
				continue;
			}
			ExprNode_set(leaf, op);
			if(_findExpr(target, root, leafCount + 1)) {
				return 1;
			}
			// try different leaf
			ExprNode_clear(leaf);
		}
		// try different op
	}
	
	return 0;
}

ExprNode* ExprNode_create() {
	ExprNode* root = (ExprNode*) malloc(sizeof(ExprNode));
	ExprNode_clear(root);
	root->parent = NULL;
	
	return root;
}

void ExprNode_destroy(ExprNode* root) {
	if(root == NULL) {
		return;
	}
	
	ExprNode_destroy(root->left);
	ExprNode_destroy(root->right);
	free(root);
}

void ExprNode_clear(ExprNode* node) {
	ExprNode_destroy(node->left);
	ExprNode_destroy(node->right);
	node->left = NULL;
	node->right = NULL;
	node->value = '\0';
}

int ExprNode_leaves(ExprNode* root, ExprNode** list, int index) {
	if(ExprNode_isLeaf(root)) {
		list[index++] = root;
	} else {
		index = ExprNode_leaves(root->left, list, index);
		index = ExprNode_leaves(root->right, list, index);
	}
	return index;
}

int ExprNode_isLeaf(ExprNode* node) {
	return node->left == NULL && node->right == NULL;
}

double ExprNode_eval(ExprNode* root) {
	switch(root->value) {
		case '|':
			return concat(ExprNode_eval(root->left), ExprNode_eval(root->right));
		
		case '+':
			return ExprNode_eval(root->left) + ExprNode_eval(root->right);
		
		case '-':
			return ExprNode_eval(root->left) - ExprNode_eval(root->right);
		
		case '*':
			return ExprNode_eval(root->left) * ExprNode_eval(root->right);
		
		case '/':
			return ExprNode_eval(root->left) / ExprNode_eval(root->right);
		
		case '^':
			return pow(ExprNode_eval(root->left), ExprNode_eval(root->right));
		
		default:
			return root->value - '0';
	}
}

int ExprNode_toString(ExprNode* root, char* expr, int index) {
	if(root == NULL) {
		return index;
	}
	if(isdigit(root->value)) {
		expr[index++] = root->value;
	} else if(root->value == '|') {
		index = ExprNode_toString(root->left, expr, index);
		index = ExprNode_toString(root->right, expr, index);
	} else {
		expr[index++] = '(';
		index = ExprNode_toString(root->left, expr, index);
		expr[index++] = ')';
		expr[index++] = root->value;
		expr[index++] = '(';
		index = ExprNode_toString(root->right, expr, index);
		expr[index++] = ')';
	}
	
	return index;
}

void ExprNode_set(ExprNode* node, char value) {
	ExprNode_clear(node);
	node->value = value;
	node->left = ExprNode_create();
	node->right = ExprNode_create();
	node->left->parent = node;
	node->right->parent = node;
}

int concat(int a, int b) {
	int exp = 1;
	int n = b;
	
	if(n == 0) {
		exp = 10;
	} else {
		while(n != 0) {
			exp *= 10;
			n /= 10;
		}
	}
	
	return a * exp + b;
}

int isdigit(char c) {
	return c >= '0' && c <= '9';
}
