#include "headers/Player.h"

Player::Player() : x(25), y(25), w(25), h(25), accelerationX(0.0), accelerationY(0.0) {}

Player::Player(double startX, double startY, double startW, double startH) : x(startX), y(startY), w(startW), h(startH), accelerationX(0.0), accelerationY(0.0) {}

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

void Player::update() {
	double moveX = 0;
	double moveY = 0;
	auto keyStates = SDL_GetKeyboardState(NULL);

	if (keyStates[SDL_SCANCODE_W] || keyStates[SDL_SCANCODE_UP]) {
		moveY -= GameConfig::PLAYER_SPEED;
		accelerationY += GameConfig::ACCELERATION_PER_FRAME;
	}
	if (keyStates[SDL_SCANCODE_S] || keyStates[SDL_SCANCODE_DOWN]) {
		moveY += GameConfig::PLAYER_SPEED;
		accelerationY += GameConfig::ACCELERATION_PER_FRAME;
	}
	if (keyStates[SDL_SCANCODE_A] || keyStates[SDL_SCANCODE_LEFT]) {
		moveX -= GameConfig::PLAYER_SPEED;
		accelerationX += GameConfig::ACCELERATION_PER_FRAME;
	}
	if (keyStates[SDL_SCANCODE_D] || keyStates[SDL_SCANCODE_RIGHT]) {
		moveX += GameConfig::PLAYER_SPEED;
		accelerationX += GameConfig::ACCELERATION_PER_FRAME;
	}
	if (accelerationX > GameConfig::MAX_ACCELERATION) {
		accelerationX = GameConfig::MAX_ACCELERATION;
	}
	if (accelerationY > GameConfig::MAX_ACCELERATION) {
		accelerationY = GameConfig::MAX_ACCELERATION;
	}

	if (moveX == 0) {
		moveX = previousSpeedX;
	}
	if (moveY == 0) {
		moveY = previousSpeedY;
	}

	double newX = x + moveX * accelerationX;
	double newY = y + moveY * accelerationY;

	if (newX < 0 || newX + w > GameConfig::WINDOW_WIDTH) {
		newX = x;
	}
	if (newY < 0 || newY + h > GameConfig::WINDOW_HEIGHT) {
		newY = y;
	}

	setPosition(newX, newY);
	
	accelerationX -= GameConfig::ACCELERATION_PER_FRAME / 1.75;
	accelerationY -= GameConfig::ACCELERATION_PER_FRAME / 1.75;
	if (accelerationX < 0) accelerationX = 0;
	if (accelerationY < 0) accelerationY = 0;
 
	previousSpeedX = moveX;
	previousSpeedY = moveY;
}