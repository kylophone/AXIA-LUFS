AXIA-LUFS
=========
An EBU R128 loudness meter for Axia Livewire streams with short-term (3 sec.) loudness readouts. 100ms Resolution.

###What is Axia Livewire?
Axia Livewire is an IP networked audio protocol used in broadcast. Among other things, it allows for 32767 channels of raw multicast PCM audio distributed over a standard IP network. Axia <a href="http://axiaaudio.com/xnodes">nodes</a> are connected to the network and provide audio I/O.
###What does a Livewire audio stream consist of?
Raw PCM (stereo interleaved, 24-bit, 48000 kHz, big endian) delivered in the form of multicast IP/UDP/RTP packets.
###How does this work?
Raw PCM is extracted from each packet. PCM converted to 32-bit float, and handed to libebur128 for loudness measurement.
###Setup and Use
* Get `libebur128`, and then compile axialufs.c.
###To Do
Buffer input / confirm packet sequence
