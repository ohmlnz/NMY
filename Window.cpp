#include "Window.h"

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
