#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <time.h> 
#include <algorithm>

#include "Physics.h"
#include "Scene.h"
#include "SceneMain.h"
#include "SceneMenu.h"

struct Mouse {
	float x;
	float y;
};

class Game
{
private:
	SDL_Window*										m_window		=	 nullptr;
	SDL_Renderer*									m_renderer		=	 nullptr;
	SDL_Event										m_event;
	bool											m_isRunning		=	 false;
	Mouse											m_mouse			=	 { 0, 0 };
	int												m_currentFrame	=	 0;
	std::string										m_currentScene	=	 "Menu";
	std::map<std::string, std::shared_ptr<Scene>>	m_scenes;
	Assets m_assets;

public:
	void init(int width, int height);
	void run();
	void quit();

	void process();
	void update(float deltaTime);
	void render();

	SDL_Renderer* currentRenderer();
	std::shared_ptr<Scene> currentScene();
	void changeScene(const std::string& name, std::shared_ptr<Scene> scene);
};