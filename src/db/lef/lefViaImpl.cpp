#include "lefViaImpl.hpp"
#include "lefVia.hpp"

PROJECT_NAMESPACE_START

LefViaImplementor::LefViaImplementor(LefVia *lefVia)
{
  _pLefVia = lefVia;
}

bool LefViaImplementor::configureOnMetalWidthHeight(
    Int_t botWidth, Int_t botHeight, Int_t topWidth, Int_t topHeight)
{
}

PROJECT_NAMESPACE_END
