// Copyright (c) 2006 Fernando Luis Cacciola Carballal. All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//

// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Straight_skeleton_2/include/CGAL/create_straight_skeleton_2.h $
// $Id: include/CGAL/create_straight_skeleton_2.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Fernando Cacciola <fernando_cacciola@ciudad.com.ar>
//
#ifndef CGAL_CREATE_STRAIGHT_SKELETON_2_H
#define CGAL_CREATE_STRAIGHT_SKELETON_2_H

#include <CGAL/license/Straight_skeleton_2.h>

#include <CGAL/compute_outer_frame_margin.h>
#include <CGAL/Straight_skeleton_2.h>
#include <CGAL/Straight_skeleton_builder_2.h>
#include <CGAL/Straight_skeleton_2/Polygon_iterators.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Kernel_traits.h>
#include <CGAL/Polygon_2.h>

#include <optional>
#include <memory>

#include <algorithm>
#include <iterator>
#include <vector>

namespace CGAL {

template<class PointIterator, class HoleIterator, class K>
std::shared_ptr< Straight_skeleton_2<K> >
create_interior_straight_skeleton_2 ( PointIterator aOuterContour_VerticesBegin
                                    , PointIterator aOuterContour_VerticesEnd
                                    , HoleIterator  aHolesBegin
                                    , HoleIterator  aHolesEnd
                                    , K const&
                                    )
{
  typedef Straight_skeleton_2<K> Ss ;

  typedef Straight_skeleton_builder_traits_2<K> SsBuilderTraits;

  typedef Straight_skeleton_builder_2<SsBuilderTraits,Ss> SsBuilder;

  typedef typename std::iterator_traits<PointIterator>::value_type InputPoint ;
  typedef typename Kernel_traits<InputPoint>::Kernel InputKernel ;

  Cartesian_converter<InputKernel, K> Point_converter ;

  SsBuilder ssb ;

  ssb.enter_contour( aOuterContour_VerticesBegin, aOuterContour_VerticesEnd, Point_converter ) ;

  for ( HoleIterator hi = aHolesBegin ; hi != aHolesEnd ; ++ hi )
    ssb.enter_contour( CGAL_SS_i::vertices_begin(*hi), CGAL_SS_i::vertices_end(*hi), Point_converter ) ;

  return ssb.construct_skeleton();
}

template<class PointIterator, class HoleIterator>
std::shared_ptr< Straight_skeleton_2< Exact_predicates_inexact_constructions_kernel > >
inline
create_interior_straight_skeleton_2 ( PointIterator aOuterContour_VerticesBegin
                                    , PointIterator aOuterContour_VerticesEnd
                                    , HoleIterator  aHolesBegin
                                    , HoleIterator  aHolesEnd
                                    )
{
  return create_interior_straight_skeleton_2(aOuterContour_VerticesBegin
                                            ,aOuterContour_VerticesEnd
                                            ,aHolesBegin
                                            ,aHolesEnd
                                            ,Exact_predicates_inexact_constructions_kernel()
                                            );
}

template<class PointIterator, class K>
std::shared_ptr< Straight_skeleton_2<K> >
inline
create_interior_straight_skeleton_2 ( PointIterator aOuterContour_VerticesBegin
                                    , PointIterator aOuterContour_VerticesEnd
                                    , K const&      k
                                    )
{
  typedef typename std::iterator_traits<PointIterator>::value_type InputPoint ;
  typedef typename Kernel_traits<InputPoint>::Kernel InputKernel ;

  std::vector< Polygon_2<InputKernel> > no_holes ;
  return create_interior_straight_skeleton_2(aOuterContour_VerticesBegin
                                            ,aOuterContour_VerticesEnd
                                            ,no_holes.begin()
                                            ,no_holes.end()
                                            ,k
                                            );
}

template<class PointIterator>
std::shared_ptr< Straight_skeleton_2<Exact_predicates_inexact_constructions_kernel> >
inline
create_interior_straight_skeleton_2 ( PointIterator aOuterContour_VerticesBegin
                                    , PointIterator aOuterContour_VerticesEnd
                                    )
{
  return create_interior_straight_skeleton_2(aOuterContour_VerticesBegin
                                            ,aOuterContour_VerticesEnd
                                            ,Exact_predicates_inexact_constructions_kernel()
                                            );
}

template<class Polygon, class K>
std::shared_ptr< Straight_skeleton_2<K> >
inline
create_interior_straight_skeleton_2 ( Polygon const& aOutContour,
                                      K const& k,
                                      std::enable_if_t<
                                        ! CGAL_SS_i::has_Hole_const_iterator<Polygon>::value>* = nullptr)
{
  return create_interior_straight_skeleton_2(CGAL_SS_i::vertices_begin(aOutContour)
                                            ,CGAL_SS_i::vertices_end(aOutContour)
                                            ,k
                                            );
}

template<class Polygon>
std::shared_ptr< Straight_skeleton_2< Exact_predicates_inexact_constructions_kernel > >
inline
create_interior_straight_skeleton_2 ( Polygon const& aOutContour )
{
  return create_interior_straight_skeleton_2(aOutContour, Exact_predicates_inexact_constructions_kernel() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/// EXTERIOR

template<class FT, class PointIterator, class K>
std::shared_ptr< Straight_skeleton_2<K> >
create_exterior_straight_skeleton_2 ( FT const&      aMaxOffset
                                    , PointIterator  aVerticesBegin
                                    , PointIterator  aVerticesEnd
                                    , K const&       k
                                    )
{
  CGAL_precondition( aMaxOffset > 0 ) ;

  typedef typename std::iterator_traits<PointIterator>::value_type   Point_2 ;
  typedef typename Kernel_traits<Point_2>::Kernel                    IK;
  typedef typename IK::FT                                            IFT;

  std::shared_ptr<Straight_skeleton_2<K> > rSkeleton;

  // That's because we might not have FT == IK::FT (e.g. `double` and `Core`)
  // Note that we can also have IK != K (e.g. `Simple_cartesian<Core>` and `EPICK`)
  IFT lOffset = aMaxOffset;

  // @todo This likely should be done in the kernel K rather than the input kernel (i.e. the same
  // converter stuff that is done in `create_partial_exterior_straight_skeleton_2`?).
  std::optional<IFT> margin = compute_outer_frame_margin( aVerticesBegin,
                                                            aVerticesEnd,
                                                            lOffset );

  if ( margin )
  {
    const IFT lm = *margin;
    const Bbox_2 bbox = bbox_2(aVerticesBegin, aVerticesEnd);

    const IFT fxmin = IFT(bbox.xmin()) - lm ;
    const IFT fxmax = IFT(bbox.xmax()) + lm ;
    const IFT fymin = IFT(bbox.ymin()) - lm ;
    const IFT fymax = IFT(bbox.ymax()) + lm ;

    Point_2 frame[4] ;
    frame[0] = Point_2(fxmin,fymin) ;
    frame[1] = Point_2(fxmax,fymin) ;
    frame[2] = Point_2(fxmax,fymax) ;
    frame[3] = Point_2(fxmin,fymax) ;

    typedef std::vector<Point_2> Hole ;

    Hole lPoly(aVerticesBegin, aVerticesEnd);
    std::reverse(lPoly.begin(), lPoly.end());

    std::vector<Hole> holes ;
    holes.push_back(lPoly) ;

    rSkeleton = create_interior_straight_skeleton_2(frame, frame+4, holes.begin(), holes.end(), k ) ;
  }

  return rSkeleton ;
}

template<class FT, class PointIterator>
std::shared_ptr< Straight_skeleton_2<Exact_predicates_inexact_constructions_kernel> >
inline
create_exterior_straight_skeleton_2 ( FT const&      aMaxOffset
                                    , PointIterator  aVerticesBegin
                                    , PointIterator  aVerticesEnd
                                    )
{
  return create_exterior_straight_skeleton_2(aMaxOffset
                                            ,aVerticesBegin
                                            ,aVerticesEnd
                                            ,Exact_predicates_inexact_constructions_kernel()
                                            );
}

template<class FT, class Polygon, class K>
std::shared_ptr< Straight_skeleton_2<K> >
inline
create_exterior_straight_skeleton_2 ( FT const& aMaxOffset, Polygon const& aPoly, K const& k )
{
  return create_exterior_straight_skeleton_2(aMaxOffset
                                            ,CGAL_SS_i::vertices_begin(aPoly)
                                            ,CGAL_SS_i::vertices_end  (aPoly)
                                            ,k
                                            );
}

template<class FT, class Polygon>
std::shared_ptr< Straight_skeleton_2<Exact_predicates_inexact_constructions_kernel> >
inline
create_exterior_straight_skeleton_2 ( FT const& aMaxOffset, Polygon const& aPoly )
{
  return create_exterior_straight_skeleton_2(aMaxOffset
                                            ,aPoly
                                            ,Exact_predicates_inexact_constructions_kernel()
                                            );
}

} // namespace CGAL

#endif // CGAL_CREATE_STRAIGHT_SKELETON_2_H
