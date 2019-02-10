#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
	//creating an array of chars that meet the expected 'pre-condition' (with some extra chars thrown in)
	char domain[] = { '[','(','{',' ','a','x','}',')',']' };
	int domainSize = 9;

	//selecting a random character from our domain
	char c = domain[rand() % domainSize];

	//and returning randomly selected char
    return c;
}

char *inputString()
{
	//creating a string of size 6 since s[5] = '\0' in test function
	int strSize = 6;

	//dynamically allocating memory (note this creates memory leaks since it isn't freed later)
	char *str = malloc(sizeof(char) * strSize);

	//initially setting all chars in the array to '\0' (we'll fill in chars 0 - 5 next)
	memset(str, '\0', sizeof(char)*strSize);

	//creating an array of chars that meet the expected 'pre-condition' in test function (with extra chars)
	char domain[] = { 'e','r','s','t'};
	int domainSize = 4;

	//looping through indexes 0 - 4 in string and randomly selecting a character from domain
	int i = 0;
	for (i = 0; i < strSize - 1; i++)
	{
		str[i] = domain[rand() % domainSize];
	}

	//returning the string (note we did not free dynamic memory... causes leaks...)
    return str;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
