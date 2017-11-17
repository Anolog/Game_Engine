#include "pch.h"
#include "TurretEnemy.h"

const float BULLET_TIMER = 3.2f;
const float BULLET_SPEED =2.5f;
TurretEnemy::TurretEnemy(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename, int ID)
	: GameObject(pScene, name, pos, rot, scale, meshname, shadername, texturename)
	, m_pTurretBullets()
{
	m_isHit = false;
	m_BulletTimer = 0.0f;
	m_DetectionRange = 7.0f;
	m_ID = ID;
	m_AimPos = vec2(0, 0);

	m_disableOnce = false;

	//Make the bullet, using object pooling
	for (int i = 0; i < 20; i++)
	{
		//Formatted as, TurretBullet_Sn_Bn. TurretBullet, Static Num, Bullet Num
		std::string name = "TurretBullet_S" + std::to_string(m_ID) + "_B" + std::to_string(i);
		vec3 pos = vec3(0, 0, 0);

		//The playerbullet and enemybullet are both internally the same, but I didn't really realize that when I coppied the files, so.... I'm just too lazy to change it for now
		EnemyBullet* pEnemyBullet = new EnemyBullet(m_pScene, name, m_Position, vec3(0, 0, 0), vec3(0.5 / 2, 0.5 / 2, 0.5 / 2), "Sprite", "Texture", "Bullet");
		pEnemyBullet->SetRenderOrder(3);
		pEnemyBullet->AddPhysicsCircle(0.5 / 2, true, false, 0.5f, CATEGORY_EBULLET, MASK_EBULLET);
		m_pTurretBullets.AddNewObjectToPool(pEnemyBullet);
	}

}

TurretEnemy::~TurretEnemy()
{
	/*
	for (int i = 0; i < m_pActiveBullets.size(); i++)
	{
		if (m_pActiveBullets.at(i) != nullptr)
		{
			delete m_pActiveBullets.at(i);
			m_pActiveBullets.at(i) = nullptr;
		}
	}
	*/
}

void TurretEnemy::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
	if (m_isHit == true)
	{
		//Only do this once, so you can keep disabled turret on screen and not have any active pool items
		if (m_disableOnce == false)
		{
			m_pActiveBullets.empty();
		}

		return;
	}

	if (m_isHit == false)
	{
		m_BulletTimer += deltaTime;
		//Check for player in range
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

			//Calculate Aim Pos
			m_AimPos = (p_Player->GetPosition() - m_Position) / dist ;

			//start firing
			//Check timer
			if (m_BulletTimer >= BULLET_TIMER)
			{
				//grab from pool
				GameObject* pGameObject = m_pTurretBullets.GetObjectFromPool();
				vec3 offset(0, 0.5f, 0);
				if (pGameObject)
				{
					pGameObject->AddToScene(this->m_pScene);
					pGameObject->SetPosition(m_Position-offset);
					pGameObject->GetPhysicsBody()->SetActive(true);

					pGameObject->GetPhysicsBody()->SetLinearVelocity(b2Vec2(m_AimPos.x * BULLET_SPEED, m_AimPos.y * BULLET_SPEED));

					m_BulletTimer = 0;

					m_pActiveBullets.push_back(pGameObject);
				}
			}

			//Loop through bullets, determine how far away they are, and reset them if they are too far
			for (int i = 0; i < m_pActiveBullets.size(); i++)
			{
				vec3 bulletPos= m_pActiveBullets.at(i)->GetPosition();
				float fallOffRad = 20.0f;

				//Circle to circle so you can check both directions
				if (sqrtf((bulletPos.x - m_Position.x)* (bulletPos.x - m_Position.x) + (bulletPos.y - m_Position.y) * (bulletPos.y - m_Position.y)) > fallOffRad + 0.5 / 2)
				{
					GameObject* bullet = m_pActiveBullets.at(i);
					bullet->GetPhysicsBody()->SetLinearVelocity(b2Vec2_zero);
					bullet->GetPhysicsBody()->SetActive(false);

					m_pTurretBullets.ReturnObject(bullet);

					//erase at index
					m_pActiveBullets.erase(m_pActiveBullets.begin() + i);
				}
			}
		}
	}
}

void TurretEnemy::BeginContact(b2Fixture * pOtherFixture, vec2 worldPosition, vec2 worldNormal)
{
	if (pOtherFixture->GetFilterData().categoryBits == CATEGORY_PBULLET)
	{
		m_isHit = true;
		m_disableOnce = true;
	}
}

void TurretEnemy::EndContact(b2Fixture * pOtherFixture)
{

}
