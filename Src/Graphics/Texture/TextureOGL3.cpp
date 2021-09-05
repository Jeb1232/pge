#include "../GraphicsOGL3.h"
#include <stdlib.h>

using namespace PGE;

static void textureImage(int width, int height, const byte* buffer, Texture::Format format) {
    GLint glInternalFormat;
    GLenum glFormat;
    GLenum glPixelType;
    int bytesPerPixel;
    switch (format) {
        case Texture::Format::RGBA64: {
            glInternalFormat = GL_RGBA16;
            glFormat = GL_RGBA;
            glPixelType = GL_UNSIGNED_SHORT;
            bytesPerPixel = 8;
        } break;
        case Texture::Format::RGBA32: {
            glInternalFormat = GL_RGBA8;
            glFormat = GL_RGBA;
            glPixelType = GL_UNSIGNED_BYTE;
            bytesPerPixel = 4;
        } break;
        case Texture::Format::R32F: {
            glInternalFormat = GL_DEPTH_COMPONENT32F;
            glFormat = GL_DEPTH_COMPONENT;
            glPixelType = GL_FLOAT;
            bytesPerPixel = 4;
        } break;
        case Texture::Format::R8: {
            glInternalFormat = GL_R8;
            glFormat = GL_RED;
            glPixelType = GL_UNSIGNED_BYTE;
            bytesPerPixel = 1;
        } break;
        default: {
            throw PGE_CREATE_EX("Invalid format");
        }
    }

    //Flip the texture on the Y axis because OpenGL's texture coordinate system is Y-up while we expect Y-down.
    //This matters because render targets will have to be flipped by the fragment shader, so to avoid turning
    //shaders into a clusterfuck, they will flip all textures.
    byte* yFlippedBuffer = (buffer != nullptr) ? new byte[width * height * bytesPerPixel] : nullptr;
    if (yFlippedBuffer != nullptr) {
        for (int y = 0; y < height; y++) {
            memcpy(yFlippedBuffer + (width * y * bytesPerPixel), buffer + (width * (height - y - 1) * bytesPerPixel), width * bytesPerPixel);
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glPixelType, yFlippedBuffer);
    GLenum glError = glGetError();
    PGE_ASSERT(glError == GL_NO_ERROR, "Failed to create texture (" + String::from(width) + "x" + String::from(height) + "; GLERROR: " + String::from(glError) + ")");

    if (yFlippedBuffer != nullptr) { delete[] yFlippedBuffer; }
}

static void applyTextureParameters(bool rt) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, rt ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, rt ? GL_NEAREST : GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, rt ? 1.f : 4.f);
}

TextureOGL3::TextureOGL3(Graphics& gfx, int w, int h, Format fmt) : Texture(w, h, true, fmt), resourceManager((GraphicsOGL3&)gfx) {
    ((GraphicsOGL3&)gfx).takeGlContext();
    glTexture = resourceManager.addNewResource<GLTexture>();
    textureImage(w, h, nullptr, fmt);
    applyTextureParameters(true);
    /*glGenFramebuffers(1,&glFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,glFramebuffer);*/
    glDepthbuffer = resourceManager.addNewResource<GLDepthBuffer>(w, h);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, glDepthbuffer);
}

TextureOGL3::TextureOGL3(Graphics& gfx, int w, int h, const byte* buffer, Format fmt, bool mipmaps) : Texture(w, h, false, fmt), resourceManager((GraphicsOGL3&)gfx) {
    ((GraphicsOGL3&)gfx).takeGlContext();
    glTexture = resourceManager.addNewResource<GLTexture>();
    textureImage(w, h, buffer, fmt);
    if (mipmaps) { glGenerateMipmap(GL_TEXTURE_2D); }
    applyTextureParameters(false);
}

GLuint TextureOGL3::getGlTexture() const {
    return glTexture;
}

/*GLuint TextureOGL3::getGlFramebuffer() const {
    return glFramebuffer;
}*/

GLuint TextureOGL3::getGlDepthbuffer() const {
    return glDepthbuffer;
}

void* TextureOGL3::getNative() const {
    return (void*)&glTexture;
}
