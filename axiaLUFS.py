#!/usr/bin/python
import sys, os
RTPDUMP_BIN = "/usr/bin/rtpdump"
AXIALUFS_BIN = "/usr/bin/axialufs"
TEMPFILE_LOC = "~/axiaDump.raw"

if len(sys.argv) != 2:
	print "Please supply a valid Livewire channel number (1 - 32767). Correct usage: xplay 32767"
	sys.exit(1)
else:
	multicastAddr = int(sys.argv[1]) + 0xEFC00000 #Axia channel number + base IP (239.192.0.0 [in hex])
	while True:
		os.system(RTPDUMP_BIN + " -F payload -t .08 " + hex(multicastAddr) + "/5004 > " + TEMPFILE_LOC)
		os.system(AXIALUFS_BIN + " " + TEMPFILE_LOC)
