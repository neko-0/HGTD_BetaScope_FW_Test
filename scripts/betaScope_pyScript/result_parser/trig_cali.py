# trigger calibration
# with the form run_number : (scope, trigger_name, run_number, cali_setting)
TRIG_CALI = {
    660: {
        ("keysight", "hpks8664", "default"): {
            "res": 14.9,
            "res_err": 0.7,
            "var": "cfd{trig_ch}[20]",
            "default_cuts": "pmax3[0]>70 && pmax3[0]<350 && tmax3[0]-cfd3[20]>200 && tmax3[0]-cfd3[20]<500",
            "recommand_cuts": [
                "pmax{trig_ch}[0]>70",
                "pmax{trig_ch}[0]<350",
                "tmax{trig_ch}[0]-cfd{trig_ch}[20]>200 ",
                "tmax{trig_ch}[0]-cfd{trig_ch}[20]<500",
            ],
        },
        ("keysight", "hpks8664", "2"): {
            "res": 13.4,
            "res_err": 0.7,
            "var": "cfd{trig_ch}[20]",
            "default_cuts": "tmax3[0]-cfd3[20]>200 && tmax3[0]-cfd3[20]<450 && isGoodTrig3 && riseTime3[0]>270 && cfd_fall3[50]-cfd3[50]>540",
            "recommand_cuts": [
                " tmax{trig_ch}[0]-cfd{trig_ch}[20]>200",
                "tmax{trig_ch}[0]-cfd{trig_ch}[20]<450",
                "isGoodTrig{trig_ch}",
                "riseTime{trig_ch}[0]>270",
                "cfd_fall{trig_ch}[50]-cfd{trig_ch}[50]>540",
            ],
        },
    },
    520: {
        ("lecroy", "hpks8664", "default"): {
            "res": 16.9,
            "res_err": 0.7,
            "var": "cfd{trig_ch}[20]",
            "default_cuts": "pmax3[0]>70 && pmax3[0]<350 && tmax3[0]-cfd3[20]>200 && tmax3[0]-cfd3[20]<500",
            "recommand_cuts": [
                "pmax{trig_ch}[0]>70",
                "pmax{trig_ch}[0]<350",
                "tmax{trig_ch}[0]-cfd{trig_ch}[20]>200 ",
                "tmax{trig_ch}[0]-cfd{trig_ch}[20]<500",
            ],
        },
    },
    400: {
        ("lecroy", "hpks8664", "default"): {
            "res": 17.6,
            "res_err": 0.7,
            "var": "cfd{trig_ch}[20]",
            "default_cuts": "pmax3[0]>70 && pmax3[0]<350 && tmax3[0]-cfd3[20]>200 && tmax3[0]-cfd3[20]<500",
            "recommand_cuts": [
                "pmax{trig_ch}[0]>70",
                "pmax{trig_ch}[0]<350",
                "tmax{trig_ch}[0]-cfd{trig_ch}[20]>200 ",
                "tmax{trig_ch}[0]-cfd{trig_ch}[20]<500",
            ],
        },
    },
}
