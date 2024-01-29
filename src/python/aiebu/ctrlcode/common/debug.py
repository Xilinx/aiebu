
class Line:
    def __init__(self, linenumber, high_pc, low_pc, opcode):
        self.linenumber = linenumber
        self.high_pc = high_pc
        self.low_pc = low_pc
        self.opcode = opcode

    def __str__(self):
        return f"linenumber:{self.linenumber} high_pc:{self.high_pc} low_pc:{self.low_pc} opcode:{self.opcode}"

    def getlinenumber(self):
        return self.linenumber

    def gethigh_pc(self):
        return self.high_pc

    def getlow_pc(self):
        return self.low_pc

    def getopcode(self):
        return self.opcode

class Function:
    def __init__(self, filename, name, high_pc, low_pc, col, pagenum):
        self.filename = filename
        self.name = name
        self.col_num = col
        self.page_num = pagenum
        self.high_pc = high_pc
        self.low_pc = low_pc
        self.textlines = []
        self.datalines = []

    def addtextline(self, line):
        self.textlines.append(line)

    def adddataline(self, line):
        self.datalines.append(line)

    def __str__(self):
        s = f"\tfilename:{self.filename} name:{self.name} col_num:{self.col_num} page:{self.page_num} high_pc:{self.high_pc} low_pc:{self.low_pc} line:\n"
        for l in self.textlines:
            s = s + f"\t\t{l}\n"
        for l in self.datalines:
            s = s + f"\t\t{l}\n"
        return s

    def getfilename(self):
        return self.filename

    def getname(self):
        return self.name

    def getcolumnnumber(self):
        return self.col_num

    def gethigh_pc(self):
        return self.high_pc

    def getlow_pc(self):
        return self.low_pc

class Debug:
    def __init__(self):
        self.functions = {}

    def addfunction(self, filename, name, high_pc, low_pc, col, pagenum):
        self.functions[col+name] = Function(filename, name, high_pc, low_pc, col, pagenum)
        return col+name

    def addtextline(self, func, linenumber, high_pc, low_pc, token):
        self.functions[func].addtextline(Line(linenumber, high_pc, low_pc, token))

    def adddataline(self, func, linenumber, high_pc, low_pc, token):
        self.functions[func].adddataline(Line(linenumber, high_pc, low_pc, token))

    def __str__(self):
        s = ""
        for i in self.functions:
            s = s + f"{self.functions[i]} "
        return s
