#include <stdio.h>
#include <stdlib.h>

int main(void){
	char classCode[30],nameOfClass[60], c;
	char nameOfDoc[30] = "dersler.txt";
	char str[200];
	int credit;	
	FILE* fp;
	FILE* fpForCounting;
	int i = 1;
	int lineNumberOfDoc = 0;	
	
	fpForCounting = fopen(nameOfDoc, "r");
	
	for (c = getc(fpForCounting); c != EOF; c = getc(fpForCounting)){
    	if (c == '\n') 
        	lineNumberOfDoc = lineNumberOfDoc + 1;
	}
	
	printf("Line number: %d\n", lineNumberOfDoc);
	
	fclose(fpForCounting);
	
	fp = fopen(nameOfDoc, "r");
	
	if(fp == NULL){
		printf("HATA %s isimli dosya acilamadi\n", nameOfDoc);
	}
	
	else{
		while(lineNumberOfDoc != 0){ 
			fscanf(fp,"%s , %s , %d\n", classCode, nameOfClass,&credit);
			printf("%s\n", classCode);
			printf("%s\n", nameOfClass);
			printf("%d\n", credit);
			printf("%s\n", str);
			lineNumberOfDoc--;
		}
	}
	fclose(fp);
	
	return 0;
}
