a.out: parse.y check2.l main.c
	bison -y -d parse.y
	flex check2.l
	gcc lex.yy.c y.tab.c main.c stable.c stable.h ast.c ast.h bisonHelper.c bisonHelper.h -ll

debug: parse.y check2.l main.c
	bison --debug --verbose -y -d parse.y
	flex check2.l
	gcc -g lex.yy.c y.tab.c main.c stable.c stable.h ast.c ast.h bisonHelper.c bisonHelper.h -ll

windows: parse.y check2.l main.c
	bison -y -d parse.y
	flex check2.l
	gcc lex.yy.c y.tab.c main.c stable.c stable.h ast.c ast.h -lfl
clean:
	rm -f a.exe a.out lex.yy.c y.tab.c y.tab.h stable.h.gch ast.h.gch bisonHelper.h.gch
cleanDebug:
	rm -f -r a.out.dSYM a.exe a.out lex.yy.c y.tab.c y.tab.h y.output stable.h.gch ast.h.gch bisonHelper.h.gch
