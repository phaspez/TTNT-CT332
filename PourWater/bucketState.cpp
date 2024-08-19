//
// Created by DELL on 19/08/2024.
//
#include "stdio.h"
#include "bucketState.h"

#define TANKCAPACITY_X 9
#define TANKCAPACITY_Y 4
#define EMPTY 0
#define GOAL 6
#define MAXLENGTH 100

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

void makeNullState(State *state){
    state->x = 0;
    state->y = 0;
}

void printState(State state){
    printf(" X:%d --- Y:%d\n", state.x, state.y);
}

int goalCheck(State state){
    return (state.x == GOAL || state.y == GOAL);
}

int pourWaterFullX(State curState, State *result){
    if(curState.x < TANKCAPACITY_X){
        result->x = TANKCAPACITY_X;
        result->y = curState.y;
        return 1;
    }
    return 0;
}

int pourWaterFullY(State curState, State *result){
    if(curState.y < TANKCAPACITY_Y){
        result->x = curState.x;
        result->y = TANKCAPACITY_Y;
        return 1;
    }
    return 0;
}

int pourWaterEmptyX(State curState, State *result){
    if(curState.x > EMPTY){
        result->x = EMPTY;
        result->y = curState.y;
        return 1;
    }
    return 0;
}

int pourWaterEmptyY(State curState, State *result){
    if(curState.y > EMPTY){
        result->x = curState.x;
        result->y = EMPTY;
        return 1;
    }
    return 0;
}

int pourWaterXtoY(State curState, State *result){
    if(curState.x > EMPTY && curState.y < TANKCAPACITY_Y){
        result->x = max(EMPTY, curState.x - (TANKCAPACITY_Y - curState.y));
        result->y = min(TANKCAPACITY_Y, curState.x + curState.y);
        return 1;
    }
    return 0;
}

int pourWaterYtoX(State curState, State *result){
    if(curState.y > EMPTY && curState.x < TANKCAPACITY_X){
        result->y = max(EMPTY, curState.y - (TANKCAPACITY_X - curState.x));
        result->x = min(TANKCAPACITY_X, curState.x + curState.y);
        return 1;
    }
    return 0;
}

int callOperator(State curState, State *result, int option){
    switch(option){
        case 1: return pourWaterFullX(curState, result);
        case 2: return pourWaterFullY(curState, result);
        case 3: return pourWaterEmptyX(curState, result);
        case 4: return pourWaterEmptyY(curState, result);
        case 5: return pourWaterXtoY(curState, result);
        case 6: return pourWaterYtoX(curState, result);
        default: {
            printf("\nError pourWater option!");
            return 0;
        };
    }
}

const char* printActionName(int option){
    switch(option){
        case 1: return "Do day nuoc binh X";
        case 2: return "Do day nuoc binh Y";
        case 3: return "Lam rong nuoc binh X";
        case 4: return "Lam rong nuoc binh Y";
        case 5: return "Chuyen nuoc tu binh X sang binh Y";
        case 6: return "Chuyen nuoc tu binh Y sang binh X";
        default: return "Error action name!";
    }
}