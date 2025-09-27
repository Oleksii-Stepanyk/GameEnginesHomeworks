#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <thread>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 1280, 720, SDL_WINDOW_BORDERLESS, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	glm::vec3 vec1(1.0f, 2.0f, 3.0f);
	glm::vec3 vec2(4.0f, 5.0f, 6.0f);
	glm::vec3 vec3 = vec1 + vec2;
	std::cout << "vec3: (" << vec3.x << ", " << vec3.y << ", " << vec3.z << ")\n";
	std::cout << "SDL version: " << SDL_VERSION << "\n";

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
	}
	else if (event->type == SDL_EVENT_KEY_DOWN) {
		if (event->key.key == SDLK_ESCAPE) {
			return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
		}
	}
	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
	const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
	/* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
	const float red = (float)(0.5 + 0.5 * glm::sin(now));
	const float green = (float)(0.5 + 0.5 * glm::sin(now + glm::pi<double>() * 2 / 3));
	const float blue = (float)(0.5 + 0.5 * glm::sin(now + glm::pi<double>() * 4 / 3));
	SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

	/* clear the window to the draw color. */
	SDL_RenderClear(renderer);

	/* put the newly-cleared rendering on the screen. */
	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	/* SDL will clean up the window/renderer for us. */
}