import configparser

from core import ExcelBase


class BetaExcel(ExcelBase):

    temperature = "A"
    sensor_name = "B"
    bias_voltage = "H"
    resistance = "L"
    pulse_area = "J"
    pulse_area_error = "K"
    pmax = "R"
    pmax_error = "S"
    rms = "T"
    rms_error = "U"
    rise_time = "Z"
    rise_time_error = "AA"
    dvdt = "AB"
    dvdt_error = "AC"
    fwhm = "AL"
    fwhm_error = "AM"
    new_pulse_area = "CA"
    new_pulse_area_error = "CB"
    fall_time = "DG"
    fall_time = "DH"
    cycle = "F"
    cfd50_res = "BW"
    cfd50_res_error = "BX"
    cfd20_res = "DD"
    cfd20_res_error = "DE"
    leakage = "C"
    charge = "M" #"={c1}{row}/{c2}{row}".format(c1=pulse_area,c2=resistance, row="{row}")
    charge_error = "N"
    num_e = "O"
    pin = "P"
    new_charge = "CC" #"={c1}{row}/{c2}{row}".format(c1=new_pulse_area,c2=resistance, row="{row}")
    new_charge_error = "CD"
    snr = "X"
    snr_corr = "Y"
    jitter_rise = "AJ"
    new_num_e = "CE"
    gain = "Q"
    new_gain = "CF"

    def __init__(self):
        pass

    @classmethod
    def parse_ini(cls, fname="_results.ini"):
        new_wb = cls.new()
        config = configparser.ConfigParser()
        config.read(fname)
        config_section = config.sections()
        new_wb.workbook.create_sheet("DUT")
        new_wb.workbook.create_sheet("TRIG")
        row_counter = 1
