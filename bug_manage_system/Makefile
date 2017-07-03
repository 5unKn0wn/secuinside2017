all:
	gcc -o bug_manage_system source.c -m32 -fstack-protector -fPIE -pie -Wl,-z,relro,-z,now
	strip ./bug_manage_system

clean:
	rm bug_manage_system
