all:
	$(CC) hwhl.c -o hwhl

clean:
	$(RM) hwhl

install:
	$(CC) hwhl.c -o /bin/hwhl

uninstall:
	$(RM) /bin/hwhl
