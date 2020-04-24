from core import BetaResult, BetaCollector

import os


pwd = os.getcwd()

mdir_list = [
    "/media/mnt/BigHD/BetaScope_Data/Analyzed_YZ/",
    "/media/mnt/gunter/betaAna3/",
    "/media/mnt/gunter/betaAna2/",
]

dirlist = [[x for x in os.listdir(mdir_list[i])] for i in range(len(mdir_list))]

runlist = []
for i in range(1, 1000):
    runlist.append("Sr_Run{}_".format(i))

beta_scope = BetaCollector()

pList = []
for mdir in mdir_list:
    dirlist = os.listdir(mdir)
    for run in runlist:
        for fold in dirlist:
            if run in fold:
                os.chdir(mdir + "/" + fold)
                # import subprocess
                # p = subprocess.call("python2 $BETASCOPE_SCRIPTS/betaScope_pyScript/get_time_res.py --CFD 50", shell=True)
                # p = subprocess.call("python2 $BETASCOPE_SCRIPTS/betaScope_pyScript/get_time_res.py --CFD 20", shell=True)
                # p = subprocess.call("python2 $BETASCOPE_SCRIPTS/betaScope_pyScript/read_current.py", shell=True)
                # p = subprocess.Popen("$BETASCOPE_SCRIPTS/betaScopePlot//bin/getResults run_info_v08022018.ini", shell=True)
                """
                pList.append(p)
                if len(pList)==12:
                    for pp in pList:
                        pp.wait()
                    pList  = []
                """
                os.chdir(pwd)

                beta_run = BetaResult(run, fold)
                fit_result_file = "{master_dir}/{dir}/Results/_results.ini".format(
                    master_dir=mdir, dir=fold
                )
                time_res_file = [
                    (mdir + "/" + fold + "/res50.txt", 50),
                    (mdir + "/" + fold + "/res20.txt", 20),
                ]
                leakage_file = mdir + "/" + fold + "/leakage.txt"
                beta_run.load_result(
                    fit_result_file, time_res_file, leakage_file, "DUT"
                )
                beta_run.load_result(
                    fit_result_file, time_res_file, leakage_file, "Trig"
                )
                beta_run.load_daq_info(run, mdir + "/" + fold + "/Description.ini")

                beta_scope.add_run(beta_run)


beta_scope.save("test.pkl")
beta_scope.to_root("test_run.root")
