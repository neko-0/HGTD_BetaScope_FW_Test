import ROOT
import configparser
import argparse

from atlasplots import atlas_style as astyle
astyle.SetAtlasStyle()

def plotHisto( config, volt_min = 0, volt_max=700, set_temperature=-20, param="pmax", binSize=10, low_b=0, high_b=400):
    config_file = configparser.ConfigParser()
    config_file.read( config )
    num_file = config_file["header"]["number_of_runs"]
    file_prefix = ""
    if config_file["header"]["use_selected_events"]:
        file_prefix = "Selected_"
    else:
        file_prefix = ""

    histo_list = []
    sensor_name = config_file["header"]["sensor"]
    color = 2
    binNum = int( (high_b-low_b)/binSize)
    for runIndex in range(int(num_file)):
        fileName = file_prefix + config_file["run%s"%runIndex]["file_name"]
        bias = config_file["run%s"%runIndex]["bias"]
        bias_value = int(bias.split("V")[0])
        try:
            temperature = config_file["run%s"%runIndex]["file_name"].split("temp")[1].split(".root")[0]
        except:
            temperature = -273
        if volt_min < bias_value and bias_value < volt_max and temperature == set_temperature :
            tfile = ROOT.TFile.Open(fileName)
            ttree = tfile.Get("wfm")
            histo1D = ""
            if binSize < 0:
                histo1D = ROOT.TH1D("bias", "Sensor:%s Param:%s"%(sensor_name, param), 200, 1, 1)
            else:
                histo1D = ROOT.TH1D("bias", "Sensor:%s Param:%s"%(sensor_name, param), binNum, low_b, high_b)
            ttree.Project(histo1D.GetName(), "%s"%param)
            histo1D.SetDirectory(0)
            histo1D.GetYaxis().SetRangeUser(0, 2000)
            histo1D.SetLineColor(color)
            histo1D.SetLineWidth(2)
            histo1D.Scale(1/histo1D.Integral("width"))
            color+=1
            histo_list.append( [sensor_name, bias, histo1D] )
        else:
            continue

    return histo_list

if __name__ == "__main__":

    commandline_ArgsParser = argparse.ArgumentParser()
    commandline_ArgsParser.add_argument("--Vmin", dest="Vmin", nargs="?", default="0", type=int, help="min bias")
    commandline_ArgsParser.add_argument("--Vmax", dest="Vmax", nargs="?", default="1000", type=int, help="max bias")
    commandline_ArgsParser.add_argument("--binSize", dest="bin_size", nargs="?", default="10", type=float, help="bin size")
    commandline_ArgsParser.add_argument("--low", dest="low_b", nargs="?", default="0", type=int, help="low bound")
    commandline_ArgsParser.add_argument("--high", dest="high_b", nargs="?", default="500", type=int, help="high bound")
    commandline_ArgsParser.add_argument("--param", dest="param", nargs="?", default="pmax2[0]", type=str, help="parameter")
    commandline_ArgsParser.add_argument("--legTitle", dest="legTitle", nargs="?", default="", type=str, help="legend title")
    commandline_ArgsParser.add_argument("--xtitle", dest="xtitle", nargs="?", default="", type=str, help="x axis tilte")
    commandline_ArgsParser.add_argument("--temp", dest="temp", nargs="?", default="-30", type=str, help="temperature")
    commandline_ArgsParser.add_argument("--configFile", dest="configFile", nargs="?", default="run_info_v08022018.ini", type=str, help="Configuration file")

    argv = commandline_ArgsParser.parse_args()

    histoList = plotHisto(argv.configFile, argv.Vmin, argv.Vmax, argv.temp, argv.param, argv.bin_size, argv.low_b, argv.high_b)

    FWHM_of_distri = []
    #ROOT.gROOT.SetBatch(True)
    c = ROOT.TCanvas()
    c.cd()
    leg = ROOT.TLegend()
    #leg.SetHeader(stuff[0][0])
    leg.SetTextSize(0.03)
    first = True
    color = 2
    for stuff in histoList:
        if first:
            stuff[2].GetXaxis().SetTitle(argv.xtitle)
            stuff[2].Draw("histo ")
            first = False
        else:
            stuff[2].GetXaxis().SetTitle(argv.xtitle)
            stuff[2].Draw("histo same")
        fit = ROOT.TF1("landau", "landau", stuff[2].GetMean()-stuff[2].GetStdDev(), stuff[2].GetMean()+5*stuff[2].GetStdDev() )
        stuff[2].Fit(fit, "R")
        fit.SetLineColor(ROOT.kBlack)
        fit.Draw("same")
        color+=1
        leg.AddEntry(stuff[2], stuff[1], "l")

        maxValue = stuff[2].GetBinContent( stuff[2].GetMaximumBin() )
        print(maxValue)

        print(stuff[2].FindFirstBinAbove(maxValue/2))
        low_50percent_bin = stuff[2].GetXaxis().GetBinCenter( stuff[2].FindFirstBinAbove(maxValue/2))
        high_50percent_bin = stuff[2].GetXaxis().GetBinCenter( stuff[2].FindLastBinAbove(maxValue/2))
        print(low_50percent_bin)
        print(high_50percent_bin)
        FWHM_of_distri.append([stuff[1], high_50percent_bin-low_50percent_bin])
    if argv.legTitle != "":
        leg.SetHeader(argv.legTitle)
    leg.Draw()
    astyle.ATLASLabel(0.25, 0.87, "Internal", 1)

    for item in FWHM_of_distri:
        print("%s,%s,%s"%(argv.temp, item[0], item[1]))

    raw_input("Finished.")
    c.SaveAs("ave.png")
    c.SaveAs("ave.pdf")
