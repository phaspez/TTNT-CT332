#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define MAX_LENGTH 500

const char* action[] = {"Init", "UP", "DOWN", "LEFT", "RIGHT"};

typedef struct {
    int eightPuzzle[ROWS][COLS];
    int emptyRow;
    int emptyCol;
} State;

void printState(State state){
    int row,  col;
    printf("\n---------\n");
    for (row = 0; row < ROWS; row++){
        for (col = 0; col < COLS; col++){
            printf("|%d ", state.eightPuzzle[row][col]);
        }
        printf("\n");
    }
    printf("\n---------\n");
}

int compareStates(State a, State b){
    if (a.emptyRow != b.emptyRow || a.emptyCol != b.emptyCol){
        return 0;
    }
    int row, col;
    for (row = 0; row < ROWS; row++){
        for (col = 0; col < COLS; col++){
            if (a.eightPuzzle[row][col] != b.eightPuzzle[row][col]){
                return 0;
            }
        }
    }
    return 1;
}

int goalCheck(State state, State goal){
    return compareStates(state, goal);
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

// generic move function that swap the empty cell with new cell in newEmptyRow and newEmptyCol
// returns if this move is successful
int applyMove(State state, State *next, int newEmptyRow, int newEmptyCol){
    *next = state;
    if (newEmptyRow >= 0 && newEmptyRow < ROWS && newEmptyCol >= 0 && newEmptyCol < COLS) {
        next->emptyRow = newEmptyRow;
        next->emptyCol = newEmptyCol;
        swap(&next->eightPuzzle[next->emptyRow][next->emptyCol], &next->eightPuzzle[state.emptyRow][state.emptyCol]);
        return 1;
    }
    return 0;
}

int upOperator(State state, State *next) {
    return applyMove(state, next, state.emptyRow - 1, state.emptyCol);
}

int downOperator(State state, State *next) {
    return applyMove(state, next, state.emptyRow + 1, state.emptyCol);
}

int leftOperator(State state, State *next) {
    return applyMove(state, next, state.emptyRow, state.emptyCol - 1);
}

int rightOperator(State state, State *next) {
    return applyMove(state, next, state.emptyRow, state.emptyCol + 1);
}

int callOperators(State state, State *next, int op){
    switch (op){
        case 1: return upOperator(state, next);
        case 2: return downOperator(state, next);
        case 3: return leftOperator(state, next);
        case 4: return rightOperator(state, next);
        default: printf("Invalid call operator");
        return 0;
    }
}

// heuristic 1: number of misplaced tiles
int heuristic1(State state, State goal){
    int row, col, count = 0;
    for (row = 0; row < ROWS; row++){
        for (col = 0; col < COLS; col++){
            if (goal.eightPuzzle[row][col] == EMPTY) {
                continue;
            }
            count += state.eightPuzzle[row][col] != goal.eightPuzzle[row][col];
        }
    }
    return count;
}

// heuristic 2: sum of manhattan distance of each tile
int heuristic2(State state, State goal) {
    int row, col, manhattanDistance = 0;
    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            int value = state.eightPuzzle[row][col];
            if (value != EMPTY) {  // Skip the empty tile
                // Find the position of this value in the goal state
                int goalRow, goalCol;
                for (goalRow = 0; goalRow < ROWS; goalRow++) {
                    for (goalCol = 0; goalCol < COLS; goalCol++) {
                        if (goal.eightPuzzle[goalRow][goalCol] == value) {
                            // Calculate the Manhattan distance
                            manhattanDistance += abs(row - goalRow) + abs(col - goalCol);
                        }
                    }
                }
            }
        }
    }
    return manhattanDistance;
}

typedef struct Node{
    State state;
    struct Node* parent;
    int op;
    int heuristic;
} Node;

void sortNodes(vector<Node*>& list){
    sort(list.begin(), list.end(), [](const Node* a, const Node* b){ return a->heuristic < b->heuristic; });
}

Node* findState(State state, vector<Node*> list, int *position){
    for (int i = 0; i < list.size(); i++){
        if (compareStates(state, list[i]->state)){
            *position = i;
            return list[i];
        }
    }
    *position = -1;
    return NULL;
}

Node* bestFirstSearch(State state, State goal){
    vector<Node*> open, closed;
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->op = 0;
    root->heuristic = heuristic1(root->state, goal);
    open.push_back(root);

    while(!open.empty()){
        Node* node = open.front();
        open.erase(open.begin());
        closed.push_back(node);
        if (goalCheck(node->state, goal)){
            return node;
        };
        for (int opt = 1; opt <= MAX_OPERATOR; opt++){
            State newState;
            newState = node->state;
            if (callOperators(node->state, &newState, opt)){
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->op = opt;
                newNode->heuristic = heuristic1(newState, goal);
                // kiem tra trang thai moi sinh ra co thuoc open closed
                int posOpen, posClosed;
                Node* nodeFoundOpen = findState(newState, open, &posOpen);
                Node* nodeFoundClosed = findState(newState, closed, &posClosed);
                if (nodeFoundOpen == NULL && nodeFoundClosed == NULL){
                    open.push_back(newNode);
                } else if (nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic){
                    open.erase(open.begin() + posOpen);
                    open.push_back(newNode);
                } else if (nodeFoundClosed != NULL && nodeFoundClosed->heuristic > newNode->heuristic){
                    closed.erase(closed.begin() + posClosed);
                    open.push_back(newNode);
                }
                sortNodes(open);
            }
        }
    }
    return NULL;
}

Node* bestFirstSearch2(State state, State goal){
    vector<Node*> open, closed;
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->op = 0;
    root->heuristic = heuristic2(root->state, goal);
    open.push_back(root);

    while(!open.empty()){
        Node* node = open.front();
        open.erase(open.begin());
        closed.push_back(node);
        if (goalCheck(node->state, goal)){
            return node;
        };
        for (int opt = 1; opt <= MAX_OPERATOR; opt++){
            State newState;
            newState = node->state;
            if (callOperators(node->state, &newState, opt)){
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->op = opt;
                newNode->heuristic = heuristic1(newState, goal);
                // kiem tra trang thai moi sinh ra co thuoc open closed
                int posOpen, posClosed;
                Node* nodeFoundOpen = findState(newState, open, &posOpen);
                Node* nodeFoundClosed = findState(newState, closed, &posClosed);
                if (nodeFoundOpen == NULL && nodeFoundClosed == NULL){
                    open.push_back(newNode);
                } else if (nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic){
                    open.erase(open.begin() + posOpen);
                    open.push_back(newNode);
                } else if (nodeFoundClosed != NULL && nodeFoundClosed->heuristic > newNode->heuristic){
                    closed.erase(closed.begin() + posClosed);
                    open.push_back(newNode);
                }
                sortNodes(open);
            }
        }
    }
    return NULL;
}

void printWayToGetGoal(Node* node) {
    vector<Node*> list;
    while (node->parent != NULL) {
        list.push_back(node);
        node = node->parent;
    }
    list.push_back(node);
    int count = 0;
    for (int i = list.size() - 1; i >= 0; i--) {
        printState(list[i]->state);
        printf("Action %d: %s\n", count, action[list[i]->op]);
        count++;
    }
}


int main(){
    State state, result;
    state.emptyRow = 1;
    state.emptyCol = 1;
    state.eightPuzzle[0][0] = 3;
    state.eightPuzzle[0][1] = 4;
    state.eightPuzzle[0][2] = 5;
    state.eightPuzzle[1][0] = 1;
    state.eightPuzzle[1][1] = 0;
    state.eightPuzzle[1][2] = 2;
    state.eightPuzzle[2][0] = 6;
    state.eightPuzzle[2][1] = 7;
    state.eightPuzzle[2][2] = 8;
    // BAI TAP 1
    printf("Initial state\n");
    printState(state);
    int op;
    for (op=1; op<=MAX_OPERATOR; op++){
        if (callOperators(state, &result, op)){
            printf("Action %s\n", action[op]);
            printState(result);
        } else {
            printf("Action %s failed\n", action[op]);
        }
    }

    // BAI TAP 2 & 3
    result.emptyRow = 0;
    result.emptyCol = 0;
    result.eightPuzzle[0][0] = 0;
    result.eightPuzzle[0][1] = 1;
    result.eightPuzzle[0][2] = 2;
    result.eightPuzzle[1][0] = 3;
    result.eightPuzzle[1][1] = 4;
    result.eightPuzzle[1][2] = 5;
    result.eightPuzzle[2][0] = 6;
    result.eightPuzzle[2][1] = 7;
    result.eightPuzzle[2][2] = 8;
    printf("Initial state\n");
    printState(state);
    printf("Goal state\n");
    printState(result);

    printf("\nBest first search heuristic 1\n");
    Node* end = bestFirstSearch(state, result);
    printWayToGetGoal(end);

    printf("\nBest first search heuristic 2\n");
    end = bestFirstSearch2(state, result);
    printWayToGetGoal(end);

    return 0;
}
