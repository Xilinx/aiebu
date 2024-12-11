# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

from ctrlcode.common.section import Section
from ctrlcode.common.label import Label
from ctrlcode.common.operation import Operation
from ctrlcode.common.util import parse_num_arg
from ctrlcode.common.util import parse_barrier

class Job:
    """ Job class """
    def __init__(self, jobid, pos, index, eop_number, is_deferred):
        self._jobid = jobid
        self._start = pos
        self._end = pos
        self._start_index = index
        self._end_index = index
        self._page_num = None
        self._barrier_ids = []
        self._eop_number = eop_number
        self._is_deferred = is_deferred
        self._dependent_jobs = set()

    def getsize(self):
        return self._end - self._start

    def insertbarrier(self, lb_id):
        self._barrier_ids.append(lb_id)

    def getbarrierids(self):
        return self._barrier_ids

    def insertdependentjob(self, job_id):
        self._dependent_jobs.add(job_id)

    def getdependentjobs(self):
        return list(self._dependent_jobs)

    def setpagenum(self, page_num):
        self._page_num = page_num

    def getpagenum(self):
        return self._page_num

    def getstart(self):
        return self._start

    def getend(self):
        return self._end

    def getstartindex(self):
        return self._start_index

    def getendindex(self):
        return self._end_index

    def setend(self, end):
        self._end = end

    def setendindex(self, end_index):
        self._end_index = end_index

    def geteopnumber(self):
        return self._eop_number

    def isdeferred(self):
        return self._is_deferred

class LabelState:
    """ Label class """
    def __init__(self, name, index, pos):
        self._name = name
        self._pos = pos
        self._index = index
        self._count = -1
        self._size = 0
        self._page_num = -1

    def incrementcount(self, inc):
        self._count += inc

    def addsize(self, inc):
        self._size += inc

    def getpos(self):
        return self._pos

    def getindex(self):
        return self._index

    def getcount(self):
        return self._count

    def getsize(self):
        return self._size

    def setpagenum(self, page_num):
        self._page_num = page_num

    def getpagenum(self):
        return self._page_num

    def __str__(self):
        return f"name:{self._name} pos:{self._pos} index:{self._index} count:{self._count} size:{self._size} page_num:{self._page_num}"

class LocalBarrier:
    """ LocalBarrier class """
    def __init__(self):
        self._job_ids = []

    def insertjob(self, jobid):
        self._job_ids.append(jobid)

    def getjobids(self):
        return self._job_ids

class AssemblerState:
    """ class to hold state """
    def __init__(self, target, isa, code, scratchpads, labelpageindex, makeunique, control_packet_index, controlpacket_shimbd):
        self.target = target
        self._isa_ops = isa
        self._pos = 0
        self._labels = {}
        self._jobs = {}
        self._current_job_id = None
        self.section = Section.UNKNOWN
        self._current_label = None
        self._local_barriers = {}
        self._code = code
        self._text_size = 0
        self._data_size = 0
        self._job_launchers = {}
        self._scratchpads = scratchpads
        self.labelpageindex = labelpageindex
        self.patch = {}
        self.control_packet_index = control_packet_index
        self.controlpacket_shimbd = controlpacket_shimbd
        self.statecreator(makeunique)

    def getjobsize(self, jobid):
        return self._jobs[jobid].getsize()

    def containlabel(self, label):
        return label in self._labels

    def containscratchpads(self, label):
        return label in self._scratchpads

    def getscratchpadpos(self, label):
        return self._scratchpads[label]["base"] + self._scratchpads[label]["offset"]

    def getlabelpageindex(self, label):
        return self.labelpageindex[label]

    def getlabelpos(self, label):
        return self._labels[label].getpos()

    def getpos(self):
        return self._pos

    def setpos(self, pos):
        self._pos = pos

    def settextsize(self, size):
        self._text_size = size

    def gettextsize(self):
        return self._text_size

    def setdatasize(self, size):
        self._data_size = size

    def getdatasize(self):
        return self._data_size

    def getjoblist(self):
        return list(self._jobs.keys())

    def getlocalbarrier(self):
        return self._local_barriers

    def getjob(self, jobid):
        return self._jobs[jobid]

    def getlabel(self, labelid):
        return self._labels[labelid]

    def getlocalbarrierwithid(self, lbid):
        return self._local_barriers[lbid]

    def getjoblaunchers(self):
        return self._job_launchers

    def __str__(self):
        from pprint import pformat
        return pformat(vars(self), indent=4, width=1)

    def getlabelkeyfromtoken(self, token, makeunique=False):
        if makeunique:
            return token.filename + ":" + token.name
        return token.name

    def getlabelkey(self, label, filename, makeunique=False):
        if makeunique:
            return filename + ":" + label
        return label

    def getjobidkey(self, jobid, filename, makeunique=False):
        if makeunique:
            return filename + ":" + jobid
        return int(jobid)

    def statecreator(self, makeunique):
        """ create state for code """
        # Pass 1: Calculate sizes
        self.section = Section.TEXT
        eop_number = 0
        #last_jobid = None
        for index, data in enumerate(self._code):
            token = data.gettoken()
            #print("\t\t",token)
            if isinstance(token, Label):
                self.section = Section.DATA
                data.setsize(0)
                self._current_label = self.getlabelkeyfromtoken(token, makeunique)
                assert self._current_label not in self._labels, f"Duplicate label: {self._current_label}"
                self._labels[self._current_label] = LabelState(self._current_label, index, self._pos)

            elif isinstance(token, Operation):
                if token.name in ['start_job', 'start_job_deferred']:
                    self._current_label = None
                    job_id = self.getjobidkey(str(parse_num_arg(token.args[0], self)), token.filename, makeunique)
                    assert job_id not in self._jobs, f"Duplicate job id: {job_id}"
                    self._jobs[job_id] = Job(job_id, self._pos, index, eop_number, is_deferred=(token.name == 'start_job_deferred'))
                    self._current_job_id = job_id

                if token.name in self._isa_ops:
                    self._pos += self._isa_ops[token.name].serializer(token.args, self).size()
                    data.setsize(self._isa_ops[token.name].serializer(token.args, self).size())
                elif token.name == '.eop':
                    self._jobs['eop_'+ str(eop_number)] = Job('eop_'+ str(eop_number), self._pos, index, eop_number, is_deferred=False)
                    eop_number += 1
                else:
                    raise RuntimeError('Invalid operation: {}'.format(token.name))

                if token.name == 'local_barrier':
                    lb_id = parse_barrier(token.args[0])
                    if lb_id not in self._local_barriers:
                        self._local_barriers[lb_id] = LocalBarrier()
                    self._jobs[self._current_job_id].insertbarrier(lb_id)
                    self._local_barriers[lb_id].insertjob(self._current_job_id)

                if token.name == 'launch_job':
                    launch_job_id = self.getjobidkey(token.args[0], token.filename, makeunique)
                    self._jobs[self._current_job_id].insertdependentjob(launch_job_id)
                    if launch_job_id not in self._job_launchers:
                        self._job_launchers[launch_job_id] = []
                    self._job_launchers[launch_job_id].append(self._current_job_id)

                if token.name == 'end_job':
                    assert self._current_job_id is not None, f"Unmatched END_JOB!"
                    self._jobs[job_id].setend(self._pos)
                    self._jobs[job_id].setendindex(index)
                    self._current_job_id = None

                if token.name == 'eof':
                    self._jobs['eof'] = Job('eof', self._pos, index, eop_number, is_deferred=False)

            if not self._current_label == None and not token.name == '.align' and not token.name == '.eop':
                self._labels[self._current_label].incrementcount(1)
                self._labels[self._current_label].addsize(data.getsize())

            data.setsection(self.section)

        # Sanity checks for job launch relationships - note that this cannot be done during the loop above as a LAUNCH_JOB
        # instruction might be encountered before the corresponding START_JOB_DEFERRED
        for launched_job_id, job_launcher_ids in self._job_launchers.items():
            assert launched_job_id in self._jobs, "Job {} launched by job(s) {} not found!".format(launched_job_id, job_launcher_ids)
            assert self._jobs[launched_job_id].isdeferred(), "Job {} launched by job(s) {} is not a deferred job!".format(launched_job_id, job_launcher_ids)

        return self
