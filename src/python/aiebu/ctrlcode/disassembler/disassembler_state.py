class DisAssemblerState:
    """ hold dissassembler state """
    def __init__(self):
        self.pos = 0
        self.labels = {}
        self.local_ptrs = {}

    def __str__(self):
        return f"[POS:{self.pos}\tLABEL:{self.labels}\tlocal_ptrs:{self.local_ptrs}"

    def reset(self):
        self.pos = 0
        self.labels = {}
        self.job_offsets = {}
        self.current_job_id = None
        self.local_ptrs = {}
