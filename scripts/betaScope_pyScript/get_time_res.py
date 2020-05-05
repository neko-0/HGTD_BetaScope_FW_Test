'''
function to extract time resolution from the beta scope measurement
'''

import ROOT

def get_time_resolution( tfile_name, cuts, cfd, dut_ch, trig_ch, return_histo=False, xmin=None, xmax=None, nbin=None ):
    tfile = ROOT.TFile.Open(tfile_name, "r")
    ttree_wfm = tfile.wfm

    #parameter to project on the histogram. Time difference of dut and trig

    try:
        tdiff = "cfd%s[%s]-cfd%s[20]"%(dut_ch, int(cfd), trig_ch)
    except:
        if cfd == "tmax": tdiff = "tmax%s-cfd%s[20]"%(dut_ch, trig_ch)
        if cfd == "fit_tmax": tdiff = "fit_tmax%s-cfd%s[20]"%(dut_ch, trig_ch)
        if cfd == "zero_cross_tmax": tdiff = "zero_cross_tmax%s-cfd%s[20]"%(dut_ch, trig_ch)

    #create default histogram for pre-processing.
    preHisto = ROOT.TH1D("preHisto", "preHisto", 100, 1, 1)
    ttree_wfm.Project("preHisto", tdiff, cuts)
    sample_std = preHisto.GetStdDev(1)
    sample_mean = preHisto.GetMean(1)
    num_events = preHisto.GetEntries()
    IQR = 2*0.67845*sample_std #might not be the correct one

    #cfd time different histogram for timing resolution extraction
    bin_width = 2*IQR/pow(num_events, 1.0/3.0)
    #raw_input(bin_width)
    #raw_input(sample_std)
    #raw_input(sample_mean)
    min_range = sample_mean - 5.0*sample_std
    max_range = sample_mean + 5.0*sample_std
    num_bins = int((max_range-min_range)/bin_width)
    if xmin:
        min_range = xmin
    if xmax:
        max_range = xmax
    if nbin:
        num_bins = nbin
    tdiff_histo = ROOT.TH1D("tdiff_histo", "tdiff_histo", num_bins, min_range, max_range)
    ttree_wfm.Project("tdiff_histo", tdiff, cuts)
    gaussian = ROOT.TF1("gaussian", "gaus")
    tdiff_histo.Fit(gaussian)
    sigma = gaussian.GetParameter(2)
    sigma_err = gaussian.GetParError(2)

    #returning histogram
    if return_histo:
        tdiff_histo.SetDirectory(0)
        return {"histo":tdiff_histo, "sigma":sigma, "sigma_err":sigma_err}
    else:
        return {"sigma":sigma, "sigma_err":sigma_err}


#main
if __name__ == "__main__":

    import argparse
    cml_parser = argparse.ArgumentParser()
    cml_parser.add_argument("--CFD", dest="CFD", nargs="?", default="50", type=str, help="CFD")
    cml_parser.add_argument("--scope", dest="scope", nargs="?", default="lecroy", type=str, help="scope")
    cml_parser.add_argument("--xmin", dest="xmin", nargs="?", default=None, type=float, help="scope")
    cml_parser.add_argument("--xmax", dest="xmax", nargs="?", default=None, type=float, help="scope")
    cml_parser.add_argument("--nbin", dest="nbin", nargs="?", default=None, type=int, help="scope")

    argv = cml_parser.parse_args()

    import math
    ROOT.gROOT.SetBatch(True)
    ROOT.gStyle.SetOptFit(1)

    if "keysight" in argv.scope:
        trigger_resolution = 14.8 #ps 14.8+-0.1 for keysight 16.7 for lecroy
        trigger_resolution_err = 0.1
    else:
        trigger_resolution = 16.7 #ps 14.8+-0.1 for keysight 16.7 for lecroy
        trigger_resolution_err = 0.7

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
    run_num = ""

    output = []
    for runIndex in range(int(num_files)):

        fileName = file_prefix + config_file["run%s"%runIndex]["file_name"]

        cycle = 1
        if "root." in fileName:
            cycle = int(fileName.split("root.")[1])

        if not run_num:
            run_num = fileName.split("Sr_Run")[1].split("_")[0]

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
        result = get_time_resolution(fileName, cuts, argv.CFD, dut_ch, trig_ch, True, argv.xmin, argv.xmax, argv.nbin)

        dut_time_res = math.sqrt( math.pow(result["sigma"],2) - math.pow(trigger_resolution,2) )
        dut_time_res_err = math.sqrt( math.pow(result["sigma"],2)/(math.pow(result["sigma"],2) - math.pow(trigger_resolution,2))*math.pow(result["sigma_err"],2) + math.pow(trigger_resolution,2)/(math.pow(result["sigma"],2) - math.pow(trigger_resolution,2))*math.pow(trigger_resolution_err, 2))

        output.append("%s,%s,%s,%s,%s,%s"%(run_num, temperature, bias, dut_time_res,dut_time_res_err,cycle))

        #saving plots
        result["histo"].GetXaxis().SetTitle("Time Difference")
        c = ROOT.TCanvas("c%s"%runIndex)
        c.cd()
        result["histo"].Draw()
        c.SaveAs("bias_%s_temp_%sC_tdiff_fit_CFD%s.png"%(bias, temperature, argv.CFD))


    print("Sensor: %s"%sensor_name)
    print("Run,Temp,Bias,Res,ResErr,cycle")
    for o in output:
        print(o)
    with open("res{}.txt".format(argv.CFD),"w") as f:
        for o in output:
            f.write("{}\n".format(o) )
