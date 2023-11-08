#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <malloc.h>
#define buffersize 5
int processnum = 0;
struct pcb
{
	int flag;
	int numlabel;
	char product;
	char state;
	struct pcb* processlink;
}*exe = NULL, * over = NULL;
typedef struct pcb PCB;
PCB* readyhead = NULL, * readytail = NULL;
PCB* consumerhead = NULL, * consumertail = NULL;
PCB* producerhead = NULL, * producertail = NULL;
int productnum = 0;
int full = 0, empty = buffersize;
char buffer[buffersize];
int bufferpoint = 0;
void linklist(PCB* p, PCB* listhead)
{
	PCB* cursor = listhead;
	while (cursor->processlink != NULL)
	{
		cursor = cursor->processlink;
	}
	cursor->processlink = p;
}
void freelink(PCB* linkhead)
{
	PCB* p;
	while (linkhead != NULL)
	{
		p = linkhead;
		linkhead = linkhead->processlink;
		free(p);
	}
}
void linkqueue(PCB* process, PCB** tail)
{
	if ((*tail) != NULL)
	{
		(*tail)->processlink = process;
		(*tail) = process;
	}
	else
	{
		printf("����δ��ʼ��");
	}
}
PCB* getq(PCB* head, PCB** tail)
{
	PCB* p;
	p = head->processlink;
	if (p != NULL)
	{
		head->processlink = p->processlink;
		p->processlink = NULL;
		if (head->processlink == NULL)
		{
			(*tail) = head;
		}
	}
	else return NULL;
	return p;
}
bool processproc()
{
	int i, f, num;
	char ch;
	PCB* p = NULL;
	PCB** p1 = NULL;
	printf("\n������ϣ�������Ľ��̸���: ");
	scanf("%d", &num);
	getchar();
	for (i = 0; i < num; i++)
	{
		printf("\n��������Ҫ�����Ľ���: ����1Ϊ�����߽���;����2Ϊ�����߽���\n");
		scanf("%d", &f);
		getchar();
		p = (PCB*)malloc(sizeof(PCB));
		if (!p)
		{
			printf("�ڴ����ʧ��");
			return false;
		}
		p->flag = f;
		processnum++;
		p->numlabel = processnum;
		p->state = 'w';
		p->processlink = NULL;
		if (p->flag == 1)
		{
			printf("��Ҫ�����Ľ�����������,���ǵ�%d������. ����������Ҫ�ý��̲������ַ�:\n", processnum);
			scanf("%c", &ch);
			getchar();
			p->product = ch;
			processnum++;
			printf("��Ҫ�ý��̲������ַ���%c\n", p->product);
		}
		else { printf("��Ҫ�����Ľ��������ߣ����ǵ�%d�����̡�\n", p->numlabel); }
		linkqueue(p, &readytail);
	}
	return true;
}
bool hasElement(PCB* pro)
{

	if (pro->processlink == NULL)return false;
	else return true;
}
bool waitempty()
{
	if (empty <= 0)
	{
		printf("����%d�������������������������ó̽��������ߵȴ�����\n", exe->numlabel);
		linkqueue(exe, &producertail);
		return false;
	}
	else {
		empty--;
		return true;
	}
}
void signalempty() {
	PCB* p;
	if (hasElement(producerhead)) {
		p = getq(producerhead, &producertail);
		linkqueue(p, &readytail);
		printf("�ȴ��������߽��̽���������У������̺���%d\n", p->numlabel);

	}
	empty++;
}
bool waitfull()
{
	if (full <= 0)
	{
		printf("����%d��������ȡ�����������գ��ó̽��������ߵȴ�����\n", exe->numlabel);
		linkqueue(exe, &consumertail);
		return false;
	}
	else {
		full--;
		return true;
	}
}

void signalfull()
{
	PCB* p;
	if (hasElement(consumerhead)) {
		p = getq(consumerhead, &consumertail);
		linkqueue(p, &readytail);
		printf("�ȴ��������߽��̽����������У��Ľ��̺���%d\n", p->numlabel);
	}
	full++;
}
void producerrun()
{
	if (!waitempty())return;
	printf("����%d��ʼ�򻺳�������%c\n", exe->numlabel, exe->product);
	buffer[bufferpoint] = exe->product;
	bufferpoint++;
	printf("����%d�򻺳���������������\n", exe->numlabel);
	signalfull();
	linklist(exe, over);

}

void comsuerrun()
{
	if (!waitfull())return;
	printf("����%d��ʼ�򻺳���ȡ��\n", exe->numlabel);
	exe->product = buffer[bufferpoint - 1];
	bufferpoint--;
	printf("����%d�򻺳���ȡ������������ȡ����%\n", exe->numlabel, exe->product);
	signalempty();
	linklist(exe, over);
}
void display(PCB* p)
{
	p = p->processlink;
	while (p != NULL) {
		printf("����%d������һ��", p->numlabel);
		p->flag == 1 ? printf("������\n") : printf("������\n");
		p = p->processlink;
	}
}

int main()
{
	char terminate;
	bool element;
	printf("���뿪ʼ������(y/n)");
	scanf("%c", &terminate);
	getchar();
	readyhead = (PCB*)malloc(sizeof(PCB));
	if (readyhead == NULL)
	return true;

	readytail = readyhead;
	readyhead->flag = 3;
	readyhead->numlabel = processnum;
	readyhead->state = 'w';
	readyhead->processlink = NULL;
	consumerhead = (PCB*)malloc(sizeof(PCB));
	if (consumerhead == NULL) return true;
	consumertail = consumerhead;
	consumerhead->processlink = NULL;
	consumerhead->flag = 4;
	consumerhead->numlabel = processnum;
	consumerhead->state = 'w';
	consumerhead->processlink = NULL;
	producerhead = (PCB*)malloc(sizeof(PCB));
	if (producerhead == NULL) return true;
	producertail = producerhead;
	producerhead->processlink = NULL;
	producerhead->flag = 5;
	producerhead->numlabel = processnum;
	producerhead->state = 'w';
	producerhead->processlink = NULL;
	over = (PCB*)malloc(sizeof(PCB));
	if (over == NULL) return true;
	over->processlink = NULL;
	while (terminate == 'y')
	{
		if (!processproc()) break;
		element = hasElement(readyhead);
		while (element)
		{
			exe = getq(readyhead, &readytail);
			printf("����%d�������У�����һ��", exe->numlabel);
			exe->flag == 1 ? printf("������\n") :
				printf("������\n");
			if (exe->flag == 1) producerrun();
			else comsuerrun();
			element = hasElement(readyhead);
		}
		printf("��������û�н���\n");
		if (hasElement(consumerhead))
		{
			printf("�����ߵȴ��������н���:\n");
			if (hasElement(producerhead))
			{
				printf("�����ߵȴ��������н���:\n");
				display(producerhead);
			}
			else
			{
				printf("�����ߵȴ�������û�н���:\n");
			}
			printf("���������(press'y for on)");
			scanf("%c", &terminate);
			getchar();
		}
		printf("\n\n����ģ�����.\n");
		freelink(over);
		over = NULL;
		freelink(readyhead);
		readyhead = NULL;
		readytail = NULL;
		freelink(consumerhead);
		consumerhead = NULL;
		consumertail = NULL;
		freelink(producerhead);
		producerhead = NULL;
		producertail = NULL;
		getchar();
	}
}
