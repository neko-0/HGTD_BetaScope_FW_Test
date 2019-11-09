import configparser
import pickle
from copy import deepcopy

class BetaScopeResult(object):
    def __init__(self, fname=""):
        self.beta_runs = {}
        if fname:
            self.load(fname)

    def run_exists(self, beta_run):
        if beta_run.run_number in self.beta_runs:
            return True
        else:
            return False

    def add_run(self, beta_run):
        self.beta_runs[beta_run.run_number] = deepcopy(beta_run)

    def save(self, out_path):
        with open(out_path, "wb") as f:
            pickle.dump(self, f, 2)

    def load(self, ifile):
        try:
            with open(ifile, "rb") as f:
                data = pickle.load(f)
                self.beta_runs = data.beta_runs
        except:
            pass

class BetaRun(object):
    def __init__(self, run_number, name):
        self.run_number = run_number
        self.name = name
        self.fit_results = None
        self.cv_results = None
        self.daq_info = None

    def add_fit_result(self, fit_result):
        self.fit_results = deepcopy(fit_result)

    def add_cv_result(self):
        pass

    def update_daq_info(self, daq_info):
        self.daq_info = daq_info

class FitResult(object):

    def __init__(self, channel):
        self.channel = channel
        self.bias_voltage = None
        self.pulse_area = None
        self.pmax = None
        self.rms = None
        self.rise_time = None
        self.fwhm = None
        self.fall_time = None
        self.new_pulse_area = None
        self.resistance = None
        self.temperature = None
        self.dvdt = None
        self.cycle = 1
        self.time_resolution_50 = None
        self.time_resolution_err_50 = None
        self.time_resolution_20 = None
        self.time_resolution_err_20 = None
        self.leakage = None

    def update_time_resolution(self, fname, cfd):
        try:
            with open(fname, "r") as f:
                for line in f.readlines():
                    line_split = line.split(",")
                    #print(type(self.cycle))
                    #print(line_split[2])
                    if(str(self.cycle) in str(line_split[5]).split("\n")[0] and str(self.bias_voltage) in str(line_split[2])):
                        if cfd==50:
                            self.time_resolution_50 = float(line_split[3])
                            self.time_resolution_err_50 = float(line_split[4])
                        if cfd==20:
                            self.time_resolution_20 = float(line_split[3])
                            self.time_resolution_err_20 = float(line_split[4])
                        #print(self.time_resolution_50)
        except:
            pass

    def update_leakage(self, fname):
        try:
            with open(fname, "r") as f:
                for line in f.readlines():
                    line_split = line.split(",")
                    if(str(self.cycle) in str(line_split[4]).split("\n")[0] and str(self.bias_voltage) in str(line_split[2])):
                        self.leakage = float(line_split[3])
        except:
            pass

class DAQInfo(object):

    def __init__(self, run_number, daq_description_name=""):
        self.daq_description = configparse.ConfigParser()
        self.sensor_name = None
        self.temperature = None
        self.trigger_bias = None
        self.dut_board = None
        self.dut_board_number = None
        self.dut_fluence_type = None
        self.dut_fluence = None
        self.run_number = run_number
        if daq_description_name:
            try:
                self.daq_description.read(daq_description_name)
                self.sensor_name  = self.daq_description["Run_Description"]["DUT_Senor_Name"]
                self.temperature = self.daq_description["Run_Description"]["Temperature"]
                self.trigger_bias = self.daq_description["Run_Description"]["Trigger_Voltage"]
                self.dut_board = self.daq_description["Run_Description"]["DUT_Readout_Board"]
                self.dut_board_number = self.daq_description["Run_Description"]["DUT_Readout_Board_Number"]
                self.dut_fluence_type = self.daq_description["Run_Description"]["DUT_Fluence_Type"]
                self.dut_fluence = self.daq_description["Run_Description"]["DUT_Fluence"]
            except:
                raise IOError
