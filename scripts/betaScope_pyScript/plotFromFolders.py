from openpyxl import Workbook
import configparser
import os
import ROOT
from atlasplots import atlas_style

#ROOT.gROOT.LoadMacro("~/atlasstyle-00-04-02/AtlasStyle.C")
#ROOT.SetAtlasStyle()

atlas_style.SetAtlasStyle()

par_list = [
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
"FallTime_Error"
]

par_dict = {
"SensorName" : "B",
"Temp" : "G",
"Bias" : "H",
"Resistance" : "L",
"pulseArea" : "J",
"pulseArea_Error" : "K",
"Pmax" : "R",
"Pmax_Error" : "S",
"RMS" : "T",
"RMS_Error" : "U",
"Rise_Time" : "Z",
"Rise_Time_Error" : "AA",
"dvdt" : "AB",
"dvdt_Error" : "AC",
"FWHM" : "AL",
"FWHM_Error" : "AM",
"NewPulseArea" : "CA",
"NewPulseArea_Error" : "CB",
"FallTime" : "DG",
"FallTime_Error" : "DH",
"cycle" : "F"
}

folders = ["/media/mnt/BigHD/BetaScope_Data/Analyzed_YZ/",
            "/media/mnt/BigHD/BetaScope_Data/Analyzed_Simone/",
            "/media/mnt/gunter/betaAna2/"
            ]

dirlist = []
for folder in folders:
    for run in os.listdir(folder):
        dirlist.append(folder + run)

#print dirlist
#raw_input()

#cantag = "FBK_1p5E15_3E15"
#runlist = ["426", "535", "434", "564", "567", "601", "490", "536", "541", "591", "426", "570", "571"]

#cantag = "FBK_1E14"
#runlist = ["427"]
#cantag = "FBK_4E14"
#runlist = ["435"]
#cantag = "FBK_8E14"
#runlist = ["428"]
#cantag = "FBK_1p5E15"
#runlist = ["536", "541", "591", "426", "571", "669"]
#cantag = "FBK_3E15"
#runlist = ["434", "535", "564", "601", "490", "570"]
#cantag = "FBK_4E15"
#runlist = ["534", "589"]
#cantag = "FBK_5E15"
#runlist = ["533"]
#cantag = "FBK_6E15"
#runlist = ["532"]

#cantag = "HPK_3p2_3E15"
#runlist = ["614", "672", "673", "674", "675", "676", "677"]
#cantag = "HPK_3p2_3E15"
#runlist = ["539", "477"]
#cantag = "HPK_3p2_2p25E15"
#runlist = ["615", "664", "665"]
cantag = "HPK_3p2_1p5E15_singles"
runlist = ["613", "509", "586", "684", "685", "550", "475", "542", "622", "686", "620", "689"]
#cantag = "HPK_3p1_3p2"
#runlist = ["653", "649", "688"]

#cantag = "Test"
#runlist = ["689"]


pars = [["NewPulseArea:V", "Collected Charge [fC]"],
        ["Pmax:V", "Pulse Maximum [mV]"],
        ["RMS:V", "RMS [mV]"]
        ]

for parv in pars:
    maxval = 0
    maxvol = 0
    if parv[0].split(":")[1] == "V":
        par = parv[0].split(":")[0]
        partag = parv[1]
        gs = []
        leg = ROOT.TLegend(0.17, 0.5, 0.7, 0.93)
        leg.SetBorderSize(0)
        leg.SetFillColorAlpha(0, 0.5)

        runs_done = []
        for folder in dirlist:
            fold = folder.split("/")[len(folder.split("/"))-1]
            markstyle = 20
            if "1E14" in fold or "1e14" in fold: markstyle = 21
            if "4E14" in fold or "4e14" in fold: markstyle = 22
            if "8E14" in fold or "8e14" in fold: markstyle = 23
            if "1p5E15" in fold or "1p5e15" in fold: markstyle = 24
            if "2p25E15" in fold or "2p25e15" in fold: markstyle = 25
            if "3E15" in fold or "3e15" in fold: markstyle = 26
            if "4E15" in fold or "4e15" in fold: markstyle = 27
            if "_5E15" in fold or "_5e15" in fold: markstyle = 28
            if "6E15" in fold or "6e15" in fold: markstyle = 29

            if "C_p" in fold: markstyle = 31
            #if "2x2" in fold: markstyle = 30

            bias_voltages = []
            values = []
            for col,run in enumerate(runlist):
                if "Run" + run in fold:
                    #print folder
                    if fold in runs_done: continue
                    runs_done.append(fold)
                    
                    if len(fold.split("_Trig")) > 1:
                        runtag = fold.split("_Trig")[0]
                    else:
                        runtag = fold.split("_trig")[0]
                    config = configparser.ConfigParser()
                    config.read(folder + "/Results/_results.ini")
                    config_section = config.sections()
                    if len(config_section) == 0:
                        config.read(folder + "/fromDAQ/Results/_results.ini")
                        config_section = config.sections()
                    print runtag
                    #print config_section

                    dut_trig = ["DUT", "Trig"]
                    for ch in dut_trig:
                        for bias_run in config_section:
                            if ch in bias_run:
                                if ch != "Trig":
                                    biasV = bias_run[bias_run.find("_")+1:bias_run.find("V")]
                                    value = config[bias_run][par]
                                    bias_voltages.append(float(biasV))
                                    if par == "NewPulseArea": values.append(float(value)/4700.)
                                    else: values.append(float(value))
                    #print bias_voltages
                    #print values
                    xv = ROOT.TVectorF(len(bias_voltages))
                    yv = ROOT.TVectorF(len(values))

                    for i, x in enumerate(bias_voltages):
                        xv[i] = x
                    for i, y in enumerate(values):
                        yv[i] = y

                    g = ROOT.TGraph(xv, yv)
                    g.SetName(runtag)
                    g.SetTitle(runtag)
                    g.SetMarkerStyle(markstyle)
                    g.SetMarkerSize(1.5)

                    if col < 9: coltag = col + 1
                    else: coltag  = col + 10
                    g.SetMarkerColor(coltag)
                    g.SetLineColor(coltag)

                    #g.GetXaxis().SetTitle("Bias V [V]")
                    #g.GetYaxis().SetTitle(partag)
                    #g.GetXaxis().SetRangeUser(0., 800.)
                    #g.GetYaxis().SetRangeUser(0., max(values)*2.)

                    if maxval < max(values): maxval = max(values)
                    if maxvol < max(bias_voltages): maxvol = max(bias_voltages)
                    leg.AddEntry(g, runtag, "P")
                    gs.append(g)

        can = ROOT.TCanvas("c", "c", 1280, 1024)

        h = ROOT.TH1F("h", "h", 1, 0, maxvol*1.2)
        h.GetXaxis().SetRangeUser(0., maxvol*1.2)
        h.GetYaxis().SetRangeUser(0, maxval*2.)
        h.GetXaxis().SetTitle("Bias V [V]")
        h.GetYaxis().SetTitle(partag)
        h.Draw()

        #gs[0].GetXaxis().SetRangeUser(0., 800.)
        #gs[0].Draw("A")
        for g in gs:
            g.Draw("sameP")
        leg.Draw()
        atlas_style.ATLASLabel(0.7, 0.9, "Internal")

        can.SaveAs("Plot_" + cantag + "_" + par + ".pdf")
        raw_input()
