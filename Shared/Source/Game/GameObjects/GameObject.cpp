#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename)
{
    m_RenderOrder = 2;

    Init( pScene, name, pos, rot, scale, meshname, shadername, texturename );
}

GameObject::~GameObject()
{

}

void GameObject::Init(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename)
{
    m_InitialPosition = pos;
    m_InitialRotation = rot;

    m_pScene = pScene;

    m_Name = name;

    m_Position = pos;
    m_Rotation = rot;
    m_Scale = scale;

    m_pPhysicsBody = 0;

	
    m_pMesh = 0;
    if( meshname != 0 )
        m_pMesh = m_pScene->GetResourceManager()->GetMesh( meshname );

    m_pShaderProgram = 0;
    if( shadername != 0 )
        m_pShaderProgram = m_pScene->GetResourceManager()->GetShader( shadername );

    m_TextureHandle = 0;
    if( texturename != 0 )
        m_TextureHandle = m_pScene->GetResourceManager()->GetTexture( texturename );
}

void GameObject::Reset()
{
    m_Position = m_InitialPosition;
    m_Rotation = m_InitialRotation;

    if( m_pPhysicsBody )
    {
        m_pPhysicsBody->SetLinearVelocity( b2Vec2( 0, 0 ) );
        m_pPhysicsBody->SetAngularVelocity( 0 );
        m_pPhysicsBody->SetTransform( b2Vec2( m_InitialPosition.x, m_InitialPosition.y ), m_InitialRotation.z * PI / 180 );
        m_pPhysicsBody->SetAwake( true );
    }
}

void GameObject::AddToScene(Scene* pScene)
{
    m_pScene = pScene;
    m_pScene->AddToScene( m_Name, this );
}

void GameObject::RemoveFromScene()
{
    if( m_pScene )
    {
        m_pScene->RemoveFromScene( this );
        m_pScene = 0;
    }
}

void GameObject::Update(float deltatime)
{
    if( m_pPhysicsBody )
    {
        b2Vec2 pos = m_pPhysicsBody->GetPosition();
        float32 angle = -m_pPhysicsBody->GetAngle() / PI * 180.0f;

        m_Position.x = pos.x;
        m_Position.y = pos.y;

        m_Rotation.z = angle;
    }
}

void GameObject::Draw(int renderorder, mat4* viewmat, mat4* projmat, vec3 campos)
{
    if( m_pMesh == 0 )
        return;

    mat4 worldmat;
    worldmat.CreateSRT( m_Scale, m_Rotation, m_Position );

    m_pMesh->SetupAttributes( m_pShaderProgram );

	vec4 lightColor[3] = { vec4(1, 0, 0, 1), vec4(0, 1, 0, 1), vec4(0, 0, 1, 1) };
	vec3 lightPos[3] = { vec3(-0.5, 0.5, 0), vec3(0.5, 0.5, 0), vec3(0, -0.5, 0) };

    m_pMesh->SetupUniforms( m_pShaderProgram, &worldmat, viewmat, projmat, campos, m_TextureHandle, m_Frame, vec4(1,1,1,1), lightColor, lightPos );

    CustomUniformSetup();

    m_pMesh->Draw( m_pShaderProgram );

	//OutputMessage( "Drawing: %s\n", m_Name.c_str() );
}

void GameObject::BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal)
{
    //OutputMessage( "Normal: %s, %0.2f, %0.2f\n", m_Name.c_str(), worldNormal.x, worldNormal.y );
}

void GameObject::EndContact(b2Fixture* pOtherFixture)
{
    //OutputMessage( "Normal: %s, %0.2f, %0.2f\n", m_Name.c_str(), worldNormal.x, worldNormal.y );
}

b2Fixture* GameObject::AddPhysicsShape(b2Shape* shape, bool dynamic, bool active, float density, short category, short mask, bool sensor)
{
    if( m_pPhysicsBody == 0 )
    {
        b2BodyDef bodydef;
        bodydef.position.Set( m_Position.x, m_Position.y ); // bottom-left corner
        bodydef.angle = -m_Rotation.z * PI / 180.0f;
        if( dynamic )
            bodydef.type = b2_dynamicBody;
        else
            bodydef.type = b2_staticBody;

        bodydef.active = active;

        m_pPhysicsBody = m_pScene->GetBox2DWorld()->GetWorld()->CreateBody( &bodydef );
        m_pPhysicsBody->SetUserData( this );
    }

    b2FixtureDef fixturedef;
    fixturedef.shape = shape;
    fixturedef.density = density;
    fixturedef.isSensor = sensor;
    fixturedef.friction = 0.2f;
    fixturedef.restitution = 0;
	fixturedef.filter.categoryBits = category;
	if (mask != -1)
		fixturedef.filter.maskBits = mask;

    return m_pPhysicsBody->CreateFixture( &fixturedef );
}

b2Fixture* GameObject::AddPhysicsBox(vec2 size, bool dynamic, bool active, float density, short category, short mask, bool sensor)
{
    b2PolygonShape boxshape;
    boxshape.SetAsBox( size.x/2, size.y/2 );

    return AddPhysicsShape( &boxshape, dynamic, active, density, category, mask, sensor );
}

b2Fixture* GameObject::AddPhysicsDentedBox(vec2 size, float indent, bool dynamic, bool active, float density, short category, short mask, bool sensor)
{
    b2PolygonShape boxshape;

    b2Vec2 points[8];
    float hx = size.x/2;
    float hy = size.y/2;

    points[0].Set( -hx         , -hy + indent ); // lower left - left
    points[1].Set( -hx + indent, -hy          ); // lower left - lower
    points[2].Set(  hx - indent, -hy          ); // lower right - lower
    points[3].Set(  hx         , -hy + indent ); // lower right - right
    points[4].Set(  hx         ,  hy - indent ); // upper right - right
    points[5].Set(  hx - indent,  hy          ); // upper right - upper
    points[6].Set( -hx + indent,  hy          ); // upper left - upper
    points[7].Set( -hx         ,  hy - indent ); // upper left - left

    boxshape.Set( points, 8 );

    return AddPhysicsShape( &boxshape, dynamic, active, density, category, mask, sensor );
}

b2Fixture* GameObject::AddPhysicsChain(b2Vec2* pointlist, int numpoints, bool dynamic, bool active, float density, short category, short mask, bool sensor)
{
    b2ChainShape chainshape;
    chainshape.CreateChain( pointlist, numpoints );

    return AddPhysicsShape( &chainshape, dynamic, active, density, category, mask, sensor );
}

b2Fixture* GameObject::AddPhysicsCircle(float radius, bool dynamic, bool active, float density, short category, short mask, bool sensor)
{
    b2CircleShape circleshape;
    circleshape.m_radius = radius;

    return AddPhysicsShape( &circleshape, dynamic, active, density, category, mask, sensor );
}

void GameObject::SetPosition(vec3 pos)
{
    m_Position = pos;
    if( m_pPhysicsBody )
        m_pPhysicsBody->SetTransform( b2Vec2( m_Position.x, m_Position.y ), -m_Rotation.z * PI / 180.0f );
}

void GameObject::SetRotation(vec3 rot)
{
    m_Rotation = rot;
    if( m_pPhysicsBody )
        m_pPhysicsBody->SetTransform( b2Vec2( m_Position.x, m_Position.y ), -m_Rotation.z * PI / 180.0f );
}

std::string GameObject::returnType()
{
	return type;
}

void GameObject::SetMask(FilterMasks mask)
{
	b2Filter filter = m_pPhysicsBody->GetFixtureList()->GetFilterData();
	filter.maskBits = mask;
	m_pPhysicsBody->GetFixtureList()->SetFilterData(filter);
}

void GameObject::SetCategory(FilterCategory category)
{
	b2Filter filter = m_pPhysicsBody->GetFixtureList()->GetFilterData();
	filter.categoryBits = category;
	m_pPhysicsBody->GetFixtureList()->SetFilterData(filter);
}