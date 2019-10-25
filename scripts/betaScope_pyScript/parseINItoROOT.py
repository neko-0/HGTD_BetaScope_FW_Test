import ROOT
from parseBetaResultsToExcel import *

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
            SensorName += "-Fluence "+description_file["Run_Description"]["DUT_Fluence_Type"]+"-"+description_file["Run_Description"]["DUT_Fluence"]
            SensorName += "--"+description_file["Run_Description"]["DUT_Readout_Board"]+"-"+description_file["Run_Description"]["DUT_Readout_Board_Number"]

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

        ttree = ROOT.TTree("run"+str(RunNum),"from _results.ini")
        for par in par_dict.keys():
            if "SensorName" in par:
                branches[par] = array("c", SensorName)
                ttree.Branch(par, branches[par], "{}/C".format(par) )
            elif "runNumber" in par:
                continue
            else:
                branches[par] = array("d", [0])
                ttree.Branch(par, branches[par], "{}/D".format(par) )

        for bias in config_section:
            myRunNum = str(RunNum)+"->"+str(rowCounter)
            if ch in bias:
                if ch != "Trig":
                    if ".." in bias:
                        Bias = bias[bias.find("_")+1:bias.find("V")]
                        cycle = bias.split("..")[1]
                        if "_" in cycle:
                            cycle = cycle.split("_")[0]
                    else:
                        Bias = bias[bias.find("_")+1:bias.find("V")]
                        cycle = 1
                else:
                    try:
                        SensorName = description_file["Run_Description"]["Trigger_Sensor_Name"]
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
                    if (par == "SensorName"):
                        continue
                    elif(par=="runNumber"):
                        continue
                    elif (par == "Temp"):
                        try:
                            Temp = config[bias]["temperature"]
                        except:
                            Temp = "-30"
                        branches[par][0]=Temp
                    elif (par == "Bias"):
                        branches[par][0] = float(Bias)
                    elif par=="cycle":
                            branches[par][0] = float(cycle)
                    elif (par == "Resistance"):
                        branches[par][0] = float(Resistance)
                    else:
                        branches[par][0] = float(config[bias][par])
            ttree.Fill()

        ttree.Write(key, ROOT.TObject.kOverwrite)
        tfile.Close()

if __name__=="__main__":
    parseINItoROOT()
