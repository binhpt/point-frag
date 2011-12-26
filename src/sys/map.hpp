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

#ifndef __PF_MAP_HPP__
#define __PF_MAP_HPP__

#include <map>

namespace pf
{
  template<class Key, class T, class Pred = std::less<Key>>
  class map : public std::map<Key,T,Pred,Allocator<std::pair<const Key, T>>>
  {
  public:
    // Typedefs
    typedef std::pair<const Key, T> value_type;
    typedef Allocator<value_type> allocator_type;
    typedef std::map<Key,T,Pred,allocator_type> parent_type;
    typedef Key key_type;
    typedef T mapped_type;
    typedef Pred key_compare;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;

    /*! Default constructor */
    INLINE map(const key_compare &comp = key_compare(),
               const allocator_type &a = allocator_type()) :
      parent_type(comp, a) {}
    /*! Iteration constructor */
    template<class InputIterator>
    INLINE map(InputIterator first,
               InputIterator last,
               const key_compare &comp = key_compare(),
               const allocator_type& a = allocator_type()) :
      parent_type(first, last, comp, a) {}
    /*! Copy constructor */
    INLINE map(const map& x) : parent_type(x) {}
  };
} /* namespace pf */

#endif /* __PF_MAP_HPP__ */

