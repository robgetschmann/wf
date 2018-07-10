AM_CFLAGS = -Wall -Werror -Wextra

AM_LDFLAGS = -Wl,-wrap,calloc,-wrap,malloc,-wrap,realloc,-wrap,strdup

CLEANFILES = \
	wf.core \
	config.log \
	core \
	cscope.files \
	cscope.in.out \
	cscope.out \
	cscope.po.out

bin_PROGRAMS = wf

wf_SOURCES = \
	count.c \
	count.h \
	heap.c \
	heap.h \
	memory.c \
	squeeze.c \
	squeeze.h \
	trie.c \
	trie.h \
	wf.c

sample-test: sample-mobydick sample-warandpeace

sample-mobydick: sample-mobydick-commandline sample-mobydick-stdin

sample-mobydick-commandline: wf
	@echo "Comparing 'Moby Dick Frequencies' - Command Line"
	@$(PWD)/wf sample/mobydick.input > /tmp/mobydick.output
	@diff -b -q sample/mobydick.output /tmp/mobydick.output
	@rm -f /tmp/mobydick.output

sample-mobydick-stdin: wf
	@echo "Comparing 'Moby Dick Frequencies' - Standard Input"
	@$(PWD)/wf < sample/mobydick.input > /tmp/mobydick.output
	@diff -b -q sample/mobydick.output /tmp/mobydick.output
	@rm -f /tmp/mobydick.output

sample-warandpeace: sample-warandpeace-commandline sample-warandpeace-stdin

sample-warandpeace-commandline: wf
	@echo "Comparing 'War and Peace Frequencies' - Command Line"
	@$(PWD)/wf sample/warandpeace.input > /tmp/warandpeace.output
	@diff -b -q sample/warandpeace.output /tmp/warandpeace.output
	@rm -f /tmp/warandpeace.output

sample-warandpeace-stdin: wf
	@echo "Comparing 'War and Peace Frequencies' - Standard Input"
	@$(PWD)/wf < sample/warandpeace.input > /tmp/warandpeace.output
	@diff -b -q sample/warandpeace.output /tmp/warandpeace.output
	@rm -f /tmp/warandpeace.output

.PHONY: sample-test
.PHONY: sample-mobydick
.PHONY: sample-warandpeace
.PHONY: sample-mobdydick-commandline sample-mobydick-stdin
.PHONY: sample-mobdydick-commandline sample-warandpeace-stdin
