# Last modified: 25-04-2022
#
# To compile prog:
#    make
# To run all tests:
#    make t
#----------------------------------------------------------------------


bin: *.c
	cc -g -Wall -O3 basic_functions.c main_functions.c pQueue.c LinkedList.c main.c -o wrdmttns

FILES = $(shell ls probs/*/*.pals)

VALG = valgrind --leak-check=full

t:
	for F in ${FILES}; do ./wrdmttns portugues.dict $${F} ; done;