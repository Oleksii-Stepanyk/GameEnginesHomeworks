#include "headers/Object.h"
#include <algorithm>

Object::Object()
: x(25), y(25), w(25), h(25), textureId(""),
type(ObjectType::Dynamic), shape(CollisionShape::Box), radius(12.5), isImmovable(false),
onTriggerCallback(nullptr), isTriggered(false), isContinuous(false) {}

Object::Object(double startX, double startY, double startW, double startH)
: x(startX), y(startY), w(startW), h(startH), textureId(""),
type(ObjectType::Dynamic), shape(CollisionShape::Box), radius(std::min(startW, startH) / 2.0), isImmovable(false),
onTriggerCallback(nullptr), isTriggered(false), isContinuous(false) {}

Object::Object(double startX, double startY, double startW, double startH, const std::string& textureId)
: x(startX), y(startY), w(startW), h(startH), textureId(textureId),
type(ObjectType::Dynamic), shape(CollisionShape::Box), radius(std::min(startW, startH) / 2.0), isImmovable(false),
onTriggerCallback(nullptr), isTriggered(false), isContinuous(false) {}

Object::Object(double startX, double startY, double startW, double startH, const std::string& textureId, ObjectType objType)
: x(startX), y(startY), w(startW), h(startH), textureId(textureId),
type(objType), shape(CollisionShape::Box), radius(std::min(startW, startH) / 2.0), isImmovable(objType == ObjectType::Static),
onTriggerCallback(nullptr), isTriggered(false), isContinuous(false) {}

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

std::string Object::getTextureId() const {
	return textureId;
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

ObjectType Object::getType() const {
	return type;
}

void Object::setType(ObjectType objType) {
	type = objType;
	if (type == ObjectType::Static) {
		isImmovable = true;
	}
	else if (type == ObjectType::Dynamic) {
		isImmovable = false;
	}
}

bool Object::getIsImmovable() const {
	return isImmovable;
}

void Object::setImmovable(bool immovable) {
	isImmovable = immovable;
}

void Object::setTriggerCallback(std::function<void(Object*)> callback) {
	onTriggerCallback = callback;
}

void Object::trigger(Object* other) {
	if (isContinuous || !isTriggered) {
		isTriggered = true;
		if (onTriggerCallback) {
			onTriggerCallback(other);
		}
	}
}

void Object::resetTrigger() {
	isTriggered = false;
}

bool Object::getIsTriggered() const {
	return isTriggered;
}

void Object::setContinuous(bool continuous) {
	isContinuous = continuous;
}

bool Object::getIsContinuous() const {
	return isContinuous;
}

CollisionShape Object::getShape() const {
	return shape;
}

void Object::setShape(CollisionShape collisionShape) {
	shape = collisionShape;
	if (shape == CollisionShape::Sphere) {
		radius = std::min(w, h) / 2.0;
	}
}

double Object::getRadius() const {
	return radius;
}

void Object::setRadius(double r) {
	radius = r;
}