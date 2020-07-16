import configparser

RUN_INFO_VER = "2020_7_14"


class Run(object):
    def __init__(
        self, sensor, bias, file_name, temperature, cycle, dut_ch, trig_ch, cuts
    ):
        self.sensor = sensor
        self.bias = bias
        self.file_name = file_name
        self.temperature = temperature
        self.cycle = cycle
        self.dut_ch = dut_ch
        self.trig_ch = trig_ch
        self.cuts = cuts


class ConfigReader(object):
    @staticmethod
    def open(config=f"run_info_v{RUN_INFO_VER}.ini"):
        config_file = configparser.ConfigParser()
        config_file.read(config)

        sensor = config_file["header"]["sensor"]

        dut_ch = config_file["header"]["dut_channel"]
        trig_ch = config_file["header"]["trigger_channel"]

        run_list = []

        header = {key: config_file["header"][key] for key in config_file["header"]}

        for run_num in range(int(config_file["header"]["number_of_runs"])):

            fname = config_file[f"run{run_num}"]["file_name"]

            cycle = 1
            if "root." in fname:
                cycle = int(fname.split("root.")[1])

            bias = int(config_file[f"run{run_num}"]["bias"].split("V")[0])
            try:
                temperature = config_file[f"run{run_num}"]["temperature"]
            except:
                temperature = -30

            raw_cut = config_file[f"run{run_num}"][f"cut_{dut_ch}"].split(" ")

            dut_cut = "tmax{dut_ch}[0]-cfd{trig_ch}[20] > {dut[0]} && tmax{dut_ch}[0]-cfd{trig_ch}[20] < {dut[1]} && pmax{dut_ch}[0] > {dut[2]} && pmax{dut_ch}[0] < {dut[3]}".format(
                dut_ch=dut_ch, trig_ch=trig_ch, dut=raw_cut[:4]
            )

            trig_cut = "tmax{trig_ch}[0]-cfd{trig_ch}[20] > {trig[0]} && tmax{trig_ch}[0]-cfd{trig_ch}[20] < {trig[1]} && pmax{trig_ch}[0] > {trig[2]} && pmax{trig_ch}[0] < {trig[3]}".format(
                trig_ch=trig_ch, trig=raw_cut[4:8]
            )
            if len(raw_cut) == 8:
                cuts = f"{dut_cut} && {trig_cut}"
            elif len(raw_cut) == 9:
                cuts = f"{dut_cut} && {trig_cut} && {raw_cut[8]}"
            else:
                cuts = f"{dut_cut} && {trig_cut}"

            run_list.append(
                Run(sensor, bias, fname, temperature, cycle, dut_ch, trig_ch, cuts)
            )

        return (header, run_list)
