#ifndef MENU_H
#define MENU_H

#include "fSharp32.h"
#include "Inputs.h"
#include "Instruments.h"
#include "SSD1306.h"


extern void (*screenControlFunction)();

void mainScreenInit();
void mainScreenController();

#endif