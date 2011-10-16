@echo off

REM Auto tester for the SimpleRPG program.
REM Automatically tests the same map until all creatures are dead
REM or a maximum number of days has been reached.

set base=autoOut
set /A index=0

:StartLoop
IF %index% GEQ 10 GOTO EndLoop

set output=%base%%index%.out
echo Running test %index%.
start /Wait SimpleRPG.exe -m 200 -a -t 600 -o %output% map3.txt
set /A index += 1
GOTO:StartLoop

:EndLoop

pause