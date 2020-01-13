#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>

//������Ա����ż�״̬
typedef struct person
{
	int number;
	char name[30];
	int status;
}Person;

//��Ʒ��Ϣ
typedef struct prize
{
	int number;
	char name[30];
	int price;
}Prize;

//�ִν�Ʒ
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
	srand((unsigned)time(NULL)); //���
	system("mode con cols=81");
	while(1){
		system("cls");
		gamebegin();
		printf("��ѡ���Ӧ��ŵĲ���:");
		fflush(stdin);
		c=getch();
		system("cls");
		if(c=='4'){
			printf("ллʹ��!");
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
	printf("��������������������������������������������������������������������������������\n");
	printf("                                                                                \n");
	printf("                                   1.��ʼ��Ϸ                                   \n");
	printf("                                                                                \n");
	printf("                                                                                \n");
	printf("                               2.��ʾ�Ѵ��ִ���Ϣ                               \n");
	printf("                                                                                \n");
	printf("                                                                                \n");
	printf("                               3.ɾ���Ѵ��ִ���Ϣ                               \n");
	printf("                                                                                \n");
	printf("                                                                                \n");
	printf("                                   4.�˳���Ϸ                                   \n");
	printf("                                                                                \n");
	printf("��������������������������������������������������������������������������������\n");
}

void gameprogress()
{
	int numluck;
	int k;//������
	inputperson();
	inputprize();
	information();
	k=1+rand()%6;
	while(k>numperson/2){
		k=1+rand()%6;
	}
	printf("������:%d\n",k);
	numluck=gameplay(k);
	winnerinformation(numluck);
}

void information()
{
	FILE *fp;
	int i;
	fp=fopen("C:\\Program Files\\��Ϣ.txt","a");
	fprintf(fp,"������:\n");
	for(i=0;i<numperson;i++){
		fprintf(fp,"No.%d%s  ",person[i].number,person[i].name);//��ţ�����
	}
	fprintf(fp,"\n��Ʒ��:\n");
	for(i=0;i<numprize;i++){
		fprintf(fp,"No.%d%s %d��  ",prize[i].number,prize[i].name,prize[i].price);//��ţ����ƣ��۸�
	}
	fclose(fp);
}
void winnerinformation(int numluck)
{
	FILE *fp;
	fp=fopen("C:\\Program Files\\��Ϣ.txt","a");
	fprintf(fp,"\n������:\n");
	fprintf(fp,"%s %s%d�� %s%d�� %s%d��",person[numluck-1].name,
									  temprize[0].name,temprize[0].price,
									  temprize[1].name,temprize[1].price,
									  temprize[2].name,temprize[2].price);
	fprintf(fp,"\n");
	fclose(fp);
}

void delinformation()
{
	FILE * fp=fopen("C:\\Program Files\\��Ϣ.txt","w");
	fclose(fp);
}

void inputperson()
{
	int i;
	printf("�������������:");
	scanf("%d",&numperson);
	person=(Person*)malloc(sizeof(Person)*numperson);
	printf("�����������������ϵͳ�������ţ�");
	for(i=0;i<numperson;i++){
		scanf("%s",person[i].name);
		person[i].number=i+1;
		person[i].status=0;
	}
}

void inputprize()
{
	int i;
	printf("�����뽱Ʒ����:");
	scanf("%d",&numprize);
	prize=(Prize*)malloc(sizeof(Prize)*numprize);
	printf("�����뽱Ʒ��Ϣ�����ơ��۸�\n");
	for(i=0;i<numprize;i++){
		scanf("%s%d",prize[i].name,&prize[i].price);
		prize[i].number=i+1;
	}
}


int gameplay(int k)
{
	int numluck;
	int number=1;//����
	int i=0;
	int j;//�����Ʒ��
	int oddperson=numperson;//ʣ������
	printf("��̭��:\n");
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
			printf("������:%s\n",person[i].name);
			break;
		}
	}
	for(i=0;i<3;i++){
		j=rand()%numprize;
		temprize[i].number=prize[j].number;
		temprize[i].price=prize[j].price;
		strcpy(temprize[i].name,prize[j].name);
	}
	printf("����������Ʒ:\n");
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
		printf("����ѡ������������ѡ��!\n");
			system("pause");
	}
}

void putinformation()
{
	FILE*fp=fopen("C:\\Program Files\\��Ϣ.txt","r");
	char ch;
	if(fp==NULL)
	{
		printf("������δ��ʼ��Ϸ����ɾ����Ϣ\n");
	}
	else
	{
		printf("��������������������������������������������������������������������������������\n");
		fscanf(fp,"%c",&ch);
		while(!feof(fp))
		{
			putchar(ch);
			fscanf(fp,"%c",&ch);
		}
		printf("��������������������������������������������������������������������������������\n");
	}
}