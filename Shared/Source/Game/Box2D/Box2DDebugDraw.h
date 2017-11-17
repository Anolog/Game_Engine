#ifndef __Box2DDebugDraw_H__
#define __Box2DDebugDraw_H__

class Box2DDebugDraw : public b2Draw
{
protected:
    ShaderProgram* m_pShaderProgram;
    mat4* m_pMatView;
    mat4* m_pMatProj;

public:
    Box2DDebugDraw(ShaderProgram* pShaderProgram, mat4* pMatView, mat4* pMatProj);
    ~Box2DDebugDraw();

    virtual void Draw(const b2Vec2* vertices, int32 vertexCount, const b2Color& color, unsigned char alpha, int primitivetype);
    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color); // CCW order.
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color); // CCW order.
    virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    virtual void DrawTransform(const b2Transform& xf);

    void SetViewProjMatrices(mat4* pMatView, mat4* pMatProj);
};

#endif //__Box2DDebugDraw_H__
