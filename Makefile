NAME = gitin
VERSION = 0.1
HOMEPAGE = https://git.friedelschoen.io/web/gitin

# paths
PREFIX = /usr/local

# flags
CC 		 ?= gcc
CFLAGS 	 += -Wall -Wextra -Wpedantic -Werror -O2 -g \
		    $(shell pkg-config --cflags $(LIBS))
CPPFLAGS += -D_XOPEN_SOURCE=700 -D_GNU_SOURCE -DVERSION=\"$(VERSION)\" -DGIT_DEPRECATE_HARD
LDFLAGS  += $(shell pkg-config --libs $(LIBS))

BINS = gitin findrepos
MAN1 = gitin.1 findrepos.1
MAN5 = gitin.conf.5

DOCS = \
	assets/favicon.png \
	assets/logo.png \
	assets/style.css \
	README.md \
	LICENSE

ICONS = \
	icons/image.svg \
	icons/binary.svg \
	icons/source.svg \
	icons/text.svg \
	icons/readme.svg \
	icons/directory.svg \
	icons/other.svg \
	icons/command.svg

HEADER = \
	arg.h \
	filetypes.h \
	gitin.h

OBJECTS = \
	common.o \
	config.o \
	hprintf.o \
	getdiff.o \
	parseconfig.o \
	writearchive.o \
	writeatom.o \
	writecommit.o \
	writefiles.o \
	writehtml.o \
	writeindex.o \
	writejson.o \
	writelog.o \
	writerefs.o \
	writerepo.o

.PHONY: all clean install uninstall

# default target, make everything
all: $(BINS) $(MAN1) $(MAN5) compile_flags.txt

# automatic tagets

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

%: %.in Makefile
	sed 's/%VERSION%/$(VERSION)/g;s|%HOMEPAGE%|$(HOMEPAGE)|g' $< > $@

%: %.o
	$(CC) -o $@ $^ $(LDFLAGS)

# binary targets

gitin: LIBS = libgit2 libarchive
gitin: $(OBJECTS)

findrepos: LIBS = libgit2
findrepos: findrepos.o config.o

compile_flags.txt: LIBS = libgit2 libarchive
compile_flags.txt: Makefile
	echo $(CFLAGS) $(CPPFLAGS) | tr ' ' '\n' > $@

filetypes.h: filetypes.txt
	@echo "static const char* filetypes[][2] = {" > $@
	sed -E 's/([a-z]+) (.*)/{ "\2", "\1" },/' $< >> $@
	@echo "{0} };" >> $@

# pseudo targets

clean:
	rm -f $(BINS) $(BINS:=.o) $(OBJECTS) $(MAN1) $(MAN5) compile_flags.txt filetypes.h

install: $(BINS) $(MAN1) $(MAN5) $(DOCS) $(ICONS)
	install -d $(PREFIX)/bin
	install -m 755 $(BINS) $(PREFIX)/bin

	install -d $(PREFIX)/share/man/man1
	install -m 644 $(MAN1) $(PREFIX)/share/man/man1

	install -d $(PREFIX)/share/man/man5
	install -m 644 $(MAN5) $(PREFIX)/share/man/man5

	install -d $(PREFIX)/share/doc/$(NAME)
	install -m 644 $(DOCS) $(PREFIX)/share/doc/$(NAME)

	install -d $(PREFIX)/share/doc/$(NAME)/icons
	install -m 644 $(ICONS) $(PREFIX)/share/doc/$(NAME)/icons

uninstall:
	rm -f $(addprefix $(PREFIX)/bin/, $(BINS))
	rm -f $(addprefix $(PREFIX)/share/man/man1/, $(MAN1))
	rm -f $(addprefix $(PREFIX)/share/man/man5/, $(MAN5))
	rm -rf $(PREFIX)/share/doc/$(NAME)
