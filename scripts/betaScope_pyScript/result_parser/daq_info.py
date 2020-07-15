import configparser
import logging

logging.basicConfig()
# logging.getLogger().setLevel(logging.CRITICAL)
log = logging.getLogger(__name__)


class DAQInfo(object):
    """
    class handle the DAQ info.
    """

    def __init__(self):
        self.daq_description = configparser.ConfigParser()
        self.run_description = None
        self.is_open = False
        self.run_number = "Run_Number_NA"
        self.dut_name = "DUT_Name_NA"
        self.fluence_type = "Fluence_Type_NA"
        self.fluence = "Fluence_NA"
        self.board = "Board_NA"
        self.board_number = "Board_Number_NA"
        self.temperature = "Temperature_NA"
        self.trig_bias = "Trigger_Voltage_NA"
        self.trig_name = "Trigger_Name_NA"
        self.scope = "Scope_Name_NA"
        self.full_name = f"{daq_info.dut_name}-{daq_info.fluence_type}-{daq_info.fluence}-{daq_info.board}{daq_info.board_number}-{daq_info.temperature}"

    @classmethod
    def open(cls, daq_description_name=""):
        daq_info = cls()
        try:
            daq_info.daq_description.read(daq_description_name)
            daq_info.run_description = daq_info.daq_description["Run_Description"]
            daq_info.is_open = True
        except Exception as e:
            log.critical(
                f"{daq_info.__class__.__name__}-> Error in reading DAQ info. Error: {e}"
            )
            self.is_open = False

        if daq_info.is_open:
            try:
                daq_info.run_number = daq_info.run_description["Run_Number"]
            except:
                daq_info.run_number = "Run_Number_NA"
            try:
                daq_info.dut_name = daq_info.run_description["DUT_Senor_Name"]
            except:
                daq_info.dut_name = "DUT_Name_NA"
            try:
                daq_info.fluence_type = daq_info.run_description["DUT_Fluence_Type"]
            except:
                daq_info.fluence_type = "Fluence_Type_NA"
            try:
                daq_info.fluence = daq_info.run_description["DUT_Fluence"]
            except:
                daq_info.fluence = "Fluence_NA"
            try:
                daq_info.board = daq_info.run_description["DUT_Readout_Board"]
            except:
                daq_info.board = "Board_NA"
            try:
                daq_info.board_number = daq_info.run_description[
                    "DUT_Readout_Board_Number"
                ]
            except:
                daq_info.board_number = "Board_Number_NA"
            try:
                daq_info.temperature = daq_info.run_description["Temperature"]
            except:
                daq_info.temperature = "Temperature_NA"
            try:
                daq_info.trig_bias = daq_info.run_description["Trigger_Voltage"]
            except:
                daq_info.trig_bias = "Trigger_Voltage_NA"
            try:
                daq_info.trig_name = daq_info.run_description["Trigger_Sensor_Name"]
            except:
                daq_info.trig_bias = "Trigger_Name_NA"
            try:
                daq_info.scope = daq_info.run_description["Scope_Name"]
            except:
                daq_info.scope = "Scope_Name_NA"

        daq_info.full_name = f"{daq_info.dut_name}-{daq_info.fluence_type}-{daq_info.fluence}-{daq_info.board}{daq_info.board_number}-{daq_info.temperature}"

        return daq_info
