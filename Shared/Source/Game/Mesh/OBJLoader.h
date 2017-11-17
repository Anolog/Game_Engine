#ifndef __OBJLoader_H__
#define __OBJLoader_H__

void LoadBasicOBJFromFile(const char* filename, float scale, unsigned int* pNumVerts, unsigned int* pNumIndices, VertexFormat** ppVBO, unsigned int** ppIBO);

#endif //__OBJLoader_H__
