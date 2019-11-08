'''
Class for reading fit results (.ini) from beta scope fit process.
'''

import configparser

from core import FitResult

class BetaResultReader(object):

    def __init__(self, run_number):
        pass

    def read_ini_result(self, fname):
        fit_result_file = configparse.ConfigParser()
        fit_result_file.read(fname)
        fit_result_file_section = fit_result_file.sections()

        dut_trig = ["DUT", "Trig"]

        fit_results = []
        for ch in dut_trig:
            for sec in fit_result_file_section:
                fit_result = FitResult(ch)
                if ch in sec:
                    if ch != "Trig":
                        if ".." in sec:
                            fit_result.bias = sec[sec.find("_")+1:sec.find("V")]
                            fit_result.cycle = sec.split("..")[1]
                            if "_" in fit_result.cycle:
                                fit_result.cycle = fit_result.cycle.split("_")[0]
                        else:
                            fit_result.bias = sec[sec.find("_")+1:sec.find("V")]
                            fit_result.cycle = 1
                    else:
                        try:
                            Bias = fit_result_file[sec]["trigger_bias"]
                            if ".." in sec:
                                fit_result.cycle = sec.split("..")[1]
                                if "_" in fit_result.cycle:
                                    fit_result.cycle = fit_result.cycle.split("_")[0]
                            else:
                                fit_result.cycle = 1
                        except:
                            fit_result.bias = -390
                            fit_result.cycle = 1

                    fit_result.pulase_area = float(fit_result[sec]["pulseArea"])
                    fit_result.pmax = float(fit_result[sec]["Pmax"])
                    fit_result.rms = float(fit_result[sec]["RMS"])
                    fit_result.rise_time = float(fit_result[sec]["Rise_Time"])
                    fit_result.dvdt = float(fit_result[sec]["dvdt"])
                    fit_result.fwhm = float(fit_result[sec]["FWHM"])
                    fit_result.new_pulse_area = float(fit_result[sec]["NewPulseArea"])
                    fit_result.fall_time = float(fit_result[sec]["FallTime"])
                fit_results.append(fit_result)
        return fit_results
