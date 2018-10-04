//Составить программу построчной обработки текста. Суть обработки - отбор строк, содержащих одинаковое количество открывающих и закрывающих круглых скобок.
//Программа считывает входные данные со стандартного ввода, и печатает результат в стандартный вывод.
//Процедура отбора нужных строк должна быть оформлена в виде отдельной функции, которой на вход подается массив строк (который необходимо обработать), количество переданных строк, а также указатель на переменную, в которой необходимо разместить результат - массив отобранных строк.
//В качестве возвращаемого значения функция должна возвращать количество строк, содержащихся в результирующем массиве.
//Программа должна уметь обрабатывать ошибки - такие как неверные входные данные(отсутствие входных строк) или ошибки выделения памяти и т.п.
//В случае возникновения ошибки нужно выводить об этом сообщение "[error]" и завершать выполнение программы.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для освобождения памяти для двумерного массива
// Принимает указатель на массив указателей и размер массива
void freeArray(char** stringArray, int sizeOfStringArray) {
    if(stringArray != NULL) {
		for(int i = 0; i < sizeOfStringArray; i++) 
			free(stringArray[i]);
		free(stringArray);
    }
}

// В случае недостатка памяти генерируется "исключение".
// Память, которая выделялась ранее, освобождается, выводится сообщение "[error]" в стандартный поток вывода, программа завершается успешно
// Если память не была выделена вообще, но исключение сработало, помещаем в char* str NULL, в size - 0
void genMemoryError(char** str, int size) {
    freeArray(str, size);
    printf("[error]");
    exit(EXIT_SUCCESS);
}

// Функция обработки строк.
// Принимает указатель на обрабатываемый массив строк, пустой указатель на новый массив указателей, куда поместится результат обработки, размер обрабатываемого массива.
// Возвращает размер уже заполненного нового массива. 
int processString(char** stringArray, char*** newStringArray, int sizeOfStringArray) {
    int counter = 0, sizeOfNewStringArray = 0;
    char* iterator = NULL;
    (*newStringArray) = (char**)calloc(sizeOfStringArray, sizeof(char*));
    if((*newStringArray) == NULL) {
		freeArray(stringArray, sizeOfStringArray);
		genMemoryError(NULL, 0);
    }
    for(int i = 0; i < sizeOfStringArray; i++, counter = 0) {
		iterator = stringArray[i];
		for(; *iterator != '\0' && counter >= 0 ; iterator++) {
			if( *iterator == '(')
				counter++;
			if( *iterator == ')')
				counter--;
		}
		if( *iterator  == '\0' && counter == 0) {
			(*newStringArray)[sizeOfNewStringArray++] = (char*)calloc(strlen(stringArray[i]) + 1, sizeof(char));
			if((*newStringArray)[sizeOfNewStringArray - 1] == NULL) {
				freeArray(stringArray, sizeOfStringArray);
				genMemoryError(*newStringArray, sizeOfNewStringArray - 1);
			}
			strcpy((*newStringArray)[sizeOfNewStringArray - 1], stringArray[i]);
		}		
    }
    return sizeOfNewStringArray;
}

// Функция перевыделения памяти в случае, если capacity и size совпадают
// Принимает указатель на массив указателей и новый размер массива
void reallocArray(char*** ptr, int newSize) {
	(*ptr) = (char**)realloc((*ptr), newSize*sizeof(char*));
	if((*ptr) == NULL) 
		genMemoryError(NULL, 0);
}

// Функция для чтения строк и страндартного ввода.
// Принимает пустой указатель.
// Возвращает количество считанных строк
int readFromStdInput(char*** stringArray) {
    const int BUFFER_SIZE = 16384;
	char buffer[BUFFER_SIZE];
    int capacity = 1; 
	int sizeOfStringArray = 0;
	if(!fgets(buffer, BUFFER_SIZE, stdin)) {
		printf("[error]");
        exit(EXIT_SUCCESS);
    }
	(*stringArray) = (char**)calloc(capacity, sizeof(char*));
	if((*stringArray) == NULL)  
		genMemoryError(NULL, 0);
	while(1){
		if(sizeOfStringArray == capacity) {
			capacity *= 2;
			reallocArray(&(*stringArray), capacity);
		}
		(*stringArray)[sizeOfStringArray++] = (char*)calloc(strlen(buffer) + 1, sizeof(char));
		if((*stringArray)[sizeOfStringArray - 1] == NULL)
			genMemoryError((*stringArray), sizeOfStringArray - 1);
		strcpy((*stringArray)[sizeOfStringArray - 1], buffer);
		if(fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
			break;
		}
	}
	return sizeOfStringArray;
}

// Функция для вывода строк в стандартный вывод
// Пинимает указатель на массива указателей и размер массива
void printStringArray(char** stringArray, int sizeOfStringArray) {
	if(stringArray != NULL) {
		for(int i = 0; i < sizeOfStringArray; i++) {
			printf("%s", stringArray[i]);
		}
	}
}

int main(void) { 
	char** stringArray = NULL;
	char ** newStringArray = NULL;  
	int sizeOfStringArray = readFromStdInput(&stringArray);
	int sizeOfNewStringArray = processString(stringArray, &newStringArray, sizeOfStringArray); 
	printStringArray(newStringArray, sizeOfNewStringArray);
    freeArray(stringArray, sizeOfStringArray);
	freeArray(newStringArray, sizeOfNewStringArray);
	return 0;
} 
