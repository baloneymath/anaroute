#include "lefViaRule.hpp"
#include <limbo/parsers/lef/adapt/LefDriver.h>

PROJECT_NAMESPACE_START

/// @brief check if this is a enclosure + width layer
/// @return true if the lefiViaLayer is fitted into enclosure and width template
bool isEnclosureWidthViaLayer(lefiViaRuleLayer &l)
{
    if (l.hasDirection() == 1)
    {
        return false;
    }
    if (l.hasEnclosure() == 0)
    {
        return false;
    }
    if (l.hasWidth() == 0)
    {
        return false;
    }
    if (l.hasResistance() == 1)
    {
        return false;
    }
    if (l.hasOverhang() == 1)
    {
        return false;
    }
    if (l.hasMetalOverhang() == 1)
    {
        return false;
    }
    if (l.hasSpacing() == 1)
    {
        return false;
    }
    if (l.hasRect() == 1)
    {
        return false;
    }
    return true;
}

/// @brief check if this is a rect + spacing layer
/// @return true if this lefiViaRuleLayer is fitted into width + spacing template
bool isRectSpacingViaRule(lefiViaRuleLayer &l)
{
    if (l.hasDirection() == 1)
    {
        return false;
    }
    if (l.hasEnclosure() == 1)
    {
        return false;
    }
    if (l.hasWidth() == 1)
    {
        return false;
    }
    if (l.hasResistance() == 1)
    {
        return false;
    }
    if (l.hasOverhang() == 1)
    {
        return false;
    }
    if (l.hasMetalOverhang() == 1)
    {
        return false;
    }
    if (l.hasSpacing() == 0)
    {
        return false;
    }
    if (l.hasRect() == 0)
    {
        return false;
    }
    return true;
}

bool LefViaRuleTemplate1::constructFromLefParser(const LefDefParser::lefiViaRule &v)
{
    if (v.hasGenerate() == 0)
    {
        return false;
    }
    if (v.hasDefault() == 1)
    {
         return false;
    }
    this->name = std::string(v.name());
    if (v.numLayers() != 3)
    {
        return false;
    }
    if (v.numVias() > 0)
    {
        return false;
    }
    if (v.numProps() > 0)
    {
        return false;
    }
    name = std::string(v.name());
    // Construct the layers
    Int_t numMetalLayers = 0;
    Int_t numViaLayers = 0;
    for (Int_t layerIdx = 0; layerIdx < 3; ++layerIdx) // have checked there were three layers
    {
        auto layerPtr = v.layer(layerIdx);
        if (isEnclosureWidthViaLayer(*layerPtr))
        {
            if (numMetalLayers == 0)
            {
                // layer 1
                numMetalLayers = 1;
                metalLayerNames.at(0) = std::string(layerPtr->name());
                enclosureOverhang1.at(0) = layerPtr->enclosureOverhang1();
                enclosureOverhang2.at(0) = layerPtr->enclosureOverhang2();
                widthLo.at(0) = layerPtr->widthMin();
                widthHi.at(0) = layerPtr->widthMax();
            }
            else if (numMetalLayers == 1)
            {
                // layer 2
                numMetalLayers = 2;
                metalLayerNames.at(1) = std::string(layerPtr->name());
                enclosureOverhang1.at(1) = layerPtr->enclosureOverhang1();
                enclosureOverhang2.at(1) = layerPtr->enclosureOverhang2();
                widthLo.at(1) = layerPtr->widthMin();
                widthHi.at(1) = layerPtr->widthMax();
            }
            else
            {
                // There are more than two
                return false;
            }
        }
        else if (isRectSpacingViaRule(*layerPtr))
        {
            if (numViaLayers == 0)
            {
                // layer 3
                numViaLayers = 1;
                viaLayerName = std::string(layerPtr->name());
                rectXLo = layerPtr->xl();
                rectYLo = layerPtr->yl();
                rectXHi = layerPtr->xh();
                rectYHi = layerPtr->yh();
                spacingX = layerPtr->spacingStepX();
                spacingY = layerPtr->spacingStepY();
            }
            else
            {
                return false;
            }
        }
        else
        {
            // Three are more than one
            return false;
        }
    }
    return true;
}

std::string LefViaRuleTemplate1::str() const
{
    std::stringstream ss;
    ss<< "Name " << name <<"\n";
    ss << "Layer "<< metalLayerNames[0] <<"\n";
    ss << "Enclosure " << enclosureOverhang1[0] <<" " << enclosureOverhang2[0] <<"\n";
    ss << " Width "<< widthLo[0] << " "<< widthHi[0] << "\n";
    ss << "Layer "<< metalLayerNames[1] <<"\n";
    ss << "Enclosure " << enclosureOverhang1[1] <<" " << enclosureOverhang2[1] <<"\n";
    ss << " Width "<< widthLo[1] << " "<< widthHi[1] << "\n";
    ss << "Layer " << viaLayerName << "\n";
    ss << "rect " << rectXLo << " " << rectYLo << " " << rectXHi << " "<< rectYHi << "\n";
    ss << "spacing " <<  spacingX << " " << spacingY << "\n";
    return ss.str();
}

PROJECT_NAMESPACE_END
