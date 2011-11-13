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

#ifndef __RT_TRIANGLE_HPP__
#define __RT_TRIANGLE_HPP__

#include "math/bbox.hpp"

namespace pf
{
  /*! Store three vertices */
  struct RTTriangle
  {
    RTTriangle(void);
    RTTriangle(const vec3f &a, const vec3f &b, const vec3f &c);
    BBox3f getAABB(void) const;
    vec3f v[3];
  };

  INLINE RTTriangle::RTTriangle(void) {}
  INLINE RTTriangle::RTTriangle(const vec3f &a, const vec3f &b, const vec3f &c)
  { v[0] = a; v[1] = b; v[2] = c; }

  INLINE BBox3f RTTriangle::getAABB(void) const {
    return BBox3f(vec3f(min(min(v[0].x, v[1].x), v[2].x),
                        min(min(v[0].y, v[1].y), v[2].y),
                        min(min(v[0].z, v[1].z), v[2].z)),
                  vec3f(max(max(v[0].x, v[1].x), v[2].x),
                        max(max(v[0].y, v[1].y), v[2].y),
                        max(max(v[0].z, v[1].z), v[2].z)));
  }
} /* namespace pf */

#endif /* __RT_TRIANGLE_HPP__ */
