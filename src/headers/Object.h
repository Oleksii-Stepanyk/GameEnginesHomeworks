#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <SDL3/SDL.h>
#include "GameConfig.h"

class Object
{
protected:
	double x;
	double y;
	double w;
	double h;
	std::string textureId;

public:
	Object();
	Object(double startX, double startY, double startW, double startH);
	Object(double startX, double startY, double startW, double startH, const std::string &textureId);

	double getX() const;
	double getY() const;
	double getW() const;
	double getH() const;

	SDL_FRect getRect() const;
	std::string getTextureId() const;

	std::vector<double> getPosition() const;
	std::vector<double> getSize() const;

	void setSize(double newW, double newH);
	void setPosition(double newX, double newY);
};
#endif // OBJECT_H
