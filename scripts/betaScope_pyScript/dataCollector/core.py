"""
structure class for sensor's data
"""
import logging, coloredlogs

logging.basicConfig()
log = logging.getLogger(__name__)
coloredlogs.install(level="CRITICAL", logger=log)

import configparser
import pickle

import ROOT
from array import array
from copy import deepcopy

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
            self.cycle = 1

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
                            if cfd == "tmax":
                                self.time_resolution_tmax = float(line_split[3])
                                self.time_resolution_err_tmax = float(line_split[4])
                            if cfd == "fit_tmax":
                                self.time_resolution_fit_tmax = float(line_split[3])
                                self.time_resolution_err_fit_tmax = float(line_split[4])
                            if cfd == "zero_cross_tmax":
                                self.time_resolution_zero_cross_tmax = float(line_split[3])
                                self.time_resolution_err_zero_cross_tmax = float(line_split[4])
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

            self.fit_result[tag].append(fit_result)

        if tag != "Trig":
            for fit_r in self.fit_result[tag]:
                for time_res_file, cfd in time_res_file_list:
                    fit_r.load_time_resolution(time_res_file, cfd)
                fit_r.load_leakage(leakage_file)

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
            with open(ifile,  "rb") as f:
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

    def add_run(self, beta_run, icount=2):
        '''
        beta_run should be class instance of BetaResult
        '''
        counter = icount
        if beta_run.run_number in self.beta_runs:
            if "add{}_".format(counter-1) in beta_run.run_number:
                beta_run.run_number=beta_run.run_number.split("add{}_".format(counter-1))[1]
            beta_run.run_number="add{}_".format(counter)+beta_run.run_number
            self.add_run(beta_run, counter+1)
        self.beta_runs[beta_run.run_number] = beta_run

    def save(self, out_path):
        with open(out_path, "wb") as f:
            pickle.dump(self, f, 2)

    @staticmethod
    def _roottree_setup(run_num, tag, name, fit_result_cls_obj, branch_holder):
        """
        setup output ttree. automatically create branch from the fit results object.

        param:
            run_num := run number, integer value
            tag := name tag for lookup in the fit result INI file. 'DUT' or 'Trig' for example
            name := description for full file name. Use for reference for plotting.
            fit_result_cls_obj := class object from BetaResult.FitResult

        return:
            ttree
        """
        tree_name = "{}_{}".format(run_num, tag)
        description = name
        ttree = ROOT.TTree(tree_name, description)
        """
        Found on stackover-flow. Get the variables out of a class object
        """
        obj_attr = inspect.getmembers(
            fit_result_cls_obj, lambda a: not (inspect.isroutine(a))
        )
        obj_vars = [
            attr
            for attr in obj_attr
            if not (attr[0].startswith("__") and attr[0].endswith("__"))
        ]

        for var_name, var_value in obj_vars:
            if not isinstance(var_value, str):
                branch_holder[var_name] = array("d", [0])
                ttree.Branch(var_name, branch_holder[var_name], "{}/D".format(var_name))

        return (ttree, obj_vars)

    def to_root(self, ofile_name):
        if self.beta_runs:
            ofile = ROOT.TFile(ofile_name, "RECREATE")
            ofile.cd()
            for run_num, run_item in self.beta_runs.items():
                '''
                run_num is the run number,
                run_item is class instance of BetaResult
                '''
                for tag, result in run_item.fit_result.items():
                    '''
                    tag in here is just a label for 'DUT' or 'Trig' in fit result file or ttree
                    '''
                    if len(result) == 0:
                        continue
                    branch_dict = {}
                    ttree, obj_vars = self._roottree_setup(
                        run_num, tag, run_item.name, result[0], branch_dict
                    )

                    sort_buffer = {}
                    for r in result:
                        for var_name, _ in obj_vars:
                            if var_name in branch_dict and var_name in r.__dict__:
                                if not(var_name in sort_buffer):
                                    sort_buffer[var_name] = []
                                sort_buffer[var_name].append(r.__dict__[var_name])

                    sort_against_key = "bias_voltage"
                    if sort_against_key in sort_buffer:
                        sort_against_list = deepcopy(sort_buffer[sort_against_key])
                        for key, l in sort_buffer.items():
                            if key != sort_against_key:
                                #print(tag)
                                #if key=="pmax" and "Run643" in run_item.name:print("before {}".format(l))
                                sort_buffer[key] = [x for _,x in sorted(zip(sort_against_list, l))]
                                #if key=="pmax" and "Run643" in run_item.name:print("after {}".format(l))
                        #if "Run643" in run_item.name:print("before {}".format(sort_buffer[sort_against_key]))
                        sort_buffer[sort_against_key] = sorted(sort_buffer[sort_against_key])
                        #if "Run643" in run_item.name:print("after {}".format(sort_buffer[sort_against_key]))
                        #if "Run643" in run_item.name:print("after {}".format(sort_buffer["pmax"]))
                    else:
                        continue
                    '''
                    for r in result:
                        for var_name, _ in obj_vars:
                            if var_name in branch_dict and var_name in r.__dict__:
                                branch_dict[var_name][0] = r.__dict__[var_name]
                    '''
                    count = len(sort_buffer[sort_against_key])
                    '''
                    if "Run643" in run_item.name:
                        print("{}\n{}".format(sort_buffer["bias_voltage"], sort_buffer["pmax"]))
                    '''
                    if count <= 0:
                        continue
                    for i in range(count):
                        for var_name in sort_buffer.keys():
                            if var_name in branch_dict:
                                if len(sort_buffer[var_name])!=count:
                                    log.critical("list size dose not match: {}->{}".format(run_item.name, var_name))
                                else:
                                    branch_dict[var_name][0] = sort_buffer[var_name][i]
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
