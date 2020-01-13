#include<windows.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


/*--------------------------------------------------------结构体的建立-------------------------------------------------------------------*/
//参赛人员及编号
typedef struct person
{
	int number;
	char name[30];

	struct person *next;
}Person;

//奖品信息
typedef struct prize
{
	int number;
	char name[30];
	int price;
	struct prize *next;
}Prize;

//淘汰人员
typedef struct eliminate
{
	int number;
	char name[30];
	struct eliminate *next;
}Elimination;

//轮次奖品
typedef struct temporaryprize
{
	int number;
	char name[30];
	int price;
}Temporary;

//幸运之星
typedef struct luck
{
	char name[30];
	char prize[3][30];
	int price[3];
	struct luck * next;
}Luck;


/*--------------------------------------------------------------全局变量以及函数的声明------------------------------------------------------*/
int numperson,numprize=3; //参赛游戏人数，奖品总数
Person * personhead=NULL; //人员头指针
Prize * prizehead=NULL; //奖品头指针
Elimination * peoplehead=NULL; //淘汰人员头指针
Person * final=NULL;//用于存放圆环最后一人
Person * enter(); //输入参赛游戏人员信息
void wholeprize(); //输入奖品池信息
void gamebegin();//开始界面
void gameplay(); //游戏过程
void information();//开局信息
void saveinformation();//存储信息
void read();//显示排行榜
void backspace();//删除信息
void seek();//查找学生幸运状态
void error();//菜单错误输入
void end();//结束
void del();//删除文件
void load();//载入信息-----------不写了


/*------------------------------------------------------------主函数------------------------------------------------------------------*/
int main()
{
	char c;//选择
	srand((unsigned)time(NULL)); //随机
	system("mode con cols=81");
	system("color f0");
	while(1)
	{
		gamebegin();
		fflush(stdin);
		c=getch();
		system("cls");
		if(c=='1')
		{   
			printf("\n");
			printf("\n");
			printf("\n");
			printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
			printf("  \t\t请输入参与本游戏的人数：");
			scanf("%d",&numperson);
			printf("  \t\t围成圈（顺时针）站好后的人名：\n");
			final=enter();
			printf("  \t\t请输入奖品总数（最少三个）：");
			do
			{
				if(numprize<3)
				{
					printf("  \t\t数量不足，请重新输入：");
				}
				scanf("%d",&numprize);
			}while(numprize<3);//奖品最少三个
			printf("  \t\t请输入奖品信息（奖品名字 奖品价格）\n");
			wholeprize();
			printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
			Sleep(300);
			system("cls");
			information();
			saveinformation();
			gameplay();
		}
		else if(c=='2')
		{
			read();
			if((c=getch())=='1')
			{
				seek();
			}
		}
		else if(c=='3')
		{
			//backspace();
			del();
		}
		else if(c=='4')
		{
			end();
			break;
		}
		else
		{
			error();
		}
		system("pause");
		system("cls");
	}
	return 0;
}


/*-------------------------------------------------------人员信息输入-------------------------------------------------------------------*/
Person * enter()
{
	int number;//用来暂时存参与游戏人员的编号
	char name[30];//用来暂时存参与游戏人员的姓名
	Person *p;//用于指向新创建的内存
	Person *last;//当前指针位置
	printf("  \t\t");
	for(number=1;number<numperson+1;number++)
	{
		scanf("%s",name);
		p=(Person*)malloc(sizeof(Person));//动态分配内存
		p->next=NULL;
		strcpy(p->name,name);
		p->number=number;
		last=personhead;
		if(last!=NULL)//判断是否存在头结点
		{
			while(last->next!=NULL)
			{
				last=last->next;
			}
			last->next=p;
		}
		else
		{
			personhead=p;
		}
	}
	p->next=personhead;
	return p;//返回圆环最后一人用于游戏中一种特殊情况：当每次淘汰第一人时
}


/*-------------------------------------------------------奖品池信息输入-----------------------------------------------------------------*/
void wholeprize()
{
	int number;//暂时存放奖品编号
	char name[30];//暂时存放奖品名称
	int price;//暂时存放奖品价格
	Prize * p;//用于创建内存指向所创建内存首地址
	Prize * last;//当前指针位置
	for(number=1;number<numprize+1;number++)
	{
		printf("  \t\t");
		scanf("%s %d",name,&price);
		p=(Prize*)malloc(sizeof(Prize));//动态分布内存
		p->price=price;
		p->next=NULL;
		p->number=number;
		strcpy(p->name,name);
		last=prizehead;
		if(last!=NULL)//判断是否存在头结点
		{
			while(last->next!=NULL)//将“导向”指针从链表开头指向结尾
			{
				last=last->next;
			}
			last->next=p;
		}
		else
		{
			prizehead=p;
		}
	}
}


/*---------------------------------------------------------游戏过程---------------------------------------------------------------------*/
void gameplay()
{
	int k;//骰子随机数
	int i;//报数
	int j;//随机奖品编号
	Temporary temprize[3];//存放从奖品池中抽取的三个奖品信息
	Elimination * p;//用于指向创建的用来存取淘汰者的内存的首地址
	Elimination * last;//当前指针位置
	Person * q=NULL,*s=NULL;//q用于从链表中删除一个结构体，s用于释放已从链表删除的结构体的内存
	Person * again=personhead;//用于遍历全部参赛人员的链表，而不影响全局变量的指向
//	Person * _again;//用于存放最后剩余的幸运星的指针????????????????????????????
	Prize * _prizehead=prizehead;//用于遍历全部奖品池的链表，而不影响全局变量的指向
	FILE * fp;//存取指向存取总信息txt文档的指针
	FILE * fp2;//存取指向存取结束信息txt文档的指针
	FILE * fp3;//存取指向存取幸运星信息txt文档的指针
	Elimination * _peoplehead=NULL;
	Luck luck;//幸运星信息
	printf("┃\t\t游戏开始  ");
	k=1+rand()%6;//骰子数
	while(k>numperson/2)//防止骰子数大于人数的一半
	{
		k=1+rand()%6;
//		printf("%d ",k);
	}
//	printf("k正常");
	printf("  k=%d\t\t\t\t\t              ┃",k);
	while(again->next!=again)
	{
		for(i=1;i<k;i++)
		{
			q=again;
			again=again->next;
//			printf("%s *",again->name);
		}
		p=(Elimination*)malloc(sizeof(Elimination));
		p->number=again->number;
		strcpy(p->name,again->name);
		p->next=NULL;
		last=peoplehead;
		if(last!=NULL)
		{
			while(last->next!=NULL)
			{
				last=last->next;
			}
			last->next=p;
		}
		else
		{
			peoplehead=p;
			_peoplehead=peoplehead;
		}
//		if(k=1)
//		{
//			s=again;
//			again=again->next;
//			final->next=again;
//		}
//		else
//		{
			if(q!=NULL)
			{
				s=again;
				q->next=again->next;
				again=again->next;
			}
			else
			{
				s=again;
				again=again->next;
				final->next=again;
			}
//		}
		free(s);
//		printf("%s ",again->name);
	}
//	_again=again;??????????????????????
	printf("\n┃\t\t游戏结束\t\t\t\t\t              ┃\n");
	printf("┃\t\t被淘汰人名单：\t\t\t\t\t              ┃\n");

	fp=fopen("C:\\Program Files\\数据.txt","a");
	fprintf(fp,"被淘汰人名单：\n");
	fp2=fopen("C:\\Program Files\\排行榜数据.txt","a");
	fprintf(fp2,"\t\t    被淘汰人名单：\n");
	printf("┃                                                                            ┃\t\b\b\b\b\b\b\b\b\b");
	fprintf(fp2,"\t\t    ");
	for(;peoplehead;peoplehead=peoplehead->next)
	{
		printf("%d%s  ",peoplehead->number,peoplehead->name);
		fprintf(fp,"%d%s  ",peoplehead->number,peoplehead->name);
		fprintf(fp2,"%d%s  ",peoplehead->number,peoplehead->name);
	}
	printf("\n");
	printf("┃                                                                            ┃\t\b\b\b\b\b\b\b\b\b");
	printf("幸运星：%d%s\n",again->number,again->name);
	fprintf(fp,"\n幸运星：%d%s\n",again->number,again->name);
	fprintf(fp2,"\n\t\t    幸运星：%d%s\n",again->number,again->name);
	fp3=fopen("C:\\Program Files\\幸运星信息.txt","a");
	strcpy(luck.name,again->name);
	fprintf(fp3,"%s ",luck.name);
	printf("┃                                                                            ┃\t\b\b\b\b\b\b\b\b\b");
	printf("幸运星所获奖品：\n");
	fprintf(fp,"幸运星所获奖品：\n");
	fprintf(fp2,"\t\t    幸运星所获奖品：\n");
	for(i=0;i<3;i++)
	{
		j=1+rand()%numprize;//从奖品池中随机抽出三个奖品 
		for(;_prizehead->number!=j;_prizehead=_prizehead->next);//将指针从奖品池开头一直指到所抽到的奖品
		if(_prizehead->number==j)
		{
			temprize[i].number=_prizehead->number;
			temprize[i].price=_prizehead->price;
			strcpy(temprize[i].name,_prizehead->name);
			_prizehead=prizehead;
			strcpy(luck.prize[i],temprize[i].name);
			luck.price[i]=temprize[i].price;
			printf("┃                                                                            ┃\t\b\b\b\b\b\b\b\b\b");
			printf("%d---%s %d元\n",temprize[i].number,temprize[i].name,temprize[i].price);
			fprintf(fp,"%d---%s %d元\n",temprize[i].number,temprize[i].name,temprize[i].price);
			fprintf(fp2,"\t\t    %d---%s %d元\n",temprize[i].number,temprize[i].name,temprize[i].price);
			fprintf(fp3,"%s %d,",luck.prize[i],luck.price[i]);
//			fprintf(fp3,"%d---%s %d元\n",temprize[i].number,temprize[i].name,temprize[i].price);
		}
	}
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
//	fp=fopen("C:\\Program Files\\数据.txt","a");
//	fprintf(fp,"被淘汰人名单：\n");
//	for(;_peoplehead;_peoplehead=_peoplehead->next)
//	{
//		fprintf(fp,"%d%s  ",_peoplehead->number,_peoplehead->name);
//	}
//	fprintf(fp,"\n幸运星：%d%s\n",again->number,again->name);
//	fprintf(fp,"幸运星所获奖品：\n");
//	for(i=0;i<3;i++)
//	{
//		fprintf(fp,"%d---%s %d元\n",temprize[i].number,temprize[i].name,temprize[i].price);
//	}
	fprintf(fp,"****************************************************************************\n");
	fprintf(fp,"****************************************************************************\n");
	fprintf(fp,"****************************************************************************\n");
	fclose(fp);
//	fp2=fopen("C:\\Program Files\\排行榜数据.txt","a");
//	_peoplehead=peoplehead;
//	fprintf(fp2,"被淘汰人名单：\n");
//	for(;_peoplehead;_peoplehead=_peoplehead->next)
//	{
//		fprintf(fp2,"%d%s  ",_peoplehead->number,_peoplehead->name);
//	}
//	fprintf(fp2,"\n幸运星：%d%s\n",again->number,again->name);
//	fprintf(fp2,"幸运星所获奖品：\n");
//	for(i=0;i<3;i++)
//	{
//		fprintf(fp2,"%d---%s %d元\n",temprize[i].number,temprize[i].name,temprize[i].price);
//	}
	fprintf(fp2," ******************************************************************************\n");
	fclose(fp2);
	fclose(fp3);
}


/*-------------------------------------------------------游戏开始后所有信息展示----------------------------------------------------------*/
void information()//开局信息
{
	int number;//用于从第一个人开始到一圈的最后一个人
	Person * personlist=personhead;//建立用于移动的不影响头指针的指针
	Prize * prizelist=prizehead;//建立用于移动的不影响头指针的指针
//	printf();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃\t\t参赛人员：\t\t\t\t\t              ┃\n");
	printf("┃                                                                            ┃\t\b\b\b\b\b\b\b\b\b");
	for(number=1;number<=numperson;number++,personlist=personlist->next)
	{
	printf("%d%s  ",personlist->number,personlist->name);
	}
	printf("\n");
	printf("┃\t\t奖品清单：\t\t\t\t\t              ┃\n");
	for(number=1;prizelist;number++,prizelist=prizelist->next)
	{
	printf("┃                                                                            ┃\t\b\b\b\b\b\b\b\b\b%d--------%s  %d元"
		,number,prizelist->name,prizelist->price);
	printf("\n");
	}
	printf("┃----------------------------------------------------------------------------┃\n");
}


/*--------------------------------------------------参与人员和奖品池信息的存储-------------------------------------------------------------*/
void saveinformation()
{
	int number;//用于从第一个人开始到一圈的最后一个人
	Person * PERSONhead=personhead;//建立用于移动的不影响头指针的指针
	Prize * PRIZEhead=prizehead;//建立用于移动的不影响头指针的指针
	FILE * fp=fopen("C:\\Program Files\\数据.txt","a");
	if(fp==NULL)
	{
		printf("打开文件失败\n");
		exit(0);
	}
	fprintf(fp,"参赛人员：\n");
	for(number=1;number<=numperson;number++,PERSONhead=PERSONhead->next)
	{
	fprintf(fp,"%d%s",PERSONhead->number,PERSONhead->name);
	fprintf(fp,"\t");
	}
	fprintf(fp,"\n");
	fprintf(fp,"奖品清单：\n");
	for(number=1;PRIZEhead;number++,PRIZEhead=PRIZEhead->next)
	{
	fprintf(fp,"%d--------%s  %d元",number,PRIZEhead->name,PRIZEhead->price);
	fprintf(fp,"\n");
	}
	fprintf(fp,"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	fclose(fp);
}


/*--------------------------------------------------------开始界面--------------------------------------------------------------------*/
void gamebegin()
{
personhead=NULL; //人员头指针
prizehead=NULL; //奖品头指针
peoplehead=NULL; //淘汰人员头指针
final=NULL;//用于存放圆环最后一人
	printf("\n");
	printf("\n");
	printf("\n");
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                 1.开始游戏                                 ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                            2.显示幸运之星排行榜                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                             3.删除已存轮次信息                             ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                 4.退出游戏                                 ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
}


/*-------------------------------------------------------排行榜显示-------------------------------------------------------------------*/
void read()
{
	FILE * fp=fopen("C:\\Program Files\\排行榜数据.txt","r");
	char ch;
	if(fp==NULL)
	{
		printf("您可能未开始游戏或已删除信息\n");
	}
	else
	{
		printf("\n");
		printf("\n");
		printf("\n");
		printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t\t幸运之星排行榜\n");
		printf(" ------------------------------------------------------------------------------\n");
		fscanf(fp,"%c",&ch);
		while(!feof(fp))
		{
			putchar(ch);
			fscanf(fp,"%c",&ch);
		}
		printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
		printf("（按1根据姓名查询）");
		fclose(fp);
	}
}


/*----------------------------------------------------已存信息删除（旧不删文件）-----------------------------------------------------------*/
void backspace()
{
	FILE * fp;
	fp=fopen("C:\\Program Files\\数据.txt","w");
	fclose(fp);
	fp=fopen("C:\\Program Files\\排行榜数据.txt","w");
	fclose(fp);
	fp=fopen("C:\\Program Files\\幸运星信息.txt","w");
	fclose(fp);
}

void del()
{
	system("del \"C:\\Program Files\\数据.txt\"");
	system("del \"C:\\Program Files\\排行榜数据.txt\"");
	system("del \"C:\\Program Files\\幸运星信息.txt\"");//！！！！！！！！！！！！！！！！！dos命令含空格的处理
}


/*-------------------------------------------------------排行榜后的查找功能--------------------------------------------------------------*/
void seek()
{
	int i;
	FILE * fp;
	Luck * p=NULL;
	Luck * head=NULL;
	Luck * last=NULL;
	int count=0;
	char name[30];
	if((fp=fopen("C:\\Program Files\\幸运星信息.txt","r+"))==NULL)
	{
		printf("您可能未开始游戏或已删除信息");
		exit(0);
	}
	else
	{
		printf("请输入所要查询人名：");
		scanf("%s",name);
	//	printf("*****\n");
		while(!feof(fp))
		{
	//		printf("-----\n");
			p=(Luck*)malloc(sizeof(Luck));
			p->next=NULL;
			fscanf(fp,"%s ",p->name);
			for(i=0;i<3;i++)
			{
	//			printf("//////\n");
				fscanf(fp,"%s %d,",p->prize[i],&p->price[i]);
			}
			last=head;
	//		printf("ppppp\n");
			if(last!=NULL)//判断是否存在头结点
			{
	//			printf("+----+\n");
				while(last->next!=NULL)//将“导向”指针从链表开头指向结尾
				{
					last=last->next;
				}
				last->next=p;
	//			printf("--+--\n");
			}
			else
			{
				head=p;
			}
		}
	//	printf("+++++");
		fclose(fp);
		for(;head;head=head->next)
		{
			if(strcmp(name,head->name)==0)
			{
				count++;
				if(count==1)
				{
					printf("幸运之星\t奖品\n");
				}
				printf("%s\t\t%s%d %s%d %s%d\n",head->name,
					head->prize[0],head->price[0],
					head->prize[1],head->price[1],
					head->prize[2],head->price[2]);
			}
		}
		if(count==0)
		{
				printf("此人非幸运星\n");
		}
	}
}


/*-------------------------------------------------------输入错误界面-------------------------------------------------------------------*/
void error()
{
	printf("\n");
	printf("\n");
	printf("\n");
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                         您的输入有误，请重新输入！                         ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
}


/*--------------------------------------------------------游戏结束界面-------------------------------------------------------------------*/
void end()
{
	printf("\n");
	printf("\n");
	printf("\n");
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                谢谢使用！                                  ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┃                                                                            ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
}