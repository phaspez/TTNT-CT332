//
// Created by DELL on 19/08/2024.
//
#include <cstdlib>
#include "stdio.h"
#include "node.h"

using namespace std;

//region STACK IMPLEMENTATION
// push Node *x to Stack *stack
void push(Node *x, Stack *stack) {
    if (fullStack(*stack)){
        printf("Error! Stack is full!");
    } else {
        stack->topIndex--;
        stack->Elements[stack->topIndex] = x;
    }
}

void makeNullStack(Stack *stack) {
    stack->topIndex = MAXLENGTH;
}

// checks if stack is empty
int emptyStack(Stack stack) {
    return stack.topIndex == MAXLENGTH;
}

// checks if stack is full
int fullStack(Stack stack) {
    return stack.topIndex == 0;
}

Node* top(Stack stack) {
    if (!emptyStack(stack)){
        return stack.Elements[stack.topIndex];
    } else {
        printf("Error! Stack is empty!");
        return NULL;
    }
}

void pop(Stack *stack) {
    if (!emptyStack(*stack)){
        stack->topIndex++;
    } else {
        printf("Error! Stack is empty!");
    }
}
//endregion

//region QUEUE IMPLEMENTATION
void makeNullQueue(Queue *queue) {
    queue->front = -1;
    queue->rear = -1;
}

int emptyQueue(Queue queue) {
    return queue.front == -1;
}

int fullQueue(Queue queue) {
    return (queue.rear - queue.front + 1) % MAXLENGTH == 0;
}

Node* getFront(Queue queue) {
    if (emptyQueue(queue)){
        printf("Error! Queue is empty!");
        return NULL;
    } else {
        return queue.elements[queue.front];
    }
}

void pop(Queue *queue) {
    if (!emptyQueue(*queue)){
        if (queue->front == queue->rear){
            makeNullQueue(queue);
        } else {
            queue->front = (queue->front + 1) % MAXLENGTH;
        }
    } else {
        printf("Error! Queue is empty!");
    }
}

void push(Node *node, Queue *queue) {
    if (!fullQueue(*queue)){
        if (emptyQueue(*queue)){
            queue->front = 0;
        }
        queue->rear = (queue->rear + 1) % MAXLENGTH;
        queue->elements[queue->rear] = node;
    } else {
        printf("Error! Queue is full!");
    }
}
//endregion

// finds a state in a Stack, return 1 if found, 0 otherwise
// there are 4 implementations, stack and queue, std and custom.
//region FIND STATE
int findState(State state, Stack stack){
    while (!emptyStack(stack)){
        Node* node = top(stack);
        pop(&stack);
        if (compareStates(node->state, state)){
            return 1;
        }
    }
    return 0;
}

int findState(State state, Queue queue){
    while (!emptyQueue(queue)){
        Node* node = getFront(queue);
        pop(&queue);
        if (compareStates(node->state, state)){
            return 1;
        }
    }
    return 0;
}

int findState(State state, stack<Node*> stack){
    while (!stack.empty()){
        Node* node = stack.top();
        stack.pop();
        if (compareStates(node->state, state)){
            return 1;
        }
    }
    return 0;
}

int findState(State state, queue<Node*> queue){
    while (!queue.empty()){
        Node* node = queue.front();
        queue.pop();
        if (compareStates(node->state, state)){
            return 1;
        }
    }
    return 0;
}
//endregion

// check if two states are equal
int compareStates(State state1, State state2){
    return state1.x == state2.x && state1.y == state2.y;
}

// create a new node and assigs values
Node* makeNewNode(State state, Node* parent, int option){
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->state = state;
    newNode->parent = parent;
    newNode->noFunction = option;
    return newNode;
}

//region SEARCH ALGORITHMS

Node* DFSAlgorithm(State state) {
    Stack open, closed;
    makeNullStack(&open);
    makeNullStack(&closed);

    Node* root = makeNewNode(state, NULL, 0);
    push(root, &open);
    while (!emptyStack(open)){
        Node* node = top(open);
        pop(&open);
        push(node, &closed);
        if (goalCheck(node->state)){
            return node;
        }

        int option;
        for (option=1; option<= 6; option++){
            State newState;
            makeNullState(&newState);
            if (callOperator(node->state, &newState, option)){
                if (findState(newState, closed) || findState(newState, open)){
                    continue;
                }
                //printf("alloc\n");
                Node* newNode = makeNewNode(newState, node, option);
                push(newNode, &open);
            }
        }
    }

    freeStack(&open);
    freeStack(&closed);
    return NULL;
}

// if preservative, will not consider fill bucked and empty bucket action
Node* BFSAlgorithm(State state, int preservative = 0) {
    Queue open, closed;
    makeNullQueue(&open);
    makeNullQueue(&closed);

    //printf("start of dfs\n");

    Node* root = makeNewNode(state, NULL, 0);
    push(root, &open);
    while (!emptyQueue(open)){
        Node* node = getFront(open);
        pop(&open);
        push(node, &closed);
        if (goalCheck(node->state)){
            return node;
        }

        int option;
        for (option=1; option<= 6; option++){
            State newState;
            makeNullState(&newState);
            if (callOperator(node->state, &newState, option)){
                if (findState(newState, closed) || findState(newState, open)){
                    continue;
                }
                Node* newNode = makeNewNode(newState, node, option);
                push(newNode, &open);
            }
        }
    }

    freeQueue(&open);
    freeQueue(&closed);
    return NULL;
}

// DFS algorithm with standard stack library
Node* DFSAlgorithmStd(State state) {
    stack<Node*> open, closed;

    Node* root = makeNewNode(state, NULL, 0);
    open.push(root);
    while (!open.empty()){
        Node* node = open.top();
        open.pop();
        closed.push(node);
        if (goalCheck(node->state)){
            return node;
        }

        int option;
        for (option=1; option<= 6; option++){
            State newState;
            makeNullState(&newState);
            if (callOperator(node->state, &newState, option)){
                if (findState(newState, closed) || findState(newState, open)){
                    continue;
                }
                Node* newNode = makeNewNode(newState, node, option);
                open.push(newNode);
            }
        }
    }

    return NULL;
}
// BFS algorithm with standard queue library
Node* BFSAlgorithmStd(State state) {
    queue<Node *> open, closed;

    Node* root = makeNewNode(state, NULL, 0);
    open.push(root);
    while (!open.empty()){
        Node* node = open.front();
        open.pop();
        closed.push(node);
        if (goalCheck(node->state)){
            return node;
        }

        int option;
        for (option=1; option<= 6; option++){
            State newState;
            makeNullState(&newState);
            if (callOperator(node->state, &newState, option)){
                if (findState(newState, closed) || findState(newState, open)){
                    continue;
                }
                Node* newNode = makeNewNode(newState, node, option);
                open.push(newNode);
            }
        }
    }

    return NULL;
}

//endregion

// print the path to the goal/result
void printResult(Node *result) {
    printf("cur: %d\n", result->noFunction);
    Stack stackPrint;
    makeNullStack(&stackPrint);

    while(result->parent != NULL){
        push(result, &stackPrint);
        result = result->parent;
    }
    push(result, &stackPrint);

    int action = 0;
    while(!emptyStack(stackPrint)) {
        Node *node = top(stackPrint);
        pop(&stackPrint);
        if (action == 0) {
            action++;
            printf("Initial state\n");
        } else {
            printf("Action %d %s\n", action++, printActionName(node->noFunction));
        }
        printState(node->state);
    }
}

// like printResult but using stack std instead
void printResultStd(Node *result) {
    //printf("cur: %d\n", result->noFunction);
    stack<Node*> stackPrint;

    while(result->parent != NULL){
        stackPrint.push(result);
        result = result->parent;
    }
    stackPrint.push(result);

    int action = 0;
    while(!stackPrint.empty()) {
        Node *node = stackPrint.top();
        stackPrint.pop();
        if (action == 0) {
            action++;
            printf("Initial state\n");
        } else {
            printf("Action %d %s\n", action++, printActionName(node->noFunction));
        }
        printState(node->state);
    }
}

void freeNodeRecursive(Node *node) {
    if (node->parent != NULL){
        freeNodeRecursive(node->parent);
    }
    freeNode(node);
}

//region FREE MALLOC FUNCTIONS

// free all nodes in a stack
void freeStack(Stack *stack) {
    while (!emptyStack(*stack)){
        freeNode(top(*stack));
        pop(stack);
    }
}

void freeQueue(Queue *queue) {
    while (!emptyQueue(*queue)){
        freeNode(getFront(*queue));
        pop(queue);
    }
}

void freeNode(Node *node) {
    free(node);
}

//endregion