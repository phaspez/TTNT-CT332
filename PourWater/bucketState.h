//
// Created by DELL on 19/08/2024.
//

#ifndef TTNT_BUCKETSTATE_H
#define TTNT_BUCKETSTATE_H

typedef struct {
    int x;
    int y;
} State;

void makeNullState(State *state);
void printState(State state);
int goalCheck(State state);
int callOperator(State curState, State *result, int option);
const char* printActionName(int option);

int pourWaterFullX(State curState, State *result);
int pourWaterFullY(State curState, State *result);
int pourWaterEmptyX(State curState, State *result);
int pourWaterEmptyY(State curState, State *result);
int pourWaterXtoY(State curState, State *result);
int pourWaterYtoX(State curState, State *result);

#endif //TTNT_BUCKETSTATE_H
