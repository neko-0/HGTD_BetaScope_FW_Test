import ROOT
import argparse
import multiprocessing as mp

from config_reader import ConfigReader


def _read_current(run):
    tfile = ROOT.TFile.Open(run.file_name)
    ttree = tfile.Get("wfm")
    currentValue = 0
    histo = ROOT.TH1D(f"{run.file_name}_histo", "", 100, 1, 1)
    ttree.Project(histo.GetName(), "current")
    current = histo.GetMean()
    return (
        (run.bias, run.cycle),
        [
            run.sensor,
            run.temperature,
            run.bias,
            current,
            run.file_name.split("Sr_Run")[1].split("_")[0],
            run.cycle,
        ],
    )


def Read_Current(config):

    config_file = ConfigReader.open(config)

    pool = mp.Pool()
    output = pool.map_async(_read_current, config_file)
    pool.close()
    pool.join()

    output_data = output.get()

    return {key: value for key, value in output_data}


if __name__ == "__main__":

    commandline_ArgsParser = argparse.ArgumentParser()
    # commandline_ArgsParser.add_argument("--Vmin", dest="Vmin", nargs="?", default="0", type=int, help="min bias")
    # commandline_ArgsParser.add_argument("--Vmax", dest="Vmax", nargs="?", default="1000", type=int, help="max bias")
    # commandline_ArgsParser.add_argument("--binSize", dest="bin_size", nargs="?", default="10", type=float, help="bin size")
    # commandline_ArgsParser.add_argument("--low", dest="low_b", nargs="?", default="0", type=int, help="low bound")
    # commandline_ArgsParser.add_argument("--high", dest="high_b", nargs="?", default="500", type=int, help="high bound")
    # commandline_ArgsParser.add_argument("--param", dest="param", nargs="?", default="pmax2[0]", type=str, help="parameter")
    # commandline_ArgsParser.add_argument("--legTitle", dest="legTitle", nargs="?", default="", type=str, help="legend title")
    # commandline_ArgsParser.add_argument("--xtitle", dest="xtitle", nargs="?", default="", type=str, help="x axis tilte")
    # commandline_ArgsParser.add_argument("--temp", dest="temp", nargs="?", default="-30", type=str, help="temperature")
    commandline_ArgsParser.add_argument(
        "--configFile",
        dest="configFile",
        nargs="?",
        default="run_info_v08022018.ini",
        type=str,
        help="Configuration file",
    )

    argv = commandline_ArgsParser.parse_args()

    current_data = Read_Current(argv.configFile)
    print("Run,Temp,Bias,Current[uA],cycle")
    for _, item in current_data.items():
        print(f"{item[4]},{item[1]},{item[2]},{ item[3] * 1000.0},{item[5]}")

    with open("leakage.txt", "w") as f:
        for _, item in current_data.items():
            f.write(f"{item[4]},{item[1]},{item[2]},{item[3] * 1000.0},{item[5]}\n")
