#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define CHAR_SIZE 50

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

typedef struct classRegistiration{
	int ID;
	char idOfClass[CHAR_SIZE];
	int idOfStudent;
	char date[CHAR_SIZE];
	char state[CHAR_SIZE];
	struct classRegistiration* next;
}CLASSREGISTIRATION;

int countLineNumberOfDoc(char *name);
int isFileEmpty(char *nameOfDoc);
void getStudentsFromDoc(STUDENT** head, STUDENT** tail);
STUDENT* getStudentInformationFromFile(FILE *fp,char delimiter[2]);
void getClassesFromDoc(CLASS** head);
CLASS* getClassInformationFromFile(FILE *fp,char delimiter[2]);
void getClassRegistirationsFromFile(CLASSREGISTIRATION** head);
CLASSREGISTIRATION* getClassRegInfo(FILE* fp);
void sortList(int* list, int n);
void getClassesInformations(CLASS** headOfClassesList);
void menu(STUDENT** headOfStudentsList,STUDENT** tailOfStudentList, CLASS** headOfClassesList, CLASSREGISTIRATION** headOfClassRegList,int totalNumOfCredit, int totalNumOfClass);
void studentOperations(STUDENT** head, STUDENT** tail, CLASS** headOfClassList,CLASSREGISTIRATION** headOfClassRegList);
void classOperations(CLASS** headOfClassList, STUDENT** headOfStudentList,STUDENT** tailOfStudentList,CLASSREGISTIRATION** headOfClassRegList);
void selectClassOperations(CLASS** headOfClassList, STUDENT** headOfStudentList, STUDENT** tailOfStudentList,CLASSREGISTIRATION** headOfClassRegList, int creditLimit, int numOfClassLimit);
void printStudentList(STUDENT **head);
void printStudentListV2(STUDENT **tail);
void addStudent(STUDENT** head,STUDENT **tail);
STUDENT* createStudent(int id);
STUDENT* findStudent(int id, STUDENT **head);
void deleteStudent(STUDENT** head, STUDENT** tail,CLASSREGISTIRATION** headOfClassRegList);
void printClassesOfStudent(CLASS** head);
void addNewClassToList(CLASS** head);
CLASS* createClass();
void updateStates(CLASSREGISTIRATION** headOfClassRegList, char* newState, char* idOfClass);
void increaseCredit(int* totalCredit, int incrementQuantity);
void decreaseCredit(int* totalCredit, int decrementQuantity);
void increaseNumOfClass(int* totalNumOfClass);
void decreaseNumOfClass(int* totalNumOfClass);
void updateStudentCredit(STUDENT** head, int id, int creditQuantity, void(*updateCreditFunction)(int* , int ),void(*updateNumOfClassFunction)(int*) );
void deleteClassFromList(CLASS** head,CLASSREGISTIRATION** headOfClassRegList,STUDENT** headOfStudentList);
void getStudentListOfClass(CLASS** headOfClassList, STUDENT **headOfStudentList,STUDENT** tailOfStudentList);
char* createDateText();
CLASSREGISTIRATION* createClassRec(int id, char* classId, int studentId);
void selectClass(CLASS** headOfClassList, STUDENT** headOfStudentList, STUDENT** tailOfStudentList, CLASSREGISTIRATION **headOfClassRegList,int studentId, int creditLimit, int numOfClassLimit);
void removeStudentFromClass(CLASS** headOfClassList, STUDENT** headOfStudentList, STUDENT** tailOfStudentList,CLASSREGISTIRATION** headOfClassRegList,int studentId);
void updateStudentsFile(STUDENT** head);
void updateClassesFile(CLASS** head);
void updateClassRegistirationFile(CLASSREGISTIRATION** head);

int main(void){
	int totalNumOfCredit, totalNumOfClass;
	STUDENT *headOfStudentsList; 
	STUDENT *tailOfStudentsList;
	CLASS *headOfClassesList;
	CLASSREGISTIRATION* headOfClassRegList;
	
	setlocale(LC_ALL, "Turkish"); 
	
	headOfClassesList = NULL;
	headOfStudentsList = NULL;
	headOfClassRegList = NULL;
	
	printf("Ogrencinin secebilecegi kredi sayisini giriniz: ");
	scanf("%d", &totalNumOfCredit);
	
	printf("Ogrencinin secebilecegi ders sayisini giriniz: ");
	scanf("%d", &totalNumOfClass);
	
	getStudentsFromDoc(&headOfStudentsList, &tailOfStudentsList);
	getClassesFromDoc(&headOfClassesList);
	getClassRegistirationsFromFile(&headOfClassRegList);
	getClassesInformations(&headOfClassesList);
	
	menu(&headOfStudentsList,&tailOfStudentsList, &headOfClassesList,&headOfClassRegList, totalNumOfCredit,totalNumOfClass);
	
	updateStudentsFile(&headOfStudentsList);
	updateClassesFile(&headOfClassesList);
	updateClassRegistirationFile(&headOfClassRegList);
	
	free(headOfStudentsList);
	free(tailOfStudentsList);
	free(headOfClassesList);
	free(headOfClassRegList);

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

int isFileEmpty(char *nameOfDoc){
	int size;
	FILE *fp;
	
	fp = fopen(nameOfDoc, "r");
	size = ftell(fp);
	fclose(fp);
	
	if(size == 0)
		return 1;
		
	return 0;
}

void getStudentsFromDoc(STUDENT** head, STUDENT** tail){	
	int lineNumberOfDoc;
	STUDENT *ptr;
	FILE *fp;
	char nameOfDoc[] = "ogrenciler.txt"; 

    if (isFileEmpty(nameOfDoc) == 0) {
        return;
    }
	
	lineNumberOfDoc = countLineNumberOfDoc(nameOfDoc);
	fp = fopen(nameOfDoc, "r");
	
	if(fp == NULL){
		printf("HATA '%s' isimli dosya acilamadi", nameOfDoc);
	}
	
	if(feof(fp) != 0){
		printf("ogrenciler dosyasi bos oldugu icin okuma yapilmadi.\n");
		return;
	}
	
	while(lineNumberOfDoc > 0){	
		if(*head == NULL){
			*head = getStudentInformationFromFile(fp,",\0");
			(*head)->prev = NULL;
		}
		else{
			ptr = *head; 
			while(ptr->next != NULL){
				ptr = ptr->next;
			}
			ptr->next = getStudentInformationFromFile(fp,",\0");
			ptr->next->prev = ptr;
		}
		*tail = ptr->next;
		lineNumberOfDoc--;
	}
	fclose(fp);
}

STUDENT* getStudentInformationFromFile(FILE *fp,char delimiter[2]){
	int i,totalCredit,numOfClasses;
	STUDENT* sp;
	char n[CHAR_SIZE],sn[CHAR_SIZE];
	char *token;
	char buf[255];
	
	sp  = (STUDENT*)malloc( sizeof(STUDENT) );
	
	fgets(buf,255,fp);
	
	token=strtok(buf,delimiter);	
	sp->ID = atoi(token);
	
	token=strtok(NULL,delimiter);
	strcpy(sp->name,token);
	
	token=strtok(NULL,delimiter);
	strcpy(sp->surname,token);
	
	token=strtok(NULL,delimiter);
	sp->totalCredit = atoi(token);
	
	token=strtok(NULL,delimiter);
	sp->numOfClasses = atoi(token);
	
	sp->next = NULL;
	
	return sp;	
}

void getClassesFromDoc(CLASS** head){
	int lineNumberOfDoc;
	FILE* fp;
	CLASS* ptr;
	char nameOfDoc[] = "dersler.txt"; 	
	
    if (isFileEmpty(nameOfDoc) == 0) {
        return;
    }
	
	lineNumberOfDoc =  countLineNumberOfDoc(nameOfDoc);	
	fp = fopen(nameOfDoc, "r");
	
	if(fp == NULL){
		printf("HATA '%s' isimli dosya acilamadi", nameOfDoc);
	}
	
	while(lineNumberOfDoc > 0){
		if(*head == NULL){
			*head = getClassInformationFromFile(fp,",\0");
		}
		else{
			ptr = *head; 
			while(ptr->next != NULL){
				ptr = ptr->next;
			}
			ptr->next = getClassInformationFromFile(fp,",\0");
		}
		lineNumberOfDoc--;
	}
	fclose(fp);	
}

CLASS* getClassInformationFromFile(FILE *fp,char delimiter[2]){
	char i[10],buf[255],n[CHAR_SIZE];
	int cap, cre;
	CLASS* cp;
	char *tmp;
	
	cp = (CLASS*)malloc(sizeof(CLASS));
	
	fgets(buf,255,fp);
	
	tmp=strtok(buf,delimiter);
	strcpy(cp->ID,tmp);

	tmp=strtok(NULL,delimiter);
	strcpy(cp->name,tmp);
	
	tmp=strtok(NULL,delimiter);
	cp->credit = atoi(tmp);
	
	tmp=strtok(NULL,delimiter);
	cp->capacity = atoi(tmp);
	
	cp->idsOfStudents = NULL;
	cp->numOfStudents = 0;
	cp->next = NULL;
		
	return cp;
}

void getClassRegistirationsFromFile(CLASSREGISTIRATION** head){
	int lineNumberOfDoc;
	char nameOfDoc[] = "OgrenciDersKayit.txt"; 
	FILE* fp;
	CLASSREGISTIRATION* ptr;
	
    if (isFileEmpty(nameOfDoc) == 0) {
        return;
    }
    
	lineNumberOfDoc =  countLineNumberOfDoc(nameOfDoc);
	fp = fopen(nameOfDoc, "r");
	
	if(fp == NULL){
		printf("HATA '%s' isimli dosya acilamadi", nameOfDoc);
	}
	
	while(lineNumberOfDoc > 0){
		
		if(*head == NULL){
			*head = getClassRegInfo(fp);
		}
		else{
			ptr = *head;
			while(ptr->next != NULL){
				ptr = ptr->next;
			}
			ptr->next = getClassRegInfo(fp);
		}
		lineNumberOfDoc--;
	}
	fclose(fp);
}

CLASSREGISTIRATION* getClassRegInfo(FILE* fp){
	
	CLASSREGISTIRATION* p;
	char buf[256],delim[] = ",\0";
	char *tmp;
	
	p = (CLASSREGISTIRATION*)malloc(sizeof(CLASSREGISTIRATION));
	
	fgets(buf, 255,fp);
	
	tmp = strtok(buf,delim);
	p->ID = atoi(tmp);
	
	tmp = strtok(NULL,delim);
	strcpy(p->idOfClass, tmp);
	
	tmp = strtok(NULL,delim);
	p->idOfStudent = atoi(tmp);
	
	tmp = strtok(NULL,delim);
	strcpy(p->date,tmp);
	tmp = strtok(NULL,delim);
	
	tmp[strlen(tmp)-1] = '\0';
	strcpy(p->state,tmp);
	
	p->next = NULL;
	
	return p;
}

void sortList(int* list, int n){
	int i,j,tmp;
	
	for(i=0; i<n; i++){
		for(j=0; j<n-1; j++){
			if(list[j] > list[j+1]){
				tmp = list[j];
				list[j] = list[j+1];
				list[j+1] = tmp;
			}
		}
	}
}

void getClassesInformations(CLASS** headOfClassesList){
	int id, numOfLines,i,lineNumber,IDOfStudent;
	char IDOfClass[10],date[CHAR_SIZE],state[CHAR_SIZE],nameOfFile[] = "OgrenciDersKayit.txt";;
	CLASS* ptr;
	CLASSREGISTIRATION* ptrCR;
	FILE *fp;
	
	numOfLines = countLineNumberOfDoc(nameOfFile);
	lineNumber = 0;
	ptr = *headOfClassesList;
	i = 0;
	fp = fopen(nameOfFile, "r");
	(ptr->idsOfStudents) = (int*)malloc( (ptr->numOfStudents)*sizeof(int) );

	while(ptr != NULL){
		lineNumber = 0;
		
		while(lineNumber < numOfLines){
			ptrCR = getClassRegInfo(fp);
			if(strcmp(ptrCR->state,"kayitli") == 0 && strcmp(ptrCR->idOfClass,ptr->ID) == 0){
				(ptr->numOfStudents)++;
				(ptr->idsOfStudents) = (int*)realloc((ptr->idsOfStudents),(ptr->numOfStudents)*sizeof(int));
				*((ptr->idsOfStudents)+i) = ptrCR->idOfStudent;
				i++;
			}
			lineNumber++;
		}
		sortList(ptr->idsOfStudents,ptr->numOfStudents);
		i = 0;
		ptr = ptr->next;
		rewind(fp);
	}
	fclose(fp);
	ptr = *headOfClassesList;	
}

void menu(STUDENT** headOfStudentsList,STUDENT** tailOfStudentList, CLASS** headOfClassesList, CLASSREGISTIRATION** headOfClassRegList,int totalNumOfCredit, int totalNumOfClass){
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
			studentOperations(headOfStudentsList,tailOfStudentList,headOfClassesList,headOfClassRegList);
		
		else if(menuInput == 2)
			classOperations(headOfClassesList,headOfStudentsList,tailOfStudentList,headOfClassRegList);
			
		else if(menuInput == 3)
			selectClassOperations(headOfClassesList,headOfStudentsList,tailOfStudentList,headOfClassRegList,totalNumOfCredit, totalNumOfClass);
			
		else if(menuInput != 0){
			printf("Hatali secim yaptiniz tekrar deneyiniz.\n");
		}
	}
		
}

void studentOperations(STUDENT** head, STUDENT** tail, CLASS** headOfClassList,CLASSREGISTIRATION** headOfClassRegList){
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
			deleteStudent(head,tail,headOfClassRegList);
			
		else if(studentOperationsMenuInput == 3)
			printClassesOfStudent(headOfClassList);			
	}
}

void classOperations(CLASS** headOfClassList, STUDENT** headOfStudentList,STUDENT** tailOfStudentList,CLASSREGISTIRATION** headOfClassRegList){
	int classOperationsMenuInput = 1;
	
	while(classOperationsMenuInput != 0){
		printf("Ders acmak icin 1,\n");
		printf("Ders kapatmak icin 2,\n");
		printf("Dersi alan ogrencileri yazdirmak icin 3\n");
		printf("Cikmak icin 0 giriniz\nSeciminiz: ");
		scanf("%d", &classOperationsMenuInput);
		printf("\n");
		
		if(classOperationsMenuInput == 1)
			addNewClassToList(headOfClassList);
		
		else if(classOperationsMenuInput == 2)
			deleteClassFromList(headOfClassList, headOfClassRegList,headOfStudentList);
			
		else if(classOperationsMenuInput == 3){
			getStudentListOfClass(headOfClassList,headOfStudentList,tailOfStudentList);
		}
	}	
}

void selectClassOperations(CLASS** headOfClassList, STUDENT** headOfStudentList, STUDENT** tailOfStudentList,CLASSREGISTIRATION** headOfClassRegList, int creditLimit, int numOfClassLimit){
	int selectClassOperationsMenuInput = 1;
	int studentId;
		
	printf("Islem yapmak istediginiz ogrencinin numarasini giriniz: ");
	scanf("%d", &studentId);
	
	while(selectClassOperationsMenuInput != 0){
		printf("Ders secmek icin 1,\n");
		printf("Secili bir dersi silmek icin 2,\n");
		printf("Islem yapilan ogrenciyi degistirmek icin 3,\n");
		printf("Cikmak icin 0 giriniz\nSeciminiz: ");
		scanf("%d", &selectClassOperationsMenuInput);
		printf("\n");
		
		if(selectClassOperationsMenuInput == 1)
			selectClass(headOfClassList,headOfStudentList,tailOfStudentList,headOfClassRegList,studentId,creditLimit,numOfClassLimit);
		
		else if(selectClassOperationsMenuInput == 2){
			removeStudentFromClass(headOfClassList, headOfStudentList, tailOfStudentList,headOfClassRegList,studentId);	
		}
		
		else if(selectClassOperationsMenuInput == 3){
			printf("Islem yapmak istediginiz ogrencinin numarasini giriniz: ");
			scanf("%d", &studentId);
		}
	}
}

void printStudentList(STUDENT **head){
	STUDENT *ptr;
	int i = 0;
	
	if(*head == NULL){
		printf("Ogrenciler listesi Head NULL oldugu icin yazdirilamiyor.\n");
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
		printf("Ogrenci listesi Tail NULL oldugu icin yazdirilamiyor.\n");
		return;
	}
	
	printf("ID\t\tName\tSurname\tNum Of Classes\tTotal Credit\n");
	ptr = (*tail);
	
	while(ptr != NULL){
		printf("%d\t%s\t%s\t%d\t\t%d\n", ptr->ID, ptr->name, ptr->surname, ptr->numOfClasses, ptr->totalCredit);
		ptr = ptr->prev;
	}
}

void addStudent(STUDENT** head,STUDENT **tail){	
	STUDENT* ptr;
	STUDENT* prevPtr;
	int id;
	
	printf("Eklemek istediginiz ogrencinin ogrenci numarasini giriniz: ");
	scanf("%d", &id);
	
	if(*head == NULL){
		*head = createStudent(id);
	}
	else{
		ptr = *head;
		
		if(id < (*head)->ID){
			ptr->prev= createStudent(id);
			ptr->prev->next = ptr;
			*head = ptr->prev;
			return;
		}
		
		while((ptr->ID) < id && ptr->next != NULL){
			prevPtr = ptr;
			ptr = ptr->next;
		}
		if(ptr->ID == id){
			printf("Ogrenci kaydi mevcut, tekrar eklenemez\n");
			return;
		}
		if(ptr->next != NULL){
			ptr->prev->next = createStudent(id);
			ptr->prev->next->next = ptr;
			ptr->prev->next->prev = ptr->prev;
			ptr->prev = ptr->prev->next;
			return;
		}
		else{
			ptr->next = createStudent(id);
			ptr->next->prev = ptr;
			*tail = ptr->next;
		}

	}	
}

STUDENT* createStudent(int id){
	char n[CHAR_SIZE];
	char sn[CHAR_SIZE];
	STUDENT* sp;
	
	sp = (STUDENT*)malloc(sizeof(STUDENT));
	
	printf("Eklemek istediginiz ogrencinin adini giriniz: ");
	scanf("%s", n);
	
	printf("Eklemek istediginiz ogrencinin soyadini giriniz: ");
	scanf("%s", sn);
	
	sp->ID = id;
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
	
	while(ptr != NULL && ptr->ID != id)
		ptr = ptr->next;
	
	return ptr;
}

void deleteStudent(STUDENT** head, STUDENT** tail,CLASSREGISTIRATION** headOfClassRegList){	
	int id;
	STUDENT *ptr;
	
	printf("Silmek istediginiz ogrencinin ogrenci numarasini giriniz: ");
	scanf("%d", &id);
	
	if((*head)->ID == id){
		(*head)->next->prev = (*head)->prev;
		(*head) = (*head)->next;
		printf("%d numarali ogrenciyi silme islemi basarili.\n", id);
	}
	else if((*tail)->ID == id){
		(*tail)->prev->next = (*tail)->next;
		(*tail) = (*tail)->prev;
		printf("%d numarali ogrenciyi silme islemi basarili.\n", id);
	}
	
	else{
		ptr = findStudent(id,head);
		
		if(ptr == NULL){
			printf("Silmek istenilen ogrenci bulunamadi.\n");
			return;
		}
		
		else if(ptr->ID == id){
			ptr->prev->next = ptr->next;
			ptr->next->prev = ptr->prev;
			printf("%d numarali ogrenciyi silme islemi basarili.\n", id);
			return;
		}
	}
}

void printClassesOfStudent(CLASS** head){
	int i,id,isClassFound;
	CLASS* ptr;
	char *classes[20];
	char nameOfFile[60],idText[15];
	FILE *fp;
	
	printf("Derslerini yazdirmak istediginiz ogrencinin ogrenci numarasini giriniz: ");
	scanf("%d", &id);
	
	ptr = *head;
	isClassFound = 0;
	i = 0; 
	
	itoa(id,idText,10);
	strcpy(nameOfFile,idText);
	strcat(nameOfFile, "_DERSPROGRAMI");
	strcat(nameOfFile,".txt");
	
	while(ptr!=NULL){
		i = 0;
		while(i < (ptr->numOfStudents) && *((ptr->idsOfStudents)+i) != id)	
			i++;
		
		if( !(i >= ptr->numOfStudents) ){
			if(!isClassFound){
				fp = fopen(nameOfFile, "w");
				printf("%d Numarali ogrencinin aldigi dersler: \n",id);
			}
			printf("%s\t%s\n", ptr->ID,ptr->name);
			fprintf(fp,"%s\t%s\n",ptr->ID, ptr->name);	

			isClassFound = 1;	
		}
		ptr = ptr->next;
	}
	
	if(!(isClassFound))
		printf("Ogrencinin kayit oldugu bir ders bulunamadi.\n");
	else{
		fclose(fp);
		printf("Ogrencinin aldigi dersler %s isimli dosyaya kaydedilmiştir.\n",nameOfFile);
	}
	
	printf("\n");	
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

void updateStates(CLASSREGISTIRATION** headOfClassRegList, char* newState, char* idOfClass){
	CLASSREGISTIRATION* ptr;
	ptr = *headOfClassRegList;
	
	while(ptr != NULL){
		if(strcmp(ptr->idOfClass,idOfClass) == 0){
			strcpy(ptr->state, newState);
		}
		ptr = ptr->next;
	}
}

void increaseCredit(int* totalCredit, int incrementQuantity){
	*(totalCredit) += incrementQuantity; 	
}

void decreaseCredit(int* totalCredit, int decrementQuantity){
	*(totalCredit) -= decrementQuantity;
}

void increaseNumOfClass(int* totalNumOfClass){
	*(totalNumOfClass) += 1;
}

void decreaseNumOfClass(int* totalNumOfClass){
	*(totalNumOfClass) -= 1;
}

void updateStudentCredit(STUDENT** head, int id, int creditQuantity, void(*updateCreditFunction)(int* , int ),void(*updateNumOfClassFunction)(int*) ){
	STUDENT *ptr;
	
	ptr = *head;
	
	while(ptr!=NULL && ptr->ID != id){
		ptr = ptr->next;
	}
	
	if(ptr!=NULL){
		updateCreditFunction(&(ptr->totalCredit),creditQuantity);
		updateNumOfClassFunction(&(ptr->numOfClasses));
	}
}

void deleteClassFromList(CLASS** head,CLASSREGISTIRATION** headOfClassRegList,STUDENT** headOfStudentList){
	CLASS* ptr;
	CLASS* prevPtr;
	FILE *fp;

	char id[10];
	int i;
	
	printf("Kapatmak istediginiz dersin kodunu giriniz: ");
	scanf("%s", id);
	
	ptr = *head;
	
	if(strcmp(ptr->ID, id) == 0){
		*head = ptr->next;
		printf("%s kodlu ders basariyla kapatildi\n", id);
		updateStates(headOfClassRegList, "ders_kapandi",id);
		for(i=0; i<(ptr->numOfStudents); i++){
			updateStudentCredit(headOfStudentList,*((ptr->idsOfStudents)+i),ptr->credit,decreaseCredit,decreaseNumOfClass);
		}
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
		updateStates(headOfClassRegList, "ders_kapandi",id);
		for(i=0; i<(ptr->numOfStudents); i++){
			updateStudentCredit(headOfStudentList,*((ptr->idsOfStudents)+i),ptr->credit,decreaseCredit,decreaseNumOfClass);
		}
	}
}

void getStudentListOfClass(CLASS** headOfClassList, STUDENT **headOfStudentList,STUDENT** tailOfStudentList){
	int i;
	STUDENT* stdPtr;
	CLASS* clsPtr;
	int *idsOfStudents;
	char id[10];
	char nameOfDoc[30];
	FILE* fp;
	
	printf("Listesini yazdirmak istediginiz dersin kodunu giriniz: ");
	scanf("%s", id);
	
	strcpy(nameOfDoc,id);
	strcat(nameOfDoc,".txt");
	
	clsPtr = *headOfClassList;
	
	while(clsPtr != NULL && strcmp(id,clsPtr->ID) != 0){
		clsPtr = clsPtr->next;
	}
	
	if(clsPtr == NULL){
		printf("Listesi yazdirilmak istenilen ders bulunamadi\n");
		return;
	}
	
	if(strcmp(id,clsPtr->ID) == 0){
		idsOfStudents = (int*)malloc(clsPtr->numOfStudents*sizeof(int));
		
		if(clsPtr->numOfStudents == 0){
			printf("Dersi alan ogrenci bulunmamaktadir.\n");
			return;
		}
		
		for(i=0; i<clsPtr->numOfStudents; i++){
			idsOfStudents[i] = *((clsPtr->idsOfStudents)+i);
		}

		fp = fopen(nameOfDoc,"w");
		
		if(idsOfStudents[0]-(*headOfStudentList)->ID < (*tailOfStudentList)->ID-idsOfStudents[clsPtr->numOfStudents]){
			printf("%s Kodlu Dersi Alan Ogrenciler:\n",id);
			stdPtr = *headOfStudentList;	
			for(i=0; i<clsPtr->numOfStudents; i++){
				stdPtr = findStudent(idsOfStudents[i], headOfStudentList);
				if(stdPtr != NULL){
					fprintf(fp,"%d,%d,%s,%s\n",i+1,stdPtr->ID,stdPtr->name, stdPtr->surname);
					printf("%d %s %s\n",stdPtr->ID,stdPtr->name, stdPtr->surname);	
				}
			}
		}
		else{
			stdPtr = *tailOfStudentList;	
			for(i=0; i<clsPtr->numOfStudents; i++){
				stdPtr = findStudent(idsOfStudents[i], tailOfStudentList);
				if(stdPtr != NULL){
					fprintf(fp,"%d,%d,%s,%s\n",i+1,stdPtr->ID,stdPtr->name, stdPtr->surname);
					printf("%d %s %s\n",stdPtr->ID,stdPtr->name, stdPtr->surname);	
				}
			}
		}
		printf("%s kodlu dersin listesi %s dosyasina kaydedildi.\n",id,nameOfDoc);
	}
}

char* createDateText(){
	int length = 8;	
	char *date;
						
	date = (char*)malloc(length*sizeof(char));	
			
	printf("Derse kayit olma tarihini giriniz(gg.aa.yyyy): ");
	scanf("%s",date);

	return date;
}

CLASSREGISTIRATION* createClassRec(int id, char* classId, int studentId){
	CLASSREGISTIRATION* p;
	
	p = (CLASSREGISTIRATION*)malloc(sizeof(CLASSREGISTIRATION));
	
	p->ID = id;
	strcpy(p->idOfClass,classId);
	p->idOfStudent = studentId;
	
	strcpy(p->date,createDateText()); 
	strcpy(p->state, "kayitli");
	p->next = NULL;
	
	return p;
}

void selectClass(CLASS** headOfClassList, STUDENT** headOfStudentList, STUDENT** tailOfStudentList, CLASSREGISTIRATION **headOfClassRegList,int studentId, int creditLimit, int numOfClassLimit){
	int idOfStudent,i,prevRecordId = 10001;
	int isRecordAlreadyExists = 0;
	char idOfClass[10];
	CLASS *clsPtr;
	STUDENT* stdPtr;
	CLASSREGISTIRATION* clsRegPtr;
	
	clsPtr = *headOfClassList;
	printf("Secilebilecek Dersler: \n");
	
	while(clsPtr != NULL){
		printf("%s\t%s\n", clsPtr->ID, clsPtr->name);
		clsPtr = clsPtr->next;
	}
	
	printf("\n");	
	printf("Eklemek istenilen dersin kodunu giriniz: ");
	scanf("%s", idOfClass);
	
	clsPtr = *headOfClassList;
	
	while(clsPtr != NULL && strcmp(idOfClass,clsPtr->ID) != 0){
		clsPtr = clsPtr->next;
	}
	
	stdPtr = findStudent(studentId, headOfStudentList);
	
	if(stdPtr == NULL){
		printf("Ders secimi yapmak istenilen ogrenci bulunamadi.\n");
		return;
	}	
	else if(clsPtr == NULL){
		printf("Eklemek istediginiz ders acilmadigi icin ders eklenemedi.\n");
		return;
	}
	else if((stdPtr->totalCredit) + clsPtr->credit > creditLimit){
		printf("Ogrenci kredisi yetersiz oldugu icin ders eklenemedi.\n");
		return;
	}
	else if( (stdPtr->numOfClasses)+1 > numOfClassLimit){
		printf("Ogrenci maksimum secilebilecek ders sayisina ulastigi icin ders eklenemedi.\n");
		return;
	}
	else if((clsPtr->numOfStudents)+1 > clsPtr->capacity){
		printf("Dersin kontenjani yetersiz oldugu icin ders eklenemedi.\n");
		return;
	}
	
	else if(strcmp(idOfClass,clsPtr->ID) == 0 && clsPtr != NULL){
		i = 0;
		
		while(i<(clsPtr->numOfStudents) && *((clsPtr->idsOfStudents)+i) != studentId){
			i++;
		}
		if(i < (clsPtr->numOfStudents) ){
			printf("%s kodlu ders daha onceden secilmis tekrar secilemez. \n", clsPtr->ID);
			return;
		}
		
		(clsPtr->numOfStudents)++;
		clsPtr->idsOfStudents = realloc(clsPtr->idsOfStudents,clsPtr->numOfStudents*sizeof(int));
		*((clsPtr->idsOfStudents)+clsPtr->numOfStudents-1) = studentId;
		sortList(clsPtr->idsOfStudents, clsPtr->numOfStudents);
		
		updateStudentCredit(headOfStudentList,*((clsPtr->idsOfStudents)+i),clsPtr->credit,increaseCredit,increaseNumOfClass);
				
		clsRegPtr = *headOfClassRegList;
		
		while(clsRegPtr != NULL && !isRecordAlreadyExists){
			if(strcmp(clsRegPtr->idOfClass,idOfClass) == 0 && clsRegPtr->idOfStudent == studentId)
				isRecordAlreadyExists = 1;
			
			else
				clsRegPtr = clsRegPtr->next;
		}
		
		if(isRecordAlreadyExists == 1){
			strcpy(clsRegPtr->state,"kayitli");
			printf("%s kodlu ders basariyla alindi.\n",idOfClass);
		}
		else{
			clsRegPtr = *headOfClassRegList;
		
			if(*headOfClassRegList == NULL){
				*headOfClassRegList = createClassRec(prevRecordId+1, idOfClass, studentId); 
			}
			else{
				while(clsRegPtr->next != NULL){
					clsRegPtr = clsRegPtr->next;
					prevRecordId = clsRegPtr->ID;
				}
		
				clsRegPtr->next = createClassRec(prevRecordId+1, idOfClass, studentId);
				printf("%s kodlu ders basariyla alindi.\n",idOfClass);
			}
		}
	}
	else{
		printf("HATA selectClass()\n");
	}	
}

void removeStudentFromClass(CLASS** headOfClassList, STUDENT** headOfStudentList, STUDENT** tailOfStudentList,CLASSREGISTIRATION** headOfClassRegList,int studentId){
	CLASS* clsPtr;
	STUDENT* stdPtr;
	CLASSREGISTIRATION* ptr;
	char idOfClass[10];
	int i,j;
	
	clsPtr = *headOfClassList;
	stdPtr = *headOfStudentList;
	
	while(stdPtr != NULL && stdPtr->ID != studentId)	
		stdPtr = stdPtr->next;
			
	if(stdPtr == NULL){
		printf("Kayitli olmayan bir ogrenciyle ders silme islemi yapilamaz.\n");
		return;
	}
	
	printf("Ogrencinin Aldigi Dersler:\n");
	while(clsPtr != NULL){
		for(i=0; i<(clsPtr->numOfStudents); i++){
			if(*((clsPtr->idsOfStudents)+i) == studentId){
				printf("%s\t%s\n",clsPtr->ID,clsPtr->name);
			}
		}
		clsPtr = clsPtr->next;	
	}
	
	printf("Silmek istediginiz dersin kodunu giriniz: ");
	scanf("%s", idOfClass);
	
	clsPtr = *headOfClassList;
	
	while(clsPtr != NULL && strcmp(clsPtr->ID,idOfClass) != 0){
		clsPtr = clsPtr->next; 
	}
	
	if(strcmp(clsPtr->ID,idOfClass) == 0){
		i = 0;
		while(i<clsPtr->numOfStudents && *((clsPtr->idsOfStudents)+i) != studentId){
			i++;
		}
		
		if(i == clsPtr->numOfStudents){
			printf("Ogrenci dersi almiyor. Ders silinemez.\n");
			return;
		}
				
		for(j=i; j<clsPtr->numOfStudents; j++){
			*((clsPtr->idsOfStudents)+j) = *((clsPtr->idsOfStudents)+j+1);
		}
		
		(clsPtr->numOfStudents)--;
		
		if((studentId-(*headOfStudentList)->ID) < (studentId-(*tailOfStudentList)->ID) ){
			stdPtr = *headOfStudentList;
			
			while(stdPtr != NULL && stdPtr->ID != studentId)	
				stdPtr = stdPtr->next;
				
			if(stdPtr->ID == studentId){
				(stdPtr->numOfClasses)--;
				stdPtr->totalCredit -= clsPtr->credit;
			}
		}
		else{	
			stdPtr = *tailOfStudentList;
	
			while(stdPtr != NULL && stdPtr->ID != studentId)	
				stdPtr = stdPtr->prev;

			if(stdPtr->ID == studentId){
				(stdPtr->numOfClasses)--;
				stdPtr->totalCredit -= clsPtr->credit;
			}
		}
	}
		
	ptr = *headOfClassRegList;
	
	while(ptr != NULL){
		if(strcmp(ptr->idOfClass,idOfClass) == 0 && ptr->idOfStudent == studentId){
			strcpy(ptr->state, "sildi");
			printf("Ders basariyla silindi.\n");
		}	
		ptr = ptr->next;
	}	
}

void updateStudentsFile(STUDENT** head){
	STUDENT* ptr;
	FILE* fp;
	
	ptr = *head;
	fp = fopen("ogrenciler.txt", "w");
	
	while(ptr != NULL){
		fprintf(fp,"%d,%s,%s,%d,%d\n",ptr->ID, ptr->name,ptr->surname,ptr->totalCredit,ptr->numOfClasses);	
		ptr = ptr->next;
	}	
}

void updateClassesFile(CLASS** head){
	CLASS* ptr;
	FILE *fp;
	
	ptr = *head;
	fp = fopen("dersler.txt", "w");
	
	while(ptr != NULL){
		fprintf(fp,"%s,%s,%d,%d\n",ptr->ID,ptr->name,ptr->credit,ptr->capacity);
		ptr = ptr->next;
	}
}

void updateClassRegistirationFile(CLASSREGISTIRATION** head){
	CLASSREGISTIRATION* ptr;
	FILE *fp;
	
	ptr = *head;
	fp = fopen("OgrenciDersKayit.txt", "w");
	
	while(ptr != NULL){
		fprintf(fp,"%d,%s,%d,%s,%s\n",ptr->ID,ptr->idOfClass,ptr->idOfStudent,ptr->date,ptr->state);
		ptr = ptr->next;
	}
}