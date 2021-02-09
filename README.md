# Surround
It implements a virtual surround sound system and it was developed as part of the Component Based Softwared Design Course in the SSSUP-Pisa.

# Requirements

1. _Surround_ needs the loopback of ALSA, that's why snd-aloop module has to be enabled:

` 
$ sudo modprobe snd-aloop
`

2. Select pulseaudio's output the snd-aloop that might have the following name "Analogue Output  - Built-in Audio".

