/**
 * @file   segment.hpp
 * @brief  Geometric Data Structure: 2D segment type
 * @author Hao Chen
 * @date   09/05/2019
 *
 **/

#ifndef _GEO_SEGMENT_HPP_
#define _GEO_SEGMENT_HPP_

#include "src/global/namespace.hpp"
#include "point.hpp"

PROJECT_NAMESPACE_START

template<typename T>
class Segment {
public:
  Segment(T x1 = 0, T y1 = 0, T x2 = 0, T y2 = 0)
    : _p1(x1, y1), _p2(x2, y2) { check(); }
  Segment(const Point<T>& p1, const Point<T>& p2)
    : _p1(p1), _p2(p2) { check(); }
  ~Segment() {}

  // Basic setting functions
  void check()                  { if (_p1.x() > _p2.x()) std::swap(_p1, _p2); }
  void setP1(const Point<T> p)  { _p1 = p; check(); }
  void setP2(const Point<T> p)  { _p2 = p; check(); }

  // Basic access functions
  Point<T>&        p1()           { return _p1; }
  const Point<T>&  p1()     const { return _p1; }
  Point<T>&        p2()           { return _p2; }
  const Point<T>&  p2()     const { return _p2; }

  T         length()        const { return Point<T>::Mdistance(_p1, _p2); }
  T         xl()            const { return std::min(_p1.x(), _p2.x()); }
  T         xh()            const { return std::max(_p1.x(), _p2.x()); }
  T         yl()            const { return std::min(_p1.y(), _p2.y()); }
  T         yh()            const { return std::max(_p1.y(), _p2.y()); }
  T         centerX()       const { return (xl() + xh()) / 2; }
  T         centerY()       const { return (yl() + yh()) / 2; }
  Point<T>  center()        const { return Point<T>(centerX(), centerY()); }
  Point<T>  min_corner()    const { return Point<T>(xl(), yl()); }
  Point<T>  max_corner()    const { return Point<T>(xh(), yh()); }
  bool      bHor()          const { return _p1.x() != _p2.x() && _p1.y() == _p2.y(); }
  bool      bVer()          const { return _p1.x() == _p2.x() && _p1.y() != _p2.y(); }
  bool      b90()           const { return bHor() or bVer(); }

  // util functions
  void shiftX(const T x)               { _p1.shiftX(x); _p2.shiftX(x); check(); }
  void shiftY(const T y)               { _p1.shiftY(y); _p2.shiftY(y); check(); }
  void shiftXY(const T x, const T y)   { shiftX(x); shiftY(y); check(); }

  // operators
  friend std::ostream&  operator <<  (std::ostream& os, const Segment& s)         { os << s._p1 << ' ' << s._p2; return os; }
  bool                  operator ==  (const Segment& s)                    const  { return _p1 == s._p1 && _p2 == s._p2; }
  bool                  operator !=  (const Segment& s)                    const  { return !(*this == s); }
  void                  operator =   (const Segment& s)                           { _p1 = s._p1; _p2 = s._p2; check(); }
  bool                  operator <   (const Segment& s) const {
    if (_p1.x() != s.p1().x()) return _p1.x() < s.p1().x();
    else if (_p1.y() != s.p1().y()) return _p1.y() < s.p1().y();
    else if (_p2.x() != s.p2().x()) return _p2.x() < s.p2().x();
    else return _p2.y() < s.p2().y();
  }
  
  // static funcs
  static bool  bConnect(const Segment& s1, const Segment& s2);

private:
  Point<T> _p1, _p2;  
};

template<typename T>
bool Segment<T>::bConnect(const Segment& s1, const Segment& s2) {
  Int_t o1 = Point<T>::orientation(s1.p1(), s1.p2(), s2.p1());
  Int_t o2 = Point<T>::orientation(s1.p1(), s1.p2(), s2.p2());
  Int_t o3 = Point<T>::orientation(s2.p1(), s2.p2(), s1.p1());
  Int_t o4 = Point<T>::orientation(s2.p1(), s2.p2(), s1.p2());

  // general case
  if (o1 != o2 and o3 != o4)
    return true;

  // special cases
  if (o1 == 0 and Point<T>::bOnSegment(s1.p1(), s2.p1(), s1.p2()))
    return true; 
  if (o2 == 0 and Point<T>::bOnSegment(s1.p1(), s2.p2(), s1.p2()))
    return true; 
  if (o3 == 0 and Point<T>::bOnSegment(s2.p1(), s1.p1(), s2.p2()))
    return true; 
  if (o4 == 0 and Point<T>::bOnSegment(s2.p1(), s1.p2(), s2.p2()))
    return true; 

  return false;
}


PROJECT_NAMESPACE_END

#endif /// _GEO_SEGMENT_HPP_
