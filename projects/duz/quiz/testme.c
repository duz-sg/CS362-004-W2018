#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    // TODO: rewrite this function
    // From 88 to 125 in Ascii table
    char randomletter = 'X' + (random() % 38);
    if (randomletter == 'X')
    {
        return ' ';
    }
    if (randomletter == 'Y')
    {
        return '(';
    }
    if (randomletter == 'Z')
    {
        return ')';
    }
    return randomletter;
}

char *inputString()
{
    // TODO: rewrite this function
    // Input pool be the lowercase letters
    char charset[] = "abcdefghijklmnopqrstuvwxyz"; 
    char *randomstring;
    int n = 0;
    randomstring = malloc(6);
    for (n = 0; n < 5; n++)
    {
        int key = random() % 26;
        randomstring[n] = charset[key];
    }
    randomstring[5] = '\0';
    return randomstring;
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
