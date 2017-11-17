#ifndef __Box2DContactListener_H__
#define __Box2DContactListener_H__

class Box2DContactListener : public b2ContactListener
{
public:

public:
    Box2DContactListener();
    ~Box2DContactListener();

    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
};

#endif //__Box2DContactListener_H__
