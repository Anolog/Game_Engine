#include "pch.h"

const float BULLET_TIMER = 0.5f;
const float FRAME_RESET = 0.6f;
const float SOUND_RESET = 0.3f;
const float DEATH_TIMER = 1.0f;

PlayerObject::PlayerObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename)
	: GameObject(pScene, name, pos, rot, scale, meshname, shadername, texturename)
	, m_PlayerBulletPool()
{
	m_pController = 0;
	m_bulletTimerCounter = 0;
	m_WalkSoundTimer = 0;
	m_AnimTimer = 0;
	m_DeathTimer = 0;
	m_CanJump = false;
	m_IsAlive = true;
	m_BulletAimPos = 1;

	m_pSoundDude = new SoundManager();

	//Make the bullet, using object pooling
	for (int i = 0; i < 100; i++)
	{
		std::string PBName = "PlayerBullet" + std::to_string(i);
		vec3 pos = vec3(0, 0, 0);

		PlayerBullet* pPlayerBullet = new PlayerBullet(m_pScene, PBName, m_Position, vec3(0, 0, 0), vec3(0.5 / 2, 0.5 / 2, 0.5 / 2), "Sprite", "Texture", "Bullet");
		pPlayerBullet->SetRenderOrder(3);
		//pPlayerBullet->SetPool(&m_PlayerBulletPool);
		pPlayerBullet->AddPhysicsCircle(0.5 / 2, true, false, 0.5f, CATEGORY_PBULLET, MASK_PBULLET); // may need to make a mask that collides with enemies
		m_PlayerBulletPool.AddNewObjectToPool(pPlayerBullet);

	}

}

PlayerObject::~PlayerObject()
{
	if (m_pSoundDude != nullptr)
	{
		delete m_pSoundDude;
		m_pSoundDude = nullptr;
	}
}

void PlayerObject::SetController(PlayerController* pController)
{
	m_pController = pController;
}

void PlayerObject::Update(float deltatime)
{
	GameObject::Update(deltatime);
	if (m_IsAlive == true) //this may break bullet stuff if we wrap this all around the update
	{
		m_bulletTimerCounter += deltatime;
		m_WalkSoundTimer += (deltatime * (m_pPhysicsBody->GetLinearVelocity().x * 0.5f));
		m_AnimTimer += (deltatime * (m_pPhysicsBody->GetLinearVelocity().x * 0.5f));
		if (m_pController == 0)
			return;

		vec3 dir(0, 0, 0);

		//if( m_pController->IsButtonPressed( PCB_Up ) )
		//    dir.y += 1;
		//if( m_pController->IsButtonPressed( PCB_Down ) )
		//    dir.y -= 1;


		if (m_pController->IsButtonPressed(PCB_Left))
		{
			dir.x -= 1;
			m_BulletAimPos = -1;

			//play walk sounds
			if (m_WalkSoundTimer >= SOUND_RESET && m_CanJump == true)
			{
				m_pSoundDude->playSound(Sound_Effect_Walk);
				m_WalkSoundTimer = 0;
			}
			if (m_WalkSoundTimer <= -SOUND_RESET && m_CanJump == true)
			{
				m_pSoundDude->playSound(Sound_Effect_Walk);
				m_WalkSoundTimer = 0;
			}
		}


		if (m_pController->IsButtonPressed(PCB_Right))
		{
			dir.x += 1;
			m_BulletAimPos = 1;

			//play walk sounds
			if (m_WalkSoundTimer >= SOUND_RESET && m_CanJump == true)
			{
				m_pSoundDude->playSound(Sound_Effect_Walk);
				m_WalkSoundTimer = 0;
			}
			if (m_WalkSoundTimer <= -SOUND_RESET && m_CanJump == true)
			{
				m_pSoundDude->playSound(Sound_Effect_Walk);
				m_WalkSoundTimer = 0;
			}
		}

		dir.Normalize();

		float speed = 10;

		dir *= speed;



		//AnimFrames
		{

			if (dir.x < 0) //if facing left
			{
				SetMesh(m_pScene->GetResourceManager()->GetMesh("SpriteReversed"));
			}
			if (dir.x > 0)
			{
				SetMesh(m_pScene->GetResourceManager()->GetMesh("Sprite"));
			}
			//if idle
			if (m_pPhysicsBody->GetLinearVelocity().x == 0 && m_CanJump)
				SetFrame(0);

			//if moving on the ground
			if (m_pPhysicsBody->GetLinearVelocity().x != 0 && m_CanJump)
			{
				if (m_AnimTimer <= FRAME_RESET && m_AnimTimer >= -FRAME_RESET)
					SetFrame(1);
				if (m_AnimTimer <= FRAME_RESET / 3 * 2 && m_AnimTimer >= -FRAME_RESET / 3 * 2)
					SetFrame(2);
				if (m_AnimTimer <= FRAME_RESET / 3 && m_AnimTimer >= -FRAME_RESET / 3)
					SetFrame(3);
			}
			if (m_pPhysicsBody->GetLinearVelocity().y > 0 && m_CanJump == false)
				SetFrame(5);
			if (m_pPhysicsBody->GetLinearVelocity().y < 0 && m_CanJump == false)
				SetFrame(4);

			if (m_AnimTimer >= FRAME_RESET)
				m_AnimTimer = 0;
			if (m_AnimTimer <= -FRAME_RESET)
				m_AnimTimer = 0;
		}

		//smoother player movement
		m_pPhysicsBody->ApplyForce(b2Vec2(dir.x / 2, dir.y / 2), m_pPhysicsBody->GetWorldCenter(), true);
		m_pPhysicsBody->ApplyTorque(10, true);
		m_pPhysicsBody->SetLinearDamping(1);


		//When space is pressed, shoot a bullet
		if (m_pController->IsButtonPressed(PCB_Space))
		{

			//Check timer
			if (m_bulletTimerCounter >= BULLET_TIMER)
			{
				m_pSoundDude->playSound(Sound_Effect_Shoot);
				SetFrame(6);
				if (m_pPhysicsBody->GetLinearVelocity().y != 0)
					SetFrame(7);

				//grab from pool
				GameObject* pGameObject = m_PlayerBulletPool.GetObjectFromPool();

				if (pGameObject)
				{
					//Set force, and add to world, activate, put into the vector of active ones
					b2Vec2 gunForce = b2Vec2(m_BulletAimPos * 10.0f, 0.0f);
					pGameObject->AddToScene(this->m_pScene);
					pGameObject->SetPosition(m_Position);
					pGameObject->GetPhysicsBody()->SetActive(true);
					pGameObject->GetPhysicsBody()->ApplyLinearImpulse(gunForce, pGameObject->GetPhysicsBody()->GetWorldCenter(), false);

					m_bulletTimerCounter = 0;

					m_pActiveBullets.push_back(pGameObject);



				}
			}

			//Loop through bullets, determine how far away they are, and reset them if they are too far
			for (int i = 0; i < m_pActiveBullets.size(); i++)
			{
				//bullet pos
				vec3 bulletPos = m_pActiveBullets.at(i)->GetPosition();
				float fallOffRad = 10.0f;
				SetFrame(6);
				if (m_pPhysicsBody->GetLinearVelocity().y != 0)
					SetFrame(7);
				//Circle to circle so you can check both directions from player
				if (sqrtf((bulletPos.x - m_Position.x)* (bulletPos.x - m_Position.x) + (bulletPos.y - m_Position.y) * (bulletPos.y - m_Position.y)) > fallOffRad + 0.5 / 2)
				{
					GameObject* bullet = m_pActiveBullets.at(i);
					bullet->GetPhysicsBody()->SetLinearVelocity(b2Vec2_zero);
					bullet->GetPhysicsBody()->SetActive(false);

					m_PlayerBulletPool.ReturnObject(bullet);

					//erase at index
					m_pActiveBullets.erase(m_pActiveBullets.begin() + i);
				}
			}

		}

		if (m_CanJump)
		{
			if (m_pController->IsButtonPressed(PCB_Up))
			{
				m_pPhysicsBody->ApplyLinearImpulse(b2Vec2(0, 8), m_pPhysicsBody->GetWorldCenter(), true);
				m_CanJump = false;
				m_pSoundDude->playSound(Sound_Effect_Jump);
			}
		}

		if (g_KeyStates['K'])
		{
			m_pScene->GetGameCore()->SwitchScene("Pause");
		}
	}


	if (m_IsAlive == false)
	{
		//death anim
		m_DeathTimer += (deltatime);
		if (m_DeathTimer < DEATH_TIMER)
			SetFrame(8);
		if (m_DeathTimer <= DEATH_TIMER * 2 / 3)
			SetFrame(9);
		if (m_DeathTimer <= DEATH_TIMER / 3)
			SetFrame(10);
		if (m_DeathTimer >= DEATH_TIMER)
		{
			//m_IsAlive = true; // for testingg
			//m_DeathTimer = 0; //for testing
			//start game over scene

			m_pScene->GetGameCore()->SwitchScene("GameOver");
			
		}
	}

}

void PlayerObject::BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal)
{
	GameObject::BeginContact(pOtherFixture, worldPosition, worldNormal);

	if (worldNormal.y < 0.0f && pOtherFixture->GetFilterData().categoryBits == CATEGORY_GROUND)
	{
		m_CanJump = true;
		m_pSoundDude->playSound(Sound_Effect_Land);
	}

	if (pOtherFixture->GetFilterData().categoryBits == CATEGORY_EBULLET || pOtherFixture->GetFilterData().categoryBits == CATEGORY_ENEMY)
	{
		m_IsAlive = false;
	}
}

void PlayerObject::EndContact(b2Fixture* pOtherFixture)
{
	GameObject::EndContact(pOtherFixture);
}
