"""
for making scatering plot
"""

import ROOT
import os

ROOT.gROOT.SetBatch(True)


def plotScatter(fname, cuts, x, y, xtitle, ytitle, title):
    tfile = ROOT.TFile.Open(fname)
    ttree = tfile.wfm
    expre = "{}:{}".format(y, x)
    histo2D = ROOT.TH2D("histo2D", title, 150, 0, 1500, 150, 0, 1500)
    histo2D.GetXaxis().SetTitle(xtitle)
    histo2D.GetYaxis().SetTitle(ytitle)
    histo2D.SetMarkerStyle(20)
    # print(cuts)
    # print(expre)
    ttree.Project("histo2D", expre, cuts)
    canvas = ROOT.TCanvas("c", "", 800, 600)
    canvas.cd()
    histo2D.Print()
    histo2D.Draw("col")
    canvas.Update()
    # raw_input()
    canvas.SaveAs("scatter/{}_{}_vs_{}.pdf".format(fname, x, y))
    canvas.SaveAs("scatter/{}_{}_vs_{}.png".format(fname, x, y))
    tfile.Close()


if __name__ == "__main__":

    import argparse

    cml_parser = argparse.ArgumentParser()
    cml_parser.add_argument("-x", dest="x", nargs="?", default="", type=str, help="x")
    cml_parser.add_argument("-y", dest="y", nargs="?", default="", type=str, help="y")
    cml_parser.add_argument(
        "-xtitle", dest="xtitle", nargs="?", default="", type=str, help="xtitle"
    )
    cml_parser.add_argument(
        "-ytitle", dest="ytitle", nargs="?", default="", type=str, help="ytitle"
    )
    argv = cml_parser.parse_args()

    ROOT.gStyle.SetOptFit(1)

    import configparser

    config_file = configparser.ConfigParser()
    config_file.read("run_info_v08022018.ini")

    # get number of files(runs)
    num_files = config_file["header"]["number_of_runs"]
    file_prefix = ""
    if config_file["header"]["use_selected_events"] == "true":
        file_prefix = "Selected_"
    else:
        file_prefix = ""
    sensor_name = config_file["header"]["sensor"]
    dut_ch = config_file["header"]["dut_channel"]
    trig_ch = config_file["header"]["trigger_channel"]
    run_num = ""

    try:
        os.makedirs("scatter")
    except:
        pass
    for runIndex in range(int(num_files)):
        fileName = file_prefix + config_file["run%s" % runIndex]["file_name"]
        raw_cut = config_file["run%s" % runIndex]["cut_%s" % dut_ch].split(" ")
        dut_cut = (
            "tmax%s[0]-cfd3[20] > %s && tmax%s[0]-cfd3[20] < %s && pmax%s[0] > %s && pmax%s[0] < %s"
            % (
                dut_ch,
                raw_cut[0],
                dut_ch,
                raw_cut[1],
                dut_ch,
                raw_cut[2],
                dut_ch,
                raw_cut[3],
            )
        )
        trig_cut = (
            "tmax%s[0]-cfd3[20] > %s && tmax%s[0]-cfd3[20] < %s && pmax%s[0] > %s && pmax%s[0] < %s"
            % (
                trig_ch,
                raw_cut[4],
                trig_ch,
                raw_cut[5],
                trig_ch,
                raw_cut[6],
                trig_ch,
                raw_cut[7],
            )
        )
        cuts = dut_cut + " && " + trig_cut
        plotScatter(
            fileName,
            cuts,
            argv.x,
            argv.y,
            argv.xtitle,
            argv.ytitle,
            config_file["run%s" % runIndex]["bias"],
        )
