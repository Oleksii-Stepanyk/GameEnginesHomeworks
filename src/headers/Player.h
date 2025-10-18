#include<vector>
#include<SDL3/SDL_rect.h>
#ifndef PLAYER_H
#define PLAYER_H
class Player // Square
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
};
#endif // PLAYER_H