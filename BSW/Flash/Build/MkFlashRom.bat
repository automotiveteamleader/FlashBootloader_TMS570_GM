if "%1" == "" goto end

@echo [C_EXPORT]                                                                          >  %~dp0\%~n1.ini
@echo Decryption=1         ;  0=Off,  1=On                                                >> %~dp0\%~n1.ini
@echo Decryptvalue=0xCC    ; Value for encryption using XOR with each uchar/ushort/ulong  >> %~dp0\%~n1.ini
@echo Prefix=flashDrv                                                                  >> %~dp0\%~n1.ini
@echo WordSize=0           ; 0=uchar, 1=ushort, 2=ulong                                   >> %~dp0\%~n1.ini
@echo WordType=0           ; 0=Intel, 1=Motorola (only used if wordsize > 0).             >> %~dp0\%~n1.ini


"D:\uti\Hexview\hexview.exe" %1 /s /FAFF /XC -o %~d0%~p0..\flashrom.c

:end
