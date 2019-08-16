'''
commandline oriented interface using the cmd module for the beta-scope analysis
Testing version
'''

import readline
readline.get_completer_delims()
#readline.set_completer_delims(' \t\n;')

import glob
import os
import sys
import cmd
import subprocess
from colorStringFormating import *
from lgad import *

predefined_path = {
"__raw":"/media/mnt/BigHD/Beta_DAQ_Data/",
"__yuzhan":"/media/mnt/BigHD/BetaScope_Data/Analyzed_YZ/"
}


class Meow(Lgad, object):

    intro = colorString.colorFormat("   Interface for beta-scope analysis (Testing ver)", "cyan")
    prompt = colorString.colorFormat("(Meow~) ", "yellow")

    '''
    def __init__(self, mode):
    if mode == "test":
        self.prompt = colorString.colorFormat("(Meow~) ", "yellow")
    cmd.Cmd.__init__(self)
    '''
    def __init__(self):
        self.files = os.listdir(os.getcwd())
        super(Meow, self).__init__()

    def do_cd(self, line="./"):
        super(Meow, self).do_cd(line)
        self.files = os.listdir(os.getcwd())

    def completedefault(self, text, line, begidx, endidx):
        return [i for i in self.files if i.startswith(text)]


if __name__ == "__main__":
    interface = Meow()
    interface.cmdloop()
