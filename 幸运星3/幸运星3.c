#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>

//参赛人员、编号及状态
typedef struct person
{
	int number;
	char name[30];
	int status;
}Person;

//奖品信息
typedef struct prize
{
	int number;
	char name[30];
	int price;
}Prize;

//轮次奖品
typedef struct temporaryprize
{
	int number;
	char name[30];
	int price;
}Temporary;

void gamebegin();
void gameprogress();
void delinformation();
void inputprize();
void inputperson();
int gameplay(int k);
void choose(char c);
void winnerinformation(int numluck);
void information();
void putinformation();

Person *person;
Prize *prize;
Temporary temprize[3];
int numprize;
int numperson;

int main()
{
	char c;
	srand((unsigned)time(NULL)); //随机
	system("mode con cols=81");
	while(1){
		system("cls");
		gamebegin();
		printf("请选择对应编号的操作:");
		fflush(stdin);
		c=getch();
		system("cls");
		if(c=='4'){
			printf("谢谢使用!");
			system("pause");
			break;
		}
		else{
			choose(c);
		}
	}
	return 0;
}

void gamebegin()
{
	printf("\n");
	printf("\n");
	printf("\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("                                                                                \n");
	printf("                                   1.开始游戏                                   \n");
	printf("                                                                                \n");
	printf("                                                                                \n");
	printf("                               2.显示已存轮次信息                               \n");
	printf("                                                                                \n");
	printf("                                                                                \n");
	printf("                               3.删除已存轮次信息                               \n");
	printf("                                                                                \n");
	printf("                                                                                \n");
	printf("                                   4.退出游戏                                   \n");
	printf("                                                                                \n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

void gameprogress()
{
	int numluck;
	int k;//骰子数
	inputperson();
	inputprize();
	information();
	k=1+rand()%6;
	while(k>numperson/2){
		k=1+rand()%6;
	}
	printf("骰子数:%d\n",k);
	numluck=gameplay(k);
	winnerinformation(numluck);
}

void information()
{
	FILE *fp;
	int i;
	fp=fopen("C:\\Program Files\\信息.txt","a");
	fprintf(fp,"参与者:\n");
	for(i=0;i<numperson;i++){
		fprintf(fp,"No.%d%s  ",person[i].number,person[i].name);//编号，姓名
	}
	fprintf(fp,"\n奖品池:\n");
	for(i=0;i<numprize;i++){
		fprintf(fp,"No.%d%s %d￥  ",prize[i].number,prize[i].name,prize[i].price);//编号，名称，价格
	}
	fclose(fp);
}
void winnerinformation(int numluck)
{
	FILE *fp;
	fp=fopen("C:\\Program Files\\信息.txt","a");
	fprintf(fp,"\n幸运星:\n");
	fprintf(fp,"%s %s%d￥ %s%d￥ %s%d￥",person[numluck-1].name,
									  temprize[0].name,temprize[0].price,
									  temprize[1].name,temprize[1].price,
									  temprize[2].name,temprize[2].price);
	fprintf(fp,"\n");
	fclose(fp);
}

void delinformation()
{
	FILE * fp=fopen("C:\\Program Files\\信息.txt","w");
	fclose(fp);
}

void inputperson()
{
	int i;
	printf("请输入参与人数:");
	scanf("%d",&numperson);
	person=(Person*)malloc(sizeof(Person)*numperson);
	printf("请输入参与者姓名（系统将按序编号）");
	for(i=0;i<numperson;i++){
		scanf("%s",person[i].name);
		person[i].number=i+1;
		person[i].status=0;
	}
}

void inputprize()
{
	int i;
	printf("请输入奖品数量:");
	scanf("%d",&numprize);
	prize=(Prize*)malloc(sizeof(Prize)*numprize);
	printf("请输入奖品信息（名称、价格）\n");
	for(i=0;i<numprize;i++){
		scanf("%s%d",prize[i].name,&prize[i].price);
		prize[i].number=i+1;
	}
}


int gameplay(int k)
{
	int numluck;
	int number=1;//报数
	int i=0;
	int j;//随机奖品号
	int oddperson=numperson;//剩余人数
	printf("淘汰者:\n");
	while(oddperson>1){
		if(person[i].status==0){
			if(number!=k){
				number++;
				i++;
				if(i>=numperson){
					i=0;
				}
			}
			else{
				person[i].status=1;
				printf("%d %s\n",person[i].number,person[i].name);
				oddperson--;
				number=1;
				i++;
				if(i>=numperson){
					i=0;
				}
			}
		}
		else{
			i++;
			if(i>=numperson){
				i=0;
			}
		}
	}
	for(i=0;i<numperson;i++){
		if(person[i].status==0){
			numluck=person[i].number;
			printf("幸运星:%s\n",person[i].name);
			break;
		}
	}
	for(i=0;i<3;i++){
		j=rand()%numprize;
		temprize[i].number=prize[j].number;
		temprize[i].price=prize[j].price;
		strcpy(temprize[i].name,prize[j].name);
	}
	printf("幸运星所获奖品:\n");
	for(i=0;i<3;i++){
		printf("%s %d  ",temprize[i].name,temprize[i].price);
	}
	printf("\n");
	return numluck;
}

void choose(char c)
{
	switch(c){
	case'1':gameprogress();system("pause");break;
	case'2':putinformation();system("pause");break;
	case'3':delinformation();system("pause");break;
	default:
		printf("您的选择有误，请重新选择!\n");
			system("pause");
	}
}

void putinformation()
{
	FILE*fp=fopen("C:\\Program Files\\信息.txt","r");
	char ch;
	if(fp==NULL)
	{
		printf("您可能未开始游戏或已删除信息\n");
	}
	else
	{
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		fscanf(fp,"%c",&ch);
		while(!feof(fp))
		{
			putchar(ch);
			fscanf(fp,"%c",&ch);
		}
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	}
}