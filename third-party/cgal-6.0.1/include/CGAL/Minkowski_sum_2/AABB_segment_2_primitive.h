// Copyright (c) 2015  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Minkowski_sum_2/include/CGAL/Minkowski_sum_2/AABB_segment_2_primitive.h $
// $Id: include/CGAL/Minkowski_sum_2/AABB_segment_2_primitive.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s): Sebastian Morr    <sebastian@morr.cc>

#ifndef CGAL_AABB_SEGMENT_2_PRIMITIVE_H
#define CGAL_AABB_SEGMENT_2_PRIMITIVE_H

#include <CGAL/license/Minkowski_sum_2.h>


namespace CGAL {

// Wraps around a Segment_2 and provides its iterator as Id
template <class GeomTraits, class Iterator_, class ContainerType>
class AABB_segment_2_primitive
{

public:

  typedef Iterator_ Id;
  typedef typename GeomTraits::Segment_2 Datum;
  typedef typename GeomTraits::Point_2 Point;
  typedef ContainerType Container;

  AABB_segment_2_primitive() {}

  AABB_segment_2_primitive(Id it) : m_it(it)
  {
  }

  AABB_segment_2_primitive(const AABB_segment_2_primitive& primitive) = default;
  AABB_segment_2_primitive& operator=(const AABB_segment_2_primitive& primitive) = default;

  const Id &id() const
  {
    return m_it;
  }

  const Datum datum() const
  {
    return *m_it;
  }

  // Return a point on the primitive
  Point reference_point() const
  {
    return m_it->source();
  }

private:

  Id m_it;

};

} // namespace CGAL

#endif
