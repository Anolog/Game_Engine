#ifndef __PlayerBullet_H__
#define __PlayerBullet_H__

class PlayerBullet : public GameObject
{
protected:

	//SimplePool<GameObject>* m_pPool;
	float fallOffRad;
public:
	PlayerBullet(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename);
    virtual ~PlayerBullet();

    virtual void Update(float deltatime);
	//void SetPool(SimplePool<GameObject>* pPool) { m_pPool = pPool; }

    virtual void BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal);
    virtual void EndContact(b2Fixture* pOtherFixture);
};

#endif //__PlayerObject_H__
