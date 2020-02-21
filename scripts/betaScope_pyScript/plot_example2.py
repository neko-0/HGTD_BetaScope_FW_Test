from plotMaker.runMatch import runMatch, runlist_from_root
from plotMaker.plotMaker import PlotMaker
from plotMaker.param import param


# (?!abc)
plot = PlotMaker("~/test_run.root")

plot.filter("Irra_2020", "(.*)(Sr_Run7(1[8-9]|2[0-9]|3[0-9]))(.*)")
plot.filter("Irra_2020", "(.*)([Cc][Yy][rR])(.*)(2p5)(.*)")
#plot.filter("Irra_2020", "(.*)(!FBK)(.*)")

plot.prepare_plot(
    param,
    "Irra_2020",
    marker=20,
    #fitter={"bias_voltage_vs_charge"}, #: "0.5+expo(x)"},
    #calc_at=[2.2, 5.0, 8.0, 10],
)
plot.make_plots(param, ["Irra_2020"], "/home/white_meow/beta_plot", "Irra_2020_pro", attach_fit_var=False, var_at_calc="fC")
