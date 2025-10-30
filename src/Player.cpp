#include "headers/Player.h"

Player::Player() : Object(), accelerationX(0.0), accelerationY(0.0) {}
Player::Player(double startX, double startY, double startW, double startH)
	: Object(startX, startY, startW, startH), accelerationX(0.0), accelerationY(0.0) {}

static inline void applyFriction(double& acceleration, double friction) {
	if (acceleration > 0) {
		acceleration -= friction;
		if (acceleration < 0) acceleration = 0;
	}
	else if (acceleration < 0) {
		acceleration += friction;
		if (acceleration > 0) acceleration = 0;
	}
	if (std::abs(acceleration) < GameConfig::ACCELERATION_EPSILON) acceleration = 0;
}

static inline double normalize(double acceleration, double lowBound, double highBound) {
	return acceleration < lowBound ? lowBound : (acceleration > highBound ? highBound : acceleration);
};

void Player::update() {
	const double acceleration = GameConfig::ACCELERATION;
	const double maxAcceleration = GameConfig::MAX_ACCELERATION;
	const double friction = acceleration * GameConfig::ACCELERATION_BREAK_COEF;
	const double speed = GameConfig::PLAYER_SPEED;

	auto key = SDL_GetKeyboardState(NULL);

	int inputX = (key[SDL_SCANCODE_D] || key[SDL_SCANCODE_RIGHT] ? 1 : 0)
		- (key[SDL_SCANCODE_A] || key[SDL_SCANCODE_LEFT] ? 1 : 0);
	int inputY = (key[SDL_SCANCODE_S] || key[SDL_SCANCODE_DOWN] ? 1 : 0)
		- (key[SDL_SCANCODE_W] || key[SDL_SCANCODE_UP] ? 1 : 0);

	accelerationX += inputX * acceleration;
	accelerationY += inputY * acceleration;

	accelerationX = normalize(accelerationX, -maxAcceleration, maxAcceleration);
	accelerationY = normalize(accelerationY, -maxAcceleration, maxAcceleration);

	double newX = x + speed * accelerationX;
	double newY = y + speed * accelerationY;

	if (newX < 0 || newX + w > GameConfig::WINDOW_WIDTH)
	{
		newX = x;
		accelerationX = 0;
	}
	if (newY < 0 || newY + h > GameConfig::WINDOW_HEIGHT)
	{
		newY = y;
		accelerationY = 0;
	}

	setPosition(newX, newY);

	applyFriction(accelerationX, friction);
	applyFriction(accelerationY, friction);
}