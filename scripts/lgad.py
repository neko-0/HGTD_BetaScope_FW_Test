"""
commandline oriented interface using the cmd module for the beta-scope analysis
"""

import readline

readline.get_completer_delims()
# readline.set_completer_delims(' \t\n;')

import glob
import os
import sys
import cmd
import subprocess
import threading
import time
import pandas
import multiprocessing as mp
from Gdrive_interface.Gdrive_interface import gdrive_interface

from shutil import copyfile, copy, move
from colorStringFormating import *
sys.path.append('/home/datataking/betascope-plots/')

predefined_path = {
    "": "/media/mnt/COVID-19/betaAna4/",
    "__raw": "/media/mnt/BigHD/Beta_DAQ_Data/",
    "__raw2": "/media/mnt/gunter/Beta_DAQ_Data_2/",
    "__yuzhan": "/media/mnt/BigHD/BetaScope_Data/Analyzed_YZ/",
    "__simone": "/media/mnt/BigHD/BetaScope_Data/Analyzed_Simone/",
    "__yuzhan2": "/media/mnt/gunter/betaAna2/",
    "__covid": "/media/mnt/COVID-19/betaAna4/",
    "__gunter": "/media/mnt/gunter/betaAna2/",
    "__old_remake": "/media/mnt/gunter/betaAna3/",
    "__central_data_folder": "/media/mnt/COVID-19/Central_data_folder/",
}

SRC_PATH = [
"/media/mnt/BigHD/Beta_DAQ_Data/",
"/media/mnt/gunter/Beta_DAQ_Data_2/",
"/media/mnt/COVID-19/Beta_DAQ_Data_3/",
]

class Lgad(cmd.Cmd, object):

    intro = colorString.colorFormat("   Interface for beta-scope analysis", "cyan")
    prompt = colorString.colorFormat("(LGAD) ", "yellow")

    global predefined_path

    def __init__(self):
        self.raw_dir = SRC_PATH
        self.files = os.listdir(os.getcwd())
        self.central_data_folder = predefined_path["__central_data_folder"]
        self.package_dir = os.environ["BETASCOPE_SCRIPTS"]
        self.output_dir = predefined_path[""]
        try:
            copyfile(
                "{}/user_data/merged_beta_results.xlsx".format(self.package_dir),
                "{}/user_data/bkup/merged_beta_results_{}.xlsx".format(
                    self.package_dir, str(time.time())
                ),
            )
            copyfile(
                "{}/user_data/merged_log.json".format(self.package_dir),
                "{}/user_data/bkup/merged_log_{}.json".format(
                    self.package_dir, str(time.time())
                ),
            )
        except:
            pass
        cmd.Cmd.__init__(self)

    def cmdloop(self, intro=None):
        print(self.intro)
        while True:
            try:
                super(Lgad, self).cmdloop(intro="")
            except KeyboardInterrupt:
                colorString.sysError(
                    "You have pressed ctrl+C! do you want to exit?(press again to exit)"
                )
                raw_input()

    def completedefault(self, text, line, begidx, endidx):
        return [i for i in self.files if i.startswith(text)]

    def emptyline(self):
        pass

    def do_cd(self, tdir):
        "change directory, similar to the usual cd in cml."
        # glob.glob(os.path.expanduser(str(tdir)+"*"))
        # os.path.expanduser(tdir)
        if tdir:
            pass
        else:
            tdir = "."
        if "~" in tdir:
            tdir = os.path.expanduser("~") + tdir.split("~")[1]
            self.files = os.listdir(os.getcwd())

        if tdir in predefined_path:
            tdir = predefined_path[tdir]
            self.files = os.listdir(os.getcwd())

        try:
            os.chdir(tdir)
            colorString.sysMsg(f"changed dir to {tdir}")
            self.files = os.listdir(os.getcwd())
        except OSError:
            print(colorString.colorFormat(f"Cannot find {tdir}", "red"))

    #'''
    def default(self, line):
        subprocess.call(line, shell=True)

    #'''

    """
    def do_pwd(self, tdir=""):
        print( os.getcwd() )
    """

    def do_set_output_dir(self, path):
        "Setup the stats file output directory. If you have predefined path, you can use it. "
        if path in predefined_path:
            self.output_dir = predefined_path[path]
            colorString.sysMsg("output path is set to {}".format(self.output_dir))
        else:
            try:
                self.output_dir = path
                colorString.sysMsg("output path is set to {}".format(self.output_dir))
            except:
                colorString.sysError(
                    "cannot to output path to {}".format(self.output_dir)
                )

    def do_unpack(self, tdir=""):
        "unpack segmented data"
        self.do_cd(f"{self.my_raw_dir}{self.runNum_dir}/fromDAQ/")
        if not os.path.isdir("raw"):
            os.mkdir("raw")
            os.system("mv *.root* raw/")
            self.do_cd(f"{self.my_raw_dir}{self.runNum_dir}/fromDAQ/")
            subprocess.call("/home/datataking/DAQForProduction/utility/unpack.sh")
        else:
            colorString.sysError("Raw directory already there, is the data already unpacked?")
        self.do_cd_current_run()

    def do_reanalysis(self, runs):
        "Re-run full analysis on multiple runs passed as 001,002,003 ... or 001to005"

        self.do_download_latest_UDI()

        run_ns = []
        if "to" in runs:
            min = int(runs.split("to")[0])
            max = int(runs.split("to")[1])
            print( min, max)
            for run in range(min, max + 1): run_ns.append(str(run))
        else:
            for run in runs.split(","): run_ns.append(run)

        print(run_ns)
        for run in run_ns:
            self.do_set_run(run)
            self.do_cd(self.current_run)

            os.system(f"rm raw_results.root")
            os.system(f"rm _results.root")
            os.system(f"rm -r Results")

            self.do_run_analysis("res_only")

            os.system(f"rm -r {self.central_data_folder}/Folders/{self.runNum_dir}")
            self.do_cp_central_data(self.runNum)

        self.do_upload_gdrive()

    def do_replot(self, runs):
        "Re-run plotting on multiple runs passed as 001,002,003 ... or 001to005"

        self.do_download_latest_UDI()

        run_ns = []
        if "to" in runs:
            min = int(runs.split("to")[0])
            max = int(runs.split("to")[1])
            print( min, max)
            for run in range(min, max + 1): run_ns.append(str(run))
        else:
            for run in runs.split(","): run_ns.append(run)

        print(run_ns)
        for run in run_ns:
            self.do_set_run(run)
            self.do_run_analysis("plot")

            os.system(f"rm -r {self.central_data_folder}/Folders/{self.runNum_dir}")
            self.do_cp_central_data(self.runNum)

        self.do_upload_gdrive()

    def do_download_latest_UDI(self, runNum=""):
        gdrive = gdrive_interface()
        self.do_cd(self.central_data_folder)
        gdrive.download_UDI_file()

    def do_upload_gdrive(self, runNum=""):
        gdrive = gdrive_interface()

        # Delete everything
        gdrive.empty_central_folder()

        # Create folders
        zip_folder = gdrive.create_folder("Compressed_folders")
        root_folder = gdrive.create_folder("Root_files")
        excel_folder = gdrive.create_folder("Excel_files")
        average_folder = gdrive.create_folder("Average_waveform_files")

        # Upload
        self.do_cd(f"{self.central_data_folder}/Compressed_folders")
        print(f"Uploading compressed folders to {zip_folder['id']}")
        for f in glob.glob("*.zip"):
            gdrive.upload_file(f, [zip_folder['id']])

        self.do_cd(f"{self.central_data_folder}/Root_files")
        print(f"Uploading cumulative rootfiles to {root_folder['id']}")
        for f in glob.glob("*.root"):
            gdrive.upload_file(f, [root_folder['id']])

        self.do_cd(f"{self.central_data_folder}/Root_files/Singles")
        root_folder = gdrive.create_folder("Singles", [root_folder['id']])
        print(f"Uploading single rootfiles to {root_folder['id']}")
        for f in glob.glob("*.root"):
            gdrive.upload_file(f, [root_folder['id']])

        self.do_cd(f"{self.central_data_folder}/Excel_files")
        print(f"Uploading cumulative excel files to {excel_folder['id']}")
        for f in glob.glob("*.xlsx"):
            gdrive.upload_file(f, [excel_folder['id']])

        self.do_cd(f"{self.central_data_folder}/Excel_files/Singles")
        excel_folder = gdrive.create_folder("Singles", [excel_folder['id']])
        print(f"Uploading single excel files to {excel_folder['id']}")
        for f in glob.glob("*.xlsx"):
            gdrive.upload_file(f, [excel_folder['id']])

        self.do_cd(f"{self.central_data_folder}/Average_waveform_files")
        print(f"Uploading average waveforms {average_folder['id']}")
        for f in glob.glob("*"):
            gdrive.upload_file(f, [average_folder['id']])

    def do_cp_central_data(self, runNum=""):
        if runNum == "": runNum = self.runNum
        self.do_set_run(runNum)
        self.do_cd(self.current_run)
        if not os.path.isdir(f"{self.central_data_folder}/Folders/{self.runNum_dir}"):
            os.system(f"cp -r Results {self.central_data_folder}")
            os.system(f"mv {self.central_data_folder}Results {self.central_data_folder}/{self.runNum_dir}")
            self.do_cd(self.central_data_folder)
            os.system(f"cp {self.runNum_dir}/_results.xlsx Excel_files/Singles/run{self.runNum}.xlsx")
            os.system(f"cp {self.runNum_dir}/_results.root Root_files/Singles/run{self.runNum}.root")

            try:
                os.system(f"cp {self.runNum_dir}/ave_wfm_run*.root Average_waveform_files/")
                os.system(f"cp {self.runNum_dir}/average_waveform/average_waveform.png Average_waveform_files/ave_wfm_run{self.runNum}.png")
            except:
                print("No average waveform, skipping")

            os.system(f"zip -r {self.runNum_dir}.zip {self.runNum_dir}")
            os.system(f"mv {self.runNum_dir}.zip Compressed_folders/")
            os.system(f"mv {self.runNum_dir} Folders/")

            self.do_cd(f"{self.central_data_folder}/Root_files")
            timestamp = str(time.ctime()).replace("  ", "_").replace(" ", "_").replace(":", "p")
            timestamp = timestamp.split("_")[0] + "_" + timestamp.split("_")[1] + "_" + \
			 timestamp.split("_")[2] + "_" + timestamp.split("_")[4]
            os.system(f"mv Beta_run_latest.root Beta_run_{timestamp}.root")
            os.system(f"hadd Beta_run_latest.root Beta_run_until_763.root Singles/*.root")

            os.system(f"mv Beta_run_latest_noOld.root Beta_run_{timestamp}_noOld.root")
            os.system(f"hadd Beta_run_latest_noOld.root Singles/*.root")

            self.do_cd(f"{self.central_data_folder}/Excel_files")
            all_data = pandas.DataFrame()
            df = pandas.read_excel("Header_Logbook.xlsx", "DUT", None)
            all_data = all_data.append(df,ignore_index=True)
            all_data = all_data.append(pandas.Series(), ignore_index=True)
            for f in sorted(glob.glob("Singles/*.xlsx")):
                print(f)
                df = pandas.read_excel(f, "DUT", None)
                all_data = all_data.append(df,ignore_index=True)
                all_data = all_data.append(pandas.Series(), ignore_index=True)
                all_data = all_data.append(pandas.Series(), ignore_index=True)
            os.system(f"mv Beta_run_latest.xlsx Beta_run_{timestamp}.xlsx")
            all_data.to_excel(f"Beta_run_latest.xlsx", "DUT", '', None, None, False, False)

            self.do_cd(self.current_run)
        else:
            colorString.sysError(
                "Run already in the central data folder, please delete it first"
            )

    def do_plot_run(self, runNum=""):
        "create simple plots for runNum"
        if runNum == "": runNum = self.runNum
        self.do_set_run(runNum)
        try:
            self.do_cd(f"{self.current_run}/Results")
        except:
            print ("No result folder, analyze data first!")
        try:
            os.mkdir("plots")
        except:
            print ("Plot folder in place, overwriting")

        from root_plotter import simple_plots
        simple_plots([runNum], "_results.root" , "plots/", 0.8, "")


    def do_average_waveform(self, runNum=""):
        "create simple plots for runNum"
        if runNum == "": runNum = self.runNum
        self.do_set_run(runNum)
        self.do_cd(f"{self.current_run}")

        try:
            p = subprocess.call(
                f"python3 ~/HGTD_BetaScope_FW_Test/scripts/betaScope_pyScript/ave_wfm.py",
                shell=True,
            )

            p = subprocess.call(
                f"python3 ~/HGTD_BetaScope_FW_Test/scripts/betaScope_pyScript/plotAverageWave.py",
                shell=True,
            )

            os.system(f"hadd ave_wfm_run{self.runNum}.root ave_wfm*.root")

            self.do_cd(f"{self.current_run}/Results")
            try:
                os.mkdir("average_waveform")
            except:
                print ("Average waveform folder in place, overwriting")

            os.system(f"mv {self.current_run}/ave_wfm_run{self.runNum}.root {self.current_run}/Results")
            os.system(f"mv {self.current_run}/ave_wfm* {self.current_run}/Results/average_waveform")
            os.system(f"mv {self.current_run}/average_waveform* {self.current_run}/Results/average_waveform")

        except:
            print ("Something went wrong, did you analyze the data first?")

    def do_set_run(self, runNum):
        "Setup a run for analysis. It will automatically search the run number in the pre-defined raw data directory. If it can find the run number , it will create a folder for the run in your output directory"
        if hasattr(self, "output_dir"):
            if self.output_dir:
                pass
            else:
                colorString.sysError(
                    "output directory has not been set. Please run set_output_dir"
                )
        else:
            colorString.sysError(
                "output directory has not been set. Please run set_output_dir"
            )

        for rawDir in self.raw_dir:
            for d in os.listdir(rawDir):
                if f"Sr_Run{runNum}" in d:
                    self.runNum = runNum
                    self.runNum_dir = d
                    self.my_raw_dir = rawDir

        if hasattr(self, "runNum"):

            self.current_run = f"{self.output_dir}/{self.runNum_dir}"
            if not os.path.isdir(f"{self.current_run}"):
                os.mkdir(f"{self.current_run}")
            else:
                colorString.sysError(f"directory {self.current_run} is already there")

            try:
                copyfile(
                    f"{self.my_raw_dir}{self.runNum_dir}/fromDAQ/Sr_Run_{self.runNum}_Description.ini",
                    f"{self.current_run}/Sr_Run_{self.runNum}_Description.ini",
                )
            except:
                pass

            self.do_cd_current_run()
        else:
            colorString.sysError(f"No run number {runNum}")

    def do_cd_current_run(self, tdir=""):
        "change directory to the run you set from set_run"
        self.do_cd(self.current_run)

    def do_generate_config(self, place_holder=""):
        "Generate configuration file for the beta-scope waveform analyzer."
        subprocess.call(
            "$BETASCOPE_SCRIPTS/../BetaScope_Ana/BetaScopeWaveformAna/bin/GenerateWaveformConfig",
            shell=True,
        )
        self.do_set_default_config()

    def do_set_default_config(self, place_holder=""):
        "Set the configuration file to default for beta-scope waveform analyzer"
        import configparser

        global predefined_path

        __raw_data_dir = self.my_raw_dir
        __data_output_dir = self.current_run
        __runNum = self.runNum

        parser = configparser.ConfigParser()
        parser.optionxform = str
        parser.read("WaveformAnaConfig.ini")
        parser.set(
            "General", "rawFilesDir", f"{__raw_data_dir}/{self.runNum_dir}/fromDAQ/"
        )
        parser.set("Channel_Activation", "channel_2", "1")
        parser.set("Channel_Activation", "channel_3", "1")
        parser.set("Channel_Invertion", "channel_2", "1")
        with open("WaveformAnaConfig.ini", "w") as config:
            parser.write(config)

    def do_set_active_channel(self, ch):
        import configparser

        parser = configparser.ConfigParser()
        parser.set("Channel_Activation", "channel_{ch}".format(ch=ch), 1)
        with open("WaveformAnaConfig.ini", "w") as config:
            parser.write(config)

    def do_show_ana_progress(self, opt=""):
        if "persist" in opt:
            os.system("watch --color tail /tmp/betaPlot_nohup.log")
        else:
            os.system("tail /tmp/betaPlot_nohup.log")

    def do_auto_cut(self, run=""):
        if not hasattr(self, "current_run"):
            if not run:
                print("please specify a run number")
                return -1
            else:
                self.do_set_run(run)
                self.do_cd_current_run()
        else:
            self.do_cd_current_run()
        p = subprocess.call(
            f"python3 $BETASCOPE_SCRIPTS/betaScope_pyScript/autoCut_v2.py --runNum {self.runNum}",
            shell=True,
        )

    def do_run_analysis(self, i_mode):
        "Run routine beta-scope analysis. Argument with 'full' will do the full rountine analysis, else it will only generate stats files. Argument 'resonly' will only run the result calculation. Argument with 'nohup' will supress the output "

        mode = i_mode.split(" ")

        #self.do_download_latest_UDI()

        if "dry" in mode[0]:
            colorString.sysMsg(f"dry run. mode:")
            for m in mode:
                colorString.sysMsg(m)
            return 0

        if not hasattr(self, "current_run"):
            colorString.sysError("current run is not set")
            return -1
        else:
            self.do_cd_current_run()
            nohup = ""
            nohup_log = ""
            if "nohup" in mode:
                nohup = "nohup"
                nohup_log = " >> /tmp/betaPlot_nohup.log"

                def isRunning(pid):
                    while True:
                        if os.path.isdir(f"/proc/{pid}"):
                            continue
                        else:
                            break

            if "ana_only" in mode[0]:
                p = subprocess.Popen(
                    f"{nohup} $BETASCOPE_ANALYSIS_DIR/BetaScopeWaveformAna/bin/Run_WaveformAna -config {self.current_run}/WaveformAnaConfig.ini {' '.join(mode[1:])} {nohup_log}",
                    shell=True,
                )
                p.wait()

            elif "res_only" in mode[0]:
                p = subprocess.Popen(
                    f"{nohup} $BETASCOPE_SCRIPTS/betaScopePlot/bin/genPlotConfig {nohup_log}",
                    shell=True,
                )
                p.wait()

                p = subprocess.call(
                    f"{nohup} python3 $BETASCOPE_SCRIPTS/betaScope_pyScript/autoCut_v2.py --runNum {self.runNum} {nohup_log}",
                    shell=True,
                )

                p = subprocess.Popen(
                    f"{nohup} $BETASCOPE_SCRIPTS/betaScopePlot/bin/getResults run_info_v{os.environ['RUN_INFO_VER']}.ini {nohup_log}",
                    shell=True,
                )
                p.wait()

                self.do_plot_run(self.runNum)
                self.do_average_waveform(self.runNum)
                self.do_cp_central_data(self.runNum)

            elif "get_res" in mode[0]:
                p = subprocess.Popen(
                    f"{nohup} $BETASCOPE_SCRIPTS/betaScopePlot/bin/getResults run_info_v{os.environ['RUN_INFO_VER']}.ini {nohup_log}",
                    shell=True,
                )
                p.wait()

            elif "full" in mode[0]:
                p = subprocess.Popen(
                    f"{nohup} $BETASCOPE_ANALYSIS_DIR/BetaScopeWaveformAna/bin/Run_WaveformAna -config {self.current_run}/WaveformAnaConfig.ini {' '.join(mode[1:])} {nohup_log}",
                    shell=True,
                )
                p.wait()

                p = subprocess.Popen(
                    f"{nohup} $BETASCOPE_SCRIPTS/betaScopePlot/bin/genPlotConfig {nohup_log}",
                    shell=True,
                )
                p.wait()

                p = subprocess.call(
                    f"{nohup} python3 $BETASCOPE_SCRIPTS/betaScope_pyScript/autoCut_v2.py --runNum {self.runNum} {nohup_log}",
                    shell=True,
                )
                p = subprocess.Popen(
                    f"{nohup} $BETASCOPE_SCRIPTS/betaScopePlot//bin/getResults run_info_v{os.environ['RUN_INFO_VER']}.ini {nohup_log}",
                    shell=True,
                )
                p.wait()

                self.do_plot_run(self.runNum)
                self.do_average_waveform(self.runNum)
                self.do_cp_central_data(self.runNum)

            elif "plot" in mode[0]:
                self.do_plot_run(self.runNum)
                self.do_average_waveform(self.runNum)

            else:
                colorString.sysError("please specify analysis mode.")

    def do_batch(self, txtFile):
        name_pattern = "S8664"
        with open(txtFile, "r") as f:
            for line in f.readlines():
                if name_pattern in line:
                    run_num = line.split("Sr_Run")[1]
                    run_num = int(run_num.split("_")[0])
                    self.do_set_run(run_num)
                    self.do_generate_config()
                    self.do_set_default_config()
                    self.do_run_analysis("full")


if __name__ == "__main__":
    interface = Lgad()
    interface.cmdloop()
