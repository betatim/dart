/*
 * Copyright (c) 2015, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Michael X. Grey <mxgrey@gatech.edu>
 *
 * Georgia Tech Graphics Lab and Humanoid Robotics Lab
 *
 * Directed by Prof. C. Karen Liu and Prof. Mike Stilman
 * <karenliu@cc.gatech.edu> <mstilman@cc.gatech.edu>
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DART_COMMON_DETAIL_SPECIALIZEDFORASPECT_H_
#define DART_COMMON_DETAIL_SPECIALIZEDFORASPECT_H_

#include "dart/common/SpecializedForAspect.h"

// This preprocessor token should only be used by the unittest that is
// responsible for checking that the specialized routines are being used to
// access specialized Aspects
#ifdef DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS
bool usedSpecializedAspectAccess;
#endif // DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS

namespace dart {
namespace common {

//==============================================================================
template <class SpecAspect>
SpecializedForAspect<SpecAspect>::SpecializedForAspect()
{
  mAspectMap[typeid( SpecAspect )] = nullptr;
  mSpecAspectIterator = mAspectMap.find(typeid( SpecAspect ));
}

//==============================================================================
template <class SpecAspect>
template <class T>
bool SpecializedForAspect<SpecAspect>::has() const
{
  return _has(type<T>());
}

//==============================================================================
template <class SpecAspect>
template <class T>
T* SpecializedForAspect<SpecAspect>::get()
{
  return _get(type<T>());
}

//==============================================================================
template <class SpecAspect>
template <class T>
const T* SpecializedForAspect<SpecAspect>::get() const
{
  return _get(type<T>());
}

//==============================================================================
template <class SpecAspect>
template <class T>
void SpecializedForAspect<SpecAspect>::set(const T* aspect)
{
  _set(type<T>(), aspect);
}

//==============================================================================
template <class SpecAspect>
template <class T>
void SpecializedForAspect<SpecAspect>::set(std::unique_ptr<T>&& aspect)
{
  _set(type<T>(), std::move(aspect));
}

//==============================================================================
template <class SpecAspect>
template <class T, typename ...Args>
T* SpecializedForAspect<SpecAspect>::create(Args&&... args)
{
  return _create(type<T>(), std::forward<Args>(args)...);
}

//==============================================================================
template <class SpecAspect>
template <class T>
void SpecializedForAspect<SpecAspect>::erase()
{
  _erase(type<T>());
}

//==============================================================================
template <class SpecAspect>
template <class T>
std::unique_ptr<T> SpecializedForAspect<SpecAspect>::release()
{
  return _release(type<T>());
}

//==============================================================================
template <class SpecAspect>
template <class T>
constexpr bool SpecializedForAspect<SpecAspect>::isSpecializedFor()
{
  return _isSpecializedFor(type<T>());
}

//==============================================================================
template <class SpecAspect>
template <class T>
bool SpecializedForAspect<SpecAspect>::_has(type<T>) const
{
#ifdef DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS
  usedSpecializedAspectAccess = true;
#endif // DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS

  return Composite::has<T>();
}

//==============================================================================
template <class SpecAspect>
bool SpecializedForAspect<SpecAspect>::_has(type<SpecAspect>) const
{
#ifdef DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS
  usedSpecializedAspectAccess = true;
#endif // DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS

  return (mSpecAspectIterator->second.get() != nullptr);
}

//==============================================================================
template <class SpecAspect>
template <class T>
T* SpecializedForAspect<SpecAspect>::_get(type<T>)
{
  return Composite::get<T>();
}

//==============================================================================
template <class SpecAspect>
SpecAspect* SpecializedForAspect<SpecAspect>::_get(type<SpecAspect>)
{
#ifdef DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS
  usedSpecializedAspectAccess = true;
#endif // DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS

  return static_cast<SpecAspect*>(mSpecAspectIterator->second.get());
}

//==============================================================================
template <class SpecAspect>
template <class T>
const T* SpecializedForAspect<SpecAspect>::_get(type<T>) const
{
  return Composite::get<T>();
}

//==============================================================================
template <class SpecAspect>
const SpecAspect* SpecializedForAspect<SpecAspect>::_get(type<SpecAspect>) const
{
#ifdef DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS
  usedSpecializedAspectAccess = true;
#endif // DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS

  return static_cast<SpecAspect*>(mSpecAspectIterator->second.get());
}

//==============================================================================
template <class SpecAspect>
template <class T>
void SpecializedForAspect<SpecAspect>::_set(type<T>, const T* aspect)
{
  Composite::set<T>(aspect);
}

//==============================================================================
template <class SpecAspect>
void SpecializedForAspect<SpecAspect>::_set(
    type<SpecAspect>, const SpecAspect* aspect)
{
#ifdef DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS
  usedSpecializedAspectAccess = true;
#endif // DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS

  if(aspect)
  {
    mSpecAspectIterator->second = aspect->cloneAspect(this);
    addToComposite(mSpecAspectIterator->second.get());
  }
  else
  {
    mSpecAspectIterator->second = nullptr;
  }
}

//==============================================================================
template <class SpecAspect>
template <class T>
void SpecializedForAspect<SpecAspect>::_set(type<T>, std::unique_ptr<T>&& aspect)
{
  Composite::set<T>(std::move(aspect));
}

//==============================================================================
template <class SpecAspect>
void SpecializedForAspect<SpecAspect>::_set(
    type<SpecAspect>, std::unique_ptr<SpecAspect>&& aspect)
{
#ifdef DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS
  usedSpecializedAspectAccess = true;
#endif // DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS

  mSpecAspectIterator->second = std::move(aspect);
  addToComposite(mSpecAspectIterator->second.get());
}

//==============================================================================
template <class SpecAspect>
template <class T, typename ...Args>
T* SpecializedForAspect<SpecAspect>::_create(type<T>, Args&&... args)
{
  return Composite::create<T>(std::forward<Args>(args)...);
}

//==============================================================================
template <class SpecAspect>
template <typename ...Args>
SpecAspect* SpecializedForAspect<SpecAspect>::_create(
    type<SpecAspect>, Args&&... args)
{
#ifdef DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS
  usedSpecializedAspectAccess = true;
#endif // DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS

  SpecAspect* aspect = new SpecAspect(this, std::forward<Args>(args)...);
  mSpecAspectIterator->second = std::unique_ptr<SpecAspect>(aspect);
  addToComposite(aspect);

  return aspect;
}

//==============================================================================
template <class SpecAspect>
template <class T>
void SpecializedForAspect<SpecAspect>::_erase(type<T>)
{
  Composite::erase<T>();
}

//==============================================================================
template <class SpecAspect>
void SpecializedForAspect<SpecAspect>::_erase(type<SpecAspect>)
{
#ifdef DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS
  usedSpecializedAspectAccess = true;
#endif // DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS

  DART_COMMON_CHECK_ILLEGAL_ASPECT_ERASE(erase, SpecAspect, DART_BLANK);

  removeFromComposite(mSpecAspectIterator->second.get());
  mSpecAspectIterator->second = nullptr;
}

//==============================================================================
template <class SpecAspect>
template <class T>
std::unique_ptr<T> SpecializedForAspect<SpecAspect>::_release(type<T>)
{
  return Composite::release<T>();
}

//==============================================================================
template <class SpecAspect>
std::unique_ptr<SpecAspect> SpecializedForAspect<SpecAspect>::_release(
    type<SpecAspect>)
{
#ifdef DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS
  usedSpecializedAspectAccess = true;
#endif // DART_UNITTEST_SPECIALIZED_ASPECT_ACCESS

  DART_COMMON_CHECK_ILLEGAL_ASPECT_ERASE(release, SpecAspect, nullptr);

  removeFromComposite(mSpecAspectIterator->second.get());
  std::unique_ptr<SpecAspect> extraction(
        static_cast<SpecAspect*>(mSpecAspectIterator->second.release()));

  return extraction;
}

//==============================================================================
template <class SpecAspect>
template <class T>
constexpr bool SpecializedForAspect<SpecAspect>::_isSpecializedFor(type<T>)
{
  return false;
}

//==============================================================================
template <class SpecAspect>
constexpr bool SpecializedForAspect<SpecAspect>::_isSpecializedFor(type<SpecAspect>)
{
  return true;
}

} // namespace common
} // namespace dart

#endif // DART_COMMON_DETAIL_SPECIALIZEDFORASPECT_H_