// Copyright (c) 1999,2001
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/STL_Extension/include/CGAL/Twotuple.h $
// $Id: include/CGAL/Twotuple.h 50cfbde3b84 $
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Andreas Fabri

#ifndef CGAL_TWOTUPLE_H
#define CGAL_TWOTUPLE_H

#include <CGAL/config.h>

#define CGAL_DEPRECATED_HEADER "<CGAL/Twotuple.h>"
#define CGAL_DEPRECATED_MESSAGE_DETAILS "Please use `std::array` instead."
#include <CGAL/Installation/internal/deprecation_warning.h>

#ifndef CGAL_NO_DEPRECATED_CODE

namespace CGAL {

template < class T >
struct CGAL_DEPRECATED Twotuple
{
  typedef T value_type;

  T  e0, e1;

  Twotuple()
  {}

  Twotuple(const T & a0, const T &a1)  : e0(a0), e1(a1)
  {}
};

} //namespace CGAL

#endif // CGAL_NO_DEPRECATED_CODE

#endif // CGAL_TWOTUPLE_H
