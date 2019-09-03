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
#include "namespace.hpp"

PROJECT_NAMESPACE_START

using Int_t   = std::int32_t;
using Index_t = std::uint32_t;
using Real_t  = double;
using Loc_t   = std::int32_t; // Location
using Byte_t  = std::uint8_t;

constexpr Int_t   MAX_INT   = std::numeric_limits<Int_t>::max()       / 3;
constexpr Index_t MAX_INDEX = std::numeric_limits<Index_t>::max()     / 3;
constexpr Real_t  MAX_REAL  = std::numeric_limits<Real_t>::max()      / 3;
constexpr Loc_t   MAX_LOC   = std::numeric_limits<Loc_t>::max()       / 3;
constexpr Int_t   MIN_INT   = std::numeric_limits<Int_t>::lowest()    / 3;
constexpr Index_t MIN_INDEX = std::numeric_limits<Index_t>::lowest()  / 3;
constexpr Real_t  MIN_REAL  = std::numeric_limits<Real_t>::lowest()   / 3;
constexpr Loc_t   MIN_LOC   = std::numeric_limits<Loc_t>::lowest()    / 3;

constexpr Real_t EPSILON   = 1e-8;

constexpr Real_t TIME_SCALE = 1000000.00;
constexpr Real_t MEM_SCALE  = 1024.0;


PROJECT_NAMESPACE_END

#endif /// ANAROUTE_TYPE_HPP_
