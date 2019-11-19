from core import BetaScopeResult
from runMatch import runMatch, runlist_from_root
from plotMaker import PlotMaker

data_prep = [
{"nick_name":"HPK" ,"reg":"^((?!FBK).)*$", "style":20, "color":2, "matched_runs":[] },
]

# (?!abc)
plot = PlotMaker("test_run.root")

fetched_data = plot.fetchRun(data_prep)

fetched_data.show_sensors()
raw_input()

data_prep = [
{"nick_name":"HPK" ,"reg":"^((?!2[xX]2).)*$", "style":20, "color":2, "matched_runs":[] },
]

fetched_data = fetched_data.fetchRun(data_prep)
fetched_data.show_sensors()
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
