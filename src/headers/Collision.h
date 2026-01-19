#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include <utility>
#include "Object.h"

class TriggerObject;
class StaticObject;

struct CollisionPair {
    Object* objA;
    Object* objB;
    
    CollisionPair(Object* a, Object* b) : objA(a), objB(b) {}
};

struct CollisionInfo {
    Object* objA;
    Object* objB;
    double penetrationDepth;
    double normalX;
    double normalY;
    
    CollisionInfo(Object* a, Object* b, double depth, double nx, double ny)
        : objA(a), objB(b), penetrationDepth(depth), normalX(nx), normalY(ny) {}
};

class CollisionSystem {
public:
    static std::vector<CollisionPair> broadPhase(const std::vector<Object*>& objects);
    
    static std::vector<CollisionInfo> narrowPhase(const std::vector<CollisionPair>& potentialCollisions);
    
    static void resolveCollisions(const std::vector<CollisionInfo>& collisions);

private:
    static bool aabbIntersect(const Object* a, const Object* b);
    
    static CollisionInfo* checkAABBCollision(Object* a, Object* b);
    static CollisionInfo* checkSphereCollision(Object* a, Object* b);
    static CollisionInfo* checkSphereBoxCollision(Object* sphere, Object* box);
    
    static bool isStaticObject(Object* obj);
    static bool isTriggerObject(Object* obj);
    
    static void handleTriggerCollision(Object* trigger, Object* other);
    
    static double clamp(double value, double min, double max);
};

#endif // COLLISION_H
