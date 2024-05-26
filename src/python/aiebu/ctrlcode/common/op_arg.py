class OpArg():
    CONST = 0
    REG = 1
    PAD = 2
    JOBSIZE = 3
    BARRIER = 4
    PAGE_ID = 5

    def __init__(self, name, argtype, width=None):
        self.name = name
        self.argtype = argtype
        self.width = 8 if width is None and (argtype == OpArg.REG or argtype == OpArg.BARRIER) else width

    def __str__(self):
        return f"{self.name} {self.argtype} {self.width}"

    @staticmethod
    def fromString(s):
        if s == 'const':
            return OpArg.CONST
        elif s == 'register':
            return OpArg.REG
        elif s == 'pad':
            return OpArg.PAD
        elif s == 'jobsize':
            return OpArg.JOBSIZE
        elif s == 'barrier':
            return OpArg.BARRIER
        elif s == 'page_id':
            return OpArg.PAGE_ID
        else:
            raise RuntimeError('Invalid OpArg: {}'.format(s))
