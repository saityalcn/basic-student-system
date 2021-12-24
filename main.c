#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHAR_SIZE 50

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

typedef struct Class{
	char ID[10];
	char name[CHAR_SIZE];
	int credit;
	int capacity;
	int *idsOfStudents;
	int numOfStudents;
	struct Class* next;
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
STUDENT* getStudentInformationFromFile(FILE *fp);
void addStudent(STUDENT** head, STUDENT** tail);
CLASS* getClassInformationFromFile(FILE *fp);
void sortList();
STUDENT* createStudent();
void selectClassOperations();
void addStudentToClass();
STUDENT* getStudentListOfClass();
CLASS* getClassesOfStudent();
void writeToDocs();
void appendToDocs();
void printStudentList(STUDENT** head);
void printStudentListV2(STUDENT** tail);
void printClassesList(CLASS** head);




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
	getStudentsFromDoc(&headOfStudentsList, &tailOfStudentsList);
	getClassesFromDoc(&headOfClassesList);
	sortList(&headOfStudentList);
	sortList(&headOfClassesList);
	
	menu(&headOfStudentsList,&tailOfStudentsList, &headOfClassesList);
	printStudentList(&headOfStudentsList);
	printStudentListV2(&tailOfStudentsList);
	printf("\n\n\n");
	printClassesList(&headOfClassesList);
	
	return 0;
}

/*
void createList(STUDENT** head, STUDENT* (*function)(STUDENT **head)){
	
	if(*head == NULL){
		
	}
	
}
*/

int countLineNumberOfDoc(char *name){
	
	FILE *fp;
	char c;
	int lineNumber;
	
	lineNumber = 0;
	
	fp = fopen(name,"r");
	
	for (c = getc(fp); c != EOF; c = getc(fp)){
    	if (c == '\n') 
        	lineNumber++;
	}
	
	fclose(fp);
	
	return lineNumber;
}


void getStudentsFromDoc(STUDENT** head, STUDENT** tail){
	
	int lineNumberOfDoc;
	STUDENT *ptr;
	FILE *fp;
	char nameOfDoc[] = "ogrenciler.txt"; 

	
	lineNumberOfDoc = countLineNumberOfDoc(nameOfDoc);
	
	fp = fopen(nameOfDoc, "r");
	
	if(fp == NULL){
		printf("HATA '%s' isimli dosya acilamadi", nameOfDoc);
	}
	
	
	while(lineNumberOfDoc > 0){
		
		if(*head == NULL){
			*head = getStudentInformationFromFile(fp);
			(*head)->prev = NULL;
		}
		
		else{
			
			ptr = *head; 
			
			while(ptr->next != NULL){
				ptr = ptr->next;
			}
			
			ptr->next = getStudentInformationFromFile(fp);
			ptr->next->prev = ptr;
		
		}
		
		*tail = ptr->next;
	
		lineNumberOfDoc--;
	}
	
	fclose(fp);

}

STUDENT* getStudentInformationFromFile(FILE *fp){
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
	
	sp->next = NULL;
	sp->prev = NULL;
	
	
	return sp;
	
}

void getClassesFromDoc(CLASS** head){
	int lineNumberOfDoc;
	FILE* fp;
	CLASS* ptr;
	char nameOfDoc[] = "dersler.txt"; 
	
	lineNumberOfDoc =  countLineNumberOfDoc(nameOfDoc);
	
	fp = fopen(nameOfDoc, "r");
	
	if(fp == NULL){
		printf("HATA '%s' isimli dosya acilamadi", nameOfDoc);
	}
	
	
	while(lineNumberOfDoc > 0){
		
		if(*head == NULL){
			*head = getClassInformationFromFile(fp);
		}
		
		else{
			
			ptr = *head; 
			
			while(ptr->next != NULL){
				ptr = ptr->next;
			}
			
			ptr->next = getClassInformationFromFile(fp);
		
		}
	
		lineNumberOfDoc--;
	}
	
	fclose(fp);
	
}

CLASS* getClassInformationFromFile(FILE *fp){
	
	char i[10];
	char n[CHAR_SIZE];
	int cap, cre;
	
	CLASS* cp;
	
	cp = (CLASS*)malloc(sizeof(CLASS));
	
	fscanf(fp,"%s , %s ,%d,%d\n",i,n,&cre,&cap);
	
	strcpy(cp->ID,i);
	strcpy(cp->name,n);
	cp->credit = cre;
	cp->capacity = cap;
	cp->next = NULL;
	
	return cp;
	
}






void menu(STUDENT** headOfStudentsList,STUDENT** tailOfStudentList, CLASS** headOfClassesList){
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
			studentOperations(headOfStudentsList,tailOfStudentList);
		
		else if(menuInput == 2)
			classOperations(headOfClassesList);
			
		else if(menuInput == 3)
			selectClassOperations();
			
		else if(menuInput != 0){
			printf("Hatali secim yaptiniz tekrar deneyiniz.\n");
		}
	}
		
}

void studentOperations(STUDENT** head, STUDENT** tail){
	int studentOperationsMenuInput = 1;
	
	while(studentOperationsMenuInput != 0){
		printf("Ogrenci eklemek icin 1\n");
		printf("Ogrenci silmek icin 2\n");
		printf("Ogrencinin aldigi dersleri yazdirmak icin 3\n");
		printf("Cikmak icin 0 giriniz\nSeciminiz: ");
		scanf("%d", &studentOperationsMenuInput);
		printf("\n");
		if(studentOperationsMenuInput == 1)
			addStudent(head,tail);
		
		else if(studentOperationsMenuInput == 2)
			//deleteStudent(head,tail);
		else if(studentOperationsMenuInput == 3)
			//printClassesOfStudent();	*/
	}
	
}


void classOperations(CLASS** head){
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
	
	while(ptr != NULL){
		printf("%d\t%s\t%s\t%d\t\t%d\n", ptr->ID, ptr->name, ptr->surname, ptr->numOfClasses, ptr->totalCredit);
		ptr = ptr->next;
	}
}

void printStudentListV2(STUDENT **tail){
	STUDENT *ptr;
	int i = 0;

	if((*tail) == NULL){
		printf("Tail NULL");
		return;
	}
	
	printf("ID\t\tName\tSurname\tNum Of Classes\tTotal Credit\n");
	
	ptr = (*tail);
	
	while(ptr != NULL){
		printf("%d\t%s\t%s\t%d\t\t%d\n", ptr->ID, ptr->name, ptr->surname, ptr->numOfClasses, ptr->totalCredit);
		ptr = ptr->prev;
	}
}


void printClassesList(CLASS **head){
	CLASS *ptr;
	int i = 0;
	
	if(*head == NULL){
		printf("Head NULL");
		return;
	}

	
	ptr = *head;
	
	while(ptr != NULL){
		printf("%s\t%s\t%d\t%d\t\n", ptr->ID, ptr->name, ptr->credit, ptr->capacity);
		ptr = ptr->next;
	}
}

void addStudent(STUDENT** head,STUDENT **tail){
		
	STUDENT* ptr;
	
	if(*head == NULL){
		*head = createStudent();
	}
		
	else{
		ptr = *tail;
		ptr->next = createStudent();
		ptr->next->prev = ptr;
		*tail = ptr->next;
	}	
	
}

STUDENT* createStudent(){
	
	int i;
	char n[CHAR_SIZE];
	char sn[CHAR_SIZE];
	STUDENT* sp;
	
	sp = (STUDENT*)malloc(sizeof(STUDENT));
	
	printf("Eklemek istediginiz ogrencinin ogrenci numarasini giriniz: ");
	scanf("%d", &i);
	
	printf("Eklemek istediginiz ogrencinin adini giriniz: ");
	scanf("%s", n);
	
	printf("Eklemek istediginiz ogrencinin soyadini giriniz: ");
	scanf("%s", sn);
	
	sp->ID = i;
	strcpy(sp->name, n);
	strcpy(sp->surname,sn);
	
	sp->next = NULL;
	sp->prev = NULL;
	sp->numOfClasses = 0;
	sp->totalCredit = 0;
	
	return sp;

}
