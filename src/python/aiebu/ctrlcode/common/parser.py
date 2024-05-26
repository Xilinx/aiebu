import re
import sys
import os.path
from abc import ABC, abstractmethod

from ctrlcode.common.operation import Operation
from ctrlcode.common.label import Label
from ctrlcode.common.section import Section
from ctrlcode.common.util import parse_num_arg

class Patch(ABC):
    def __init__(self):
        self._op = ""
        self._args = []

    @abstractmethod
    def patch(self, op, args):
        self._args = [x.strip() for x in args.strip().split(',')] if args is not None else []
        print(f"WARNING: opcode {op} is deprecated. use {self._op} instead!!")

class WordPatch(Patch):
    def __init__(self):
        self._op = '.long'

    def patch(self, op, args):
        super().patch(op, args)
        return self._op, args

class AlignPatch(Patch):
    def __init__(self):
        self._op = '.align'

    def patch(self, op, args):
        super().patch(op, args)
        return self._op, f"{parse_num_arg(self._args[0], None)}"

class P2alignPatch(AlignPatch):
    def __init__(self):
        self._op = '.align'

    def patch(self, op, args):
        super().patch(op, args)
        # .p2align 4 is equal to .align 16
        return self._op, f"{1 << parse_num_arg(self._args[0], None)}"

class Data:
    def __init__(self, token, section, size, page_num, ifile, line, ln):
        self._token = token
        self._section = section
        self._size = size
        self._page_num = page_num
        self._line_number = ln
        self._file = ifile
        self._line = line

    def gettoken(self):
        return self._token

    def getsection(self):
        return self._section

    def setsection(self, section):
        self._section = section

    def getsize(self):
        return self._size

    def setsize(self, size):
        self._size = size

    def getpagenum(self):
        return self._page_num

    def getlinenum(self):
        return self._line_number

    def getline(self):
        return self._line

    def getfilename(self):
        return self._file

    def setpagenum(self, page_num):
        self._page_num = page_num

    #def __str__(self):
    #    return f"TOKEN:{self._token} SECTION:{self._section}  SIZE:{self._size} pagenum:{self._page_num} ln:{self._line_number}\tfile:{self._file}\n"

    def __str__(self):
        from pprint import pformat
        return pformat(vars(self), indent=4, width=1)

class AttachToGroup:
    def operate(self, args, parser):
        args = [x.strip() for x in args.strip().split(',')] if args is not None else []
        parser.setcurrentcol(args[0])

class Include:
    def readfile(self, path, parser):
        if not os.path.isfile(path):
            return False
        print(f"Reading file: {path}")
        ifile = open(path, 'r')
        for linenumber, line in enumerate(ifile, 1):
            parser.parse_line(line, path, linenumber)
        ifile.close()
        return True

    def operate(self, args, parser):
        args = [x.strip() for x in args.strip().split(',')] if args is not None else []
        if os.path.isabs(args[0]):
            if not self.readfile(args[0], parser):
                raise RuntimeError(f"File {args[0]} not exist\n")
        for path in parser.includedirlist:
            path = path + "/" + args[0]
            if os.path.isfile(path):
                self.readfile(path, parser)
                parser.isdata = False
                return

        raise RuntimeError(f"File {args[0]} not exist....\n")

class EndOfLabel:
    def operate(self, args, parser):
        s = parser.current_label
        label = s.rsplit('::', 1)[-1] or s
        parser.current_label = s.rsplit('::', 1)[-2] or s
        if label != args:
            raise RuntimeError(f"endl label missmatch, {label} != {args}")

class ScratchPad:
    def operate(self, args, parser):
        args = [x.strip() for x in args.strip().split(',')] if args is not None else []
        parser.insertscratchpad(args[0], args[1])

class SectionDirective:
    def operate(self, args, parser):
        args = [x.strip() for x in args.strip().split(',')] if args is not None else []
        if args[0][0:9] == ".ctrltext":
            parser.isdata = False

class Directive:
    DIRECTIVE = {'.attach_to_group': AttachToGroup(),
                 '.include': Include(),
                 '.endl': EndOfLabel(),
                 '.setscratchpad': ScratchPad(),
                 '.section': SectionDirective() }
    DIRECTIVE_REGEX = re.compile(r'^([.a-zA-Z0-9_]+)(?:\s+(.+)+)?$')
    def __init__(self, parser):
        self.Parser = parser

    def operate(self, line):
        directive = Directive.DIRECTIVE_REGEX.match(line)
        if directive:
            if directive[1] in Directive.DIRECTIVE:
                Directive.DIRECTIVE[directive[1]].operate(directive[2], self.Parser)
                return True
        return False

class Column_Code:
    def __init__(self):
        self.text = {}
        self.data = []
        self.scratchpad = {}
        self.labelpageindex = {}

    def __str__(self):
        from pprint import pformat
        return pformat(vars(self), indent=4, width=1)

class Parser:
    COMMENT_REGEX = re.compile(r'^;(.*)$')
    LABEL_REGEX = re.compile(r'^([a-zA-Z0-9_]+)\:$')
    OP_REGEX = re.compile(r'^([.a-zA-Z0-9_]+)(?:\s+(.+)+)?$')
    PATCHMAP = {'WORD':WordPatch(), \
                'ALIGN':AlignPatch(), \
                '.balign':AlignPatch(), \
                '.p2align':P2alignPatch()
               }

    def __init__(self, includedirlist):
        self.col = {}
        self.includedirlist = includedirlist
        self.current_col = None
        self.isdata = False
        self.current_label = "default"
        self.Directive = Directive(self)

    def setcurrentcol(self, col):
        self.current_col = col
        if self.current_col not in self.col:
            self.col[self.current_col] = Column_Code()

    def insertcoldata(self, data, isdata):
        if self.current_col == None:
            self.setcurrentcol('0')
        if isdata:
            self.col[self.current_col].data.append(data)
        else:
            # First segregate control code based on column and then base on label
            if self.current_label not in self.col[self.current_col].text:
                self.col[self.current_col].text[self.current_label] = []
            self.col[self.current_col].text[self.current_label].append(data)
            self.col[self.current_col].labelpageindex[self.current_label.rsplit('::', 1)[-1] or self.current_label] = 0

    def insertscratchpad(self, name, size):
        if self.current_col == None:
            self.setcurrentcol('0')
        size = int(size, 16) if size.startswith('0x') else int(size)
        self.col[self.current_col].scratchpad[name] = {"name": name, "size":size, "offset": 0, "base": 0}

    def getcoldata(self, col):
        return self.col[col].data

    def getcolscratchpad(self, col):
        return self.col[col].scratchpad

    def getcollabelpageindex(self, col):
        return self.col[col].labelpageindex

    def getcoltextforlabel(self, col, label):
        return self.col[col].text[label]

    def getcollist(self):
        return self.col.keys()

    def gettextlabelsforcol(self, col):
        return self.col[col].text.keys()

    def parse_line(self, line, ifile, ln):
        line = line.strip()

        if len(line) == 0:
            return

        # Skip comments
        comment_match = Parser.COMMENT_REGEX.match(line)
        if comment_match:
            return

        # Directive
        if self.Directive.operate(line):
            return

        # Label
        label_match = Parser.LABEL_REGEX.match(line)
        if label_match:
            if not self.isdata:
                self.current_label = self.current_label + "::" + label_match[1]
            else:
                self.insertcoldata(Data(Label(label_match[1]), Section.UNKNOWN, 0, -1, ifile, line, ln), self.isdata)
            return

        # Operation
        op_match = Parser.OP_REGEX.match(line)
        if op_match:
            op = op_match[1]
            args = op_match[2]
            if op in Parser.PATCHMAP:
                op, args = Parser.PATCHMAP[op].patch(op, args)
            self.insertcoldata(Data(Operation(op, args), Section.UNKNOWN, 0, -1, ifile, line, ln), self.isdata)
            if op == "EOF":
                self.isdata = True
            return

        raise RuntimeError('Parse error: Invalid line: {}'.format(line))

    def __str__(self):
        from pprint import pformat
        return pformat(vars(self), indent=4, width=1)
