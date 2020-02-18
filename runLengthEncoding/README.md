Alexander Vilesov
Email: asvilesov@gmail.com

Project: Implementation of Run Length Encoding

Instructions:

cd build
cmake ..
make
cd ../bin
./main {file input}

If {file input} does not have '.rl1' extension, it will be encoded.
If {file input} has a '.rl1' extension, it will be decoded.

Example:
Encoding: ./main ../data/pic.jpg
Decoding: ./main ../data/works.bmp.rl1
