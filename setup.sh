sudo apt-get -y install sigrok autoconf libtool
git clone git://sigrok.org/libserialport
cd libserialport
./autogen.sh
./configure
make
sudo make install
cd ..
rm -r libserialport