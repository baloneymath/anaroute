/**
 * @file   type.hpp
 * @brief  Types used across the project
 * @author Hao Chen
 * @date   09/01/2019
 *
 **/

#ifndef ANAROUTE_TYPE_HPP_
#define ANAROUTE_TYPE_HPP_

#include <cstdint>
#include <limits>
#include <climits>
#include <string>
#include "namespace.hpp"

PROJECT_NAMESPACE_START

using Int_t     = std::int32_t;
using Index_t   = std::uint32_t;
using Real_t    = double;
using Byte_t    = std::uint8_t;
using String_t  = std::string;
template<typename T, typename U>
using Pair_t    = std::pair<T, U>;
template<typename T>
using Vector_t  = std::vector<T>;
template<typename T>
using List_t    = std::list<T>;
template<typename T>
using Set_t     = std::set<T>;
template<typename T, typename U>
using Map_t     = std::map<T, U>;
template<typename T>
using USet_t    = std::unordered_set<T>;
template<typename T, typename U>
using UMap_t    = std::unordered_map<T, U>;

constexpr Int_t   MAX_INT   = std::numeric_limits<Int_t>::max()       / 3;
constexpr Index_t MAX_INDEX = std::numeric_limits<Index_t>::max()     / 3;
constexpr Real_t  MAX_REAL  = std::numeric_limits<Real_t>::max()      / 3;
constexpr Int_t   MIN_INT   = std::numeric_limits<Int_t>::lowest()    / 3;
constexpr Index_t MIN_INDEX = std::numeric_limits<Index_t>::lowest()  / 3;
constexpr Real_t  MIN_REAL  = std::numeric_limits<Real_t>::lowest()   / 3;

constexpr Real_t EPSILON   = 1e-8;

constexpr Real_t TIME_SCALE = 1000000.00;
constexpr Real_t MEM_SCALE  = 1024.0;


PROJECT_NAMESPACE_END

#endif /// ANAROUTE_TYPE_HPP_
