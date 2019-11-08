from core import BetaScopeResult
from core import BetaRun
from core import FitResult
from core import DAQInfo

from BetaResultReader import BetaResultReader

import os

beta_scope = BetaScopeResult()

dirlist = os.listdir(".")
runlist = ["614", "672", "673", "674", "675", "676", "677"]

for run in runlist:
    for fold in dirlist:
        if run in fold:
            beta_run = BetaRun(run, fold)

            fit_reader = BetaResultReader()
            fit_result = fit_reader.read_ini_result(fold + "/Results/_results.ini")

            beta_run.add_fit_result(fit_result)



beta_scope.save("test.pickle")            
