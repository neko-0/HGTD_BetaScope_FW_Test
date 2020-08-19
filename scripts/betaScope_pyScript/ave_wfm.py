"""
Generate average waveform using ROOT.
"""
import os
import ROOT
from result_parser.config_reader import ConfigReader
import logging, coloredlogs

logging.basicConfig()
log = logging.getLogger(__name__)
coloredlogs.install(level="INFO", logger=log)

def ave_wfm(run, xbin=(50,-25e3,25e3), ybin=(2,-20,360)):
    """
    run (Obj::ConfigReader.Run)
    """
    log.info(f"Opening {run.file_name}")

    if not os.path.exists("ave_wfm_files/"):
        os.makedirs("ave_wfm_files/")

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
    i_ttree.Project(th2.GetName(), f"w{run.dut_ch}:t{run.dut_ch}", run.cuts)
    th2_projX = th2.ProfileX()

    o_tfile = ROOT.TFile.Open(f"ave_wfm_files/ave_wfm_{run.file_name}", "RECREATE")
    o_tfile.cd()
    th2_projX.Write()
    o_tfile.Close()

    log.info(f"Finished {run.file_name}")


if __name__ == "__main__":

    import multiprocessing as mp

    header, runs = ConfigReader.open()
    pool = mp.Pool()
    result = pool.map_async(ave_wfm, runs)
    pool.close()
    pool.join()
