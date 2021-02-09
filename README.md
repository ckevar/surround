# Surround
It implements a virtual surround sound system and it was developed as part of the Component Based Softwared Design Course in the SSSUP-Pisa.

# Requirements

1. _Surround_ needs the loopback of ALSA, that's why snd-aloop module has to be enabled:

` 
$ sudo modprobe snd-aloop
`

2. Select pulseaudio's output the snd-aloop that might have the following name "_Analogue Output  - Built-in Audio_".

3. Identify the ALSA devices input devices, the loopback also counts as ALSA device:

` 
$ arecord -l
`

Note the Loopback Card number with Device number 1.

4. Pick the desired card and device, you can identify it using 

` 
$ aplay -l
`


# Compile

1. Run on terminal 

` 
$ make
`

2. launch with

` 
$ bin/main <LOOPBACK DEVICE> <PLAYBACK DEVICE>
`

i.e.:

` 
$ bin/main hw:3,1 hw:1,0
`

