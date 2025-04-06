@echo off
assets2banks ..\assets --compile
sdcc -c -mz80 --opt-code-speed --peep-file peep-rules.txt ..\src\main.c

@if %errorlevel% NEQ 0 goto :EOF

sdcc -o LDJAM57.ihx -mz80 --no-std-crt0 --data-loc 0xC000 ^
-Wl-b_BANK2=0x28000 ^
-Wl-b_BANK3=0x38000 ^
..\lib\crt0_sms.rel main.rel ^
bank2.rel ^
bank3.rel ^
SMSlib.lib ..\lib\PSGLib.rel ..\lib\GSLib.rel

makesms LDJAM57.ihx ..\banks\LDJAM57.sms

echo clean up files
del *.lk
del *.map
del *.noi
del *.ihx
del *.rel
del *.lst
del *.sym
del *.asm

java -jar ..\Emulicious\Emulicious.jar LDJAM57.sms