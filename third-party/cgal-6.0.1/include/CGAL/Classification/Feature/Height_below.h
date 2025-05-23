// Copyright (c) 2012 INRIA Sophia-Antipolis (France).
// Copyright (c) 2017 GeometryFactory Sarl (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Classification/include/CGAL/Classification/Feature/Height_below.h $
// $Id: include/CGAL/Classification/Feature/Height_below.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Florent Lafarge, Simon Giraudot

#ifndef CGAL_CLASSIFICATION_FEATURE_HEIGHT_BELOW_H
#define CGAL_CLASSIFICATION_FEATURE_HEIGHT_BELOW_H

#include <CGAL/license/Classification.h>

#include <vector>

#include <CGAL/Classification/Feature_base.h>
#include <CGAL/Classification/compressed_float.h>
#include <CGAL/Classification/Image.h>
#include <CGAL/Classification/Planimetric_grid.h>

namespace CGAL {

namespace Classification {

namespace Feature {

  /*!
    \ingroup PkgClassificationFeatures

    %Feature based on local height distribution This feature computes
    the distance between a point's height and the minimum height on
    the local cell of the planimetric grid.

    Its default name is "height_below".

    \tparam GeomTraits model of \cgal Kernel.
    \tparam PointRange model of `ConstRange`. Its iterator type
    is `RandomAccessIterator` and its value type is the key type of
    `PointMap`.
    \tparam PointMap model of `ReadablePropertyMap` whose key
    type is the value type of the iterator of `PointRange` and value type
    is `GeomTraits::Point_3`.

  */
template <typename GeomTraits, typename PointRange, typename PointMap>
class Height_below : public Feature_base
{
  typedef Image<float> Image_float;
  typedef Planimetric_grid<GeomTraits, PointRange, PointMap> Grid;

  const PointRange& input;
  PointMap point_map;
  const Grid& grid;
  Image_float dtm;
  std::vector<float> values;

public:
  /*!
    \brief constructs the feature.

    \param input point range.
    \param point_map property map to access the input points.
    \param grid precomputed `Planimetric_grid`.
  */
  Height_below (const PointRange& input,
                PointMap point_map,
                const Grid& grid)
    : input(input), point_map(point_map), grid(grid)
  {
    this->set_name ("height_below");

    dtm = Image_float(grid.width(),grid.height());

    for (std::size_t j = 0; j < grid.height(); ++ j)
      for (std::size_t i = 0; i < grid.width(); ++ i)
        if (grid.has_points(i,j))
        {
          float z_min = (std::numeric_limits<float>::max)();

          typename Grid::iterator end = grid.indices_end(i,j);
          for (typename Grid::iterator it = grid.indices_begin(i,j); it != end; ++ it)
          {
            float z = float(get(point_map, *(input.begin()+(*it))).z());
            z_min = ((std::min)(z_min, z));
          }

          dtm(i,j) = z_min;
        }

    if (grid.width() * grid.height() > input.size())
    {
      values.resize (input.size(), 0.f);
      for (std::size_t i = 0; i < input.size(); ++ i)
      {
        std::size_t I = grid.x(i);
        std::size_t J = grid.y(i);
        values[i] = float(get (point_map, *(input.begin() + i)).z() - dtm(I,J));
      }
      (dtm.free)();
    }

  }

  /// \cond SKIP_IN_MANUAL
  virtual float value (std::size_t pt_index)
  {
    if (values.empty())
    {
      std::size_t I = grid.x(pt_index);
      std::size_t J = grid.y(pt_index);
      return float(get (point_map, *(input.begin() + pt_index)).z() - dtm(I,J));
    }

    return values[pt_index];
  }

  /// \endcond
};

} // namespace Feature

} // namespace Classification


} // namespace CGAL

#endif // CGAL_CLASSIFICATION_FEATURE_HEIGHT_BELOW_H
