/**
 * @file   box.hpp
 * @brief  Geometric Data Structure: Box type
 * @author Hao Chen
 * @date   09/02/2019
 *
 **/

#ifndef ANAROUTE_BOX_HPP_
#define ANAROUTE_BOX_HPP_

#include "src/global/global.hpp"

PROJECT_NAMESPACE_START

template<typename T>
class Box {
public:
  Box(T left = 0, T bottom = 0, T right = 0, T top = 0)
    : _bl(left, bottom), _tr(right, top) {
    assert(left <= right && bottom <= top);
  }
  Box(const Point<T>& p1, const Point<T>& p2) {
    assert(p1.x() <= p2.x() && p1.y() <= p2.y());
    _bl = p1;
    _tr = p2;
  }
  ~Box() {}

  // Basic setting functions
  void  setL(T l)                      { _bl.setX(l); }
  void  setR(T r)                      { _tr.setX(r); }
  void  setB(T b)                      { _bl.setY(b); }
  void  setT(T t)                      { _tr.setY(t); }
  void  setBounds(T l, T b, T r, T t)  { setL(l); setB(b); setR(r); setT(t); }
  // Basic access functions
  T          left()             const { return _bl.x(); }
  T          bottom()           const { return _bl.y(); }
  T          right()            const { return _tr.x(); }
  T          top()              const { return _tr.y(); }
  T          width()            const { return right() - left(); }
  T          height()           const { return top() - bottom(); }
  T          centerX()          const { return (left() + right()) / 2; }
  T          centerY()          const { return (bottom() + top()) / 2; }
  T          hpwl()             const { return width() + height(); }
  T          perimeter()        const { return 2 * hpwl(); }
  T          area()             const { return width() * height(); }
  
  // Points
  Point<T>&         bl()               { return _bl; }
  const Point<T>&   bl()         const { return _bl; }
  Point<T>&         tr()               { return _tr; }
  const Point<T>&   tr()         const { return _tr; }
  Point<T>&         min_corner()       { return _bl; }
  const Point<T>&   min_corner() const { return _bl; }
  Point<T>&         max_corner()       { return _tr; }
  const Point<T>&   max_corner() const { return _tr; }
  Point<T>          center()     const { return Point<T>(centerX(), centerY());  }

  // utils
  void shiftX(const T x);
  void shiftY(const T y);
  void shift(const T x, const T y);
  void rotate90(const T x, const T y, const bool bClockWise); // rotate 90 degree with respect to (x, y)
  void rotate180(const T x, const T y);                       // rotate 180 degree with respect to (x, y)
  void flipX(const T x);                                      // flip by line x = x
  void flipY(const T y);                                      // flip by line y = y
  void expand(const T s);
  void expand(const T s, const int dim);
  void expandX(const T s);
  void expandY(const T s);
  void shrink(const T s);
  void shrinkX(const T s);
  void shrinkY(const T s);
  void difference(const Box& r, std::vector<Box>& result);
  void multi_diff(const std::vector<Box>& vRects, std::vector<Box>& result) const;
  void multi_diff(const std::list<const Box*>& vRects, std::vector<Box>& result) const;
  void multi_diff(const std::list<Index_t>& ord, const std::vector<Box>& vRects, std::vector<Box>& result) const;

  //static functions
  static T     Mdistance(const Box& box1, const Box& box2);
  static T     Mcenterdistance(const Box& box1, const Box& box2);
  static T     Mdistance(const Box& box1, const Point<T>& pt);
  static bool  bOverlap(const Box& box1, const Box& box2);
  static bool  bConnect(const Box& box1, const Box& box2);
  static bool  bInside(const Box& rect, const Point<T>& pt);
  static bool  bConnect(const Box& rect, const Point<T>& pt);
  static T     overlapArea(const Box& box1, const Box& box2);
  static void  intersection(const Box& box1, const Box& box2, std::vector<Box>& result);
  static void  intersection2(const Box& box1, const Box& box2, std::vector<Box>& result);
  static void  difference2(const Box& box1, const Box& box2, std::vector<Box>& result);

  //operator
  bool operator < (const Box<T> &rect) const {
    if (_bl.x()   != rect.left())   return _bl.x() < rect.left();
    if (_bl.y() != rect.bottom()) return _bl.y() < rect.bottom();
    if (_tr.x()  != rect.right())  return _tr.x() < rect.right();
    return _tr.y() < rect.top();
  }
  bool operator == (const Box<T> &rect) const {
    return _bl.x() == rect.left() && _bl.y() == rect.bottom() && _tr.x() == rect.right() && _tr.y() == rect.top();
  }

  friend std::ostream& operator << (std::ostream& os, const Box& r) {
    os << '(' << r._bl.x() << ' ' << r._bl.y() << ' ' << r._tr.x() << ' ' << r._tr.y() << ')';
    return os;
  }

  //Debugging
  void printRectInfo() const {
    std::cout << "  Bound : " << '(' << _bl.x() << ' ' << _bl.y() << ' ' << _tr.x() << ' ' << _tr.y() << ')';
    std::cout << "  CenterXY : " << ' ' << (_bl.x() + _tr.x()) / 2  << ' ' << (_bl.y() + _tr.y()) / 2 << ')' << std::endl;
  }

private:
  Point<T> _bl; // bottom left
  Point<T> _tr; // top right
};

// member fucntions definition
template<typename T>
void Box<T>::shiftX(const T x) {
  setL(_bl.x() + x);
  setR(_tr.x() + x);
}

template<typename T>
void Box<T>::shiftY(const T y) {
  setB(_bl.y() + y);
  setT(_tr.y() + y);
}

template<typename T>
void Box<T>::shift(const T x, const T y) {
  shiftX(x);
  shiftY(y);
}

template<typename T>
void Box<T>::rotate90(const T x, const T y, const bool bClockWise) {
  Point<T> p1(_bl);
  Point<T> p2(_tr);
  p1.rotate90(x, y, bClockWise);
  p2.rotate90(x, y, bClockWise);
  setL(std::min(p1.x(), p2.x()));
  setB(std::min(p1.y(), p2.y()));
  setR(std::max(p1.x(), p2.x()));
  setT(std::max(p1.y(), p2.y()));
}

template<typename T>
void Box<T>::rotate180(const T x, const T y) {
  Point<T> p1(_bl);
  Point<T> p2(_tr);
  p1.rotate180(x, y);
  p2.rotate180(x, y);
  setL(std::min(p1.x(), p2.x()));
  setR(std::max(p1.x(), p2.x()));
  setB(std::min(p1.y(), p2.y()));
  setT(std::max(p1.y(), p2.y()));
}

template<typename T>
void Box<T>::flipX(const T x) {
  int l = _bl.x();
  int r = _tr.x();
  setL(x + (x - r));
  setR(x + (x - l));
}

template<typename T>
void Box<T>::flipY(const T y) {
  int b = _bl.y();
  int t = _tr.y();
  setB(y + (y - t));
  setT(y + (y - b));
}

template<typename T>
void Box<T>::expand(const T s) {
  setL(_bl.x() - s);
  setB(_bl.y() - s);
  setR(_tr.x() + s);
  setT(_tr.y() + s);
}

template<typename T>
void Box<T>::expand(const T s, const int dim) {
  if (dim == 0) {
    setL(_bl.x() - s);
    setR(_tr.x() + s);
  }
  else {
    assert(dim == 1);
    setB(_bl.y() - s);
    setT(_bl.y() + s);
  }
}

template<typename T>
void Box<T>::expandX(const T s) {
  setL(_bl.x() - s);
  setR(_tr.x() + s);
}

template<typename T>
void Box<T>::expandY(const T s) {
  setB(_bl.y() - s);
  setT(_bl.y() + s);
}

template<typename T>
void Box<T>::shrink(const T s) {
  setL(_bl.x() + s);
  setB(_bl.y() + s);
  setR(_tr.x() - s);
  setT(_tr.y() - s);
}

template<typename T>
void Box<T>::shrinkX(const T s) {
  setL(_bl.x() + s);
  setR(_tr.x() - s);
}

template<typename T>
void Box<T>::shrinkY(const T s) {
  setB(_bl.y() + s);
  setT(_tr.y() - s);
}

template<typename T>
void Box<T>::difference(const Box<T>& r, std::vector<Box<T> >& result) {
  T a = std::min(_bl.x(), r._bl.x());
  T b = std::max(_bl.x(), r._bl.x());
  T c = std::min(_tr.x(), r._tr.x());
  T d = std::max(_tr.x(), r._tr.x());

  T e = std::min(_bl.y(), r._bl.y());
  T f = std::max(_bl.y(), r._bl.y());
  T g = std::min(_tr.y(), r._tr.y());
  T h = std::max(_tr.y(), r._tr.y());

  if (f - e > 0) {
    if (b - a > 0) {
      Box<T> tmp(a, e, b, f);
      if (bOverlap(*this, tmp))
        result.push_back(tmp);
    }
    if (c - b > 0) {
      Box<T> tmp(b, e, c, f);
      if (bOverlap(*this, tmp))
        result.push_back(tmp);
    }
    if (d - c > 0) {
      Box<T> tmp(c, e, d, f);
      if (bOverlap(*this, tmp))
        result.push_back(tmp);
    }
  }
  if (g - f > 0) {
    if (b - a > 0) {
      Box<T> tmp(a, f, b, g);
      if (bOverlap(*this, tmp))
        result.push_back(tmp);
    }
    if (d - c > 0) {
      Box<T> tmp(c, f, d, g);
      if (bOverlap(*this, tmp))
        result.push_back(tmp);
    }
  }
  if (h - g > 0) {
    if (b - a > 0) {
      Box<T> tmp(a, g, b, h);
      if (bOverlap(*this, tmp))
        result.push_back(tmp);
    }
    if (c - b > 0) {
      Box<T> tmp(b, g, c, h);
      if (bOverlap(*this, tmp))
        result.push_back(tmp);
    }
    if (d - c > 0) {
      Box<T> tmp(c, g, d, h);
      if (bOverlap(*this, tmp))
        result.push_back(tmp);
    }
  }
}

template<typename T>
void Box<T>::multi_diff(const std::vector<Box<T>>& vRects, std::vector<Box<T>>& result) const {
  std::vector<T> hor = { _bl.y(), _tr.y() };
  std::vector<T> ver = { _bl.x(), _tr.x() };
  for (Index_t i = 0; i < vRects.size(); ++i) {
    Box<T>& box= vRects[i];
    if (box.left() > _bl.x() && box.left() < _tr.x())
      ver.push_back(box.left());
    if (box.right() > _bl.x() && box.right() < _tr.x())
      ver.push_back(box.right());
    if (box.bottom() > _bl.y() && box.bottom() < _tr.y())
      hor.push_back(box.bottom());
    if (box.top() > _bl.y() && box.top() < _tr.y())
      hor.push_back(box.top());
  }
  std::sort(hor.begin(), hor.end());
  std::sort(ver.begin(), ver.end());
  hor.resize(std::unique(hor.begin(), hor.end()) - hor.begin());
  ver.resize(std::unique(ver.begin(), ver.end()) - ver.begin());
  for (Index_t i = 1; i < ver.size(); ++i)
    for (Index_t j = 1; j < hor.size(); ++j)
      result.push_back(Box(ver[i - 1], hor[j - 1], ver[i], hor[j]));
}

template<typename T>
void Box<T>::multi_diff(const std::list<Index_t>& ord, const std::vector<Box<T>>& vRects, std::vector<Box<T>>& result) const {
  std::vector<T> hor = { _bl.y(), _tr.y() };
  std::vector<T> ver = { _bl.x(), _tr.x() };
  for (Index_t i : ord) {
    const Box<T>& rect = vRects[i];
    if (rect.left() > _bl.x() && rect.left() < _tr.x())
      ver.push_back(rect.left());
    if (rect.right() > _bl.x() && rect.right() < _tr.x())
      ver.push_back(rect.right());
    if (rect.bottom() > _bl.y() && rect.bottom() < _tr.y())
      hor.push_back(rect.bottom());
    if (rect.top() > _bl.y() && rect.top() < _tr.y())
      hor.push_back(rect.top());
  }
  std::sort(hor.begin(), hor.end());
  std::sort(ver.begin(), ver.end());
  hor.resize(std::unique(hor.begin(), hor.end()) - hor.begin());
  ver.resize(std::unique(ver.begin(), ver.end()) - ver.begin());
  for (Index_t i = 1; i < ver.size(); ++i)
    for (Index_t j = 1; j < hor.size(); ++j)
      result.push_back(Box(ver[i - 1], hor[j - 1], ver[i], hor[j]));
}

template<typename T>
void Box<T>::multi_diff(const std::list<const Box<T>*>& vRects, std::vector<Box<T>>& result) const {
  std::vector<T> hor = { _bl.y(), _tr.y() };
  std::vector<T> ver = { _bl.x(), _tr.x() };
  for (const Box<T>* rect : vRects) {
    if (rect->left() > _bl.x() && rect->left() < _tr.x())
      ver.push_back(rect->left());
    if (rect->right() > _bl.x() && rect->right() < _tr.x())
      ver.push_back(rect->right());
    if (rect->bottom() > _bl.y() && rect->bottom() < _tr.y())
      hor.push_back(rect->bottom());
    if (rect->top() > _bl.y() && rect->top() < _tr.y())
      hor.push_back(rect->top());
  }
  std::sort(hor.begin(), hor.end());
  std::sort(ver.begin(), ver.end());
  hor.resize(std::unique(hor.begin(), hor.end()) - hor.begin());
  ver.resize(std::unique(ver.begin(), ver.end()) - ver.begin());
  for (Index_t i = 1; i < ver.size(); ++i)
    for (Index_t j = 1; j < hor.size(); ++j)
      result.push_back(Box<T>(ver[i - 1], hor[j - 1], ver[i], hor[j]));
}

// static functions
template<typename T>
T Box<T>::Mdistance(const Box<T>& box1, const Box<T>& box2) {
  T d1 = max({box1._bl.x() - box2._tr.x(), box2._bl.x() - box1._tr.x(), (T)0});
  T d2 = max({box1._bl.y() - box2._tr.y(), box2._bl.y() - box1._tr.y(), (T)0});
  return d1 + d2;
}

template<typename T>
T Box<T>::Mcenterdistance(const Box<T>& box1, const Box<T>& box2) {
  return abs((box1.left() + box1.right()) / 2 - (box2.left() + box2.right()) / 2)
       + abs((box1.bottom() + box1.top()) / 2 - (box2.bottom() + box2.top()) / 2);
}

template<typename T>
T Box<T>::Mdistance(const Box<T>& box, const Point<T>& pt) {
  T d1 = 0, d2 = 0;
  if (pt.x() < box.left())
    d1 = box.left() - pt.x();
  else if (pt.x() > box.right())
    d1 = pt.x() - box.right();
  if (pt.y() < box.bottom())
    d2 = box.bottom() - pt.y();
  else if
    (pt.y() > box.top()) d2 = pt.y() - box.top();
  return d1 + d2;
}

template<typename T>
bool Box<T>::bOverlap(const Box<T>& box1, const Box<T>& box2) {
  if (box1._bl.y() >= box2._tr.y()) return false;
  if (box1._tr.y() <= box2._bl.y()) return false;
  if (box1._bl.x() >= box2._tr.x()) return false;
  if (box1._tr.x() <= box2._bl.x()) return false;
  return true;
}

template<typename T>
bool Box<T>::bConnect(const Box<T>& box1, const Box<T>& box2) {
  if (box1._bl.y() > box2._tr.y()) return false;
  if (box1._tr.y() < box2._bl.y()) return false;
  if (box1._bl.x() > box2._tr.x()) return false;
  if (box1._tr.x() < box2._bl.x()) return false;
  return true;
}

template<typename T>
bool Box<T>::bInside(const Box<T>& box, const Point<T>& pt) {
  return (pt.x() > box.left() && pt.x() < box.right() &&
          pt.y() > box.bottom() && pt.y() < box.top());
}

template<typename T>
bool Box<T>::bConnect(const Box<T>& box, const Point<T>& pt) {
  return (pt.x() >= box.left() && pt.x() <= box.right() &&
          pt.y() >= box.bottom() && pt.y() <= box.top());
}

template<typename T>
T Box<T>::overlapArea(const Box<T>& box1, const Box<T>& box2) {
  T overlapH = std::min(box1._tr.x(), box2._tr.x()) - std::max(box1._bl.x(), box2._bl.x());
  T overlapV = std::min(box1._tr.y(), box2._tr.y()) - std::max(box1._bl.y(), box2._bl.y());
  if (overlapH <= 0 || overlapV <= 0)
    return 0;
  return overlapH * overlapV;
}

template<typename T>
void Box<T>::intersection(const Box<T>& box1, const Box<T>& box2, std::vector<Box<T> >& result) {
  if (!bOverlap(box1, box2))
    return;
  Box<T> box;
  box.setL(std::max(box1._bl.x(), box2._bl.x()));
  box.setR(std::min(box1._tr.x(), box2._tr.x()));
  box.setB(std::max(box1._bl.y(), box2._bl.y()));
  box.setT(std::min(box1._tr.y(), box2._tr.y()));
  result.push_back(box);
}

template<typename T>
void Box<T>::intersection2(const Box<T>& box1, const Box<T>& box2, std::vector<Box<T> >& result) {
  if (!bConnect(box1, box2))
    return;
  Box<T> box;
  box.setL(std::max(box1._bl.x(), box2._bl.x()));
  box.setR(std::min(box1._tr.x(), box2._tr.x()));
  box.setB(std::max(box1._bl.y(), box2._bl.y()));
  box.setT(std::min(box1._tr.y(), box2._tr.y()));
  result.push_back(box);
}

template<typename T>
void Box<T>::difference2(const Box<T>& box1, const Box<T>& box2, std::vector<Box<T> >& result) {
  // X = intersection, 0-7 = possible difference areas
  // h +-+-+-+
  // . |5|6|7|
  // g +-+-+-+
  // . |3|X|4|
  // f +-+-+-+
  // . |0|1|2|
  // e +-+-+-+
  // . a b c d
  T a = std::min(box1._bl.x(), box2._bl.x());
  T b = std::max(box1._bl.x(), box2._bl.x());
  T c = std::min(box1._tr.x(), box2._tr.x());
  T d = std::max(box1._tr.x(), box2._tr.x());

  T e = std::min(box1._bl.y(), box2._bl.y());
  T f = std::max(box1._bl.y(), box2._bl.y());
  T g = std::min(box1._tr.y(), box2._tr.y());
  T h = std::max(box1._tr.y(), box2._tr.y());

  if (f - e > 0) {
    if (b - a > 0) {
      Box<T> tmp(a, e, b, f);
      if (bOverlap(box1, tmp) || bOverlap(box2, tmp))
        result.push_back(tmp);
    }
    if (c - b > 0) {
      Box<T> tmp(b, e, c, f);
      if (bOverlap(box1, tmp) || bOverlap(box2, tmp))
        result.push_back(tmp);
    }
    if (d - c > 0) {
      Box<T> tmp(c, e, d, f);
      if (bOverlap(box1, tmp) || bOverlap(box2, tmp))
        result.push_back(tmp);
    }
  }
  if (g - f > 0) {
    if (b - a > 0) {
      Box<T> tmp(a, f, b, g);
      if (bOverlap(box1, tmp) || bOverlap(box2, tmp))
        result.push_back(tmp);
    }
    if (d - c > 0) {
      Box<T> tmp(c, f, d, g);
      if (bOverlap(box1, tmp) || bOverlap(box2, tmp))
        result.push_back(tmp);
    }
  }
  if (h - g > 0) {
    if (b - a > 0) {
      Box<T> tmp(a, g, b, h);
      if (bOverlap(box1, tmp) || bOverlap(box2, tmp))
        result.push_back(tmp);
    }
    if (c - b > 0) {
      Box<T> tmp(b, g, c, h);
      if (bOverlap(box1, tmp) || bOverlap(box2, tmp))
        result.push_back(tmp);
    }
    if (d - c > 0) {
      Box<T> tmp(c, g, d, h);
      if (bOverlap(box1, tmp) || bOverlap(box2, tmp))
        result.push_back(tmp);
    }
  }
}

PROJECT_NAMESPACE_END

#endif /// ANAROUTE_BOX_HPP_
