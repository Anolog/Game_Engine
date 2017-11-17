#include "pch.h"
#include "Mesh.h"

void Mesh::ScaleUVCOORDS(VertexFormat verts[], int numVerts, vec2 UVScale)
{
	for (int i = 0; i < numVerts; i++)
	{
		verts[i].uv.x * UVScale.x;
		verts[i].uv.y * UVScale.y;
	}

	vec2 NewScale;
	NewScale.x = 1.0f / UVScale.x;
	NewScale.y = 1.0f / UVScale.y;

	for (int i = 0; i < numVerts; i++)
	{
		verts[i].uv.x = verts[i].uv.x / NewScale.x;
		verts[i].uv.y = verts[i].uv.y / NewScale.y;
	}
}

void Mesh::CalculateUVCoords(std::vector<VertexFormat> &verts) // fill UV coordinates of each vertex value based on its current position in object space
{
	// initialize temporary storage values to a valid vertex position to compare with
	VertexFormat t_InitializeValue = verts.at(0);
	// x
	float t_LeftX = t_InitializeValue.pos.x;
	float t_RightX = t_InitializeValue.pos.x;
	// y
	float t_BottomY = t_InitializeValue.pos.y;
	float t_TopY = t_InitializeValue.pos.y;

	// find furthest values / boundaries. These values are the least or greatest X or Y position values
	for (VertexFormat t_TempValue : verts)
	{
		if (t_TempValue.pos.x > t_RightX)
		{
			t_RightX = t_TempValue.pos.x;
		}
		if (t_TempValue.pos.x < t_LeftX)
		{
			t_LeftX = t_TempValue.pos.x;
		}
		if (t_TempValue.pos.y < t_BottomY)
		{
			t_BottomY = t_TempValue.pos.y;
		}
		if (t_TempValue.pos.y > t_TopY)
		{
			t_TopY = t_TempValue.pos.y;
		}
	}

	// store values to shift vertex to positive position values
	//x
	float t_ShiftValueX = 0.0f;
	if (t_LeftX < 0.0f)
		t_ShiftValueX = t_LeftX * -1.0f;
	else
		t_ShiftValueX = -t_LeftX;
	//y
	float t_ShiftValueY = 0.0f;
	if (t_BottomY < 0.0f)
		t_ShiftValueY = t_BottomY * -1.0f;
	else
		t_ShiftValueY = -t_BottomY;

	// assign position to UV, using t_ShiftedValue to ensure positive values
	for (int i = 0; i < verts.size(); i++)
	{
		verts.at(i).uv.x = verts.at(i).pos.x + t_ShiftValueX;
		verts.at(i).uv.y = verts.at(i).pos.y + t_ShiftValueY;
	}

	// calculate distance between boundaries in object space
	float t_DistanceX = t_RightX - t_LeftX;
	float t_DistanceY = t_TopY - t_BottomY;

	// normalize UV points to 0 - 1 for UV coord space using the distance between boundaries
	for (int i = 0; i < verts.size(); i++)
	{
		if (t_DistanceX != 0.0f && t_DistanceY != 0.0f) // check to make sure we are not dividing by 0
		{
			verts.at(i).uv.x /= t_DistanceX;
			verts.at(i).uv.y /= t_DistanceY;
		}
	}
}

void Mesh::CalculateUVCoords(VertexFormat vertArray[], int numVertices) // fill UV coordinates of each vertex value based on its current position in object space
{
	// initialize temporary storage values to a valid vertice position to compare with
	std::vector<VertexFormat> verts;

	for (int i = 0; i < numVertices; i++)
	{
		verts.push_back(vertArray[i]);
	}

	VertexFormat t_InitializeValue = verts.at(0);
	// x
	float t_LeftX = t_InitializeValue.pos.x;
	float t_RightX = t_InitializeValue.pos.x;
	// y
	float t_BottomY = t_InitializeValue.pos.y;
	float t_TopY = t_InitializeValue.pos.y;

	// find furthest values / boundaries. These values are the least or greatest X or Y position values
	for (VertexFormat t_TempValue : verts)
	{
		if (t_TempValue.pos.x > t_RightX)
		{
			t_RightX = t_TempValue.pos.x;
		}
		if (t_TempValue.pos.x < t_LeftX)
		{
			t_LeftX = t_TempValue.pos.x;
		}
		if (t_TempValue.pos.y < t_BottomY)
		{
			t_BottomY = t_TempValue.pos.y;
		}
		if (t_TempValue.pos.y > t_TopY)
		{
			t_TopY = t_TempValue.pos.y;
		}
	}

	// store values to shift vertex to positive position values
	//x
	float t_ShiftValueX = 0.0f;
	if (t_LeftX < 0.0f)
		t_ShiftValueX = t_LeftX * -1.0f;
	else
		t_ShiftValueX = -t_LeftX;
	//y
	float t_ShiftValueY = 0.0f;
	if (t_BottomY < 0.0f)
		t_ShiftValueY = t_BottomY * -1.0f;
	else
		t_ShiftValueY = -t_BottomY;

	// assign position to UV, using t_ShiftedValue to ensure positive values
	for (int i = 0; i < verts.size(); i++)
	{
		verts.at(i).uv.x = verts.at(i).pos.x + t_ShiftValueX;
		verts.at(i).uv.y = verts.at(i).pos.y + t_ShiftValueY;
	}

	// calculate distance between boundaries in object space
	float t_DistanceX = t_RightX - t_LeftX;
	float t_DistanceY = t_TopY - t_BottomY;

	// normalize UV points to 0 - 1 for UV coord space using the distance between boundaries
	for (int i = 0; i < verts.size(); i++)
	{
		if (t_DistanceX != 0.0f && t_DistanceY != 0.0f) // check to make sure we are not dividing by 0
		{
			verts.at(i).uv.x /= t_DistanceX;
			verts.at(i).uv.y /= t_DistanceY;
		}
	}

	// transfer uv coords back to array

	for (int i = 0; i < numVertices; i++)
	{
		vertArray[i].uv = verts.at(i).uv;
	}
}

void Mesh::CalculateXZUVCoords(VertexFormat vertArray[], int numVertices) // fill UV coordinates of each vertex value based on its current position in object space
{
	// initialize temporary storage values to a valid vertice position to compare with
	std::vector<VertexFormat> verts;

	for (int i = 0; i < numVertices; i++)
	{
		verts.push_back(vertArray[i]);
	}

	VertexFormat t_InitializeValue = verts.at(0);
	// x
	float t_LeftX = t_InitializeValue.pos.x;
	float t_RightX = t_InitializeValue.pos.x;
	// y
	float t_BottomZ = t_InitializeValue.pos.z;
	float t_TopZ = t_InitializeValue.pos.z;

	// find furthest values / boundaries. These values are the least or greatest X or Y position values
	for (VertexFormat t_TempValue : verts)
	{
		if (t_TempValue.pos.x > t_RightX)
		{
			t_RightX = t_TempValue.pos.x;
		}
		if (t_TempValue.pos.x < t_LeftX)
		{
			t_LeftX = t_TempValue.pos.x;
		}
		if (t_TempValue.pos.y < t_BottomZ)
		{
			t_BottomZ = t_TempValue.pos.z;
		}
		if (t_TempValue.pos.z > t_TopZ)
		{
			t_TopZ = t_TempValue.pos.z;
		}
	}

	// store values to shift vertex to positive position values
	//x
	float t_ShiftValueX = 0.0f;
	if (t_LeftX < 0.0f)
		t_ShiftValueX = t_LeftX * -1.0f;
	else
		t_ShiftValueX = -t_LeftX;
	//y
	float t_ShiftValueZ = 0.0f;
	if (t_BottomZ < 0.0f)
		t_ShiftValueZ = t_BottomZ * -1.0f;
	else
		t_ShiftValueZ = -t_BottomZ;

	// assign position to UV, using t_ShiftedValue to ensure positive values
	for (int i = 0; i < verts.size(); i++)
	{
		verts.at(i).uv.x = verts.at(i).pos.x + t_ShiftValueX;
		verts.at(i).uv.y = verts.at(i).pos.z + t_ShiftValueZ;
	}

	// calculate distance between boundaries in object space
	float t_DistanceX = t_RightX - t_LeftX;
	float t_DistanceZ = t_TopZ - t_BottomZ;

	// normalize UV points to 0 - 1 for UV coord space using the distance between boundaries
	for (int i = 0; i < verts.size(); i++)
	{
		if (t_DistanceX != 0.0f && t_DistanceZ != 0.0f) // check to make sure we are not dividing by 0
		{
			verts.at(i).uv.x /= t_DistanceX;
			verts.at(i).uv.y /= t_DistanceZ;
		}
	}

	// transfer uv coords back to array

	for (int i = 0; i < numVertices; i++)
	{
		vertArray[i].uv = verts.at(i).uv;
	}
}


void Mesh::InvertFaces(unsigned int currIBO[], unsigned int arraySize)
{
	m_oldIBO = new unsigned int[arraySize];

	for (int i = 0; i < arraySize; i++)
	{
		m_oldIBO[i] = currIBO[arraySize - i - 1];
	}

	for (int i = 0; i < arraySize; i++)
	{
		currIBO[i] = m_oldIBO[i];
	}
	delete[] m_oldIBO;
}


Mesh* Mesh::ImportOBJMesh(const char* fileDirectory)
{
	// find file
	long length = 0;
	char* buffer = LoadCompleteFile(fileDirectory, &length);
	if (buffer == 0 || length == 0)
	{
		delete buffer;
		return 0;
	}
	// read and store file text
	char* next_token = 0;
	char* line = strtok_s(buffer, "\n", &next_token);

	std::vector<std::string> stringList;

	while (line)
	{
		//OutputMessage("%s\n", line);
		stringList.push_back(line);
		line = strtok_s(0, "\n", &next_token);
	}
	// parse file for data
	std::vector<VertexFormat> verts; // vertexFormat data
	std::vector<unsigned int> indices; // IBO data

	unsigned int textureCounter = 0;
	unsigned int normalCounter = 0;

	for (int i = 0; i < stringList.size(); i++) // stringList.size() = number of lines in file
	{
		std::string loopString = stringList.at(i);

		if (loopString.at(0) == 'v') // check type of info
		{
			if (loopString.at(1) == ' ')
			{
				// read vertex data, create VBO
				vec3 vertexPosition = vec3(0, 0, 0);

				sscanf_s(loopString.data(), "%*s %f %f %f", &vertexPosition.x, &vertexPosition.y, &vertexPosition.z);// tempString.data(), tempString.length(), &tempInteger);

				verts.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 255, 255, 255, 255, 0, 1, 0, 0, 0));
			}
			else if (loopString.at(1) == 't')
			{
				// uv coords
				vec2 vertexUV = vec2(0, 0);
				sscanf_s(loopString.data(), "%*s %f %f", &vertexUV.x, &vertexUV.y);
				//verts.at(textureCounter).uv = vertexUV;
				textureCounter++;
			}
			else if (loopString.at(1) == 'n')
			{
				// normal
				vec3 vertexNormal = vec3(0, 0, 0);
				sscanf_s(loopString.data(), "%*s %f %f %f", &vertexNormal.x, &vertexNormal.y, &vertexNormal.z);
				//verts.at(normalCounter).normal = vertexNormal;
				normalCounter++;
			}

		}
		else if (loopString.at(0) == 'f')
		{
			// read faces, create IBO
			int tempVariable[] = { 0,0,0 };
			sscanf_s(loopString.data(), "%*s %i %*s %i %*s %i", &tempVariable[0], &tempVariable[1], &tempVariable[2]);
			indices.push_back(tempVariable[0] - 1); // OBJ exporter thinks first index is 1
			indices.push_back(tempVariable[1] - 1);
			indices.push_back(tempVariable[2] - 1);
		}
	}
	// generate mesh
	Mesh* pMesh = new Mesh();

	// generate UVCOORDS
	pMesh->CalculateUVCoords(verts); // default scale 1,1
									 // scale UVCOORDS
	vec2 tempScale = vec2(1, 1);
	if (tempScale != vec2(1.0f, 1.0f))
	{
		pMesh->ScaleUVCOORDS(verts.data(), verts.size(), tempScale);
	}
	// invert faces
	bool invertFaces = false;
	if (invertFaces)
	{
		pMesh->InvertFaces(indices.data(), indices.size());
	}

	// initialize mesh
	pMesh->Init(verts.data(), verts.size(), indices.data(), indices.size(), GL_STATIC_DRAW); // create and fill buffers
	pMesh->m_PrimitiveType = GL_TRIANGLE_FAN;


	delete[] buffer;

	return pMesh;

}