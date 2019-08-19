'''
commandline oriented interface using the cmd module for the beta-scope analysis
'''

import readline
readline.get_completer_delims()
#readline.set_completer_delims(' \t\n;')

import glob
import os
import sys
import cmd
import subprocess
import threading
import multiprocessing as mp
from colorStringFormating import *

predefined_path = {
"__raw":"/media/mnt/BigHD/Beta_DAQ_Data/",
"__yuzhan":"/media/mnt/BigHD/BetaScope_Data/Analyzed_YZ/"
}

class Lgad(cmd.Cmd, object):

    intro = colorString.colorFormat("   Interface for beta-scope analysis", "cyan")
    prompt = colorString.colorFormat("(LGAD) ", "yellow")

    global predefined_path

    def __init__(self):
        self.raw_dir = predefined_path["__raw"]
        self.files = os.listdir(os.getcwd())
        cmd.Cmd.__init__(self)

    def cmdloop(self, intro=None):
        print(self.intro)
        while True:
            try:
                super(Lgad, self).cmdloop(intro="")
            except KeyboardInterrupt:
                colorString.sysError("You have pressed ctrl+C! do you want to exit?(press again to exit)" )
                raw_input()

    def completedefault(self, text, line, begidx, endidx):
        return [i for i in self.files if i.startswith(text)]

    def do_cd(self, tdir):
        "change direcotry, similar to the usual cd in cml."
        #glob.glob(os.path.expanduser(str(tdir)+"*"))
        #os.path.expanduser(tdir)
        self.files = os.listdir(os.getcwd())
        if tdir:
            pass
        else:
            tdir = "."
        if "~" in tdir:
            tdir = os.path.expanduser("~") + tdir.split("~")[1]

        if tdir in predefined_path:
            tdir = predefined_path[tdir]

        try:
            os.chdir(tdir)
            colorString.sysMsg("changed dir to {}".format(tdir) )
        except OSError:
            print(colorString.colorFormat("Cannot find {}".format(tdir), "red"))

    #'''
    def default(self, line):
        subprocess.call(line, shell=True)
    #'''

    '''
    def do_pwd(self, tdir=""):
        print( os.getcwd() )
    '''

    def do_set_output_dir(self, path ):
        "Setup the stats file output direcotry. If you have predefined path, you can use it. "
        if path in predefined_path:
            self.output_dir = predefined_path[path]
            colorString.sysMsg("output path is set to {}".format(self.output_dir) )
        else:
            try:
                self.output_dir = predefined_path[path]
                colorString.sysMsg("output path is set to {}".format(self.output_dir) )
            except:
                colorString.sysError("cannot to output path to {}".format(self.output_dir) )


    def do_set_run( self, runNum):
        "Setup a run for analysis. It will automatically search the run number in the pre-defined raw data direcotry. If it can find the run number , it will create a folder for the run in your output direcotry"
        if hasattr(self, "output_dir"):
            if self.output_dir:
                pass
            else:
                colorString.sysError("output direcotry has not been set. Please run set_output_dir" )
        else:
            colorString.sysError("output direcotry has not been set. Please run set_output_dir" )

        for d in os.listdir( self.raw_dir ):
            if "Sr_Run"+str( runNum ) in d:
                self.runNum = runNum
                self.runNum_dir = d


        if hasattr(self, "runNum"):
            if not os.path.isdir(self.output_dir + "/" + self.runNum_dir):
                os.mkdir( self.output_dir + "/" + self.runNum_dir )
                self.current_run = self.output_dir + "/" + self.runNum_dir
            else:
                self.current_run = self.output_dir + "/" + self.runNum_dir
                colorString.sysError("direcotry {} is already there".format(self.current_run) )
        else:
            colorString.sysError("No run number {}".format(runNum) )

    def do_cd_current_run(self, tdir=""):
        "change direcotry to the run you set from set_run"
        self.do_cd( self.current_run )


    def do_generate_config(self, place_holder=""):
        "Generate configuration file for the beta-scope waveform analyzer."
        subprocess.call("${BETASCOPE_SCRIPTS}/../BetaScope_Ana/BetaScopeWaveformAna/bin/GenerateWaveformConfig", shell=True)

    def do_set_default_config(self, place_holder=""):
        "Set the configuration file to default for beta-scope waveform analyzer"
        import configparser

        global __raw_data_dir
        global __data_output_dir
        global __runNum

        parser = configparser.ConfigParser()
        parser.optionxform = str
        parser.read("WaveformAnaConfig.ini")
        parser.set("General","rawFilesDir", "{raw_dir}/{raw_data}/fromDAQ/".format(raw_dir=__raw_data_dir, raw_data=__runNum) )
        parser.set("Channel_Activation", "channel_2", "1")
        parser.set("Channel_Activation", "channel_3", "1")
        parser.set("Channel_Invertion", "channel_2", "1")
        with open("WaveformAnaConfig.ini", "wb") as config:
            parser.write(config)

    def do_set_active_channel(self, ch):
        import configparser
        parser = configparser.ConfigParser()
        parser.set("Channel_Activation", "channel_{ch}".format(ch=ch), 1)
        with open("WaveformAnaConfig.ini", "wb") as config:
            parser.write(config)


    def do_show_ana_progress(self, line=""):
        os.system("tail $BETASCOPE_SCRIPTS/nohup.log")

    def do_run_analysis(self, mode=""):
        "Run routine beta-scope analysis. Argument with 'full' will do the full rountine analysis, else it will only generate stats files. Argument with 'nohup' will supress the output "
        if not hasattr(self, "current_run"):
            colorString.sysError("current run is not set" )

        else:
            self.do_cd_current_run()

            if "nohup" in mode:
                nohup = "nohup"
                nohup_log = " >> $BETASCOPE_SCRIPTS/nohup.log"

                def isRunning(pid):
                    while True:
                        if os.path.isdir("/proc/{}".format(pid)):
                            continue
                        else:
                            break

            else:
                nohup = ""
                nohup_log = ""

            if not nohup:
                p = subprocess.Popen("{nohup} $BETASCOPE_SCRIPTS/../BetaScope_Ana/BetaScopeWaveformAna/bin/Run_WaveformAna {tdir}/WaveformAnaConfig.ini --skipWaveform {nohup_log}".format(nohup=nohup, nohup_log=nohup_log, tdir=self.current_run), shell=True)
                p.wait()

                if "full" in mode:
                    p = subprocess.Popen("{} /home/yuzhan/HGTD_BetaScope/BetaScopeDataProcessor/bin/GenerateDataProcessorConfig.exe {}".format(nohup, nohup_log), shell=True)
                    p.wait()

                    p = subprocess.call("{nohup} python2 $BETASCOPE_SCRIPTS/betaScope_pyScript/autoCut_v2.py --runNum {num} {nohup_log}".format(num=__num, nohup=nohup, nohup_log=nohup_log), shell=True)

                    p = subprocess.Popen("{nohup} /home/yuzhan/HGTD_BetaScope/BetaScopeDataProcessor/bin/GetResults.exe run_info_v08022018.ini {nohup_log}".format(nohup=nohup, nohup_log=nohup_log), shell=True)
                    p.wait()
            else:
                def nohupRun(mode):
                    p = subprocess.Popen("{nohup} $BETASCOPE_SCRIPTS/../BetaScope_Ana/BetaScopeWaveformAna/bin/Run_WaveformAna {tdir}/WaveformAnaConfig.ini --skipWaveform {nohup_log}".format(nohup=nohup, nohup_log=nohup_log, tdir=self.current_run), shell=True)
                    #pid = p.pid
                    #isRunning(pid)
                    p.wait()
                    if "full" in mode:
                        p = subprocess.Popen("{} /home/yuzhan/HGTD_BetaScope/BetaScopeDataProcessor/bin/GenerateDataProcessorConfig.exe {}".format(nohup, nohup_log), shell=True)
                        #pid = p.pid
                        #isRunning(pid)
                        p.wait()

                        p = subprocess.call("{nohup} python2 $BETASCOPE_SCRIPTS/betaScope_pyScript/autoCut_v2.py --runNum {num} {nohup_log}".format(num=__num, nohup=nohup, nohup_log=nohup_log), shell=True)

                        p = subprocess.Popen("{nohup} /home/yuzhan/HGTD_BetaScope/BetaScopeDataProcessor/bin/GetResults.exe run_info_v08022018.ini {nohup_log}".format(nohup=nohup, nohup_log=nohup_log), shell=True)
                        p.wait()

                #job = threading.Thread(name="nohupRun", target=nohupRun, args=(mode,) )
                job = mp.Process(target=nohupRun, args=(mode,) )
                job.daemon = True 
                job.start()


if __name__ == "__main__":
    interface = Lgad()
    interface.cmdloop()
