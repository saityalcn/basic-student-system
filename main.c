#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_SIZE 30

typedef struct{
	short day;
	short month;
	short year;
}DATE;

typedef struct Student{
	int ID;
	char name[CHAR_SIZE];
	char surname[CHAR_SIZE];
	int numOfClasses;
	int totalCredit;
	struct Student *next;
	struct Student *prev;
}STUDENT;

typedef struct{
	char IDOfClass[10];
	char nameOfClass[CHAR_SIZE];
	int *idsOfStudents;
	int numOfStudents;
}CLASS;

typedef struct{
	int ID;
	char idOfClass[CHAR_SIZE];
	int idOfStudent;
	DATE dateOfLastProcess;
}CLAss;



void getStudentsFromDoc();
void getClassesFromDoc();
void menu();
void studentOperations();
void classOperations();
STUDENT* createStudent(FILE *fp);
void addStudent();
CLASS* createClass();
void selectClassOperations();
void addStudentToClass();
STUDENT* getStudentListOfClass();
CLASS* getClassesOfStudent();
void writeToDocs();
void appendToDocs();
void printStudentList(STUDENT** head);




int main(void){
	
	int totalNumOfCredit, totalNumOfClass;
	STUDENT *headOfStudentsList; 
	STUDENT *tailOfStudentsList;
	CLASS *headOfClassesList;
	

	headOfClassesList = NULL;
	headOfStudentsList = NULL;
	
	printf("Ogrencinin secebilecegi kredi sayisini giriniz: ");
	scanf("%d", &totalNumOfCredit);
	
	printf("Ogrencinin secebilecegi ders sayisini giriniz: ");
	scanf("%d", &totalNumOfClass);
	
	
	// Dosyalardan verilerin çekilmesi ve değişkenlere kaydedilmesi işlemleri menu'den once yapılmalıdır.
	getStudentsFromDoc(&headOfStudentsList);
	getClassesFromDoc(&headOfClassesList);
	
	menu();
	printStudentList(&headOfStudentsList);
	
	return 0;
}


void createList(STUDENT** head, STUDENT* (*function)(STUDENT **head)){
	
	if(*head == NULL){
		
	}
	
}


void getStudentsFromDoc(STUDENT** head){
	
	int lineNumberOfDoc;
	char c;
	FILE *fpForCounting;
	STUDENT *ptr;
	FILE *fp;
	char nameOfDoc[] = "ogrenciler.txt"; 
	
	fpForCounting = fopen(nameOfDoc, "r");
	
	lineNumberOfDoc = 0;
	
	for (c = getc(fpForCounting); c != EOF; c = getc(fpForCounting)){
    	if (c == '\n') 
        	lineNumberOfDoc++;
	}
	
	fclose(fpForCounting);
	
	fp = fopen(nameOfDoc, "r");
	
	if(fp == NULL){
		printf("HATA '%s' isimli dosya acilamadi", nameOfDoc);
	}
	
	
	while(lineNumberOfDoc > 0){
		// head' sp'nin pointerını atadığın için daha sonra atama yapmasan bile onun değerini alıyor.
		
		if(*head == NULL){
			*head = createStudent(fp);
		}
		
		else{
			
			ptr = *head; 
			
			while(ptr->next != NULL){
				ptr = ptr->next;
			}
			
			ptr->next = createStudent(fp);
		
		}
	
		lineNumberOfDoc--;
	}
	
	fclose(fp);

}

STUDENT* createStudent(FILE *fp){
	int i,totalCredit,numOfClasses;
	STUDENT* sp;
	char n[CHAR_SIZE];
	char sn[CHAR_SIZE];
	
	sp  = (STUDENT*)malloc( sizeof(STUDENT) );
			
	fscanf(fp,"%d, %s , %s ,%d,%d\n",&i,n,sn,&totalCredit,&numOfClasses);
				
	sp->ID = i;
	strcpy(sp->name, n);
	strcpy(sp->surname, sn);
	sp->totalCredit = totalCredit;
	sp->numOfClasses = numOfClasses;
	
	
	return sp;
	
}

void getClassesFromDoc(CLASS** head){
	int credit;
	char n[CHAR_SIZE];
	char sn[CHAR_SIZE];
	FILE* fp;
	CLASS* cp;
	STUDENT* ptr;
	char nameOfDoc[] = "deneme.txt"; 
	
}


void menu(){
	int menuInput = 1;
	
	while(menuInput != 0){
		printf("Menu\n");
		printf("Ogrenci islemleri icin 1,\n");
		printf("Ders islemleri icin 2,\n");
		printf("Ders Secimi islemleri icin 3,\nCikmak icin 0 giriniz.\n");
		printf("Seciminiz: ");
		scanf("%d", &menuInput);
		printf("\n");
		
		if(menuInput == 1)
			studentOperations();
		
		else if(menuInput == 2)
			classOperations();
			
		else if(menuInput == 3)
			selectClassOperations();
			
		else if(menuInput != 0){
			printf("Hatali secim yaptiniz tekrar deneyiniz.\n");
		}
	}
		
}

void studentOperations(){
	int studentOperationsMenuInput = 1;
	
	while(studentOperationsMenuInput != 0){
		printf("Ogrenci eklemek icin 1\n");
		printf("Ogrenci silmek icin 2\n");
		printf("Ogrencinin aldigi dersleri yazdirmak icin 3\n");
		printf("Cikmak icin 0 giriniz\nSeciminiz: ");
		scanf("%d", &studentOperationsMenuInput);
		printf("\n");		
	}
	
}


void classOperations(){
	int classOperationsMenuInput = 1;
	
	while(classOperationsMenuInput != 0){
		printf("Ders acmak icin 1,\n");
		printf("Ders kapatmak icin 2,\n");
		printf("Dersi alan ogrencileri yazdirmak icin 3\n");
		printf("Cikmak icin 0 giriniz\nSeciminiz: ");
		scanf("%d", &classOperationsMenuInput);
		printf("\n");	
	}
	
}

void selectClassOperations(){
	int selectClassOperationsMenuInput = 1;
	
	while(selectClassOperationsMenuInput != 0){
		printf("Ders secmek icin 1,\n");
		printf("Secili bir dersi silmek icin 2,\n");
		printf("Cikmak icin 0 giriniz\nSeciminiz: ");
		scanf("%d", &selectClassOperationsMenuInput);
		printf("\n");
	}
}


void printStudentList(STUDENT **head){
	STUDENT *ptr;
	int i = 0;
	
	if(*head == NULL){
		printf("Head NULL");
		return;
	}
	
	printf("ID\t\tName\tSurname\tNum Of Classes\tTotal Credit\n");
	
	ptr = *head;

	printf("%d\t%s\t%s\t%d\t\t%d\n", (*head)->ID, (*head)->name, (*head)->surname, (*head)->numOfClasses, (*head)->totalCredit);

}

/*

STUDENT* createStudent(){
	
		
	int i,totalCredit,numOfClasses;
	char n[CHAR_SIZE];
	char sn[CHAR_SIZE];
	STUDENT *sp;
	STUDENT *ptr;
	
	sp = (STUDENT*)malloc(sizeof(STUDENT));
	
	printf("Enter id of student: ");
	scanf("%d", &i);
	
	printf("Enter name of student: ");
	scanf("%s", n);
	
	printf("Enter surname of student: ");
	scanf("%d", sn);

	sp->ID  = i;
	strcpy(sp->name, n);
	strcpy(sp->surname, sn);
	sp->totalCredit = 0;
	sp->numOfClasses = 0;
	
	
	return sp;	

}
*/
