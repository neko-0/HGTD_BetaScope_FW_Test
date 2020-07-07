from openpyxl import Workbook
import configparser
import os
import ROOT
from atlasplots import atlas_style
import parseINItoROOT

# ROOT.gROOT.LoadMacro("~/atlasstyle-00-04-02/AtlasStyle.C")
# ROOT.SetAtlasStyle()

atlas_style.SetAtlasStyle()

par_list = [
    "runNumber",
    "SensorName",
    "Temp",
    "Bias",
    "Resistance",
    "pulseArea",
    "pulseArea_Error",
    "Pmax",
    "Pmax_Error",
    "RMS",
    "RMS_Error",
    "Rise_Time",
    "Rise_Time_Error",
    "dvdt",
    "dvdt_Error",
    "FWHM",
    "FWHM_Error",
    "NewPulseArea",
    "NewPulseArea_Error",
    "FallTime",
    "FallTime_Error",
    "cycle",
]

par_dict = {
    "runNumber": "A",
    "SensorName": "B",
    "Temp": "G",
    "Bias": "H",
    "Resistance": "L",
    "pulseArea": "J",
    "pulseArea_Error": "K",
    "Pmax": "R",
    "Pmax_Error": "S",
    "RMS": "T",
    "RMS_Error": "U",
    "Rise_Time": "Z",
    "Rise_Time_Error": "AA",
    "dvdt": "AB",
    "dvdt_Error": "AC",
    "FWHM": "AL",
    "FWHM_Error": "AM",
    "NewPulseArea": "CA",
    "NewPulseArea_Error": "CB",
    "FallTime": "DG",
    "FallTime_Error": "DH",
    "cycle": "F",
    "CFD50Time": "BW",
    "CFD50Time_Err": "BX",
    "CFD20Time": "DD",
    "CFD20Time_Err": "DE",
    "Leakage": "C",
}

folders = [
    "/media/mnt/BigHD/BetaScope_Data/Analyzed_YZ/",
    "/media/mnt/BigHD/BetaScope_Data/Analyzed_Simone/",
    "/media/mnt/gunter/betaAna2/",
]

dirlist = []
for folder in folders:
    for run in os.listdir(folder):
        dirlist.append(folder + run)

pars = [
    ["NewPulseArea:V", "Collected Charge [fC]"],
    ["Pmax:V", "Pulse Maximum [mV]"],
    ["RMS:V", "RMS [mV]"],
]

filout = ROOT.TFile.Open("beta_scope_output.root", "recreate", "beta_scope_data")
runs_done = []
for i, folder in enumerate(dirlist):
    fold = folder.split("/")[len(folder.split("/")) - 1]
    if "Run" not in fold:
        continue
    if ".txt" in fold:
        continue
    if "RunTest" in fold:
        continue
    if "Splitter" in fold:
        continue

    if fold in runs_done:
        continue
    runs_done.append(fold)

    if len(fold.split("_Trig_")) > 1:
        runtag = fold.split("_Trig_")[0]
    else:
        runtag = fold.split("_trig_")[0]

    config = configparser.ConfigParser()

    config_folder = folder + "/Results/_results.ini"
    config.read(config_folder)
    config_section = config.sections()
    if len(config_section) == 0:
        config_folder = folder + "/fromDAQ/Results/_results.ini"
        config.read(config_folder)
        config_section = config.sections()

    parseINItoROOT.parseINItoROOT2(filout, runtag, folder + "/Results/", config_folder)
    # if i>5: break
    # print runtag

    if False:
        dut_trig = ["DUT", "Trig"]
        for ch in dut_trig:
            for bias_run in config_section:
                if ch in bias_run:
                    if ch != "Trig":
                        biasV = bias_run[bias_run.find("_") + 1 : bias_run.find("V")]
                        value = config[bias_run][par]
                        bias_voltages.append(float(biasV))
                        if par == "NewPulseArea":
                            values.append(float(value) / 4700.0)
                        else:
                            values.append(float(value))

filout.Print()
filout.Close()
