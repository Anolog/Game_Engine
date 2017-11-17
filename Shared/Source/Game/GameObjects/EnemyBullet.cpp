#include "pch.h"

EnemyBullet::EnemyBullet(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename)
: GameObject( pScene, name, pos, rot, scale, meshname, shadername, texturename )
{
	type = "Pooled";
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Update(float deltatime)
{
    GameObject::Update( deltatime );

}

void EnemyBullet::BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal)
{
    GameObject::BeginContact( pOtherFixture, worldPosition, worldNormal );

}

void EnemyBullet::EndContact(b2Fixture* pOtherFixture)
{
    GameObject::EndContact( pOtherFixture );
}
