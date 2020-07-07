# --This script is for ploting threashold scan
#
#########
#

# --get all the files in the local directory.
from os import listdir

file_list = listdir("./")

# --open files and parsed the threshold data
output = []
for f in file_list:
    if "_Ch" in f and ".text" in f:
        data = dict()
        with open(f) as ifile:
            data["voltage"] = f.split("_")[2]
            file_content = ifile.readlines()[1:]
            threashold = []
            frequency = []

            # --parsing each line of the file.
            for item in file_content:
                raw_data = item.split(",")
                threashold.append(float(raw_data[0]))
                frequency.append(1.0 / float(raw_data[1]))
            data["threshold"] = threashold
            data["frequency"] = frequency
        output.append(data)

# --this part use ROOT's graphing library to plot the data.
import ROOT
from array import array

# --preparing graphs
for item in output:
    x = array("d", item["threshold"])
    y = array("d", item["frequency"])
    graph = ROOT.TGraph(len(x), x, y)
    item["graph"] = graph

# --ploting
graph_list = []
for color, odata in enumerate(output, start=2):
    odata["graph"].GetXaxis().SetTitle("threshold [mV]")
    odata["graph"].GetYaxis().SetTitle("frequency [Hz]")
    odata["graph"].GetYaxis().SetRangeUser(0, 1e7)
    # odata["graph"].SetLineColor(color)
    # odata["graph"].SetMarkerColor(color)
    odata["graph"].SetLineWidth(2)
    odata["graph"].SetMarkerStyle(7)
    graph_list.append([odata["graph"], int(odata["voltage"].split("V")[0])])

leg = ROOT.TLegend()


def getVoltage(input):
    return input[1]


graph_list.sort(key=getVoltage)
# print(graph_list)
# raw_input()
first = True
color = 2
for i, g in enumerate(graph_list):
    # print(g[1])
    if g[1] > 600:
        g[0].SetMarkerColor(color)
        g[0].SetLineColor(color)
        g[0].Draw("ap") if first else g[0].Draw("p")
        leg.AddEntry(g[0], "%sV" % g[1])
        first = False
        color += 1
leg.Draw()
raw_input()
