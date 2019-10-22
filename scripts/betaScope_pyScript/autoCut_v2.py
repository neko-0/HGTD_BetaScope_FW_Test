import ROOT
import configparser
import os
import argparse
from array import array

def generate_cuts( dutCh, trigCh, runNum, configFileName, trigFixedCut, tmax_min_b=-2000, tmax_max_b=2000 ):
    ROOT.gROOT.SetBatch(True)
    betaRunNum = runNum
    betaRunConfig = configparser.ConfigParser()
    betaRunConfig.read( configFileName )
    num_file = betaRunConfig["header"]["number_of_runs"]
    trigger_preset = trigFixedCut #{}"200 500 70 375"

    numOfSigma = 3.0

    tmax_expr = "tmax%s[0]-cfd%s[20]"%(dutCh, trigCh)
    pmax_expr = "pmax%s[0]"%(dutCh)

    for i in range(int(num_file)):
        runNum = "run"+str(i)
        fileName = betaRunConfig[runNum]["file_name"]
        try:
            spliter = "stats_Sr_Run"+str(betaRunNum)+"_"
            biasVoltage = fileName.split(spliter)[1].split("_trig")[0]
        except:
            spliter = "stats_unseg_Sr_Run"+str(betaRunNum)+"_"
            biasVoltage = fileName.split(spliter)[1].split("_trig")[0]
        betaRunConfig.set( runNum, "bias", biasVoltage)

        tfile = ROOT.TFile.Open(fileName)
        ttree = tfile.Get("wfm")
        INVALID=False
        for entry in ttree:
            try:
                getattr(entry,'cfd%s'%dutCh)
            except:
                print("get CFD fail")
                INVALID=True
            break
        if INVALID: break
        tmax_min_bound = tmax_min_b
        tmax_max_bound = tmax_max_b

        '''
        pmax_vs_tmax = ROOT.TH2D("pmax_vs_tmax", "pmax_vs_tmax", int((tmax_max_bound-tmax_min_bound)/50), tmax_max_bound, tmax_min_bound, 500, -10, 400)
        ttree.Project("pmax_vs_tmax", "%s:%s"%(pmax_expr, tmax_expr), "%s>%s && %s<%s"%(tmax_expr, tmax_min_bound, tmax_expr, tmax_max_bound))
        pmax_vs_tmax_proj = pmax_vs_tmax.ProfileX()
        pmax = ROOT.RooRealVar("pmax", "pmax", -2000, 2000)
        tmax = ROOT.RooRealVar("tmax", "tmax", -2000, 2000)
        Roohisto = ROOT.RooDataHist("Roohisto", "Tmax distribution", ROOT.RooArgList(tmax), pmax_vs_tmax_proj)
        tmax_mean = ROOT.RooRealVar("mean", "guassian mean",-1500, 1500)
        tmax_width = ROOT.RooRealVar("sigma", "guassian width", 1, 1000)
        gaussian = ROOT.RooGaussian("gaussian", "guassian for signal", tmax, tmax_mean, tmax_width)
        poly0 = ROOT.RooPolynomial("poly0", "poly0", tmax)
        gauss_coeff = ROOT.RooRealVar("gauss_coeff", "gaussian yiled", 0.5, 0, 1)
        tmaxModel = ROOT.RooAddPdf("tmaxModel", "Tmax Fit Model", ROOT.RooArgList(gaussian,poly0), ROOT.RooArgList(gauss_coeff))
        frame = tmax.frame()
        tmaxModel.fitTo(Roohisto)
        Roohisto.plotOn(frame)
        tmaxModel.plotOn(frame)
        #pmax_vs_tmax.Draw("same")
        frame.Draw()
        raw_input("pause")
        '''

        histo = ROOT.TH1D("histo", "Tmax Distribution", 500, 1, 1)
        ttree.Project("histo", tmax_expr, "%s>%s && %s<%s"%(tmax_expr, tmax_min_bound, tmax_expr, tmax_max_bound))
        tmax = ROOT.RooRealVar("tmax", "tmax", -2000, 2000)
        tmax_mean = ROOT.RooRealVar("mean", "guassian mean",-1500, 1500)
        tmax_width = ROOT.RooRealVar("sigma", "guassian width", 1, 1000)
        Roohisto = ROOT.RooDataHist("Roohisto", "Tmax distribution", ROOT.RooArgList(tmax), histo)
        gaussian = ROOT.RooGaussian("gaussian", "guassian for signal", tmax, tmax_mean, tmax_width)
        poly0 = ROOT.RooPolynomial("poly0", "poly0", tmax)
        gauss_coeff = ROOT.RooRealVar("gauss_coeff", "gaussian yiled", 0.5, 0, 1)
        tmaxModel = ROOT.RooAddPdf("tmaxModel", "Tmax Fit Model", ROOT.RooArgList(gaussian,poly0), ROOT.RooArgList(gauss_coeff))
        frame = tmax.frame()
        tmaxModel.fitTo(Roohisto)
        Roohisto.plotOn(frame)
        tmaxModel.plotOn(frame)
        #Roohisto.plotOn(frame)
        #frame.Draw()
        fitted_mean = tmax_mean.getValV()
        fitted_sigma = tmax_width.getValV()
        tCut = "{} {}".format(fitted_mean-numOfSigma*fitted_sigma, fitted_mean+numOfSigma*fitted_sigma)
        print(fitted_sigma)

        os.system("clear")
        pmaxSignal = ROOT.RooRealVar("pmaxSignal", "pmax signal region", 50, -60, 360)
        landauMean = ROOT.RooRealVar("landauMean", "landau mean", 50, -50, 360)
        landauSigma = ROOT.RooRealVar("landauSigma", "landau sigma", 30, 1, 300)
        pmaxSignalHisto = ROOT.TH1D("pmaxSignalHisto", "Pmax Distribution in Signal Region", 250, -50, 360)
        ttree.Project("pmaxSignalHisto", pmax_expr, "%s < %s && %s < %s"%( fitted_mean-numOfSigma*fitted_sigma, tmax_expr, tmax_expr, fitted_mean+numOfSigma*fitted_sigma) )
        RoopmaxSignalHisto = ROOT.RooDataHist("RoopmaxSignalHisto", "Pmax distribution in Singal", ROOT.RooArgList(pmaxSignal), pmaxSignalHisto)
        landauDistri = ROOT.RooLandau("landauDistri", "landau distribution", pmaxSignal, landauMean, landauSigma)
        gaussNoise_mean = ROOT.RooRealVar("gaussNoise_mean", "gaussNoise_mean", 50, -50, 360)
        gaussNoise_sigma = ROOT.RooRealVar("gaussNoise_sigma", "gaussNoise_sigma", 50, -50, 360)
        gaussianNoise = ROOT.RooGaussian("gaussianNoise", "gaussianNoise", pmaxSignal, gaussNoise_mean, gaussNoise_sigma)
        pmaxSignal.setBins(10000,"cache")
        signal_plus_bkg = ROOT.RooFFTConvPdf("lxg","landau (X) gauss", pmaxSignal, landauDistri, gaussianNoise)
        frame2 = pmaxSignal.frame()
        signal_plus_bkg.fitTo(RoopmaxSignalHisto)
        RoopmaxSignalHisto.plotOn(frame2)
        signal_plus_bkg.plotOn(frame2)
        #frame2.Draw()
        tf1 = signal_plus_bkg.asTF(ROOT.RooArgList(pmaxSignal))
        pmax_peak = tf1.GetMaximumX()
        #raw_input(pmax_peak)

        #sideband Region
        os.system("clear")
        pmaxNoise = ROOT.RooRealVar("pmaxNoise", "pmaxNoise", 0, -80, 80)
        pmaxSidebandHisto = ROOT.TH1D("pmaxSidebandHisto", "Pmax Distribution in sideband Region", 200, -80, 80)
        ttree.Project("pmaxSidebandHisto", pmax_expr, "%s < %s || %s > %s"%( tmax_expr, fitted_mean-numOfSigma*fitted_sigma, tmax_expr, fitted_mean-numOfSigma*fitted_sigma) )
        RoopmaxSidebandHisto = ROOT.RooDataHist("RoopmaxSidebandHisto", "Pmax distribution in sideband", ROOT.RooArgList(pmaxNoise), pmaxSidebandHisto)
        sideband_gaussNoise_mean = ROOT.RooRealVar("sideband_gaussNoise_mean", "sideband_gaussNoise_mean", 50, -80, 80)
        sideband_gaussNoise_sigma = ROOT.RooRealVar("sideband_gaussNoise_sigma", "sideband_gaussNoise_sigma", 50, 1, 80)
        sideband_gaussianNoise = ROOT.RooGaussian("sideband_gaussianNoise", "sideband_gaussianNoise", pmaxNoise, sideband_gaussNoise_mean, sideband_gaussNoise_sigma)
        frame3 = pmaxNoise.frame()
        sideband_gaussianNoise.fitTo(RoopmaxSidebandHisto)
        RoopmaxSidebandHisto.plotOn(frame3)
        sideband_gaussianNoise.plotOn(frame3)
        #frame3.Draw()
        noise_mean = sideband_gaussNoise_mean.getValV()
        #raw_input()

        outPlot = ROOT.TCanvas("c")
        outPlot.cd()

        #Drawing tmax vs pmax plot
        x = array("d")
        y = array("d")

        threshold = []
        for i in range(100):
            threshold.append( [5*1.6+0.5*i,0] )

        for ientry, entry in enumerate(ttree):
            x.append(getattr(entry, "pmax%s"%dutCh)[0])
            y.append(getattr(entry,"tmax%s"%dutCh)[0] - getattr(entry,"cfd%s"%trigCh)[20] )
            if ientry%1000==0:print("@ event {}".format(ientry))
            for item in threshold:
                if fitted_mean-numOfSigma*fitted_sigma < y[ientry] and y[ientry] < fitted_mean+numOfSigma*fitted_sigma and x[ientry] > item[0]:
                    item[1]+=1
        graph = ROOT.TGraph(len(x), x, y)
        graph.GetXaxis().SetTitle("Pmax_ch%s[mV]"%dutCh)
        graph.GetYaxis().SetTitle("Tmax_ch%s-TrigCFD_ch%s[ps]"%(dutCh,trigCh) )
        graph.GetYaxis().SetRangeUser(fitted_mean-7.0*fitted_sigma, fitted_mean+7.0*fitted_sigma)
        graph.GetXaxis().SetRangeUser(0, 5*pmax_peak)
        tCutLine_min = ROOT.TLine(0, fitted_mean-numOfSigma*fitted_sigma, graph.GetXaxis().GetXmax(), fitted_mean-numOfSigma*fitted_sigma )
        tCutLine_max = ROOT.TLine(0, fitted_mean+numOfSigma*fitted_sigma, graph.GetXaxis().GetXmax(), fitted_mean+numOfSigma*fitted_sigma )
        graph.SetMarkerStyle(7)
        graph.Draw("ap")
        tCutLine_min.SetLineColor(ROOT.kRed)
        tCutLine_min.Draw()
        tCutLine_max.SetLineColor(ROOT.kRed)
        tCutLine_max.Draw()

        #determine pmax cut base on the efficiency
        import numpy
        tot_events = threshold[0][1]
        efficiency = []
        threshold_cut = []
        for i,item in enumerate(threshold):
            if i>0:
                threshold_cut.append(item[0])
                efficiency.append(float(item[1])/tot_events)
                #print("%s,%s"%(threshold_cut[i-1],efficiency[i-1]))
        #raw_input()
        #dydx_1 = numpy.diff(efficiency)/numpy.diff(threshold_cut)
        #threshold_cut = threshold_cut[:len(threshold_cut)-1]
        find_good_threshold = False
        typical_noise = 1.6
        '''
        for th,dydx in zip(threshold_cut,dydx_1):
            print("%s,%s"%(th,dydx))
        raw_input()
        '''
        apply_th_cut = typical_noise
        for index, (th,effi) in enumerate(zip(threshold_cut,efficiency),start=0):
            #if th >= 5.0*typical_noise and -0.015 < dydx and dydx < -0.0095:
            #raw_input(effi)
            if th >= 5.0*typical_noise and effi >= 0.99: #0.975
                apply_th_cut = th
                find_good_threshold = True
                #raw_input(effi)
            #else:
                #break

        '''
        dydx_2 = numpy.diff(dydx_1)/numpy.diff(threshold_cut)
        threshold_cut = threshold_cut[:len(threshold_cut)-1]
        dydx_3 = numpy.diff(dydx_2)/numpy.diff(threshold_cut)
        threshold_cut = threshold_cut[:len(threshold_cut)-1]
        finding zero-crossing
        find_zero = False
        find_2nd_zero = False
        crossing_index = 0
        for index, (th, _3rd) in enumerate(zip(threshold_cut,dydx_3)):
            #print("%s,%s"%(th,_3rd))
            if _3rd < 0 and dydx_3[index+1] >= 0:
                if not find_zero:
                    find_zero = True
                    crossing_index = index+1
                if find_zero and not find_2nd_zero:
                    find_2nd_zero = True
                    crossing_index = index+1
                if find_zero and find_2nd_zero:
                    break
        #raw_input()
        zero_point = 0
        if find_2nd_zero or find_zero:
            slope = float((dydx_3[crossing_index]-dydx_3[crossing_index-1]))/(threshold_cut[crossing_index]-threshold_cut[crossing_index-1])
            interception =  dydx_3[crossing_index]-slope*threshold_cut[crossing_index]
            zero_point = abs(interception/slope)
            #raw_input(zero_point)
        '''

        pmaxCutLine_min = ""
        pmaxCut = ""
        #if find_2nd_zero or find_zero:
        if find_good_threshold:
            pmaxCutLine_min = ROOT.TLine( apply_th_cut, graph.GetYaxis().GetXmin(), apply_th_cut, graph.GetYaxis().GetXmax() )
            pmaxCutLine_max = ROOT.TLine( 360, graph.GetYaxis().GetXmin(), 360, graph.GetYaxis().GetXmax() )
            pmaxCut = "{} {}".format(apply_th_cut, 360)
        else:
            pmaxCutLine_min = ROOT.TLine( typical_noise, graph.GetYaxis().GetXmin(), typical_noise, graph.GetYaxis().GetXmax() )
            pmaxCutLine_max = ROOT.TLine( 360, graph.GetYaxis().GetXmin(), 360, graph.GetYaxis().GetXmax() )
            pmaxCut = "{} {}".format(typical_noise, 360)
        '''
        else:
            if abs(noise_mean - pmax_peak) < 5.0:
                pmaxCutLine_min = ROOT.TLine( noise_mean/2.0, graph.GetYaxis().GetXmin(), noise_mean/2.0, graph.GetYaxis().GetXmax() )
                pmaxCutLine_max = ROOT.TLine( 360, graph.GetYaxis().GetXmin(), 360, graph.GetYaxis().GetXmax() )
                pmaxCut = "{} {}".format(noise_mean/2.0, 360)
            elif abs(noise_mean - pmax_peak) > 5.0 and abs(noise_mean - pmax_peak) < 15.0:
                pmaxCutLine_min = ROOT.TLine( noise_mean, graph.GetYaxis().GetXmin(), noise_mean, graph.GetYaxis().GetXmax() )
                pmaxCutLine_max = ROOT.TLine( 360, graph.GetYaxis().GetXmin(), 360, graph.GetYaxis().GetXmax() )
                pmaxCut = "{} {}".format(noise_mean, 360)
            else:
                min_line = (pmax_peak-noise_mean)/2.0
                if min_line > noise_mean: min_line = noise_mean
                pmaxCutLine_min = ROOT.TLine( min_line, graph.GetYaxis().GetXmin(), min_line, graph.GetYaxis().GetXmax() )
                pmaxCutLine_max = ROOT.TLine( 360, graph.GetYaxis().GetXmin(), 360, graph.GetYaxis().GetXmax() )
                pmaxCut = "{} {}".format(min_line, 360)
        '''

        pmaxCutLine_min.SetLineColor(ROOT.kRed)
        pmaxCutLine_min.Draw()
        pmaxCutLine_max.SetLineColor(ROOT.kRed)
        pmaxCutLine_max.Draw()

        #raw_input()

        allCut = tCut + " " + pmaxCut + " " + trigger_preset
        betaRunConfig.set( runNum, "cut_%s"%dutCh, allCut)

        #parse the temperature and trigger bias voltage, for temperatrue scan measurement.
        try:
            temperature = betaRunConfig[runNum]["file_name"].split("_temp")[1].split(".root")[0]
        except:
            temperature = "-273"
        trigger_bias = betaRunConfig[runNum]["file_name"].split("_trig")[1].split("V_")[0]
        betaRunConfig.set( runNum, "temperature", temperature )
        betaRunConfig.set( runNum, "trigger_bias", trigger_bias )

        outPlot.SaveAs("%s_ch%s_tmax_pmax.png"%(fileName, dutCh))


        with open( "%s"%configFileName, "wb") as configOutput:
            betaRunConfig.write(configOutput,space_around_delimiters=False)

        folderName="tmax_autoCut"
        if not os.path.isdir(folderName):
            os.mkdir(folderName)
        os.system("mv *png tmax_autoCut")

if __name__ == '__main__':

    commandline_ArgsParser = argparse.ArgumentParser()
    commandline_ArgsParser.add_argument("--dutCh", dest="dutCh", nargs="?", default=2, type=str, help="DUT Channel")
    commandline_ArgsParser.add_argument("--trigCh", dest="trigCh", type=str, nargs="?", default=3, help="Trigger Channel")
    commandline_ArgsParser.add_argument("--runNum", dest="runNum", type=int, help="Run number")
    commandline_ArgsParser.add_argument("--configFile", dest="configFile", nargs="?", default="run_info_v08022018.ini", type=str, help="Configuration file")
    commandline_ArgsParser.add_argument("--trigFixedCut", dest="trigFixedCut", nargs="?", default="200 500 70 375", type=str, help="Preset trigger cut")
    commandline_ArgsParser.add_argument("--tmax_minB", dest="tmax_minB", nargs="?", default=-2000, type=int, help="dut tmax min bound")
    commandline_ArgsParser.add_argument("--tmax_maxB", dest="tmax_maxB", nargs="?", default=2000, type=int, help="dut tmax max bound")
    argv = commandline_ArgsParser.parse_args()

    generate_cuts( argv.dutCh, argv.trigCh, argv.runNum, argv.configFile, argv.trigFixedCut, argv.tmax_minB, argv.tmax_maxB )
