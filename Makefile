FAUST_PREFIX = /usr
#FAUST_PREFIX = ${HOME}/local/faust/
FAUST_BIN = $(FAUST_PREFIX)/bin
FAUST_FLAGS = -quad

LIBS = shell-env.lib shell-core.lib shell-wave.lib shell-fm.lib
TARGETS = kicker-sin-exp kicker-square-exp

all: $(TARGETS)

%: %.dsp $(LIBS)
	$(FAUST_BIN)/faust2alsa $(FAUST_FLAGS) $<

svg:
	$(FAUST_BIN)/faust2svg kicker.dsp && firefox ${PWD}/kicker-svg/process.svg
