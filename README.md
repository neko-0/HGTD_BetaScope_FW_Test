# HGTD_BetaScope_FW_Test
Beta-Scope Ana Framework for analyzing waveform.

## Installations
To begin:

  cd scripts/ \
  source setup.sh

## Usage 

### LGAD Interface.
After sourcing the setup.sh, you will now have a function call "LGAD".
This will setup an command line wrapper for the beta-scope routine analysis scripts. Here is the example:

type "LGAD" in your terminal, you will now have a new terminal interface with prompt (LGAD).
You can also pass "--test" argument to "LGAD", which will invoke a testing version with prompt (Meow~). Some new features might be found in the testing version.

In this new terminal interface, in principle you will have all the usual terminal commands(e.g. ls, pwd, cat ...). Don't forget to use your smart TAB.

After entering the new terminal interface, you can use "?" to list all of the available commands.

To run routine analysis, you can follow this example:

  (LGAD) set_output_dir my_dir/testing_dir/ \
  (LGAD) set_run 624 \
  (LGAD) generate_config \
  (LGAD) set_default_config \
  (LGAD) run_analysis "nohup full"

In the last command, "nohup" suppress the output. This is same as the usual nohup. "full" will tell the analyzer to run the full routine analysis.
If "full" is not specified, the analyzer will only produce "stats_" files

extra options:
- "full": full analysis
- "ana_only": only produce stat files
- "res_only": only get results from stat files
- "no_autocut": get results without re-running autcut script (useful if autocut fails)

### BetaScope Waveform Ana

Routine waveform analysis for beta-scope charge collection measurement.

### Argone Xray Ana

Originate from SSRL bunch signals analysis.

####Updates

2019/8/27: only maximum and minimum peak of bunch signals are included for now, but it's enough to do some preliminary event filtering.

## Known issues

### LGAD Interface
Using "nohup" in run_analysis will create huge memory usage, but running the executable independently is fine. Try to avoid "nohup" for now.
