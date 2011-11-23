// ======================================================================== //
// Copyright (C) 2011 Benjamin Segovia                                      //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#ifndef __RENDERER_OBJ_HPP__
#define __RENDERER_OBJ_HPP__

#include "renderer/texture.hpp"
#include "math/bbox.hpp"
#include "rt/bvh2.hpp"
#include "GL/gl3.h"

namespace pf
{
  // Renderer owns all RendererObj
  class Renderer;
  // We build the renderer obj from a Wavefront OBJ
  class Obj;
  // We build the hierarchy of segment if a BVH of triangles is given
  struct RTTriangle;
  template <typename T> class BVH2;

  /*! Entity used for rendering of OBJ models */
  class RendererObj : public RefCount
  {
  public:

    /*! Store triangles with the same material */
    struct Segment
    {
      BBox3f bbox;        //!< Bounding box of the triangles
      uint32 first, last; //!< First and last index in the index buffer
      uint32 matID;       //!< Material ID
    };

    /*! Renderer mapping of a OBJ material */
    struct Material
    {
      std::string name;
      std::string name_Ka;
      std::string name_Kd;
      std::string name_D;
      std::string name_Bump;
      Ref<Texture2D> map_Ka;
      Ref<Texture2D> map_Kd;
      Ref<Texture2D> map_D;
      Ref<Texture2D> map_Bump;
      vec3f amb;
      vec3f diff;
      vec3f spec;
      float km;
      float reflect;
      float refract;
      float trans;
      float shiny;
      float glossy;
      float refract_index;
    };

    /*! Note that this object actually belongs to a renderer */
    RendererObj(Renderer &renderer, const Obj &obj, const BVH2<RTTriangle> *bvh = NULL);
    /*! Release it (still from a renderer */
    ~RendererObj(void);
    /*! Valid means it is OGL uploaded */
    INLINE bool isValid(void) { return this->sgmtNum > 0; }
    /*! Display it using the renderer */
    void display(void);
    /*! Index of the first and last triangle index */
    struct Group { GLuint first, last; };
    Renderer &renderer;   //!< A RendererObj belongs to a renderer
    Material *mat;        //!< All the material of the object
    Segment *sgmt;        //!< All the sub-meshes to display
    uint32 matNum;        //!< Number of materials
    uint32 sgmtNum;       //!< Number of segments
    BVH2<Segment> bvh;    //!< Sorts the geometry
    GLuint vertexArray;   //!< Vertex declaration
    GLuint arrayBuffer;   //!< Vertex data (positions, normals...)
    GLuint elementBuffer; //!< Indices
    GLuint topology;      //!< Mostly triangle or triangle strip
    Ref<Task> texLoading; //!< XXX To load the textures
    MutexSys mutex;       //!< XXX just to play with async load
    friend class Renderer;
  };
}

#endif /* __RENDERER_OBJ_HPP__ */

