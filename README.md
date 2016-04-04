# HybridMem
CS736 project

1. Download Gentoo 64 bit image from this site:

http://fs.devloop.org.uk/

bunzip2 Gentoo-AMD64-root_fs.bz2 

2. Make the linux-4.5 which is shared in the repo:
   make arch=x86_64
   
3. Install qemu in the sytem.

4. Boot the freshly made linux with the following command:
    qemu-system-x86_64 -hda Gentoo-AMD64-root_fs -append "root=/dev/sda/ console=ttys0 -mem=512M numa=fake=2" -m 512M -nographic
    
