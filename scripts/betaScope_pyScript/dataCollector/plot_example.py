from core import BetaScopeResult
from runMatch import runMatch, runlist_from_root
from plotMaker import PlotMaker

data_prep = [
{"nick_name":"HPK" ,"reg":"^((?!FBK).)*$", "style":20, "color":2, "matched_runs":[] },
]

# (?!abc)
plot = PlotMaker("test_run.root")

filtered_data = plot.filter("hpk", "(.*)(HPK)(.*)")
filtered_data.filter("hpk", "^((?!2[xX]2).)*$")
filtered_data.filter("hpk", "^((?!FBK).)*$")
filtered_data.filter("hpk", "(.*)(8664)(.*)")

_1e15 = filtered_data.filter("1e15","(.*)(3p?2)(.*)(1[eE]15)")
_3e15 = filtered_data.filter("3e15","(.*)(3p?2)(.*)(3[eE]15)")

_1e15.show("1e15")
raw_input()
_3e15.show("3e15")

'''

fetched_data = fetched_data.fetchRun(data_prep)
fetched_data.show()

raw_input()

data_prep = [
{"nick_name":"HPK" ,"reg":"^((?!CNM).)*$", "style":20, "color":2, "matched_runs":[] },
]

fetched_data = fetched_data.fetchRun(data_prep)
fetched_data.show_sensors()
raw_input()


data_prep = [
{"nick_name":"HPK" ,"reg":"(.*)(3[eE]15)(.*)(S8664)(.*)", "style":20, "color":2, "matched_runs":[] },
]

fetched_data = fetched_data.fetchRun(data_prep)

fetched_data.show_sensors()
raw_input()


fetched_data.makePlots("my_exmaple")
'''
