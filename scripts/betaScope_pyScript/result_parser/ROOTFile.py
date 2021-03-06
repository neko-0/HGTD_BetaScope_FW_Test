import ROOT
from array import array

import logging, coloredlogs

logging.basicConfig()
log = logging.getLogger(__name__)
coloredlogs.install(level="CRITICAL", logger=log)


class RootFile(object):
    def __init__(self, filename, treename, descri=""):
        self.filename = filename
        self.treename = treename
        self.ttree = ROOT.TTree(treename, descri)
        self.branches = {}

    def __del__(self):
        tfile = ROOT.TFile(self.filename, "RECREATE")
        tfile.cd()
        self.ttree.Write()
        tfile.Close()

    def __getitem__(self, name):
        if name not in self.branches:
            raise KeyError(f"branch {name} dose not exist.")
        else:
            return self.branches[name]

    def create_branch(self, name, type):
        if name in self.branches:
            log.warning(f"branch {name} is already existed.")
        else:
            self.branches[name] = array(type, [0])
            self.ttree.Branch(name, self.branches[name], f"{name}/{type.upper()}")

    def set_branch_value(self, name, value):
        self.branches[name][0] = value

    def create_char_branch(self, name):
        if name in self.branches:
            log.warning(f"char branch {name} is already existed.")
        else:
            self.branches[name] = bytearray(1024)
            self.ttree.Branch(name, self.branches[name], f"{name}[1024]/C")

    def set_char_branch_value(self, name, value):
        value_encode = value.encode()
        self.branches[name][: len(value_encode)] = value_encode

    def fill(self):
        self.ttree.Fill()
