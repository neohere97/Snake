default:
	sdcc --std-c99 -mstm8 -c main.c
	sdcc --std-c99 -mstm8 -c joystick.c
	sdcc --std-c99 -mstm8 -c display.c
	sdcc --std-c99 -mstm8 -c spi.c
	sdcc --std-c99 -mstm8 -c game.c
	sdcc --std-c99 -mstm8 -c beeper_motor.c
	sdcc --std-c99 -mstm8 -c systick.c
	sdcc --std-c99 -mstm8 -c uart.c
	sdcc -mstm8 main.rel joystick.rel uart.rel systick.rel beeper_motor.rel game.rel display.rel spi.rel
	del /S *.lst *.sym *.rel *.lst *.rst *.lk *.asm *.map
	./stm8flash -c stlinkv2 -p stm8s103f3 -w main.ihx
	plink.exe -serial COM5 -sercfg 115200,8,n,1,N
