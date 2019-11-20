from core import BetaScopeResult
from core import BetaRun
from core import FitResult
from core import DAQInfo

from BetaResultReader import BetaResultReader

import os
import subprocess

pwd = os.getcwd()

beta_scope = BetaScopeResult()

mdir_list = [
"/media/mnt/gunter/betaAna3/",
"/media/mnt/gunter/betaAna2/",
"/media/mnt/BigHD/BetaScope_Data/Analyzed_YZ/"
]

dirlist = [[x for x in os.listdir(mdir_list[i])] for i in range(len(mdir_list)) ]

runlist = []
for i in range(100,700):
    runlist.append("Sr_Run{}".format(i) )

pList = []
for mdir in mdir_list:
    dirlist = os.listdir(mdir)
    for run in runlist:
        for fold in dirlist:
            if run in fold:
                os.chdir(mdir+"/"+fold)
                inner_dir = os.listdir("./")
                if True:#not "Results" in inner_dir:
                    pass
                    #p = subprocess.call("python2 $BETASCOPE_SCRIPTS/betaScope_pyScript/get_time_res.py --CFD 50", shell=True)
                    #p = subprocess.call("python2 $BETASCOPE_SCRIPTS/betaScope_pyScript/get_time_res.py --CFD 20", shell=True)
                    #p = subprocess.call("python2 $BETASCOPE_SCRIPTS/betaScope_pyScript/read_current.py", shell=True)
                    #p = subprocess.Popen("${BETASCOPE_SCRIPTS}/../BetaScope_Ana/BetaScopeWaveformAna/bin/Run_WaveformAna WaveformAnaConfig.ini --skipWaveform", shell=True)
                    #p = subprocess.Popen("$BETASCOPE_SCRIPTS/betaScopePlot//bin/getResults run_info_v08022018.ini", shell=True)
                '''
                pList.append(p)
                if len(pList)==12:
                    for pp in pList:
                        pp.wait()
                    pList  = []
                '''
                os.chdir(pwd)


                beta_run = BetaRun(run, fold)

                fit_reader = BetaResultReader()
                fit_results = fit_reader.read_ini_result(mdir+"/"+fold+"/Results/_results.ini")
                for fit in fit_results:
                    fit.update_time_resolution(mdir+"/"+fold+"/res50.txt", 50)
                    fit.update_time_resolution(mdir+"/"+fold+"/res20.txt", 20)
                    fit.update_leakage(mdir+"/"+fold+"/leakage.txt")
                beta_run.add_fit_result(fit_results)

                try:
                    daq_info = DAQInfo(mdir+"/"+fold+"Description.ini")
                except:
                    daq_info = None

                beta_run.update_daq_info(daq_info)

                beta_scope.add_run(beta_run)



beta_scope.save("test.pkl")
beta_scope.to_root("test_run.root")
