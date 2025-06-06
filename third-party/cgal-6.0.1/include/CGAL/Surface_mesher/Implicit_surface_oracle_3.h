// Copyright (c) 2003-2007  INRIA Sophia-Antipolis (France).
// Copyright (c) 2008       GeometryFactory, Sophia Antipolis (France)
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Surface_mesher/include/CGAL/Surface_mesher/Implicit_surface_oracle_3.h $
// $Id: include/CGAL/Surface_mesher/Implicit_surface_oracle_3.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Steve OUDOT, Laurent RINEAU

#ifndef CGAL_SURFACE_MESHER_IMPLICIT_SURFACE_ORACLE_3_H
#define CGAL_SURFACE_MESHER_IMPLICIT_SURFACE_ORACLE_3_H

#include <CGAL/license/Surface_mesher.h>

#define CGAL_DEPRECATED_HEADER "<CGAL/Surface_mesher/Implicit_surface_oracle_3.h>"
#define CGAL_DEPRECATED_MESSAGE_DETAILS \
  "The 3D Mesh Generation package (see https://doc.cgal.org/latest/Mesh_3/) should be used instead."
#include <CGAL/Installation/internal/deprecation_warning.h>

#include <CGAL/disable_warnings.h>

#include <CGAL/Surface_mesher/Null_oracle_visitor.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/Surface_mesher/Sphere_oracle_3.h>
#include <CGAL/Real_embeddable_traits.h>
#include <CGAL/Surface_mesher/Profile_counter.h>
#include <CGAL/squared_distance_3.h>
#include <CGAL/Origin.h>

#include <boost/mpl/has_xxx.hpp>

#include <queue>

#include <CGAL/Surface_mesher/Profile_timer.h>

#ifdef CGAL_SURFACE_MESHER_DEBUG_IMPLICIT_ORACLE
#  define CGAL_SURFACE_MESHER_DEBUG_CLIPPED_SEGMENT
#endif

#ifdef CGAL_SURFACE_MESHER_DEBUG_CLIPPED_SEGMENT
#  include <string>
#  include <sstream>
#  include <boost/format.hpp>
#endif

// NB: this oracle requires that the user provide a function that can
// compute the value of the potential in any point of space
namespace CGAL {

#ifdef  BOOST_MPL_CFG_NO_HAS_XXX
#  error "BOOST_MPL_HAS_XXX_TRAIT_DEF is needed!"
#else
   BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_set_on_surface, Set_on_surface_tag, true)
#endif

  namespace Surface_mesher {

  namespace {

  template <typename T>
  struct Return_min : CGAL::cpp98::binary_function<T, T, T>
  {
    T operator()(const T& a, const T& b) const
    {
      return (std::min)(a, b);
    }
  };

  } // end anonymous namespace

  template <
    class GT,
    class Surface,
    class Transform_functor_ =
      typename Real_embeddable_traits<typename Surface::FT>::Sgn,
    class Surface_identifiers_generator_ =
      Return_min<typename Transform_functor_::result_type>,
    class Point_creator = Creator_uniform_3<typename GT::FT,
                                            typename GT::Point_3>,
    class Visitor = Null_oracle_visitor
    >
  class Implicit_surface_oracle_3
  {
    // private types
    typedef Implicit_surface_oracle_3<GT,
                                      Surface,
                                      Transform_functor_,
                                      Surface_identifiers_generator_,
                                      Point_creator,
                                      Visitor> Self;

    typedef Sphere_oracle_3<GT, Point_creator> Sphere_oracle;

    typedef typename GT::Point_3 Point;

    typedef typename GT::FT FT;
    typedef typename GT::Sphere_3 Sphere_3;

  public:

    // Public types
    typedef GT Geom_traits;
    typedef typename GT::Point_3 Point_3;
    typedef typename GT::Segment_3 Segment_3;
    typedef typename GT::Ray_3 Ray_3;
    typedef typename GT::Line_3 Line_3;

    typedef Transform_functor_ Transform_functor;
    typedef Surface_identifiers_generator_ Surface_identifiers_generator;

    typedef Point_3 Intersection_point;

    typedef Surface Surface_3;

    typedef typename Surface::FT Surface_value_type;
    typedef typename Transform_functor::result_type result_type;

  private:
    // Private members
    Visitor visitor; // a visitor that can modify a point, before returning
                     // it.
    Transform_functor transform_functor;
    Surface_identifiers_generator surface_identifiers_generator;

  public:

    // Constructors
//     Implicit_surface_oracle_3 (Visitor visitor_ = Visitor()) :
//       visitor(visitor_),
//       transform_functor(),
//       surface_identifiers_generator()
//     {
// #ifdef CGAL_SURFACE_MESHER_DEBUG_CONSTRUCTORS
//       std::cerr << "CONS: Implicit_surface_oracle_3\n";
// #endif
//     }

    Implicit_surface_oracle_3 (Transform_functor transform_functor
                                 = Transform_functor(),
                               Surface_identifiers_generator
                                 surface_identifiers_generator
                                   = Surface_identifiers_generator(),
                               Visitor visitor_ = Visitor()) :
      visitor(visitor_),
      transform_functor(transform_functor),
      surface_identifiers_generator(surface_identifiers_generator)
    {
#ifdef CGAL_SURFACE_MESHER_DEBUG_CONSTRUCTORS
      std::cerr << "CONS: Implicit_surface_oracle_3\n";
#endif
    }

    class Intersect_3
    {
      Visitor visitor;
      Transform_functor transform_functor;
      Surface_identifiers_generator surface_identifiers_generator;

      // The two following template and specialization are used in
      // intersect_clipped_segment(...).
      template <typename Point, typename Identifier, bool b>
      struct Set_on_surface {
        void operator()(Point &, const Identifier&) const
        {
        }
      };

      template <typename Point, typename Identifier>
      struct Set_on_surface<Point, Identifier, true> {
        void operator()(Point & p, const Identifier& id) const
        {
          p.set_on_surface(id);
        }
      };

      Set_on_surface<Point,
                     typename Surface_identifiers_generator::result_type,
                     CGAL::has_set_on_surface<Point>::value> set_on_surface;

    public:
      Intersect_3(Visitor visitor, Transform_functor transform_functor,
                  Surface_identifiers_generator surface_identifiers_generator)
        : visitor(visitor),
          transform_functor(transform_functor),
          surface_identifiers_generator(surface_identifiers_generator)
      {
      }

      Object operator()(const Surface_3& surface, Segment_3 s)
      // s is passed by value, because it is clipped below
      {
        CGAL_SURFACE_MESHER_PROFILER("Implificit_surface_oracle::Intersect_3::operator(Segment_3)");
        CGAL_SURFACE_MESHER_TIME_PROFILER("Implificit_surface_oracle::Intersect_3::operator(Segment_3)");
        typename GT::Construct_point_on_3 point_on =
          GT().construct_point_on_3_object();

        typename Sphere_oracle::Intersect_3 clip =
          Sphere_oracle().intersect_3_object();

        const Sphere_3& sphere = surface.bounding_sphere();

        Point_3 a = point_on(s, 0);
        Point_3 b = point_on(s, 1);

#ifdef CGAL_SURFACE_MESHER_DEBUG_IMPLICIT_ORACLE
        std::cerr <<
          boost::format("** Implicit_surface_oracle_3::operator()( (%1%), (%2%) )\n")
          % a % b;
#endif
        // if both extremities are on the same side of the surface, return
        // no intersection
        if(surf_equation(surface, a) == surf_equation(surface, b))
          return Object();

        // Code for surfaces with boundaries
        // First rescale segment s = [a, b]
        if( clip.clip_segment(sphere, a, b) )
          return intersect_clipped_segment(surface,
                                           a,
                                           b,
                                           surface.squared_error_bound());
        // else
#ifdef CGAL_SURFACE_MESHER_DEBUG_IMPLICIT_ORACLE
        std::cerr << "No intersection\n";
#endif
        return Object();
      } // end operator()(Surface_3, Segment_3)

      Object operator()(const Surface_3& surface, const Ray_3& r) {
        CGAL_SURFACE_MESHER_PROFILER("Implificit_surface_oracle::Intersect_3::operator(Ray_3)");
        CGAL_SURFACE_MESHER_TIME_PROFILER("Implificit_surface_oracle::Intersect_3::operator(Ray_3)");
        typename Sphere_oracle::Intersect_3 clip =
          Sphere_oracle().intersect_3_object();

        const Sphere_3& sphere = surface.bounding_sphere();

        Point_3 a, b;
        if(clip.clip_ray(sphere, r, a, b))
        {
          return intersect_clipped_segment(surface,
                                           a,
                                           b,
                                           surface.squared_error_bound());
        }
        // else
        return Object();
      } // end operator()(Surface_3, Ray_3)

      Object operator()(const Surface_3& surface, const Line_3& l) {
        CGAL_SURFACE_MESHER_PROFILER("Implificit_surface_oracle::Intersect_3::operator(Line_3)");
        CGAL_SURFACE_MESHER_TIME_PROFILER("Implificit_surface_oracle::Intersect_3::operator(Line_3)");
        typename Sphere_oracle::Intersect_3 clip =
          Sphere_oracle().intersect_3_object();

        const Sphere_3& sphere = surface.bounding_sphere();

        Point_3 a, b;
        if(clip.clip_line(sphere, l, a, b))
        {
          return intersect_clipped_segment(surface,
                                           a,
                                           b,
                                           surface.squared_error_bound());
        }
        else
          return Object();
      } // end operator()(Surface_3, Line_3)

      // debug function
      std::string debug_point(const Surface_3& surface,
                              const Point& p)
      {
        std::stringstream s;
        s << p << " (distance="
          << CGAL::sqrt(CGAL::squared_distance(p,
                                 surface.bounding_sphere().center()))
          << ", value=" << surf_equation(surface, p)
          << ")";
        return s.str();
      }

      result_type surf_equation (const Surface_3& surface,
                                 const Point& p)
      {
        return transform_functor(surface(p));
      } // @TODO, @WARNING: we use x(), y() and z()

    private:
      // Private functions
      Object intersect_clipped_segment(const Surface_3& surface,
                                       Point p1,
                                       Point p2,
                                       const FT& squared_distance_bound)
      {
        CGAL_SURFACE_MESHER_PROFILER("intersect_clipped_segment");
#ifdef CGAL_SURFACE_MESHER_DEBUG_CLIPPED_SEGMENT
        std::cerr << "clipped_segment( ("  << p1 << "), (" << p2 << ") )\n";
#endif
        typename GT::Compute_squared_distance_3 squared_distance =
          GT().compute_squared_distance_3_object();
        typename GT::Construct_midpoint_3 midpoint =
          GT().construct_midpoint_3_object();

        // Cannot be const: those values are modified below.
        result_type value_at_p1 = surf_equation(surface, p1);
        result_type value_at_p2 = surf_equation(surface, p2);

        // If both extremities are in the same volume component, returns
        // no intersection.
        if(value_at_p1 == value_at_p2)
          return Object();

#ifdef CGAL_SURFACE_MESHER_PROFILE
        int steps = 0;
#endif
        while(true)
        {
#ifdef CGAL_SURFACE_MESHER_PROFILE
          ++steps;
#endif
#ifdef CGAL_SURFACE_MESHER_DEBUG_CLIPPED_SEGMENT
          std::cerr << debug_point(surface, p1) << ", "
                    << debug_point(surface, p2) << "\n";
#endif
          // mid cannot be const, because it is modified below.
          Point mid = midpoint(p1, p2);
          const result_type value_at_mid = surf_equation(surface, mid);

          if ( squared_distance(p1, p2) < squared_distance_bound )
          // If the two points are close, then we must decide
          {
#ifdef CGAL_SURFACE_MESHER_DEBUG_CLIPPED_SEGMENT
            std::cerr << "=" << debug_point(surface, mid) << "\n";
#endif
            // the following function conditionally call
            // mid.set_on_surface(...) if mid has such a function.
            set_on_surface(mid,
                           surface_identifiers_generator(value_at_p1,
                                                         value_at_p2));

            visitor.new_point(mid);
            CGAL_SURFACE_MESHER_HISTOGRAM_PROFILER("Implificit_surface_oracle::Intersect_3::operator(Segment_3) bissection steps", steps)
            return make_object(mid);
          }

          // Else we must go on
          if ( value_at_p1 != value_at_mid )
          {
            p2 = mid;
            value_at_p2 = value_at_mid;
          }
          else
          {
            p1 = mid;
            value_at_p1 = value_at_mid;
          }
        }
      } // end intersect_clipped_segment

    }; // end nested class Intersect_3

    class Construct_initial_points
    {
      const Self& oracle;
    public:
      Construct_initial_points(const Self& oracle) : oracle(oracle)
      {
      }

      // Random points
      template <typename OutputIteratorPoints>
      OutputIteratorPoints operator() (const Surface_3& surface,
                                       OutputIteratorPoints out,
                                       int n = 20) // WARNING: why 20?
      {
        const Sphere_3& sphere = surface.bounding_sphere();
        const Point initial_center = GT().construct_center_3_object()(sphere);
        const FT squared_radius =
          GT().compute_squared_radius_3_object()(sphere);
        const FT radius = CGAL::sqrt(squared_radius);
        typename Self::Intersect_3 intersect = oracle.intersect_3_object();

        typename CGAL::Random_points_on_sphere_3<Point,
          Point_creator> random_point_on_sphere(CGAL::to_double(radius));
        typename CGAL::Random_points_in_sphere_3<Point,
          Point_creator> random_point_in_sphere(CGAL::to_double(radius));
        typename GT::Construct_segment_3 segment_3 =
          GT().construct_segment_3_object();
        typename GT::Construct_vector_3 vector_3 =
          GT().construct_vector_3_object();
        typename GT::Construct_translated_point_3 translate =
          GT().construct_translated_point_3_object();

        Point center = initial_center;
        while (n>0)
        {
          const Point p = translate(*random_point_on_sphere++,
                                    vector_3(CGAL::ORIGIN, initial_center));

#ifdef CGAL_SURFACE_MESHER_DEBUG_INITIAL_POINTS
          std::cerr << "test "
                    << intersect.debug_point(surface, center)
                    << ", " << intersect.debug_point(surface, p);
#endif

          Object o = intersect(surface, segment_3(center, p));
          if (const Point* intersection = object_cast<Point>(&o)) {
            *out++= *intersection;
            --n;
#ifdef CGAL_SURFACE_MESHER_DEBUG_INITIAL_POINTS
            std::cerr << " = "
                      << intersect.debug_point(surface, *intersection)
                      << "\n";
#endif
          }
          else
          {
            center = translate(*random_point_in_sphere++,
                               vector_3(CGAL::ORIGIN, initial_center));
#ifdef CGAL_SURFACE_MESHER_VERBOSE
            std::cerr << "Warning: new center " << center << "\n";
#endif
#ifdef CGAL_SURFACE_MESHER_DEBUG_INITIAL_POINTS
            std::cerr << " = null\n";
#endif
          }
        }
        return out;
      }
    }; // end nested class Construct_initial_points

    Construct_initial_points construct_initial_points_object() const
    {
      return Construct_initial_points(*this);
    }

    Intersect_3 intersect_3_object() const
    {
      return Intersect_3(visitor,
                         transform_functor,
                         surface_identifiers_generator);
    }

    bool is_in_volume(const Surface_3& surface, const Point& p) const
    {
      return transform_functor(surface(p)) != 0;
    }

  };  // end Implicit_surface_oracle_3

  }  // namespace Surface_mesher

} // namespace CGAL

#include <CGAL/enable_warnings.h>

#endif  // CGAL_SURFACE_MESHER_IMPLICIT_SURFACE_ORACLE_3_H
