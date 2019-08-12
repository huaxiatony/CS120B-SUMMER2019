# EE/<del>CS</del> 120B
<del>As we all known there is no CS classes offered in UCR.</del>
I will keep this repo public unless you guys get caught for plagiarism. 
### Every computer science student is consider as an electrical engineer.

So here is the brief introduce for this class. 
First beside purchsing a expensive textbook. 
We need to purchase some toy to let you suffering the whole quarter. (And you have to paying for you pain!)
Yes! This is 120B! A class for weebs. 

### You will type these command everyday! Trust me copy it down!
## Recommand all Windows user use:
ssh -v -X YOUR_NET_ID@bolt.cs.ucr.edu
ssh -v -X wch136-18

## This is how you create your first workspace
/usr/csshare/pkgs/cs120b-avrtools/createProject.sh

## EXAMPLE_FOR_LAB NAME
Lab2_introToAVR
Lab3_bitManipulation

avrdude -c atmelice_isp -p atmega1284 -U hfuse:w:0xD9:m

## This code is very helpful after lab4. Because you will have errors about not running as a test case

avr-gcc -mmcu=atmega1284 -I/usr/csshare/src/simavr/ -Iheader/ -D_SIMULATE_ -Wall -Wl,--undefined=_mmcu,--section-start=.mmcu=910000 -o build/objects/main.elf source/main.c && make program

avr-gcc -mmcu=atmega1284 -Wall -o build/objects/main.elf source/main.c

## Homework Submission Format
cp source/main.c turnin/YOUR_NET_ID_lab11_part1.c
cp test/tests.gdb turnin/YOUR_NET_ID_lab11_part1_tests.gdb

cp source/main.c turnin/YOUR_NET_ID_lab11_part2.c
cp test/tests.gdb turnin/YOUR_NET_ID_lab11_part2_tests.gdb

cp source/main.c turnin/YOUR_NET_ID_lab11_part3.c
cp test/tests.gdb turnin/YOUR_NET_ID_lab11_part3_tests.gdb

cp source/main.c turnin/YOUR_NET_ID_lab11_part4.c
cp test/tests.gdb turnin/YOUR_NET_ID_lab11_part4_tests.gdb

tar -czvf YOUR_NET_ID_lab11.tgz turnin/
