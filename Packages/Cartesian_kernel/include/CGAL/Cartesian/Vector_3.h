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
// file          : include/CGAL/Cartesian/Vector_3.h
// revision      : $Revision$
// revision_date : $Date$
// author        : Andreas Fabri
// coordinator   : INRIA Sophia-Antipolis (Mariette.Yvinec@sophia.inria.fr)
//
// ======================================================================

#ifndef CGAL_CARTESIAN_VECTOR_3_H
#define CGAL_CARTESIAN_VECTOR_3_H

CGAL_BEGIN_NAMESPACE

template < class R_ >
class VectorC3 CGAL_ADVANCED_KERNEL_PARTIAL_SPEC
  : public R_::Vector_handle_3
{
  typedef typename R_::FT                        FT;

  typedef typename R_::Vector_handle_3	   	 base;
  typedef typename base::element_type   	 rep;

  typedef typename R_::Kernel_base::Point_3              Point_3;
  typedef typename R_::Kernel_base::Direction_3          Direction_3;
  typedef typename R_::Kernel_base::Aff_transformation_3 Aff_transformation_3;

public:
  typedef R_                                     R;

  VectorC3()
    : base(rep()) {}

  VectorC3(const Null_vector &)
    : base(rep(FT(0), FT(0), FT(0))) {}

  VectorC3(const Point_3 &p)
    : base(p) {}

  VectorC3(const Point_3 &a, const Point_3 &b)
    : base(b-a) {}

  VectorC3(const Direction_3 &d)
    : base(d) {}

  VectorC3(const FT &x, const FT &y, const FT &z)
    : base(rep(x, y, z)) {}

  VectorC3(const FT &x, const FT &y, const FT &z, const FT &w)
  {
    if (w != FT(1))
      initialize_with(rep(x/w, y/w, z/w));
    else
      initialize_with(rep(x, y, z));
  }

  bool operator==(const VectorC3 &p) const;
  bool operator!=(const VectorC3 &p) const;

  bool operator==(const Null_vector &) const;
  bool operator!=(const Null_vector &) const;

  const FT & x() const
  {
      return Ptr()->e0;
  }
  const FT & y() const
  {
      return Ptr()->e1;
  }
  const FT & z() const
  {
      return Ptr()->e2;
  }

  const FT & hx() const
  {
      return x();
  }
  const FT & hy() const
  {
      return y();
  }
  const FT & hz() const
  {
      return z();
  }
  FT hw() const
  {
      return FT(1);
  }

  const FT & cartesian(int i) const;
  const FT & operator[](int i) const;
  FT homogeneous(int i) const;

  int dimension() const
  {
      return 3;
  }

  VectorC3 operator+(const VectorC3 &w) const;
  VectorC3 operator-(const VectorC3 &w) const;
  VectorC3 operator-() const;
  VectorC3 operator/(const FT &c) const;
  FT operator*(const VectorC3 &w) const;
  FT squared_length() const;
  Direction_3 direction() const;
  VectorC3 transform(const Aff_transformation_3 &t) const
  {
    return t.transform(*this);
  }
};

#ifdef CGAL_CFG_TYPENAME_BUG
#define typename
#endif

template < class R >
inline
bool
VectorC3<R CGAL_CTAG>::operator==(const VectorC3<R CGAL_CTAG> &v) const
{
  if (identical(v))
      return true;
  return x() == v.x() && y() == v.y() && z() == v.z();
}

template < class R >
inline
bool
VectorC3<R CGAL_CTAG>::operator!=(const VectorC3<R CGAL_CTAG> &v) const
{
  return !(*this == v);
}

template < class R >
inline
bool
VectorC3<R CGAL_CTAG>::operator==(const Null_vector &) const
{
  return CGAL_NTS is_zero(x()) && CGAL_NTS is_zero(y()) &&
         CGAL_NTS is_zero(z());
}

template < class R >
inline
bool
VectorC3<R CGAL_CTAG>::operator!=(const Null_vector &v) const
{
  return !(*this == v);
}

template < class R >
inline
const typename VectorC3<R CGAL_CTAG>::FT &
VectorC3<R CGAL_CTAG>::cartesian(int i) const
{
  CGAL_kernel_precondition( (i>=0) && (i<3) );
  if (i==0) return x();
  if (i==1) return y();
  return z();
}

template < class R >
inline
const typename VectorC3<R CGAL_CTAG>::FT &
VectorC3<R CGAL_CTAG>::operator[](int i) const
{
  return cartesian(i);
}

template < class R >
typename VectorC3<R CGAL_CTAG>::FT
VectorC3<R CGAL_CTAG>::homogeneous(int i) const
{
  if (i==3) return FT(1);
  return cartesian(i);
}

template < class R >
inline
VectorC3<R CGAL_CTAG>
VectorC3<R CGAL_CTAG>::
operator+(const VectorC3<R CGAL_CTAG> &w) const
{
  return VectorC3<R CGAL_CTAG>(x() + w.x(), y() + w.y(), z() + w.z());
}

template < class R >
inline
VectorC3<R CGAL_CTAG>
VectorC3<R CGAL_CTAG>::operator-(const VectorC3<R CGAL_CTAG> &w) const
{
  return VectorC3<R CGAL_CTAG>(x() - w.x(), y() - w.y(), z() - w.z());
}

template < class R >
inline
VectorC3<R CGAL_CTAG>
VectorC3<R CGAL_CTAG>::operator-() const
{
  return VectorC3<R CGAL_CTAG>(-x(), -y(), -z());
}

template < class R >
inline
typename VectorC3<R CGAL_CTAG>::FT
VectorC3<R CGAL_CTAG>::operator*(const VectorC3<R CGAL_CTAG> &w) const
{
  return x() * w.x() + y() * w.y() + z() * w.z();
}

template < class R >
inline
typename VectorC3<R CGAL_CTAG>::FT
VectorC3<R CGAL_CTAG>::squared_length() const
{
  return CGAL_NTS square(x()) + CGAL_NTS square(y()) + CGAL_NTS square(z());
}

template < class R >
inline
VectorC3<R CGAL_CTAG>
VectorC3<R CGAL_CTAG>::
operator/(const typename VectorC3<R CGAL_CTAG>::FT &c) const
{
  return VectorC3<R CGAL_CTAG>(x()/c, y()/c, z()/c);
}

template < class R >
inline
typename VectorC3<R CGAL_CTAG>::Direction_3
VectorC3<R CGAL_CTAG>::direction() const
{
  return Direction_3(*this);
}

#ifndef CGAL_CARTESIAN_NO_OSTREAM_INSERT_VECTORC3
template < class R >
std::ostream &
operator<<(std::ostream &os, const VectorC3<R CGAL_CTAG> &v)
{
  switch(os.iword(IO::mode)) {
    case IO::ASCII :
      return os << v.x() << ' ' << v.y()  << ' ' << v.z();
    case IO::BINARY :
      write(os, v.x());
      write(os, v.y());
      write(os, v.z());
      return os;
    default:
      os << "VectorC3(" << v.x() << ", " << v.y() <<  ", " << v.z() << ")";
      return os;
  }
}
#endif // CGAL_CARTESIAN_NO_OSTREAM_INSERT_VECTORC3

#ifndef CGAL_CARTESIAN_NO_ISTREAM_EXTRACT_VECTORC3
template < class R >
std::istream &
operator>>(std::istream &is, VectorC3<R CGAL_CTAG> &p)
{
  typename R::FT x, y, z;
  switch(is.iword(IO::mode)) {
    case IO::ASCII :
      is >> x >> y >> z;
      break;
    case IO::BINARY :
      read(is, x);
      read(is, y);
      read(is, z);
      break;
    default:
      std::cerr << "" << std::endl;
      std::cerr << "Stream must be in ascii or binary mode" << std::endl;
      break;
  }
  if (is)
      p = VectorC3<R CGAL_CTAG>(x, y, z);
  return is;
}
#endif // CGAL_CARTESIAN_NO_ISTREAM_EXTRACT_VECTORC3

#ifdef CGAL_CFG_TYPENAME_BUG
#undef typename
#endif

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_VECTOR_3_H
