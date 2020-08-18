"""
Generate average waveform using ROOT.
"""

import ROOT
from result_parser.config_reader import ConfigReader

def ave_wfm(run, xbin=(50,-25e3,25e3), ybin=(2,-20,360)):
    """
    run (Obj::ConfigReader.Run)
    """
    print ("Opening", run.file_name)

    i_tfile = ROOT.TFile.Open(run.file_name)
    i_ttree = i_tfile.wfm

    xbin_w, xmin, xmax = xbin
    ybin_w, ymin, ymax = ybin

    th2 = ROOT.TH2D(
        f"V{run.bias}",
        "",
        int((xmax - xmin) / xbin_w),
        xmin,
        xmax,
        int((ymax - ymin) / ybin_w),
        ymin,
        ymax,
    )
    i_ttree.Project(th2.GetName(), f"w{ch}:t{ch}".format(ch=run.dut_ch), run.cuts)
    th2_projX = th2.ProfileX()

    o_tfile = ROOT.TFile.Open(f"ave_wfm_{run.file_name}", "RECREATE")
    o_tfile.cd()
    th2_projX.Write()
    o_tfile.Close()


if __name__ == "__main__":

    import multiprocessing as mp

    header, runs = ConfigReader.open()
    pool = mp.Pool()
    pool.map_async(ave_wfm, runs)
