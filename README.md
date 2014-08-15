AXIA-LUFS
=========
An EBU R128 short-term (3 sec.) loudness meter for Axia Livewire streams. I wish this was more elegant and less naïve, but at this point it's more of a proof of concept. More importantly, it works!

###What is Axia Livewire?
Axia Livewire is an IP networked audio protocol used in broadcast. Among other things, it allows for 32767 channels of raw multicast PCM audio distributed over a standard IP network. Axia <a href="http://axiaaudio.com/xnodes">nodes</a> are connected to the network and provide audio I/O.
###What does a Livewire audio stream consist of?
Raw PCM (stereo interleaved, 24-bit, 48000 kHz, big endian) delivered in the form of multicast IP/UDP/RTP packets.
###How does this work?
`rtpdump` is used to capture the audio payload from each RTP packet into a temporary file. The EBU R128 short-term loudness is then computed by a small C program built to handle stereo 24/48 big-endian raw PCM. Short-term loudness is printed to stdout every three seconds.
###Setup and Use
* First get `rtpdump`.
* Next get `libsndfile` and `libebur128`, and then compile livewireScan.c.
* Update the location of `rtpdump` and `livewireScan` in axiaLufs.py 
* Run axiaLUFS.py with the Livewire channel you'd like to meter.
```bash
$ axiaLUFS 32767
```
