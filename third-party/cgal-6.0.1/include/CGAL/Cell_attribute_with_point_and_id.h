// Copyright (c) 2017 CNRS and LIRIS' Establishments (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Linear_cell_complex/include/CGAL/Cell_attribute_with_point_and_id.h $
// $Id: include/CGAL/Cell_attribute_with_point_and_id.h 50cfbde3b84 $
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Guillaume Damiand <guillaume.damiand@liris.cnrs.fr>
//
#ifndef CGAL_CELL_ATTRIBUTE_WITH_POINT_AND_ID_H
#define CGAL_CELL_ATTRIBUTE_WITH_POINT_AND_ID_H 1

#include <CGAL/Cell_attribute_with_point.h>

namespace CGAL {

  // A cell attribute with point and id, when Info_!=void
  template <class Refs, class Info_=void, class Tag_=Tag_true,
            class OnMerge=Null_functor,
            class OnSplit=Null_functor>
  class Cell_attribute_with_point_and_id: public
      Cell_attribute_with_point<Refs, Info_, Tag_, OnMerge, OnSplit, Tag_true>
  {
    typedef Cell_attribute_with_point
          <Refs, Info_, Tag_, OnMerge, OnSplit, Tag_true> Base;

    template <class, class, class, class>
    friend class Compact_container;

    template <class, class>
    friend class Concurrent_compact_container;

    template <class, class, class, class>
    friend class Compact_container_with_index;

  public:
    typedef typename Base::Point Point;

  protected:
    /// Default constructor.
    Cell_attribute_with_point_and_id()
    {}

    /// Constructor with an info in parameter.
    Cell_attribute_with_point_and_id(const Point& apoint) : Base(apoint)
    {}

    /// Constructor with a point and an attribute in parameters.
    Cell_attribute_with_point_and_id(const Point& apoint, const Info_& ainfo) :
      Base(apoint, ainfo)
    {}
  };

  /// Specialization when Info==void.
  template <class Refs, class Tag_, class OnMerge, class OnSplit>
  class Cell_attribute_with_point_and_id<Refs, void, Tag_, OnMerge, OnSplit>:
      public Cell_attribute_with_point<Refs, void, Tag_, OnMerge, OnSplit, Tag_true>
  {
    typedef Cell_attribute_with_point
         <Refs, void, Tag_, OnMerge, OnSplit, Tag_true> Base;

    template <class, class, class, class>
    friend class Compact_container;

    template <class, class>
    friend class Concurrent_compact_container;

    template <class, class, class, class>
    friend class Compact_container_with_index;

  public:
    typedef typename Base::Point Point;

  protected:
    /// Default constructor.
    Cell_attribute_with_point_and_id()
    {}

    /// Constructor with a point in parameter.
    Cell_attribute_with_point_and_id(const Point& apoint) : Base(apoint)
    {}
  };
} // namespace CGAL

#endif // CGAL_CELL_ATTRIBUTE_WITH_POINT_AND_ID_H //
// EOF //
