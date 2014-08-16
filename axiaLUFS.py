#!/usr/bin/python
import sys, os
RTPDUMP_BIN = "/usr/bin/rtpdump"
AXIALUFS_BIN = "/usr/bin/axialufs"

if len(sys.argv) != 2:
	print "Please supply a valid Livewire channel number (1 - 32767)."
	sys.exit(1)
else:
	multicastAddr = int(sys.argv[1]) + 0xEFC00000 #Axia channel number + base IP (239.192.0.0 [in hex])
	os.system(AXIALUFS_BIN + " " + TEMPFILE_LOC)