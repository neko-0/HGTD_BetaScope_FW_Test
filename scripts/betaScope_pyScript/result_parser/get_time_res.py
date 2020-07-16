"""
function to extract time resolution from the beta scope measurement
"""

import math
import configparser

import ROOT

from config_reader import ConfigReader
from config_reader import RUN_INFO_VER

from trig_cali import TRIG_CALI


def _get_time_diff(
    tfile_name,
    cuts,
    cfd,
    dut_ch,
    trig_ch,
    return_histo=False,
    xmin=None,
    xmax=None,
    nbin=None,
):
    tfile = ROOT.TFile.Open(tfile_name, "r")
    ttree_wfm = tfile.wfm

    # parameter to project on the histogram. Time difference of dut and trig
    res_type_dict = {
        "tmax": f"tmax{dut_ch}-cfd{trig_ch}[20]",
        "fit_tmax": f"fit_tmax{dut_ch}-cfd{trig_ch}[20]",
        "zero_cross_tmax": f"zero_cross_tmax{dut_ch}-cfd{trig_ch}[20]",
    }

    if cfd in res_type_dict.keys():
        tdiff = res_type[cfd]
    else:
        tdiff = f"cfd{dut_ch}[{cfd}]-cfd{trig_ch}[20]"

    # create default histogram for pre-processing.
    preHisto = ROOT.TH1D("preHisto", "preHisto", 100, 1, 1)
    ttree_wfm.Project("preHisto", tdiff, cuts)
    sample_std = preHisto.GetStdDev(1)
    sample_mean = preHisto.GetMean(1)
    num_events = preHisto.GetEntries()
    IQR = 2 * 0.67845 * sample_std  # might not be the correct one

    # cfd time different histogram for timing resolution extraction
    bin_width = 2 * IQR / pow(num_events, 1.0 / 3.0)
    min_range = sample_mean - 5.0 * sample_std
    max_range = sample_mean + 5.0 * sample_std
    nbin = int((max_range - min_range) / bin_width)
    if xmin:
        min_range = xmin
    if xmax:
        max_range = xmax
    if nbin:
        nbin = nbin
    tdiff_histo = ROOT.TH1D("tdiff_h", "tdiff_h", nbin, min_range, max_range)
    ttree_wfm.Project("tdiff_h", tdiff, cuts)
    gaussian = ROOT.TF1("gaussian", "gaus")
    tdiff_histo.Fit(gaussian)
    sigma = gaussian.GetParameter(2)
    sigma_err = gaussian.GetParError(2)

    # returning histogram
    if return_histo:
        tdiff_histo.SetDirectory(0)
        return {"histo": tdiff_histo, "sigma": sigma, "sigma_err": sigma_err}
    else:
        return {"sigma": sigma, "sigma_err": sigma_err}


def Get_Time_Resolution(
    config,
    CFD="20",
    scope="keysight",
    run_number=None,
    *,
    xmin=None,
    xmax=None,
    nbin=None,
):

    ROOT.gROOT.SetBatch(True)
    ROOT.gStyle.SetOptFit(1)

    config_file = ConfigReader.open(config)

    if "keysight" in scope:
        trigger_resolution = 14.8  # ps 14.8+-0.1 for keysight 16.7 for lecroy
        trigger_resolution_err = 0.1
    else:
        trigger_resolution = 16.7  # ps 14.8+-0.1 for keysight 16.7 for lecroy
        trigger_resolution_err = 0.7

    output = {}
    for runIndex, run in enumerate(config_file):

        if run_number:
            run_num = run_number
        else:
            run_num = run.file_name.split("Sr_Run")[1].split("_")[0]

        result = _get_time_diff(
            run.file_name,
            run.cuts,
            CFD,
            run.dut_ch,
            run.trig_ch,
            return_histo=True,
            xmin=xmin,
            xmax=xmax,
            nbin=nbin,
        )

        if result["sigma"] < 1:
            result = _get_time_diff(
                run.file_name,
                run.cuts,
                CFD,
                run.dut_ch,
                run.trig_ch,
                return_histo=True,
                xmin=1,
                xmax=1,
                nbin=100,
            )

        # saving plots
        result["histo"].GetXaxis().SetTitle("Time Difference")
        c = ROOT.TCanvas(f"c{runIndex}")
        c.cd()
        result["histo"].Draw()
        c.SaveAs(f"bias_{run.bias}_temp_{run.temperature}C_tdiff_fit_CFD{CFD}.png")

        dut_time_res = math.sqrt(
            math.pow(result["sigma"], 2) - math.pow(trigger_resolution, 2)
        )

        sig_sq = math.pow(result["sigma"], 2)
        sig_err_sq = math.pow(result["sigma_err"], 2)
        trig_sq = math.pow(trigger_resolution, 2)
        trig_err_sq = math.pow(trigger_resolution_err, 2)
        dut_time_res_err = math.sqrt(
            sig_sq / (sig_sq - trig_sq) * sig_err_sq
            + trig_sq / (sig_sq - trig_sq) * trig_err_sq
        )

        output[(run.bias, run.cycle)] = [
            run_num,
            run.temperature,
            run.bias,
            dut_time_res,
            dut_time_res_err,
            run.cycle,
        ]

    return output


# main
if __name__ == "__main__":

    import argparse

    cml_parser = argparse.ArgumentParser()
    cml_parser.add_argument(
        "--CFD", dest="CFD", nargs="?", default="50", type=str, help="CFD"
    )
    cml_parser.add_argument(
        "--scope", dest="scope", nargs="?", default="lecroy", type=str, help="scope"
    )
    cml_parser.add_argument(
        "--xmin", dest="xmin", nargs="?", default=None, type=float, help="xmin"
    )
    cml_parser.add_argument(
        "--xmax", dest="xmax", nargs="?", default=None, type=float, help="xmax"
    )
    cml_parser.add_argument(
        "--nbin", dest="nbin", nargs="?", default=None, type=int, help="nbin"
    )
    cml_parser.add_argument(
        "--run", dest="nbin", nargs="?", default=None, type=int, help="run number"
    )

    argv = cml_parser.parse_args()

    output = Get_Time_Resolution(
        config=f"run_info_v{RUN_INFO_VER}.ini",
        CFD=argv.CFD,
        scope=argv.scope,
        run_number=argv.run,
        xmin=argv.xmin,
        xmax=argv.xmax,
        nbin=argv.nbin,
    )

    print("Run,Temp,Bias,Res,ResErr,cycle")
    for key, value in output.items():
        print(value)
    with open(f"res{CFD}.txt", "w") as f:
        for key, value in output:
            f.write(f"{','.join(value)}\n")
