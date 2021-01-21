/**
 * @file   point3d.hpp
 * @brief  Geometric Data Structure: 3D point type
 * @author Hao Chen
 * @date   09/02/2019
 *
 **/

#ifndef _GEO_POINT3D_HPP_
#define _GEO_POINT3D_HPP_

#include <boost/geometry.hpp>
#include <boost/functional/hash.hpp>
#include "src/global/namespace.hpp"

PROJECT_NAMESPACE_START

template<typename T>
class Point3d {
 public:
  Point3d(T x = 0, T y = 0, T z = 0) : _d{x, y, z} {}
  Point3d(const Point3d& p) { memcpy(_d, p.d(), 3 * sizeof(T)); }
  ~Point3d() {}

  // Basic setting functions
  void set(const T v, const int i)                 { _d[i] = v;                           }
  void setX(const T x)                             { _d[0] = x;                           }
  void setY(const T y)                             { _d[1] = y;                           }
  void setZ(const T z)                             { _d[2] = z;                           }
  void setXYZ(const T x, const T y, const T z)     { _d[0] = x; _d[1] = y; _d[2] = z;     }
  void shift(const T v, const int i)               { _d[i] += v;                          }
  void shiftX(const T x)                           { _d[0] += x;                          }
  void shiftY(const T y)                           { _d[1] += y;                          }
  void shiftZ(const T z)                           { _d[2] += z;                          }
  void shiftXYZ(const T x, const T y, const T z)   { _d[0] += x; _d[1] += y; _d[2] += z;  }

  // Basic access functions
  T  x() const              { return _d[0]; }
  T  y() const              { return _d[1]; }
  T  z() const              { return _d[2]; }
  T* d()                    { return _d;    }
  T const *d()        const { return _d;    }
  T  val(const int i) const { assert(i >= 0 && i < 3); return _d[i]; }
  T& val(const int i)       { assert(i >= 0 && i < 3); return _d[i]; }

  // util functions
  static T Mdistance(const Point3d& p1, const Point3d& p2) {
    return (p1._d[0] > p2._d[0] ? p1._d[0] - p2._d[0] : p2._d[0] - p1._d[0]) + 
      (p1._d[1] > p2._d[1] ? p1._d[1] - p2._d[1] : p2._d[1] - p1._d[1]) +
      (p1._d[2] > p2._d[2] ? p1._d[2] - p2._d[2] : p2._d[2] - p1._d[2]);
  }
  static T dot(const Point3d& p1, const Point3d& p2) {
    return p1.x() * p2.x() + p1.y() * p2.y() + p1.z() * p2.z();
  }
  static T norm(const Point3d& p) {
    return p.x() * p.x() + p.y() * p.y() + p.z() * p.z();
  }
  static Point3d cross(const Point3d& p1, const Point3d& p2) {
    return Point3d(p1.y() * p2.z() - p2.y() * p1.z(), 
                   p1.z() * p2.x() - p2.z() * p1.x(), 
                   p1.x() * p2.y() - p2.x() * p1.y());
  }

  void flipX(const T x) { _d[0] = 2 * x - _d[0]; }
  void flipY(const T y) { _d[1] = 2 * y - _d[1]; }
  void flipZ(const T z) { _d[2] = 2 * z - _d[2]; }

  // operators
  friend std::ostream&  operator <<  (std::ostream& os, const Point3d& p)       { os << '(' << p._d[0] << ' ' << p._d[1] << ' ' << p._d[2] << ')'; return os; }
  bool                  operator ==  (const Point3d& p)                   const { return _d[0] == p._d[0] && _d[1] == p._d[1] && _d[2] == p._d[2]; }
  bool                  operator !=  (const Point3d& p)                   const { return !(*this == p); }
  bool                  operator <   (const Point3d& p)                   const { if (_d[2] != p._d[2]) return _d[2] < p._d[2]; else if (_d[0] != p._d[0]) return _d[0] < p._d[0]; else return _d[1] < p._d[1]; }
  void                  operator =   (const Point3d& p)                         { setXYZ(p._d[0], p._d[1], p._d[2]); }
  Point3d               operator +   (const Point3d& p)                   const { return Point3d(_d[0] + p._d[0], _d[1] + p._d[1], _d[2] + p._d[2]); }
  Point3d               operator -   (const Point3d& p)                   const { return Point3d(_d[0] - p._d[0], _d[1] - p._d[1], _d[2] + p._d[2]); }

  // comparators
  struct Compare {
    bool operator() (const Point3d& p1, const Point3d& p2) const {
      if (p1.z() != p2.z()) return p1.z() < p2.z();
      else if (p1.x() != p2.x()) return p1.x() < p2.x();
      else return p1.y() < p2.y();
    }
  };
  // hasher
  struct hasher {
    std::size_t operator() (const Point3d<T> &p) const {
      return boost::hash_range(p.d(), p.d()+3);
    }
  };
  // string
  std::string str() const 
  {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

 private:
  T _d[3];
};

PROJECT_NAMESPACE_END

#include <parallel_hashmap/phmap_utils.h>
namespace std {
  template<typename CoordType>
  struct hash<PROJECT_NAMESPACE::Point3d<CoordType>> {
    std::size_t operator() (const PROJECT_NAMESPACE::Point3d<CoordType>& p) const {
      return phmap::HashState().combine(0, p.x(), p.y(), p.z());
    }
  };
}

namespace boost { namespace geometry { namespace traits {
  template<typename CoordType>
  struct tag<PROJECT_NAMESPACE::Point3d<CoordType>> {
    typedef point_tag type;
  };

  template<typename CoordType>
  struct coordinate_type<PROJECT_NAMESPACE::Point3d<CoordType>> {
    typedef CoordType type;
  };

  template<typename CoordType>
  struct coordinate_system<PROJECT_NAMESPACE::Point3d<CoordType>> {
    typedef boost::geometry::cs::cartesian type;
  };
  
  template<typename CoordType>
  struct dimension<PROJECT_NAMESPACE::Point3d<CoordType>>
    : boost::mpl::int_<3> {};

  template<typename CoordType>
  struct access<PROJECT_NAMESPACE::Point3d<CoordType>, 0> {
    static inline CoordType get(const PROJECT_NAMESPACE::Point3d<CoordType>& p) {
      return p.x();
    }
    static inline void set(PROJECT_NAMESPACE::Point3d<CoordType>& p, CoordType const& value) {
      p.setX(value);
    }
  };

  template<typename CoordType>
  struct access<PROJECT_NAMESPACE::Point3d<CoordType>, 1> {
    static inline CoordType get(const PROJECT_NAMESPACE::Point3d<CoordType>& p) {
      return p.y();
    }
    static inline void set(PROJECT_NAMESPACE::Point3d<CoordType> & p, CoordType const& value) {
      p.setY(value);
    }
  };

  template<typename CoordType>
  struct access<PROJECT_NAMESPACE::Point3d<CoordType>, 2> {
    static inline CoordType get(PROJECT_NAMESPACE::Point3d<CoordType>  const& p) {
      return p.z();
    }
    static inline void set(PROJECT_NAMESPACE::Point3d<CoordType> & p, CoordType const& value) {
      p.setZ(value);
    }
  };

}}}

#endif /// _GEO_POINT3D_HPP_

