#pragma once
#include <time.h> 
#include <algorithm>

class Utils
{
public:
	Utils()
	{
		srand(time(NULL));
	}

	int random(int min, int max)
	{
		return rand() % (max - min + 1) + min;
	}
};