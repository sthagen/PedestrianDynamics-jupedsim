// Copyright (c) 2009 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Mesh_3/include/CGAL/Mesh_3/Triangle_accessor_primitive.h $
// $Id: include/CGAL/Mesh_3/Triangle_accessor_primitive.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Stephane Tayeb
//
//******************************************************************************
// File Description :
//******************************************************************************

#ifndef CGAL_MESH_3_TRIANGLE_ACCESSOR_PRIMITIVE_H
#define CGAL_MESH_3_TRIANGLE_ACCESSOR_PRIMITIVE_H

#include <CGAL/license/Mesh_3.h>



namespace CGAL {
namespace Mesh_3 {

template<typename TriangleAccessor, typename GT>
class Triangle_accessor_primitive
{
public:
  typedef typename TriangleAccessor::Triangle_handle  Id;
  typedef typename GT::Triangle_3                     Datum;
  typedef typename GT::Point_3                        Point;

  Triangle_accessor_primitive(const Id& h)
    : handle_(h) {}
  // Default dtor, copy ctor and operator= are ok

  Datum datum() const
  {
    return TriangleAccessor().triangle(handle_);
  }

  Id id() const
  {
    return handle_;
  }

  Point reference_point() const
  {
    return TriangleAccessor().triangle(handle_).vertex(0);
  }

private:
  Id handle_;
};

//template <typename GT>
//class Triangle_accessor_primitive<Triangle_accessor<Polyhedron_3<GT>,GT>, GT>
//{
//  typedef class Triangle_accessor<Polyhedron_3<GT>,GT> Triangle_accessor;
//
//public:
//  typedef typename Triangle_accessor::Triangle_iterator Id;
//  typedef typename GT::Triangle_3                       Datum;
//  typedef typename GT::Point_3                          Point;
//
//  Triangle_accessor_primitive(const Id& h)
//    : handle_(h) {}
//  // Default dtor, copy ctor and operator= are ok
//
//  Datum datum() const
//  {
//    const Point& a = handle_->halfedge()->vertex()->point();
//    const Point& b = handle_->halfedge()->next()->vertex()->point();
//    const Point& c = handle_->halfedge()->next()->next()->vertex()->point();
//    return Datum(a,b,c);
//  }
//
//  Id id() const
//  {
//    return handle_;
//  }
//
//  Point reference_point() const
//  {
//    return handle_->halfedge()->vertex()->point();
//  }
//
//private:
//  Id handle_;
//};

} // end namespace Mesh_3

} // end namespace CGAL



#endif // CGAL_MESH_3_TRIANGLE_ACCESSOR_PRIMITIVE_H
