from core import BetaScopeResult
from runMatch import runMatch, runlist_from_root
from plotMaker import PlotMaker

data_prep = [
{"nick_name":"HPK_3p15" ,"reg":"(HPK)(.*)(2[xX]2)(.*)(3[eE]15)(.?)", "style":20, "color":2, "matched_runs":[] },
#{"nick_name":"FBK" ,"reg":"(FBK)(.*)(.*)(3[eE]15)(.*)(.*)(.?)", "style":21, "color":2, "matched_runs":[] },
{"nick_name":"HPK_1p15" ,"reg":"(HPK)(.*)(.*)(1[eE]15)(.?)", "style":20, "color":2, "matched_runs":[] },
]


plot = PlotMaker("test_run.root")
fetched_data = plot.fetchRun(data_prep)
fetched_data.makePlots("my_exmaple")
