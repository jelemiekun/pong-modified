#pragma once

struct GameFlags {
	unsigned int inStart : 1;
	unsigned int inPlaying : 1;
	unsigned int inWin : 1;
	unsigned int inLose : 1;

	unsigned int isTwoPlayer : 1;
	unsigned int isClassic : 1;
	unsigned int isDoubleEnemy : 1;
	unsigned int isDoublePaddle : 1;

	unsigned int clicked : 1;
	unsigned int dirR : 1;
	unsigned int dirUR : 1;
	unsigned int dirU : 1;
	unsigned int dirUL : 1;
	unsigned int dirL : 1;
	unsigned int dirDL : 1;
	unsigned int dirD : 1;
	unsigned int dirDR : 1;
};