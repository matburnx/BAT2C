LABEL foobar
#LABEL main
LET index = 1
PRINT index
GOTO main
#LET foo = bar * 3 + 2
#IF foo > 0 THEN
#  IF 10 * 10 < 100 THEN
#    PRINT bar
#  ENDIF
#ENDIF
#LET foo = bar * 2 + 3
#PRINT "hello, world!"
#PRINT "how are ya?"
#PRINT "this is promising"
#LET i = 0
#WHILE i<5 REPEAT
#  PRINT "hey"
#ENDWHILE
#LABEL loop
#PRINT "hello, world!"
#GOTO loop
#JUMP GOTO
