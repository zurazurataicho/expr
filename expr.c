#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void error();
int getnum();
void factor();
void term();
void expr();
void init();
int main();

#define	LABEL_MAXLEN	32

#define	fetch()		(*(str+pos++))
#define getch()     do { ch=fetch(); } while (isspace(ch))

char str[BUFSIZ];
int pos;
int ch = ' ';


void error()
{
	printf("err: pos=%d, char='%c' (%02xH)\n", pos, ch, ch);
	exit(0);
}

int getnum()
{
	int num;

	num = 0;
	do {
		num = num * 10 + (ch-(int)'0');
		getch();
	} while (isdigit(ch));

	return num;
}

void factor()
{
	int num, sign, ptr;
	char lbl[LABEL_MAXLEN];

	if (isdigit(ch)) {
		num = getnum();
		printf("PUSH %d\n", num);
	} else if (ch == '+' || ch == '-') {
		sign = (ch == '-') ? -1 : 1;
		getch();
		if (isdigit(ch)) {
			num = getnum() * sign;
			printf("PUSH %d\n", num);
		} else {
			error();
		}
	} else if (isalpha(ch)) {
		ptr = 0;
		do {
			lbl[ptr++] = ch;
			getch();
		} while ((isalnum(ch) || ch == '_') && ptr < LABEL_MAXLEN+1);
		lbl[ptr] = '\0';
		printf("PUSH (%s)\n", lbl);
	} else if (ch == '(') {
		getch();
		expr();
		if (ch != ')') {
			error();
		}
		getch();
	} else if (ch == ')') {
		return;
	} else if (ch == '\0') {
		return;
	} else {
		error();
	}
}

void term()
{
	factor();
	for (;;) {
		switch (ch) {
			case '*':
				getch();
				factor();
				printf("POP B\n");
				printf("POP A\n");
				printf("MUL A,B\n");
				printf("PUSH A\n");
				break;
			case '/':
				getch();
				factor();
				printf("POP B\n");
				printf("POP A\n");
				printf("DIV A,B\n");
				printf("PUSH A\n");
				break;
			default:
				return;
		}
	}
}

void expr()
{
	term();
	for (;;) {
		switch (ch) {
			case '+':
				getch();
				term();
				printf("POP B\n");
				printf("POP A\n");
				printf("ADD A,B\n");
				printf("PUSH A\n");
				break;
			case '-':
				getch();
				term();
				printf("POP B\n");
				printf("POP A\n");
				printf("SUB A,B\n");
				printf("PUSH A\n");
				break;
			case ')':
			case '\0':
				return;
			default:
				error();
		}
	}
}

void init()
{
	pos = 0;
}

int main()
{
	init();
	scanf("%[^\n]", str);
	printf(">> %s\n", str);
	getch();
	expr();
	return 0;
}
