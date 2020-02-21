import logging, coloredlogs

logging.basicConfig()
log = logging.getLogger(__name__)
coloredlogs.install(level="INFO", logger=log)

from .runMatch import runlist_from_root, runMatch
from copy import deepcopy
import os
import numpy
import math

try:
    from atlasplots import atlas_style

    atlas_style.SetAtlasStyle()
except:
    pass
import ROOT
from array import array


class PlotSensor(object):
    def __init__(self, sensor_name):
        self.sensor_name = sensor_name
        self.plot_data = {}

    def add_plot_data(self, name, plot_data):
        self.plot_data[name] = plot_data


class PlotData(object):
    def __init__(
        self, ttree, expression, selection, name, xtitle, ytitle, marker_style, marker_color
    ):
        self.expression = expression
        self.name = name
        self.xtitle = xtitle
        self.ytitle = ytitle
        self.marker_style = marker_style
        self.marker_color = marker_color
        self.plot = None
        self.ymax = None
        self.xmax = None

        self.npt = ttree.Draw(expression, selection, "goff")
        if self.npt > 0:
            self.raw_x = ttree.GetV1()
            self.raw_y = ttree.GetV2()
            self.np_x = numpy.ndarray(self.npt, "d", self.raw_x)
            self.np_y = numpy.ndarray(self.npt, "d", self.raw_y)
            self._x , self._y = zip(*sorted(zip(list(self.np_x), list(self.np_y))))
            self.x = numpy.array(self._x)
            self.y = numpy.array(self._y)
            self._find_max()
            self.filled = True
        else:
            self.filled = False

    def add_plot(self, plot):
        self.plot = plot

    def _find_max(self):
        ymax = 0
        xmax = 0
        i = 0
        while i<100:
            if self.raw_x[i]:
                if xmax < self.raw_x[i]:
                    xmax = self.raw_x[i]
                if ymax < self.raw_y[i]:
                    ymax = self.raw_y[i]
                i+=1
            else:
                break
        self.xmax = xmax
        self.ymax = ymax


class PlotMakerBase(object):
    def __init__(self, filename):
        self.filename = filename


class PlotMaker(PlotMakerBase):
    def __init__(self, filename, backend="root"):
        super(PlotMaker, self).__init__(filename)
        self.backend = backend
        if self.backend.lower() == "root":
            self.runlist = runlist_from_root(self.filename)
        else:
            self.runlist = []
        self.prep_plot_list = {}
        self.matched_runs = {}
        self.fit_value = {}

    def get_list(self):
        return self.runlist

    def show_sensors(self):
        for run, run_name in self.runlist:
            log.info("{} {}".format(run, run_name))
        log.info("total {}".format(len(self.runlist)))

    def show(self, name_tag):
        for run, run_name in self.matched_runs[name_tag]:
            log.info("{} {}".format(run, run_name))
        log.info("total {}".format(len(self.matched_runs[name_tag])))

    # ===========================================================================
    # ===========================================================================
    def reset_matched(self, name_tag=""):
        if name_tag:
            if name_tag in self.matched_runs:
                self.matched_runs[name_tag] = []
            else:
                pass
        else:
            self.matched_runs = {}

    # ===========================================================================
    # ===========================================================================
    def filter(self, name_tag, name_pattern, tree_tag="DUT"):
        if not name_tag in self.matched_runs:
            self.matched_runs[name_tag] = []
            runlist_for_compare = self.runlist
        else:
            runlist_for_compare = self.matched_runs[name_tag]
        matched = runMatch(name_pattern, runlist_for_compare)
        self.matched_runs[name_tag] = [(run, run_name) for run, run_name in matched if tree_tag in run]
        input(self.matched_runs[name_tag])

    # ===========================================================================
    # ===========================================================================
    @staticmethod
    def filtered(plotMaker_obj, name_tag, name_pattern):
        plotMaker_obj_copy = deepcopy(plotMaker_obj)
        if not name_tag in plotMaker_obj_copy.matched_runs:
            plotMaker_obj_copy.matched_runs[name_tag] = []
            runlist_for_compare = plotMaker_obj_copy.runlist
        else:
            runlist_for_compare = plotMaker_obj_copy.matched_runs[name_tag]
        matched = runMatch(name_pattern, runlist_for_compare)
        plotMaker_obj_copy.matched_runs[name_tag] = [
            (run, run_name) for run, run_name in matched
        ]
        return plotMaker_obj_copy

    # ===========================================================================
    # ===========================================================================
    def get_matehced_runs(self, name_tag):
        return self.matched_runs[name_tag]

    # ===========================================================================
    """
    param = {
        "bias_voltage_vs_charge": {
            "expr": "bias_voltage:new_pulse_area/4700.0",
            "xtitle": "Bias Voltage [V]",
            "ytitle": "Charge [fC]",
        },
        "bias_voltage_vs_pmax": {
            "expr": "bias_voltage:pmax",
            "xtitle": "Bias Voltage [V]",
            "ytitle": "Pmax [mV]",
        },
        "bias_voltage_vs_rise_time": {
            "expr": "bias_voltage:rise_time",
            "xtitle": "Bias Voltage [V]",
            "ytitle": "rise time [ps]",
        },
        "bias_voltage_vs_fwhm": {
            "expr": "bias_voltage:fwhm",
            "xtitle": "Bias Voltage [V]",
            "ytitle": "FWHM [ps]",
        },
        "bias_voltage_vs_res50": {
            "expr": "bias_voltage:time_resolution_50",
            "xtitle": "Bias Voltage [V]",
            "ytitle": "Time Resolution [ps]",
        },
        "charge_vs_res50": {
            "expr": "new_pulse_area/4700.0:time_resolution_50",
            "xtitle": "Charge [fC]",
            "ytitle": "Time Resolution [ps]",
        },
    }
    """
    # ===========================================================================

    def prepare_plot(self, plot_params, name_tag, marker, fitter={}, calc_at=10.0):
        if self.backend is "root":
            tfile = ROOT.TFile.Open(self.filename, "r")
        else:
            log.warning("no file is opened.")
            return None

        self.prep_plot_list[name_tag] = []
        color = 2

        # looping through the run that have been filtered and mateched
        for run, run_name in self.matched_runs[name_tag]:
            ttree = tfile.Get(run)

            # this is a container for all ploting parameters for a single run
            sensor = PlotSensor(run_name)

            # now loop through all the ploting parameters in plot_params and create plot
            for par_name, par in plot_params.items():

                # ---------------------------------------------------------------
                # this block of codes is for fitting.
                if par_name in fitter:
                    fitFunc = self.make_fit(par_name, fitter[par_name])
                    if not name_tag in self.fit_value:
                        self.fit_value[name_tag] = {}
                        self.fit_value[name_tag][par_name] = []
                        if not isinstance(calc_at, list):
                            self.fit_value[name_tag][par_name].append(([], calc_at))
                        else:
                            for calc in calc_at:
                                self.fit_value[name_tag][par_name].append(([], calc))
                    else:
                        if not par_name in self.fit_value[name_tag]:
                            self.fit_value[name_tag][par_name] = []
                            self.fit_value[name_tag][par_name].append(([], calc_at))
                else:
                    fitFunc = None
                # ---------------------------------------------------------------
                if "selection" in par:
                    selection = par["selection"]
                else:
                    selection = ""

                plotdata = PlotData(
                    ttree,
                    par["expr"],
                    selection,
                    par_name,
                    par["xtitle"],
                    par["ytitle"],
                    marker,
                    color,
                )
                if not plotdata.filled:
                    continue

                g = ROOT.TGraph(plotdata.npt, plotdata.x, plotdata.y)

                if not(fitFunc is None):
                    fit = fitFunc(0, plotdata.xmax)
                    fit.SetLineColor(color)
                    g.Fit(fit)
                    for fit_calc in self.fit_value[name_tag][par_name]:
                        fit_calc[0].append(fit.GetX(fit_calc[1]))

                g.GetXaxis().SetTitle(par["xtitle"])
                g.GetYaxis().SetTitle(par["ytitle"])
                g.SetTitle("")
                g.SetMarkerColor(color)
                g.SetMarkerStyle(marker)
                g.SetLineColor(color)
                g.SetLineStyle(2)
                g.SetLineWidth(5)
                g.SetName(run)
                plotdata.add_plot(g)
                sensor.add_plot_data(par_name, plotdata)
            self.prep_plot_list[name_tag].append(sensor)
            color += 1

    def make_plots(
        self, plot_params, name_tag_list, output_dir, output_name="", attach_fit_var=True, var_at_calc=""
    ):
        ROOT.gROOT.SetBatch(True)
        if output_name:
            try:
                os.makedirs("{odir}/{oname}".format(odir=output_dir, oname=output_name))
            except:
                pass
        for entry, param_name in enumerate(plot_params.keys()):
            leg = ROOT.TLegend(0.15, 0.65, 0.70, 0.85)
            leg.SetTextSize(0.02)
            canvas = ROOT.TCanvas("canvas", "canvas", 1800, 1200)
            canvas.cd()
            first_draw = 0
            for name_tag in name_tag_list:
                for sensor in self.prep_plot_list[name_tag]:
                    if not param_name in sensor.plot_data:
                        continue
                    if first_draw == 0:
                        first_draw = 1
                        sensor.plot_data[param_name].plot.GetYaxis().SetRangeUser(
                            0, sensor.plot_data[param_name].ymax * 3
                        )
                        sensor.plot_data[param_name].plot.GetXaxis().SetRangeUser(
                            0, sensor.plot_data[param_name].xmax * 1.2
                        )
                        sensor.plot_data[param_name].plot.GetXaxis().SetLimits(
                            0, sensor.plot_data[param_name].xmax * 1.2
                        )
                        sensor.plot_data[param_name].plot.Draw("apl")
                    else:
                        sensor.plot_data[param_name].plot.GetYaxis().SetRangeUser(
                            0, sensor.plot_data[param_name].ymax * 3
                        )
                        sensor.plot_data[param_name].plot.GetXaxis().SetRangeUser(
                            0, sensor.plot_data[param_name].xmax * 1.2
                        )
                        sensor.plot_data[param_name].plot.GetXaxis().SetLimits(
                            0, sensor.plot_data[param_name].xmax * 1.2
                        )
                        sensor.plot_data[param_name].plot.Draw("plsame")
                    leg.AddEntry(
                        sensor.plot_data[param_name].plot,
                        sensor.sensor_name.split("_Trig")[0].replace("_", " "),
                    )

                if attach_fit_var:
                    if not param_name in self.fit_value[name_tag]:
                        pass
                    else:
                        if self.fit_value[name_tag][param_name][0]:
                            leg2 = ROOT.TLegend(0.17, 0.3, 0.6, 0.4)
                            leg2.SetBorderSize(0)
                            leg2.SetFillColorAlpha(0, 0.5)
                            for fit_calc in self.fit_value[name_tag][param_name]:
                                fitted_value = [
                                    value
                                    for value in fit_calc[0]
                                    if not math.isnan(value)
                                ]
                                fitted_value_at = fit_calc[1]
                                # log.info(fitted_value)
                                # gerror = ROOT.TGraphErrors(len(fitted_value))
                                # gerror.SetPoint(i, numpy.mean(fitted_value), fitted_value_at) for i in len(fitted_value)
                                # gerror.SetPointError(i, numpy.std(fitted_value), 0)
                                leg2.AddEntry(
                                    0,
                                    "V for %.1f %s: %.1f #pm %.1f, Variation: %.1f%%"
                                    % (
                                        fitted_value_at,
                                        var_at_calc,
                                        numpy.mean(fitted_value),
                                        numpy.std(fitted_value),
                                        100
                                        * numpy.std(fitted_value)
                                        / numpy.mean(fitted_value),
                                    ),
                                    "",
                                )
                                # gerror.Draw("sameP>")
                            leg2.Draw()
            leg.Draw()
            # raw_input()
            canvas.Update()
            canvas.SaveAs("{}/{}/{}.png".format(output_dir, output_name, param_name))

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
                    for par_name, par in param.items():
                        plotdata = PlotData(
                            par_name,
                            par["xtitle"],
                            par["ytitle"],
                            run_info["style"],
                            run_info["color"],
                        )
                        if "selection" in par:
                            selection = par["selection"]
                        else:
                            selection = ""
                        n = ttree.Draw(par["expr"], selection, "goff")
                        if n <= 0:
                            continue
                        d = ttree.GetV2()
                        dd = ttree.GetV1()
                        v2 = numpy.ndarray(n, "d", d)
                        v1 = numpy.ndarray(n, "d", dd)
                        max = 0
                        xmax = 0
                        i = 0
                        while i < 100:
                            if d[i]:
                                if max < d[i]:
                                    max = d[i]
                                if xmax < dd[i]:
                                    xmax = dd[i]
                                i += 1
                            else:
                                break
                        g = ROOT.TGraph(n, v1, v2)
                        g.SetTitle("")
                        g.GetXaxis().SetTitle(par["xtitle"])
                        g.GetYaxis().SetTitle(par["ytitle"])
                        g.SetMarkerColor(color)
                        g.SetMarkerStyle(run_info["style"])
                        g.SetLineColor(color)
                        g.SetLineStyle(2)
                        g.SetLineColor(color)
                        plotdata.add_plot(g)
                        plotdata.max = max
                        plotdata.xmax = xmax
                        sensor.add_plot_data(par_name, plotdata)
                    color += 1
        copy_self.runlist = matched
        return copy_self

    def makePlots(self, output_name=""):
        ROOT.gROOT.SetBatch(True)
        if output_name:
            try:
                os.makedirs("./user_data/{}".format(output_name))
            except:
                pass
        for entry, param_name in enumerate(param.keys()):
            leg = ROOT.TLegend(0.15, 0.45, 0.35, 0.65)
            leg.SetTextSize(0.02)
            canvas = ROOT.TCanvas("canvas", "canvas", 1800, 1200)
            canvas.cd()
            first_draw = 0
            max_buffer = 0.0
            xmax_buffer = 0.0
            for nick, sensors in self.sensor_list.items():
                for sensor in sensors:
                    if max_buffer < sensor.plot_data[param_name].max:
                        max_buffer = sensor.plot_data[param_name].max
                    if xmax_buffer < sensor.plot_data[param_name].xmax:
                        xmax_buffer = sensor.plot_data[param_name].xmax
                    if not param_name in sensor.plot_data:
                        continue
                    if first_draw == 0:
                        first_draw = 1
                        sensor.plot_data[param_name].plot.GetYaxis().SetRangeUser(
                            0, sensor.plot_data[param_name].max * 3
                        )
                        sensor.plot_data[param_name].plot.GetXaxis().SetRangeUser(
                            0, sensor.plot_data[param_name].xmax * 3
                        )
                        sensor.plot_data[param_name].plot.Draw("ap")
                        leg.AddEntry(
                            sensor.plot_data[param_name].plot,
                            sensor.sensor_name.split("_Trig")[0],
                        )
                    else:
                        sensor.plot_data[param_name].plot.GetYaxis().SetRangeUser(
                            0, sensor.plot_data[param_name].max * 3
                        )
                        sensor.plot_data[param_name].plot.GetXaxis().SetRangeUser(
                            0, sensor.plot_data[param_name].xmax * 3
                        )
                        sensor.plot_data[param_name].plot.Draw("psame")
                        leg.AddEntry(
                            sensor.plot_data[param_name].plot,
                            sensor.sensor_name.split("_Trig")[0],
                        )
            leg.Draw()
            canvas.SaveAs("./user_data/{}/{}.png".format(output_name, param_name))

    def make_fit(self, name, expr):
        def fit(xmin, xmax):
            # "0.5+expo(x)"
            f = ROOT.TF1("fitCC" + name, expr, xmin, 1.2 * xmax)
            return f

        return fit

    # {"nick_name": "hellow world", "reg":"(HPK)(.*)(2[xX]2)(.*)(3[eE]15)(.*)(UCSC)(.*)(.?)", "style":20, "matched":False, "matched_runs":[] },
