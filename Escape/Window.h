#pragma once
#include<SDL.h>
#include "Vec2.h"

struct ScreenDimension
{
	int width;
	int height;
};

class Window
{
public:
	ScreenDimension getDisplayDimension()
	{
		SDL_DisplayMode dimension;

		if (SDL_GetDesktopDisplayMode(0, &dimension) != 0)
		{
			SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		}

		ScreenDimension dimensions = { dimension.w, dimension.h };
		return dimensions;
	}

	// TODO: can those be globals?
	// TODO: handle multiple dimensions by multiplying accordingly
	// 480	 x 270	 (1 / 4 HD)
	// 960	 x 540	 (1 / 2 HD)
	// 1,920 x 1,080 (HD)
	const int SCREEN_WIDTH = 960;
	const int SCREEN_HEIGHT = 540;
	const int BLOCK_SIZE = 32;
};
