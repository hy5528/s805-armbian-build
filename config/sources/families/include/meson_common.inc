ARCH=armhf
BOOTSOURCE='https://github.com/hardkernel/u-boot.git'
BOOTBRANCH='branch:odroidc-v2011.03'
BOOTDIR='u-boot-odroidc1'
UBOOT_COMPILER="arm-linux-gnueabihf-"
UBOOT_USE_GCC='< 4.9'
BOOTSCRIPT="boot-odroid-c1.ini:boot.ini"
SERIALCON="ttyAML0"
UBOOT_TARGET_MAP=';;sd_fuse/bl1.bin.hardkernel sd_fuse/u-boot.bin'
SRC_LOADADDR='LOADADDR=0x00208000'
KERNEL_IMAGE_TYPE=uImage
LINUXFAMILY=meson
CPUMIN=504000
CPUMAX=1632000
GOVERNOR=ondemand

case $BRANCH in
	legacy)

		KERNELBRANCH="branch:linux-5.11.y"
		KERNELPATCHDIR='meson-'$BRANCH

		BUILD_DESKTOP=no

        ;;
	current)

		KERNELBRANCH="branch:linux-5.14.y"
		KERNELPATCHDIR='meson-'$BRANCH

		BUILD_DESKTOP=no

	;;
	edge)

		KERNELDIR='linux-meson'
		KERNELSOURCE='https://github.com/xdarklight/linux'
		KERNELBRANCH="branch:meson-mx-integration-5.9-20201003"
		KERNELPATCHDIR='meson-'$BRANCH

	;;
esac

if [[ $ADD_UBOOT == yes ]]; then
    UBOOT_FILES="sd_fuse/bl1.bin.hardkernel sd_fuse/u-boot.bin"

    write_uboot_platform()
    {
	dd if=$1/bl1.bin.hardkernel of=$2 bs=1 count=442 conv=fsync > /dev/null 2>&1
	dd if=$1/bl1.bin.hardkernel of=$2 bs=512 skip=1 seek=1 conv=fsync > /dev/null 2>&1
	dd if=$1/u-boot.bin of=$2 bs=512 seek=64 conv=fsync > /dev/null 2>&1
	dd if=/dev/zero of=$2 seek=1024 count=32 bs=512 conv=fsync > /dev/null 2>&1
    }
fi

family_tweaks()
{
	echo 'ttyAML0' >> $SDCARD/etc/securetty
	echo 'DAEMON_ARGS="-w 1024 --data=16"' > $SDCARD/etc/default/haveged

	cp $SRC/packages/bsp/mpv/mpv_mainline.conf $SDCARD/etc/mpv/mpv.conf

	cp $SRC/$BOARD_DIR/$SCR_ENV $SDCARD/boot/uEnv.txt

	install -m 755 $SRC/$BOARD_DIR/ddbr_backup_nand $SDCARD/usr/bin/
	install -m 755 $SRC/$BOARD_DIR/ddbr_backup_nand_full $SDCARD/usr/bin/
	install -m 755 $SRC/$BOARD_DIR/ddbr_restore_nand $SDCARD/usr/bin/
	install -m 755 $SRC/$BOARD_DIR/ddbr $SDCARD/usr/bin/

	cp $SRC/$BOARD_DIR/aml_autoscript.src $SDCARD/boot/aml_autoscript.cmd
	mkimage -C none -A arm -T script -d $SDCARD/boot/aml_autoscript.cmd $SDCARD/boot/aml_autoscript
	cp $SRC/$BOARD_DIR/aml_autoscript.zip $SDCARD/boot/aml_autoscript.zip

	cp $SRC/config/bootscripts/$SCR_BOOT $SDCARD/boot/s805_autoscript.cmd
	mkimage -C none -A arm -T script -d $SDCARD/boot/s805_autoscript.cmd $SDCARD/boot/s805_autoscript

	cp $SRC/$BOARD_DIR/boot.ini $SDCARD/boot/boot.ini

	install -m 755 $SRC/$BOARD_DIR/sound.sh $SDCARD/usr/bin/sound.sh

	echo 'Section "Device"
    Identifier  "Default Device"
    Driver      "modesetting"
    Option      "AccelMethod"    "none"     ### "glamor" to enable 3D acceleration, "none" to disable.
EndSection
Section "ServerFlags"
        Option  "AutoAddGPU" "off"
        Option "Debug" "dmabuf_capable"
EndSection
Section "OutputClass"
        Identifier "Lima"
        MatchDriver "meson"
        Driver "modesetting"
        Option "PrimaryGPU" "true"
EndSection' >> $SDCARD/etc/X11/xorg.conf.d/01-armbian-defaults.conf

}
