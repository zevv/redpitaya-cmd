#!/bin/sh

make && \
	./rp 192.168.1.236 $1 \
	| baudline \
		-stdin \
		-format le32f \
		-record \
		-samplerate 2500000 \
		-quadrature \
		-channels 2
