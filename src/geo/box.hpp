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

template<class T>
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
  void  setLeft(T l)                                  { _bl.setX(l); }
  void  setRight(T r)                                 { _tr.setX(r); }
  void  setBottom(T b)                                { _bl.setY(b); }
  void  setTop(T t)                                   { _tr.setY(t); }
  //void  setBounds(T left, T bottom, T right, T top)   { _left = left; _bottom = bottom; _right = right; _top = top; }
  ////Basic access functions
  //T          left()             const { return _left;                            }
  //T          bottom()           const { return _bottom;                          }
  //T          right()            const { return _right;                           }
  //T          top()              const { return _top;                             }
  //T          width()            const { return _right - _left;                   }
  //T          height()           const { return _top - _bottom;                   }
  //T          centerX()          const { return (_left + _right) / 2;             }
  //T          centerY()          const { return (_bottom + _top) / 2;             }
  //T          hpwl()             const { return width() + height();               }
  //T          perimeter()        const { return 2 * hpwl();                       }
  //T          area()             const { return width() * height();               }
  //Point<T>  center()           const { return Point<T>(centerX(), centerY());  }
  //Point<T>  min_corner()       const { return Point<T>(_left, _bottom);        }
  //Point<T>  max_corner()       const { return Point<T>(_right, _top);          }
  //Point<T>  bottomLeft()       const { return Point<T>(_left, _bottom);        }
  //Point<T>  topRight()         const { return Point<T>(_right, _top);          }

  ////utils
  //void shift(const T x, const T y);
  //void rotate90(const T x, const T y, const bool bClockWise); // rotate 90 degree with respect to (x, y)
  //void rotate180(const T x, const T y);                       // rotate 180 degree with respect to (x, y)
  //void flipX(const T x);                                      // flip by line x = x
  //void flipY(const T y);                                      // flip by line y = y
  //void expand(const T s);
  //void expand(const T s, const int dim);
  //void expandX(const T s);
  //void expandY(const T s);
  //void shrink(const T s);
  //void shrinkX(const T s);
  //void shrinkY(const T s);
  //void difference(const Box& r, vector<Box>& result);
  //void multi_diff(const vector<Box>& vRects, vector<Box>& result) const;
  //void multi_diff(const list<const Box* >& vRects, vector<Box>& result) const;
  //void multi_diff(const list<int>& ord, const vector<Box> &vRects, vector<Box>& result) const;

  ////static functions
  //static T     Mdistance(const Box& rect1, const Box& rect2);
  //static T     Mcenterdistance(const Box& rect1, const Box& rect2);
  //static T     Mdistance(const Box& rect1, const Point<T>& pt);
  //static bool  isOverlap(const Box& rect1, const Box& rect2);
  //static bool  isConnect(const Box& rect1, const Box& rect2);
  //static bool  isInside(const Box& rect, const Point<T>& pt);
  //static bool  isConnect(const Box& rect, const Point<T>& pt);
  //static T     overlapArea(const Box& rect1, const Box& rect2);
  //static void  intersection(const Box& rect1, const Box& rect2, vector<Box>& result);
  //static void  intersection2(const Box& rect1, const Box& rect2, vector<Box>& result);
  //static void  difference2(const Box& rect1, const Box& rect2, vector<Box>& result);

  ////operator
  //bool operator < (const Box<T> &rect) const {
    //if (_left   != rect.left())   return _left < rect.left();
    //if (_bottom != rect.bottom()) return _bottom < rect.bottom();
    //if (_right  != rect.right())  return _right < rect.right();
    //return _top < rect.top();
  //}
  //bool operator == (const Box<T> &rect) const {
    //return _left == rect.left() && _bottom == rect.bottom() && _right == rect.right() && _top == rect.top();
  //}

  //friend ostream& operator << (ostream& os, const Box& r) {
    //os << '(' << r._left << ' ' << r._bottom << ' ' << r._right << ' ' << r._top << ')';
    //return os;
  //}

  ////Debugging
  //void printRectInfo() const {
    //cout << "  Bound : " << '(' << _left << ' ' << _bottom << ' ' << _right << ' ' << _top << ')';
    //cout << "  CenterXY : " << ' ' << (_left + _right) / 2  << ' ' << (_bottom + _top) / 2 << ')' << endl;
  //}

private:
  Point<T> _bl; // bottom left
  Point<T> _tr; // top right
};

////member fucntions definition
//template<class T>
//void Box<T>::shift(const T x, const T y) {
  //_left += x;
  //_right += x;
  //_bottom += y;
  //_top += y;
//}

//template<class T>
//void Box<T>::rotate90(const T x, const T y, const bool bClockWise) {
  //Point<T> p1(_left, _bottom);
  //Point<T> p2(_right, _top);
  //p1.rotate90(x, y, bClockWise);
  //p2.rotate90(x, y, bClockWise);
  //_left = min(p1.x(), p2.x());
  //_right = max(p1.x(), p2.x());
  //_bottom = min(p1.y(), p2.y());
  //_top = max(p1.y(), p2.y());
//}

//template<class T>
//void Box<T>::rotate180(const T x, const T y) {
  //Point<T> p1(_left, _bottom);
  //Point<T> p2(_right, _top);
  //p1.rotate180(x, y);
  //p2.rotate180(x, y);
  //_left = min(p1.x(), p2.x());
  //_right = max(p1.x(), p2.x());
  //_bottom = min(p1.y(), p2.y());
  //_top = max(p1.y(), p2.y());
//}

//template<class T>
//void Box<T>::flipX(const T x) {
  //int l = _left;
  //int r = _right;
  //_left = x + (x - r);
  //_right = x + (x - l);
//}

//template<class T>
//void Box<T>::flipY(const T y) {
  //int b = _bottom;
  //int t = _top;
  //_bottom = y + (y - t);
  //_top = y + (y - b);
//}

//template<class T>
//void Box<T>::expand(const T s) {
  //_left -= s;
  //_right += s;
  //_bottom -= s;
  //_top += s;
//}

//template<class T>
//void Box<T>::expand(const T s, const int dim) {
  //if (dim == 0) {
    //_left -= s;
    //_right += s;
  //}
  //else {
    //assert(dim == 1);
    //_bottom -= s;
    //_top += s;
  //}
//}

//template<class T>
//void Box<T>::expandX(const T s) {
  //_left -= s;
  //_right += s;
//}

//template<class T>
//void Box<T>::expandY(const T s) {
  //_bottom -= s;
  //_top += s;
//}

//template<class T>
//void Box<T>::shrink(const T s) {
  //_left += s;
  //_right -= s;
  //_bottom += s;
  //_top -= s;
//}

//template<class T>
//void Box<T>::shrinkX(const T s) {
  //_left += s;
  //_right -= s;
//}

//template<class T>
//void Box<T>::shrinkY(const T s) {
  //_bottom += s;
  //_top -= s;
//}

//template<class T>
//void Box<T>::difference(const Box<T>& r, vector<Box<T> >& result) {
  //T a = min(_left, r._left);
  //T b = max(_left, r._left);
  //T c = min(_right, r._right);
  //T d = max(_right, r._right);

  //T e = min(_bottom, r._bottom);
  //T f = max(_bottom, r._bottom);
  //T g = min(_top, r._top);
  //T h = max(_top, r._top);

  //if (f - e > 0) {
    //if (b - a > 0) {
      //Box<T> tmp(a, e, b, f);
      //if (isOverlap(*this, tmp))
        //result.push_back(tmp);
    //}
    //if (c - b > 0) {
      //Box<T> tmp(b, e, c, f);
      //if (isOverlap(*this, tmp))
        //result.push_back(tmp);
    //}
    //if (d - c > 0) {
      //Box<T> tmp(c, e, d, f);
      //if (isOverlap(*this, tmp))
        //result.push_back(tmp);
    //}
  //}
  //if (g - f > 0) {
    //if (b - a > 0) {
      //Box<T> tmp(a, f, b, g);
      //if (isOverlap(*this, tmp))
        //result.push_back(tmp);
    //}
    //if (d - c > 0) {
      //Box<T> tmp(c, f, d, g);
      //if (isOverlap(*this, tmp))
        //result.push_back(tmp);
    //}
  //}
  //if (h - g > 0) {
    //if (b - a > 0) {
      //Box<T> tmp(a, g, b, h);
      //if (isOverlap(*this, tmp))
        //result.push_back(tmp);
    //}
    //if (c - b > 0) {
      //Box<T> tmp(b, g, c, h);
      //if (isOverlap(*this, tmp))
        //result.push_back(tmp);
    //}
    //if (d - c > 0) {
      //Box<T> tmp(c, g, d, h);
      //if (isOverlap(*this, tmp))
        //result.push_back(tmp);
    //}
  //}
//}

//template<class T>
//void Box<T>::multi_diff(const vector<Box<T> >& vRects, vector<Box<T> >& result) const {
  //vector<T> hor = { _bottom, _top };
  //vector<T> ver = { _left, _right };
  //for (int i = 0; i < (int)vRects.size(); ++i) {
    //Box<T>& rect = vRects[i];
    //if (rect->left() > _left && rect->left() < _right)
      //ver.push_back(rect->left());
    //if (rect->right() > _left && rect->right() < _right)
      //ver.push_back(rect->right());
    //if (rect->bottom() > _bottom && rect->bottom() < _top)
      //hor.push_back(rect->bottom());
    //if (rect->top() > _bottom && rect->top() < _top)
      //hor.push_back(rect->top());
  //}
  //sort(hor.begin(), hor.end());
  //sort(ver.begin(), ver.end());
  //hor.resize(unique(hor.begin(), hor.end()) - hor.begin());
  //ver.resize(unique(ver.begin(), ver.end()) - ver.begin());
  //for (int i = 1; i < (int)ver.size(); ++i)
    //for (int j = 1; j < (int)hor.size(); ++j)
      //result.push_back(Box(ver[i - 1], hor[j - 1], ver[i], hor[j]));
//}

//template<class T>
//void Box<T>::multi_diff(const list<int>& ord, const vector<Box<T> >& vRects, vector<Box<T> >& result) const {
  //vector<T> hor = { _bottom, _top };
  //vector<T> ver = { _left, _right };
  //for (int i : ord) {
    //const Box<T>& rect = vRects[i];
    //if (rect.left() > _left && rect.left() < _right)
      //ver.push_back(rect.left());
    //if (rect.right() > _left && rect.right() < _right)
      //ver.push_back(rect.right());
    //if (rect.bottom() > _bottom && rect.bottom() < _top)
      //hor.push_back(rect.bottom());
    //if (rect.top() > _bottom && rect.top() < _top)
      //hor.push_back(rect.top());
  //}
  //sort(hor.begin(), hor.end());
  //sort(ver.begin(), ver.end());
  //hor.resize(unique(hor.begin(), hor.end()) - hor.begin());
  //ver.resize(unique(ver.begin(), ver.end()) - ver.begin());
  //for (int i = 1; i < (int)ver.size(); ++i)
    //for (int j = 1; j < (int)hor.size(); ++j)
      //result.push_back(Box(ver[i - 1], hor[j - 1], ver[i], hor[j]));
//}

//template<class T>
//void Box<T>::multi_diff(const list<const Box<T>* >& vRects, vector<Box<T> >& result) const {
  //vector<T> hor = { _bottom, _top };
  //vector<T> ver = { _left, _right };
  //for (const Box<T>* rect : vRects) {
    //if (rect->left() > _left && rect->left() < _right)
      //ver.push_back(rect->left());
    //if (rect->right() > _left && rect->right() < _right)
      //ver.push_back(rect->right());
    //if (rect->bottom() > _bottom && rect->bottom() < _top)
      //hor.push_back(rect->bottom());
    //if (rect->top() > _bottom && rect->top() < _top)
      //hor.push_back(rect->top());
  //}
  //sort(hor.begin(), hor.end());
  //sort(ver.begin(), ver.end());
  //hor.resize(unique(hor.begin(), hor.end()) - hor.begin());
  //ver.resize(unique(ver.begin(), ver.end()) - ver.begin());
  //for (int i = 1; i < (int)ver.size(); ++i)
    //for (int j = 1; j < (int)hor.size(); ++j)
      //result.push_back(Box<T>(ver[i - 1], hor[j - 1], ver[i], hor[j]));
//}

static functions
//template<class T>
//T Box<T>::Mdistance(const Box<T>& rect1, const Box<T>& rect2) {
  //T d1 = max({rect1._left - rect2._right, rect2._left - rect1._right, (T)0});
  //T d2 = max({rect1._bottom - rect2._top, rect2._bottom - rect1._top, (T)0});
  //return d1 + d2;
//}
//template<class T>
//T Box<T>::Mcenterdistance(const Box<T>& rect1, const Box<T>& rect2) {
  //return abs((rect1.left() + rect1.right()) / 2 - (rect2.left() + rect2.right()) / 2)
       //+ abs((rect1.bottom() + rect1.top()) / 2 - (rect2.bottom() + rect2.top()) / 2);
//}

//template<class T>
//T Box<T>::Mdistance(const Box<T>& rect, const Point<T>& pt) {
  //T d1 = 0, d2 = 0;
  //if (pt.x() < rect.left()) d1 = rect.left() - pt.x();
  //else if (pt.x() > rect.right()) d1 = pt.x() - rect.right();
  //if (pt.y() < rect.bottom()) d2 = rect.bottom() - pt.y();
  //else if (pt.y() > rect.top()) d2 = pt.y() - rect.top();
  //return d1 + d2;
//}

//template<class T>
//bool Box<T>::isOverlap(const Box<T>& rect1, const Box<T>& rect2) {
  //if (rect1._bottom >= rect2._top   ) return false;
  //if (rect1._top    <= rect2._bottom) return false;
  //if (rect1._left   >= rect2._right ) return false;
  //if (rect1._right  <= rect2._left  ) return false;
  //return true;
//}

//template<class T>
//bool Box<T>::isConnect(const Box<T>& rect1, const Box<T>& rect2) {
  //if (rect1._bottom > rect2._top   ) return false;
  //if (rect1._top    < rect2._bottom) return false;
  //if (rect1._left   > rect2._right ) return false;
  //if (rect1._right  < rect2._left  ) return false;
  //return true;
//}

//template<class T>
//bool Box<T>::isInside(const Box<T>& rect, const Point<T>& pt) {
  //return (pt.x() > rect.left() && pt.x() < rect.right() &&
          //pt.y() > rect.bottom() && pt.y() < rect.top());
//}

//template<class T>
//bool Box<T>::isConnect(const Box<T>& rect, const Point<T>& pt) {
  //return (pt.x() >= rect.left() && pt.x() <= rect.right() &&
          //pt.y() >= rect.bottom() && pt.y() <= rect.top());
//}

//template<class T>
//T Box<T>::overlapArea(const Box<T>& rect1, const Box<T>& rect2) {
  //T overlapH = min( rect1._right, rect2._right ) - max( rect1._left, rect2._left     );
  //T overlapV = min( rect1._top, rect2._top     ) - max( rect1._bottom, rect2._bottom );
  //if (overlapH < 0) overlapH = 0;
  //if (overlapV < 0) overlapV = 0;
  //return overlapH * overlapV;
//}

//template<class T>
//void Box<T>::intersection(const Box<T>& rect1, const Box<T>& rect2, vector<Box<T> >& result) {
  //if (!isOverlap(rect1, rect2))
    //return;
  //Box<T> rect;
  //rect.setLeft(max(rect1._left, rect2._left));
  //rect.setRight(min(rect1._right, rect2._right));
  //rect.setBottom(max(rect1._bottom, rect2._bottom));
  //rect.setTop(min(rect1._top, rect2._top));
  //result.push_back(rect);
//}

//template<class T>
//void Box<T>::intersection2(const Box<T>& rect1, const Box<T>& rect2, vector<Box<T> >& result) {
  //if (!isConnect(rect1, rect2))
    //return;
  //Box<T> rect;
  //rect.setLeft(max(rect1._left, rect2._left));
  //rect.setRight(min(rect1._right, rect2._right));
  //rect.setBottom(max(rect1._bottom, rect2._bottom));
  //rect.setTop(min(rect1._top, rect2._top));
  //result.push_back(rect);
//}

//template<class T>
//void Box<T>::difference2(const Box<T>& rect1, const Box<T>& rect2, vector<Box<T> >& result) {
  X = intersection, 0-7 = possible difference areas
  h +-+-+-+
  . |5|6|7|
  g +-+-+-+
  . |3|X|4|
  f +-+-+-+
  . |0|1|2|
  e +-+-+-+
  . a b c d
  //T a = min(rect1._left, rect2._left);
  //T b = max(rect1._left, rect2._left);
  //T c = min(rect1._right, rect2._right);
  //T d = max(rect1._right, rect2._right);

  //T e = min(rect1._bottom, rect2._bottom);
  //T f = max(rect1._bottom, rect2._bottom);
  //T g = min(rect1._top, rect2._top);
  //T h = max(rect1._top, rect2._top);

  //if (f - e > 0) {
    //if (b - a > 0) {
      //Box<T> tmp(a, e, b, f);
      //if (isOverlap(rect1, tmp) || isOverlap(rect2, tmp))
        //result.push_back(tmp);
    //}
    //if (c - b > 0) {
      //Box<T> tmp(b, e, c, f);
      //if (isOverlap(rect1, tmp) || isOverlap(rect2, tmp))
        //result.push_back(tmp);
    //}
    //if (d - c > 0) {
      //Box<T> tmp(c, e, d, f);
      //if (isOverlap(rect1, tmp) || isOverlap(rect2, tmp))
        //result.push_back(tmp);
    //}
  //}
  //if (g - f > 0) {
    //if (b - a > 0) {
      //Box<T> tmp(a, f, b, g);
      //if (isOverlap(rect1, tmp) || isOverlap(rect2, tmp))
        //result.push_back(tmp);
    //}
    //if (d - c > 0) {
      //Box<T> tmp(c, f, d, g);
      //if (isOverlap(rect1, tmp) || isOverlap(rect2, tmp))
        //result.push_back(tmp);
    //}
  //}
  //if (h - g > 0) {
    //if (b - a > 0) {
      //Box<T> tmp(a, g, b, h);
      //if (isOverlap(rect1, tmp) || isOverlap(rect2, tmp))
        //result.push_back(tmp);
    //}
    //if (c - b > 0) {
      //Box<T> tmp(b, g, c, h);
      //if (isOverlap(rect1, tmp) || isOverlap(rect2, tmp))
        //result.push_back(tmp);
    //}
    //if (d - c > 0) {
      //Box<T> tmp(c, g, d, h);
      //if (isOverlap(rect1, tmp) || isOverlap(rect2, tmp))
        //result.push_back(tmp);
    //}
  //}
//}

//struct PIIII_hasher {
  //size_t operator() (const Box<int> &rect) const {
    //size_t v = rect.left() * 1000000007;
    //v = (v + rect.bottom()) * 1000001449;
    //v = (v + rect.right()) * 1000003163;
    //return (v + rect.top()) * 1000003787;
  //}
//};

PROJECT_NAMESPACE_END

#endif /// ANAROUTE_BOX_HPP_
