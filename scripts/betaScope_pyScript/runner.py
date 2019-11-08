from core import BetaScopeResult
from core import BetaRun
from core import FitResult
from core import DAQInfo

from BetaResultReader import BetaResultReader

import os

beta_scope = BetaScopeResult()

mdir = "/media/mnt/gunter/betaAna2/"
dirlist = os.listdir(mdir)
runlist = ["614", "672", "673", "674", "675", "676", "677"]

for run in runlist:
    for fold in dirlist:
        if run in fold:
            beta_run = BetaRun(run, fold)

            fit_reader = BetaResultReader()
            fit_results = fit_reader.read_ini_result(mdir+"/"+fold+"/Results/_results.ini")
            beta_run.add_fit_result(fit_results)
            
            beta_scope.add_run(beta_run)



beta_scope.save("test.pickle")            
