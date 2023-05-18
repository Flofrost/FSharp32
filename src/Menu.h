#ifndef MENU_H
#define MENU_H

#include "fSharp32.h"
#include "Inputs.h"
#include "Instruments.h"
#include "SSD1306.h"


extern void (*screenControlFunction)();

void mainScreenInit();
void mainScreenController();

void mainMenuInit();
void mainMenuController();

void instrumentMenuInit();
void instrumentMenuController();

#endif