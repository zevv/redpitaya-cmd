#!/bin/sh

make && \
	./rp 192.168.1.240 $1 \
	| baudline \
		-stdin \
		-format le32f \
		-record \
		-flipcomplex \
		-samplerate 250000 \
		-quadrature \
		-channels 2
