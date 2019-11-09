from core import BetaScopeResult
from core import BetaRun
from core import FitResult
from core import DAQInfo

from BetaResultReader import BetaResultReader

import os

beta_scope = BetaScopeResult("test.pickle")

mdir = "/media/mnt/gunter/betaAna2/"
dirlist = os.listdir(mdir)

runlist = []
for i in range(500,700):
    runlist.append("Sr_Run{}".format(i) )

for run in runlist:
    for fold in dirlist:
        if run in fold:
            beta_run = BetaRun(run, fold)

            fit_reader = BetaResultReader()
            fit_results = fit_reader.read_ini_result(mdir+"/"+fold+"/Results/_results.ini")
            for fit in fit_results:
                fit.update_time_resolution(mdir+"/"+fold+"Res50.txt", 50)
                fit.update_time_resolution(mdir+"/"+fold+"Res20.txt", 20)
            beta_run.add_fit_result(fit_results)

            try:
                daq_info = DAQInfo(mdir+"/"+fold+"Description.ini")
            except:
                daq_info = None

            beta_run.update_daq_info(daq_info)

            beta_scope.add_run(beta_run)


beta_scope.save("test.pickle")
