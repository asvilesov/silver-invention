Alexander Vilesov
Email: asvilesov@gmail.com

Project: Genetic Algorithm Applied to the Traveling Salesman Problem

Instructions:

cd build
cmake ..
make
cd ../bin
./main {file input} {population size} {generations} {mutation probability per generation} {seed}

Example:
./main ../input/locations.txt 64 500 30 12165465

Results:
You can check 'log.txt' in the /bin folder for one way to navigate through the locations in 'locations.txt' in a hopefully efficient manner.

