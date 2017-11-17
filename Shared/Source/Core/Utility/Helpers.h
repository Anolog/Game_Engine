#ifndef __Helpers_H__
#define __Helpers_H__

#define MyAssert assert

void OutputMessage(const char* message, ...);

void CheckForGLErrors();

GLuint LoadTexture(const char* filename, GLuint oldtexturehandle = 0);
void Flip32BitImageVertically(unsigned char* buffer, unsigned int width, unsigned int height);

GLuint LoadTextureCubemap(const char ** filenames, GLuint oldtexturehandle=0);

class MyColor
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    MyColor()
    {
    }
    MyColor(unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na)
    {
        r = nr;
        g = ng;
        b = nb;
        a = na;
    }
};

#endif //__Helpers_H__
