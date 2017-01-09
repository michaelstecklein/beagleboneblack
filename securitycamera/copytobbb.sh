# !/bin/bash
evm="root@192.168.7.2"

cd ~/Projects/beagleboneblack/securitycamera/
shopt -s extglob
scp -r !(copytobbb.sh) $evm:
#scp -r * $evm:
