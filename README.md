AXIA-LUFS
=========

An EBU R128 short-term (3 sec.) loudness meter for Axia Livewire streams. I wish this was more elegant and less naïve, but at this point it's more of a proof of concept. More importantly, it works!
--------
###What is Axia Livewire?
Axia Livewire is a IP networked audio protocol used in broadcast. Among other things, it allows 32767 channels of raw multicast PCM audio distributed over a standard IP network. Axia <a href="http://axiaaudio.com/xnodes">nodes</a> are connected to the network and provide audio I/O.
###What does a Livewire audio stream look like?
Raw PCM (24-bit, 48000 kHz, big endian) delivered in the form of multicast IP/UDP/RTP packets.
###How does this work?
`rtpdump` is used to capture the audio payload from each RTP packet into a temporary file. The EBU R128 short-term (3 sec.) loudness is then computed with a small C program written to handle 24/48 big-endian raw PCM. Results are printed to stdout.
###Setup and Use
Get `rtpdump`
Get `libsndfile` and `libebur128`, and then compile livewireScan.c.
Update the location of `rtpdump` and `livewireScan` in axiaLufs.py 
Run axiaLUFS.py with the Livewire channel you'd like to meter.
```bash
$ axiaLUFS 32767
```
