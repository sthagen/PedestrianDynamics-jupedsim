// Copyright (c) 2001,2004  INRIA Sophia-Antipolis (France).
// Copyright (c) 2009  GeometryFactory Sarl (France)
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Filtered_kernel/include/CGAL/Filtered_kernel_fwd.h $
// $Id: include/CGAL/Filtered_kernel_fwd.h 50cfbde3b84 $
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Sylvain Pion, Laurent Rineau

#ifndef CGAL_FILTERED_KERNEL_FWD_H
#define CGAL_FILTERED_KERNEL_FWD_H

#include <CGAL/config.h>

namespace CGAL {

#ifdef CGAL_NO_STATIC_FILTERS
template < typename CK, bool UseStaticFilters = false >
#else
template < typename CK, bool UseStaticFilters = true >
#endif
struct Filtered_kernel;

} // end namespace CGAL

#endif // CGAL_FILTERED_KERNEL_FWD_H
