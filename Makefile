# * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
# * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
# * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
# * THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# *
# * @file Makefile
# * @author Joseph David Tarango
# * @brief Core Benchmark
# * @see https://josephtarango.com
# * @benchmark: N/A
# * @researcher: Joseph David Tarango
# * @website http://www.cs.ucr.edu/~jtarango
# * @affiliation: University of California, Riverside; University of Colorado, Boulder
# * @date: OCTOBER 27, 2020
# *
# To generate the everything, type "make all".
# To run other commands type "make <command>". I.E. "make turnin".
# To remove the executable and any extraneous files type "make clean" at
# the command prompt.
###############################################################################
# General extension information.
###############################################################################
# Linux extension    |Windows Equivalent    |Short description
#  ------------------------------------------------------------
#  .so, .o            | .dll                 | Object that can be loaded at runtime (Similar to DLL)
#  .a                 | .lib                 | Static library
#  [none], .elf(rare),| .exe, .com(rare)     | Linux executables
#    .bin(rare)       |                      |
#  .sh                | .bat                 | Shell script
#  .exe               | .exe                 | Mono application, Wine application
#  .deb               | .msi                 | Installer package for Debian/Ubuntu releases
#                     |                      |   (Though .deb is much more powerful with
#                     |                      |   native support for dependencies and repos). Note that .deb is actually a .ar archive with a special control file, a special file order, and a different extension.
#  .rpm               | .msi                 | Installer package for RedHat/CentOS releases.
#  .tar.gz, .tar, .gz | .zip                 | Compressed files that can contain a program or any other data, like images, documents, etc
#  .ko                | .sys                 | Drivers and kernel modules are loaded into the Linux kernel and have more hardware access than other programs.
#  .sh, .php, .py, etc| .bat, .cmd, .vbs,    | Linux is capable of running any file that it has an interpreter for.
#                     |   .js                |   A line at the top of the file called the shebang specifies what interpreter to run the file with.
#                     |                      |   Windows only supports .bat and .cmd files as Batch files, and .vbs (vbscript) and .js (JScript, not to be confused with JavaScript) via the Windows Script Host (WSH).
###############################################################################
# MAKEFILE_LIST
#	Contains the name of each makefile that is parsed by make, in the order in which it was parsed. The name is appended
#   just before make begins to parse the makefile. Thus, if the first thing a makefile does is examine the last word in
#   this variable, it will be the name of the current makefile. Once the current makefile has used include, however,
#   the last word will be the just-included makefile.
#   @echo $(lastword $(MAKEFILE_LIST))
###############################################################################
# GCC, GPP, LLVM Compiler information.
# Refer to:
# 	https://www.gnu.org/software/make/manual/make.html#index-ARFLAGS
#   https://gcc.gnu.org/onlinedocs/gcc/Environment-Variables.html
# CC is the compiler to use such as GCC
# LIBRARY_PATH, LD_LIBRARY_PATH
# 	The value of LIBRARY_PATH is a colon-separated list of directories, much like PATH. When configured as a native
#   compiler, GCC tries the directories thus specified when searching for special linker files, if it cannot find them
#   using GCC_EXEC_PREFIX. Linking using GCC also uses these directories when searching for ordinary libraries for the
#   -l option (but directories specified with -L come first).
# CPATH, C_INCLUDE_PATH, CPLUS_INCLUDE_PATH, OBJC_INCLUDE_PATH
#	Each variable’s value is a list of directories separated by a special character, much like PATH, in which to look
#   for header files. The special character, PATH_SEPARATOR, is target-dependent and determined at GCC build time. For
#   Microsoft Windows-based targets it is a semicolon, and for almost all other targets it is a colon.
#   CPATH specifies a list of directories to be searched as if specified with -I, but after any paths given with -I
#   options on the command line. This environment variable is used regardless of which language is being preprocessed.
#   The remaining environment variables apply only when preprocessing the particular language indicated. Each specifies
#   a list of directories to be searched as if specified with -isystem, but after any paths given with -isystem options
#   on the command line.
#   In all these variables, an empty element instructs the compiler to search its current working directory. Empty
#   elements can appear at the beginning or end of a path. For instance, if the value of CPATH is :/special/include,
#   that has the same effect as ‘-I. -I/special/include’.
# LIBS
#	Libraries for compiler
# CFLAGS
#	Extra flags to give to the C compiler.
# CPPFLAGS
# 	Extra flags to give to the C preprocessor and programs that use it (the C and Fortran compilers).
# CXXFLAGS
# 	Extra flags to give to the C++ compiler.
#
# Intel Compiler information setup.
# 	INTEL_LIB_PATH = /opt/intel/system_studio_2020/lib/intel64_lin/
# 	CPATH = ${ISS_ROOT}/compilers_and_libraries/linux/include
# 	CPATH_MORE = ${ISS_ROOT}/compilers_and_libraries/linux/daal/include
# 	DAALROOT = ${ISS_ROOT}/compilers_and_libraries/linux/daal
# 	TBBROOT = ${ISS_ROOT}/compilers_and_libraries/linux/tbb
# 	IE_COMPILER = ${ISS_ROOT}/compilers_and_libraries/linux/bin/intel64/icc
# Environment Setup
# 	source /opt/intel/system_studio_2020/bin/compilervars.sh -arch intel64 -platform linux
# GUI Location
# 	sh /opt/intel/system_studio_2020/iss_ide_eclipse-launcher.sh
# Example Compile
# 	gcc main.o calcProgram.o -L $(INTEL_LIB_PATH) -lirc -lsvml -limf -o calcProgram
# 	/permissive- /MP /GS /GA /debug:expr-source-pos /analyze /Wall /QxCOMMON-AVX512 /Gy /Zc:wchar_t /I"..\..\src\include" /I"..\..\src" /I"..\..\mpfr\dll\x64\Debug" /I"..\..\mpir\dll\x64\Debug" /I"..\..\mpfr" /Qconditional-branch:keep /Zi /O2 /Qopt-report:5 /Ob2 /Fd"x64\Release\vc141.pdb" /fp:precise /Quse-intel-optimized-headers /tune:skylake /Qunroll:1024 /fp:extended /D "NDEBUG" /D "_CONSOLE" /D "_UNICODE" /D "UNICODE" /Qipo /fp:except /Zc:forScope- /Qopt-matmul /arch:SSE2 /showIncludes /Oi /MT /QaxCOMMON-AVX512 /FC /Fa"x64\Release\" /EHsc /nologo /Fo"x64\Release\" /Qprof-dir "x64\Release\" /Ot /Fp"x64\Release\cpuBenchmark.pch"
###############################################################################
# Directory
###############################################################################
path := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
cwd  := $(shell pwd)
CURRENT_PATH := $(subst $(lastword $(notdir $(MAKEFILE_LIST))),,$(subst $(space),\$(space),$(shell realpath '$(strip $(MAKEFILE_LIST))')))

SRCDIR=$(CURRENT_PATH)src

DATADIR=$(CURRENT_PATH)data

###############################################################################
# Build Dir
###############################################################################
BDIR=$(CURRENT_PATH)builds
TARBALLDIR=$(BDIR)/package

# Test harnesses
TEST_CPP_BIN=cpuBenchmark
TEST_CPP_FILE=$(SRCDIR)/cpuBenchmark.cpp

TEST_PARALLEL_CPP_BIN=cpuBenchmarkParallel
TEST_PARALLEL_CPP_FILE=$(SRCDIR)/cpuBenchmarkParallel.cpp

# Lib Dir
LIBDIR=$(BDIR)/libs

# Objects folder
ODIR=$(BDIR)/obj

# Include Dir
IDIR=$(CURRENT_PATH)src/include
ILDIR=$(IDIR)/lib

# Plots directory
PLTDIR=$(CURRENT_PATH)src/plots

# Files such as *.h, $(error none yet.)
_DEPS = $(ILDIR)
# DEPS = $(patsubst %, -I$(IDIR)/%, -I$(_DEPS))
DEPS = -I$(IDIR) -I$(_DEPS)
# Compile order -IprogramA -IprogramA -IprogramA
INCLUDES = $(DEPS)
LIBINCLUDES = -L$(LIBDIR) -L$(IDIR)/lib
INC = $(LIBINCLUDES) $(INCLUDES)

# Detect if variables already exist
# Path additions
ifeq ($(origin LIBRARY_PATH),undefined)
    $(info LIBRARY_PATH is undefined)
else
    INC = $(INC) $(LIBRARY_PATH)
endif

ifeq ($(origin LD_LIBRARY_PATH),undefined)
    $(info LD_LIBRARY_PATH is undefined)
else
    INC = $(INC) $(LD_LIBRARY_PATH)
endif

ifeq ($(origin CPATH),undefined)
    $(info CPATH is undefined)
else
    INC = $(INC) $(CPATH)
endif

ifeq ($(origin C_INCLUDE_PATH),undefined)
    $(info C_INCLUDE_PATH is undefined)
else
    INC = $(INC) $(C_INCLUDE_PATH)
endif

ifeq ($(origin CPLUS_INCLUDE_PATH),undefined)
    $(info CPLUS_INCLUDE_PATH is undefined)
else
    INC = $(INC) $(CPLUS_INCLUDE_PATH)
endif

ifeq ($(origin CFLAGS),undefined)
    $(info CFLAGS is undefined)
else
    COMPILEFLAGS = $(COMPILEFLAGS) $(CFLAGS)
endif

ifeq ($(origin CPPFLAGS),undefined)
    $(info CPPFLAGS is undefined)
else
    COMPILEFLAGS = $(COMPILEFLAGS) $(CPPFLAGS)
endif

ifeq ($(origin CXXFLAGS),undefined)
    $(info CXXFLAGS is undefined)
else
    COMPILEFLAGS = $(COMPILEFLAGS) $(CXXFLAGS)
endif

###############################################################################
# Linking Libs
###############################################################################
ifeq ($(origin LIBS),undefined)
    $(info LIBS is undefined)
else
    LIBS = -lm $(LIBS)
endif

LDFLAGS_PTHREAD=-lpthread $(LIBS)

# Compile with no main.
NO_MAIN=-c

###############################################################################
# Help info
###############################################################################
help:
	@echo "Usage: make all"
	@echo "[compiler=compiler_name]"
	@echo "compiler_name    - intel or gnu"
	$(info Please note. To create all please do the following sequence.)
	$(info  make all)

# Variable to hold Revision number
REVNUM=1

# Variable to hold last name
LASTNAME=Tarango

# Variable to hold first first name
FIRSTNAME=Joseph

TAR_NAME_PREFIX=cpuBenchmark

# Variable of Tarball generated
TARNAME=$(TAR_NAME_PREFIX)_$(REVNUM)_$(FIRSTNAME)_$(LASTNAME).tgz

#Variable of Post Script generated
PSPRINT=$(TAR_NAME_PREFIX)_$(REVNUM)_$(FIRSTNAME)_$(LASTNAME).ps
PDFPRINT=$(TAR_NAME_PREFIX)_$(REVNUM)_$(FIRSTNAME)_$(LASTNAME).pdf

# File hiearachy is $(TAR_NAME_PREFIX)_$(REVNUM)_$(PART)$(SUBPART)$(TESTBENCH).$(EXTENSION)
# $(REVNUM)= Revision number
# $(PART)= if a lab part exists in order from 1 to infinity
# $(SUBPART)= if other components were used to make this component that were
#            not included then put them here in order from a to z. If "a"
#            requires sub parts then put "a"$(subsubpart) I.E. "a1" now
#            numbering. Continue letter/number trend to infinity.
# $(TESTBENCH)= if this is a test bench file then put "tb" to indicate it
#              is test bench for this part
#$(EXTENSION)= is the character file extension

# Files to be included in the PS/PDF print
FILES := $(wildcard $(SRCDIR)/*.h) $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*.hpp) $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*.py) Makefile

# Note do not add CSV, build, include/lib unless you want big tar packages!
# README *.h *.hpp *.cpp *.csv

# Variable of to use GCC, G++, ICC, ICPC compilers.
ifndef compiler
    compiler = gnu
    # intel
endif

# Existing compilers
GCCCOMPILER=gcc
GPPCOMPILER=g++
INTEL_C_COMPILER=icc
INTEL_C_COMPILER_MICROSOFT=icl
INTEL_CPP_COMPILER=icpc

# User choice of compiler
ifeq ($(compiler),intel)
    COMPILER=$(INTEL_CPP_COMPILER)
else
ifeq ($(compiler),gnu)
    COMPILER=$(GPPCOMPILER)
    COPTS += $(if $(filter ia32, $(_IA)), -m32, -m64)
else
    $(info Error in compiler selection.)
    $(warning Please select valid compiler.)
    COMPILER=$(GPPCOMPILER)
    COPTS += $(if $(filter ia32, $(_IA)), -m32, -m64)
    # $(error Halting make with compiler=$(compiler). You shall not pass!)
endif
endif

# GNU Options
# -g Produce debug information, necessary for debugging.
# -Wall 	Show all reasonable warnings...
# -Werror Make all warnings into errors.
# -ansi This flag tells the compiler to enforce ANSI C standards
# -pedantic More pedantic ansi, warnings for stuff you probably didn't mean.
# -O2 Optimize even more. GCC performs nearly all supported optimizations that
#  do not involve a space-speed tradeoff. As compared to -O, this option
#	increases both compilation time and the performance of the generated code.
# -O3 Optimize yet more. -O3 turns on all optimizations specified by -O2
#  and also turns on the -finline-functions, -funswitch-loops,
#  -fpredictive-commoning, -fgcse-after-reload, -ftree-vectorize and
#  -fipa-cp-clone options.
#  -ffloat-store Do not store floating point variables in registers,
#    and inhibit other options that might change whether a floating point value
#    is taken from a register or memory.
#  -fexcess-precision=standard his option allows further control over excess
#    precision on machines where floating-point registers have more precision
#    than the IEEE float and double types and the processor does not support
#    operations rounding to those types.
# Intel Options
#  -fp-model precise=Intel compiler option for floats

# Variable of GCC Compiler flags
# Verbose mode for compiler debug.
# -pedantic -ansi
# Strict language flags base on extensions for maximal portability by complying to the letter of the standard by generating warnings.
STRICT_FLAGS=-pedantic

# GCC Compile with no optimizations, debug, warnings, stack split feature, static compilation, and FP restrictions for reproducible results. For fastest -fprofile-generate then -fprofile-use
GCC_COMPILE_FLAGS_PERFORMANCE=                 -Wno-endif-labels -std=gnu18		-O3 -march=native -fsplit-stack -m64 -static -ffloat-store
GCC_COMPILE_FLAGS=                             -Wno-endif-labels -std=gnu18		-O0               -fsplit-stack -m64 -static -ffloat-store -v -g
GCC_COMPILE_FLAGS_OVERLOAD= -fprofile-generate -Wno-endif-labels -std=gnu18		-O3 -march=native -fsplit-stack -m64 -static -ffloat-store -v -g -Wall -Werror -Wfatal-errors $STRICT_FLAGS
# GPP
GPP_COMPILE_FLAGS_PERFORMANCE=                 -Wno-endif-labels -std=gnu++17	-O3 -march=native -fsplit-stack -m64 -static -ffloat-store
GPP_COMPILE_FLAGS=                             -Wno-endif-labels -std=gnu++17	-O0               -fsplit-stack -m64 -static -ffloat-store -v -g
GPP_COMPILE_FLAGS_OVERLOAD= -fprofile-generate -Wno-endif-labels -std=gnu++17	-O3 -march=native -fsplit-stack -m64 -static -ffloat-store -v -g -Wall -Werror -Wfatal-errors $STRICT_FLAGS
# Intel compiler with float precise mode and static compilation.
ICC_COMPILE_FLAGS= -g -std=c17 -fp-model=strict -static-intel
# Windows compilation flags and linker flags
ICL_COMPILE_FLAGS= /Qipo /QxHost /Qopt-report:3 /O3 /Ob2 /Oi /Ot /Wall /EHsc /MTd /GS- /Gy /arch:CORE-AVX2 /fp:strict /fp:except
ICL_LINKER_FLAGS= /qipo_facs /LARGEADDRESSAWARE /DYNAMICBASE:NO

ifeq ($(origin COMPILEFLAGS),undefined)
	ifeq ($(compiler),intel)
		COMPILEFLAGS = $(ICC_COMPILE_FLAGS)
	else
		ifeq ($(compiler),gnu_perf)
			COMPILEFLAGS = $(GPP_COMPILE_FLAGS_PERFORMANCE)
		else # ($(compiler),gnu)
			COMPILEFLAGS = $(GPP_COMPILE_FLAGS)
		endif
	endif
else
	ifeq ($(compiler),intel)
		COMPILEFLAGS = $(COMPILEFLAGS) $(ICC_COMPILE_FLAGS)
	else
		ifeq ($(compiler),gnu_perf)
			COMPILEFLAGS = $(COMPILEFLAGS) $(GPP_COMPILE_FLAGS_PERFORMANCE)
		else # ($(compiler),gnu)
			COMPILEFLAGS = $(COMPILEFLAGS) $(GPP_COMPILE_FLAGS)
		endif
	endif
endif
###############################################################################
# Exe files
###############################################################################
PYTHON_ANACONDA := ~/anaconda3/bin/python3.7
PYTHON_DEFAULT := /usr/bin/python
MD := /usr/bin/mkdir
MV := /usr/bin/mv
RM := /usr/bin/rm
TEST := /usr/bin/test
A2PS := /usr/bin/a2ps
ENSCRIPT:= /usr/bin/enscript
TR := /usr/bin/tar
# Valgrind
FPBENCH_GRIND := /usr/bin/valgrind
# Ubuntu-Linux unlimited heap and stack
UNLIMITED_POWER := ulimit -s unlimited &&
###############################################################################
# Parallel Execution
#  Query the total number of processors, then execute make in parallel to max capabilities.
###############################################################################
DIV_VALUE = 1
ZERO_VALUE = 0
NPROCS = $(shell grep -c 'processor' /proc/cpuinfo)
NPROCS_DIV := $(shell echo ${NPROCS}/${DIV_VALUE} | bc)
NPROCS_GREATEREQ_ZERO := $(shell echo ${NPROCS_DIV}\>${ZERO_VALUE} | bc)
ifeq ($(NPROCS_GREATEREQ_ZERO),0)
    NPROCS_DIV = 1
else
MAKEFLAGS += -j$(NPROCS)
endif
$(info Make runs in parallel, set to $(NPROCS), half is $(NPROCS_DIV). Comment out MAKEFLAGS is you want serial.)
$(info Make flags are: $(MAKEFLAGS))
###############################################################################
# Target
###############################################################################
.DEFAULT_GOAL := all

# Do all, except run and debugging...
all: create_dirs print printpdf turnin compile_all
	$(info Making all. Fin.)
    $(info MAKEFILE_LIST: $(MAKEFILE_LIST))
    $(info          path: $(path))
    $(info           cwd: $(cwd))
    $(info  current path: $(CURRENT_PATH))
.PHONY: all

# Create directories
create_dirs:
	$(MD) -p $(BDIR)
	$(MD) -p $(ODIR)
	$(MD) -p $(LIBDIR)
	$(MD) -p $(IDIR)
	$(MD) -p $(ILDIR)
	$(MD) -p $(PLTDIR)
	$(MD) -p $(DATADIR)
	$(MD) -p $(TARBALLDIR)
.PHONY: create_dirs

# Remove unnecessary files
clean:
	$(RM) -rf *~ $(TARBALLDIR)/*~ $(BDIR)/*.a $(ODIR)/*.o $(LIBDIR)/*.so $(IDIR)/*.so $(ILDIR)/*.so
	$(RM) -rf $(PSPRINT) $(PDFPRINT) $(TARBALLDIR)/$(TARNAME) $(TARBALLDIR)/*.pdf $(TARBALLDIR)/*.ps $(FILES_TO_CLEAN) $(FILES_TO_CLEAN_MOVED)
.PHONY: clean

########################################################################################################################
# PDF and zip file generation.
########################################################################################################################
# Print files to a .ps document
print: create_dirs
	$(A2PS) -M letter --line-numbers=1 --pro=color --highlight-level=light --pretty-print -o $(TARBALLDIR)/$(PSPRINT) $(FILES)
.PHONY: print

printpdf: create_dirs
	$(ENSCRIPT) -2 --fancy-header --line-numbers=1 --truncate-lines \
	--word-wrap --style=emacs --tabsize=3 --landscape $(FILES) \
	-o - | ps2pdfwr - $(TARBALLDIR)/$(PDFPRINT)
.PHONY: printpdf

# Create a tar ball for project
turnin: create_dirs print printpdf
	$(TR) -czvf $(TARBALLDIR)/$(TARNAME) $(TARBALLDIR)/$(PSPRINT) $(TARBALLDIR)/$(PDFPRINT) $(FILES) $(BDIR)/$(TEST_CPP_BIN).a $(BDIR)/$(TEST_PARALLEL_CPP_BIN).a
.PHONY: turnin

########################################################################################################################
# Compile major parts.
########################################################################################################################
# compile_dynamicpt
compile_all: create_dirs cpuBenchmark cpuBenchmarkParallel
	$(info Compiling all...)
.PHONY: compile_all

cpuBenchmark: create_dirs
	$(info Compile cpuBenchmark testharness)
	$(COMPILER)            $(COMPILEFLAGS) $(INC)                                   -o $(BDIR)/$(TEST_CPP_BIN).a $(TEST_CPP_FILE) $(LIBS)
.PHONY: cpuBenchmark

cpuBenchmarkParallel: create_dirs
	$(info Compile cpuBenchmark parallel testharness)
	$(COMPILER)            $(COMPILEFLAGS) $(INC)                                   -o $(BDIR)/$(TEST_PARALLEL_CPP_BIN).a $(TEST_PARALLEL_CPP_FILE) $(LDFLAGS_PTHREAD) $(LIBS)
.PHONY: cpuBenchmarkParallel

########################################################################################################################
# Execute Program
########################################################################################################################
run_cpuBenchmark: create_dirs cpuBenchmark
	$(info Run cpuBenchmark testharness)
	$(UNLIMITED_POWER) $(BDIR)/$(TEST_CPP_BIN).a
.PHONY: run_cpuBenchmark

run_cpuBenchmarkParallel: create_dirs cpuBenchmarkParallel
	$(info Run cpuBenchmarkParallel testharness)
	$(UNLIMITED_POWER) $(BDIR)/$(TEST_PARALLEL_CPP_BIN).a
.PHONY: run_cpuBenchmarkParallel

########################################################################################################################
# Performance profile execute and compile
########################################################################################################################
cpuBenchmarkFaster: create_dirs
	$(info Making cpuBenchmark program faster by -fprofile-generate -fprofile-use)
	$(COMPILER)            $(COMPILEFLAGS) $(INC)    -fprofile-generate -O3 -march=native -o $(BDIR)/$(TEST_CPP_BIN).a $(TEST_CPP_FILE) $(LIBS)
	$(UNLIMITED_POWER) $(BDIR)/$(TEST_CPP_BIN).a
	$(COMPILER)            $(COMPILEFLAGS) $(INC)    -fprofile-use      -O3 -march=native -o $(BDIR)/$(TEST_CPP_BIN).a $(TEST_CPP_FILE) $(LIBS)
	$(UNLIMITED_POWER) $(BDIR)/$(TEST_CPP_BIN).a
.PHONY: cpuBenchmarkFaster

cpuBenchmarkParallelFaster: create_dirs
	$(info Making cpuBenchmarkParallel program faster by -fprofile-generate -fprofile-use)
	$(COMPILER)            $(COMPILEFLAGS) $(INC)    -fprofile-generate -O3 -march=native -o $(BDIR)/$(TEST_PARALLEL_CPP_BIN).a $(TEST_PARALLEL_CPP_FILE) $(LIBS)
	$(UNLIMITED_POWER) $(BDIR)/$(TEST_PARALLEL_CPP_BIN).a
	$(COMPILER)            $(COMPILEFLAGS) $(INC)    -fprofile-use      -O3 -march=native -o $(BDIR)/$(TEST_PARALLEL_CPP_BIN).a $(TEST_PARALLEL_CPP_FILE) $(LIBS)
	$(UNLIMITED_POWER) $(BDIR)/$(TEST_PARALLEL_CPP_BIN).a
.PHONY: cpuBenchmarkParallelFaster

########################################################################################################################
# Debugging make file options. Only for developer usage.
########################################################################################################################
# Controlled test for debugging.
# Valgrind and debug program if we encounter issues.
VALGRIND_FLAGS=--undef-value-errors=no --main-stacksize=99999999 --max-stackframe=99999999 --valgrind-stacksize=10485760 --num-callers=500 --verbose
GRIND_LOG=Grinder.log
cpuBenchmarkGrinder: create_dirs cpuBenchmark
	$(info Debugging program log is cpuBenchmarkGrinder.log)
	$(FPBENCH_GRIND) $(VALGRIND_FLAGS) --log-file=$(BDIR)/$(TEST_CPP_BIN)$(GRIND_LOG) $(BDIR)/$(TEST_CPP_BIN).a
.PHONY: cpuBenchmarkGrinder

cpuBenchmarkParallelGrinder: create_dirs cpuBenchmarkParallel
	$(info Debugging program log is cpuBenchmarkParallelGrinder.log)
	$(FPBENCH_GRIND) $(VALGRIND_FLAGS) --log-file=$(BDIR)/$(TEST_PARALLEL_CPP_BIN)$(GRIND_LOG) $(BDIR)/$(TEST_PARALLEL_CPP_BIN).a
.PHONY: cpuBenchmarkParallelGrinder
