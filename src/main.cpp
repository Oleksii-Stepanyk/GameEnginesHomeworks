#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "headers/Player.h"
#include "headers/Object.h"
#include "headers/Textures.h"
#include "headers/GameConfig.h"
#include "headers/Collision.h"

#include <iostream>
#include <thread>
#include <unordered_map>

using namespace GameConfig;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

std::vector<Player*> players;
std::vector<Object*> objects;

constexpr int pausePartsCount = 2;
const std::vector<std::pair<std::string, std::string>> texturePaths = {
	std::pair("crate.png", TextureID::Crate), std::pair("Barrel3.png", TextureID::Barrel)
};

std::unordered_map<std::string, SDL_Texture*> loadedTextures;

static bool loadTexture(const std::string& path, const std::string& identifier) {
	char* png_path = NULL;

	SDL_asprintf(&png_path, std::string(ASSETS_PATH + path).c_str(), SDL_GetBasePath());
	SDL_Texture* texture = IMG_LoadTexture(renderer, png_path);

	if (!texture) {
		SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
		return true;
	}
	loadedTextures[identifier] = texture;
	return false;
}

static SDL_Texture* getTexture(const std::string& identifier) {
	auto it = loadedTextures.find(identifier);
	if (it != loadedTextures.end()) {
		return it->second;
	}
	return nullptr;
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

	for (const auto& texturePath : texturePaths) {
		if (loadTexture(texturePath.first, texturePath.second)) {
			return SDL_APP_FAILURE;
		}
	}

	players.emplace_back(new Player(WINDOW_WIDTH / 2 - PLAYER_WIDTH, WINDOW_HEIGHT / 2 - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT));

	objects.emplace_back(new Object(15, 15, 100, 100, TextureID::Crate));
	objects.emplace_back(new Object(200, 150, 100, 100, TextureID::Barrel));

	Object* wall1 = new Object(60, 200, 80, 300, TextureID::Crate, ObjectType::Static);
	objects.emplace_back(wall1);

	Object* trigger1 = new Object(900, 300, 120, 120, TextureID::Barrel, ObjectType::Trigger);
	trigger1->setTriggerCallback([](Object* other) {
		const char* shapeStr = (other->getShape() == CollisionShape::Sphere) ? "sphere" : "box";
		SDL_Log("BOX TRIGGER activated by %s at (%.2f, %.2f)!", shapeStr, other->getX(), other->getY());
		});
	trigger1->setContinuous(false);
	objects.emplace_back(trigger1);
	
	
	Object* sphere1 = new Object(250, 400, 60, 60, TextureID::Barrel);
	sphere1->setShape(CollisionShape::Sphere);
	sphere1->setRadius(30.0);
	objects.emplace_back(sphere1);
	
	Object* sphere2 = new Object(350, 400, 50, 50, TextureID::Barrel);
	sphere2->setShape(CollisionShape::Sphere);
	sphere2->setRadius(25.0);
	objects.emplace_back(sphere2);
	
	Object* sphereWall = new Object(700, 100, 80, 80, TextureID::Barrel, ObjectType::Static);
	sphereWall->setShape(CollisionShape::Sphere);
	sphereWall->setRadius(40.0);
	objects.emplace_back(sphereWall);
	
	Object* sphereTrigger = new Object(1000, 450, 100, 100, TextureID::Barrel, ObjectType::Trigger);
	sphereTrigger->setShape(CollisionShape::Sphere);
	sphereTrigger->setRadius(50.0);
	sphereTrigger->setTriggerCallback([](Object* other) {
		const char* shapeStr = (other->getShape() == CollisionShape::Sphere) ? "sphere" : "box";
		SDL_Log("SPHERE TRIGGER activated by %s at (%.2f, %.2f)!", shapeStr, other->getX(), other->getY());
		});
	sphereTrigger->setContinuous(false);
	objects.emplace_back(sphereTrigger);

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
		if (key == SDLK_F1) {
			renderOverlay = !renderOverlay;
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

		std::vector<Object*> allObjects;
		allObjects.insert(allObjects.end(), players.begin(), players.end());
		allObjects.insert(allObjects.end(), objects.begin(), objects.end());

		std::vector<CollisionPair> potentialCollisions = CollisionSystem::broadPhase(allObjects);
		std::vector<CollisionInfo> collisions = CollisionSystem::narrowPhase(potentialCollisions);
		CollisionSystem::resolveCollisions(collisions);
	}

	const size_t playerCount = players.size();
	std::vector<SDL_FRect> playerRects(playerCount);

	for (size_t i = 0; i < playerCount; i++) {
		playerRects[i] = players[i]->getRect();
	}

	SDL_SetRenderDrawColor(renderer, 225, 225, 225, SDL_ALPHA_OPAQUE);
	SDL_RenderRects(renderer, playerRects.data(), players.size());
	SDL_RenderFillRects(renderer, playerRects.data(), players.size());

	const size_t objectCount = objects.size();
	std::vector<SDL_FRect> objectRects(objectCount);

	for (size_t i = 0; i < objectCount; i++) {
		objectRects[i] = objects[i]->getRect();
		SDL_Texture* texture = getTexture(objects[i]->getTextureId());
		SDL_RenderTexture(renderer, texture, NULL, &objectRects[i]);
	}

	if (renderOverlay) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		for (size_t i = 0; i < objects.size(); i++) {
			ObjectType type = objects[i]->getType();
			CollisionShape shape = objects[i]->getShape();

			if (type == ObjectType::Trigger) {
				SDL_SetRenderDrawColor(renderer, 100, 255, 100, 75);
			}
			else if (type == ObjectType::Static) {
				SDL_SetRenderDrawColor(renderer, 255, 100, 100, 75);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 219, 171, 83, 75);
			}
			SDL_RenderFillRect(renderer, &objectRects[i]);
			
			if (type == ObjectType::Trigger) {
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 200);
			}
			else if (type == ObjectType::Static) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 219, 171, 83, 160);
			}
			SDL_RenderRect(renderer, &objectRects[i]);
			
			if (shape == CollisionShape::Sphere) {
				SDL_FRect* rect = &objectRects[i];
				SDL_RenderLine(renderer, rect->x, rect->y, rect->x + rect->w, rect->y + rect->h);
				SDL_RenderLine(renderer, rect->x + rect->w, rect->y, rect->x, rect->y + rect->h);
			}
		}

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
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
	for (size_t i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
}
