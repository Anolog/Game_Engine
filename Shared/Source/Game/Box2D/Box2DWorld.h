#ifndef __Box2DWorld_H__
#define __Box2DWorld_H__

class Box2DWorld;
class Box2DDebugDraw;
class MaterialDefinition;

class Box2DWorld
{
protected:
    b2World* m_pWorld;
    Box2DDebugDraw* m_pDebugDraw;
    Box2DContactListener* m_pContactListener;

    b2Body* m_pGround;

    float m_UnusedTime;

public:
    Box2DWorld(ShaderProgram* debugdrawshader, mat4* pMatView, mat4* pMatProj);
    ~Box2DWorld();

    void CreateWorld(ShaderProgram* debugdrawshader, mat4* pMatView, mat4* pMatProj);
    void Update(float deltatime);
    void Cleanup();

    b2World* GetWorld() { return m_pWorld; }
    void SetViewProjMatrices(mat4* pMatView, mat4* pMatProj);

    b2Body* GetGroundBody() { return m_pGround; }
};

#endif //__Box2DWorld_H__
