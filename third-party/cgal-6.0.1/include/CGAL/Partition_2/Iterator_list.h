// Copyright (c) 2000  Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Partition_2/include/CGAL/Partition_2/Iterator_list.h $
// $Id: include/CGAL/Partition_2/Iterator_list.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Susan Hert <hert@mpi-sb.mpg.de>

#ifndef CGAL_ITERATOR_LIST_H
#define CGAL_ITERATOR_LIST_H

#include <CGAL/license/Partition_2.h>


#include <list>

namespace CGAL {

template <class Iterator>
class Iterator_list : public std::list<Iterator> {

public:
  Iterator_list() {}

  Iterator_list(Iterator first, Iterator beyond)
  {
      if (first == beyond) return;

      for (Iterator current = first; current != beyond; current++)
      {
         this->push_back(current);
      }
  }
};

}

#endif // CGAL_ITERATOR_LIST_H
