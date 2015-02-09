#!/bin/sh

gcc -c linked_list.c -Iinclude
mv linked_list.o libs/
cd libs/
ar rcs liblinked_list.a linked_list.o
cd ..

