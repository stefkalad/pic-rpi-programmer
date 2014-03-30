## OpenWrt Feeds

This Git contains an OpenWrt feed. The main purpose is an in system low-voltage programmer for PIC18 Microcontroller.

The mc-icsp module was written by Eran Duchan http://www.pavius.net/2011/06/lpicp-the-embedded-linux-pic-programmer/ -
I just added a GPIO customizing module (mc-icsp-gpio-custom).

Darron Broad was so kind to adapt his k8048 programmer code and greatly optimzed the modules. He is still
active and is on the way to get more PICs working (eg PIC32). 

### OpenWrt Compiling

To use this feed just edit your feeds.conf or feeds.conf.default in the OpenWrt root dir:

<pre><code>src-git openwrtfiles git://github.com/GBert/openwrt-files</pre></code>
and do
<pre><code>scripts/feeds update
scripts/feeds install kmod-mc-icsp kmod-mc-icsp-gpio-custom picprog</pre></code>
in your OpenWrt source tree.

### Using
On the OpenWrt Router just install both modules:
<pre><code>insmod mc-icsp
insmod gpio-bb</pre></code>
and add the device:
<pre><code>mknod /dev/gpi-bb c 180 0</pre></code>
Please edit the /root/.k8048 to your settings.
### Test

<pre><code>root@OpenWrt:~# ls -l /usr/bin/k\*
root@OpenWrt:~# ls -l /usr/bin/k\*
lrwxrwxrwx    1 root     root            14 Mar 30 13:24 /usr/bin/k14 -> /usr/bin/k8048
lrwxrwxrwx    1 root     root            14 Mar 30 13:24 /usr/bin/k16 -> /usr/bin/k8048
-rwxr-xr-x    1 root     root        462152 Mar 30 13:18 /usr/bin/k8048
lrwxrwxrwx    1 root     root            17 Mar 30 13:24 /usr/bin/killall -> ../../bin/busybox
-rwxr-xr-x    1 root     root         19624 Mar 30 13:18 /usr/bin/kio
lrwxrwxrwx    1 root     root            14 Mar 30 13:24 /usr/bin/ktest -> /usr/bin/k8048

root@OpenWrt:~# k16 lvp info
root@OpenWrt:~# k16 lvp info
[000000] [PROGRAM]     8000 WORDS (0400 ROWS OF 0020 WORDS)
[200000] [IDLOCATION1] FF .
[200001] [IDLOCATION2] FF .
[200002] [IDLOCATION3] FF .
[200003] [IDLOCATION4] FF .
[200004] [IDLOCATION5] FF .
[200005] [IDLOCATION6] FF .
[200006] [IDLOCATION7] FF .
[200007] [IDLOCATION8] FF .
[300000] [CONFIG1]     125D
[300002] [CONFIG2]     7E7F
[300004] [CONFIG3]     8900
[300006] [CONFIG4]     0091
[300008] [CONFIG5]     C00F
[30000A] [CONFIG6]     E00F
[30000C] [CONFIG7]     400F
[3FFFFE] [DEVICEID]    6123 DEV:309 REV:03 PIC18F26K80
[F00000] [DATA]	       0400 BYTES
root@OpenWrt-MC-ICSP:~# k16 lvp blank
Blank device: Are you sure [y/N]? y
root@OpenWrt:~# k16 lvp programm 18f26k80\_blink.hex 
Total: 132
root@OpenWrt:~# k16 lvp verify 18f26k80\_blink.hex 
Total: 118 Pass: 118 Fail: 0
</pre></code>

### Todo

Darron Broad is doing a very good job getting more PICs integrated. The speed is impressive.
Hopefully sometime the PIC32 are supported.
 
### Thanks

Many thanks to Eran Duchan and Darron Broad for their help.
