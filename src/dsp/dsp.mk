#-*-mode:makefile-gmake;indent-tabs-mode:t;tab-width:8;coding:utf-8-*-┐
#───vi: set et ft=make ts=8 tw=8 fenc=utf-8 :vi───────────────────────┘

.PHONY:		o/$(MODE)/dsp
o/$(MODE)/dsp:	o/$(MODE)/dsp/core	\
		o/$(MODE)/dsp/mpeg	\
		o/$(MODE)/dsp/scale	\
		o/$(MODE)/dsp/tty
