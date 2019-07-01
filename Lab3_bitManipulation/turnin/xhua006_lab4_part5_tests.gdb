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

test "PINA: 0x00, 0x00, 0x00, 0x00 => PORTC: 0x01, PORTB: 0x00 state: INIT"
set state = INIT
setPINA 0x00
continue 2
setPINA 0x00
continue 2
setPINA 0x00
continue 2
setPINA 0x00
continue 2
#expectPORTC 0x01
expectPORTB 0x00
expect state INIT
checkResult

test "PINA: 0x00, 0x00, 0x00, 0x04 => PORTC: 0x03, PORTB: 0x00 state: WAIT"
set state = INIT
setPINA 0x00
continue 2
setPINA 0x00
continue 2
setPINA 0x00
continue 2
setPINA 0x04
continue 2
#expectPORTC 0x03
expectPORTB 0x00
expect state WAIT
checkResult

test "PINA: 0x00, 0x00, 0x80, 0x80 => PORTC: 0x06, PORTB: 0x00 state: INIT"
set state = INIT
setPINA 0x00
continue 2
setPINA 0x00
continue 2
setPINA 0x80
continue 2
setPINA 0x80
continue 2
#expectPORTC 0x06
expectPORTB 0x00
expect state INIT
checkResult

test "PINA: 0x00, 0x00, 0x04, 0x02 => PORTC: 0x04, PORTB: 0x01 state: PAUSE"
set state = INIT
setPINA 0x00
continue 2
setPINA 0x00
continue 2
setPINA 0x04
continue 2
setPINA 0x02
continue 2
#expectPORTC 0x04
expectPORTB 0x01
expect state PAUSE
checkResult

test "PINA: 0x00, 0x04, 0x02, 0x80 => PORTC: 0x01, PORTB: 0x00 state: LOCK"
set state = INIT
setPINA 0x00
continue 2
setPINA 0x04
continue 2
setPINA 0x02
continue 2
setPINA 0x80
continue 2
#expectPORTC 0x01
expectPORTB 0x00
expect state LOCK
checkResult

test "PINA: 0x04, 0x02, 0x08, 0x04 => PORTC: 0x01, PORTB: 0x00 state: KEY_POUND"
set state = INIT
setPINA 0x04
continue 2
setPINA 0x02
continue 2
setPINA 0x08
continue 2
setPINA 0x04
continue 2
#expectPORTC 0x01
expectPORTB 0x00
expect state KEY_POUND
checkResult

test "PINA: 0x04, 0x01, 0x02, 0x04 => PORTC: 0x03, PORTB: 0x00 state: WAIT "
set state = INIT
setPINA 0x04
continue 2
setPINA 0x01
continue 2
setPINA 0x02
continue 2
setPINA 0x04
continue 2
#expectPORTC 0x03
expectPORTB 0x00
expect state WAIT
checkResult

test "PINA: 0x04, 0x02, 0x04, 0x00 => PORTB: 0x00 state: WAIT"
set state = INIT
setPINA 0x04
continue 2
setPINA 0x02
continue 2
setPINA 0x04
continue 2
setPINA 0x00
continue 2
#expectPORTC 0x02
expectPORTB 0x00
expect state WAIT 
checkResult

test "PINA: 0x04, 0x02, 0x04, 0x02 => PORTB: 0x01 state: KEY_Y"
set state = INIT
setPINA 0x04
continue 2
setPINA 0x02
continue 2
setPINA 0x04
continue 2
setPINA 0x02
continue 2
#expectPORTC 0x02
expectPORTB 0x00
expect state KEY_Y
checkResult

test "PINA: 0x00, 0x04, 0x04, 0x02 => PORTB: 0x01 state: PAUSE"
set state = INIT
setPINA 0x00
continue 2
setPINA 0x04
continue 2
setPINA 0x04
continue 2
setPINA 0x02
continue 2
#expectPORTC 0x02
expectPORTB 0x01
expect state PAUSE
checkResult

test "PINA: 0x02, 0x04, 0x02, 0x04 => PORTB: 0x00 state: KEY_POUND"
set state = INIT
setPINA 0x02
continue 2
setPINA 0x04
continue 2
setPINA 0x02
continue 2
setPINA 0x04
continue 2
#expectPORTC 0x02
expectPORTB 0x00
expect state KEY_POUND
checkResult

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests

echo ======================================================\n
