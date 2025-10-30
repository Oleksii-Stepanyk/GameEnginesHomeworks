#include "headers/Object.h"

Object::Object() : x(25), y(25), w(25), h(25) {}

Object::Object(double startX, double startY, double startW, double startH) : x(startX), y(startY), w(startW), h(startH) {}

double Object::getX() const {
	return x;
}

double Object::getY() const {
	return y;
}

double Object::getW() const {
	return w;
}

double Object::getH() const {
	return h;
}

SDL_FRect Object::getRect() const {
	SDL_FRect rect;
	rect.x = static_cast<float>(x);
	rect.y = static_cast<float>(y);
	rect.w = static_cast<float>(w);
	rect.h = static_cast<float>(h);
	return rect;
}

std::vector<double> Object::getPosition() const {
	return { x, y };
}

std::vector<double> Object::getSize() const {
	return { w, h };
}

void Object::setPosition(double newX, double newY) {
	x = newX;
	y = newY;
}

void Object::setSize(double newW, double newH) {
	w = newW;
	h = newH;
}