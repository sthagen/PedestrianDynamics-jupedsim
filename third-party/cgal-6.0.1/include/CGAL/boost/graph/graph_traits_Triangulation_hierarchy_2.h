// Copyright (c) 2007  GeometryFactory (France).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Triangulation_2/include/CGAL/boost/graph/graph_traits_Triangulation_hierarchy_2.h $
// $Id: include/CGAL/boost/graph/graph_traits_Triangulation_hierarchy_2.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Andreas Fabri, Fernando Cacciola

#ifndef CGAL_GRAPH_TRAITS_TRIANGULATION_HIERARCHY_2_H
#define CGAL_GRAPH_TRAITS_TRIANGULATION_HIERARCHY_2_H

#include <CGAL/license/Triangulation_2.h>


#include <CGAL/Triangulation_hierarchy_2.h>
#include <CGAL/boost/graph/properties_Triangulation_hierarchy_2.h>

// The functions and classes in this file allows the user to
// treat a CGAL Delaunay_triangulation_2 object as a boost graph "as is". No
// wrapper is needed for the Constrained_triangulation_2 object.

#define CGAL_2D_TRIANGULATION_TEMPLATE_PARAMETERS typename Tr
#define CGAL_2D_TRIANGULATION CGAL::Triangulation_hierarchy_2<Tr>
#define CGAL_2D_TRIANGULATION_TEMPLATES Tr

#include <CGAL/boost/graph/internal/graph_traits_2D_triangulation.h>

#endif // CGAL_GRAPH_TRAITS_TRIANGULATION_HIERARCHY_2_H
