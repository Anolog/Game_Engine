#include "pch.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    for( auto object: m_pMeshes )
        delete object.second;

    for( auto object: m_pShaders )
        delete object.second;

    for( auto object: m_pTextures )
        glDeleteTextures( 1, &object.second );
}

void ResourceManager::OnSurfaceChanged(unsigned int width, unsigned int height)
{
}
