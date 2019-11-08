import configparser
import pickle

class BetaScopeResult(object):
    def __init__(self):
        self.beta_runs = {}

    def add_run(self, beta_run):
        if not beta_run.run_number in self.beta_runs:
            self.beta_runs[beta_run.run_number]( beta_run )
        else:
            pass

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
        self.fit_results = []
        self.cv_results = None
        self.daq_info = None

    def add_fit_result(self, fit_result):
        self.fit_results.append(fit_result)

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
        self.time_resolution = None

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
            except Exception as e:
                print(e)
