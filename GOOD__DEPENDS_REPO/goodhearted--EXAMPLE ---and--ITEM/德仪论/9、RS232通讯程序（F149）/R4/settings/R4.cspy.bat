@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM 


"E:\IAR for MSP430\common\bin\cspybat" "E:\IAR for MSP430\430\bin\430proc.dll" "E:\IAR for MSP430\430\bin\430fet.dll"  %1 --plugin "E:\IAR for MSP430\430\bin\430bat.dll" --backend -B "--hardware_multiplier" "16" "--hwmult_type" "1" "-p" "E:\IAR for MSP430\430\config\MSP430F149.ddf" "--iv_base" "0xFFE0" "-d" "fet" "--erase_main_and_info" "--derivative" "MSP430F149" "-lpt1" "--attach" "--protocol" "4wire" "--eem" "EMEX_MEDIUM" "--disable_memory_cache" "--connection" "ti_lpt" "--msp430_dll" "msp430.dll" "--vccDefault" "3.3" 


