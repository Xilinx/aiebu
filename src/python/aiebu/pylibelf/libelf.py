"""
 SPDX-License-Identifier: MIT

 Copyright (C) 2022 Advanced Micro Devices, Inc.
 Author(s): Sonal Santan

 ctypes based Python binding for libelf
 Enumerations and classes
"""

import errno
import ctypes
import enum

import pylibelf.elf

_libelf = ctypes.CDLL("libelf.so", mode=ctypes.RTLD_GLOBAL)

class CtypesEnum(enum.IntEnum):
    """
    A ctypes-compatible IntEnum superclass.
    https://stackoverflow.com/questions/38356698/how-to-pass-enum-as-argument-in-ctypes-python
    """
    @classmethod
    def from_param(cls, obj):
        return int(obj)

class Elf_Type(CtypesEnum):
    """ Binding for Elf_Type enumeration in libelf library """
    ELF_T_BYTE =     0
    ELF_T_ADDR =     1
    ELF_T_DYN =      2
    ELF_T_EHDR =     3
    ELF_T_HALF =     4
    ELF_T_OFF =      5
    ELF_T_PHDR =     6
    ELF_T_RELA =     7
    ELF_T_REL =      8
    ELF_T_SHDR =     9
    ELF_T_SWORD =   10
    ELF_T_SYM =     11
    ELF_T_WORD =    12
    ELF_T_XWORD =   13
    ELF_T_SXWORD =  14
    ELF_T_VDEF =    15
    ELF_T_VDAUX =   16
    ELF_T_VNEED =   17
    ELF_T_VNAUX =   18
    ELF_T_NHDR =    19
    ELF_T_SYMINFO = 20
    ELF_T_MOVE =    21
    ELF_T_LIB =     22
    ELF_T_GNUHASH = 23
    ELF_T_AUXV =    24
    ELF_T_CHDR =    25
    ELF_T_NHDR8 =   26
    ELF_T_NUM =     27

class Elf_Cmd(CtypesEnum):
    """ Binding for Elf_Cmd enumeration in libelf library """
    ELF_C_NULL =               0
    ELF_C_READ =               1
    ELF_C_RDWR =               2
    ELF_C_WRITE =              3
    ELF_C_CLR =                4
    ELF_C_SET =                5
    ELF_C_FDDONE =             6
    ELF_C_FDREAD =             7
    ELF_C_READ_MMAP =          8
    ELF_C_RDWR_MMAP =          9
    ELF_C_WRITE_MMAP =        10
    ELF_C_READ_MMAP_PRIVATE = 11
    ELF_C_EMPTY =             12
    ELF_C_NUM =               13


ELF_F_DIRTY =      0x1
ELF_F_LAYOUT =     0x4
ELF_F_PERMISSIVE = 0x8

class Elf_Kind(CtypesEnum):
    """ Binding for Elf_Kind enumeration in libelf library """
    ELF_K_NONE = 0
    ELF_K_AR   = 1
    ELF_K_COFF = 2
    ELF_K_ELF  = 3
    ELF_K_NUM  = 4


class ElfError(Exception):
    """ Convert libelf C-style error code and error string to Python exception """
    def __init__(self, *args):
        super().__init__(args)
        self.errno = _libelf.elf_errno()
        self.errmsg = _libelf.elf_errmsg(errno)

    def __str__(self):
        return self.errmsg

def _not_null_or_error(res):
    """ Validate returned pointer from libelf library """
    if (res is None):
        raise ElfError()
    return res

def _true_or_error(res):
    """ Validate return status from libelf library """
    if (res == 0):
        raise ElfError()
    return res


class Elf_Data(ctypes.Structure):
    """ Binding for Elf_Data structure in libelf """
    _fields_ = [
        ("d_buf",     ctypes.c_void_p),
        ("d_type",    ctypes.c_int),
        ("d_version", ctypes.c_uint),
        ("d_size",    ctypes.c_size_t),
        ("d_off",     ctypes.c_longlong),
        ("d_align",   ctypes.c_size_t) ]


class Elf_ScnDescriptor:
    """ Binding for Elf_Scn descriptor in libelf """
    def __init__(self, scn):
        self.scn = scn

    def elf32_getshdr(self):
        return _not_null_or_error(_libelf.elf32_getshdr(self.scn))

    def elf_getdata(self):
        return _not_null_or_error(_libelf.elf_getdata(self.scn, None))

    def elf_newdata(self):
        return _not_null_or_error(_libelf.elf_newdata(self.scn))

    def elf_ndxscn(self):
        return _libelf.elf_ndxscn(self.scn)


class ElfDescriptor:
    """ Binding for Elf descriptor in libelf """
    def _cleanup(self):
        if (self.elfnative is not None):
            _libelf.elf_end(self.elfnative)
        if (self.filehandle is not None):
            self.filehandle.close()

    def __init__(self, elfnative, filehandle = None):
        self.filehandle = filehandle
        self.elfnative = elfnative

    def __del__(self):
        self._cleanup()

    @classmethod
    def fromfile(cls, filename, cmd):
        mode = None
        if (cmd == Elf_Cmd.ELF_C_READ):
            mode = "r"
        elif (cmd == Elf_Cmd.ELF_C_WRITE):
            mode = "wb"
        elif (cmd == Elf_Cmd.ELF_C_RDWR):
            mode = "r+b"
        else:
            assert False, f"Command {cmd} not supported"

        filehandle = open(filename, mode)
        elfnative = _libelf.elf_begin(filehandle.fileno(), cmd, None)
        return cls(_not_null_or_error(elfnative), filehandle)

    @classmethod
    def frommemory(cls, image, size):
        elfnative = _libelf.elf_memory(image, size)
        return cls(_not_null_or_error(elfnative))

    def elf_kind(self):
        return _libelf.elf_kind(self.elfnative)

    def elf32_getehdr(self):
        return _not_null_or_error(_libelf.elf32_getehdr(self.elfnative))

    def elf32_newehdr(self):
        return _not_null_or_error(_libelf.elf32_newehdr(self.elfnative))

    def elf32_getphdr(self):
        return _not_null_or_error(_libelf.elf32_newphdr(self.elfnative))

    def elf32_newphdr(self, count):
        return _not_null_or_error(_libelf.elf32_newphdr(self.elfnative, count))

    def elf_flagphdr(self, cmd, flags):
        return _not_null_or_error(_libelf.elf_flagphdr(self.elfnative, cmd, flags))

    def elf_update(self, cmd):
        return _not_null_or_error(_libelf.elf_update(self.elfnative, cmd))

    def elf_getscn(self, index):
        scn = _libelf.elf_getscn(self.elfnative, index)
        return Elf_ScnDescriptor(scn) if scn is not None else scn

    def elf_nextscn(self, scn):
        if (scn is not None):
            scn = scn.scn
        nscn = _libelf.elf_nextscn(self.elfnative, scn)
        return Elf_ScnDescriptor(nscn) if nscn is not None else nscn

    def elf_newscn(self):
        scn = _libelf.elf_newscn(self.elfnative)
        return Elf_ScnDescriptor(_not_null_or_error(scn))


def elf32_fsize(typ, count, version):
    return _libelf.elf32_fsize(typ, count, version)


def _setup():
    _libelf.elf_begin.restype = ctypes.c_void_p
    _libelf.elf_begin.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_void_p]

    _libelf.elf_end.restype = ctypes.c_int
    _libelf.elf_end.argtypes = [ctypes.c_void_p]

    _libelf.elf_version.restype = ctypes.c_uint
    _libelf.elf_version.argtypes = [ctypes.c_uint]

    _libelf.elf_kind.restype = ctypes.c_uint
    _libelf.elf_kind.argtypes = [ctypes.c_void_p]

    _libelf.elf32_getehdr.restype = ctypes.POINTER(pylibelf.elf.Elf32_Ehdr)
    _libelf.elf32_getehdr.argtypes = [ctypes.c_void_p]

    _libelf.elf32_newehdr.restype = ctypes.POINTER(pylibelf.elf.Elf32_Ehdr)
    _libelf.elf32_newehdr.argtypes = [ctypes.c_void_p]

    _libelf.elf32_getphdr.restype = ctypes.POINTER(pylibelf.elf.Elf32_Phdr)
    _libelf.elf32_getphdr.argtypes = [ctypes.c_void_p]

    _libelf.elf32_newphdr.restype = ctypes.POINTER(pylibelf.elf.Elf32_Phdr)
    _libelf.elf32_newphdr.argtypes = [ctypes.c_void_p, ctypes.c_size_t]

    _libelf.elf_flagphdr.restype = ctypes.c_uint
    _libelf.elf_flagphdr.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_uint]

    _libelf.elf_update.restype = ctypes.c_int
    _libelf.elf_update.argtypes = [ctypes.c_void_p, ctypes.c_int]

    _libelf.elf32_fsize.restype = ctypes.c_size_t
    _libelf.elf32_fsize.argtypes = [ctypes.c_int, ctypes.c_size_t, ctypes.c_uint]

    _libelf.elf_getscn.restype = ctypes.c_void_p
    _libelf.elf_getscn.argtypes = [ctypes.c_void_p, ctypes.c_int]

    _libelf.elf_nextscn.restype = ctypes.c_void_p
    _libelf.elf_nextscn.argtypes = [ctypes.c_void_p, ctypes.c_void_p]

    _libelf.elf_newscn.restype = ctypes.c_void_p
    _libelf.elf_newscn.argtypes = [ctypes.c_void_p]

    _libelf.elf32_getshdr.restype = ctypes.POINTER(pylibelf.elf.Elf32_Shdr)
    _libelf.elf32_getshdr.argtypes = [ctypes.c_void_p]

    _libelf.elf_getdata.restype = ctypes.POINTER(Elf_Data)
    _libelf.elf_getdata.argtypes = [ctypes.c_void_p, ctypes.c_void_p]

    _libelf.elf_newdata.restype = ctypes.POINTER(Elf_Data)
    _libelf.elf_newdata.argtypes = [ctypes.c_void_p]

    _libelf.elf_ndxscn.restype = ctypes.c_size_t
    _libelf.elf_ndxscn.argtypes = [ctypes.c_void_p]

    _true_or_error(_libelf.elf_version(1) != pylibelf.elf.EV_NONE)


if __name__ == "pylibelf.libelf":
    _setup()
