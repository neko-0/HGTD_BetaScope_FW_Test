from plotMaker.runMatch import runMatch, runlist_from_root
from plotMaker.plotMaker import PlotMaker

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

# (?!abc)
plot = PlotMaker("./dataCollector/test_run.root")

"""
HPK32 = [442, 606, 596, 542, 615, 477]

data_list = []

m =20
for hpk in HPK32:
    plot.filter(str(hpk), "(.*)(Sr_Run{})(.*)".format(hpk))
    plot.prepare_plot(str(hpk), marker=m)
    m+=1

h = [str(i) for i in HPK32]
#plot.make_plots(h,"hi")

my_graph = plot.get_all()

print(my_graph)
print(my_graph['442'][0].plot_data)
"""

# hpk = plot.filter("1e15", "(.*)(HPK)(.*)(3[.p]?2)(.*)(S8664)(.*)")
#plot.filter("test", "^((?!UBM).)*$")
plot.filter("test", "(.*)(Sr_Run649)(.*)")
# hpk.filter("3e15","(.*)(3[eE]15)")

plot.prepare_plot(
    param,
    "test",
    marker=20,
    fitter={"bias_voltage_vs_charge": "0.5+expo(x)"},
    calc_at=[2.2, 5.0, 8.0, 10],
)
plot.make_plots(param, ["test"], "test", attach_fit_var=True, var_at_calc="fC")

"""
filtered_data = plot.filter("hpk", "(.*)(HPK)(.*)")


_1e15 = filtered_data.filter("1e15","(.*)(3p?2)(.*)(1[eE]15)")
_3e15 = filtered_data.filter("3e15","(.*)(3p?2)(.*)(3[eE]15)")

_1e15.show("1e15")
_3e15.show("3e15")

filtered_data.prepare_plot("1e15", marker=20)
filtered_data.prepare_plot("3e15", marker=21)

filtered_data.make_plots(["1e15","3e15"], "hello")


my_list = _1e15.get_matehced_runs("1e15")
print(my_list)
"""
