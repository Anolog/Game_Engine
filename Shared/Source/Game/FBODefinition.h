#ifndef __FBODefinition_H__
#define __FBODefinition_H__

class FBODefinition
{
protected:
    bool m_HasValidResources;
    bool m_FullyLoaded;

    bool m_FailedToInit;
    bool m_OnlyFreeOnShutdown;

    GLuint m_ColorTextureID;
    GLuint m_DepthTextureID;
    GLuint m_FrameBufferID;

    unsigned int m_Width; // size requested, mainly used by glViewport call.
    unsigned int m_Height;

    unsigned int m_TextureWidth; // generally will be power of 2 bigger than requested width/height
    unsigned int m_TextureHeight;

    int m_MinFilter;
    int m_MagFilter;

    bool m_NeedColorTexture;
    int m_DepthBits;
    bool m_DepthIsTexture;

    int m_LastFrameBufferID;

public:
    FBODefinition();
    virtual ~FBODefinition();

    bool IsFullyLoaded() { return m_FullyLoaded; }

    // returns true if a new texture needs to be created.
    bool Setup(unsigned int width, unsigned int height, int minfilter, int magfilter, bool needcolor, int depthbits, bool depthreadable);

    void Bind();
    void Unbind();

    void Invalidate(bool cleanglallocs);

    bool Create();

    GLuint GetColorTextureHandle() { return m_ColorTextureID; }
    GLuint GetDepthTextureHandle() { return m_DepthTextureID; }
};

#endif //__FBODefinition_H__
