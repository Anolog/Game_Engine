#include "pch.h"
#include "Collectable.h"



Collectable::Collectable(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename, int pointsadded)
	: GameObject(pScene, name, pos, rot, scale, meshname, shadername, texturename), m_PointsWorth(pointsadded)

{
	m_IsPicked = false;
	m_pSoundDude = new SoundManager();
}

Collectable::~Collectable()
{
	if (m_pSoundDude != nullptr)
	{
		delete m_pSoundDude;
		m_pSoundDude = nullptr;
	}
}

void Collectable::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
	if (m_IsPicked == true)
	{
		m_pScene->AddtoScore(m_PointsWorth);
		m_pSoundDude->playSound(Sound_Effect_Coin);
		RemoveFromScene();
		
	}

	
}

void Collectable::BeginContact(b2Fixture * pOtherFixture, vec2 worldPosition, vec2 worldNormal)
{
	if (pOtherFixture->GetFilterData().categoryBits == CATEGORY_PLAYER)
	{
		m_IsPicked = true;
	}
}

void Collectable::EndContact(b2Fixture * pOtherFixture)
{

}
