"""
 SPDX-License-Identifier: MIT

 Copyright (C) 2022-2023 Advanced Micro Devices, Inc.
 Author(s): Sonal Santan

 ctypes based Python binding for elf.h
 Please see elf.h man page for definition of the enumerations and structures
"""

import ctypes

Elf32_Half = ctypes.c_ushort
Elf64_Half = ctypes.c_ushort

Elf32_Word = ctypes.c_uint
Elf64_Word = ctypes.c_uint

Elf32_Sword = ctypes.c_int
Elf64_Sword = ctypes.c_int

Elf32_Addr = ctypes.c_uint
Elf64_Addr = ctypes.c_ulonglong

Elf32_Off = ctypes.c_uint
Elf64_Off = ctypes.c_ulonglong

Elf32_Section = ctypes.c_ushort
Elf64_Section = ctypes.c_ushort

EI_NIDENT = 16

EI_MAG0 =   0
ELFMAG0 =   0x7F

EI_MAG1 =   1
ELFMAG1 =   0x45

EI_MAG2 =   2
ELFMAG2 =   0x4C

EI_MAG3 =   3
ELFMAG3 =   0x46

ELFMAG	=  "\177ELF"
SELFMAG	=  4

EI_CLASS      = 4
ELFCLASSNONE  =	0
ELFCLASS32    =	1
ELFCLASS64    =	2
ELFCLASSNUM   =	3

EI_DATA       = 5
ELFDATANONE   =	0
ELFDATA2LSB   =	1
ELFDATA2MSB   =	2
ELFDATANUM    =	3

EI_VERSION    = 6

EI_OSABI             =  7

ELFOSABI_NONE        =	0
ELFOSABI_SYSV	     =	0
ELFOSABI_HPUX	     =	1
ELFOSABI_NETBSD	     =	2
ELFOSABI_GNU	     =	3
ELFOSABI_LINUX	     =	ELFOSABI_GNU
ELFOSABI_SOLARIS     =	6
ELFOSABI_AIX	     =	7
ELFOSABI_IRIX	     =	8
ELFOSABI_FREEBSD     =	9
ELFOSABI_TRU64	     =	10
ELFOSABI_MODESTO     =	11
ELFOSABI_OPENBSD     =	12
ELFOSABI_ARM_AEABI   =	64
ELFOSABI_ARM	     =	97
ELFOSABI_STANDALONE  =	255

EI_ABIVERSION        =  8

EI_PAD               =  9

ET_NONE   = 0
ET_REL    = 1
ET_EXEC   = 2
ET_DYN    = 3
ET_CORE   = 4
ET_NUM	  = 5
ET_LOOS	  = 0xfe00
ET_HIOS	  = 0xfeff
ET_LOPROC = 0xFF00
ET_HIPROC = 0xFFFF


EM_NONE =        0
EM_M32 =         1
EM_SPARC =       2
EM_386 =         3
EM_68K =         4
EM_88K =         5
EM_IAMCU =       6
EM_860 =         7
EM_MIPS =        8
EM_S370 =        9
EM_MIPS_RS3_LE = 10

EM_PARISC =      15

EM_VPP500 =      17
EM_SPARC32PLUS = 18
EM_960 =         19
EM_PPC =         20
EM_PPC64 =       21
EM_S390 =        22
EM_SPU =         23

EM_V800 = 36
EM_FR20 = 37
EM_RH32 = 38
EM_RCE = 39
EM_ARM = 40
EM_FAKE_ALPHA = 41
EM_SH = 42
EM_SPARCV9 = 43
EM_TRICORE = 44
EM_ARC = 45
EM_H8_300 = 46
EM_H8_300H = 47
EM_H8S = 48
EM_H8_500 = 49
EM_IA_64 = 50
EM_MIPS_X = 51
EM_COLDFIRE = 52
EM_68HC12 = 53
EM_MMA = 54
EM_PCP = 55
EM_NCPU = 56
EM_NDR1 = 57
EM_STARCORE = 58
EM_ME16 = 59
EM_ST100 = 60
EM_TINYJ = 61
EM_X86_64 = 62
EM_PDSP = 63
EM_PDP10 = 64
EM_PDP11 = 65
EM_FX66 = 66
EM_ST9PLUS = 67
EM_ST7 = 68
EM_68HC16 = 69
EM_68HC11 = 70
EM_68HC08 = 71
EM_68HC05 = 72
EM_SVX = 73
EM_ST19 = 74
EM_VAX = 75
EM_CRIS = 76
EM_JAVELIN = 77
EM_FIREPATH = 78
EM_ZSP = 79
EM_MMIX = 80
EM_HUANY = 81
EM_PRISM = 82
EM_AVR = 83
EM_FR30 = 84
EM_D10V = 85
EM_D30V = 86
EM_V850 = 87
EM_M32R = 88
EM_MN10300 = 89
EM_MN10200 = 90
EM_PJ = 91
EM_OPENRISC = 92
EM_ARC_COMPACT = 93
EM_XTENSA = 94
EM_VIDEOCORE = 95
EM_TMM_GPP = 96
EM_NS32K = 97
EM_TPC = 98
EM_SNP1K = 99
EM_ST200 = 100
EM_IP2K = 101
EM_MAX = 102
EM_CR = 103
EM_F2MC16 = 104
EM_MSP430 = 105
EM_BLACKFIN = 106
EM_SE_C33 = 107
EM_SEP = 108
EM_ARCA = 109
EM_UNICORE = 110
EM_EXCESS = 111
EM_DXP = 112
EM_ALTERA_NIOS2 = 113
EM_CRX = 114
EM_XGATE = 115
EM_C166 = 116
EM_M16C = 117
EM_DSPIC30F = 118
EM_CE = 119
EM_M32C = 120

EM_TSK3000 = 131
EM_RS08 = 132
EM_SHARC = 133
EM_ECOG2 = 134
EM_SCORE7 = 135
EM_DSP24 = 136
EM_VIDEOCORE3 = 137
EM_LATTICEMICO32 = 138
EM_SE_C17 = 139
EM_TI_C6000 = 140
EM_TI_C2000 = 141
EM_TI_C5500 = 142
EM_TI_ARP32 = 143
EM_TI_PRU = 144

EM_MMDSP_PLUS = 160
EM_CYPRESS_M8C = 161
EM_R32C = 162
EM_TRIMEDIA = 163
EM_QDSP6 = 164
EM_8051 = 165
EM_STXP7X = 166
EM_NDS32 = 167
EM_ECOG1X = 168
EM_MAXQ30 = 169
EM_XIMO16 = 170
EM_MANIK = 171
EM_CRAYNV2 = 172
EM_RX = 173
EM_METAG = 174
EM_MCST_ELBRUS = 175
EM_ECOG16 = 176
EM_CR16 = 177
EM_ETPU = 178
EM_SLE9X = 179
EM_L10M = 180
EM_K10M = 181

EM_AARCH64 = 183

EM_AVR32 = 185
EM_STM8 = 186
EM_TILE64 = 187
EM_TILEPRO = 188
EM_MICROBLAZE = 189
EM_CUDA = 190
EM_TILEGX = 191
EM_CLOUDSHIELD = 192
EM_COREA_1ST = 193
EM_COREA_2ND = 194
EM_ARCV2 = 195
EM_OPEN8 = 196
EM_RL78 = 197
EM_VIDEOCORE5 = 198
EM_78KOR = 199
EM_56800EX = 200
EM_BA1 = 201
EM_BA2 = 202
EM_XCORE = 203
EM_MCHP_PIC = 204
EM_INTELGT = 205

EM_KM32 = 210
EM_KMX32 = 211
EM_EMX16 = 212
EM_EMX8 = 213
EM_KVARC = 214
EM_CDP = 215
EM_COGE = 216
EM_COOL = 217
EM_NORC = 218
EM_CSR_KALIMBA = 219
EM_Z80 = 220
EM_VISIUM = 221
EM_FT32 = 222
EM_MOXIE = 223
EM_AMDGPU = 224

EM_RISCV = 243

EM_BPF = 247
EM_CSKY = 252

EM_NUM = 253

EV_NONE       = 0
EV_CURRENT    = 1
EV_NUM        = 2

PT_NULL         = 0
PT_LOAD         = 1
PT_DYNAMIC      = 2
PT_INTERP       = 3
PT_NOTE         = 4
PT_SHLIB        = 5
PT_PHDR         = 6
PT_TLS          = 7
PT_NUM          = 8
PT_LOOS         = 0x60000000
PT_GNU_EH_FRAME = 0x6474e550
PT_GNU_STACK    = 0x6474e551
PT_GNU_RELRO    = 0x6474e552
PT_GNU_PROPERTY = 0x6474e553
PT_LOSUNW       = 0x6ffffffa
PT_SUNWBSS      = 0x6ffffffa
PT_SUNWSTACK    = 0x6ffffffb
PT_HISUNW       = 0x6fffffff
PT_HIOS         = 0x6fffffff
PT_LOPROC       = 0x70000000
PT_HIPROC       = 0x7fffffff

PF_X		= (1 << 0)
PF_W		= (1 << 1)
PF_R		= (1 << 2)
PF_MASKOS	= 0x0ff00000
PF_MASKPROC	= 0xf0000000

SHN_UNDEF	= 0
SHN_LORESERVE	= 0xff00
SHN_LOPROC	= 0xff00
SHN_BEFORE	= 0xff00

SHN_AFTER	= 0xff01

SHN_HIPROC	= 0xff1f
SHN_LOOS	= 0xff20
SHN_HIOS	= 0xff3f
SHN_ABS		= 0xfff1
SHN_COMMON	= 0xfff2
SHN_XINDEX	= 0xffff


SHT_NULL =   0
SHT_PROGBITS =   1
SHT_SYMTAB =   2
SHT_STRTAB =   3
SHT_RELA =   4
SHT_HASH =   5
SHT_DYNAMIC =   6
SHT_NOTE =   7
SHT_NOBITS =   8
SHT_REL =     9
SHT_SHLIB =   10
SHT_DYNSYM =   11
SHT_INIT_ARRAY =   14
SHT_FINI_ARRAY =   15
SHT_PREINIT_ARRAY = 16
SHT_GROUP =   17
SHT_SYMTAB_SHNDX  = 18
SHT_NUM =    19
SHT_LOOS =   0x60000000
SHT_GNU_ATTRIBUTES = 0x6ffffff5
SHT_GNU_HASH =   0x6ffffff6
SHT_GNU_LIBLIST =   0x6ffffff7
SHT_CHECKSUM =   0x6ffffff8
SHT_LOSUNW =   0x6ffffffa
SHT_SUNW_move =   0x6ffffffa
SHT_SUNW_COMDAT =   0x6ffffffb
SHT_SUNW_syminfo = 0x6ffffffc
SHT_GNU_verdef =   0x6ffffffd
SHT_GNU_verneed =   0x6ffffffe
SHT_GNU_versym =   0x6fffffff
SHT_HISUNW =   0x6fffffff
SHT_HIOS =   0x6fffffff
SHT_LOPROC =   0x70000000
SHT_HIPROC =   0x7fffffff
SHT_LOUSER =   0x80000000
SHT_HIUSER =   0x8fffffff

SHF_WRITE =            (1 << 0)
SHF_ALLOC =            (1 << 1)
SHF_EXECINSTR =        (1 << 2)
SHF_MERGE =            (1 << 4)
SHF_STRINGS =          (1 << 5)
SHF_INFO_LINK =        (1 << 6)
SHF_LINK_ORDER =       (1 << 7)
SHF_OS_NONCONFORMING = (1 << 8)

SHF_GROUP         = (1 << 9)
SHF_TLS           = (1 << 10)
SHF_COMPRESSED    = (1 << 11)
SHF_MASKOS        = 0x0ff00000
SHF_MASKPROC      = 0xf0000000
SHF_GNU_RETAIN    = (1 << 21)
SHF_ORDERED       = (1 << 30)

#SHF_EXCLUDE       = (1U << 31)

STB_LOCAL         = 0
STB_GLOBAL        = 1
STB_WEAK          = 2
STB_LOOS          = 10
STB_GNU_UNIQUE    = 10
STB_HIOS          = 12
STB_LOPROC        = 13
STB_HIPROC        = 15

STT_NOTYPE        = 0
STT_OBJECT        = 1
STT_FUNC          = 2
STT_SECTION       = 3
STT_FILE          = 4
STT_COMMON        = 5
STT_TLS           = 6
STT_RELC          = 8
STT_SRELC         = 9
STT_LOOS          = 10
STT_GNU_IFUNC     = 10
STT_HIOS          = 12
STT_LOPROC        = 13
STT_HIPROC        = 15

R_386_NONE        = 0
R_386_32          = 1
R_386_PC32        = 2
R_386_GOT32       = 3
R_386_PLT32       = 4
R_386_COPY        = 5
R_386_GLOB_DAT    = 6
R_386_JUMP_SLOT   = 7
R_386_RELATIVE    = 8
R_386_GOTOFF      = 9
R_386_GOTPC       = 10

DT_NULL             = 0
DT_NEEDED           = 1
DT_PLTRELSZ         = 2
DT_PLTGOT           = 3
DT_HASH             = 4
DT_STRTAB           = 5
DT_SYMTAB           = 6
DT_RELA             = 7
DT_RELASZ           = 8
DT_RELAENT          = 9
DT_STRSZ            = 10
DT_SYMENT           = 11
DT_INIT             = 12
DT_FINI             = 13
DT_SONAME           = 14
DT_RPATH            = 15
DT_SYMBOLIC         = 16
DT_REL              = 17
DT_RELSZ            = 18
DT_RELENT           = 19
DT_PLTREL           = 20
DT_DEBUG            = 21
DT_TEXTREL          = 22
DT_JMPREL           = 23
DT_BIND_NOW         = 24
DT_INIT_ARRAY       = 25
DT_FINI_ARRAY       = 26
DT_INIT_ARRAYSZ     = 27
DT_FINI_ARRAYSZ     = 28
DT_RUNPATH          = 29
DT_FLAGS            = 30
DT_PREINIT_ARRAY    = 32
DT_PREINIT_ARRAYSZ  = 33
DT_SYMTAB_SHNDX     = 34
DT_RELRSZ           = 35
DT_RELR             = 36
DT_RELRENT          = 37
DT_ENCODING         = 38

class Elf32_Ehdr(ctypes.Structure):
    """ Python binding for ELF struct Elf32_Ehdr """
    _fields_ = [
        ("e_ident",     ctypes.c_ubyte * EI_NIDENT),
        ("e_type",      Elf32_Half),
        ("e_machine",   Elf32_Half),
        ("e_version",   Elf32_Word),
        ("e_entry",     Elf32_Addr),
        ("e_phoff",     Elf32_Off),
        ("e_shoff",     Elf32_Off),
        ("e_flags",     Elf32_Word),
        ("e_ehsize",    Elf32_Half),
        ("e_phentsize", Elf32_Half),
        ("e_phnum",     Elf32_Half),
        ("e_shentsize", Elf32_Half),
        ("e_shnum",     Elf32_Half),
        ("e_shstrndx",  Elf32_Half) ]

class Elf32_Phdr(ctypes.Structure):
    """ Python binding for ELF struct Elf32_Phdr """
    _fields_ = [
        ("p_type",   Elf32_Word),
        ("p_offset", Elf32_Off),
        ("p_vaddr",  Elf32_Addr),
        ("p_paddr",  Elf32_Addr),
        ("p_filesz", Elf32_Word),
        ("p_memsz",  Elf32_Word),
        ("p_flags",  Elf32_Word),
        ("p_align",  Elf32_Word) ]


class Elf32_Shdr(ctypes.Structure):
    """ Python binding for ELF struct Elf32_Shdr """
    _fields_ = [
        ("sh_name",      Elf32_Word),
        ("sh_type",      Elf32_Word),
        ("sh_flags",     Elf32_Word),
        ("sh_addr",      Elf32_Addr),
        ("sh_offset",    Elf32_Off),
        ("sh_size",      Elf32_Word),
        ("sh_link",      Elf32_Word),
        ("sh_info",      Elf32_Word),
        ("sh_addralign", Elf32_Word),
        ("sh_entsize",   Elf32_Word) ]


class Elf32_Rela(ctypes.Structure):
    """ Python binding for ELF struct Elf32_Rela """
    _fields_ = [
        ("r_offset", Elf32_Addr),
        ("r_info",   Elf32_Word),
        ("r_addend", Elf32_Sword) ]


def ELF32_R_SYM(val):
    return (val >> 8)

def ELF32_R_TYPE(val):
    return (val & 0xff)

def ELF32_R_INFO(rsym, rtype):
    return ((rsym << 8) + (rtype & 0xff))


class _d_un(ctypes.Union):
    """ Python binding for ELF struct Elf32_Dyn::d_un """
    _fields_ = [("d_val", Elf32_Word),
                ("d_ptr", Elf32_Addr)]


class Elf32_Dyn(ctypes.Structure):
    """ Python binding for ELF struct Elf32_Dyn """
    _fields_ = [
        ("d_tag", Elf32_Sword),
        ("d_un", _d_un)]


class Elf32_Sym(ctypes.Structure):
    """ Python binding for ELF struct Elf32_Sym """
    _fields_ = [
        ("st_name",  Elf32_Word),
        ("st_value", Elf32_Addr),
        ("st_size",  Elf32_Word),
        ("st_info",  ctypes.c_ubyte),
        ("st_other", ctypes.c_ubyte),
        ("st_shndx", Elf32_Section)]

def ELF32_ST_BIND(val):
    return (((ctypes.c_ubyte)(val)) >> 4)

def ELF32_ST_TYPE(val):
    return (val & 0xf)

def ELF32_ST_INFO(sbind, stype):
    return ((sbind << 4) + (stype & 0xf))


R_M32R_NONE               = 0
R_M32R_16                 = 1
R_M32R_32                 = 2
R_M32R_24                 = 3
R_M32R_10_PCREL           = 4
R_M32R_18_PCREL           = 5
R_M32R_26_PCREL           = 6
R_M32R_HI16_ULO           = 7
R_M32R_HI16_SLO           = 8
R_M32R_LO16               = 9
R_M32R_SDA16              = 10
R_M32R_GNU_VTINHERIT      = 11
R_M32R_GNU_VTENTRY        = 12

R_M32R_16_RELA            = 33
R_M32R_32_RELA            = 34
R_M32R_24_RELA            = 35
R_M32R_10_PCREL_RELA      = 36
R_M32R_18_PCREL_RELA      = 37
R_M32R_26_PCREL_RELA      = 38
R_M32R_HI16_ULO_RELA      = 39
R_M32R_HI16_SLO_RELA      = 40
R_M32R_LO16_RELA          = 41
R_M32R_SDA16_RELA         = 42
R_M32R_RELA_GNU_VTINHERIT = 43
R_M32R_RELA_GNU_VTENTRY   = 44
R_M32R_REL32              = 45
R_M32R_GOT24              = 48
R_M32R_26_PLTREL          = 49
R_M32R_COPY               = 50
R_M32R_GLOB_DAT           = 51
R_M32R_JMP_SLOT           = 52
R_M32R_RELATIVE           = 53
R_M32R_GOTOFF             = 54
R_M32R_GOTPC24            = 55
R_M32R_GOT16_HI_ULO       = 56
R_M32R_GOT16_HI_SLO       = 57
R_M32R_GOT16_LO           = 58
R_M32R_GOTPC_HI_ULO       = 59
R_M32R_GOTPC_HI_SLO       = 60
R_M32R_GOTPC_LO           = 61
R_M32R_GOTOFF_HI_ULO      = 62
R_M32R_GOTOFF_HI_SLO      = 63
R_M32R_GOTOFF_LO          = 64
R_M32R_NUM                = 256
