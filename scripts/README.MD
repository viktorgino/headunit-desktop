## headunit-desktop build scripts ##

**recompile-pi-qemu.sh**

Recompiles headunit-desktop for the Rapsberry Pi using chroot and copies it over to a Rapsberry Pi using rsync

I suggest setting up RSA key based authentication for the Pi. For more info : https://www.raspberrypi.org/documentation/remote-access/ssh/passwordless.md 
To run this command you need to install the following packages on Debian/Ubuntu:

    qemu qemu-user-static binfmt-support

Usage example:

    sudo ./recompile-pi-qemu.sh -i ~/hu-images/2017-08-08-Raspbian-viktorginos-headunit-lite.img -c ~/headunit-desktop -r pi@raspberrypi:/opt

This command builds the contents of `~/headunit-desktop` in the image `~/hu-images/2017-08-08-Raspbian-viktorginos-headunit-lite.img` and copies it over to `pi@raspberrypi` in the `/opt` folder

	Usage: recompile-pi-qemu [options]
	  options
		 ______________________ ________________________________________________________________
		| -i | --imagefile     | Specify the .img file to use.                                  |
		|    |                 | An image file with all the prerequisites installed is expected |
		| -m | --mountpoint    | The mount point for the image. ~/rpi_mnt is used by default    |
		| -c | --copyfolder    | Compile from a local folder rather than from git               |
		| -x | --clean         | run "make clean" before compiling                              |
		| -w | --welleio       | compile with welle.io, set -x when recompiling                 |
		| -d | --dryrun        | run without compiling                                          |
		| -r | --rsync         | If set to a valid destination it will rsync the new files over |
		|    |                 | ie: pi@raspberrypi:/opt/headunit-desktop                       |
		| -h | --help          | Display this help                                              |
		 ¯¯¯¯ ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯

**compile-local.sh**

Gets copied to the target Raspbian image and gets executed using chroot. 
You can configure it by setting the following environment variables:
CLEAN           - Run `make clean` and `qmake` before building
WELLEIO         - Configure to build with welle.io included
DRYRUN          - Won't run `qmake` and `make`
COPYFOLDER      - If set we will NOT pull headunit-desktop from git