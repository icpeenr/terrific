cd build
cmake clean ..
sudo touch lock.tmp
sudo make
sudo rm lock.tmp
