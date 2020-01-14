"""
structure class for sensor's data
"""
import logging, coloredlogs

logging.basicConfig()
log = logging.getLogger(__name__)
coloredlogs.install(level="INFO", logger=log)

import configparser
import pickle
from copy import deepcopy

import ROOT
from array import array

import inspect


class BetaResult(object):
    """
    The structure should be

    BetaResult(run_number, name)
    |
    |-----Fit Result from charge collection
    |
    |
    |-----DAQ information
    """

    # ===========================================================================
    # ===========================================================================
    class FitResult(object):
        """
        Internal class handle the fit results
        """

        def __init__(self, channel):
            self.channel = channel

        def load_time_resolution(self, fname, cfd):
            try:
                with open(fname, "r") as f:
                    for line in f.readlines():
                        line_split = line.split(",")
                        if str(self.cycle) in str(line_split[5]).split("\n")[0] and str(
                            self.bias_voltage
                        ) in str(line_split[2]):
                            if cfd == 50:
                                self.time_resolution_50 = float(line_split[3])
                                self.time_resolution_err_50 = float(line_split[4])
                            if cfd == 20:
                                self.time_resolution_20 = float(line_split[3])
                                self.time_resolution_err_20 = float(line_split[4])
            except Exception as e:
                log.warning(
                    "{}-> Error encounter during time resolution. Error: {}".format(
                        self.__class__.__name__, e
                    )
                )

        def load_leakage(self, fname):
            try:
                with open(fname, "r") as f:
                    for line in f.readlines():
                        line_split = line.split(",")
                        if str(self.cycle) in str(line_split[4]).split("\n")[0] and str(
                            self.bias_voltage
                        ) in str(line_split[2]):
                            self.leakage = float(line_split[3])
                            self.temperature = float(line_split[1])
            except Exception as e:
                log.warning(
                    "{}-> Error entounter during leakage. Error: {}".format(
                        self.__class__.__name__, e
                    )
                )

    # ===========================================================================
    # ===========================================================================
    class DAQInfo(object):
        """
        Internal class handle the DAQ info.
        """

        def __init__(self, run_number, daq_description_name=""):
            self.daq_description = configparser.ConfigParser()
            self.run_number = run_number
            if daq_description_name:
                try:
                    self.daq_description.read(daq_description_name)
                    self.sensor_name = self.daq_description["Run_Description"][
                        "DUT_Senor_Name"
                    ]
                    self.temperature = self.daq_description["Run_Description"][
                        "Temperature"
                    ]
                    self.trigger_bias = self.daq_description["Run_Description"][
                        "Trigger_Voltage"
                    ]
                    self.dut_board = self.daq_description["Run_Description"][
                        "DUT_Readout_Board"
                    ]
                    self.dut_board_number = self.daq_description["Run_Description"][
                        "DUT_Readout_Board_Number"
                    ]
                    self.dut_fluence_type = self.daq_description["Run_Description"][
                        "DUT_Fluence_Type"
                    ]
                    self.dut_fluence = self.daq_description["Run_Description"][
                        "DUT_Fluence"
                    ]
                except Exception as e:
                    log.warning(
                        "{}-> Error in reading DAQ info. Error: {}".format(
                            self.__class__.__name__, e
                        )
                    )

    # ===========================================================================
    # ===========================================================================
    """
    __init__ and methods for BetaResult class
    """

    def __init__(self, run_number, name):
        self.run_number = run_number
        self.name = name
        self.fit_result = {}
        self.daq_info = None

    def load_result(self, result_file_ini, time_res_file_list, leakage_file, tag):
        parser = configparser.ConfigParser()
        try:
            parser.read(result_file_ini)
        except Exception as e:
            log.critical(
                "{}-> Cannot load result file. Error: {}".format(
                    self.__class__.__name__, e
                )
            )
            return {}

        if not (tag in self.fit_result):
            self.fit_result[tag] = []

        for sec in parser.sections():
            fit_result = BetaResult.FitResult(tag)
            if tag in sec:
                if tag != "Trig":
                    if ".." in sec:
                        fit_result.bias_voltage = int(
                            sec[sec.find("_") + 1 : sec.find("V")]
                        )
                        fit_result.cycle = sec.split("..")[1]
                        if "_" in fit_result.cycle:
                            fit_result.cycle = int(fit_result.cycle.split("_")[0])
                    else:
                        fit_result.bias_voltage = int(
                            sec[sec.find("_") + 1 : sec.find("V")]
                        )
                        fit_result.cycle = 1
                else:
                    try:
                        fit_result.bias_voltage = int(parser[sec]["trigger_bias"])
                        if ".." in sec:
                            fit_result.cycle = sec.split("..")[1]
                            if "_" in fit_result.cycle:
                                fit_result.cycle = int(fit_result.cycle.split("_")[0])
                        else:
                            fit_result.cycle = 1
                    except:
                        fit_result.bias_voltage = -390
                        fit_result.cycle = 1

                fit_result.pulse_area = float(parser[sec]["PulseArea"])
                fit_result.pmax = float(parser[sec]["Pmax"])
                fit_result.rms = float(parser[sec]["RMS"])
                fit_result.rise_time = float(parser[sec]["Rise_Time"])
                fit_result.dvdt = float(parser[sec]["dvdt"])
                fit_result.fwhm = float(parser[sec]["FWHM"])
                fit_result.new_pulse_area = float(parser[sec]["NewPulseArea"])
                try:
                    fit_result.fall_time = float(parser[sec]["FallTime"])
                    fit_result.pulse_area_chi = float(parser[sec]["PulseArea_CHI_NDF"])
                    fit_result.pmax_chi = float(parser[sec]["Pmax_CHI_NDF"])
                    fit_result.rms_chi = float(parser[sec]["RMS_CHI_NDF"])
                    fit_result.rise_time_chi = float(parser[sec]["Rise_Time_CHI_NDF"])
                    fit_result.dvdt_chi = float(parser[sec]["dvdt_CHI_NDF"])
                    fit_result.fwhm_chi = float(parser[sec]["FWHM_CHI_NDF"])
                    fit_result.new_pulse_area_chi = float(
                        parser[sec]["NewPulseArea_CHI_NDF"]
                    )
                    fit_result.fall_time_chi = float(parser[sec]["FallTime_CHI_NDF"])
                except Exception as e:
                    log.warning(
                        "{}-> Error in loading fit results: {}".format(
                            self.__class__.__name__, e
                        )
                    )
            for time_res_file, cfd in time_res_file_list:
                fit_result.load_time_resolution(time_res_file, cfd)
            fit_result.load_leakage(leakage_file)

            self.fit_result[tag].append(fit_result)

    def load_daq_info(self, run_number, daq_description_name):
        self.daq_info = BetaResult.DAQInfo(run_number, daq_description_name)


# ===============================================================================
# ===============================================================================


class BetaCollector(object):
    def __init__(self):
        self.beta_runs = {}
        self.user_data_dir = "./user_data/"

    @classmethod
    def load(cls, ifile):
        try:
            with open(ifile, "rb") as f:
                data = pickle.load(f)
                cls_obj = cls()
                cls_obj.beta_runs = data.beta_runs
                return cls_obj
        except:
            log.critical("cannot load file. Return default instance of object.")

    def run_exists(self, beta_run):
        if beta_run.run_number in self.beta_runs:
            return True
        else:
            return False

    def fetchRun(self, run_list):
        output = []
        for run_num, run in run_list:
            output.append(self.beta_runs[run_num])
        return output

    def generate_run_list(self):
        run_list = []
        for run_num, run in self.beta_runs.items():
            run_list.append([run_num, run.name])
        return run_list

    def add_run(self, beta_run):
        self.beta_runs[beta_run.run_number] = beta_run

    def save(self, out_path):
        with open(out_path, "wb") as f:
            pickle.dump(self, f, 2)

    def to_root(self, ofile_name):
        if self.beta_runs:
            ofile = ROOT.TFile(ofile_name, "RECREATE")
            ofile.cd()
            for run_num, run_item in self.beta_runs.items():
                for tag, result in run_item.fit_result.items():
                    if len(result) == 0:
                        continue
                    ttree = ROOT.TTree(
                        "{run_num}_{tag}".format(run_num=run_num, tag=tag),
                        "{}".format(run_item.name),
                    )
                    array_dict = {}
                    obj_attr = inspect.getmembers(
                        result[0], lambda a: not (inspect.isroutine(a))
                    )
                    obj_var = [
                        a
                        for a in obj_attr
                        if not (a[0].startswith("__") and a[0].endswith("__"))
                    ]
                    for var_name, var_value in obj_var:
                        if not isinstance(var_value, str):
                            array_dict[var_name] = array("d", [0])
                            ttree.Branch(
                                var_name, array_dict[var_name], "{}/D".format(var_name)
                            )
                    for r in result:
                        for var_name, var_value in obj_var:
                            if var_name in array_dict and var_name in r.__dict__:
                                array_dict[var_name][0] = r.__dict__[var_name]
                        ttree.Fill()
                    ttree.Write()
            ofile.Close()


# ===============================================================================
# ===============================================================================
"""
class SensorData(object):
    def __init__(self):
        self.sensor_name = name
        self.beta_run_num = beta_run_num
        self.beta_data = None

        self.cv_data = None
        self.iv_data = None

        self.beta_daq_info = None

    def add_beta(self, beta_data):
        self.beta_data = beta_data

    def add_cv(self, cv_data):
        self.cv_data = cv_data

    def add_iv(self, iv_data):
        self.iv_data = iv_data

    def add_beta_daq(self, beta_daq_info):
        self.beta_daq_info = beta_daq_info
"""
