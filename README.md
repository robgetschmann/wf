wf - word frequency
===================

# Overview

* The *wf* (_word frequency_) program counts the number of words in a
file or input stream.  The twenty most frequent words are displayed.

* The *wf* program takes zero or one command line arguments.  If zero
command line arguments are provided the program input is taken from the
_standard input_ stream.  If one command line argument is provided this
must be a readable file, the contents of which will be used as the input.

* Any data input must be successfully processed even if it is not ASCII
data.

* The word frequency output will be to the _standard output_ stream.

* Each record of output will be on a single line.  The first field of
the output line will be the frequency for the given word.  The second
field of the output line will be the given word.

* A word is defined as containing one or more letter from the character
set _[a-zA-Z]_.  Note that ligatures are not included for words.

* Output records will be displayed starting with the most frequent word
and terminating with the twentieth most frequent word.  If two words have
the same frequency the records will be display in reverse lexicographical
order of the word.

* Any character *not* within the character set _[a-zA-Z]_ as well as
the end of input delimits a word.

* Frequencies are determined based on case independence.  For example
"Hello" and "hello" are considered equivalent.

# Equivalent Command

The *wf* program produces equivalent output to the following _UN*X_ command
line job:

---

```shell
`#! /usr/bin/env sh

cat $1 \
    | tr -cs 'a-zA-Z' '[\n*]' \
    | grep -v '^$' \
    | tr '[:upper:]' '[:lower:]'\
    | sort \
    | uniq -c \
    | sort -nr \
    | head -20`
```

---

Usage
-----

# Example

The following demonstrates executing the *wf* program with the word
"hello" repeated three times as input with different cases.  The output
is a single record reporting that "hello" has a frequency of three within
the input stream.

---

```shell
$ echo "HELLO Hello hello" | wf
      3 hello
```

---
