#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include <string>
#include <iostream>
#include <utility>

void BetaScope::_clearVecBuffer()
{
  for( auto i : this->oTreeSTLVecotr_Int_keeper ){ if(i){ i->clear(); } }
  for( auto i : this->oTreeSTLVecotr_Double_keeper ){ if(i){ i->clear(); } }
  for( auto i : this->oTreeSTLVecotr_Float_keeper ){ if(i){ i->clear(); } }
}

void BetaScope::_clearVecBuffer(std::string mode){
  for( auto i : this->oTreeSTLVecotr_Int_keeper ){ if(i){ i->clear(); } }
  for( auto i : this->oTreeSTLVecotr_Double_keeper ){ if(i){ i->clear(); } }
  for( auto i : this->oTreeSTLVecotr_Float_keeper ){ if(i){ i->clear(); } }
}
