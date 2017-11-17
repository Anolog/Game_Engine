#include "pch.h"

Mesh::Mesh()
{
    m_VBO = 0;
    m_IBO = 0;
    m_PrimitiveType = GL_TRIANGLES;
}

Mesh::~Mesh()
{
    glDeleteBuffers( 1, &m_VBO );
    glDeleteBuffers( 1, &m_IBO );
}

void Mesh::Init(VertexFormat* verts, int numverts, unsigned int* indices, int numindices, GLenum usage, GLenum primitivetype)
{
    m_NumVerts = numverts;
    m_NumIndices = numindices;

    // gen and fill buffer with our vertex attributes.
    if( m_VBO == 0 )
        glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat)*numverts, verts, usage );

    // gen and fill buffer with our indices.
    if( m_IBO == 0 )
        glGenBuffers( 1, &m_IBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numindices, indices, usage );

    m_PrimitiveType = primitivetype;


    CheckForGLErrors();
}

void Mesh::Init(const void* verts, int numverts, int vertexbytesize, GLenum usage, GLenum primitivetype)
{
    m_NumVerts = numverts;
    m_NumIndices = 0;

    // gen and fill buffer with our vertex attributes.
    if( m_VBO == 0 )
        glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, numverts * vertexbytesize, verts, usage );

    m_PrimitiveType = primitivetype;

    CheckForGLErrors();
}

void Mesh::SetupAttributes(ShaderProgram* pShaderProgram)
{
    assert( m_VBO != 0 );

    // bind our vertex and index buffers.
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    if( m_IBO != 0 )
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IBO );

    GLuint programhandle = pShaderProgram->GetProgram();

    // get the attribute locations.
    GLint aPos = glGetAttribLocation( programhandle, "a_Position" );
    GLint aColor = glGetAttribLocation( programhandle, "a_Color" );
    GLint aUV = glGetAttribLocation( programhandle, "a_UV" );
    GLint aNormal = glGetAttribLocation( programhandle, "a_Normal" );

    // setup our vbo and attributes.
    {
        // setup the position attribute.
        assert( aPos != -1 );
        glVertexAttribPointer( aPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, pos) );
        glEnableVertexAttribArray( aPos );

        // setup the color attribute.
        if( aColor != -1 )
        {
            glVertexAttribPointer( aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, color) );
            glEnableVertexAttribArray( aColor );
        }

        // setup the uv attribute.
        if( aUV != -1 )
        {
            glVertexAttribPointer( aUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, uv) );
            glEnableVertexAttribArray( aUV );
        }

        // setup the normal attribute.
        if( aNormal != -1 )
        {
            glVertexAttribPointer( aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, normal) );
            glEnableVertexAttribArray( aNormal );
        }
    }
}

void Mesh::SetupUniforms(ShaderProgram* pShaderProgram, mat4* worldmat, mat4* viewmat, mat4* projmat, vec3 campos, GLuint texturehandle, int frame, Vector4 color, Vector4 lightColor[], vec3 lightPos[])
{
    GLuint programhandle = pShaderProgram->GetProgram();

    // enable our shader program.
    glUseProgram( programhandle );

    // get the uniform locations.
    GLint uWorldMatrix = glGetUniformLocation( programhandle, "u_WorldMatrix" );
    GLint uViewMatrix = glGetUniformLocation( programhandle, "u_ViewMatrix" );
    GLint uProjMatrix = glGetUniformLocation( programhandle, "u_ProjMatrix" );
    GLint uCameraPosition = glGetUniformLocation( programhandle, "u_CameraPosition" );
    GLint uTexture = glGetUniformLocation( programhandle, "u_Texture" );
	GLint uTextureCube = glGetUniformLocation(programhandle, "u_TextureCube");
    GLint uColor = glGetUniformLocation( programhandle, "u_Color" );
    GLint uTime = glGetUniformLocation( programhandle, "u_Time" );
    GLint uUVScale = glGetUniformLocation( programhandle, "u_UVScale" );
    GLint uUVOffset = glGetUniformLocation( programhandle, "u_UVOffset" );
	
	//GLint uLightPosition = glGetUniformLocation(programhandle, "u_LightPosition");
	//GLint uLightColor = glGetUniformLocation(programhandle, "u_LightColor");

	std::string pos = "u_LightPosition[i]";
	std::string col = "u_LightColor[i]";

	//later use #DEFINE LIGHTCOUNT
	unsigned int i = 0;
	for (i; i < 3; i++)
	{
		pos.replace(16, 1, std::to_string(i));
		col.replace(13, 1, std::to_string(i));

		GLint uLightPosition = glGetUniformLocation(programhandle, pos.data());
		GLint uLightColor = glGetUniformLocation(programhandle, col.data());

		//Lights
		if (uLightPosition != -1)
		{
			glUniform3f(uLightPosition, lightPos[i].x, lightPos[i].y, lightPos[i].z);
		}

		if (uLightColor != -1)
		{
			glUniform4f(uLightColor, lightColor[i].x, lightColor[i].y, lightColor[i].z, lightColor[i].w);
		}

	}


    // setup the texture.
    if( texturehandle != 0 && uTexture != -1 )
    {
        glActiveTexture( GL_TEXTURE0 + 0 );
        glBindTexture( GL_TEXTURE_2D, texturehandle );
        glUniform1i( uTexture, 0 );
    }

	if (texturehandle != 0 && uTextureCube != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texturehandle);
		glUniform1i(uTexture, 0);
	}
    if( uWorldMatrix != -1 )
    {
        glUniformMatrix4fv( uWorldMatrix, 1, false, &worldmat->m11 );
    }

    if( uViewMatrix != -1 )
    {
        glUniformMatrix4fv( uViewMatrix, 1, false, &viewmat->m11 );
    }

    if( uProjMatrix != -1 )
    {
        glUniformMatrix4fv( uProjMatrix, 1, false, &projmat->m11 );
    }

    if( uCameraPosition != -1 )
    {
        glUniform3f( uCameraPosition, campos.x, campos.y, campos.z );
    }

    if( uColor != -1 )
    {
        glUniform4f( uColor, color.x, color.y, color.z, color.w );
    }

    // setup time.
    if( uTime != -1 )
    {
        glUniform1f( uTime, (float)MyGetSystemTimeSinceGameStart() );
    }

    // setup uvscale
    if( uUVScale != -1 )
    {
        glUniform2f( uUVScale, 64.0f/1024.0f, 1 );
    }

    // setup uvoffset
    if( uUVOffset != -1 )
    {
        glUniform2f( uUVOffset, frame * 64.0f/1024.0f, 0 );
    }

    CheckForGLErrors();
}

void Mesh::Draw(ShaderProgram* pShaderProgram)
{
    GLuint programhandle = pShaderProgram->GetProgram();

    // draw the shape.
    if( m_NumIndices > 0 )
    {
        glDrawElements( m_PrimitiveType, m_NumIndices, GL_UNSIGNED_INT, 0 );
    }
    else
    {
        glDrawArrays( m_PrimitiveType, 0, m_NumVerts );
    }

    GLint aPos = glGetAttribLocation( programhandle, "a_Position" );
    GLint aColor = glGetAttribLocation( programhandle, "a_Color" );
    GLint aUV = glGetAttribLocation( programhandle, "a_UV" );
    GLint aNormal = glGetAttribLocation( programhandle, "a_Normal" );

    // disable the attribute arrays used
    glDisableVertexAttribArray( aPos );

    if( aColor != -1 )
        glDisableVertexAttribArray( aColor );

    if( aUV != -1 )
        glDisableVertexAttribArray( aUV );

    if( aNormal != -1 )
        glDisableVertexAttribArray( aNormal );

    CheckForGLErrors();
}
