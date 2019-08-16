'''
give string some color!
'''

class colorString:

    RED = "\033[91m"
    GREEN = "\033[92m"
    YELLOW = "\033[93m"
    CYAN = "\033[96m"
    PURPLE = "\033[95m"
    END = "\033[00m"

    @staticmethod
    def colorFormat( iString, color):

        if "red" in color:
            return "{c}{content}{e}".format(c = colorString.RED, content = iString, e = colorString.END )
        elif "green" in color:
            return "{c}{content}{e}".format(c = colorString.GREEN, content = iString, e = colorString.END )
        elif "yellow" in color:
            return "{c}{content}{e}".format(c = colorString.YELLOW, content = iString, e = colorString.END )
        elif "cyan" in color:
            return "{c}{content}{e}".format(c = colorString.CYAN, content = iString, e = colorString.END )
        elif "purple" in color:
            return "{c}{content}{e}".format(c = colorString.PURPLE, content = iString, e = colorString.END )
        else:
            return iString

    @staticmethod
    def sysMsg( iString ):
        print( colorString.colorFormat(iString, "cyan") )

    @staticmethod
    def sysError( iString ):
        print( colorString.colorFormat(iString, "red") )
