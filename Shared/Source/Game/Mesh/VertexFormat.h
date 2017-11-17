#ifndef __VertexFormat_H__
#define __VertexFormat_H__

struct VertexFormat
{
    vec3 pos;
    MyColor color;
    vec2 uv;
    vec3 normal;

    VertexFormat() {}
    VertexFormat(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float u, float v, float nx, float ny, float nz)
    {
        pos.Set( x, y, z );
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
        uv.Set( u, v );
        normal.Set( nx, ny, nz );
    }    
};

#endif //__VertexFormat_H__
