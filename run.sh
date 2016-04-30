#!/bin/bash
FILES="/root/HybridMem/SPLASH2/fft/
/root/HybridMem/SPLASH2/ocean/
/root/HybridMem/SPLASH2/radix/
/root/HybridMem/SPLASH2/raytrace/
/root/HybridMem/SPEC2006/lbm/
/root/HybridMem/SPEC2006/mcf/
/root/HybridMem/SPEC2006/soplex/
/root/HybridMem/SPEC2006/zeusmp/
/root/HybridMem/PARSEC/canneal/
/root/HybridMem/PARSEC/dedup/
/root/HybridMem/PARSEC/fluidanimate/
root/HybridMem/PARSEC/streamcluster/"

#FILES="/root/HybridMem/SPLASH2/fft"

index=1
for F in $FILES
do
	echo "Processing $F"
	dmesg --clear
	insmod /root/HybridMem/pdram_daemon/wq_test.ko
	sleep 1
	cd $F
	echo $PWD
	numactl --membind=0 ./run.scr
	sleep 2
	rmmod wq_test
	cd ~
	dmesg > report_$index
	let "index++"
done
