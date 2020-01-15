from plotMaker.runMatch import runMatch, runlist_from_root
from plotMaker.plotMaker import PlotMaker

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
hpk = plot.filter("1e15", "(.*)(FBK)(.*)(S8664)(.*)")
# hpk.filter("1e15","(.*)(1[eE]15)(.*)")
hpk.filter("1e15", "^((?!Sr_Run603).)*$")
# hpk.filter("1e15","^((?!FBK).)*$")
hpk.filter("1e15", "^((?!CNM).)*$")
hpk.filter("1e15", "^((?!G35).)*$")
hpk.filter("1e15", "^((?!F35).)*$")
# hpk.filter("1e15","^((?![Cc]yric).)*$")
# hpk.filter("1e15","^((?![Pp]roton).)*$")
hpk.filter("1e15", "(.*)([Cc][yY][Rr][iI][cC])(.*)")
hpk.filter("1e15", "^((?![Pp][iI][nN]).)*$")
hpk.filter("1e15", "^((?!UBM).)*$")
# hpk.filter("3e15","(.*)(3[eE]15)")

hpk.prepare_plot(
    "1e15",
    marker=20,
    fitter={"bias_voltage_vs_charge": "0.5+expo(x)"},
    calc_at=[2.2, 5.0, 8.0, 10],
)
hpk.make_plots(["1e15"], "fbk_cyric_1e15", attach_fit_var=True, var_at_calc="fC")

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
