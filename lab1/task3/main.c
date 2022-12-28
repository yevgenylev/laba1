#include <stdbool.h>
#include "sys/wait.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"

int pointer = 0;

void executeFork(char* processTree, size_t treeSize) {
    int pid = -1;
    bool isPrevDot = false;
    int depth = 0;

    while (treeSize > pointer) {
        char currentNode = processTree[pointer++];

        if (pid != 0 && depth > 0) {
            if (currentNode == '(') {
                depth++;
            }
            if (currentNode == ')') {
                depth--;
            }
        }
        else if (currentNode == '.') {
            if (isPrevDot && pid == 0) {
                return;
            }
            pid = fork();
            isPrevDot = true;
        }
        else if (currentNode == '(' && isPrevDot) {
            if (pid == 0) {
                executeFork(processTree, treeSize);
                return;
            }
            else {
                depth = 1;
                isPrevDot = false;
            }
        }
        else if (currentNode == ')') {
            return;
        }
    }
}

int main(int argc, char* argv[]) {
    char* processTree = argv[1];
    size_t treeSize = strlen(processTree);
    executeFork(processTree, treeSize);
    sleep(40);
    return 0;
}