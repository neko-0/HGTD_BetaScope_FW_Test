#include "BetaScope_Driver/include/BetaScope_AnaFramework.h"

class ACLGAD : public BetaScope_AnaFramework<BetaScope_Ext>
{
	 private:

		 int private_variable_example = 1;

	 protected:

		 int protected_variable_example = 1;

	 public:

		 std::string ifile;

		 ACLGAD(){};
		 ACLGAD( std::string ifile )
		 {
				 std::cout<<"Using user template: " << "ACLGAD" << std::endl;
				 this->ifile = ifile;
		 }
		 ~ACLGAD(){};


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
};
