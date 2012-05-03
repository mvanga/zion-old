sudo losetup /dev/loop0 img/floppy.img
sudo mount /dev/loop0 /mnt
sudo cp ./zion /mnt/kernel
sudo umount /dev/loop0
sudo losetup -d /dev/loop0
