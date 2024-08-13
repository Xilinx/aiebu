# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

import math

from ctrlcode.common.section import Section
from ctrlcode.common.page import Page
from ctrlcode.common.util import union_of_lists_inorder
from ctrlcode.common.operation import Operation

class Pager:
    """" pager class """

    HEADER_SIZE = 16
    EOF_SIZE = 4
    DATA_SECTION_ALIGNMENT = 16
    ALIGNMAP = {"uc_dma_bd" : 16, "uc_dma_bd_shim" : 16, ".long": 4}
    OOO = ["load_pdi", "preemption_checkpoint"]

    def __init__(self, pagesize):
        self.pagesize = pagesize
        self.pages = []

    def extractlabels(self, token, state, content):
        """ extract all labels connected to token """
        labels = []
        if not isinstance(token, Operation):
            return labels

        for arg in token.args:
            if arg.startswith('@') and token.name not in Pager.OOO:
                label = arg[1:]   # scratchpad label is not having file scope
                if state.containscratchpads(label):
                    continue
                label = state.getlabelkey(label, token.filename, True)
                assert state.containlabel(label), f"Label not found: {label}"
                labels = union_of_lists_inorder(labels, [label])
                index = state.getlabel(label).getindex()
                # label_index[label]['count'] contain how many lines are there associated with label
                # +1 is for line with label
                for i in range(state.getlabel(label).getcount()+1):
                    labels = union_of_lists_inorder(labels, self.extractlabels(content[index+i].gettoken(), state, content))
        return labels

    def extract_externallabels(self, token, state, content):
        """ extract all external labels connected to token """
        labels = []
        if not isinstance(token, Operation):
            return labels

        for arg in token.args:
            if arg.startswith('@') and token.name in Pager.OOO:
                label = state.getlabelkey(arg[1:], token.filename, True)
                labels = union_of_lists_inorder(labels, [label])
        return labels

    def extractjobs(self, jobid, state):
        """ extract all jobs depending on jobid """
        jobids = [jobid]
        job = state.getjob(jobid)

        # Implicit dependencies by mutual barrier
        barriers = job.getbarrierids().copy()
        blen = int(len(barriers))
        eop_number = job.geteopnumber()
        i = int(0)
        while i < blen:
            lbid = barriers[i]
            for j in state.getlocalbarrierwithid(lbid).getjobids():
                if eop_number != state.getjob(j).geteopnumber():
                    raise RuntimeError(f"Job {j} having dependency with Job {jobids} are not expected"
                                        + " to be on same page because of .eop")
                barriers = union_of_lists_inorder(barriers, state.getjob(j).getbarrierids())
                blen = int(len(barriers))
            jobids = union_of_lists_inorder(jobids, state.getlocalbarrierwithid(lbid).getjobids())
            i += 1

        # Explicit dependencies (e.g. launch_job relation)
        jobids = union_of_lists_inorder(jobids, job.getdependentjobs())

        return jobids

    def assignpagenumber(self, state, col, jobs, labels, external_labels, content, text, data, page_index, pages, islastpage, page_tsize, page_dsize):
        """ assign pagenumber to all instruction and create text and data section lists """
        # sort lables according to label alignment
        labels = self.labelalignmentsorter(labels, state, content)

        for djid in jobs:
            state.getjob(djid).setpagenum(page_index)
            for j in range(state.getjob(djid).getstartindex(),
                           state.getjob(djid).getendindex()+1):
                content[j].setpagenum(page_index)
                text.append(content[j])
        for label in labels:
            state.getlabel(label).setpagenum(page_index)
            index = state.getlabel(label).getindex()
            for i in range(state.getlabel(label).getcount() + 1):
                content[index + i].setpagenum(page_index)
                data.append(content[index + i])

        # add eof at end of last page
        text.append(content[state.getjob('eof').getstartindex()])
        page_rsize = page_tsize + page_dsize + (self.datasectionaligner(page_tsize) if page_dsize else 0)+ Pager.HEADER_SIZE + Pager.EOF_SIZE
        page_rsize = (((page_rsize + 3) >> 2) << 2) # round off to next multiple of 4
        pages.append(Page(col, page_index, text, data, islastpage, page_rsize, 0, external_labels))
        if (len(pages) > 1):
            pages[len(pages) - 2].in_order_page_len = page_rsize

    def extractjobsandlabels(self, state, content, jobid, skip_jobids):
        """ extract all releated(jobs which share local barrier with current jobid) jobs
            and labels with jobid also return size of complete text section in that page """
        job = state.getjob(jobid)
        labels = []
        external_labels = []
        tsize = 0
        jobs = [x for x in self.extractjobs(jobid, state) if x not in skip_jobids]
        for djid in jobs:
            tsize += state.getjob(djid).getend()-state.getjob(djid).getstart()
            for j in range(state.getjob(djid).getstartindex(),
                           state.getjob(djid).getendindex()+1):
                token = content[j].gettoken()
                labels = union_of_lists_inorder(labels, self.extractlabels(token, state, content))
                external_labels = union_of_lists_inorder(external_labels, self.extract_externallabels(token, state, content))
        return tsize, jobs, labels, external_labels

    def labelalignmentsorter(self, clist, state, content):
        """ sort all 16 byte aligned labels together and 4 byte aligned labels together """
        labels = []
        for label in clist:
            index = state.getlabel(label).getindex()
            token = content[index + 1].gettoken()
            if Pager.ALIGNMAP[token.name] == 16:
                labels += [label]

        for label in clist:
            index = state.getlabel(label).getindex()
            token = content[index + 1].gettoken()
            if Pager.ALIGNMAP[token.name] == 4:
                labels += [label]

        assert len(labels) == len(clist), \
            f"len(labels) {len(labels)} != len(clist) {len(clist)}"
        return labels

    def getdatasectionsize(self, labels, state, content):
        """ get data section size for label list """
        dsize16 = 0
        dsize4 = 0
        for label in labels:
            token = content[state.getlabel(label).getindex() + 1].gettoken()
            if token.name in Pager.ALIGNMAP and 16 == Pager.ALIGNMAP[token.name]:
                dsize16 += state.getlabel(label).getsize()
            elif token.name in Pager.ALIGNMAP and 4 == Pager.ALIGNMAP[token.name]:
                dsize4 += state.getlabel(label).getsize()
            else:
                raise RuntimeError(f"Known Data section {token.name}")
        return dsize16 + dsize4

    def datasectionaligner(self, size):
        return (Pager.DATA_SECTION_ALIGNMENT - ((size + Pager.EOF_SIZE) % \
                Pager.DATA_SECTION_ALIGNMENT)) if ((size + Pager.EOF_SIZE) %
                Pager.DATA_SECTION_ALIGNMENT) > 0 else 0

    def pagify(self, state, col, content, relative_page_index=0):
        """ pagify the content """
        state.pos = Pager.HEADER_SIZE
        pagesize = Pager.HEADER_SIZE + Pager.EOF_SIZE
        page_index = relative_page_index
        pages = []
        data = []
        text = []
        page_dsize = 0
        page_tsize = 0
        page_jobs = []
        page_labels = []
        page_external_labels = []
        dsectionaligner = 0
        for jobid in state.getjoblist():
            job = state.getjob(jobid)
             # job already added if job have page assigned or job taken in current page or jodid is 'eof'
            if job.getpagenum() != None or jobid in page_jobs or jobid == 'eof':
                continue

            if type(jobid) == str and jobid[0:3] == 'eop':
                if len(page_jobs):
                    # conclude the page
                    self.assignpagenumber(state, col, page_jobs, page_labels, page_external_labels, content, text, data, page_index, pages, 0, page_tsize, page_dsize)

                    page_index += 1
                    data = []
                    text = []
                    page_tsize = 0
                    page_dsize = 0
                    page_jobs = []
                    page_labels = []
                    page_external_labels = []
                    dsectionaligner = 0
                continue

            # get total text section size, jobs and labels releated to jobid(current job) which are not already taken in current page
            # don't return jobs/labels for a job which is already in this page a second time (note that multiple jobs might depend on
            # the same job)
            tsize, jobs, labels, external_labels = self.extractjobsandlabels(state, content, jobid, page_jobs)

            # calculate alignment bytes needed bet text and data section
            # NOTE: data section is always 16 Byte aligned
            dsectionaligner = self.datasectionaligner(page_tsize + tsize)

            # get data section size for jobs(related to current job)
            dsize = self.getdatasectionsize(labels, state, content)

            jobdsectionaligner = self.datasectionaligner(tsize)

            #print(f"Pagnum:{page_index}\n jobs:{jobs}\n labels:{labels} \nexternal_labels:{external_labels} \ntsize {tsize} + dsize {dsize} + jobdsectionaligner{jobdsectionaligner} + eof{Pager.EOF_SIZE} + page header{Pager.HEADER_SIZE} > Pagesize {self.pagesize}")

            # this assert is to check if job can fit in one page
            assert tsize + jobdsectionaligner + dsize + Pager.EOF_SIZE + Pager.HEADER_SIZE <= self.pagesize, \
                f"tsize {tsize} + dsize {dsize} + jobdsectionaligner{jobdsectionaligner} + eof{Pager.EOF_SIZE} \
                  + page header{Pager.HEADER_SIZE} > Pagesize {self.pagesize}"

            # check if current job fit in current page along with other jobs already in page
            if page_tsize + page_dsize + tsize + dsize + dsectionaligner + Pager.EOF_SIZE + Pager.HEADER_SIZE > self.pagesize:

                # if current job dont fit in current page, conclude the page
                self.assignpagenumber(state, col, page_jobs, page_labels, page_external_labels, content, text, data, page_index, pages, 0, page_tsize, page_dsize)

                page_index += 1
                data = []
                text = []
                page_tsize = 0
                page_dsize = 0
                page_jobs = []
                page_labels = []
                page_external_labels = []
                dsectionaligner = 0

            page_tsize += tsize
            page_dsize += dsize
            page_jobs += jobs
            page_labels += labels
            page_external_labels += external_labels

        if len(page_jobs):
            self.assignpagenumber(state, col, page_jobs, page_labels, page_external_labels, content, text, data, page_index, pages, 1, page_tsize, page_dsize)
            page_index += 1
        else:
            pages[page_index-relative_page_index-1].islastpage = 1
        return page_index, pages
