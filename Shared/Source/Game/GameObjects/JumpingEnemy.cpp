#include "pch.h"
#include "JumpingEnemy.h"

const float JUMP_DELAY = 1.0f; //set this to 0 if you dont want any delay

JumpingEnemy::JumpingEnemy(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename)
	: GameObject(pScene, name, pos, rot, scale, meshname, shadername, texturename)

{
	m_CanJump = false;
	m_Alive = true;
	m_DetectionRange = 5.0f;
	m_JumpTimer = 0;
	m_DeathTimer = 0;
	m_StartXPos = pos.x;
	type = "Enemy";
}

JumpingEnemy::~JumpingEnemy()
{

}

void JumpingEnemy::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	if (m_Alive == false)
	{
		m_DeathTimer += deltaTime;
		if (m_pPhysicsBody->GetFixtureList()->GetFilterData().categoryBits == CATEGORY_ENEMY) // do this once
		{
			m_pPhysicsBody->SetLinearVelocity(b2Vec2(0.0f, m_pPhysicsBody->GetLinearVelocity().y));
			SetCategory(CATEGORY_DEAD);
			SetMask(MASK_DEAD);
			//m_pPhysicsBody->SetAwake(false);
			SetFrame(4);
			m_pScene->AddtoScore(100);

		}
		if (m_DeathTimer >= 2.0f)
		{
			//RemoveFromScene();
			SetPosition(-10000);
			m_DeathTimer = 0;
		}
		return;
	}

	if (m_Alive = true)
	{


		GameObject* p_Player = m_pScene->GetGameObject("Player");

		float dist = sqrtf((p_Player->GetPosition().x - m_Position.x) * (p_Player->GetPosition().x - m_Position.x) +
			(p_Player->GetPosition().y - m_Position.y) * (p_Player->GetPosition().y - m_Position.y));

		if (dist <= m_DetectionRange)
		{

			//face the player
			if (p_Player->GetPosition().x < m_Position.x)
			{
				SetMesh(m_pScene->GetResourceManager()->GetMesh("SpriteReversed"));
			}

			if (p_Player->GetPosition().x > m_Position.x)
			{
				SetMesh(m_pScene->GetResourceManager()->GetMesh("Sprite"));
			}


			if (m_CanJump)
			{
				m_JumpTimer += deltaTime;
				if (m_JumpTimer >= JUMP_DELAY)
				{

					m_pPhysicsBody->ApplyLinearImpulse(b2Vec2(0, 6), m_pPhysicsBody->GetWorldCenter(), true);
					m_JumpTimer = 0;
					m_CanJump = false;
				}
			}

			//reset x pos if pushed by player
			m_Position.x = m_StartXPos;
			m_pPhysicsBody->SetLinearVelocity(b2Vec2(0.0f, m_pPhysicsBody->GetLinearVelocity().y));
		}

		//enemyanims
		if (m_JumpTimer < JUMP_DELAY / 3 * 1)
			SetFrame(1);
		if (m_JumpTimer >= JUMP_DELAY / 3 * 1)
			SetFrame(0);
		if (m_JumpTimer >= JUMP_DELAY / 3 * 2)
			SetFrame(1);
		if (m_pPhysicsBody->GetLinearVelocity().y != 0.0f)
		{
			SetFrame(2);
		}


	}
}

void JumpingEnemy::BeginContact(b2Fixture * pOtherFixture, vec2 worldPosition, vec2 worldNormal)
{
	if (m_Alive)
	{
		if (pOtherFixture->GetFilterData().categoryBits == CATEGORY_PBULLET)
		{
			m_Alive = false;
		
		}
		if (worldNormal.y < 0.0f && pOtherFixture->GetFilterData().categoryBits == CATEGORY_GROUND)
		{
			m_CanJump = true;
			//m_pSoundDude->playSound(Sound_Effect_Land);
		}
	}
}

void JumpingEnemy::EndContact(b2Fixture * pOtherFixture)
{

}
