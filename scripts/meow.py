'''
A helper script for doing beta-scope analysis
'''

import os
import subprocess

__raw_data_dir = "/media/mnt/BigHD/Beta_DAQ_Data/"
__yuzhan_dir = "/media/mnt/BigHD/BetaScope_Data/Analyzed_YZ/"

__data_output_dir = ""
__runNum = ""
__num = ""

def set_run( runNum ):
    global __runNum
    global __num
    global __raw_data_dir
    global __data_output_dir
    global __yuzhan_dir

    os.chdir(__yuzhan_dir)

    for d in os.listdir(__raw_data_dir):
        if "Sr_Run"+str(runNum) in d:
            __runNum = d

    if __data_output_dir:
        os.chdir(__data_output_dir)
        try:
            os.mkdir(__runNum)
        except:
            os.chdir(__data_output_dir + "/" + __runNum + "/")
        os.chdir(__data_output_dir + "/" + __runNum + "/")
        __num = runNum

    print("Your working dir: {dir}".format(dir=os.getcwd()))


def set_output_dir( folder ):
    global __data_output_dir
    __data_output_dir = folder
    os.chdir(__data_output_dir)

def generate_config():
    subprocess.call("${BETASCOPE_SCRIPTS}/../BetaScope_Ana/BetaScopeWaveformAna/bin/GenerateWaveformConfig", shell=True)

def set_default_config():
    import configparser

    global __raw_data_dir
    global __data_output_dir
    global __runNum

    parser = configparser.ConfigParser()
    parser.optionxform = str
    parser.read("WaveformAnaConfig.ini")
    parser.set("General","rawFilesDir", "{raw_dir}/{raw_data}/fromDAQ/".format(raw_dir=__raw_data_dir, raw_data=__runNum) )
    parser.set("Channel_Activation", "channel_2", "1")
    parser.set("Channel_Activation", "channel_3", "1")
    parser.set("Channel_Invertion", "channel_2", "1")
    with open("WaveformAnaConfig.ini", "wb") as config:
        parser.write(config)

def set_active_channel( ch):
    import configparser
    parser = configparser.ConfigParser()
    parser.set("Channel_Activation", "channel_{ch}".format(ch=ch), 1)
    with open("WaveformAnaConfig.ini", "wb") as config:
        parser.write(config)


def run_analysis():
    global __num

    p = subprocess.Popen("${BETASCOPE_SCRIPTS}/../BetaScope_Ana/BetaScopeWaveformAna/bin/Run_WaveformAna WaveformAnaConfig.ini --skipWaveform", shell=True)
    p.wait()

    p = subprocess.Popen("/home/yuzhan/HGTD_BetaScope/BetaScopeDataProcessor/bin/GenerateDataProcessorConfig.exe", shell=True)
    p.wait()

    p = subprocess.call("python2 $BETASCOPE_SCRIPTS/betaScope_pyScript/autoCut_v2.py --runNum {num}".format(num=__num), shell=True)

    p = subprocess.Popen("/home/yuzhan/HGTD_BetaScope/BetaScopeDataProcessor/bin/GetResults.exe run_info_v08022018.ini", shell=True)
    p.wait()
