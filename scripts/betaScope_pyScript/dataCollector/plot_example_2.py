from core import BetaScopeResult
from runMatch import runMatch, runlist_from_root
from plotMaker import PlotMaker

data_prep = [
{"nick_name":"NDL" ,"reg":"(*.)(NDL)(.*)(S8664)(.*)", "style":20, "color":2, "matched_runs":[] },
]

# (?!abc)
plot = PlotMaker("test_run.root")

import TDR_runs
runs = [TDR_runs.HPK32]

for run in runs:
    fetched_data = plot.fetchRunNumbers(run)
    print fetched_data.runlist
    fetched_data.makePlots("HPK32")
