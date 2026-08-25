CC      = gcc
CFLAGS = -Wall -Wextra -I. -Ilib -Isrc -Iinclude -DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_UNIX
LIBS    =

SRCS    = main.c \
          src/wifi/connect.c \
          lib/wpa_ctrl.c \
		  src/passwords/read_passwords.c \
		  src/passwords/check_password_lists.c \
		  src/config/logging.c

TARGET  = seahammer

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)