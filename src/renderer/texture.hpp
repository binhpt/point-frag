#ifndef __PF_TEXTURE_HPP__
#define __PF_TEXTURE_HPP__

#include "sys/filename.hpp"
#include "sys/ref.hpp"
#include "renderer/GL/gl3.h"

namespace pf
{
  /*! Its owns all the textures */
  class Renderer;

  /*! Small wrapper around a GL 2D texture */
  struct Texture2D : RefCount
  {
    /*! Create texture from an image. mipmap == true will create the mipmaps */
    Texture2D(Renderer &renderer, const FileName &path, bool mipmap = true);
    ~Texture2D(void);
    Renderer &renderer;/*! Owner of this class */
    GLuint handle;     /*! Texture object itself */
    GLuint fmt;        /*! GL format of the texture */
    GLuint w, h;       /*! Dimensions of level 0 */
    GLuint minLevel;   /*! Minimum level we loaded */
    GLuint maxLevel;   /*! Maximum level we loaded */
  };
}

#endif /* __PF_TEXTURE_HPP__ */
