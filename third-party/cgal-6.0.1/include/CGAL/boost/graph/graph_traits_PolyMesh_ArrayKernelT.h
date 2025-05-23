// Copyright (c) 2007  GeometryFactory (France).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/BGL/include/CGAL/boost/graph/graph_traits_PolyMesh_ArrayKernelT.h $
// $Id: include/CGAL/boost/graph/graph_traits_PolyMesh_ArrayKernelT.h 50cfbde3b84 $
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Andreas Fabri, Philipp Moeller

#ifndef CGAL_BOOST_GRAPH_GRAPH_TRAITS_POLYMESH_ARRAYKERNELT_H
#define CGAL_BOOST_GRAPH_GRAPH_TRAITS_POLYMESH_ARRAYKERNELT_H

// https://www.graphics.rwth-aachen.de/media/openmesh_static/Documentations/OpenMesh-Doc-Latest/a02182.html
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <CGAL/boost/graph/properties_PolyMesh_ArrayKernelT.h>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

#define OPEN_MESH_CLASS OpenMesh::PolyMesh_ArrayKernelT<K>
#include <CGAL/boost/graph/graph_traits_OpenMesh.h>

#endif // CGAL_BOOST_GRAPH_TRAITS_POLYMESH_ARRAYKERNELT_H
