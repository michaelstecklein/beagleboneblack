# !/bin/bash
evm="root@192.168.7.2"

# create key
ssh-keygen -t rsa
cat ~/.ssh/id_rsa.pub | ssh $evm "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"
#cat ~/.ssh/id_rsa.pub | ssh $beagleboneblack "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"

# copy over files
./copyenvironmentfiles.sh

