/*#################################################*/
/*#snake game                                     #*/
/*#                                               #*/
/*#written by Guner Acet                          #*/
/*#                                               #*/
/*#started 2 July 2014 16:00                      #*/
/*#                                               #*/
/*#################################################*/

#include<stdio.h>
#include<stdlib.h>
#include <windows.h>
#include <time.h>
#define ROW 21
#define COLUMN 21
#define MAXSNAKE 50

typedef struct{
    int snakeCoor[MAXSNAKE][2];
    int size;
    char snakeDirection;
}
snake_t;

typedef struct{
    int x;
    int y;
}
bait_t;

typedef struct{
   char table1[ROW][COLUMN];
   char table2[ROW][COLUMN];
}
buffer_t;

void printGame(char table[ROW][COLUMN]);

char moveChar(void);

void processGame(snake_t snake,bait_t bait,char table[ROW][COLUMN]);

void clearGame(char table[ROW][COLUMN]);

int moveSnake(snake_t *snake,bait_t bait,char direction);

int randomNumber(void);

void switchBuffer(char **t1,char **t2);

void print(char table1[ROW][COLUMN],char table2[ROW][COLUMN],int con);

void gotoXY(int x, int y,char c);

void copy(char table1[ROW][COLUMN],char table2[ROW][COLUMN]);

int speed=5;

int main()
{
    int menu=0,flag=1,i,j,a,score=0,asd;
    char ch,direction;
    char table1[ROW][COLUMN];
    char table2[ROW][COLUMN];
    snake_t snake;
    bait_t bait;
    snake.size=3;
    snake.snakeDirection='r';
    snake.snakeCoor[0][0]=5;
    snake.snakeCoor[0][1]=5;
    snake.snakeCoor[1][0]=6;
    snake.snakeCoor[1][1]=5;
    snake.snakeCoor[2][0]=7;
    snake.snakeCoor[2][1]=5;
    bait.x=10;
    bait.y=10;
    direction='r';

    while(flag==1)
    {
         system("cls");
         printf("1-Start\n2-Settings\n3-exit\nEnter Number:");
         ch=getch();
         menu=atoi(&ch);

         if(menu==1)
            flag=0;
         else if(menu==2)
         {
            int temp=1;
            int x;
            int cursor=5001;
            char c;
            while(temp!=0)
            {
                system("cls");
                printf("Setting Menu\n");
                if(cursor%2==1)
                    printf("-> ");
                else
                    printf("   ");
                printf("Speed ");
                for(x=0;x<speed;++x)
                    printf("#");
                printf("\n");
                if(cursor%2==0)
                    printf("-> ");
                else
                    printf("   ");
                printf("return menu\n");
                c=getch();
                if(c=='d')
                {
                    if(cursor%2==1)
                        speed++;
                    else
                    {
                        temp=0;
                        break;
                    }
                }
                else if(c=='s')
                {
                    cursor++;
                }
                else if(c=='w')
                {
                    cursor--;
                }
                else if(c=='a')
                {
                    if(cursor%2==1)
                        speed--;
                    else
                    {
                        temp=0;
                        break;
                    }
                }
                else
                {
                    fflush(stdin);
                }
            }

         }
         else if(menu==3)
         {
             printf("Game Exit\n");
             return 0;
         }
    }

    for(i=0;i<COLUMN;++i)
    {
        for(j=0;j<ROW;++j)
        {
            if(i==0 || i==COLUMN-1)
                table1[j][i]='#';
            else if(j==0 || j==ROW-1)
                table1[j][i]='#';
            else
                table1[j][i]=' ';
        }
    }
    for(i=0;i<COLUMN;++i)
    {
        for(j=0;j<ROW;++j)
        {
            if(i==0 || i==COLUMN-1)
                table2[j][i]='#';
            else if(j==0 || j==ROW-1)
                table2[j][i]='#';
            else
                table2[j][i]=' ';
        }
    }

    processGame(snake,bait,table1);
    printGame(table1);

    /*game*/
    while(1)
    {
        int counter=1;
        direction=moveChar();
        a=moveSnake(&snake,bait,direction);
        fflush(stdin);

        if(a==2)/*game over*/
            break;
        if(a==0)
        {
            bait.x=randomNumber();
            bait.y=randomNumber();
            score+=10;
            printf("\a");
        }
            /*switchBuffer((char**)table1,(char**)table2);*/
            copy(table1,table2);
            processGame(snake,bait,table1);
            print(table1,table2,counter);
            ++counter;
            /*printf("Score:%d\n",score);*/

    }

    system("cls");
    printf("Game Over");
    Sleep(1000);
    main();
    return 0;
}

void printGame(char table[ROW][COLUMN])
{
    register int i,j;
    system("cls");
    for(i=0;i<COLUMN;++i)
    {
        for(j=0;j<ROW;++j)
            printf("%c",table[j][i]);
        printf("\n");
    }
}

char moveChar(void)
{
    int ch;
    Sleep(1000/speed);
    if(!kbhit())
        return 'e';/*error*/
    ch=getch();
    if(ch==119 || ch==87)/*up*/
        return 'u';
    else if(ch==115 || ch==83)/*down*/
        return 'd';
    else if(ch==97 || ch==65)/*left*/
        return 'l';
    else if(ch==100 || ch==68)/*right*/
        return 'r';
    else
        return 'e';/*error*/
}

void processGame(snake_t snake,bait_t bait,char table[ROW][COLUMN])
{
    register int i;
    clearGame(table);
    table[bait.x][bait.y]='+';
    for(i=0;i<snake.size;++i)
    {
        table[snake.snakeCoor[i][0]][snake.snakeCoor[i][1]]='#';
    }
}

void clearGame(char table[ROW][COLUMN])
{
    register int i,j;
    for(i=1;i<COLUMN-1;++i)
    {
        for(j=1;j<ROW-1;++j)
            table[j][i]=' ';
    }
}

int moveSnake(snake_t *snake,bait_t bait,char direction)
{
    register int i;
    int xLast,yLast,xNew,yNew;
    if(direction=='u' || ( (*snake).snakeDirection=='u' && direction=='e') )
    {
        if((*snake).snakeCoor[(*snake).size-1][0]==bait.x && (*snake).snakeCoor[(*snake).size-1][1]-1==bait.y)
        {
            (*snake).size++;
            (*snake).snakeCoor[(*snake).size-1][0]=bait.x;
            (*snake).snakeCoor[(*snake).size-1][1]=bait.y;

            if(direction!='e')
                (*snake).snakeDirection=direction;
            return 0;/*eat bait*/
        }
        xLast=(*snake).snakeCoor[(*snake).size-1][0];
        yLast=(*snake).snakeCoor[(*snake).size-1][1];

        if(yLast-1==0)
            return 2;/*game over*/

        for(i=0;i<(*snake).size;++i)
        {
            if((*snake).snakeCoor[i][0]==xLast && (*snake).snakeCoor[i][1]==yLast-1)
                return 2;/*game over*/
        }

        (*snake).snakeCoor[(*snake).size-1][1]--;

        for(i=(*snake).size-2;i>=0;--i)
        {
            xNew=(*snake).snakeCoor[i][0];
            yNew=(*snake).snakeCoor[i][1];
            (*snake).snakeCoor[i][0]=xLast;
            (*snake).snakeCoor[i][1]=yLast;
            xLast=xNew;
            yLast=yNew;
        }

        (*snake).snakeDirection='u';

        return 1;/*clear path*/
    }
    else if(direction=='d' || ( (*snake).snakeDirection=='d' && direction=='e') )
    {
        if((*snake).snakeCoor[(*snake).size-1][0]==bait.x && (*snake).snakeCoor[(*snake).size-1][1]+1==bait.y)
        {
            (*snake).size++;
            (*snake).snakeCoor[(*snake).size-1][0]=bait.x;
            (*snake).snakeCoor[(*snake).size-1][1]=bait.y;

            if(direction!='e')
                (*snake).snakeDirection=direction;
            return 0;/*eat bait*/
        }

        xLast=(*snake).snakeCoor[(*snake).size-1][0];
        yLast=(*snake).snakeCoor[(*snake).size-1][1];

        if(yLast+2==ROW)
            return 2;/*game over*/

        for(i=0;i<(*snake).size;++i)
        {
            if((*snake).snakeCoor[i][0]==xLast && (*snake).snakeCoor[i][1]==yLast+1)
                return 2;/*game over*/
        }

        (*snake).snakeCoor[(*snake).size-1][1]++;

        for(i=(*snake).size-2;i>=0;--i)
        {
            xNew=(*snake).snakeCoor[i][0];
            yNew=(*snake).snakeCoor[i][1];
            (*snake).snakeCoor[i][0]=xLast;
            (*snake).snakeCoor[i][1]=yLast;
            xLast=xNew;
            yLast=yNew;
        }

        (*snake).snakeDirection='d';

        return 1;/*clear path*/
    }
    else if(direction=='l' || ( (*snake).snakeDirection=='l' && direction=='e') )
    {
        if((*snake).snakeCoor[(*snake).size-1][0]-1==bait.x && (*snake).snakeCoor[(*snake).size-1][1]==bait.y)
        {
            (*snake).size++;
            (*snake).snakeCoor[(*snake).size-1][0]=bait.x;
            (*snake).snakeCoor[(*snake).size-1][1]=bait.y;

            if(direction!='e')
                (*snake).snakeDirection=direction;
            return 0;/*eat bait*/
        }

        xLast=(*snake).snakeCoor[(*snake).size-1][0];
        yLast=(*snake).snakeCoor[(*snake).size-1][1];

        if(xLast-1==0)
            return 2;/*game over*/

        for(i=0;i<(*snake).size;++i)
        {
            if((*snake).snakeCoor[i][0]==xLast-1 && (*snake).snakeCoor[i][1]==yLast)
                return 2;/*game over*/
        }

        (*snake).snakeCoor[(*snake).size-1][0]--;

        for(i=(*snake).size-2;i>=0;--i)
        {
            xNew=(*snake).snakeCoor[i][0];
            yNew=(*snake).snakeCoor[i][1];
            (*snake).snakeCoor[i][0]=xLast;
            (*snake).snakeCoor[i][1]=yLast;
            xLast=xNew;
            yLast=yNew;
        }
        (*snake).snakeDirection='l';

        return 1;/*clear path*/
    }
    else if(direction=='r' || ( (*snake).snakeDirection=='r' && direction=='e') )
    {
        if((*snake).snakeCoor[(*snake).size-1][0]+1==bait.x && (*snake).snakeCoor[(*snake).size-1][1]==bait.y)
        {
            (*snake).size++;
            (*snake).snakeCoor[(*snake).size-1][0]=bait.x;
            (*snake).snakeCoor[(*snake).size-1][1]=bait.y;

            if(direction!='e')
                (*snake).snakeDirection=direction;
            return 0;/*eat bait*/
        }

        xLast=(*snake).snakeCoor[(*snake).size-1][0];
        yLast=(*snake).snakeCoor[(*snake).size-1][1];
        if(xLast+2==COLUMN)
            return 2;/*game over*/
        for(i=0;i<(*snake).size;++i)
        {
            if((*snake).snakeCoor[i][0]==xLast+1 && (*snake).snakeCoor[i][1]==yLast)
                return 2;/*game over*/
        }

        (*snake).snakeCoor[(*snake).size-1][0]++;

        for(i=(*snake).size-2;i>=0;--i)
        {
            xNew=(*snake).snakeCoor[i][0];
            yNew=(*snake).snakeCoor[i][1];
            (*snake).snakeCoor[i][0]=xLast;
            (*snake).snakeCoor[i][1]=yLast;
            xLast=xNew;
            yLast=yNew;
        }
        (*snake).snakeDirection='r';

        return 1;/*clear path*/
    }
}

int randomNumber(void)
{
   time_t t;

   srand((unsigned) time(&t));

   return(rand()%19+1);
}

void switchBuffer(char **t1,char **t2)
{
    char **temp;
    temp=t1;
    t1=t2;
    t2=temp;
}


void print(char table1[ROW][COLUMN],char table2[ROW][COLUMN],int con)
{
    int i,j;
    CONSOLE_SCREEN_BUFFER_INFO save;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&save);
    for(i=0;i<ROW-1;++i)
    {
        for(j=0;j<COLUMN-1;++j)
        {
            if(table1[i][j]=='#' && table2[i][j]==' ')
            {
                gotoXY(i,j,table1[i][j]);
            }
            else if(table1[i][j]==' ' && table2[i][j]=='#')
            {
                gotoXY(i,j,table1[i][j]);
            }
            else if(table1[i][j]=='+' && table2[i][j]==' ')
            {
                gotoXY(i,j,table1[i][j]);
            }
            else if(table1[i][j]==' ' && table2[i][j]=='+')
            {
                gotoXY(i,j,table1[i][j]);
            }

        }
    }
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),save.dwCursorPosition);
}

 void gotoXY(int x, int y,char c) {
    COORD coord;
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
    printf("%c",c);
}

void copy(char table1[ROW][COLUMN],char table2[ROW][COLUMN])
{
    int i,j;
    for(i=0;i<ROW-1;++i)
        for(j=0;j<COLUMN-1;++j)
        table2[i][j]=table1[i][j];
}
