OCAMLMAKEFILE = OCamlMakefile
ANNOTATE = yes
PACKS =
LIBS =
OCAMLLIBPATH=
INCDIRS=
LIBDIRS=
EXTLIBDIRS=

CLIBS = grib2c jasper png z m
CFLAGS = -g

# We turn on debugger support in all our modules for now.
OCAMLBCFLAGS =
OCAMLBLDFLAGS =
RESULT = grib2

SOURCES = grib2.mli grib2.ml grib2_stubs.c

# Installation settings
LIBINSTALL_FILES = *.mli *.cmi *.cma *.cmxa *.a *.so

all: byte-code-library native-code-library

opt: native-code-library

mrproper: clean
	rm -f *~ *.cmi *.cmo *.top *.so

.PHONY: mrproper

install: libinstall

uninstall: libuninstall

include $(OCAMLMAKEFILE)
