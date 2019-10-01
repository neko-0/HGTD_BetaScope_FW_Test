#include "TestTemplate.h"

void TestTemplate::initialize()
{
	 //required 
	 std::string function_name = "TestTemplate::initialize";
	 this->beta_scope.fileIO_Open( ifile.c_str() );
	 BetaScope_AnaFramework::initialize("");

	 //user implementation 
	 my_counting_branch = this->beta_scope.buildPrimitiveBranch<int>("counter");	 *this->beta_scope.buildPrimitiveBranch<int>("counter") = 1;
}

void TestTemplate::loopEvetns()
{
	 // this is default implementation 
	 BetaScope_AnaFramework::loopEvents( &BetaScope_AnaFramework::analysis );
}

void TestTemplate::finalize()
{
	 // this is default implementation 
	 BetaScope_AnaFramework::finalize();
}
void TestTemplate::analysis()
{
	 // this is example implementation 
	 *this->beta_scope.get_oTree_PrimitiveBranch<int>("counter")++;
}
