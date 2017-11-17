#include "pch.h"

#include "OBJLoader.h"
#include "Mesh.h"

Mesh* Mesh::CreateBox(vec2 size, bool flipuvs, Mesh* pOldMeshPtr)
{
    VertexFormat verts[4];

    verts[0] = VertexFormat( -size.x/2, -size.y/2, 0, 255,255,255,255, 0,0, 0,0,0 );
    verts[1] = VertexFormat( -size.x/2,  size.y/2, 0, 255,255,255,255, 0,1, 0,0,0 );
    verts[2] = VertexFormat(  size.x/2,  size.y/2, 0, 255,255,255,255, 1,1, 0,0,0 );
    verts[3] = VertexFormat(  size.x/2, -size.y/2, 0, 255,255,255,255, 1,0, 0,0,0 );

    if( flipuvs )
    {
        verts[0].uv.x = 1;
        verts[1].uv.x = 1;
        verts[2].uv.x = 0;
        verts[3].uv.x = 0;
    }

    unsigned int indices[6] = { 0,1,2, 0,2,3 };

    Mesh* pMesh = pOldMeshPtr;
    if( pMesh == 0 )
        pMesh = new Mesh();
    pMesh->Init( verts, 4, indices, 6, GL_STATIC_DRAW, GL_TRIANGLES );

    return pMesh;
};

Mesh* Mesh::SkyCube(vec3 size, vec2 UVScale, bool invertFaces, Mesh* pOldMeshPtr)
{
	// image
	int imageWidth = 600; // pixels
	int imageHeight = 100; // 0.1666666666666667
	int numOfXFrames = 6;
	int numOfYFrames = 1;

	float frameWidth = (float)(imageWidth / numOfXFrames);
	float frameHeight = (float)(imageHeight / numOfYFrames);

	float UVOffsetX = frameWidth / imageWidth;
	float UVOffsetY = imageHeight / frameHeight;

	// mesh
	VertexFormat verts[24];

	// cube with 6 sides, 2 triangles per side
	// face 1 - front
	verts[0] = VertexFormat(-size.x / 2, size.y / 2, -size.z / 2, 255, 255, 255, 255, 0, 1, 0, 0, 0); //top-left
	verts[1] = VertexFormat(-size.x / 2, -size.y / 2, -size.z / 2, 255, 255, 255, 255, 0, 0, 0, 0, 0); // bottom-left
	verts[2] = VertexFormat(size.x / 2, -size.y / 2, -size.z / 2, 255, 255, 255, 255, UVOffsetX, 0, 0, 0, 0); // bottom-right
	verts[3] = VertexFormat(size.x / 2, size.y / 2, -size.z / 2, 255, 255, 255, 255, UVOffsetX, 1, 0, 0, 0); // top-right

																											 // face 2 - left
	verts[4] = VertexFormat(-size.x / 2, size.y / 2, -size.z / 2, 255, 255, 255, 255, UVOffsetX, 1, 0, 0, 0);
	verts[5] = VertexFormat(-size.x / 2, -size.y / 2, -size.z / 2, 255, 255, 255, 255, UVOffsetX, 0, 0, 0, 0);
	verts[6] = VertexFormat(-size.x / 2, -size.y / 2, size.z / 2, 255, 255, 255, 255, UVOffsetX * 2, 0, 0, 0, 0);
	verts[7] = VertexFormat(-size.x / 2, size.y / 2, size.z / 2, 255, 255, 255, 255, UVOffsetX * 2, 1, 0, 0, 0);

	// face 3 - back
	verts[8] = VertexFormat(size.x / 2, size.y / 2, size.z / 2, 255, 255, 255, 255, UVOffsetX * 2, 1, 0, 0, 0);
	verts[9] = VertexFormat(size.x / 2, -size.y / 2, size.z / 2, 255, 255, 255, 255, UVOffsetX * 2, 0, 0, 0, 0);
	verts[10] = VertexFormat(-size.x / 2, -size.y / 2, size.z / 2, 255, 255, 255, 255, UVOffsetX * 3, 0, 0, 0, 0);
	verts[11] = VertexFormat(-size.x / 2, size.y / 2, size.z / 2, 255, 255, 255, 255, UVOffsetX * 3, 1, 0, 0, 0);

	// face 4 - right
	verts[12] = VertexFormat(size.x / 2, size.y / 2, -size.z / 2, 255, 255, 255, 255, UVOffsetX * 3, 1, 0, 0, 0);
	verts[13] = VertexFormat(size.x / 2, -size.y / 2, -size.z / 2, 255, 255, 255, 255, UVOffsetX * 3, 0, 0, 0, 0);
	verts[14] = VertexFormat(size.x / 2, -size.y / 2, size.z / 2, 255, 255, 255, 255, UVOffsetX * 4, 0, 0, 0, 0);
	verts[15] = VertexFormat(size.x / 2, size.y / 2, size.z / 2, 255, 255, 255, 255, UVOffsetX * 4, 1, 0, 0, 0);

	// face 5 - top - z axis becomes vertical + reference
	verts[16] = VertexFormat(-size.x / 2, size.y / 2, size.z / 2, 255, 255, 255, 255, UVOffsetX * 4, 1, 0, 0, 0);
	verts[17] = VertexFormat(-size.x / 2, size.y / 2, -size.z / 2, 255, 255, 255, 255, UVOffsetX * 4, 0, 0, 0, 0);
	verts[18] = VertexFormat(size.x / 2, size.y / 2, -size.z / 2, 255, 255, 255, 255, UVOffsetX * 5, 0, 0, 0, 0);
	verts[19] = VertexFormat(size.x / 2, size.y / 2, size.z / 2, 255, 255, 255, 255, UVOffsetX * 5, 1, 0, 0, 0);

	// face 6 - bottom
	verts[20] = VertexFormat(-size.x / 2, -size.y / 2, size.z / 2, 255, 255, 255, 255, UVOffsetX * 5, 1, 0, 0, 0);
	verts[21] = VertexFormat(-size.x / 2, -size.y / 2, -size.z / 2, 255, 255, 255, 255, UVOffsetX * 5, 0, 0, 0, 0);
	verts[22] = VertexFormat(size.x / 2, -size.y / 2, -size.z / 2, 255, 255, 255, 255, UVOffsetX * 6, 0, 0, 0, 0);
	verts[23] = VertexFormat(size.x / 2, -size.y / 2, size.z / 2, 255, 255, 255, 255, UVOffsetX * 6, 1, 0, 0, 0);

	unsigned int indices[36] = {
		0,3,2, 2,1,0, // front - brown
		6,7,4, 4,5,6, // left - pink
		10,9,8, 8,11,10, // back - blue
		12,15,14, 14,13,12, // right - yellow
		16,19,18, 18,17,16, // top - red
		22,23,20, 20,21,22 // bottom - green
	};

	Mesh* pMesh = pOldMeshPtr;
	if (pMesh == 0)
		pMesh = new Mesh();
	// scale UVCOORDS
	if (UVScale != vec2(1.0f, 1.0f))
	{
		pMesh->ScaleUVCOORDS(verts, 24, UVScale);
	}

	if (invertFaces) // invert faces
	{
		pMesh->InvertFaces(indices, 36);
	}

	pMesh->Init(verts, 24, indices, 36, GL_STATIC_DRAW);
	pMesh->m_PrimitiveType = GL_TRIANGLES;

	return pMesh;
}

Mesh* Mesh::CreateCube(vec3 size, Mesh* pOldMeshPtr)
{
    VertexFormat verts[8];
    //   5--6
    // 1--2 |
    // |  |-7
    // 0--3

    verts[0] = VertexFormat( -size.x/2, -size.y/2, -size.z/2, 255,255,255,255, 0,0, 0,0,0 );
    verts[1] = VertexFormat( -size.x/2,  size.y/2, -size.z/2, 255,255,255,255, 0,1, 0,0,0 );
    verts[2] = VertexFormat(  size.x/2,  size.y/2, -size.z/2, 255,255,255,255, 1,1, 0,0,0 );
    verts[3] = VertexFormat(  size.x/2, -size.y/2, -size.z/2, 255,255,255,255, 1,0, 0,0,0 );
    verts[4] = VertexFormat( -size.x/2, -size.y/2,  size.z/2, 255,255,255,255, 0,0, 0,0,0 );
    verts[5] = VertexFormat( -size.x/2,  size.y/2,  size.z/2, 255,255,255,255, 0,1, 0,0,0 );
    verts[6] = VertexFormat(  size.x/2,  size.y/2,  size.z/2, 255,255,255,255, 1,1, 0,0,0 );
    verts[7] = VertexFormat(  size.x/2, -size.y/2,  size.z/2, 255,255,255,255, 1,0, 0,0,0 );

    unsigned int indices[36] =
    {
        0,1,2, 0,2,3,
        4,6,5, 4,7,6,
        0,5,1, 0,4,5,
        3,2,6, 3,6,7,
        1,5,6, 1,6,2,
        0,7,4, 0,3,7,
    };

    Mesh* pMesh = pOldMeshPtr;
    if( pMesh == 0 )
        pMesh = new Mesh();
    pMesh->Init( verts, 8, indices, 36, GL_STATIC_DRAW );

    return pMesh;
};

Mesh* Mesh::CreatePlane(vec3 topleftpos, vec2 size, ivec2 vertcount, vec2 uvstart, vec2 uvrange, Mesh* pOldMeshPtr)
{
    int numverts = vertcount.x * vertcount.y;
    int numindices = (vertcount.x - 1) * (vertcount.y - 1) * 6;

    VertexFormat* verts = new VertexFormat[numverts];
    unsigned int* indices = new unsigned int[numindices];

    for( int y = 0; y < vertcount.y; y++ )
    {
        for( int x = 0; x < vertcount.x; x++ )
        {
            verts[y * vertcount.x + x].pos.x = topleftpos.x + x * size.x / (vertcount.x - 1);
            verts[y * vertcount.x + x].pos.y = topleftpos.y;
            verts[y * vertcount.x + x].pos.z = topleftpos.z + y * size.y / (vertcount.y - 1);

            verts[y * vertcount.x + x].uv.x = uvstart.x + x * uvrange.x / (vertcount.x - 1);
            verts[y * vertcount.x + x].uv.y = uvstart.y + y * uvrange.y / (vertcount.y - 1);
        }
    }

    for( int y = 0; y < vertcount.y - 1; y++ )
    {
        for( int x = 0; x < vertcount.x - 1; x++ )
        {
            indices[ (y * (vertcount.x-1) + x) * 6 + 0 ] = (y * vertcount.x + x) + 0;
            indices[ (y * (vertcount.x-1) + x) * 6 + 1 ] = (y * vertcount.x + x) + vertcount.x;
            indices[ (y * (vertcount.x-1) + x) * 6 + 2 ] = (y * vertcount.x + x) + 1;

            indices[ (y * (vertcount.x-1) + x) * 6 + 3 ] = (y * vertcount.x + x) + 1;
            indices[ (y * (vertcount.x-1) + x) * 6 + 4 ] = (y * vertcount.x + x) + vertcount.x;
            indices[ (y * (vertcount.x-1) + x) * 6 + 5 ] = (y * vertcount.x + x) + vertcount.x + 1;
        }
    }

    Mesh* pMesh = pOldMeshPtr;
    if( pMesh == 0 )
        pMesh = new Mesh();
    pMesh->Init( verts, numverts, indices, numindices, GL_STATIC_DRAW );

    delete[] verts;
    delete[] indices;

    return pMesh;
};

Mesh* Mesh::CreateSphere(float radius, unsigned int sides, vec2 UVScale, bool invertFaces, Mesh* pOldMeshPtr)
{
	std::vector<VertexFormat> verts;
	float t_AngleBetweenPoints = 360.0f / sides; // degrees

	int currentVertex = 0;
	float yValue = -radius;

	float tempRadius = 0.0;

	vec3 Position = vec3(0, 0, 0);

	for (unsigned int i = 0; i < sides + 1 + 3; i++)
	{
		float t_IncrementAngle = 0.0f; // reset angle for next pass

		if (i > 58)
		{
			int bp = 1;
		}
		for (unsigned int j = 0; j < sides + 1; j++) // plot circle
		{
			Position.x = cosf(t_IncrementAngle / 180 * PI) * radius * sqrt(1 - yValue*yValue);// sinf(tempRadius / 180 * PI); // sqrt(1-x*x)
			Position.y = yValue;
			Position.z = sinf(t_IncrementAngle / 180 * PI) * radius * sqrt(1 - yValue*yValue); // sinf(tempRadius / 180 * PI); // pow(cos(PI * tempRadius / 2.0), 0.5);

			verts.push_back(VertexFormat(Position.x, Position.y, Position.z, 255, 255, 255, 255, 0, 1, 0, 0, 0));

			t_IncrementAngle += t_AngleBetweenPoints;
			currentVertex++;
		}

		if (i == 60)
		{
			int bp = 7;
		}

		// increment y value for next pass
		yValue += radius * 2.0f / (sides + 2); // diameter / 
											   // increment circle radius for next pass
		tempRadius += 180.0f / sides;
	}

	// generate indices
	std::vector<unsigned int> indices;
	float numVerts = ((sides + 1) * (sides + 2));
	currentVertex = 0;

	for (int i = 0; i < numVerts; i++)
	{
		for (int i = 0; i < sides; i++)
		{
			// triangle 1 - top-right - clockwise
			indices.push_back(currentVertex + sides);
			indices.push_back(currentVertex + sides + 1);
			indices.push_back(currentVertex + 1);
			// triangle 2 - bottom-left - clockwise
			indices.push_back(currentVertex + 1);
			indices.push_back(currentVertex);
			indices.push_back(currentVertex + sides);
		}
		currentVertex++;
	}

	Mesh* pMesh = pOldMeshPtr;
	if (pMesh == 0)
		pMesh = new Mesh();
	// generate UVCOORDS
	pMesh->CalculateUVCoords(verts); // default scale 1,1
									 // scale UVCOORDS
	if (UVScale != vec2(1.0f, 1.0f))
	{
		pMesh->ScaleUVCOORDS(verts.data(), verts.size(), UVScale);
	}
	// invert faces
	if (invertFaces)
	{
		pMesh->InvertFaces(indices.data(), indices.size());
	}

	// initialize mesh
	pMesh->Init(verts.data(), verts.size(), indices.data(), indices.size(), GL_STATIC_DRAW); // create and fill buffers
	pMesh->m_PrimitiveType = GL_TRIANGLES; // draw mode
										   //pMesh->m_PrimitiveType = GL_POINTS;
										   //glPointSize(10);
										   //glPolygonMode(GL_FRONT, GL_LINE);

	return pMesh;
}