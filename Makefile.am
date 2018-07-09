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

sample-mobydick: wf
	$(PWD)/wf sample/mobydick.input > /tmp/mobydick.output
	diff -b -q sample/mobydick.output /tmp/mobydick.output
	rm -f /tmp/mobydick.output

sample-warandpeace: wf
	$(PWD)/wf sample/warandpeace.input > /tmp/warandpeace.output
	diff -b -q sample/warandpeace.output /tmp/warandpeace.output
	rm -f /tmp/warandpeace.output

.PHONY: sample-test sample-mobdydick sample-warandpeace
