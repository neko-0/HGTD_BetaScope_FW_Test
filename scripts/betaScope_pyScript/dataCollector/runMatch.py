'''
ploting from the merged ROOT file in /user_data/
'''
import os
import re

def runlist_from_root(root_file):
    import ROOT
    tfile = ROOT.TFile.Open(root_file, "r")
    tfile.cd()
    keys = tfile.GetListOfKeys()
    iter = keys.MakeIterator()
    output = []
    while True:
        key=iter.Next()
        if key:
            output.append( [key.GetName(), key.GetTitle()] )
        else:
            break
    tfile.Close()
    return output

def runMatch(regExpress, run_list):
    matched_run = []
    for run_num,run in run_list:
        if bool(re.search(regExpress, run)):
            matched_run.append([run_num, run])
    '''
    print("Your matched run for {}".format(regExpress))
    for r,name in matched_run:
        print("{} {}".format(r,name))
    '''
    # [run numnber, run name]
    return matched_run
