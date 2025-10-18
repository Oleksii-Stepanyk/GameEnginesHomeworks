#ifndef PLAYER_H
#define PLAYER_H

#include<vector>
#include<SDL3/SDL.h>
#include"GameConfig.h"

class Player
{
private:
	double x;
	double y;
	double w;
	double h;

public:
	Player();
	Player(double startX, double startY, double startW, double startH);

	double getX() const;
	double getY() const;
	double getW() const;
	double getH() const;
	
	SDL_FRect getRect() const;

	std::vector<double> getPosition() const;
	std::vector<double> getSize() const;

	void setSize(double newW, double newH);
	void setPosition(double newX, double newY);

	void update();
};
#endif // PLAYER_H