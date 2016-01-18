#ifndef TESTER_H
#define TESTER_H

#include "testsuite.h"


/** Prototypes */

void executeTestcases(Implementation *_parImpl, int _threads);
void executeSlave(void (*slaveFunc)(int, int), int _threads);

#endif