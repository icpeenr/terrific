cd build
cmake ..
sudo touch lock.tmp
sudo make
sudo rm lock.tmp
