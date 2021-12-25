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


STUDENT* getStudentInformationFromFile(FILE *fp);
void addStudent(STUDENT** head, STUDENT** tail);
CLASS* getClassInformationFromFile(FILE *fp);

void sortList();

void getClassesInformations(CLASS** headOfClassesList);

STUDENT* createStudent();
STUDENT* findStudent(int id, STUDENT** head);
void deleteStudent(STUDENT** head, STUDENT** tail);
void printClassesOfStudent(CLASS** head);

void classOperations(CLASS **headOfClassList, STUDENT** headOfStudentList, STUDENT** tailOfStudentList);
void addNewClassToList(CLASS **head);
CLASS* createClass();
void deleteClassFromList(CLASS **head);
void addStudentToClass();
void getStudentListOfClass(CLASS **headOfClassList, STUDENT **headOfStudentList,STUDENT** tailOfStudentList);

void selectClassOperations();


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

	getClassesInformations(&headOfClassesList);
	
	menu(&headOfStudentsList,&tailOfStudentsList, &headOfClassesList);
	printStudentList(&headOfStudentsList);
	printStudentListV2(&tailOfStudentsList);
	printf("\n\n\n");
	printClassesList(&headOfClassesList);
	
	return 0;
}



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
	cp->idsOfStudents = NULL;
	cp->numOfStudents = 0;
	cp->next = NULL;
	
	
	return cp;
	
}



void sortStudentList(STUDENT** head){
	
	STUDENT *ptr1, *ptr2, *tmpForNext, *tmpForPrev;

	for(ptr1 = *head; ptr1->next != NULL; ptr1 = ptr1->next){
		for(ptr2 = *head; ptr2->next->next != NULL; ptr2 = ptr2->next){
			if(ptr2->ID > ptr2->next->ID){
				
				tmpForNext = ptr2->prev->next;
				ptr2->prev->next = ptr2->next;
				ptr2->next = ptr2->next->next;
				
				tmpForPrev = ptr2->prev;
				ptr2->prev = ptr2;
				
		
			}	
		}
	}	
	
}


void getClassesInformations(CLASS** headOfClassesList){
	
	int id, numOfLines,i,lineNumber;
	char IDOfClass[10],date[CHAR_SIZE],state[CHAR_SIZE];
	int IDOfStudent;
	CLASS* ptr;
	FILE *fp;
	char nameOfFile[] = "OgrenciDersKayit.txt";
	
	numOfLines = countLineNumberOfDoc(nameOfFile);
	lineNumber = 0;
	ptr = *headOfClassesList;
	i = 0;
	fp = fopen(nameOfFile, "r");
	(ptr->idsOfStudents) = (int*)malloc( (ptr->numOfStudents)*sizeof(int) );

	while(ptr != NULL){
		lineNumber = 0;
		while(lineNumber < numOfLines){
			fscanf(fp,"%d,%s ,%d,%s ,%s\n", &id,IDOfClass,&IDOfStudent,date,state);
			if(strcmp(state,"kayitli") == 0 && strcmp(IDOfClass,ptr->ID) == 0){
				(ptr->numOfStudents)++;
				(ptr->idsOfStudents) = (int*)realloc((ptr->idsOfStudents),(ptr->numOfStudents)*sizeof(int));
				*((ptr->idsOfStudents)+i) = IDOfStudent;
				i++;
			}
			lineNumber++;
		}
		i = 0;
		ptr = ptr->next;
		rewind(fp);
	}
	
	fclose(fp);
	
	ptr = *headOfClassesList;
	
	while(ptr!=NULL){
		printf("%s\n", ptr->ID);
		for(i=0; i<(ptr->numOfStudents); i++){
			printf("%d\n",*((ptr->idsOfStudents)+i));
		}
		printf("\n\n");
		ptr = ptr->next;
	}	
	
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
			studentOperations(headOfStudentsList,tailOfStudentList,headOfClassesList);
		
		else if(menuInput == 2)
			classOperations(headOfClassesList,headOfStudentsList,tailOfStudentList);
			
		else if(menuInput == 3)
			selectClassOperations();
			
		else if(menuInput != 0){
			printf("Hatali secim yaptiniz tekrar deneyiniz.\n");
		}
	}
		
}

void studentOperations(STUDENT** head, STUDENT** tail, CLASS** headOfClassList){
	int studentOperationsMenuInput = 1;
	
	while(studentOperationsMenuInput != 0){
		printf("Ogrenci eklemek icin 1\n");
		printf("Ogrenci silmek icin 2\n");
		printf("Ogrencinin aldigi dersleri yazdirmak icin 3// Calismiyor\n");
		printf("Cikmak icin 0 giriniz\nSeciminiz: ");
		scanf("%d", &studentOperationsMenuInput);
		printf("\n");
		if(studentOperationsMenuInput == 1)
			addStudent(head,tail);
		
		else if(studentOperationsMenuInput == 2)
			deleteStudent(head,tail);
		else if(studentOperationsMenuInput == 3)
			printClassesOfStudent(headOfClassList);			// Calismiyor
	}
	
}


void classOperations(CLASS** headOfClassList, STUDENT** headOfStudentList,STUDENT** tailOfStudentList){
	int classOperationsMenuInput = 1;
	
	while(classOperationsMenuInput != 0){
		printf("Ders acmak icin 1,\n");
		printf("Ders kapatmak icin 2,\n");
		printf("Dersi alan ogrencileri yazdirmak icin 3\n");
		printf("Cikmak icin 0 giriniz\nSeciminiz: ");
		scanf("%d", &classOperationsMenuInput);
		printf("\n");
		
		if(classOperationsMenuInput == 1){
			addNewClassToList(headOfClassList);
		}
		else if(classOperationsMenuInput == 2)
			deleteClassFromList(headOfClassList);
		else if(classOperationsMenuInput == 3){
			getStudentListOfClass(headOfClassList,headOfStudentList,tailOfStudentList);
		}
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

STUDENT* findStudent(int id, STUDENT **head){
	STUDENT* ptr;
	ptr = *head;
	
	while(ptr->ID != id && ptr != NULL)
		ptr = ptr->next;
	
	return ptr;
}


void deleteStudent(STUDENT** head, STUDENT** tail){
// liste sirali oldugu icin degistirilebilir.	
	int id;
	STUDENT *ptr;
	
	printf("Silmek istediginiz ogrencinin ogrenci numarasini giriniz: ");
	scanf("%d", &id);
	
	if((*head)->ID == id){
		(*head)->next->prev = (*head)->prev;
		(*head) = (*head)->next;
		printf("%d numarali ogrenciyi silme islemi basariliiii.\n", id);
	}
	
	else if((*tail)->ID == id){
		(*tail)->prev->next = (*tail)->next;
		(*tail) = (*tail)->prev;
		printf("%d numarali ogrenciyi silme islemi basariliiii.\n", id);
	}
	
	else{
		ptr = findStudent(id,head);
		if(ptr->ID == id){
			ptr->prev->next = ptr->next;
			ptr->next->prev = ptr->prev;
			printf("%d numarali ogrenciyi silme islemi basariliiii.\n", id);
			return;
		}
		
		else{
			printf("Silinmek istenen ogrenci listede bulunmadi.\n");
			return;
		}
	}
	
}

void printClassesOfStudent(CLASS** head){
	
	int i,id,j;
	CLASS* ptr;
	char *classes[20];
	
	printf("Derslerini yazdirmak istediginiz ogrencinin ogrenci numarasini giriniz: ");
	scanf("%d", &id);
	
	ptr = *head;
	
	j = 0;
	
	while(ptr!=NULL){
		
		i = 0;
		
		printf("WHILE ONCE\n");
		while(i <= (ptr->numOfStudents)-1 && *((ptr->idsOfStudents)+i) != id){
			printf("i: %d ptr: %d id of student: %d\n", i,(ptr->numOfStudents) , *((ptr->idsOfStudents)+i));	
			i++;
		}
		
		printf("WHILE SONRA\n");
		
		if(*((ptr->idsOfStudents)+i) == id)
			printf("%s\n", ptr->ID);
			
		ptr = ptr->next;
	}
	
}


void addNewClassToList(CLASS** head){
	
	CLASS *ptr, *cp, *prevPtr;

	ptr = *head;	
	cp = createClass();
	
	
	if(strcmp(ptr->ID, cp->ID) > 0){
		cp->next = ptr;
		*head = cp;
		return;
	}
	

	while(strcmp(ptr->ID, cp->ID) < 0){
		prevPtr = ptr;
		ptr = ptr->next;
	}
	
	
	if(strcmp(ptr->ID, cp->ID) == 0){
		printf("%s kodlu ders zaten acilmis, tekrar acilamaz.\n", cp->ID);
		return;
	}	
	
	cp->next = ptr;
	prevPtr->next = cp;

}

CLASS* createClass(){
	char id[10];
	char name[CHAR_SIZE];
	int credit,capacity;
	CLASS* ptr;

	ptr = (CLASS*)malloc( sizeof(CLASS) );
	
	printf("Eklemek istediginiz dersin kodunu giriniz: ");
	scanf("%s" , id);
	
	printf("Eklemek istediginiz dersin adini giriniz: ");
	scanf("%s", name);
	
	printf("Eklemek istediginiz dersin kredisini giriniz: ");
	scanf("%d", &credit);
	
	printf("Eklemek istediginiz dersin kontenjanini giriniz: ");
	scanf("%d", &capacity);
	
	strcpy(ptr->ID, id);
	strcpy(ptr->name,name);
	ptr->credit = credit;
	ptr->capacity = capacity;
	ptr->idsOfStudents = NULL;
	ptr->numOfStudents = 0;
	ptr->next = NULL;
	
	return ptr;
}

void deleteClassFromList(CLASS** head){
	
	CLASS* ptr;
	CLASS* prevPtr;;
	char id[10];
	
	printf("Kapatmak istediginiz dersin kodunu giriniz: ");
	scanf("%s", id);
	
	ptr = *head;
	
	if(strcmp(ptr->ID, id) == 0){
		*head = ptr->next;
		printf("%s kodlu ders basariyla kapatildi\n", id);
		// DersKayit.txt adlı dosyaya bu durumun eklenmis olmasi lazim
		return;
	}
	
	while(ptr != NULL && strcmp(ptr->ID,id) != 0){
		prevPtr = ptr;
		ptr = ptr->next;
	}
	

	
	if(ptr == NULL){
		printf("Kapatilmak istenen ders ders listesinde bulunamadi.\n");
		return;
	}
	
	else{
		prevPtr->next = ptr->next;
		printf("%s kodlu ders basariyla kapatildi\n", id);
		// DErsKayıt adlı dosyaya bu durumun eklenmis olmasi lazim
	}
	
	
}

void getStudentListOfClass(CLASS** headOfClassList, STUDENT **headOfStudentList,STUDENT** tailOfStudentList){
	int i;
	STUDENT* stdPtr;
	CLASS* clsPtr;
	int *idsOfStudents;
	char id[10];
	
	printf("Listesini yazdirmak istediginiz dersin kodunu giriniz: ");
	scanf("%s", id);
	
	clsPtr = *headOfClassList;
	stdPtr = *headOfStudentList;
	
	while(clsPtr != NULL && strcmp(id,clsPtr->ID) != 0){
		clsPtr = clsPtr->next;
	}
	
	if(strcmp(id,clsPtr->ID) == 0){
		idsOfStudents = (int*)malloc(clsPtr->numOfStudents*sizeof(int));
		
		for(i=0; i<clsPtr->numOfStudents; i++){
			idsOfStudents[i] = *((clsPtr->idsOfStudents)+i);
			printf("%d\t", *((clsPtr->idsOfStudents)+i));
		}
	
		if(idsOfStudents[i]-(*headOfStudentList)->ID > (*tailOfStudentList)->ID-idsOfStudents[i]){
			stdPtr = *headOfStudentList;
			// burada yapilacak islem ogrenciye idsi sayesinde ulaşıp ogrencinin bilgilerini dosyaya kaydetmek
		}
		else{
			stdPtr = *tailOfStudentList;
		}	
	}
}