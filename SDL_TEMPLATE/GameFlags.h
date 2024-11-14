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
};