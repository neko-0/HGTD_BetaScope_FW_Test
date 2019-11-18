#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

//-------c++----------------//
#include <iostream>


void BetaScope::fillEvent()
{
    this->oTree->Fill();
    BetaScope::_clearVecBuffer("default");
}
