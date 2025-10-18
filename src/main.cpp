#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "headers/Player.h"
#include "headers/GameConfig.h"

#include <iostream>
#include <thread>

using namespace GameConfig;

/* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

bool paused;
Player* player; // Create a Player object

constexpr int pausePartsCount = 2; // Just for fun

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	SDL_SetAppMetadata("HW2", "0.1", "game-engines.homework-2");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("Homework 2", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	player = new Player(WINDOW_WIDTH/2 - PLAYER_WIDTH, WINDOW_HEIGHT/2 - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT);

	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
	}
	if (event->type == SDL_EVENT_KEY_DOWN) {
		auto key = event->key.key;
		if (key == SDLK_ESCAPE) {
			return SDL_APP_SUCCESS;
		}
		if (key == SDLK_P) {
			paused = !paused;
		}
	}
	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
	if (!paused) {
		player->update();
	}

	SDL_FRect playerRect = player->getRect();
	/* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
	SDL_SetRenderDrawColor(renderer, 48, 10, 36, SDL_ALPHA_OPAQUE);  /* new color, full alpha. */

	/* clear the window to the draw color. */
	SDL_RenderClear(renderer);

	/* put the newly-cleared rendering on the screen. */
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderRect(renderer, &playerRect);
	SDL_RenderFillRect(renderer, &playerRect);

	if (paused) {
		SDL_FRect pauseParts[pausePartsCount];
		for (int i = 0; i < pausePartsCount; i++) {
			pauseParts[i].w = PAUSE_WIDTH;
			pauseParts[i].h = PAUSE_HEIGHT;
			pauseParts[i].x = WINDOW_WIDTH - ((pauseParts[i].w * (1.5 * i)) + (WINDOW_WIDTH * 0.05));
			pauseParts[i].y = WINDOW_HEIGHT * 0.05;
		}
		SDL_RenderRects(renderer, pauseParts, pausePartsCount);
		SDL_RenderFillRects(renderer, pauseParts, pausePartsCount);
	}
	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	/* SDL will clean up the window/renderer for us. */
}