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
{"reg":"(HPK)(.*)(2[xX]2)(.*)(3[eE]15)(.*)(UCSC)(.*)(.?)", "style":20, "matched":False, "matched_runs":[] }
{"reg":"(HPK)(.*)(2[xX]2)(.*)(3[eE]15)(.*)(Tori)(.*)(.?)", "style":21, "matched":False, "matched_runs":[] }
]

run_range = [500,700]

def searchRun( regExpress, run_range):
    user_data_dir = os.environ["BETASCOPE_SCRIPTS"]
    user_data_dir += "/user_data/"
    tfile = ROOT.TFile.Open("{}/merged.root".format(user_data_dir) )
    matched_run = []
    for run in range(run_range[0], run_range[1]):
        run_tag = "run"+str(run)
        try:
            ttree = getattr(tfile, run_tag)
            check_ptag = 2
            while True:
                my_run_tag = run_tag+"p"+str(check_ptag)
                if hasattr(tfile, my_run_tag):
                    run_tag = my_run_tag
                    ttree = getattr(tfile, my_run_tag)
                    check_ptag+=1
                else:
                    break
            ttree.GetEntry(0)
            if bool(re.search(regExpress, str(ttree.SensorName))):
                 matched_run.append(run_tag)
        except Exception as e:
            #print(e)
            continue

    print("Your matched run for {}".format(regExpress))
    for r in matched_run:
        print(r)

#searchRun("(HPK)(.*)(2[xX]2)(.*)(3[eE]15)(.*)(.?)", run_range)



def plotFromMergeROOT(data_prep, odir, run_range):
    user_data_dir = os.environ["BETASCOPE_SCRIPTS"]
    user_data_dir += "/user_data/"

    os.makedirs(user_data_dir+odir)

    tfile = ROOT.TFile.Open("{}/merged.root".format(user_data_dir) )

    params = [
        ["NewPulseArea/Resistance:Bias", "Collected Charge [fC]"],
        ["Pmax:Bias", "Pulse Maximum [mV]"]
    ]

    for data in data_prep:
        my_matched_runs = searchRun( data["reg"], [500,800] )
        for run in my_matched_runs:
            data["matched_runs"].append(run)

    for param in params:
        param["x"] = array("d")
        param["y"] = array("d")

        
