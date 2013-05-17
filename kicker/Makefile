FAUST_FLAGS = -quad

kicker: kicker.dsp
	faust2alsa $(FAUST_FLAGS) $<

svg:
	faust2svg kicker.dsp && firefox ${PWD}/kicker-svg/process.svg
