#include<windows.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


/*--------------------------------------------------------�ṹ��Ľ���-------------------------------------------------------------------*/
//������Ա�����
typedef struct person
{
	int number;
	char name[30];

	struct person *next;
}Person;

//��Ʒ��Ϣ
typedef struct prize
{
	int number;
	char name[30];
	int price;
	struct prize *next;
}Prize;

//��̭��Ա
typedef struct eliminate
{
	int number;
	char name[30];
	struct eliminate *next;
}Elimination;

//�ִν�Ʒ
typedef struct temporaryprize
{
	int number;
	char name[30];
	int price;
}Temporary;

//����֮��
typedef struct luck
{
	char name[30];
	char prize[3][30];
	int price[3];
	struct luck * next;
}Luck;


/*--------------------------------------------------------------ȫ�ֱ����Լ�����������------------------------------------------------------*/
int numperson,numprize=3; //������Ϸ��������Ʒ����
Person * personhead=NULL; //��Աͷָ��
Prize * prizehead=NULL; //��Ʒͷָ��
Elimination * peoplehead=NULL; //��̭��Աͷָ��
Person * final=NULL;//���ڴ��Բ�����һ��
Person * enter(); //���������Ϸ��Ա��Ϣ
void wholeprize(); //���뽱Ʒ����Ϣ
void gamebegin();//��ʼ����
void gameplay(); //��Ϸ����
void information();//������Ϣ
void saveinformation();//�洢��Ϣ
void read();//��ʾ���а�
void backspace();//ɾ����Ϣ
void seek();//����ѧ������״̬
void error();//�˵���������
void end();//����
void del();//ɾ���ļ�
void load();//������Ϣ-----------��д��


/*------------------------------------------------------------������------------------------------------------------------------------*/
int main()
{
	char c;//ѡ��
	srand((unsigned)time(NULL)); //���
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
			printf("��������������������������������������������������������������������������������\n");
			printf("  \t\t��������뱾��Ϸ��������");
			scanf("%d",&numperson);
			printf("  \t\tΧ��Ȧ��˳ʱ�룩վ�ú��������\n");
			final=enter();
			printf("  \t\t�����뽱Ʒ������������������");
			do
			{
				if(numprize<3)
				{
					printf("  \t\t�������㣬���������룺");
				}
				scanf("%d",&numprize);
			}while(numprize<3);//��Ʒ��������
			printf("  \t\t�����뽱Ʒ��Ϣ����Ʒ���� ��Ʒ�۸�\n");
			wholeprize();
			printf("��������������������������������������������������������������������������������\n");
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


/*-------------------------------------------------------��Ա��Ϣ����-------------------------------------------------------------------*/
Person * enter()
{
	int number;//������ʱ�������Ϸ��Ա�ı��
	char name[30];//������ʱ�������Ϸ��Ա������
	Person *p;//����ָ���´������ڴ�
	Person *last;//��ǰָ��λ��
	printf("  \t\t");
	for(number=1;number<numperson+1;number++)
	{
		scanf("%s",name);
		p=(Person*)malloc(sizeof(Person));//��̬�����ڴ�
		p->next=NULL;
		strcpy(p->name,name);
		p->number=number;
		last=personhead;
		if(last!=NULL)//�ж��Ƿ����ͷ���
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
	return p;//����Բ�����һ��������Ϸ��һ�������������ÿ����̭��һ��ʱ
}


/*-------------------------------------------------------��Ʒ����Ϣ����-----------------------------------------------------------------*/
void wholeprize()
{
	int number;//��ʱ��Ž�Ʒ���
	char name[30];//��ʱ��Ž�Ʒ����
	int price;//��ʱ��Ž�Ʒ�۸�
	Prize * p;//���ڴ����ڴ�ָ���������ڴ��׵�ַ
	Prize * last;//��ǰָ��λ��
	for(number=1;number<numprize+1;number++)
	{
		printf("  \t\t");
		scanf("%s %d",name,&price);
		p=(Prize*)malloc(sizeof(Prize));//��̬�ֲ��ڴ�
		p->price=price;
		p->next=NULL;
		p->number=number;
		strcpy(p->name,name);
		last=prizehead;
		if(last!=NULL)//�ж��Ƿ����ͷ���
		{
			while(last->next!=NULL)//��������ָ�������ͷָ���β
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


/*---------------------------------------------------------��Ϸ����---------------------------------------------------------------------*/
void gameplay()
{
	int k;//���������
	int i;//����
	int j;//�����Ʒ���
	Temporary temprize[3];//��Ŵӽ�Ʒ���г�ȡ��������Ʒ��Ϣ
	Elimination * p;//����ָ�򴴽���������ȡ��̭�ߵ��ڴ���׵�ַ
	Elimination * last;//��ǰָ��λ��
	Person * q=NULL,*s=NULL;//q���ڴ�������ɾ��һ���ṹ�壬s�����ͷ��Ѵ�����ɾ���Ľṹ����ڴ�
	Person * again=personhead;//���ڱ���ȫ��������Ա����������Ӱ��ȫ�ֱ�����ָ��
//	Person * _again;//���ڴ�����ʣ��������ǵ�ָ��????????????????????????????
	Prize * _prizehead=prizehead;//���ڱ���ȫ����Ʒ�ص���������Ӱ��ȫ�ֱ�����ָ��
	FILE * fp;//��ȡָ���ȡ����Ϣtxt�ĵ���ָ��
	FILE * fp2;//��ȡָ���ȡ������Ϣtxt�ĵ���ָ��
	FILE * fp3;//��ȡָ���ȡ��������Ϣtxt�ĵ���ָ��
	Elimination * _peoplehead=NULL;
	Luck luck;//��������Ϣ
	printf("��\t\t��Ϸ��ʼ  ");
	k=1+rand()%6;//������
	while(k>numperson/2)//��ֹ����������������һ��
	{
		k=1+rand()%6;
//		printf("%d ",k);
	}
//	printf("k����");
	printf("  k=%d\t\t\t\t\t              ��",k);
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
	printf("\n��\t\t��Ϸ����\t\t\t\t\t              ��\n");
	printf("��\t\t����̭��������\t\t\t\t\t              ��\n");

	fp=fopen("C:\\Program Files\\����.txt","a");
	fprintf(fp,"����̭��������\n");
	fp2=fopen("C:\\Program Files\\���а�����.txt","a");
	fprintf(fp2,"\t\t    ����̭��������\n");
	printf("��                                                                            ��\t\b\b\b\b\b\b\b\b\b");
	fprintf(fp2,"\t\t    ");
	for(;peoplehead;peoplehead=peoplehead->next)
	{
		printf("%d%s  ",peoplehead->number,peoplehead->name);
		fprintf(fp,"%d%s  ",peoplehead->number,peoplehead->name);
		fprintf(fp2,"%d%s  ",peoplehead->number,peoplehead->name);
	}
	printf("\n");
	printf("��                                                                            ��\t\b\b\b\b\b\b\b\b\b");
	printf("�����ǣ�%d%s\n",again->number,again->name);
	fprintf(fp,"\n�����ǣ�%d%s\n",again->number,again->name);
	fprintf(fp2,"\n\t\t    �����ǣ�%d%s\n",again->number,again->name);
	fp3=fopen("C:\\Program Files\\��������Ϣ.txt","a");
	strcpy(luck.name,again->name);
	fprintf(fp3,"%s ",luck.name);
	printf("��                                                                            ��\t\b\b\b\b\b\b\b\b\b");
	printf("����������Ʒ��\n");
	fprintf(fp,"����������Ʒ��\n");
	fprintf(fp2,"\t\t    ����������Ʒ��\n");
	for(i=0;i<3;i++)
	{
		j=1+rand()%numprize;//�ӽ�Ʒ����������������Ʒ 
		for(;_prizehead->number!=j;_prizehead=_prizehead->next);//��ָ��ӽ�Ʒ�ؿ�ͷһֱָ�����鵽�Ľ�Ʒ
		if(_prizehead->number==j)
		{
			temprize[i].number=_prizehead->number;
			temprize[i].price=_prizehead->price;
			strcpy(temprize[i].name,_prizehead->name);
			_prizehead=prizehead;
			strcpy(luck.prize[i],temprize[i].name);
			luck.price[i]=temprize[i].price;
			printf("��                                                                            ��\t\b\b\b\b\b\b\b\b\b");
			printf("%d---%s %dԪ\n",temprize[i].number,temprize[i].name,temprize[i].price);
			fprintf(fp,"%d---%s %dԪ\n",temprize[i].number,temprize[i].name,temprize[i].price);
			fprintf(fp2,"\t\t    %d---%s %dԪ\n",temprize[i].number,temprize[i].name,temprize[i].price);
			fprintf(fp3,"%s %d,",luck.prize[i],luck.price[i]);
//			fprintf(fp3,"%d---%s %dԪ\n",temprize[i].number,temprize[i].name,temprize[i].price);
		}
	}
	printf("��������������������������������������������������������������������������������\n");
//	fp=fopen("C:\\Program Files\\����.txt","a");
//	fprintf(fp,"����̭��������\n");
//	for(;_peoplehead;_peoplehead=_peoplehead->next)
//	{
//		fprintf(fp,"%d%s  ",_peoplehead->number,_peoplehead->name);
//	}
//	fprintf(fp,"\n�����ǣ�%d%s\n",again->number,again->name);
//	fprintf(fp,"����������Ʒ��\n");
//	for(i=0;i<3;i++)
//	{
//		fprintf(fp,"%d---%s %dԪ\n",temprize[i].number,temprize[i].name,temprize[i].price);
//	}
	fprintf(fp,"****************************************************************************\n");
	fprintf(fp,"****************************************************************************\n");
	fprintf(fp,"****************************************************************************\n");
	fclose(fp);
//	fp2=fopen("C:\\Program Files\\���а�����.txt","a");
//	_peoplehead=peoplehead;
//	fprintf(fp2,"����̭��������\n");
//	for(;_peoplehead;_peoplehead=_peoplehead->next)
//	{
//		fprintf(fp2,"%d%s  ",_peoplehead->number,_peoplehead->name);
//	}
//	fprintf(fp2,"\n�����ǣ�%d%s\n",again->number,again->name);
//	fprintf(fp2,"����������Ʒ��\n");
//	for(i=0;i<3;i++)
//	{
//		fprintf(fp2,"%d---%s %dԪ\n",temprize[i].number,temprize[i].name,temprize[i].price);
//	}
	fprintf(fp2," ******************************************************************************\n");
	fclose(fp2);
	fclose(fp3);
}


/*-------------------------------------------------------��Ϸ��ʼ��������Ϣչʾ----------------------------------------------------------*/
void information()//������Ϣ
{
	int number;//���ڴӵ�һ���˿�ʼ��һȦ�����һ����
	Person * personlist=personhead;//���������ƶ��Ĳ�Ӱ��ͷָ���ָ��
	Prize * prizelist=prizehead;//���������ƶ��Ĳ�Ӱ��ͷָ���ָ��
//	printf();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("��������������������������������������������������������������������������������\n");
	printf("��\t\t������Ա��\t\t\t\t\t              ��\n");
	printf("��                                                                            ��\t\b\b\b\b\b\b\b\b\b");
	for(number=1;number<=numperson;number++,personlist=personlist->next)
	{
	printf("%d%s  ",personlist->number,personlist->name);
	}
	printf("\n");
	printf("��\t\t��Ʒ�嵥��\t\t\t\t\t              ��\n");
	for(number=1;prizelist;number++,prizelist=prizelist->next)
	{
	printf("��                                                                            ��\t\b\b\b\b\b\b\b\b\b%d--------%s  %dԪ"
		,number,prizelist->name,prizelist->price);
	printf("\n");
	}
	printf("��----------------------------------------------------------------------------��\n");
}


/*--------------------------------------------------������Ա�ͽ�Ʒ����Ϣ�Ĵ洢-------------------------------------------------------------*/
void saveinformation()
{
	int number;//���ڴӵ�һ���˿�ʼ��һȦ�����һ����
	Person * PERSONhead=personhead;//���������ƶ��Ĳ�Ӱ��ͷָ���ָ��
	Prize * PRIZEhead=prizehead;//���������ƶ��Ĳ�Ӱ��ͷָ���ָ��
	FILE * fp=fopen("C:\\Program Files\\����.txt","a");
	if(fp==NULL)
	{
		printf("���ļ�ʧ��\n");
		exit(0);
	}
	fprintf(fp,"������Ա��\n");
	for(number=1;number<=numperson;number++,PERSONhead=PERSONhead->next)
	{
	fprintf(fp,"%d%s",PERSONhead->number,PERSONhead->name);
	fprintf(fp,"\t");
	}
	fprintf(fp,"\n");
	fprintf(fp,"��Ʒ�嵥��\n");
	for(number=1;PRIZEhead;number++,PRIZEhead=PRIZEhead->next)
	{
	fprintf(fp,"%d--------%s  %dԪ",number,PRIZEhead->name,PRIZEhead->price);
	fprintf(fp,"\n");
	}
	fprintf(fp,"����������������������������������������������������������������������������\n");
	fclose(fp);
}


/*--------------------------------------------------------��ʼ����--------------------------------------------------------------------*/
void gamebegin()
{
personhead=NULL; //��Աͷָ��
prizehead=NULL; //��Ʒͷָ��
peoplehead=NULL; //��̭��Աͷָ��
final=NULL;//���ڴ��Բ�����һ��
	printf("\n");
	printf("\n");
	printf("\n");
	printf("��������������������������������������������������������������������������������\n");
	printf("��                                                                            ��\n");
	printf("��                                 1.��ʼ��Ϸ                                 ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                            2.��ʾ����֮�����а�                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                             3.ɾ���Ѵ��ִ���Ϣ                             ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                 4.�˳���Ϸ                                 ��\n");
	printf("��                                                                            ��\n");
	printf("��������������������������������������������������������������������������������\n");
}


/*-------------------------------------------------------���а���ʾ-------------------------------------------------------------------*/
void read()
{
	FILE * fp=fopen("C:\\Program Files\\���а�����.txt","r");
	char ch;
	if(fp==NULL)
	{
		printf("������δ��ʼ��Ϸ����ɾ����Ϣ\n");
	}
	else
	{
		printf("\n");
		printf("\n");
		printf("\n");
		printf("��������������������������������������������������������������������������������\n");
		printf("\t\t\t\t����֮�����а�\n");
		printf(" ------------------------------------------------------------------------------\n");
		fscanf(fp,"%c",&ch);
		while(!feof(fp))
		{
			putchar(ch);
			fscanf(fp,"%c",&ch);
		}
		printf("��������������������������������������������������������������������������������\n");
		printf("����1����������ѯ��");
		fclose(fp);
	}
}


/*----------------------------------------------------�Ѵ���Ϣɾ�����ɲ�ɾ�ļ���-----------------------------------------------------------*/
void backspace()
{
	FILE * fp;
	fp=fopen("C:\\Program Files\\����.txt","w");
	fclose(fp);
	fp=fopen("C:\\Program Files\\���а�����.txt","w");
	fclose(fp);
	fp=fopen("C:\\Program Files\\��������Ϣ.txt","w");
	fclose(fp);
}

void del()
{
	system("del \"C:\\Program Files\\����.txt\"");
	system("del \"C:\\Program Files\\���а�����.txt\"");
	system("del \"C:\\Program Files\\��������Ϣ.txt\"");//����������������������������������dos����ո�Ĵ���
}


/*-------------------------------------------------------���а��Ĳ��ҹ���--------------------------------------------------------------*/
void seek()
{
	int i;
	FILE * fp;
	Luck * p=NULL;
	Luck * head=NULL;
	Luck * last=NULL;
	int count=0;
	char name[30];
	if((fp=fopen("C:\\Program Files\\��������Ϣ.txt","r+"))==NULL)
	{
		printf("������δ��ʼ��Ϸ����ɾ����Ϣ");
		exit(0);
	}
	else
	{
		printf("��������Ҫ��ѯ������");
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
			if(last!=NULL)//�ж��Ƿ����ͷ���
			{
	//			printf("+----+\n");
				while(last->next!=NULL)//��������ָ�������ͷָ���β
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
					printf("����֮��\t��Ʒ\n");
				}
				printf("%s\t\t%s%d %s%d %s%d\n",head->name,
					head->prize[0],head->price[0],
					head->prize[1],head->price[1],
					head->prize[2],head->price[2]);
			}
		}
		if(count==0)
		{
				printf("���˷�������\n");
		}
	}
}


/*-------------------------------------------------------����������-------------------------------------------------------------------*/
void error()
{
	printf("\n");
	printf("\n");
	printf("\n");
	printf("��������������������������������������������������������������������������������\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                         ���������������������룡                         ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��������������������������������������������������������������������������������\n");
}


/*--------------------------------------------------------��Ϸ��������-------------------------------------------------------------------*/
void end()
{
	printf("\n");
	printf("\n");
	printf("\n");
	printf("��������������������������������������������������������������������������������\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                ллʹ�ã�                                  ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��                                                                            ��\n");
	printf("��������������������������������������������������������������������������������\n");
}