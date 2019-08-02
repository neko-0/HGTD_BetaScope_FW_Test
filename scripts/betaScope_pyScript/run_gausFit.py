import histoGaussFit
import ROOT

#main
if __name__ == "__main__":

    import argparse
    cml_parser = argparse.ArgumentParser()
    cml_parser.add_argument("-var", dest="var", nargs="?", default="pmax2", type=str, help="variable expression")

    argv = cml_parser.parse_args()

    import math
    ROOT.gROOT.SetBatch(True)
    ROOT.gStyle.SetOptFit(1)

    import configparser
    config_file = configparser.ConfigParser()
    config_file.read( "run_info_v08022018.ini" )

    #get number of files(runs)
    num_files = config_file["header"]["number_of_runs"]

    file_prefix = ""
    if config_file["header"]["use_selected_events"] == "true":
        file_prefix = "Selected_"
    else:
        file_prefix = ""

    sensor_name = config_file["header"]["sensor"]
    dut_ch = config_file["header"]["dut_channel"]
    trig_ch = config_file["header"]["trigger_channel"]

    output = []
    for runIndex in range(int(num_files)):

        fileName = file_prefix + config_file["run%s"%runIndex]["file_name"]
        bias = int(config_file["run%s"%runIndex]["bias"].split("V")[0])
        try:
            temperature = config_file["run%s"%runIndex]["temperature"]
        except:
            temperature = -30
        raw_cut = config_file["run%s"%runIndex]["cut_%s"%dut_ch].split(" ")
        #raw_input(raw_cut)
        dut_cut = "tmax%s[0]-cfd3[20] > %s && tmax%s[0]-cfd3[20] < %s && pmax%s[0] > %s && pmax%s[0] < %s"%(dut_ch, raw_cut[0], dut_ch, raw_cut[1], dut_ch, raw_cut[2], dut_ch, raw_cut[3] )
        trig_cut = "tmax%s[0]-cfd3[20] > %s && tmax%s[0]-cfd3[20] < %s && pmax%s[0] > %s && pmax%s[0] < %s"%(trig_ch, raw_cut[4], trig_ch, raw_cut[5], trig_ch, raw_cut[6], trig_ch, raw_cut[7] )
        cuts = dut_cut + " && "+ trig_cut
        result = histoGaussFit.histoGaussFit(fileName, argv.var, cuts, True)

        output.append("%s,%s,%s,%s"%(temperature, bias, result["mean"], result["mean_err"]))

        #saving plots
        result["histo"].GetXaxis().SetTitle("%s"%argv.var)
        c = ROOT.TCanvas("c%s"%runIndex)
        c.cd()
        result["histo"].Draw()
        c.SaveAs("bias_%s_temp_%sC_%s.png"%(bias, temperature, argv.var))

    print("Sensor: %s"%sensor_name)
    print("Temp,Bias,Res,ResErr")
    for o in output:
        print(o)
