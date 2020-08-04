import pandas

class UDI_reader():
    def __init__(self):
        self.UDI_file_path = "/media/mnt/COVID-19/Central_data_folder/UDI_list.xlsx"
        self.read_UDI()

    def read_UDI(self):
        self.db = pandas.read_excel(self.UDI_file_path, "UDI_1")

    def print_UDI_list(self):
        print (self.db)

    def get_index(self, UDI_n):
        UDI_n = str(int(UDI_n))
        return self.db.index[self.db.UDI == UDI_n]

    def get_pin_charge(self, UDI_n):
        ix = self.get_index(UDI_n)
        return float(self.db["pin_c"][ix])

    def get_foot(self, UDI_n):
        ix = self.get_index(UDI_n)
        return float(self.db["foot_cv"][ix])

    def get_name(self, UDI_n):
        ix = self.get_index(UDI_n)
        return self.db.iloc[ix]["name"]


if __name__ == '__main__':
    reader = UDI_reader()
    #reader.print_UDI_list()

    name = reader.get_name(4)
    pinc = reader.get_pin_charge(4)
    foot = reader.get_foot('4')

    print (f"Params: {foot} {pinc}")
