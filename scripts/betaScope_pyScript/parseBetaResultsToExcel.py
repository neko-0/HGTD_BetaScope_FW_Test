from openpyxl import Workbook, load_workbook
import configparser
import os

par_list = [
"runNumber",
"SensorName",
"Temp",
"Bias",
"Resistance",
"pulseArea",
"pulseArea_Error",
"Pmax",
"Pmax_Error",
"RMS",
"RMS_Error",
"Rise_Time",
"Rise_Time_Error",
"dvdt",
"dvdt_Error",
"FWHM",
"FWHM_Error",
"NewPulseArea",
"NewPulseArea_Error",
"FallTime",
"FallTime_Error"
]

par_dict = {
"runNumber" : "A",
"SensorName" : "B",
"Temp" : "G",
"Bias" : "H",
"Resistance" : "L",
"pulseArea" : "J",
"pulseArea_Error" : "K",
"Pmax" : "R",
"Pmax_Error" : "S",
"RMS" : "T",
"RMS_Error" : "U",
"Rise_Time" : "Z",
"Rise_Time_Error" : "AA",
"dvdt" : "AB",
"dvdt_Error" : "AC",
"FWHM" : "AL",
"FWHM_Error" : "AM",
"NewPulseArea" : "CA",
"NewPulseArea_Error" : "CB",
"FallTime" : "DG",
"FallTime_Error" : "DH",
"cycle" : "F"
}

def mergeExcel(fname="_results.xlxs"):
    no_merge_file = False
    if os.path.exists("/tmp/merged_beta_results.xlxs"):
        src_wb = load_workbook("/tmp/merged_beta_results.xlsx")
    else:
        no_merge_file = True
        src_wb = Workbook()
        src_wb.create_sheet("DUT")
        scr_wb.create_sheet("TRIG")

    input_wb = load_workbook(fname)

    sheets = ["DUT", "TRIG"]
    for sheet in sheets:
        rowCounter = 2
        src_ws = scr_wb[sheet]
        input_ws = input_wb[sheet]
        max_row = src_ws.max_row
        for par in par_list:
            input_cell = par_dict[par] + str(rowCounter-1)
            src_cell = par_dict[par] + str(max_row+rowCounter)
            src_ws[src_cell] = input_ws[input_cell]

    if no_merge_file:
        src_wb.save("/tmp/merged_beta_results.xlsx")



def parseINIToExcel(fname="_results.ini"):
    config = configparser.ConfigParser()
    config.read(fname)
    config_section = config.sections()
    print(config_section)

    description_file = None
    for descr in os.listdir("./"):
        if "_Description.ini" in descr:
            description_file = configparser.ConfigParser()
            description_file.read(descr)
            print("found DAQ description file")
            break

    wb = Workbook()
    wb.create_sheet("DUT")
    wb.create_sheet("TRIG")
    #ws = wb.active

    rowCounter = 1
    dut_trig = ["DUT", "Trig"]

    RunNum = 100
    SensorName = "Hi"
    Temp = 20
    trigBias = 395

    if description_file:
        try:
            RunNum = description_file["Run_Description"]["Run_Number"]

            SensorName = description_file["Run_Description"]["DUT_Senor_Name"]
            SensorName += "-Fluence "+description_file["Run_Description"]["DUT_Fluence_Type"]+"-"+description_file["Run_Description"]["DUT_Fluence"]
            SensorName += "--"+description_file["Run_Description"]["DUT_Readout_Board"]+"-"+description_file["Run_Description"]["DUT_Readout_Board_Number"]

            Temp = description_file["Run_Description"]["Temperature"]
            trigBias = description_file["Run_Description"]["Trigger_Voltage"]
        except Exception as e:
            print(Exception)
            RunNum = 100
            SensorName = "Hi"
            Temp = 20
            trigBias = 395

    Resistance = 4700

    for ch in dut_trig:
        rowCounter = 1
        for bias in config_section:
            myRunNum = str(RunNum)+"->"+str(rowCounter)
            if ch in bias:
                if ch != "Trig":
                    ws = wb["DUT"]
                    if ".." in bias:
                        Bias = bias[bias.find("_")+1:bias.find("V")]
                        cycle = bias.split("..")[1]
                        if "_" in cycle:
                            cycle = cycle.split("_")[0]
                    else:
                        Bias = bias[bias.find("_")+1:bias.find("V")]
                        cycle = 1
                else:
                    #Bias = trigBias
                    ws = wb["TRIG"]
                    SensorName = description_file["Run_Description"]["Trigger_Sensor_Name"]
                    try:
                        Bias = config[bias]["trigger_bias"]
                        if ".." in bias:
                            cycle = bias.split("..")[1]
                            if "_" in cycle:
                                cycle = cycle.split("_")[0]
                        else:
                            cycle = 1
                    except:
                        Bias = -390
                        cycle = 1
                for par in par_list:
                    if (par == "SensorName"):
                        cell = par_dict[par] + str(rowCounter)
                        ws[cell] = SensorName
                    elif(par=="runNumber"):
                        cell = par_dict[par] + str(rowCounter)
                        ws[cell] = myRunNum
                    elif (par == "Temp"):
                        try:
                            Temp = config[bias]["temperature"]
                        except:
                            Temp = "-30"
                        cell = par_dict[par] + str(rowCounter)
                        ws[cell] = float(Temp)
                    elif (par == "Bias"):
                        cell = par_dict[par] + str(rowCounter)
                        ws[cell] = float(Bias)
                        if cycle:
                            cell = par_dict["cycle"] + str(rowCounter)
                            ws[cell] = int(cycle)
                    elif (par == "Resistance"):
                        cell = par_dict[par] + str(rowCounter)
                        ws[cell] = float(Resistance)
                    else:
                        cell = par_dict[par] + str(rowCounter)
                        ws[cell] = float(config[bias][par])
                rowCounter+=1
        rowCounter+=1

    wb.save("_results.xlsx")

    mergeExcel(fname)


parseINIToExcel()
