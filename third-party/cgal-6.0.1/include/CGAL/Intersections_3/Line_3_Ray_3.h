// Copyright (c) 1997-2021
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).
// GeometryFactory (France)
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Intersections_3/include/CGAL/Intersections_3/Line_3_Ray_3.h $
// $Id: include/CGAL/Intersections_3/Line_3_Ray_3.h 50cfbde3b84 $
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Sebastien Loriot
//

#ifndef CGAL_INTERSECTIONS_3_LINE_3_RAY_3_H
#define CGAL_INTERSECTIONS_3_LINE_3_RAY_3_H

#include <CGAL/Intersection_traits.h>
#include <CGAL/Intersections_3/internal/Line_3_Ray_3_do_intersect.h>
#include <CGAL/Intersections_3/internal/Line_3_Ray_3_intersection.h>

#include <CGAL/Line_3.h>
#include <CGAL/Ray_3.h>

namespace CGAL {

CGAL_DO_INTERSECT_FUNCTION(Line_3, Ray_3, 3)
CGAL_INTERSECTION_FUNCTION(Line_3, Ray_3, 3)

} // namespace CGAL

#endif // CGAL_INTERSECTIONS_3_LINE_3_RAY_3_H
