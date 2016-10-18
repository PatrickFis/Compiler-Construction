a.out: parse.y check2.l main.c
	bison --debug -y -d parse.y
	flex check2.l
	gcc lex.yy.c y.tab.c main.c -ll

clean:
	rm -f a.exe a.out lex.yy.c y.tab.c y.tab.h
