#ifndef __Mesh_H__
#define __Mesh_H__

class Mesh
{
    GLuint m_VBO;
    GLuint m_IBO;

	unsigned int* m_oldIBO;
    unsigned int m_NumVerts;
    unsigned int m_NumIndices;

    GLenum m_PrimitiveType;

public:
    Mesh();
    virtual ~Mesh();

    virtual void Init(VertexFormat* verts, int numverts, unsigned int* indices, int numindices, GLenum usage, GLenum primitivetype = GL_TRIANGLES);
    virtual void Init(const void* verts, int numverts, int vertexbytesize, GLenum usage, GLenum primitivetype = GL_TRIANGLES);

    virtual void SetupAttributes(ShaderProgram* pShaderProgram);
    static void SetupUniforms(ShaderProgram* pShaderProgram, mat4* worldmat, mat4* viewmat, mat4* projmat, vec3 campos, GLuint texturehandle, int frame, Vector4 color, Vector4 lightColor[], vec3 lightPos[]);
    virtual void Draw(ShaderProgram* pShaderProgram);

    GLuint GetVBO() { return m_VBO; }

    // defined in MeshShapes.cpp
    static Mesh* CreateBox(vec2 size, bool flipuvs, Mesh* pOldMeshPtr = 0);
	static Mesh * SkyCube(vec3 size, vec2 UVScale, bool invertFaces, Mesh * pOldMeshPtr=0);
    static Mesh* CreateCube(vec3 size, Mesh* pOldMeshPtr = 0);
    static Mesh* CreatePlane(vec3 topleftpos, vec2 size, ivec2 vertcount, vec2 uvstart, vec2 uvrange, Mesh* pOldMeshPtr = 0);
	static Mesh * CreateSphere(float radius, unsigned int sides, vec2 UVScale, bool invertFaces, Mesh* pOldMeshPtr = 0);
	static Mesh* Mesh::ImportOBJMesh(const char* fileDirectory);
	void ScaleUVCOORDS(VertexFormat verts[], int numVerts, vec2 UVScale);
	void CalculateUVCoords(std::vector<VertexFormat>& verts);
	void CalculateUVCoords(VertexFormat vertArray[], int numVertices);
	void CalculateXZUVCoords(VertexFormat vertArray[], int numVertices);
	void InvertFaces(unsigned int oldIBO[], unsigned int arraySize);
};

#endif //__Game_H__
