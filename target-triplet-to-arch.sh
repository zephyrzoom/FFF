#!/bin/sh
if echo "$1" | grep -Eq 'i[[:digit:]]86-'; then # $1表示命令行第一个参数，grep匹配到才执行then
  echo i386
else
  echo "$1" | grep -Eo '^[[:alnum:]_]*'
fi