import ROOT
from array import array

tfiles = []
tfiles.append( ROOT.TFile.Open("/media/mnt/gunter/betaAna2/Sr_Run765_HPK_Sample_H_Chip2_NEU_2E14neq_Trig_HPK2_S8664_chip15_neg30C_with_newChamber/raw_results.root") )
tfiles.append( ROOT.TFile.Open("/media/mnt/gunter/betaAna2/Sr_Run766_HPK_Sample_H_Chip6_NEU_4E14neq_Trig_HPK2_S8664_chip15_neg30C_with_newChamber/raw_results.root") )
#tfiles.append( ROOT.TFile.Open("/media/mnt/gunter/betaAna2/Sr_Run766_HPK_Sample_H_Chip6_NEU_4E14neq_Trig_HPK2_S8664_chip15_neg30C_with_newChamber/raw_results.root") )

x = array("d")
y = array("d")

for f in tfiles:
    f.raw.SetMarkerStyle(7)

tfiles[0].raw.SetMarkerColor(2)
tfiles[1].raw.SetMarkerColor(4)

leg = ROOT.TLegend()
leg.AddEntry(tfiles[0].raw, "2e14")
leg.AddEntry(tfiles[1].raw, "4e14")

par = "rise2:bias"
cut = "cycle==1"
xmin = 0
xmax = 400
ymin = 0
ymax = 100


tfiles[1].raw.Draw(par, cut)
tfiles[0].raw.Draw(par, cut,  "same")


input()
