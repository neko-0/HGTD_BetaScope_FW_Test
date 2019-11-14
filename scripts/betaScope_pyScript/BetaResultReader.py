'''
Class for reading fit results (.ini) from beta scope fit process.
'''

import configparser

from core import FitResult

class BetaResultReader(object):

    def __init__(self):
        pass

    def read_ini_result(self, fname):
        fit_result_file = configparser.ConfigParser()
        try:
            fit_result_file.read(fname)
            fit_result_file_section = fit_result_file.sections()
        except:
            return []

        dut_trig = ["DUT"]

        fit_results = []
        for ch in dut_trig:
            for sec in fit_result_file_section:
                fit_result = FitResult(ch)
                if ch in sec:
                    if ch != "Trig":
                        if ".." in sec:
                            fit_result.bias_voltage = sec[sec.find("_")+1:sec.find("V")]
                            fit_result.cycle = sec.split("..")[1]
                            if "_" in fit_result.cycle:
                                fit_result.cycle = int(fit_result.cycle.split("_")[0])
                        else:
                            fit_result.bias_voltage = sec[sec.find("_")+1:sec.find("V")]
                            fit_result.cycle = 1
                    else:
                        try:
                            fit_result.bias_voltage = fit_result_file[sec]["trigger_bias"]
                            if ".." in sec:
                                fit_result.cycle = sec.split("..")[1]
                                if "_" in fit_result.cycle:
                                    fit_result.cycle = int(fit_result.cycle.split("_")[0])
                            else:
                                fit_result.cycle = 1
                        except:
                            fit_result.bias_voltage = -390
                            fit_result.cycle = 1

                    fit_result.pulse_area = float(fit_result_file[sec]["PulseArea"])
                    fit_result.pulse_area_chi = float(fit_result_file[sec]["PulseArea_CHI_NDF"])
                    fit_result.pmax = float(fit_result_file[sec]["Pmax"])
                    fit_result.pmax_chi = float(fit_result_file[sec]["Pmax_CHI_NDF"])
                    fit_result.rms = float(fit_result_file[sec]["RMS"])
                    fit_result.rms_chi = float(fit_result_file[sec]["RMS_CHI_NDF"])
                    fit_result.rise_time = float(fit_result_file[sec]["Rise_Time"])
                    fit_result.rise_time_chi = float(fit_result_file[sec]["Rise_Time_CHI_NDF"])
                    fit_result.dvdt = float(fit_result_file[sec]["dvdt"])
                    fit_result.dvdt_chi = float(fit_result_file[sec]["dvdt_CHI_NDF"])
                    fit_result.fwhm = float(fit_result_file[sec]["FWHM"])
                    fit_result.fwhm_chi = float(fit_result_file[sec]["FWHM_CHI_NDF"])
                    fit_result.new_pulse_area = float(fit_result_file[sec]["NewPulseArea"])
                    fit_result.new_pulse_area_chi = float(fit_result_file[sec]["NewPulseArea_CHI_NDF"])
                    fit_result.fall_time = float(fit_result_file[sec]["FallTime"])
                    fit_result.fall_time_chi = float(fit_result_file[sec]["FallTime_CHI_NDF"])
                    fit_results.append(fit_result)
        return fit_results
