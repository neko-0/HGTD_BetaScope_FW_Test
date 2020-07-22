import ROOT

fil = ROOT.TFile.Open("test_run.root", "UPDATE")
keyList = fil.GetListOfKeys()

print "Cleaning add2 and add3"
for key in keyList:
    if "add2" in key.GetName():
        #print key
        new_name = key.GetName()
        new_name = new_name.split("dd2_")[1]
        #print new_name    tree.Scan()

        key.SetName(new_name)
    if "add3" in key.GetName():
        #print key
        new_name = key.GetName()
        new_name = new_name.split("dd3_")[1]
        #print new_name
        key.SetName(new_name)

fil.Close()
