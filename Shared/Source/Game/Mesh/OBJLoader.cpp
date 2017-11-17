#include "pch.h"

struct FaceInfo
{
    int attributes[4][3];
    int vertindex[4];
};

char*LoadFile(const char* filename, long* length)
{
    char* filecontents = 0;

    FILE* filehandle;
    errno_t error = fopen_s( &filehandle, filename, "rb" );

    if( filehandle )
    {
        fseek( filehandle, 0, SEEK_END );
        long size = ftell( filehandle );
        rewind( filehandle );

        filecontents = new char[size+1];
        fread( filecontents, size, 1, filehandle );
        filecontents[size] = 0;

        if( length )
            *length = size;

        fclose( filehandle );
    }

    return filecontents;
}

int FindIndexOfNextSpaceOrTab(char* buffer, int index)
{
    // find first non-whitespace
    while( buffer[index] != '\t' && buffer[index] != ' ' )
    {
        index++;
    }

    return index;
}

int FindIndexOfFirstNonWhitespace(char* buffer, int index)
{
    // find first non-whitespace
    while( buffer[index] == '\r' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == ' ' )
    {
        index++;
    }

    return index;
}

int FindIndexOfFirstNonSpaceOrTab(char* buffer, int index)
{
    // find first non-whitespace
    while( buffer[index] == '\t' || buffer[index] == ' ' )
    {
        index++;
    }

    return index;
}

int FindIndexOfFirstNonWhitespaceOfNextLine(char* buffer, int index)
{
    // find end of line
    while( buffer[index] != '\n' && buffer[index] != 0 )
    {
        index++;
    }

    return FindIndexOfFirstNonWhitespace( buffer, index );
}

float ReadFloatAndMoveOn(char* buffer, int* index)
{
    char numberbuffer[100];
    int numberbufferindex = 0;

    while( (buffer[*index] >= '0' && buffer[*index] <= '9') || buffer[*index] == '.' || buffer[*index] == '-' || buffer[*index] == 'e' || buffer[*index] == '+' )
    {
        numberbuffer[numberbufferindex] = buffer[*index];
        numberbuffer[numberbufferindex+1] = 0;
        numberbufferindex++;

        (*index)++;
    }

    return (float)atof( numberbuffer );
}

int ReadIntAndMoveOn(char* buffer, int* index)
{
    char numberbuffer[100];
    int numberbufferindex = 0;

    while( (buffer[*index] >= '0' && buffer[*index] <= '9') || buffer[*index] == '-' )
    {
        numberbuffer[numberbufferindex] = buffer[*index];
        numberbuffer[numberbufferindex+1] = 0;
        numberbufferindex++;

        (*index)++;
    }

    return atoi( numberbuffer );
}

vec3 ParseVertex(char* buffer, int index)
{
    assert( buffer[index] == 'v' );

    vec3 outvector(0,0,0);

    // jump to the first space after the v, vt, or vn
    index = FindIndexOfNextSpaceOrTab( buffer, index );
    index = FindIndexOfFirstNonWhitespace( buffer, index+1 );

    int count = 0;

    while( buffer[index] != '\r' && buffer[index] != '\n' && buffer[index] != 0 )
    {
        if( buffer[index] == ' ' || buffer[index] == '\t' )
        {
            count++;
            index = FindIndexOfFirstNonSpaceOrTab( buffer, index );
        }
        else
        {
            float value = ReadFloatAndMoveOn( buffer, &index );

            if( count > 2 )
                assert( value == 1 );
            else
                outvector[count] = value;
        }
    }

    return outvector;
}

FaceInfo ParseFaceInfo(char* buffer, int index)
{
    assert( buffer[index] == 'f' );

    // jump to the first number
    index = FindIndexOfFirstNonWhitespace( buffer, index+1 );

    FaceInfo faceinfo;
    memset( &faceinfo, 0, sizeof(FaceInfo) );

    int numverts = 0;
    int numattrs = 0;

    int lastnumber = -1;

    int numattribs = 0;
    while( buffer[index] != '\r' && buffer[index] != '\n' && buffer[index] != 0 )
    {
        if( buffer[index] == ' ' || buffer[index] == '\t' )
        {
            faceinfo.attributes[numverts][numattrs] = lastnumber;
            lastnumber = 0; // invalid index

            numattrs = 0;
            numverts++;
            index = FindIndexOfFirstNonSpaceOrTab( buffer, index );
        }
        else if( buffer[index] == '/' )
        {
            faceinfo.attributes[numverts][numattrs] = lastnumber;
            lastnumber = 0; // invalid index

            numattrs++;
            index++;
        }
        else
        {
            lastnumber = ReadIntAndMoveOn( buffer, &index );
        }
    }

    assert( numverts < 4 ); // only supports meshes with 3 or 4 verts in a face.

    // store the last number, before the '\n' and '\r'
    faceinfo.attributes[numverts][numattrs] = lastnumber;

    return faceinfo;
}

void LoadBasicOBJFromFile(const char* filename, float scale, unsigned int* pNumVerts, unsigned int* pNumIndices, VertexFormat** ppVBO, unsigned int** ppIBO)
{
    assert( pNumVerts != 0 );
    assert( pNumIndices != 0 );
    assert( ppVBO != 0 );
    assert( ppIBO != 0 );

    long size;
    char* buffer = LoadFile( filename, &size );

    if( buffer == 0 )
        return;

    // count the number of faces and each type of vertex attribute.
    int facecount = 0;
    int vertposcount = 0;
    int vertuvcount = 0;
    int vertnormalcount = 0;

    {
        int index = FindIndexOfFirstNonWhitespace( buffer, 0 );
        while( buffer[index] != 0 )
        {
            if( buffer[index] == 'v' )
            {
                if( buffer[index+1] == ' ' )        vertposcount++;
                else if( buffer[index+1] == 't' )   vertuvcount++;
                else if( buffer[index+1] == 'n' )   vertnormalcount++;
            }
            else if( buffer[index] == 'f' )         facecount++;

            index = FindIndexOfFirstNonWhitespaceOfNextLine( buffer, index );
        }
    }

    // allocate individual buffers for pos/uv/norm, then parse again to store values
    vec3* VertexPositions = new vec3[vertposcount];
    vec2* VertexUVs = vertuvcount ? new vec2[vertuvcount] : 0;
    vec3* VertexNormals = vertnormalcount ? new vec3[vertnormalcount] : 0;
    FaceInfo* Faces = new FaceInfo[facecount];

    {
        int index = FindIndexOfFirstNonWhitespace( buffer, 0 );
        facecount = 0;
        vertposcount = 0;
        vertuvcount = 0;
        vertnormalcount = 0;

        while( buffer[index] != 0 )
        {
            if( buffer[index] == 'v' )
            {
                if( buffer[index+1] == ' ' )
                    VertexPositions[vertposcount++] = ParseVertex( buffer, index );
                else if( buffer[index+1] == 't' )
                    VertexUVs[vertuvcount++] = ParseVertex( buffer, index ).XY();
                else if( buffer[index+1] == 'n' )
                    VertexNormals[vertnormalcount++] = ParseVertex( buffer, index );
            }

            if( buffer[index] == 'f' )
            {
                Faces[facecount] = ParseFaceInfo( buffer, index );
                facecount++;
            }

            index = FindIndexOfFirstNonWhitespaceOfNextLine( buffer, index );
        }
    }

    // check the first face, to see how many attributes we need to allocate.
    int numcomponents = 0;

    for( int i=0; i<3; i++ )
    {
        if( Faces[0].attributes[0][i] != 0 )
        {
            if( i == 1 )
                numcomponents += 2; // uv
            else
                numcomponents += 3; // position or normal
        }
    }

    // we need a vert count, the number will be different if we remove dupes or not.
    int numverts = 0;
    int numtriangles = 0;

    // count the number of verts and triangles and copy the vertex indices into the face list.
    {
        numverts = 0;
        numtriangles = 0;
        int dupesfound = 0;

        // loop through all faces and vertices
        for( int f=0; f<facecount; f++ )
        {
            int numvertsinface = 3;
            numtriangles++;
            if( Faces[f].attributes[3][0] != 0 ) // if the 4th vertex has a valid position index, then it's a valid vert
            {
                numvertsinface = 4;
                numtriangles++;
            }

            for( int v=0; v<numvertsinface; v++ )
            {
                Faces[f].vertindex[v] = numverts + 1;
                numverts++;
            }
        }
    }

    int indexcount = numtriangles * 3;

    VertexFormat* verts = new VertexFormat[numverts];
    unsigned int* indices = new unsigned int[indexcount];

    // cycle through the faces and copy the vertex info into the final vertex/index buffers.
    {
        unsigned int indexcount = 0;
        unsigned int vertcount = 0;
        for( int f=0; f<facecount; f++ )
        {
            int vertindices[4];

            vertindices[0] = Faces[f].vertindex[0] - 1;
            vertindices[1] = Faces[f].vertindex[1] - 1;
            vertindices[2] = Faces[f].vertindex[2] - 1;
            vertindices[3] = Faces[f].vertindex[3] - 1;

            int numvertsinface = 3;
            if( Faces[f].attributes[3][0] != 0 ) // if the 4th vertex has a valid position index, then it's a valid vert
                numvertsinface = 4;

            for( int v=0; v<numvertsinface; v++ )
            {
                int vertindex = vertindices[v];

                int vertposindex = Faces[f].attributes[v][0] - 1;
                verts[vertindex].pos.x = VertexPositions[vertposindex].x * scale;
                verts[vertindex].pos.y = VertexPositions[vertposindex].y * scale;
                verts[vertindex].pos.z = VertexPositions[vertposindex].z * scale;

                int vertuvindex = Faces[f].attributes[v][1] - 1;
                if( vertuvindex != -1 )
                {
                    verts[vertindex].uv.x = VertexUVs[vertuvindex].x;
                    verts[vertindex].uv.y = VertexUVs[vertuvindex].y;
                }
            }

            if( numvertsinface == 3 )
            {
                indices[indexcount] = vertindices[0]; indexcount++;
                indices[indexcount] = vertindices[1]; indexcount++;
                indices[indexcount] = vertindices[2]; indexcount++;
            }
            else if( numvertsinface == 4 )
            {
                indices[indexcount] = vertindices[0]; indexcount++;
                indices[indexcount] = vertindices[1]; indexcount++;
                indices[indexcount] = vertindices[2]; indexcount++;

                indices[indexcount] = vertindices[0]; indexcount++;
                indices[indexcount] = vertindices[2]; indexcount++;
                indices[indexcount] = vertindices[3]; indexcount++;
            }
            else
            {
                assert( false );
            }

            vertcount += numvertsinface;
        }
    }

    delete[] VertexPositions;
    delete[] VertexUVs;
    delete[] VertexNormals;
    delete[] Faces;

    // give verts and indices pointers to BufferDefinition objects, which will handle the delete[]'s
    *ppVBO = verts;
    *ppIBO = indices;

    *pNumVerts = numverts;
    *pNumIndices = indexcount;

    delete[] buffer;
}
