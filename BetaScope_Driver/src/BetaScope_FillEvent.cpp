#include "BetaScope_Driver/include/BetaScope_Class.h"

//-------c++----------------//
#include <iostream>


void BetaScope::fillEvent()
{
    this->oTree->Fill();
    BetaScope::_clearVecBuffer();
}
