#!/bin/bash
# SPDX-License-Identifier: BSD-3-Clause

function gen_test0() {
	PCAPNG_NAME="test0.pcapng"
	if [ -e $PCAPNG_NAME ]; then exit 0; fi
	# Generate the pcap
	randpkt -b 128 -c 1 -F pcapng -t tcp $PCAPNG_NAME
	# Generate the compressed versions
	bzip2 -zk9 $PCAPNG_NAME
	gzip -k9 $PCAPNG_NAME
	lz4 -zk -12 $PCAPNG_NAME
	lzma -kz9 $PCAPNG_NAME
	xz -kz9 $PCAPNG_NAME
	zstd -k -19 $PCAPNG_NAME
}


gen_test0
