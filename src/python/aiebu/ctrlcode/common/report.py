# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

from colorama import init as colorama_init
from colorama import Fore
from colorama import Style
import json
import os.path

class Report:

    def __init__(self, symbols, mapfilename, debug=None):
        self._build_id = None
        self._pages = {}
        self._symbols = symbols
        self._debug = debug
        self._mapfilename = mapfilename

    def setbuildid(self, buildid):
        self._build_id = buildid

    def addpage(self, page, state):
        if page.col_num not in self._pages:
           self._pages[page.col_num] = []
        jobs = state.getjoblist()
        jobs.remove('eof')
        self._pages[page.col_num].append({'col_num':page.col_num,
                                         'page_num':page.page_num,
                                         'jobs':jobs,
                                         'barriers':state.getlocalbarrier(),
                                         'joblaunchers':state.getjoblaunchers(),
                                         'textsize':state.gettextsize(),
                                         'datasize':state.getdatasize()
                                         })

    def generate(self):
        colorama_init()
        print(f"{Fore.GREEN}************************** ASSEMBLER REPORT **************************{Style.RESET_ALL}")
        print(f"{Fore.BLUE}BUILD ID: {Fore.MAGENTA}{self._build_id}{Style.RESET_ALL}")
        for col in self._pages:
            print(f"{Fore.BLUE}COLUMN:{Fore.MAGENTA} {col}{Style.RESET_ALL}")
            for page in self._pages[col]:
                print(f"{Fore.BLUE}\tPAGE NO.:          {Fore.CYAN}{page['page_num']}")
                print(f"{Fore.BLUE}\tJOBS:              {Fore.CYAN}{page['jobs']}")
                print(f"{Fore.BLUE}\tSECTIONS:          {Fore.CYAN}.ctrltext.{page['col_num']}.{page['page_num']} .ctrldata.{page['col_num']}.{page['page_num']}")
                print(f"{Fore.BLUE}\tTEXT SECTION SIZE: {Fore.CYAN}{page['textsize']}")
                print(f"{Fore.BLUE}\tDATA SECTION SIZE: {Fore.CYAN}{page['datasize']}")
                print(f"{Fore.BLUE}\tGRAPH:")
                for b in page['barriers']:
                    print(f"{Fore.BLUE}\t      local barrier {Fore.CYAN}{b}: {Fore.BLUE}jobids {Fore.CYAN}{page['barriers'][b].getjobids()}")
                for launched_jobid, launcher_jobids in page['joblaunchers'].items():
                    print(f"{Fore.BLUE}\t      job launchers for {Fore.CYAN}{launched_jobid}: {Fore.BLUE}jobids {Fore.CYAN}{launcher_jobids}")
                print(f"\n{Style.RESET_ALL}")

        if len(self._symbols) > 1:
            print(f"{Fore.BLUE}SYMBOLS:{Style.RESET_ALL}")
            print(f"{Fore.BLUE}\tNAME\t\t\tCOLUMN\t\tPAGE_NUM\tOFFSET\t\tSCHEMA")
            for s in self._symbols[1:]:
                print(f"{Fore.CYAN}\t{s.name}\t\t{s.col_num}\t\t{s.page_num}\t\t{s.offset}\t\t{s.schema}")

        if self._debug != None:
            k = 1
            dmap = []
            dfile = open(self._mapfilename, 'w')
            for f in self._debug.functions:
                func = self._debug.functions[f]
                for line in func.textlines:
                    if line.linenumber == -1:
                        continue
                    entry = {}
                    entry["sno"] = k
                    entry["operation"] = line.opcode
                    entry["opcode_size"] = line.high_pc - line.low_pc + 1
                    entry["column"] = func.col_num
                    entry["page_index"] = func.page_num
                    entry["page_offset"] = line.low_pc
                    entry["line"] = line.linenumber
                    entry["file"] = os.path.realpath(func.filename)
                    dmap.append(entry)
                    k = k + 1
            dfile.write(json.dumps(dmap, indent=4))
            dfile.close()
