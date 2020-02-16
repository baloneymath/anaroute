/**
 * @file   lefViaImpl.hpp
 * @brief  The configurable LefVia
 * @author Keren Zhu
 * @date   02/12/2020
 *
 **/

#ifndef DB_LEF_VIA_IMPL_HPP_
#define DB_LEF_VIA_IMPL_HPP_

#include "src/db/lef/lefViaRule.hpp"

PROJECT_NAMESPACE_START

class LefVia; 
/// @brief the class for the via configuration rule with two enclusre rule
class LefViaEnclosureConfig
{
  public:
    explicit LefViaEnclosureConfig() = default;
    explicit LefViaEnclosureConfig(const std::array<Int_t, 2> &enclosureOverhang1, const std::array<Int_t, 2> &enclosureOverhang2)
      : _enclosureOverhang1(enclosureOverhang1), _enclosureOverhang2(enclosureOverhang2) {}
    void configure(const std::array<Int_t, 2> &enclosureOverhang1, const std::array<Int_t, 2> &enclosureOverhang2)
    {
      _enclosureOverhang1 = enclosureOverhang1;
      _enclosureOverhang2 = enclosureOverhang2;
    }
  protected:
      std::array<Int_t, 2> _enclosureOverhang1;
      std::array<Int_t, 2> _enclosureOverhang2;
};

/// @brief the class for the via configuration rule with two enclusre rule
class LefViaWidthConfig
{
  public:
    explicit LefViaWidthConfig() = default;
    explicit LefViaWidthConfig(const std::array<Int_t, 2> &widthLo, const std::array<Int_t, 2> &widthHi)
      : _widthLo(widthLo), _widthHi(widthHi) {}
    void configure(const std::array<Int_t, 2> &widthLo, const std::array<Int_t, 2> &widthHi)
    {
      _widthLo = widthLo;
      _widthHi = widthHi;
    }
  protected:
      std::array<Int_t, 2> _widthLo;
      std::array<Int_t, 2> _widthHi;
};



class LefViaImplementor 
{
  public:
    explicit LefViaImplementor() = default;
    explicit LefViaImplementor(LefVia *lefVia);
    /// @brief configure the width and height for the via metals
    /// @param first: target width for bottom metal. -1 if don't care
    /// @param second: target height for bottom metal. -1 if don't care
    /// @param third: target width for top metal. -1 if don't care
    /// @param fourth: target height for top metal. -1 if don't care
    /// @return true: if configuration success. false: failed. the original shapes should not change
    bool configureOnMetalWidthHeight(Int_t botWidth, Int_t botHeight, Int_t topWidth, Int_t topHeight);
    /* Config */
    LefViaEnclosureConfig &metalEnclosureConfig() { return _metalEnclosureConfig; }
    LefViaWidthConfig &metalWidthConfig() { return _metalWidthConfig; }
  protected:
    LefVia *_pLefVia = nullptr;
    LefViaEnclosureConfig _metalEnclosureConfig;
    LefViaWidthConfig _metalWidthConfig;
};


/// @brief The traits for viarules
template<typename ViaRuleType>
struct LefViaImplementorConceptTraits
{
  typedef typename ViaRuleType::LefViaImplType ImplType;
  static inline void configure(const ViaRuleType &viaRule, ImplType &impl)
  {
    viaRule.config(impl);
  }
  static const bool enableBotMetalWidth = false;
  static const bool enableBotMetalHeight = false;
  static const bool enableTopMetalWidth = false;
  static const bool enableTopMetalHeight = false;
  static const bool hasBotMetalMinWidthRule = false;
  static const bool hasBotMetalMaxWidthRule = false;
  static const bool hasTopMetalMinWidthRule = false;
  static const bool hasTopMetalMaxWidthRule = false;
  static const bool hasBotMetalEnclosureRule = false;
  static const bool hasTopMetalEnclosureRule = false;
};


/// @brief custom LefViaImplementorConceptTraits for ViaRuleTemplate1
template<>
struct LefViaImplementorConceptTraits<LefViaRuleTemplate1>
{
  static inline void configure(const LefViaRuleTemplate1 &viaRule, LefViaImplementor &impl)
  {
    impl.metalEnclosureConfig().configure(viaRule.enclosureOverhang1, viaRule.enclosureOverhang2);
    impl.metalWidthConfig().configure(viaRule.widthLo, viaRule.widthHi);
  }
  static const bool enableBotMetalWidth = true;
  static const bool enableBotMetalHeight = true;
  static const bool enableTopMetalWidth = true;
  static const bool enableTopMetalHeight = true;
  static const bool hasBotMetalMinWidthRule = true;
  static const bool hasBotMetalMaxWidthRule = true;
  static const bool hasTopMetalMinWidthRule = true;
  static const bool hasTopMetalMaxWidthRule = true;
  static const bool hasBotMetalEnclosureRule = true;
  static const bool hasTopMetalEnclosureRule = true;
};

PROJECT_NAMESPACE_END

#endif //DB_LEF_VIA_IMPL_HPP_
