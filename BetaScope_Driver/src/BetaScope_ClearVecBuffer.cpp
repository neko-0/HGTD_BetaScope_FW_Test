#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include <iostream>
#include <string>
#include <utility>

void BetaScope::_ClearVecBuffer() {
  // for( auto &i : this->oTreeSTLVecotr_Int_keeper ){ if(i){ i->clear(); } }
  // for( auto &i : this->oTreeSTLVecotr_Double_keeper ){ if(i){ i->clear(); } }
  // for( auto &i : this->oTreeSTLVecotr_Float_keeper ){ if(i){ i->clear(); } }
  for (auto &val : this->output_vector_keeper_) {
    val->clear();
  }
}

void BetaScope::_ClearVecBuffer(std::string mode) {
  for (auto &val : this->output_vector_keeper_) {
    val->clear();
  }
}
