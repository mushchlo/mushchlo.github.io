#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 1024

int choice, buy, sell, turnips, profit, err, todomax = 8;
char buffer[BUFSIZE], addtodo[24];
char todo[40][25] = { "check mail", "visit both shops", "visit resident services", "talk to villagers", "shake trees", "water flowers", "dig up fossils", "hit rocks", "find money tree" };

void getinput(void)
{
	if(fgets(buffer, BUFSIZE, stdin) == NULL)	// gets input, handles ctrl-d/ctrl-z
		exit(0);
	buffer[strcspn(buffer, "\n")] = 0;	// strips newline
}

int isint(char *str)
{
    for(int i = 0; str[i] != '\0'; i++)
        if(!isdigit(str[i]))
            return 0;
    return 1;
}

int getchoice(char *msg, int min, int max)
{
	printf("%s", msg);
	while(1)		// loops until valid input is given
	{
		getinput();
		if (isint(buffer))
			if (atoi(buffer) >= min && atoi(buffer) <= max)
				break;
		printf("Not a valid option (%d-%d)\n", min, max);
	}
	return atoi(buffer);
}

void gohome(void)
{
	fputs("type 'home' when you're ready to return to the main menu.\n", stdout);
	while (strcmp("home", buffer) != 0)
		getinput();
}

int getturnip(char *msg)
{
	while(1)		// gets info for turnip calculator
	{
		printf("%s\n", msg);
		getinput();
		if(isint(buffer))
			if(strlen(buffer) < 13)
				break;
			else
				fputs("number too powerful!!\n", stderr);
		else
			fputs("please enter a whole number!\n", stderr);
	}
	return atoi(buffer);
}

void printtodo(void)
{
	char middle[BUFSIZE] = "";
	for(int i = 0; todo[i][0] != '\0'; i++)
			sprintf(middle + strlen(middle), "	| #%d: %-*s |\n", i + 1, i < 9 ? 24 : 23, todo[i]);
	printf("	+------------------------------+\n"
		"	| daily to-do list!            |\n"
		"	+------------------------------+\n"
		"	|                              |\n"
					"%s"
		"	|                              |\n"
		"	+------------------------------+\n", middle);
}

void additem(void)
{
	fputs("what would you like to add?\n(remember, there's a 25 character limit)\n", stdout);
	while(1)
	{
		getinput();
		if(strlen(buffer) <= 25)
			break;
		fputs("please enter a shorter to-do!", stderr);
	}
	strcpy(addtodo, buffer);
	choice = getchoice("where would you like to add the item?\n#1: at the start\n#2: in a specific spot\n#3: at the end\n", 1, 3);
	switch(choice)
	{
		case 1: todomax++;
			for(int i = todomax; i > 0; i--)
				strcpy(todo[i], todo[i-1]);
			strcpy(todo[0], addtodo);
			break;
		case 2:	choice = getchoice("what number would you like to add the item at?\n", 1, todomax + 1);
			for(int i = todomax; i >= choice - 1; i--)
				strcpy(todo[i+1], todo[i]);
			strcpy(todo[choice - 1], addtodo);
			break;
		case 3: strcpy(todo[todomax++ + 1], addtodo);
	}
}

void removeitem(void)
{
	choice = getchoice("what number on the list would you like to remove?\n", 1, todomax+1);
	for(int i = choice; i <= todomax; i++)
		strcpy(todo[i - 1], todo[i]);
	todo[todomax][0] = '\0';
	todomax--;
}

void edittodo(void)
{
	while(strcmp(buffer, "home") != 0)
	{
		printtodo();
		choice = getchoice("\nhere's what your list looks like now. what changes would you like to make?\n#1: add an item\n#2: take away an item\n#3: none, it's finished\n", 1, 3);
		switch(choice)
		{
			case 1: additem();
				break;
			case 2:	removeitem();
				break;
			case 3: gohome();
		}
	}
}

void calcmenu(void)
{
	while(tolower(*buffer) != 'y' && strcmp(buffer, "home") != 0)
	{
		if(err)
			fputs("\nthat wasn't yes or no, do you want to update your total profits? (Y/N)\n", stdout);
		else
			printf("	+------------------------------+\n"
				"	| turnip calculator!           |\n"
				"	+------------------------------+\n"
				"	|                              |\n"
				"	| turnip price: $%-13d |\n"
				"	|                              |\n"
				"	| # of turnips:  %-13d |\n"
				"	|                              |\n"
				"	| price @ sale: $%-13d |\n"
				"	|                              |\n"
				"	| profit   →    $%-13d |\n"
				"	|                              |\n"
				"	+------------------------------+\n\n"
				"do you want to update your total profits? (Y/N)\n", buy, turnips, sell, profit);
		err = 0;
		getinput();
		switch(tolower(*buffer))
		{
			case 'y':
				buy = getturnip("what price did you buy your turnips at?");
				turnips = getturnip("how many turnips did you buy?");
				sell = getturnip("what price did you sell your turnips at?");
				profit = (sell - buy) * turnips;
				break;
			case 'n':
				gohome();
				break;
			default:
				err = 1;
		}
	}
}

int main(void)
{
	while(1)
	{
		choice = getchoice("	+------------------------------+\n"
				"	| AC:NH project!               |\n"
				"	+------------------------------+\n"
				"	|                              |\n"
				"	| #1 → turnip calculator       |\n"
				"	|                              |\n"
				"	| #2 → view daily to-do list   |\n"
				"	|                              |\n"
				"	| #3 → update to-do list       |\n"
				"	|                              |\n"
				"	+------------------------------+\n"
				"\nwhat would you like to do? (1-3)\n", 1, 3);
		switch(choice)
		{
			case 1: calcmenu();
				break;
			case 2: printtodo();
				gohome();
				break;
			case 3: edittodo();
		}
	}
}
