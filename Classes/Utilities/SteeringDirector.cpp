#include <Utilities/SteeringDirector.h>

#include <2d/CCNode.h>
#include <physics/CCPhysicsBody.h>

USING_NS_CC;

SteeringDirector::SteeringDirector() {

}

SteeringDirector *SteeringDirector::getInstance() {
    static SteeringDirector *instance = new SteeringDirector();
    return instance;
}

void SteeringDirector::seek(Node *seeker, Vec2 seeking) {
    auto seekerBody = seeker->getPhysicsBody();

    CC_ASSERT(seekerBody != nullptr);

    Vec2 direction = seeking - seeker->getPosition();
    direction.normalize();

    Vec2 steeringForce = direction * seekerBody->getVelocityLimit();
    steeringForce = steeringForce / seekerBody->getMass();

    seekerBody->applyImpulse(steeringForce);
}
