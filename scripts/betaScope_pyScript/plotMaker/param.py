param = {
    "bias_voltage_vs_charge": {
        "expr": "bias_voltage:new_pulse_area/4700.0",
        "selection": "",
        "xtitle": "Bias Voltage [V]",
        "ytitle": "Charge [fC]",
    },
    "bias_voltage_vs_pmax": {
        "expr": "bias_voltage:pmax",
        "xtitle": "Bias Voltage [V]",
        "ytitle": "Pmax [mV]",
    },
    "bias_voltage_vs_rms": {
        "expr": "bias_voltage:rms",
        "xtitle": "Bias Voltage [V]",
        "ytitle": "rms [mV]",
    },
    "bias_voltage_vs_SNR": {
        "expr": "bias_voltage:pmax/rms",
        "xtitle": "Bias Voltage [V]",
        "ytitle": "SNR",
    },
    "bias_voltage_vs_rise_time": {
        "expr": "bias_voltage:rise_time",
        "xtitle": "Bias Voltage [V]",
        "ytitle": "rise time [ps]",
    },
    "bias_voltage_vs_fwhm": {
        "expr": "bias_voltage:fwhm",
        "xtitle": "Bias Voltage [V]",
        "ytitle": "FWHM [ps]",
    },
    "bias_voltage_vs_res50": {
        "expr": "bias_voltage:time_resolution_50",
        "selection": "new_pulse_area_chi<1",
        "xtitle": "Bias Voltage [V]",
        "ytitle": "Time Resolution [ps]",
    },
    "charge_vs_res50": {
        "expr": "new_pulse_area/4700.0:time_resolution_50",
        "xtitle": "Charge [fC]",
        "ytitle": "Time Resolution [ps]",
    },
}
