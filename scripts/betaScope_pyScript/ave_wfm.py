"""
Generate average waveform using ROOT.
"""

import ROOT
from config_reader import ConfigReader


def ave_wfm(run):
    """
    run (Obj::ConfigReader.Run)
    """

    i_tfile = ROOT.TFile.Open(run.file_name)
    i_ttree = i_tfile.wfm

    xbin_w = 50  # ps
    xmin = -25e3
    xmax = 25e3

    ybin_w = 2
    ymin = -20
    ymax = 360

    th2 = ROOT.TH2D(
        "V{}".format(run.bias),
        "",
        int((xmax - xmin) / xbin_w),
        xmin,
        xmax,
        int((ymax - ymin) / ybin_w),
        ymin,
        ymax,
    )
    i_ttree.Project(th2.GetName(), "w{ch}:t{ch}".format(ch=run.dut_ch), run.cuts)
    th2_projX = th2.ProfileX()

    o_tfile = ROOT.TFile.Open("ave_wfm_{}".format(run.file_name), "RECREATE")
    o_tfile.cd()
    th2_projX.Write()
    o_tfile.Close()


if __name__ == "__main__":

    runs = ConfigParser.open()

    import multiprocessing as mp

    pool = mp.Pool(12)
    pool.map(ave_wfm, runs)
