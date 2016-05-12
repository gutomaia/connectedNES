BOARD_TAG    = uno
ARDUINO_LIBS = SPI Ethernet
ARDUINO_SKETCHBOOK = .

WGET = wget -q

default: Arduino.mk
	@$(MAKE) -C . build

ifeq "true" "${shell test -f Arduino.mk && echo true}"
include Arduino.mk
endif

Common.mk:
	@${WGET} https://raw.githubusercontent.com/sudar/Arduino-Makefile/master/Common.mk && \
		touch $@

Arduino.mk: Common.mk
	@${WGET} https://raw.githubusercontent.com/sudar/Arduino-Makefile/master/Arduino.mk && \
		touch $@

build: all

purge: clean
	rm Arduino.mk
	rm Common.mk
