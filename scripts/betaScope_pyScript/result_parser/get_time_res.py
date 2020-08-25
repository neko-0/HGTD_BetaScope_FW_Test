"""
function to extract time resolution from the beta scope measurement
"""

import os
import math
import configparser

import ROOT

from config_reader import ConfigReader
from config_reader import RUN_INFO_VER

from trig_cali import TRIG_CALI

import logging

logging.basicConfig()
logging.getLogger().setLevel(logging.INFO)
log = logging.getLogger(__name__)


def Compute_Res(sigma, sigma_err, trig_res, trig_res_err):
    sq_diff = math.pow(sigma, 2) - math.pow(trig_res, 2)
    if sq_diff < 0:
        return (0, 0)
    else:
        dut_time_res = math.sqrt(sq_diff)

    sig_sq = math.pow(sigma, 2)
    sig_err_sq = math.pow(sigma_err, 2)
    trig_sq = math.pow(trig_res, 2)
    trig_err_sq = math.pow(trig_res_err, 2)
    dut_time_res_err = math.sqrt(
        sig_sq / (sig_sq - trig_sq) * sig_err_sq
        + trig_sq / (sig_sq - trig_sq) * trig_err_sq
    )

    return (dut_time_res, dut_time_res_err)


def Get_Time_Diff(
    tfile_name,
    cuts,
    cfd,
    dut_ch,
    trig_var="cfd3[20]",
    return_histo=False,
    xmin=None,
    xmax=None,
    nbin=None,
):
    tfile = ROOT.TFile.Open(tfile_name, "r")
    ttree_wfm = tfile.wfm

    # parameter to project on the histogram. Time difference of dut and trig
    res_type_dict = {
        "tmax": f"tmax{dut_ch}-{trig_var}",
        "fit_tmax": f"fit_tmax{dut_ch}-{trig_var}",
        "zero_cross_tmax": f"zero_cross_tmax{dut_ch}-{trig_var}",
    }

    if cfd in res_type_dict.keys():
        tdiff = res_type_dict[cfd]
    else:
        tdiff = f"cfd{dut_ch}[{cfd}]-{trig_var}"

    # create default histogram for pre-processing.
    preHisto = ROOT.TH1D("preHisto", "preHisto", 100, 1, 1)
    try:
        ttree_wfm.Project("preHisto", tdiff, cuts)
    except:
        return None
    sample_std = preHisto.GetStdDev(1)
    sample_mean = preHisto.GetMean(1)
    num_events = preHisto.GetEntries()
    if num_events == 0:
        log.critical(f"0 event after filling {tdiff} !")
        return None
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
        tdiff_histo.GetXaxis().SetTitle(f"Time Difference: {tdiff}")
        return {"histo": tdiff_histo, "sigma": sigma, "sigma_err": sigma_err}
    else:
        return {"sigma": sigma, "sigma_err": sigma_err}


def Get_Time_Resolution(
    config,
    CFD="20",
    scope=None,
    trig_name=None,
    run_number=None,
    trig_cali="default",
    *,
    get_histo=True,
    xmin=None,
    xmax=None,
    nbin=None,
):

    ROOT.gROOT.SetBatch(True)
    ROOT.gStyle.SetOptFit(1)

    header, config_file = ConfigReader.open(config)

    # getting trigger time resolution information
    if header["trigger_res"] != "NA":
        trig_res = header["trigger_res"]
        trig_res_err = header["trigger_res_err"]
        trig_var = header["trigger_var"]
    else:
        trig_cali_run = sorted(list(TRIG_CALI.keys()))
        use_cali_run = None
        for i in range(len(trig_cali_run)):
            try:
                if trig_cali_run[i] <= run_number:
                    use_cali_run = trig_cali_run[i]
            except:
                pass
        if use_cali_run:
            trig_info = TRIG_CALI[use_cali_run]
            try:
                trig_res = trig_info[(scope, trig_name, trig_cali)]["res"]
                trig_res_err = trig_info[(scope, trig_name, trig_cali)]["res_err"]
                trig_var = trig_info[(scope, trig_name, trig_cali)]["var"]
            except KeyError:
                log.warning(
                    f"{(scope, trig_name, trig_cali)} is not in cali {use_cali_run}"
                )
                trig_res = 17.0
                trig_res_err = 1.0
                trig_var = "cfd3[20]"
        else:
            log.warning("Cannot match trigger calibration run number.")
            trig_res = 17.0
            trig_res_err = 1.0
            trig_var = "cfd3[20]"

    log.info(f"Trigger info: {trig_res}+-{trig_res_err}, var {trig_var}")

    output = {}
    for runIndex, run in enumerate(config_file):

        if run_number:
            run_num = run_number
        else:
            run_num = run.file_name.split("Sr_Run")[1].split("_")[0]

        result = Get_Time_Diff(
            run.file_name,
            run.cuts,
            CFD,
            run.dut_ch,
            trig_var.format(trig_ch=run.trig_ch),
            return_histo=get_histo,
            xmin=xmin,
            xmax=xmax,
            nbin=nbin,
        )

        if result is None:
            return None

        if result["sigma"] < 1:
            result = Get_Time_Diff(
                run.file_name,
                run.cuts,
                CFD,
                run.dut_ch,
                trig_var.format(trig_ch=run.trig_ch),
                return_histo=get_histo,
                xmin=1,
                xmax=1,
                nbin=100,
            )

        # saving plots
        c = ROOT.TCanvas(f"c{runIndex}")
        c.cd()
        result["histo"].Draw()

        if not os.path.exists(f"plots.{run.file_name}/"):
            os.makedirs(f"plots.{run.file_name}/")
        
        c.SaveAs(
            f"plots.{run.file_name}/bias_{run.bias}_temp_{run.temperature}C_tdiff_CFD{CFD}.png"
        )

        dut_time_res, dut_time_res_err = Compute_Res(
            result["sigma"], result["sigma_err"], trig_res, trig_res_err
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
