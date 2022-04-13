default:
	sdcc --std-c99 -mstm8 main.c
	del /S *.lst *.sym *.rel *.lst *.rst *.lk *.asm *.map
	./stm8flash -c stlinkv2 -p stm8s103f3 -w main.ihx
	plink.exe -serial COM4 -sercfg 115200,8,n,1,N
