import os
import ROOT
import configparser
import argparse

from result_parser.daq_info import DAQInfo

ROOT.gROOT.ProcessLine(".x /home/datataking/atlasstyle-00-04-02/AtlasStyle.C")
ROOT.gROOT.ProcessLine(".x /home/datataking/atlasstyle-00-04-02/AtlasLabels.C")
ROOT.SetAtlasStyle()
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gStyle.SetPalette(55)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetEndErrorSize(2)


def plot_average_waveform(config, volt_min=0, volt_max=1000, norm=0):
    print ("In function")
    print ("Plotting")

    config_file = configparser.ConfigParser()
    config_file.read(config)
    num_file = config_file["header"]["number_of_runs"]
    file_prefix = ""
    if config_file["header"]["use_selected_events"] == "true":
        file_prefix = "averaged_Selected_"
    else:
        file_prefix = "ave_wfm_"
    avelist = []

    try:
        description_file = DAQInfo.open(f"Sr_Run_{config_file['header']['run_number']}_Description.ini")
        sensor_name = f"{description_file.dut_name},{description_file.temperature}"
    except:
        sensor_name = f"Run {config_file['header']['run_number']}"

    color = 2
    already_done = []
    for runIndex in range(int(num_file)):
        fileName = file_prefix + config_file[f"run{runIndex}"]["file_name"]
        bias = config_file[f"run{runIndex}"]["bias"]
        bias_value = int(bias.split("V")[0])
        print(fileName, bias_value)
        if True: #volt_min < bias_value and bias_value < volt_max:
            tfile = ROOT.TFile.Open(fileName)
            pf_name = f"V{bias_value}_pfx"
            if pf_name in already_done: continue
            already_done.append(pf_name)
            ave = tfile.Get(pf_name)

            ave.SetDirectory(0)
            ave.GetYaxis().SetRangeUser(-2000, 2000)
            ave.SetLineColor(color)
            maxValue = None
            maxValueIndex = 0
            left50 = 0
            right50 = 0
            if True:
                maxValue = 0
                for i in range(10000):
                    if abs(maxValue) <= abs(ave.GetBinContent(i)):
                        maxValue = abs(ave.GetBinContent(i))
                        maxValueIndex = i
                for k in range(0, maxValueIndex):
                    if maxValue / 2.0 == abs(ave.GetBinContent(k)):
                        left50 = ave.GetBinCenter(k)
                        break
                    elif maxValue / 2.0 < abs(ave.GetBinContent(k)):
                        slope = float(
                            (abs(ave.GetBinContent(k)) - abs(ave.GetBinContent(k - 1)))
                        ) / (ave.GetBinCenter(k) - ave.GetBinCenter(k - 1))
                        intercept = abs(
                            ave.GetBinContent(k)
                        ) - slope * ave.GetBinCenter(k)
                        left50 = (maxValue / 2.0 - intercept) / float(slope)
                        break
                    else:
                        continue
                for k in range(maxValueIndex, 10000):
                    if maxValue / 2.0 == abs(ave.GetBinContent(k)):
                        right50 = ave.GetBinCenter(k)
                        break
                    elif maxValue / 2.0 > abs(ave.GetBinContent(k)):
                        slope = (
                            abs(float(ave.GetBinContent(k - 1)))
                            - abs(ave.GetBinContent(k))
                        ) / (ave.GetBinCenter(k - 1) - ave.GetBinCenter(k))
                        intercept = abs(
                            ave.GetBinContent(k)
                        ) - slope * ave.GetBinCenter(k)
                        right50 = (maxValue / 2.0 - intercept) / float(slope)
                        break
                    else:
                        continue
                if norm != 0:
                    ave.Scale(1.0 / maxValue)
            lline = ROOT.TLine(left50, -100, left50, 100)
            rline = ROOT.TLine(right50, -100, right50, 100)
            lline.SetLineColor(color)
            rline.SetLineColor(color)
            color += 1
            avelist.append(
                {
                    "sensor": sensor_name,
                    "bias": bias,
                    "wf": ave,
                    "max": maxValue,
                    "left50": left50,
                    "right50": right50,
                    "color": color - 1,
                    "lline": lline,
                    "rline": rline,
                }
            )
        else:
            continue
    #print (avelist)
    return avelist


if __name__ == "__main__":
    commandline_ArgsParser = argparse.ArgumentParser()
    commandline_ArgsParser.add_argument(
        "--Vmin", dest="Vmin", nargs="?", default="0", type=int, help="min bias"
    )
    commandline_ArgsParser.add_argument(
        "--Vmax", dest="Vmax", nargs="?", default="1000", type=int, help="max bias"
    )
    commandline_ArgsParser.add_argument(
        "--configFile",
        dest="configFile",
        nargs="?",
        default="run_info_v2020_7_14.ini",
        type=str,
        help="Configuration file",
    )
    commandline_ArgsParser.add_argument(
        "--norm",
        dest="norm",
        nargs="?",
        default="0",
        type=int,
        help="Normalized waveform",
    )

    argv = commandline_ArgsParser.parse_args()

    #if not os.path.exists("ave_wfm_plots/"):
    #    os.makedirs("ave_wfm_plots/")

    aveWave = plot_average_waveform(argv.configFile, argv.Vmin, argv.Vmax, argv.norm)

    # ROOT.gROOT.SetBatch(True)
    c = ROOT.TCanvas()
    c.cd()
    leg = ROOT.TLegend(0.21, 0.8, 0.89, 0.94)
    leg.SetBorderSize(0)
    leg.SetNColumns(5)
    leg.SetHeader(aveWave[0]["sensor"])
    leg.SetTextSize(0.03)
    for ientry, wave in enumerate(aveWave):
        if ientry == 0:
            wave["wf"].Draw("HIST L")
            #wave["lline"].Draw()
            #wave["rline"].Draw()
            wave["wf"].GetYaxis().SetRangeUser(-50,300)
            wave["wf"].GetYaxis().SetTitle("Amplitude [mV]")
            wave["wf"].GetXaxis().SetRangeUser(-1000,5000)
            wave["wf"].GetXaxis().SetTitle("Time [ps]")
        else:
            wave["wf"].Draw("HIST L same")
            #wave["lline"].Draw()
            #wave["rline"].Draw()
        leg.AddEntry(wave["wf"], wave["bias"], "l")
    leg.Draw("")

    #astyle.ATLASLabel(0.25, 0.87, "Internal", 1)

    #raw_input("Finished.")
    c.SaveAs("average_waveform.png")
    c.SaveAs("average_waveform.pdf")
