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

dirlist = os.listdir(".")

#cantag = "FBK_1p5E15_3E15"
#runlist = ["426", "535", "434", "564", "567", "601", "490", "536", "541", "591", "426", "570", "571"]
#cantag = "FBK_1p5E15"
#runlist = ["536", "541", "591", "426", "571"]
#cantag = "FBK_3E15"
#runlist = ["434", "535", "564", "567", "601", "490", "570"]
#cantag = "FBK_spread_541"
#runlist = ["541"]
cantag = "HPK_3p2_3E15"
runlist = ["614", "672", "673", "674", "675", "676", "677"]


pars = [["NewPulseArea:V", "Collected Charge [fC]"],
        ["Pmax:V", "Pulse Maximum [mV]"]
        ]

for parv in pars:
    maxval = 0
    if parv[0].split(":")[1] == "V":
        par = parv[0].split(":")[0]
        partag = parv[1]
        gs = []
        leg = ROOT.TLegend(0.17, 0.5, 0.7, 0.93)
        leg.SetBorderSize(0)
        leg.SetFillColorAlpha(0, 0.5)
        for fold in dirlist:
            markstyle = 1
            if "3E15" in fold or "3e15" in fold: markstyle = 20
            if "2x2" in fold: markstyle = 22
            if "1p5E15" in fold or "1p5e15" in fold: markstyle = 22
            if "C_p" in fold: markstyle = 33
            bias_voltages = []
            values = []
            for col,run in enumerate(runlist):
                if run in fold:
                    runtag = fold.split("_Trig")[0]
                    config = configparser.ConfigParser()
                    config.read(fold + "/Results/_results.ini")
                    config_section = config.sections()
                    #print(config_section)

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
                    g.SetMarkerStyle(markstyle)
                    g.SetMarkerSize(1.5)

                    if col < 9: coltag = col + 1
                    else: coltag  = col + 10
                    g.SetMarkerColor(coltag)

                    #g.GetXaxis().SetTitle("Bias V [V]")
                    #g.GetYaxis().SetTitle(partag)
                    #g.GetXaxis().SetRangeUser(0., 800.)
                    #g.GetYaxis().SetRangeUser(0., max(values)*2.)

                    if maxval < max(values): maxval = max(values)
                    leg.AddEntry(g, runtag, "P")
                    gs.append(g)

        can = ROOT.TCanvas("c", "c", 1280, 1024)

        h = ROOT.TH1F("h", "h", 1, 400, 800)
        h.GetXaxis().SetRangeUser(400., 800.)
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

if False:
    wb = Workbook()
    ws = wb.active
    rowCounter = 1
    dut_trig = ["DUT", "Trig"]

    RunNum = 100
    SensorName = "Hi"
    Temp = 20
    Resistance = 4700
    trigBias = 395

    for ch in dut_trig:
        for bias in config_section:
            if ch in bias:
                if ch != "Trig":
                    if ".." in bias:
                        Bias = bias[bias.find("_")+1:bias.find("V")]
                        cycle = bias.split("..")[1]
                        if "_" in cycle:
                            cycle = cycle.split("_")[0]
                    else:
                        Bias = bias[bias.find("_")+1:bias.find("V")]
                        cycle = 1
                else:
                    #Bias = trigBias
                    try:
                        Bias = config[bias]["trigger_bias"]
                        if ".." in bias:
                            cycle = bias.split("..")[1]
                            if "_" in cycle:
                                cycle = cycle.split("_")[0]
                        else:
                            cycle = 1
                    except:
                        Bias = -390
                        cycle = 1
                for par in par_list:
                    if (par == "SensorName"):
                        cell = par_dict[par] + str(rowCounter)
                        ws[cell] = SensorName
                    elif (par == "Temp"):
                        try:
                            Temp = config[bias]["temperature"]
                        except:
                            Temp = "-30"
                        cell = par_dict[par] + str(rowCounter)
                        ws[cell] = float(Temp)
                    elif (par == "Bias"):
                        cell = par_dict[par] + str(rowCounter)
                        ws[cell] = float(Bias)
                        if cycle:
                            cell = par_dict["cycle"] + str(rowCounter)
                            ws[cell] = int(cycle)
                    elif (par == "Resistance"):
                        cell = par_dict[par] + str(rowCounter)
                        ws[cell] = float(Resistance)
                    else:
                        cell = par_dict[par] + str(rowCounter)
                        ws[cell] = float(config[bias][par])
                rowCounter+=1
        rowCounter+=1

    wb.save("_results.xlsx")
