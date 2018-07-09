#AM_CFLAGS = -Wall -Werror -Wextra
AM_CFLAGS = -Wall

AM_LDFLAGS = -Wl,-wrap,calloc,-wrap,malloc,-wrap,realloc

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
