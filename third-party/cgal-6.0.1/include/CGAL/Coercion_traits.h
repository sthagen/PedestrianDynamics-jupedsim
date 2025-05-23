// Copyright (c) 2006-2007 Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Algebraic_foundations/include/CGAL/Coercion_traits.h $
// $Id: include/CGAL/Coercion_traits.h 50cfbde3b84 $
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Michael Hemmer    <hemmer@mpi-inf.mpg.de>
//
// =============================================================================

/*! \file NiX/Coercion_traits.h
 *  \brief defines class NiX::Coercion_traits.
 *
 *  Provides the general definition of the \c Coercion_traits<A,B> class, with
 *  specializations for the builtin number types.
 */

#ifndef CGAL_COERCION_TRAITS_H
#define CGAL_COERCION_TRAITS_H 1

#include <iterator>
#include <type_traits>

#include <CGAL/boost/iterator/transform_iterator.hpp>

#include <CGAL/tags.h>

// Macro to define an additional operator for binary functors which takes
// two number types as parameters that are interoperable with the
// number type
#define CGAL_IMPLICIT_INTEROPERABLE_BINARY_OPERATOR_WITH_RT( NT, Result_type  ) \
  template < class CT_Type_1, class CT_Type_2 >                         \
  Result_type operator()( const CT_Type_1& x, const CT_Type_2& y ) const { \
    static_assert(::std::is_same<                              \
            typename Coercion_traits< CT_Type_1, CT_Type_2 >::Type, NT  \
            >::value) ;                                                 \
                                                                        \
    typename Coercion_traits< CT_Type_1, CT_Type_2 >::Cast cast;        \
    return operator()( cast(x), cast(y) );                              \
  }

#define CGAL_IMPLICIT_INTEROPERABLE_BINARY_OPERATOR( NT ) \
CGAL_IMPLICIT_INTEROPERABLE_BINARY_OPERATOR_WITH_RT( NT, NT )

#define CGAL_DEFINE_COERCION_TRAITS_FROM_TO(FROM,TO)                    \
    template <>                                                         \
    struct Coercion_traits< FROM , TO >{                                \
        typedef Tag_true  Are_explicit_interoperable;                   \
        typedef Tag_true  Are_implicit_interoperable;                   \
        typedef TO Type;                                       \
        struct Cast{                                                    \
            typedef Type result_type;                          \
            Type operator()(const TO& x)   const { return x;}  \
            Type operator()(const FROM& x) const {             \
                return Type(x);}                               \
        };                                                              \
    };                                                                  \
    template <>                                                         \
    struct Coercion_traits< TO , FROM >{                                \
        typedef Tag_true  Are_explicit_interoperable;                   \
        typedef Tag_true  Are_implicit_interoperable;                   \
        typedef TO Type;                                       \
        struct Cast{                                                    \
            typedef Type result_type;                          \
            Type operator()(const TO& x)   const { return x;}  \
            Type operator()(const FROM& x) const {             \
                return Type(x);}                               \
        };                                                              \
    };

#define CGAL_DEFINE_COERCION_TRAITS_FROM_TO_TEM(FROM,TO,TEM)            \
    template <TEM>                                                      \
    struct Coercion_traits< FROM , TO >{                                \
        typedef Tag_true  Are_explicit_interoperable;                   \
        typedef Tag_true  Are_implicit_interoperable;                   \
        typedef TO Type;                                       \
        struct Cast{                                                    \
            typedef Type result_type;                          \
            Type operator()(const TO& x)   const { return x;}  \
            Type operator()(const FROM& x) const {             \
                return Type(x);}                               \
        };                                                              \
    };                                                                  \
    template <TEM>                                                      \
    struct Coercion_traits< TO , FROM >{                                \
        typedef Tag_true  Are_explicit_interoperable;                   \
        typedef Tag_true  Are_implicit_interoperable;                   \
        typedef TO Type;                                       \
        struct Cast{                                                    \
            typedef Type result_type;                          \
            Type operator()(const TO& x)   const { return x;}  \
            Type operator()(const FROM& x) const {             \
                return Type(x);}                               \
        };                                                              \
    };



#define CGAL_DEFINE_COERCION_TRAITS_FOR_SELF(A)                         \
    template <>                                                         \
    struct Coercion_traits< A , A >{                                    \
        typedef Tag_true  Are_explicit_interoperable;                   \
        typedef Tag_true  Are_implicit_interoperable;                   \
        typedef A Type;                                        \
        struct Cast{                                                    \
            typedef Type result_type;                          \
            Type operator()(const A& x) const { return x;}     \
        };                                                              \
    };

#define CGAL_DEFINE_COERCION_TRAITS_FOR_SELF_TEM(A,TEM)                 \
    template <TEM>                                                      \
    struct Coercion_traits< A , A >{                                    \
        typedef Tag_true  Are_explicit_interoperable;                   \
        typedef Tag_true  Are_implicit_interoperable;                   \
        typedef A Type;                                        \
        struct Cast{                                                    \
            typedef Type result_type;                          \
            Type operator()(const A& x) const {return x;}      \
        };                                                              \
    };

namespace CGAL {


namespace INTERN_CT{
template< class FROM, class TO >struct Cast_from_to{
    typedef TO result_type;
    TO operator()(const TO& x) const {return x;}
    TO operator()(const FROM& x) const {return TO(x);}
};
template< class TO>
struct Cast_from_to<TO,TO>{
    typedef TO result_type;
    TO operator()(const TO& x) const {return x;}
};
}


template<class A , class B> struct Coercion_traits;
template<class A , class B, int > struct Coercion_traits_for_level;



CGAL_DEFINE_COERCION_TRAITS_FROM_TO(short,int)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(short,long)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(short,long long)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(short,float)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(short,double)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(short,long double)

CGAL_DEFINE_COERCION_TRAITS_FROM_TO(int,long)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(int,long long)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(int,float)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(int,double)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(int,long double)

CGAL_DEFINE_COERCION_TRAITS_FROM_TO(long,long long)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(long,float)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(long,double)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(long,long double)


CGAL_DEFINE_COERCION_TRAITS_FROM_TO(long long,float)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(long long,double)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(long long,long double)

CGAL_DEFINE_COERCION_TRAITS_FROM_TO(float,double)
CGAL_DEFINE_COERCION_TRAITS_FROM_TO(float,long double)

CGAL_DEFINE_COERCION_TRAITS_FROM_TO(double,long double)

//! Specialization for equal types.
template <class A>
struct Coercion_traits<A,A>{
    typedef Tag_true Are_explicit_interoperable;
    typedef Tag_true Are_implicit_interoperable;
    typedef A Type;
    struct Cast{
        typedef Type result_type;
        Type inline operator()(const A& x) const {
            return x;
        }
    };
};

CGAL_DEFINE_COERCION_TRAITS_FOR_SELF(short)
CGAL_DEFINE_COERCION_TRAITS_FOR_SELF(int)
CGAL_DEFINE_COERCION_TRAITS_FOR_SELF(long)
CGAL_DEFINE_COERCION_TRAITS_FOR_SELF(long long)
CGAL_DEFINE_COERCION_TRAITS_FOR_SELF(float)
CGAL_DEFINE_COERCION_TRAITS_FOR_SELF(double)
CGAL_DEFINE_COERCION_TRAITS_FOR_SELF(long double)

enum COERCION_TRAITS_LEVEL {
    CTL_TOP          = 4,
    CTL_POLYNOMIAL   = 4,
    CTL_COMPLEX      = 3,
    CTL_INTERVAL     = 2,
    CTL_SQRT_EXT     = 1
};

template <class A, class B, int i >
struct Coercion_traits_for_level: public Coercion_traits_for_level<A,B,i-1>{};

template <class A, class B>
struct Coercion_traits_for_level<A,B,0> {
    typedef Tag_false Are_explicit_interoperable;
    typedef Tag_false Are_implicit_interoperable;
//    typedef Null_type               Type;
    typedef Null_functor Cast;
};

template<class A , class B>
struct Coercion_traits :public Coercion_traits_for_level<A,B,CTL_TOP>{};


} //namespace CGAL

#endif //NiX_COERCION_TRAITS_H
