#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <SDL3/SDL.h>
#include "Object.h"
#include "GameConfig.h"

class Player : public Object
{
private:
	double accelerationX;
	double accelerationY;

public:
	Player();
	Player(double startX, double startY, double startW, double startH);

	void update();
};
#endif // PLAYER_H