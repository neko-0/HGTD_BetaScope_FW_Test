#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

//-------c++----------------//
#include <iostream>

void BetaScope::FillEvent() {
  this->output_ttree_->Fill();
  BetaScope::_ClearVecBuffer("default");
}
