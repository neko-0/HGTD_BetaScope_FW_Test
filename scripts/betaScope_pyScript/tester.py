# from dataCollector.core import BetaScopeResult
from plotMaker.runMatch import runMatch, runlist_from_root
from plotMaker.plotMaker import PlotMaker

data_prep = [
    {
        "nick_name": "HPK",
        "reg": "(HPK)(.*)(2[xX]2)(.*)(3[eE]15)(.?)",
        "style": 20,
        "color": 2,
        "matched_runs": [],
    },
    {
        "nick_name": "FBK",
        "reg": "(FBK)(.*)(.*)(3[eE]15)(.*)(.*)(.?)",
        "style": 21,
        "color": 2,
        "matched_runs": [],
    },
]

"""
data = BetaScopeResult()
data.load("test.pkl")
run_list = data.generate_run_list()
output = data.fetchRun(match)
"""

"""
run_list = runlist_from_root("test_run.root")
match = runMatch("(FBK)(.*)(W5)(.*)(Trig)(.?)", run_list)
for o in match:
    print(o)
"""

plot = PlotMaker("./dataCollector/test_run.root")
hi = plot.fetchRun(data_prep)
hi.makePlots()
