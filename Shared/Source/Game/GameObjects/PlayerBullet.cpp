#include "pch.h"

PlayerBullet::PlayerBullet(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename)
: GameObject( pScene, name, pos, rot, scale, meshname, shadername, texturename )
{
	type = "Pooled";
	fallOffRad = 20.0f;
}

PlayerBullet::~PlayerBullet()
{

}

void PlayerBullet::Update(float deltatime)
{
    GameObject::Update( deltatime );
	if (sqrtf((m_Position.x - m_pScene->GetGameObject("Player")->GetPosition().x)* (m_Position.x - m_pScene->GetGameObject("Player")->GetPosition().x) + (m_Position.y - m_pScene->GetGameObject("Player")->GetPosition().y) * (m_Position.y - m_pScene->GetGameObject("Player")->GetPosition().y)) > fallOffRad + 0.5 / 2)
	{
		m_pPhysicsBody->SetActive(false);
	}
	else
	{
		m_pPhysicsBody->SetActive(true);
	}
}

void PlayerBullet::BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal)
{
    GameObject::BeginContact( pOtherFixture, worldPosition, worldNormal );

}

void PlayerBullet::EndContact(b2Fixture* pOtherFixture)
{
    GameObject::EndContact( pOtherFixture );
}
