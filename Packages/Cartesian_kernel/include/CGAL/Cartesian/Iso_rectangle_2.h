// ======================================================================
//
// Copyright (c) 2000 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       :
// release_date  :
//
// file          : include/CGAL/Cartesian/Iso_rectangle_2.h
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Andreas Fabri, Herve Bronnimann
// coordinator   : INRIA Sophia-Antipolis (Mariette.Yvinec@sophia.inria.fr)
//
// ======================================================================

#ifndef CGAL_CARTESIAN_ISO_RECTANGLE_2_H
#define CGAL_CARTESIAN_ISO_RECTANGLE_2_H

CGAL_BEGIN_NAMESPACE

template <class R_>
class Iso_rectangleC2 CGAL_ADVANCED_KERNEL_PARTIAL_SPEC
  : public R_::Iso_rectangle_handle_2
{
  typedef typename R_::FT                        FT;

  typedef typename R_::Iso_rectangle_handle_2    base;
  typedef typename base::element_type            rep;

  typedef typename R_::Kernel_base::Point_2              Point_2;
  typedef typename R_::Kernel_base::Aff_transformation_2 Aff_transformation_2;

public:
  typedef R_                                     R;

  Iso_rectangleC2()
    : base(rep()) {}

  Iso_rectangleC2(const Point_2 &p, const Point_2 &q)
  { // FIXME : construction
    FT minx, maxx, miny, maxy;
    if (p.x() < q.x()) { minx = p.x(); maxx = q.x(); }
    else               { minx = q.x(); maxx = p.x(); }
    if (p.y() < q.y()) { miny = p.y(); maxy = q.y(); }
    else               { miny = q.y(); maxy = p.y(); }
    initialize_with(rep(Point_2(minx, miny),
	                                Point_2(maxx, maxy)));
  }

  Iso_rectangleC2(const FT& min_x, const FT& min_y, 
                  const FT& max_x, const FT& max_y)
  {
    initialize_with(rep(Point_2(min_x, min_y),
	                                Point_2(max_x, max_y)));
  }

  Iso_rectangleC2(const FT& min_hx, const FT& min_hy, 
                  const FT& max_hx, const FT& max_hy, const FT& hw)
  {
    if (hw == FT(1))
       initialize_with(rep(Point_2(min_hx, min_hy),
	                                   Point_2(max_hx, max_hy)));
    else
       initialize_with(rep(Point_2(min_hx/hw, min_hy/hw),
	                                   Point_2(max_hx/hw, max_hy/hw)));
  }

  bool            operator==(const Iso_rectangleC2 &s) const;
  bool            operator!=(const Iso_rectangleC2 &s) const;

  const Point_2 & min() const
  {
      return Ptr()->e0;
  }
  const Point_2 & max() const
  {
      return Ptr()->e1;
  }
  Point_2 vertex(int i) const;
  Point_2 operator[](int i) const;

  Iso_rectangleC2 transform(const Aff_transformation_2 &t) const
  {
    // FIXME : We need a precondition like this!!!
    // CGAL_kernel_precondition(t.is_axis_preserving());
    return Iso_rectangleC2(t.transform(vertex(0)), t.transform(vertex(2)));
  }

  Bounded_side    bounded_side(const Point_2 &p) const;
  bool            has_on_boundary(const Point_2 &p) const;
  bool            has_on_bounded_side(const Point_2 &p) const;
  bool            has_on_unbounded_side(const Point_2 &p) const;

  bool            is_degenerate() const;

  Bbox_2          bbox() const;

  const FT &      xmin() const;
  const FT &      ymin() const;
  const FT &      xmax() const;
  const FT &      ymax() const;
  const FT &      min_coord(int i) const;
  const FT &      max_coord(int i) const;

  FT              area() const;
};

#ifdef CGAL_CFG_TYPENAME_BUG
#define typename
#endif

template < class R >
inline
bool
Iso_rectangleC2<R CGAL_CTAG>::
operator==(const Iso_rectangleC2<R CGAL_CTAG> &r) const
{
  if (identical(r))
      return true;
  return vertex(0) == r.vertex(0) && vertex(2) == r.vertex(2);
}

template < class R >
inline
bool
Iso_rectangleC2<R CGAL_CTAG>::
operator!=(const Iso_rectangleC2<R CGAL_CTAG> &r) const
{
  return !(*this == r);
}

template < class R >
inline
const typename Iso_rectangleC2<R CGAL_CTAG>::FT &
Iso_rectangleC2<R CGAL_CTAG>::xmin() const
{
  return min().x();
}

template < class R >
inline
const typename Iso_rectangleC2<R CGAL_CTAG>::FT &
Iso_rectangleC2<R CGAL_CTAG>::ymin() const
{
  return min().y();
}

template < class R >
inline
const typename Iso_rectangleC2<R CGAL_CTAG>::FT &
Iso_rectangleC2<R CGAL_CTAG>::xmax() const
{
  return max().x();
}

template < class R >
inline
const typename Iso_rectangleC2<R CGAL_CTAG>::FT &
Iso_rectangleC2<R CGAL_CTAG>::ymax() const
{
  return max().y();
}

template < class R >
inline
const typename Iso_rectangleC2<R CGAL_CTAG>::FT &
Iso_rectangleC2<R CGAL_CTAG>::min_coord(int i) const
{
  CGAL_kernel_precondition( i == 0 || i == 1 );
  if (i == 0)
     return xmin();
  else
     return ymin();
}

template < class R >
inline
const typename Iso_rectangleC2<R CGAL_CTAG>::FT &
Iso_rectangleC2<R CGAL_CTAG>::max_coord(int i) const
{
  CGAL_kernel_precondition( i == 0 || i == 1 );
  if (i == 0)
     return xmax();
  else
     return ymax();
}

template < class R >
typename Iso_rectangleC2<R CGAL_CTAG>::Point_2
Iso_rectangleC2<R CGAL_CTAG>::vertex(int i) const
{
  switch (i%4) {
  case 0: return min();
  case 1: return Point_2(xmax(), ymin());
  case 2: return max();
  default: return Point_2(xmin(), ymax());
  }
}

template < class R >
inline
typename Iso_rectangleC2<R CGAL_CTAG>::Point_2
Iso_rectangleC2<R CGAL_CTAG>::operator[](int i) const
{
  return vertex(i);
}

template < class R >
inline
typename Iso_rectangleC2<R CGAL_CTAG>::FT
Iso_rectangleC2<R CGAL_CTAG>::area() const
{
  return (xmax()-xmin()) * (ymax()-ymin());
}

template < class R >
CGAL_KERNEL_MEDIUM_INLINE
Bounded_side
Iso_rectangleC2<R CGAL_CTAG>::
bounded_side(const typename Iso_rectangleC2<R CGAL_CTAG>::Point_2 &p) const
{ // FIXME : predicate
  bool x_incr = (xmin() < p.x()) && (p.x() < xmax()),
       y_incr = (ymin() < p.y()) && (p.y() < ymax());
  if (x_incr)
    {
      if (y_incr)
          return ON_BOUNDED_SIDE;
      if ( (p.y() == ymin()) || (ymax() == p.y()) )
          return ON_BOUNDARY;
    }
  if ( (p.x() == xmin()) || (xmax() == p.x()) )
      if ( y_incr || (p.y() == ymin()) || (ymax() == p.y()) )
          return ON_BOUNDARY;

  return ON_UNBOUNDED_SIDE;
}

template < class R >
inline
bool
Iso_rectangleC2<R CGAL_CTAG>::
has_on_boundary(const typename Iso_rectangleC2<R CGAL_CTAG>::Point_2 &p) const
{
  return bounded_side(p) == ON_BOUNDARY;
}

template < class R >
inline
bool
Iso_rectangleC2<R CGAL_CTAG>::
has_on_bounded_side(const typename Iso_rectangleC2<R CGAL_CTAG>::Point_2 &p)
    const
{
  return bounded_side(p) == ON_BOUNDED_SIDE;
}

template < class R >
inline
bool
Iso_rectangleC2<R CGAL_CTAG>::
has_on_unbounded_side(const typename Iso_rectangleC2<R CGAL_CTAG>::Point_2 &p)
    const
{
  return bounded_side(p) == ON_UNBOUNDED_SIDE;
}

template < class R >
inline
bool
Iso_rectangleC2<R CGAL_CTAG>::is_degenerate() const
{
  return (xmin() == xmax()) || (ymin() == ymax());
}

template < class R >
inline
Bbox_2
Iso_rectangleC2<R CGAL_CTAG>::bbox() const
{ // FIXME : to_interval
  return Bbox_2(CGAL::to_double(xmin()), CGAL::to_double(ymin()),
                CGAL::to_double(xmax()), CGAL::to_double(ymax()));
}

#ifndef CGAL_NO_OSTREAM_INSERT_ISO_RECTANGLEC2
template < class R >
std::ostream &
operator<<(std::ostream &os, const Iso_rectangleC2<R CGAL_CTAG> &r)
{
    switch(os.iword(IO::mode)) {
    case IO::ASCII :
        return os << r[0] << ' ' << r[2];
    case IO::BINARY :
        return os << r[0] << r[2];
    default:
        return os << "Iso_rectangleC2(" << r[0] << ", " << r[2] << ")";
    }
}
#endif // CGAL_NO_OSTREAM_INSERT_ISO_RECTANGLEC2

#ifndef CGAL_NO_ISTREAM_EXTRACT_ISO_RECTANGLEC2
template < class R >
CGAL_KERNEL_MEDIUM_INLINE
std::istream &
operator>>(std::istream &is, Iso_rectangleC2<R CGAL_CTAG> &r)
{
    typename Iso_rectangleC2<R CGAL_CTAG>::Point_2 p, q;

    is >> p >> q;

    if (is)
	r = Iso_rectangleC2<R CGAL_CTAG>(p, q);
    return is;
}
#endif // CGAL_NO_ISTREAM_EXTRACT_ISO_RECTANGLEC2

#ifdef CGAL_CFG_TYPENAME_BUG
#undef typename
#endif

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_ISO_RECTANGLE_2_H
