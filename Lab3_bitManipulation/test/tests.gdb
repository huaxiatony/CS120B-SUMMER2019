# Test file for Lab3_bitManipulation


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

test "[Part-4 PINA:0x00 => PORTB: 0x00 PORTC: 0x00]"
setPINA 0x00
continue 2
expectPORTB 0x00
expectPORTC 0x00
checkResult

test "[Part-4 PINA:0x12 => PORTB: 0x01 PORTC: 0x20]"
setPINA 0x12
continue 2
expectPORTB 0x01
expectPORTC 0x20
checkResult

test "[Part-4 PINA:0x03 => PORTB: 0x00 PORTC: 0x30]"
setPINA 0x03
continue 2
expectPORTB 0x00
expectPORTC 0x30
checkResult

test "[Part-4 PINA:0xFF => PORTB: 0x0F PORTC: 0xF0]"
setPINA 0xFF
continue 2
expectPORTB 0x0F
expectPORTC 0xF0
checkResult

test "[Part-4 PINA:0x45 => PORTB: 0x04 PORTC: 0x50]"
setPINA 0x45
continue 2
expectPORTB 0x04
expectPORTC 0x50
checkResult

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
