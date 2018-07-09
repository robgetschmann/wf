#! /usr/bin/env sh

cat $1 \
    | tr -cs 'a-zA-Z' '[\n*]' \
    | grep -v '^$' \
    | tr '[:upper:]' '[:lower:]'\
    | sort \
    | uniq -c \
    | sort -nr \
    | head -20 \
    | sed -e 's/^  *//' -e 's/  */ /g'
