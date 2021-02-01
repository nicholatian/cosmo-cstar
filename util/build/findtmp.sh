#!/usr/bin/env bash
#-*-mode:sh;indent-tabs-mode:nil;tab-width:2;coding:utf-8-*-┐
#───vi: set net ft=sh ts=2 sts=2 fenc=utf-8 :vi─────────────┘
#
# OVERVIEW
#
#   Temporary Directory Discovery
#
# DESCRIPTION
#
#   We call this script once per build to ideally find a folder that's
#   backed by an in-memory file system. We then export it to the TMPDIR
#   environment variable. Many programs use it under the hood, e.g. gcc,
#   so it grants many small performance improvements.

MKDIR=${MKDIR:-$(command -v mkdir) -p} || exit

$MKDIR o/tmp
echo o/tmp
