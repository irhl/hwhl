all:
	$(CC) hwhl.c -o hwhl

clean:
	$(RM) hwhl

install:
	$(CC) hwhl.c -o /usr/local/bin/hwhl

uninstall:
	$(RM) /usr/local/bin/hwhl
