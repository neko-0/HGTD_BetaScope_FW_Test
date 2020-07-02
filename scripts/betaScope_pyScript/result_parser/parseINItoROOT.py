import ROOT
from array import array
from parseBetaResultsToExcel import *

from ROOTFile import RootFile

import logging, coloredlogs

logging.basicConfig()
log = logging.getLogger(__name__)
coloredlogs.install(level="CRITICAL", logger=log)


def parseINItoROOT(fname="_results.ini"):
    config = configparser.ConfigParser()
    config.read(fname)
    config_section = config.sections()
    print(config_section)

    description_file = None
    for descr in os.listdir("./"):
        if "_Description.ini" in descr:
            description_file = configparser.ConfigParser()
            description_file.read(descr)
            print("found DAQ description file")
            break

    dut_trig = ["DUT"]

    tfile = ROOT.TFile("_results.root", "RECREATE")
    tfile.cd()
    branches = {}

    RunNum = 100
    SensorName = "Hi"
    Temp = 20
    trigBias = 395

    if description_file:
        try:
            RunNum = description_file["Run_Description"]["Run_Number"]

            SensorName = description_file["Run_Description"]["DUT_Senor_Name"]
            SensorName += (
                "-Fluence "
                + description_file["Run_Description"]["DUT_Fluence_Type"]
                + "-"
                + description_file["Run_Description"]["DUT_Fluence"]
            )
            SensorName += (
                "--"
                + description_file["Run_Description"]["DUT_Readout_Board"]
                + "-"
                + description_file["Run_Description"]["DUT_Readout_Board_Number"]
            )

            Temp = description_file["Run_Description"]["Temperature"]
            trigBias = description_file["Run_Description"]["Trigger_Voltage"]
        except Exception as e:
            print(Exception)
            RunNum = 100
            SensorName = "Hi"
            Temp = 20
            trigBias = 395

    Resistance = 4700

    for ch in dut_trig:
        rowCounter = 1

        ttree = ROOT.TTree("run" + str(RunNum), "from _results.ini")
        for par in beta_excel_dict.keys():
            if "SensorName" in par:
                branches[par] = array("b").frombytes(str(SensorName).encode())
                ttree.Branch(par, branches[par], "{}/C".format(par))
            elif "runNumber" in par:
                continue
            else:
                branches[par] = array("d", [0])
                ttree.Branch(par, branches[par], "{}/D".format(par))

        for bias in config_section:
            myRunNum = str(RunNum) + "->" + str(rowCounter)
            if ch in bias:
                if ch != "Trig":
                    if ".." in bias:
                        Bias = bias[bias.find("_") + 1 : bias.find("V")]
                        cycle = bias.split("..")[1]
                        if "_" in cycle:
                            cycle = cycle.split("_")[0]
                    else:
                        Bias = bias[bias.find("_") + 1 : bias.find("V")]
                        cycle = 1
                else:
                    try:
                        SensorName = description_file["Run_Description"][
                            "Trigger_Sensor_Name"
                        ]
                    except:
                        pass
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
                    if par == "SensorName":
                        continue
                    elif par == "runNumber":
                        continue
                    elif par == "Temp":
                        try:
                            Temp = config[bias]["temperature"]
                        except:
                            Temp = "-30"
                        branches[par][0] = float(Temp)
                    elif par == "Bias":
                        branches[par][0] = float(Bias)
                    elif par == "cycle":
                        branches[par][0] = float(cycle)
                    elif par == "Resistance":
                        branches[par][0] = float(Resistance)
                    else:
                        branches[par][0] = float(config[bias][par])
            ttree.Fill()

        ttree.Write("run" + str(RunNum), ROOT.TObject.kOverwrite)
        tfile.Close()


def parseINItoROOT2(fileout, title="Hi", run_folder="./", fname="_results.ini"):
    fileout.cd()
    config = configparser.ConfigParser()
    config.read(fname)
    config_section = config.sections()
    # print(config_section)

    description_file = None
    try:
        for descr in os.listdir(run_folder):
            if "_Description.ini" in descr:
                description_file = configparser.ConfigParser()
                description_file.read(descr)
                print("found DAQ description file")
                break
    except Exception as e:
        print(Exception)

    dut_trig = ["DUT"]

    branches = {}

    print(title)
    RunNum = title.split("_")[1]
    SensorName = title
    trigBias = 395
    Temp = 0

    if "_20C" in title:
        Temp = 20
        trigBias = 420
    if "_neg30C" in title:
        trigBias = 390
        Temp = -30
    if "_neg20C" in title:
        Temp = -20
    if "_neg10C" in title:
        Temp = -10

    if description_file:
        try:
            RunNum = description_file["Run_Description"]["Run_Number"]
            SensorName = description_file["Run_Description"]["DUT_Senor_Name"]
            SensorName += (
                "-Fluence "
                + description_file["Run_Description"]["DUT_Fluence_Type"]
                + "-"
                + description_file["Run_Description"]["DUT_Fluence"]
            )
            SensorName += (
                "--"
                + description_file["Run_Description"]["DUT_Readout_Board"]
                + "-"
                + description_file["Run_Description"]["DUT_Readout_Board_Number"]
            )

            Temp = description_file["Run_Description"]["Temperature"]
            trigBias = description_file["Run_Description"]["Trigger_Voltage"]
        except Exception as e:
            print(Exception)

    Resistance = 4700

    for ch in dut_trig:
        rowCounter = 1
        # print RunNum, title
        ttree = ROOT.TTree(str(RunNum), title)
        for par in beta_excel_dict.keys():
            if "SensorName" in par:
                branches[par] = array("b").frombytes(str(SensorName).encode())
                ttree.Branch(par, branches[par], "{}/C".format(par))
            elif "runNumber" in par:
                continue
            else:
                branches[par] = array("d", [0])
                ttree.Branch(par, branches[par], "{}/D".format(par))

        for bias in config_section:
            myRunNum = str(RunNum) + "->" + str(rowCounter)
            if ch in bias:
                if ch != "Trig":
                    if ".." in bias:
                        Bias = bias[bias.find("_") + 1 : bias.find("V")]
                        cycle = bias.split("..")[1]
                        if "_" in cycle:
                            cycle = cycle.split("_")[0]
                    else:
                        Bias = bias[bias.find("_") + 1 : bias.find("V")]
                        cycle = 1
                else:
                    try:
                        SensorName = description_file["Run_Description"][
                            "Trigger_Sensor_Name"
                        ]
                    except:
                        pass
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
                    if par == "SensorName":
                        continue
                    elif par == "runNumber":
                        continue
                    elif par == "Temp":
                        try:
                            Temp = config[bias]["temperature"]
                        except:
                            Temp = "-30"
                        branches[par][0] = float(Temp)
                    elif par == "Bias":
                        branches[par][0] = float(Bias)
                    elif par == "cycle":
                        branches[par][0] = float(cycle)
                    elif par == "Resistance":
                        branches[par][0] = float(Resistance)
                    else:
                        try:
                            branches[par][0] = float(config[bias][par])
                        except:
                            branches[par][0] = 0

            ttree.Fill()

        ttree.Write()  # "run"+str(RunNum), ROOT.TObject.kOverwrite)


def parseRawINIToROOT(filename="raw_results.ini"):
    config = configparser.ConfigParser()
    config.read(filename)
    output_file = RootFile("raw_results.root", "raw")
    created_branches = False
    for sec in config.sections():
        if not created_branches:
            for key in config[sec]:
                output_file.create_branch(key, "d")
            output_file.create_branch("bias", "d")
            output_file.create_branch("cycle", "i")
            created_branches = True
        for key in config[sec]:
            output_file[key][0] = float(config[sec][key])
        output_file["bias"][0] = float(sec.split("V")[0])
        output_file["cycle"][0] = (
            int(sec.split("findex")[1].split("_")[0]) if "findex" in sec else 1
        )
        """
        try:
            output_file["bias"][0] = sec.split("V")[0]
        except Exception as excep:
            log.warning(excep)
        try:
            output_file["cycle"][0] = int(sec.split(".")[1]) if sec.split(".")[1] != "" else 1
        except Exception as excep:
            log.warning(excep)
        """
        output_file.fill()


if __name__ == "__main__":
    parseINItoROOT()
    parseRawINIToROOT()
