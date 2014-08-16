AXIA-LUFS
=========
An EBU R128 loudness meter for Axia Livewire streams with momentary (400ms) and short-term (3 sec.) readouts.

###What is Axia Livewire?
Axia Livewire is an IP networked audio protocol used in broadcast. Among other things, it allows for 32767 channels of raw multicast PCM audio distributed over a standard IP network. Axia <a href="http://axiaaudio.com/xnodes">nodes</a> are connected to the network and provide audio I/O.
###What does a Livewire audio stream consist of?
Raw PCM (stereo interleaved, 24-bit, 48000 kHz, big endian) delivered in the form of multicast IP/UDP/RTP packets.
###How does this work?
`rtpdump` is used to capture the audio payload from each RTP packet, and then this data is piped to a small C program which analyzes and displays the short-term (400ms) and momentary (3 sec) loudness.
###Setup and Use
* First get `rtpdump`.
* Next get `libsndfile` and `libebur128`, and then compile axiascan.c.
* Update the location of `rtpdump` and `axiascan` in axiaLufs.py 
* Run axiaLUFS.py with the Livewire channel you'd like to meter.
```bash
$ axiaLUFS 32767
```
