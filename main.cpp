#include "stdio.h"
#include "PourWater/node.h"
#include "PourWater/bucketState.h"


int main() {
    State currState = {0, 0}, result;
    printf("Trang thai bat dau\n");
    printState(currState);

    // ex1
    printf("\nACTIONS ===============\n");
    for (int op=1; op<=6; op++){
        if (callOperator(currState, &result, op)){
            printf("Action %s\n", printActionName(op));
            printState(result);
        } else {
            printf("Action %s failed\n", printActionName(op));
        }
    }

    // ex 2
    printf("\nDFS ===============\n");
    Node* end = DFSAlgorithm(currState);
    printResult(end);
    freeNodeRecursive(end);
    // ex 3
    printf("\nBFS ===============\n");
    end = BFSAlgorithm(currState);
    printResult(end);
    freeNodeRecursive(end);
    // ex 4
    printf("\nDFS std ===========\n");
    end = DFSAlgorithmStd(currState);
    printResultStd(end);
    freeNodeRecursive(end);
    // ex 5
    printf("\nBFS std ===========\n");
    end = BFSAlgorithmStd(currState);
    printResultStd(end);
    freeNodeRecursive(end);

    return 0;
}
