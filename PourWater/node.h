//
// Created by DELL on 19/08/2024.
//
#ifndef TTNT_NODE_H
#define TTNT_NODE_H

#include "bucketState.h"
#include <stack>
#include <queue>

using namespace std;

typedef struct Node {
    State state;
    Node* parent;
    int noFunction;
} Node;
#define MAXLENGTH 1000

typedef struct {
    Node* Elements[MAXLENGTH];
    int topIndex;
} Stack;

typedef struct {
    Node*elements[MAXLENGTH];
    int front, rear;
} Queue;

// STACK implementation
void push(Node *x, Stack *stack);
void makeNullStack(Stack *stack);
int emptyStack(Stack stack);
int fullStack(Stack stack);
Node* top(Stack stack);
void pop(Stack *stack);

// QUEUE implementation
void makeNullQueue(Queue *queue);
int emptyQueue(Queue queue);
int fullQueue(Queue queue);
Node* getFront(Queue queue);
void pop(Queue *queue);
void push(Node *node, Queue *queue);


void freeStack(Stack *stack);
void freeNode(Node * node);
void freeQueue(Queue *queue);

int findState(State state, Stack stack);
int findState(State state, Queue queue);
int findState(State state, stack<Node*> stack);
int findState(State state, queue<Node*> queue);

int compareStates(State state1, State state2);

Node* DFSAlgorithm(State state);
Node* BFSAlgorithm(State state);
Node* DFSAlgorithmStd(State state);
Node* BFSAlgorithmStd(State state);


void printResult(Node* result);
void printResultStd(Node* result);

void freeStack(Stack *stack);
void freeNodeRecursive(Node *node);
void freeNode(Node *node);

#endif //TTNT_NODE_H
