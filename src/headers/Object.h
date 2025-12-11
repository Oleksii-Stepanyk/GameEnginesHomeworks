#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <functional>
#include <SDL3/SDL.h>
#include "GameConfig.h"

enum class ObjectType {
	Dynamic,
	Static,
	Trigger
};

enum class CollisionShape {
	Box,
	Sphere
};

class Object
{
protected:
	double x;
	double y;
	double w;
	double h;
	std::string textureId;
	
	ObjectType type;
	CollisionShape shape;
	double radius;
	bool isImmovable;
	
	std::function<void(Object*)> onTriggerCallback;
	bool isTriggered;
	bool isContinuous;

public:
	Object();
	Object(double startX, double startY, double startW, double startH);
	Object(double startX, double startY, double startW, double startH, const std::string &textureId);
	Object(double startX, double startY, double startW, double startH, const std::string &textureId, ObjectType objType);
	
	virtual ~Object() = default;

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
	
	ObjectType getType() const;
	void setType(ObjectType objType);
	
	bool getIsImmovable() const;
	void setImmovable(bool immovable);
	
	CollisionShape getShape() const;
	void setShape(CollisionShape collisionShape);
	double getRadius() const;
	void setRadius(double r);
	
	void setTriggerCallback(std::function<void(Object*)> callback);
	void trigger(Object* other);
	void resetTrigger();
	bool getIsTriggered() const;
	void setContinuous(bool continuous);
	bool getIsContinuous() const;
};
#endif // OBJECT_H
