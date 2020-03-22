#My makefile
OBJS = main.o Member_01.o Member_02.o Member_03.o OurTeam.o
HDRS = main.h
CFLAGS = -g
CC = gcc

program: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o program

main.o: main.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@
Member_01.o: Member_01.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@
Member_02.o: Member_02.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@
Member_03.o: Member_03.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@
OurTeam.o: OurTeam.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY:clean
clean:
	rm *.o
