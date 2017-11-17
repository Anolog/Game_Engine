#include "pch.h"

Box2DContactListener::Box2DContactListener()
{
}

Box2DContactListener::~Box2DContactListener()
{
}

void Box2DContactListener::BeginContact(b2Contact* contact)
{
    b2Fixture* pFixture[2];
    GameObject* pGameObjects[2];

    pFixture[0] = contact->GetFixtureA();
    pFixture[1] = contact->GetFixtureB();

    pGameObjects[0] = (GameObject*)pFixture[0]->GetBody()->GetUserData();
    pGameObjects[1] = (GameObject*)pFixture[1]->GetBody()->GetUserData();

    b2WorldManifold worldManifold;
    contact->GetWorldManifold( &worldManifold );
    vec2 position( worldManifold.points[0].x, worldManifold.points[0].y );
    vec2 normal( worldManifold.normal.x, worldManifold.normal.y );

    pGameObjects[0]->BeginContact( pFixture[1], position, normal );
    pGameObjects[1]->BeginContact( pFixture[0], position, normal * -1 );
}

void Box2DContactListener::EndContact(b2Contact* contact)
{
    //int bp = 1;
}

void Box2DContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    b2Fixture* pFixture[2];
    GameObject* pGameObjects[2];

    pFixture[0] = contact->GetFixtureA();
    pFixture[1] = contact->GetFixtureB();

    pGameObjects[0] = (GameObject*)pFixture[0]->GetBody()->GetUserData();
    pGameObjects[1] = (GameObject*)pFixture[1]->GetBody()->GetUserData();

    float zdiff = pGameObjects[0]->GetPosition().z - pGameObjects[1]->GetPosition().z;
    if( fabs( zdiff ) > 0.5f )
        contact->SetEnabled( false );
}
