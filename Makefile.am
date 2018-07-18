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

sample-test: \
	sample-mobydick \
	sample-warandpeace \
	sample-bsd-ls \
	sample-large \
	sample-lexiographic-1 \
	sample-lexiographic-2 \
	sample-null \
	sample-single

sample-mobydick: wf
	@echo "Comparing 'Moby Dick' Frequencies"
	@$(PWD)/wf sample/mobydick.input > /tmp/mobydick.output
	@diff -w -q sample/mobydick.output /tmp/mobydick.output
	@rm -f /tmp/mobydick.output

sample-warandpeace: wf
	@echo "Comparing 'War and Peace' Frequencies"
	@$(PWD)/wf sample/warandpeace.input > /tmp/warandpeace.output
	@diff -w -q sample/warandpeace.output /tmp/warandpeace.output
	@rm -f /tmp/warandpeace.output

sample-bsd-ls: wf
	@echo "Comparing BSD ls Binary Frequencies"
	@$(PWD)/wf sample/bsd-ls.input > /tmp/bsd-ls.output
	@diff -w -q sample/bsd-ls.output /tmp/bsd-ls.output
	@rm -f /tmp/bsd-ls.output

sample-large: wf
	@echo "Comparing large Frequencies"
	@$(PWD)/wf sample/large.input > /tmp/large.output
	@diff -w -q sample/large.output /tmp/large.output
	@rm -f /tmp/large.output

sample-lexiographic-1: wf
	@echo "Comparing lexiographic1 Frequencies"
	@$(PWD)/wf sample/lexiographic-1.input > /tmp/lexiographic-1.output
	@diff -w -q sample/lexiographic-1.output /tmp/lexiographic-1.output
	@rm -f /tmp/lexiographic-1.output

sample-lexiographic-2: wf
	@echo "Comparing lexiographic2 Frequencies"
	@$(PWD)/wf sample/lexiographic-2.input > /tmp/lexiographic-2.output
	@diff -w -q sample/lexiographic-2.output /tmp/lexiographic-2.output
	@rm -f /tmp/lexiographic-2.output

sample-null: wf
	@echo "Comparing null Frequencies"
	@$(PWD)/wf sample/null.input > /tmp/null.output
	@diff -w -q sample/null.output /tmp/null.output
	@rm -f /tmp/null.output

sample-single: wf
	@echo "Comparing single Frequencies"
	@$(PWD)/wf sample/single.input > /tmp/single.output
	@diff -w -q sample/single.output /tmp/single.output
	@rm -f /tmp/single.output

.PHONY: sample-test
.PHONY: sample-mobydick
.PHONY: sample-warandpeace
.PHONY: sample-lexiographic-1
