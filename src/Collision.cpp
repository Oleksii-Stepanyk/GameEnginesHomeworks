#include "headers/Collision.h"
#include <algorithm>
#include <cmath>

std::vector<CollisionPair> CollisionSystem::broadPhase(const std::vector<Object*>& objects) {
	std::vector<CollisionPair> potentialCollisions;

	for (size_t i = 0; i < objects.size(); i++) {
		for (size_t j = i + 1; j < objects.size(); j++) {
			if (aabbIntersect(objects[i], objects[j])) {
				potentialCollisions.emplace_back(objects[i], objects[j]);
			}
		}
	}

	return potentialCollisions;
}

std::vector<CollisionInfo> CollisionSystem::narrowPhase(const std::vector<CollisionPair>& potentialCollisions) {
	std::vector<CollisionInfo> collisions;

	for (const auto& pair : potentialCollisions) {
		CollisionInfo* info = nullptr;

		CollisionShape shapeA = pair.objA->getShape();
		CollisionShape shapeB = pair.objB->getShape();

		if (shapeA == CollisionShape::Box && shapeB == CollisionShape::Box) {
			info = checkAABBCollision(pair.objA, pair.objB);
		}
		else if (shapeA == CollisionShape::Sphere && shapeB == CollisionShape::Sphere) {
			info = checkSphereCollision(pair.objA, pair.objB);
		}
		else {
			if (shapeA == CollisionShape::Sphere) {
				info = checkSphereBoxCollision(pair.objA, pair.objB);
			}
			else {
				info = checkSphereBoxCollision(pair.objB, pair.objA);
			}
		}

		if (info != nullptr) {
			collisions.push_back(*info);
			delete info;
		}
	}

	return collisions;
}

void CollisionSystem::resolveCollisions(const std::vector<CollisionInfo>& collisions) {
	for (const auto& collision : collisions) {
		Object* objA = collision.objA;
		Object* objB = collision.objB;

		bool aIsTrigger = isTriggerObject(objA);
		bool bIsTrigger = isTriggerObject(objB);

		if (aIsTrigger || bIsTrigger) {
			if (aIsTrigger) {
				handleTriggerCollision(objA, objB);
			}
			if (bIsTrigger) {
				handleTriggerCollision(objB, objA);
			}
			continue;
		}

		bool aIsStatic = isStaticObject(objA);
		bool bIsStatic = isStaticObject(objB);

		if (aIsStatic && bIsStatic) {
			continue;
		}

		double depth = collision.penetrationDepth;
		CollisionShape shapeA = objA->getShape();
		CollisionShape shapeB = objB->getShape();

		double separationMultiplier = 1.01;
		double adjustedDepth = depth * separationMultiplier;

		double posAX, posAY, posBX, posBY;

		if (shapeA == CollisionShape::Sphere) {
			posAX = objA->getX() + objA->getW() / 2.0;
			posAY = objA->getY() + objA->getH() / 2.0;
		}
		else {
			posAX = objA->getX();
			posAY = objA->getY();
		}

		if (shapeB == CollisionShape::Sphere) {
			posBX = objB->getX() + objB->getW() / 2.0;
			posBY = objB->getY() + objB->getH() / 2.0;
		}
		else {
			posBX = objB->getX();
			posBY = objB->getY();
		}

		if (aIsStatic) {
			if (shapeB == CollisionShape::Sphere) {
				double newCenterX = posBX + collision.normalX * adjustedDepth;
				double newCenterY = posBY + collision.normalY * adjustedDepth;
				objB->setPosition(
					newCenterX - objB->getW() / 2.0,
					newCenterY - objB->getH() / 2.0
				);
			}
			else {
				objB->setPosition(
					posBX + collision.normalX * adjustedDepth,
					posBY + collision.normalY * adjustedDepth
				);
			}
		}
		else if (bIsStatic) {
			if (shapeA == CollisionShape::Sphere) {
				double newCenterX = posAX - collision.normalX * adjustedDepth;
				double newCenterY = posAY - collision.normalY * adjustedDepth;
				objA->setPosition(
					newCenterX - objA->getW() / 2.0,
					newCenterY - objA->getH() / 2.0
				);
			}
			else {
				objA->setPosition(
					posAX - collision.normalX * adjustedDepth,
					posAY - collision.normalY * adjustedDepth
				);
			}
		}
		else {
			double halfDepth = adjustedDepth * 0.5;

			if (shapeA == CollisionShape::Sphere) {
				double newCenterX = posAX - collision.normalX * halfDepth;
				double newCenterY = posAY - collision.normalY * halfDepth;
				objA->setPosition(
					newCenterX - objA->getW() / 2.0,
					newCenterY - objA->getH() / 2.0
				);
			}
			else {
				objA->setPosition(
					posAX - collision.normalX * halfDepth,
					posAY - collision.normalY * halfDepth
				);
			}

			if (shapeB == CollisionShape::Sphere) {
				double newCenterX = posBX + collision.normalX * halfDepth;
				double newCenterY = posBY + collision.normalY * halfDepth;
				objB->setPosition(
					newCenterX - objB->getW() / 2.0,
					newCenterY - objB->getH() / 2.0
				);
			}
			else {
				objB->setPosition(
					posBX + collision.normalX * halfDepth,
					posBY + collision.normalY * halfDepth
				);
			}
		}
	}
}

bool CollisionSystem::aabbIntersect(const Object* a, const Object* b) {
	double aLeft = a->getX();
	double aRight = a->getX() + a->getW();
	double aTop = a->getY();
	double aBottom = a->getY() + a->getH();

	double bLeft = b->getX();
	double bRight = b->getX() + b->getW();
	double bTop = b->getY();
	double bBottom = b->getY() + b->getH();

	return !(aRight < bLeft || aLeft > bRight || aBottom < bTop || aTop > bBottom);
}

CollisionInfo* CollisionSystem::checkAABBCollision(Object* a, Object* b) {
	if (!aabbIntersect(a, b)) {
		return nullptr;
	}

	double aLeft = a->getX();
	double aRight = a->getX() + a->getW();
	double aTop = a->getY();
	double aBottom = a->getY() + a->getH();

	double bLeft = b->getX();
	double bRight = b->getX() + b->getW();
	double bTop = b->getY();
	double bBottom = b->getY() + b->getH();

	double overlapLeft = aRight - bLeft;
	double overlapRight = bRight - aLeft;
	double overlapTop = aBottom - bTop;
	double overlapBottom = bBottom - aTop;

	double minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

	double normalX = 0.0;
	double normalY = 0.0;

	if (minOverlap == overlapLeft) {
		normalX = 1.0;
		normalY = 0.0;
	}
	else if (minOverlap == overlapRight) {
		normalX = -1.0;
		normalY = 0.0;
	}
	else if (minOverlap == overlapTop) {
		normalX = 0.0;
		normalY = 1.0;
	}
	else {
		normalX = 0.0;
		normalY = -1.0;
	}

	return new CollisionInfo(a, b, minOverlap, normalX, normalY);
}

bool CollisionSystem::isStaticObject(Object* obj) {
	return obj->getType() == ObjectType::Static && obj->getIsImmovable();
}

bool CollisionSystem::isTriggerObject(Object* obj) {
	return obj->getType() == ObjectType::Trigger;
}

void CollisionSystem::handleTriggerCollision(Object* trigger, Object* other) {
	if (trigger->getType() == ObjectType::Trigger) {
		trigger->trigger(other);
	}
}

double CollisionSystem::clamp(double value, double min, double max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

CollisionInfo* CollisionSystem::checkSphereCollision(Object* a, Object* b) {
	double aCenterX = a->getX() + a->getW() / 2.0;
	double aCenterY = a->getY() + a->getH() / 2.0;
	double aRadius = a->getRadius();

	double bCenterX = b->getX() + b->getW() / 2.0;
	double bCenterY = b->getY() + b->getH() / 2.0;
	double bRadius = b->getRadius();

	double dx = bCenterX - aCenterX;
	double dy = bCenterY - aCenterY;
	double distanceSquared = dx * dx + dy * dy;
	double radiusSum = aRadius + bRadius;

	if (distanceSquared >= radiusSum * radiusSum) {
		return nullptr;
	}

	double distance = std::sqrt(distanceSquared);

	if (distance < 0.1) {
		distance = 0.1;
		if (std::abs(dx) < 0.001 && std::abs(dy) < 0.001) {
			dx = 1.0;
			dy = 0.0;
		}
	}

	double normalX = dx / distance;
	double normalY = dy / distance;
	double penetrationDepth = radiusSum - distance;

	if (penetrationDepth < 0.1) {
		return nullptr;
	}

	return new CollisionInfo(a, b, penetrationDepth, normalX, normalY);
}

CollisionInfo* CollisionSystem::checkSphereBoxCollision(Object* sphere, Object* box) {
	double sphereCenterX = sphere->getX() + sphere->getW() / 2.0;
	double sphereCenterY = sphere->getY() + sphere->getH() / 2.0;
	double sphereRadius = sphere->getRadius();

	double boxLeft = box->getX();
	double boxRight = box->getX() + box->getW();
	double boxTop = box->getY();
	double boxBottom = box->getY() + box->getH();

	double closestX = clamp(sphereCenterX, boxLeft, boxRight);
	double closestY = clamp(sphereCenterY, boxTop, boxBottom);

	double dx = sphereCenterX - closestX;
	double dy = sphereCenterY - closestY;
	double distanceSquared = dx * dx + dy * dy;

	if (distanceSquared >= sphereRadius * sphereRadius) {
		return nullptr;
	}

	double distance = std::sqrt(distanceSquared);

	double normalX, normalY;
	if (distance < 0.001) {
		double boxCenterX = (boxLeft + boxRight) / 2.0;
		double boxCenterY = (boxTop + boxBottom) / 2.0;
		dx = sphereCenterX - boxCenterX;
		dy = sphereCenterY - boxCenterY;
		double centerDist = std::sqrt(dx * dx + dy * dy);

		if (centerDist < 0.001) {
			normalX = 1.0;
			normalY = 0.0;
		}
		else {
			normalX = dx / centerDist;
			normalY = dy / centerDist;
		}

		distance = 0.001;
	}
	else {
		normalX = dx / distance;
		normalY = dy / distance;
	}

	double penetrationDepth = sphereRadius - distance;

	if (penetrationDepth < 0.1) {
		return nullptr;
	}

	return new CollisionInfo(box, sphere, penetrationDepth, normalX, normalY);
}
