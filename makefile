CC=gcc
CFLAGS=-g -Wall

main:
	@$(CC) main.c cd.c echo_this.c print_wd.c prompt.c list_em.c getpinfo.c hist.c othercmd.c discovery.c jobs.c send_signal.c fore_g.c back_g.c sig_handle.c input.c
debug:
	@$(CC) $(CFLAGS) main.c cd.c echo_this.c print_wd.c prompt.c list_em.c getpinfo.c hist.c othercmd.c discovery.c jobs.c send_signal.c fore_g.c back_g.c sig_handle.c input.c
all:clean main
allD:clean debug
clean:
	@rm -f main
