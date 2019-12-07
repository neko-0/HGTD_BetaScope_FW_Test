from runMatch import runlist_from_root, runMatch
from copy import deepcopy
import os
import ROOT

class PlotSensor(object):
    def __init__(self, sensor_name):
        self.sensor_name = sensor_name
        self.plot_data = {}

    def add_plot_data(self, name, plot_data):
        self.plot_data[name] = plot_data

class PlotData(object):
    def __init__(self, name, xtitle, ytitle, marker_style, marker_color):
        self.name = name
        self.xtitle = xtitle
        self.ytitle = ytitle
        self.marker_style = marker_style
        self.marker_color = marker_color
        self.plot = None
        self.max = None

    def add_plot(self, plot):
        self.plot = plot

class PlotMakerBase(object):
    def __init__(self, filename):
        self.filename = filename

param = {
"bias_voltage_vs_charge":
    {
        "expr":"bias_voltage:new_pulse_area/4700.0",
        "xtitle":"Bias Voltage [V]",
        "ytitle":"Charge [fC]"
    },
"time_resolution_50_vs_charge":
    {
        "expr":"new_pulse_area/4700.0:time_resolution_50",
        "xtitle":"Charge [fC]",
        "ytitle":"Time Resolution [ps]"
    },
"bias_voltage_vs_pmax":
    {
        "expr":"bias_voltage:pmax",
        "xtitle":"Bias Voltage [V]",
        "ytitle":"Pmax [mV]"
    },
"bias_voltage_vs_rise_time":
    {
        "expr":"bias_voltage:rise_time",
        "xtitle":"Bias Voltage [V]",
        "ytitle":"rise time [ps]"
    },
"bias_voltage_vs_fwhm":
    {
        "expr":"bias_voltage:fwhm",
        "xtitle":"Bias Voltage [V]",
        "ytitle":"FWHM [ps]"
    },
"bias_voltage_vs_res50":
    {
        "expr":"bias_voltage:time_resolution_50",
        "xtitle":"Bias Voltage [V]",
        "ytitle":"Time Resolution [ps]"
    },
}

class PlotMaker(PlotMakerBase):
    def __init__(self, filename, backend="root"):
        super(PlotMaker, self).__init__(filename)
        self.backend = backend
        if self.backend is "root":
            try:
                from atlasplots import atlas_style
                atlas_style.SetAtlasStyle()
            except:
                pass
            import ROOT
            from array import array
            self.runlist = runlist_from_root(self.filename)
        self.sensor_list = {}
        self.matched_runs = {}

    def show_sensors(self):
        for run,run_name in self.runlist:
            print("{} {}".format(run,run_name))
        print("total {}".format(len(self.runlist)))

    def show(self, name_tag):
        for run,run_name in self.matched_runs[name_tag]:
            print("{} {}".format(run,run_name))
        print("total {}".format(len(self.matched_runs[name_tag])))

    def filter(self, name_tag, name_pattern):
        if not name_tag in self.matched_runs:
            self.matched_runs[name_tag] = []
            runlist_for_compare = self.runlist
        else:
            runlist_for_compare = self.matched_runs[name_tag]
        matched = runMatch(name_pattern, runlist_for_compare)
        self.matched_runs[name_tag] = [(run,run_name) for run,run_name in matched]
        return deepcopy(self)

    def get_matehced_runs(self, name_tag):
        return self.matched_runs[name_tag]

    def fetchRunNumbers(self, run_list):
        #copy_self = deepcopy(self)
        matched = []
        for run_number in run_list:
            run_number = "Sr_Run%s"%run_number
            allruns = runlist_from_root(self.filename)

            for run in allruns:
                if run_number == run[0]:
                    if run_number in self.sensor_list: continue
                    self.sensor_list[run_number] = []
                    print run[1]
                    if self.backend is "root":
                        tfile = ROOT.TFile.Open(self.filename, "r")
                    else:
                        tfile = ""

                    ttree = tfile.Get(run_number)

                    color = 2
                    sensor = PlotSensor(run_number)
                    matched.append([ttree.GetName(), ttree.GetTitle()])
                    for par_name,par in param.items():
                        plotdata = PlotData(par_name, par["xtitle"], par["ytitle"], 20, color)#run_info["style"], run_info["color"])
                        n = ttree.Draw(par["expr"], "", "")
                        if n <= 0:
                            print("no points!")
                            continue
                        d = ttree.GetV2()
                        max = 0
                        i = 0
                        while i < 100:
                            if d[i]:
                                if max < d[i]:max=d[i]
                                i+=1
                            else:
                                break
                        g = ROOT.TGraph(n, ttree.GetV1(), ttree.GetV2() )
                        g.GetXaxis().SetTitle(par["xtitle"])
                        g.GetYaxis().SetTitle(par["ytitle"])
                        g.SetNameTitle(run_number, run_number)
                        #g.SetName(run_number)
                        g.SetMarkerColor(color)
                        g.SetMarkerStyle(20)#run_info["style"])
                        g.SetLineColor(color)

                        plotdata.add_plot(g)
                        plotdata.max = max
                        sensor.add_plot_data(par_name,plotdata)
                        self.sensor_list[run_number].append(sensor)
                    if color < 20: color+=1
                    else: color = 2

        self.runlist = matched
        return self


    def fetchRun(self, run_info_list):
        copy_self = deepcopy(self)
        for run_info in run_info_list:
            matched = runMatch(run_info["reg"], copy_self.runlist)

            if copy_self.backend is "root":
                tfile = ROOT.TFile.Open(copy_self.filename, "r")
            else:
                tfile = ""
            if matched:
                copy_self.sensor_list[run_info["nick_name"]] = []
                color = 2
                for run, run_name in matched:
                    sensor = PlotSensor(run_name)
                    copy_self.sensor_list[run_info["nick_name"]].append(sensor)
                    ttree = tfile.Get(run)
                    for par_name,par in param.items():
                        plotdata = PlotData(par_name, par["xtitle"], par["ytitle"], run_info["style"], run_info["color"])
                        n = ttree.Draw(par["expr"], "", "goff")
                        if n <= 0:
                            print("no points!")
                            continue
                        d = ttree.GetV2()
                        max = 0
                        i = 0
                        while i < 100:
                            if d[i]:
                                if max < d[i]:max=d[i]
                                i+=1
                            else:
                                break
                        g = ROOT.TGraph(n, ttree.GetV1(), ttree.GetV2() )
                        g.GetXaxis().SetTitle(par["xtitle"])
                        g.GetYaxis().SetTitle(par["ytitle"])
                        g.SetNameTitle(run_name, run_name)
                        g.SetMarkerColor(color)
                        g.SetMarkerStyle(run_info["style"])
                        g.SetLineColor(color)
                        plotdata.add_plot(g)
                        plotdata.max = max
                        sensor.add_plot_data(par_name,plotdata)
                    if color < 20: color+=1
                    else: color = 2

        copy_self.runlist = matched
        return copy_self


    def makePlots(self, output_name=""):
        #ROOT.gROOT.SetBatch(True)
        if output_name:
            try:
                os.makedirs("./user_data/{}".format(output_name))
            except:
                pass
        for entry, param_name in enumerate(param.keys()):
            leg = ROOT.TLegend(0.19,0.55,0.7,0.95)
            leg.SetTextSize(0.02)
            leg.SetBorderSize(0)
            canvas = ROOT.TCanvas("canvas", "canvas", 1800, 1200)
            canvas.cd()
            #first_draw = 0

            h = ROOT.TH1F("h", "h", 1000, 0, 800)
            h.Draw()
            maxY = 0
            maxX = 0

            for nick, sensors in self.sensor_list.items():
                for sensor in sensors:
                    if not param_name in sensor.plot_data:
                        continue
                    if len(sensor.sensor_name.split("_Trig")) > 1:
                        leg.AddEntry(sensor.plot_data[param_name].plot, sensor.sensor_name.split("_Trig")[0])
                    else:
                        leg.AddEntry(sensor.plot_data[param_name].plot, sensor.sensor_name.split("_trig")[0])
                    sensor.plot_data[param_name].plot.Draw("psame")

                    h.GetXaxis().SetTitle(sensor.plot_data[param_name].plot.GetXaxis().GetTitle())
                    h.GetYaxis().SetTitle(sensor.plot_data[param_name].plot.GetYaxis().GetTitle())

                    if maxY < sensor.plot_data[param_name].max: maxY = sensor.plot_data[param_name].max
                    if maxX < sensor.plot_data[param_name].plot.GetX()[sensor.plot_data[param_name].plot.GetN()-1]:
                        maxX = sensor.plot_data[param_name].plot.GetX()[sensor.plot_data[param_name].plot.GetN()-1]
                    print sensor.plot_data[param_name].plot.GetName()
                    #print sensor.plot_data[param_name].max, sensor.plot_data[param_name].plot.GetX()[sensor.plot_data[param_name].plot.GetN()-1]
                    #if first_draw==0:
                    #    first_draw = 1
                    #    sensor.plot_data[param_name].plot.GetYaxis().SetRangeUser(0, sensor.plot_data[param_name].max*2)
                    #    sensor.plot_data[param_name].plot.GetXaxis().SetRangeUser(0, 800)
                    #    sensor.plot_data[param_name].plot.Draw("ap")
                    #else:
                    #    sensor.plot_data[param_name].plot.Draw("psame")
            print "MAX", maxY*2., maxX*1.2
            h.GetYaxis().SetRangeUser(0, maxY*1.5)
            h.GetXaxis().SetRangeUser(0, maxX*1.2)
            leg.Draw()

            canvas.Draw()
            raw_input()
            canvas.SaveAs("./user_data/{}/{}.png".format(output_name, param_name))










    # {"nick_name": "hellow world", "reg":"(HPK)(.*)(2[xX]2)(.*)(3[eE]15)(.*)(UCSC)(.*)(.?)", "style":20, "matched":False, "matched_runs":[] },
