/**
 * @file   spatial.hpp
 * @brief  Geometric Data Structure: 2D spatial type (R-Tree Kernel)
 * @author Hao Chen
 * @date   09/06/2019
 *
 **/

#ifndef _GEO_SPATIAL_HPP_
#define _GEO_SPATIAL_HPP_

#include <vector>
#include <boost/geometry.hpp>
#include <boost/function_output_iterator.hpp>

#include "src/global/global.hpp"
#include "src/geo/point.hpp"
#include "src/geo/box.hpp"

PROJECT_NAMESPACE_START

namespace spatial {

  namespace bg = boost::geometry;
  namespace bgi = boost::geometry::index;

  template<typename T>
  using b_box = bg::model::box<Point<T> >;

  template<typename T, typename Value>
  using b_value = Pair_t<b_box<T>, Value>;

  template<typename T>
  struct B_box_equal {
    bool operator() (const b_box<T>& b0, const b_box<T>& b1) const {
      return b0.min_corner() == b1.min_corner() &&
             b0.max_corner() == b1.max_corner();
    }
  };

  template<typename T, typename Value>
  struct B_value_equal {
    bool operator() (const b_value<T, Value>& b0, const b_value<T, Value>& b1) const {
      return b0.first.min_corner() == b1.first.min_corner() &&
             b0.first.max_corner() == b1.first.max_corner() &&
             b0.second == b1.second;
    }
  };

  template<typename T>
  using Rtree = bgi::rtree<b_box<T>, bgi::rstar<16>, bgi::indexable<b_box<T>>, B_box_equal<T>>;

  template<typename T, typename Value>
  using RtreeMap = bgi::rtree<b_value<T, Value>, bgi::rstar<16>, bgi::indexable<b_value<T, Value>>, B_value_equal<T, Value>>;

  template<typename T>
  struct SearchCallback { // return b_box or b_value
    SearchCallback(Vector_t<T>& ret)
      : _ret(ret) {}
    void operator () (const T& t) { _ret.push_back(t); }
    Vector_t<T>& _ret;
  };
  
  template<typename BBox>
  struct SearchCallback_first { // return b_value.first
    SearchCallback_first(Vector_t<BBox>& ret)
      : _ret(ret) {}
    template<typename BValue>
    void operator () (const BValue& bv) { _ret.push_back(bv.first); }
    Vector_t<BBox>& _ret;
  };
  
  template<typename Value>
  struct SearchCallback_second { // return b_value.second
    SearchCallback_second(Vector_t<Value>& ret)
      : _ret(ret) {}
    template<typename BValue>
    void operator () (const BValue& bv) { _ret.push_back(bv.second); }
    Vector_t<Value>& _ret;
  };

}

template<typename T>
class Spatial {
  spatial::Rtree<T> _rtree;
  using const_iterator = typename spatial::Rtree<T>::const_iterator;

public:
  // constructors
  Spatial() {}
  Spatial(const spatial::Rtree<T>& t) : _rtree(t) {}
  Spatial(const Spatial& sp) : _rtree(sp._rtree) {}
  template<typename Container>
  Spatial(const Container &container) : _rtree(container) {} // use packing algorithm
  template<typename Container_Iterator>
  Spatial(Container_Iterator begin, Container_Iterator end) : _rtree(begin, end) {} // use packing algorithm
  ~Spatial() {}

  enum class Query_Type {
    contains,
    covered_by,
    covers,
    disjoint,
    intersects,
    overlaps,
    within
  };

  // iterator
  inline const_iterator begin() const { return _rtree.begin(); }
  inline const_iterator end()   const { return _rtree.end(); }
  
  // operators
  void operator = (const Spatial& sp) { _rtree = sp._rtree; }
  
  // get
  bool    empty() const { return _rtree.empty(); }
  size_t  size()  const { return _rtree.size(); }
  
  // set
  void    clear()                                                           { _rtree.clear(); }
  
  void    insert(const spatial::b_box<T>& box)                              { _rtree.insert(box); }
  template<typename Container>
  void    insert(const Container &container)                                { _rtree.insert(container); } // use packing algorithm
  template<typename Container_Iterator>
  void    insert(Container_Iterator begin, Container_Iterator end)          { _rtree.insert(begin, end); } // use packing algorithm
  void    insert(const Box<T>& rect)                                 				{ _rtree.insert({rect.min_corner(), rect.max_corner()}); }
  void    insert(const Point<T>& min_corner, const Point<T>& max_corner)  	{ _rtree.insert({min_corner, max_corner}); }
  
  bool    erase(const spatial::b_box<T>& box)                               { return _rtree.remove(box); }
  template<typename Container>
  size_t  erase(const Container &container)                                 { return _rtree.remove(container); }
  template<typename Container_Iterator>
  size_t  erase(Container_Iterator begin, Container_Iterator end)           { return _rtree.remove(begin, end); }
  bool    erase(const Box<T>& rect)                                  				{ return _rtree.remove({rect.min_corner(), rect.max_corner()}); }
  bool    erase(const Point<T>& min_corner, const Point<T>& max_corner)   	{ return _rtree.remove({min_corner, max_corner}); }
  
  // query
  void    query(const Point<T>& min_corner, const Point<T>& max_corner, Vector_t<spatial::b_box<T> >& ret, Query_Type qt = Query_Type::intersects) const;
  void    query(const Box<T>& rect, Vector_t<spatial::b_box<T> >& ret, Query_Type qt = Query_Type::intersects) const;

};

template<typename T, typename Value>
class SpatialMap {
  spatial::RtreeMap<T, Value> _rtreeMap;
  using const_iterator = typename spatial::RtreeMap<T, Value>::const_iterator;


public:  
  // constructors
  SpatialMap() {}
  SpatialMap(const spatial::RtreeMap<T, Value>& t) : _rtreeMap(t) {}
  SpatialMap(const SpatialMap& sp) : _rtreeMap(sp._rtreeMap) {}
  template<typename Container>
  SpatialMap(const Container &container) : _rtreeMap(container) {} // use packing algorithm
  template<typename Container_Iterator>
  SpatialMap(Container_Iterator begin, Container_Iterator end) : _rtreeMap(begin, end) {} // use packing algorithm
  ~SpatialMap() {}

  enum class Query_Type {
    contains,
    covered_by,
    covers,
    disjoint,
    intersects,
    overlaps,
    within
  };

  // iterator
  inline const_iterator begin() const { return _rtreeMap.begin(); }
  inline const_iterator end()   const { return _rtreeMap.end(); }
  
  // operators
  void operator = (const SpatialMap& sp) { _rtreeMap = sp._rtreeMap; }
  
  // get
  bool    empty() const { return _rtreeMap.empty(); }
  size_t  size()  const { return _rtreeMap.size(); }
  
  // set
  void    clear()                                                                             { _rtreeMap.clear(); }
  
  void    insert(const spatial::b_value<T, Value>& bval)                                      { _rtreeMap.insert(bval); }
  template<typename Container>
  void    insert(const Container &container)                                                  { _rtreeMap.insert(container); } // use packing algorithm
  template<typename Container_Iterator>
  void    insert(Container_Iterator begin, Container_Iterator end)                            { _rtreeMap.insert(begin, end); } // use packing algorithm
  void    insert(const Box<T>& rect, const Value& val)                                 				{ _rtreeMap.insert({{rect.min_corner(), rect.max_corner()}, val}); }
  void    insert(const Point<T>& min_corner, const Point<T>& max_corner, const Value& val)  	{ _rtreeMap.insert({{min_corner, max_corner}, val}); }
  
  bool    erase(const spatial::b_value<T, Value>& bval)                                       { return _rtreeMap.remove(bval); }
  template<typename Container>
  size_t  erase(const Container &container)                                                   { return _rtreeMap.remove(container); }
  template<typename Container_Iterator>
  size_t  erase(Container_Iterator begin, Container_Iterator end)                             { return _rtreeMap.remove(begin, end); }
  bool    erase(const Box<T>& rect, const Value& val)                                  				{ return _rtreeMap.remove({{rect.min_corner(), rect.max_corner()}, val}); }
  bool    erase(const Point<T>& min_corner, const Point<T>& max_corner, const Value& val)   	{ return _rtreeMap.remove({{min_corner, max_corner}, val}); }
  
  // query
  void    query(const Point<T>& min_corner, const Point<T>& max_corner, Vector_t<Value>& ret, Query_Type qt = Query_Type::intersects) const;
  void    query(const Box<T>& rect, Vector_t<Value>& ret, Query_Type qt = Query_Type::intersects) const;
  void    queryBox(const Point<T>& min_corner, const Point<T>& max_corner, Vector_t<spatial::b_box<T> >& ret, Query_Type qt = Query_Type::intersects) const;
  void    queryBox(const Box<T>& rect, Vector_t<spatial::b_box<T> >& ret, Query_Type qt = Query_Type::intersects) const;
  void    queryBoth(const Point<T>& min_corner, const Point<T>& max_corner, Vector_t<spatial::b_value<T, Value> >& ret, Query_Type qt = Query_Type::intersects) const;
  void    queryBoth(const Box<T>& rect, Vector_t<spatial::b_value<T, Value> >& ret, Query_Type qt = Query_Type::intersects) const;

};

////////// Spatial Implementation /////////////
template<typename T>
void Spatial<T>::query(const Point<T>& min_corner, const Point<T>& max_corner, Vector_t<spatial::b_box<T> >& ret, Query_Type qt) const {
  spatial::SearchCallback<spatial::b_box<T> > callback(ret);
  spatial::b_box<T> query_box(min_corner, max_corner);
  switch (qt) {
    case Query_Type::contains :
      _rtree.query(spatial::bgi::contains(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covered_by :
      _rtree.query(spatial::bgi::covered_by(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covers :
      _rtree.query(spatial::bgi::covers(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::disjoint :
      _rtree.query(spatial::bgi::disjoint(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::intersects :
      _rtree.query(spatial::bgi::intersects(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::overlaps :
      _rtree.query(spatial::bgi::overlaps(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::within :
      _rtree.query(spatial::bgi::within(query_box), boost::make_function_output_iterator(callback)); break;
    default:
      assert(false);
  }
}

template<typename T>
void Spatial<T>::query(const Box<T>& rect, Vector_t<spatial::b_box<T> >& ret, Query_Type qt) const {
  spatial::SearchCallback<spatial::b_box<T> > callback(ret);
  spatial::b_box<T> query_box(rect.min_corner(), rect.max_corner());
  switch (qt) {
    case Query_Type::contains :
      _rtree.query(spatial::bgi::contains(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covered_by :
      _rtree.query(spatial::bgi::covered_by(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covers :
      _rtree.query(spatial::bgi::covers(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::disjoint :
      _rtree.query(spatial::bgi::disjoint(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::intersects :
      _rtree.query(spatial::bgi::intersects(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::overlaps :
      _rtree.query(spatial::bgi::overlaps(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::within :
      _rtree.query(spatial::bgi::within(query_box), boost::make_function_output_iterator(callback)); break;
    default:
      assert(false);
  }
}


////////// SpatialMap Implementation /////////////
template<typename T, typename Value>
void SpatialMap<T, Value>::query(const Point<T>& min_corner, const Point<T>& max_corner, Vector_t<Value>& ret, Query_Type qt) const {
  spatial::SearchCallback_second<Value> callback(ret);
  spatial::b_box<T> query_box(min_corner, max_corner);
  switch (qt) {
    case Query_Type::contains :
      _rtreeMap.query(spatial::bgi::contains(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covered_by :
      _rtreeMap.query(spatial::bgi::covered_by(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covers :
      _rtreeMap.query(spatial::bgi::covers(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::disjoint :
      _rtreeMap.query(spatial::bgi::disjoint(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::intersects :
      _rtreeMap.query(spatial::bgi::intersects(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::overlaps :
      _rtreeMap.query(spatial::bgi::overlaps(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::within :
      _rtreeMap.query(spatial::bgi::within(query_box), boost::make_function_output_iterator(callback)); break;
    default:
      assert(false);
  }
}

template<typename T, typename Value>
void SpatialMap<T, Value>::query(const Box<T>& rect, Vector_t<Value>& ret, Query_Type qt) const {
  spatial::SearchCallback_second<Value> callback(ret);
  spatial::b_box<T> query_box(rect.min_corner(), rect.max_corner());
  switch (qt) {
    case Query_Type::contains :
      _rtreeMap.query(spatial::bgi::contains(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covered_by :
      _rtreeMap.query(spatial::bgi::covered_by(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covers :
      _rtreeMap.query(spatial::bgi::covers(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::disjoint :
      _rtreeMap.query(spatial::bgi::disjoint(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::intersects :
      _rtreeMap.query(spatial::bgi::intersects(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::overlaps :
      _rtreeMap.query(spatial::bgi::overlaps(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::within :
      _rtreeMap.query(spatial::bgi::within(query_box), boost::make_function_output_iterator(callback)); break;
    default:
      assert(false);
  }
}

template<typename T, typename Value>
void SpatialMap<T, Value>::queryBox(const Point<T>& min_corner, const Point<T>& max_corner, Vector_t<spatial::b_box<T> >& ret, Query_Type qt) const {
  spatial::SearchCallback_first<spatial::b_box<T> > callback(ret);
  spatial::b_box<T> query_box(min_corner, max_corner);
  switch (qt) {
    case Query_Type::contains :
      _rtreeMap.query(spatial::bgi::contains(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covered_by :
      _rtreeMap.query(spatial::bgi::covered_by(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covers :
      _rtreeMap.query(spatial::bgi::covers(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::disjoint :
      _rtreeMap.query(spatial::bgi::disjoint(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::intersects :
      _rtreeMap.query(spatial::bgi::intersects(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::overlaps :
      _rtreeMap.query(spatial::bgi::overlaps(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::within :
      _rtreeMap.query(spatial::bgi::within(query_box), boost::make_function_output_iterator(callback)); break;
    default:
      assert(false);
  }
}

template<typename T, typename Value>
void SpatialMap<T, Value>::queryBox(const Box<T>& rect, Vector_t<spatial::b_box<T> >& ret, Query_Type qt) const {
  spatial::SearchCallback_first<spatial::b_box<T> > callback(ret);
  spatial::b_box<T> query_box(rect.min_corner(), rect.max_corner());
  switch (qt) {
    case Query_Type::contains :
      _rtreeMap.query(spatial::bgi::contains(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covered_by :
      _rtreeMap.query(spatial::bgi::covered_by(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covers :
      _rtreeMap.query(spatial::bgi::covers(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::disjoint :
      _rtreeMap.query(spatial::bgi::disjoint(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::intersects :
      _rtreeMap.query(spatial::bgi::intersects(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::overlaps :
      _rtreeMap.query(spatial::bgi::overlaps(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::within :
      _rtreeMap.query(spatial::bgi::within(query_box), boost::make_function_output_iterator(callback)); break;
    default:
      assert(false);
  }
}

template<typename T, typename Value>
void SpatialMap<T, Value>::queryBoth(const Box<T>& rect, Vector_t<spatial::b_value<T, Value> >& ret, Query_Type qt) const {
  spatial::SearchCallback<spatial::b_value<T, Value> > callback(ret);
  spatial::b_box<T> query_box(rect.min_corner(), rect.max_corner());
  switch (qt) {
    case Query_Type::contains :
      _rtreeMap.query(spatial::bgi::contains(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covered_by :
      _rtreeMap.query(spatial::bgi::covered_by(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covers :
      _rtreeMap.query(spatial::bgi::covers(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::disjoint :
      _rtreeMap.query(spatial::bgi::disjoint(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::intersects :
      _rtreeMap.query(spatial::bgi::intersects(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::overlaps :
      _rtreeMap.query(spatial::bgi::overlaps(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::within :
      _rtreeMap.query(spatial::bgi::within(query_box), boost::make_function_output_iterator(callback)); break;
    default:
      assert(false);
  }
}

template<typename T, typename Value>
void SpatialMap<T, Value>::queryBoth(const Point<T>& min_corner, const Point<T>& max_corner, Vector_t<spatial::b_value<T, Value> >& ret, Query_Type qt) const {
  spatial::SearchCallback<spatial::b_value<T, Value> > callback(ret);
  spatial::b_box<T> query_box(min_corner, max_corner);
  switch (qt) {
    case Query_Type::contains :
      _rtreeMap.query(spatial::bgi::contains(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covered_by :
      _rtreeMap.query(spatial::bgi::covered_by(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::covers :
      _rtreeMap.query(spatial::bgi::covers(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::disjoint :
      _rtreeMap.query(spatial::bgi::disjoint(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::intersects :
      _rtreeMap.query(spatial::bgi::intersects(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::overlaps :
      _rtreeMap.query(spatial::bgi::overlaps(query_box), boost::make_function_output_iterator(callback)); break;
    case Query_Type::within :
      _rtreeMap.query(spatial::bgi::within(query_box), boost::make_function_output_iterator(callback)); break;
    default:
      assert(false);
  }
}


PROJECT_NAMESPACE_END

#endif /// _GEO_SPATIAL_HPP_
