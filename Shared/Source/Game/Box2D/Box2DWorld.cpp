#include "pch.h"

Box2DWorld::Box2DWorld(ShaderProgram* debugdrawshader, mat4* pMatView, mat4* pMatProj)
{
    m_pWorld = 0;

    m_pDebugDraw = 0;
    m_pContactListener = 0;

    m_pGround = 0;

    m_UnusedTime = 0;

    CreateWorld( debugdrawshader, pMatView, pMatProj );
}

Box2DWorld::~Box2DWorld()
{
    Cleanup();
}

void Box2DWorld::CreateWorld(ShaderProgram* debugdrawshader, mat4* pMatView, mat4* pMatProj)
{
    m_pWorld = new b2World( b2Vec2( 0, -10 ) );

    // Setup debug draw object.
    {
        m_pDebugDraw = new Box2DDebugDraw( debugdrawshader, pMatView, pMatProj );
        m_pWorld->SetDebugDraw( m_pDebugDraw );

        uint32 flags = b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_centerOfMassBit | b2Draw::e_aabbBit | b2Draw::e_pairBit;
        m_pDebugDraw->SetFlags( flags );
    }

    // Setup contact listener object.
    {
        m_pContactListener = new Box2DContactListener();

        m_pWorld->SetContactListener( m_pContactListener );
    }

    // Create a static ground body at 0,0
    {
        b2BodyDef bodydef;
        bodydef.position = b2Vec2( 0, 0 );
        bodydef.type = b2_staticBody;
        m_pGround = m_pWorld->CreateBody( &bodydef );
    }
}

void Box2DWorld::Update(float deltatime)
{
    if( false )
    {
        m_pWorld->Step( 1/60.0f, 8, 2 );
    }
    else
    {
        m_UnusedTime += deltatime;

        float timestep = 1/60.0f;

        while( m_UnusedTime > timestep )
        {
            m_pWorld->Step( timestep, 8, 2 );
            m_UnusedTime -= timestep;
        }

        m_pWorld->ClearForces();
    }
}

void Box2DWorld::Cleanup()
{
    delete m_pWorld;
    delete m_pDebugDraw;
    delete m_pContactListener;

    m_pGround = 0;
}

void Box2DWorld::SetViewProjMatrices(mat4* pMatView, mat4* pMatProj)
{
    m_pDebugDraw->SetViewProjMatrices( pMatView, pMatProj );
}
