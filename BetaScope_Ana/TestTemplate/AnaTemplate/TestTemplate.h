#include "BetaScope_Driver/include/BetaScope_AnaFramework.h"
 
class TestTemplate : public BetaScope_AnaFramework<BetaScope>
{
	 private:

		 int private_variable_example = 1;

	 protected:

		 int protected_variable_example = 1;

	 public:

		 std::string ifile;

		 TestTemplate(){};
		 TestTemplate( std::string ifile )
		 {
				 std::cout<<"Using user template: " << "TestTemplate" << std::endl;
				 this->ifile = ifile;
		 }
		 ~TestTemplate(){};


		 void initialize();
		 void analysis();
		 void loopEvents();
		 void finalize();
		 void run()
		 {
				 initialize();
				 loopEvents();
				 finalize();
		 }
}
