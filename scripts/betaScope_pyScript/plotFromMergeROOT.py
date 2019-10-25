'''
ploting from the merged ROOT file in /user_data/
'''
import ROOT
import os
from array import array

try:
    from atlasplots import atlas_style
    atlas_style.SetAtlasStyle()
except:
    pass

import re


odir = "testPlot/"
data_prep = [
{"reg":"HPK|2[xX]2|3[eE]15", "color":ROOT.kRed, "style":20, "matched":False }
]

run_range = [500,600]

def searchRun( regExpress="HPK|2[xX]2|3[eE]15", run_range):
    user_data_dir = os.environ["BETASCOPE_SCRIPTS"]
    user_data_dir += "/user_data/"
    tfile = ROOT.TFile.Open("{}/merged.root".format(user_data_dir) )
    matched_run = []
    for run in range(run_range[0], run_range[1]):
        try:
            ttree = tfile.Get("run"+run)
            ttree.GetEntry(0)
            if bool(re.search(regExpress, str(ttree.SensorName))):
                matched_run.append(run)
        except:
            continue

    print("Your matched run for {}".format(regExpress))
    for r in matched_run:
        print(r)

searchRun()
'''
def plotFromMergeROOT(data_prep, odir, run_range):
    user_data_dir = os.environ["BETASCOPE_SCRIPTS"]
    user_data_dir += "/user_data/"

    os.makedirs(user_data_dir+odir)

    tfile = ROOT.TFile.Open("{}/merged.root".format(user_data_dir) )

    params = [
        ["NewPulseArea/Resistance:Bias", "Collected Charge [fC]"],
        ["Pmax:Bias", "Pulse Maximum [mV]"]
    ]

    for param in params:
        param["x"] = array("d")
        param["y"] = array("d")

        for run in range(run_range[0], run_range[1]):
            try:
                ttree = tfile.Get("run"+run)
                ttree.GetEntry(0)
                for data in data_prep:
                    data["data"] = []
                    if bool(re.search(data["reg"], str(ttree.SensorName))):

            except:
                continue
'''
