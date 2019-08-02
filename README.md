# HGTD_BetaScope_FW_Test
Beta-Scope Ana Framework

To begin:
   cd scripts
   source setup.sh


About the meow.py helper script:

   This helper script provides a simple interface with the BetaScope Waveform Analysis and the plotting scripts I am using.

   To run analysis with this helper script, do:

   type 'meow' after sourcing the setup script. If there's no compilation error, things should be working.

   and then type the following:

   set_output_dir( your_working_dir) < this sets up the working directory
   set_run(612) < setting the run that you want to analyze. It will look into the usual DAQ dir and find the run
   generate_config()
   set_default_config() < most of the time you can use this
   run_analysis()

   at the end you should have the usual _results.xlsx (or .ini) in the 'Results' directory.  
