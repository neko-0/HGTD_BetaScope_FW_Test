'''
python class for generating analysis template
'''
import os

class TemplateGen(object):

    def __init__(self, template_name, beta_scope_mode):
        self.template_name = template_name
        self.beta_scope_mode = beta_scope_mode
        self.new_template_dir = "../../BetaScope_Ana/{new_template}".format(new_template=self.template_name)

    def _generate_new_dir(self):

        if not os.path.exists( self.new_template_dir ):
            os.makedirs( self.new_template_dir)

            if not os.path.exists( self.new_template_dir+"/AnaTemplate/" ):
                os.makedirs( self.new_template_dir+"/AnaTemplate/" )
            if not os.path.exists( self.new_template_dir+"/bin/" ):
                os.makedirs( self.new_template_dir+"/bin/" )
            if not os.path.exists( self.new_template_dir+"/build/" ):
                os.makedirs( self.new_template_dir+"/build/" )
            if not os.path.exists( self.new_template_dir+"/main/" ):
                os.makedirs( self.new_template_dir+"/main/" )

    def _generate_header_file(self):

        with open("{}/AnaTemplate/{}.h".format(self.new_template_dir, self.template_name), "w+" ) as header:
            header.write('#include "BetaScope_Driver/include/BetaScope_AnaFramework.h"' )
            header.write('\n \n')

            header.write('class {} : public BetaScope_AnaFramework<{}>\n'.format(self.template_name, self.beta_scope_mode) )
            header.write('{\n')
            header.write('\t private:\n')
            header.write('\n')
            header.write('\t\t int private_variable_example = 1;\n' )
            header.write('\n')
            header.write('\t protected:\n')
            header.write('\n')
            header.write('\t\t int protected_variable_example = 1;\n' )
            header.write('\n')

            header.write('\t public:\n')
            header.write('\n')
            header.write('\t\t std::string ifile;\n')
            header.write('\n')
            header.write('\t\t {}(){{}};\n'.format(self.template_name) )
            header.write('\t\t {}( std::string ifile )\n'.format(self.template_name) )
            header.write('\t\t {\n')
            header.write('\t\t\t\t std::cout<<"Using user template: " << "{}" << std::endl;\n'.format(self.template_name) )
            header.write('\t\t\t\t this->ifile = ifile;\n' )
            header.write('\t\t }\n')
            header.write('\t\t ~{}(){{}};\n'.format(self.template_name) )
            header.write('\n\n')
            header.write('\t\t void initialize();\n')
            header.write('\t\t void analysis();\n')
            header.write('\t\t void loopEvents();\n')
            header.write('\t\t void finalize();\n')
            header.write('\t\t void run()\n')
            header.write('\t\t {\n')
            header.write('\t\t\t\t initialize();\n')
            header.write('\t\t\t\t loopEvents();\n')
            header.write('\t\t\t\t finalize();\n')
            header.write('\t\t }\n')
            header.write('}\n')

    def _generate_cpp_file(self):
        with open("{}/AnaTemplate/{}.cpp".format(self.new_template_dir, self.template_name), "w+" ) as cpp:
            cpp.write('#include "{}.h"\n'.format(self.template_name) )
            cpp.write('\n')
            cpp.write('void {}::initialize()\n'.format(self.template_name) )
            cpp.write('{\n')
            cpp.write('\t //required \n')
            cpp.write('\t std::string function_name = "{}::initialize";\n'.format(self.template_name) )
            cpp.write('\t this->beta_scope.fileIO_Open( ifile.c_str() );\n')
            cpp.write('\t BetaScope_AnaFramework::initialize("");\n')
            cpp.write('\n')
            cpp.write('\t //user implementation \n')
            cpp.write('\t my_counting_branch = this->beta_scope.buildPrimitiveBranch<int>("counter");')
            cpp.write('\t *this->beta_scope.buildPrimitiveBranch<int>("counter") = 1;\n')
            cpp.write('}\n')
            cpp.write('\n')

            cpp.write('void {}::loopEvetns()\n'.format(self.template_name) )
            cpp.write('{\n')
            cpp.write('\t // this is default implementation \n')
            cpp.write('\t BetaScope_AnaFramework::loopEvents( &BetaScope_AnaFramework::analysis );\n' )
            cpp.write('}\n')

            cpp.write("\n")

            cpp.write('void {}::finalize()\n'.format(self.template_name) )
            cpp.write('{\n')
            cpp.write('\t // this is default implementation \n')
            cpp.write('\t BetaScope_AnaFramework::finalize();\n' )
            cpp.write('}\n')

            cpp.write('void {}::analysis()\n'.format(self.template_name) )
            cpp.write('{\n')
            cpp.write('\t // this is example implementation \n')
            cpp.write('\t *this->beta_scope.get_oTree_PrimitiveBranch<int>("counter")++;\n' )
            cpp.write('}\n')

    def _generate_sample_main(self):
        with open("{}/main/main_{}.cpp".format(self.new_template_dir, self.template_name), "w+") as main_f:
            main_f.write('#include "BetaScope_Ana/{temp}/AnaTemplate/{temp}.h" \n'.format(temp=self.template_name) )
            main_f.write('\n')
            main_f.write('int main(int argc, char **argv)\n')
            main_f.write('{\n')
            main_f.write('\t {0} my_{0}(argv[1]);\n'.format(self.template_name) )
            main_f.write('\t my_{}.run();\n'.format(self.template_name) )
            main_f.write('}\n')

    def generate_template(self):
        self._generate_new_dir()
        self._generate_header_file()
        self._generate_cpp_file()
        self._generate_sample_main()
