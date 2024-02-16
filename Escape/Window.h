#pragma once
#include<SDL.h>
#include "Vec2.h"

struct ScreenDimension
{
	int width;
	int height;
};

// TODO: get config from file
// 480	 x 270	 (1 / 4 HD) - 0
// 960	 x 540	 (1 / 2 HD) - 2
// 1,920 x 1,080 (HD) - 4
const short int resolutionScaler = 4;
const int SCREEN_WIDTH = 1920; // 480 * resolutionScaler;
const int SCREEN_HEIGHT = 1080; // 270 * resolutionScaler;
const int BLOCK_SIZE = 64; // 16 * resolutionScaler;

ScreenDimension getDisplayDimension();
