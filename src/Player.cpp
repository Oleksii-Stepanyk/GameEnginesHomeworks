#include "headers/Player.h"

Player::Player() : x(25), y(25), w(25), h(25) {}

Player::Player(double startX, double startY, double startW, double startH) : x(startX), y(startY), w(startW), h(startH) {}

double Player::getX() const {
	return x;
}

double Player::getY() const {
	return y;
}

double Player::getW() const {
	return w;
}

double Player::getH() const {
	return h;
}

SDL_FRect Player::getRect() const {
	SDL_FRect rect;
	rect.x = static_cast<float>(x);
	rect.y = static_cast<float>(y);
	rect.w = static_cast<float>(w);
	rect.h = static_cast<float>(h);
	return rect;
}

std::vector<double> Player::getPosition() const {
	return {x, y};
}

std::vector<double> Player::getSize() const {
	return {w, h};
}

void Player::setPosition(double newX, double newY) {
	x = newX;
	y = newY;
}

void Player::setSize(double newW, double newH) {
	w = newW;
	h = newH;
}