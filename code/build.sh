cd ../build
cmake ../code
sudo touch ../build/lock.tmp
sudo make
sudo rm ../build/lock.tmp
