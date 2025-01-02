#ifndef CONTROLS_H
#define CONTROLS_H

#include "GameClasses.h"

class Controls {
public:
    static void SpecialFunc(int key, int x, int y);
    static void KeyboardUpFunc(unsigned char key, int x, int y);
    static void SpecialUpFunc(int key, int x, int y);
};

#endif