#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "headers/Player.h"
#include "headers/GameConfig.h"

#include <iostream>
#include <thread>

using namespace GameConfig;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* boxTexture = NULL;

bool paused = false;
std::vector<Player*> players;
std::vector<SDL_FRect> objects;

constexpr int pausePartsCount = 2;

static SDL_FRect CreateRect(float x, float y, float w, float h)
{
	SDL_FRect rect{};
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	SDL_Surface* surface = NULL;
	char* png_path = NULL;

	SDL_SetAppMetadata("HW3", "43.08", "game-engines.homework-3");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("Homework 3", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	SDL_asprintf(&png_path, std::string(ASSETS_PATH + "crate.png").c_str(), SDL_GetBasePath());
	boxTexture = IMG_LoadTexture(renderer, png_path);
	if (!boxTexture) {
		SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	players.emplace_back(new Player(WINDOW_WIDTH / 2 - PLAYER_WIDTH, WINDOW_HEIGHT / 2 - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT));
	objects.emplace_back(CreateRect(15, 15, 75, 75));

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
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
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	const uint64_t frameStartNS = SDL_GetTicksNS();

	SDL_SetRenderDrawColor(renderer, 48, 10, 36, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	if (!paused) {
		for (Player* player : players)
		{
			player->update();
		}
	}

	SDL_FRect playerRects[players.size()];
	for (size_t i = 0; i < players.size(); i++) {
		playerRects[i] = players[i]->getRect();
	}

	SDL_SetRenderDrawColor(renderer, 225, 225, 225, SDL_ALPHA_OPAQUE);
	SDL_RenderRects(renderer, playerRects, players.size());
	SDL_RenderFillRects(renderer, playerRects, players.size());

	SDL_FRect objectRects[objects.size()];
	std::copy(objects.begin(), objects.end(), objectRects);

	SDL_SetRenderDrawColor(renderer, 219, 171, 83, SDL_ALPHA_OPAQUE);
	SDL_RenderRects(renderer, objectRects, objects.size());
	SDL_RenderFillRects(renderer, objectRects, objects.size());
	
	for (SDL_FRect object : objects)
	{
		SDL_RenderTexture(renderer, boxTexture, NULL, &object);
	}

	if (paused) {
		SDL_FRect pauseParts[pausePartsCount]{};
		for (int i = 0; i < pausePartsCount; i++) {
			pauseParts[i].w = PAUSE_WIDTH;
			pauseParts[i].h = PAUSE_HEIGHT;
			pauseParts[i].x = WINDOW_WIDTH - ((pauseParts[i].w * (1.5 * i)) + (WINDOW_WIDTH * 0.05));
			pauseParts[i].y = WINDOW_HEIGHT * 0.05;
		}
		SDL_SetRenderDrawColor(renderer, 225, 225, 225, SDL_ALPHA_OPAQUE);
		SDL_RenderRects(renderer, pauseParts, pausePartsCount);
		SDL_RenderFillRects(renderer, pauseParts, pausePartsCount);
	}
	SDL_RenderPresent(renderer);

	const uint64_t frameEndNS = SDL_GetTicksNS();
	const uint64_t framerateNS = frameEndNS - frameStartNS;
	if (framerateNS < TARGET_FRAMERATE_NS) {
		const uint64_t remainingNS = TARGET_FRAMERATE_NS - framerateNS;
		SDL_DelayNS(remainingNS);
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	for (size_t i = 0; i < players.size(); i++)
	{
		delete players[i];
	}
}