#include "ACLGAD.h"

void ACLGAD::initialize()
{
	 //required
	 std::string function_name = "ACLGAD::initialize";
	 this->beta_scope.fileIO_Open( ifile.c_str() );
	 BetaScope_AnaFramework::initialize("");

	 //user implementation
	 this->beta_scope.buildPrimitiveBranch<int>("counter");
}

void ACLGAD::loopEvents()
{
	 // this is default implementation
	 BetaScope_AnaFramework::loopEvents( &BetaScope_AnaFramework::analysis );
}

void ACLGAD::finalize()
{
	 // this is default implementation
	 BetaScope_AnaFramework::finalize();
}
void ACLGAD::analysis()
{
	 // this is example implementation
	 *this->beta_scope.get_oTree_PrimitiveBranch<int>("counter") = 1;
}
