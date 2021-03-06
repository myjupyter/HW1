//Составить программу построчной обработки текста. Суть обработки - отбор строк,
//содержащих одинаковое количество открывающих и закрывающих круглых скобок.
//Программа считывает входные данные со стандартного ввода, и печатает результат
//в стандартный вывод. Процедура отбора нужных строк должна быть оформлена в
//виде отдельной функции, которой на вход подается массив строк (который
//необходимо обработать), количество переданных строк, а также указатель на
//переменную, в которой необходимо разместить результат - массив отобранных
//строк. В качестве возвращаемого значения функция должна возвращать количество
//строк, содержащихся в результирующем массиве. Программа должна уметь
//обрабатывать ошибки - такие как неверные входные данные(отсутствие входных
//строк) или ошибки выделения памяти и т.п. В случае возникновения ошибки нужно
//выводить об этом сообщение "[error]" и завершать выполнение программы.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для освобождения памяти для двумерного массива
// Принимает указатель на массив указателей и размер массива
void freeArray(char **stringArray, int sizeOfStringArray) {
  if (stringArray != NULL) {
    for (int i = 0; i < sizeOfStringArray; i++)
      free(stringArray[i]);
    free(stringArray);
  }
}

// В случае недостатка памяти генерируется "исключение".
// Память, которая выделялась ранее, освобождается, выводится сообщение
// "[error]" в стандартный поток вывода, программа завершается успешно Если
// память не была выделена вообще, но исключение сработало, помещаем в char* str
// NULL, в size - 0
// Возвращает 0
int genMemoryError(char **str, int size) {
  freeArray(str, size);
  printf("[error]");
  return -1;
}

// Функция обработки строк.
// Принимает константный указатель на обрабатываемый массив строк, пустой указатель на новый
// массив указателей, куда поместится результат обработки, размер
// обрабатываемого массива. Возвращает размер уже заполненного нового массива.
int processString(char * const *stringArray, char ***newStringArray,
                  int sizeOfStringArray) {
  int counter = 0, sizeOfNewStringArray = 0;
  char const *iterator = NULL;
  (*newStringArray) = (char **)calloc(sizeOfStringArray, sizeof(char *));
  if ((*newStringArray) == NULL) {
    freeArray((char **)stringArray, sizeOfStringArray);
    return genMemoryError(NULL, 0);
  }
  for (int i = 0; i < sizeOfStringArray; i++, counter = 0) {
    iterator = stringArray[i];
    for (; *iterator != '\0' && counter >= 0; iterator++) {
      if (*iterator == '(')
        counter++;
      if (*iterator == ')')
        counter--;
    }
    if (*iterator == '\0' && counter == 0) {
      (*newStringArray)[sizeOfNewStringArray++] = strdup(stringArray[i]);
      if ((*newStringArray)[sizeOfNewStringArray - 1] == NULL) {
        freeArray((char **)stringArray, sizeOfStringArray);
        return genMemoryError(*newStringArray, sizeOfNewStringArray - 1);
      }
    }
  }
  return sizeOfNewStringArray;
}

// Функция перевыделения памяти в случае, если capacity и size совпадают
// Принимает указатель на массив указателей и старый capacity массива
int reallocArray(char ***ptr, int* oldSize) {
  char **handler = (char **)realloc((*ptr), (*oldSize + (*oldSize)/2) * sizeof(char *));
  if (!handler) {
    return genMemoryError((*ptr), *oldSize);
  }
  *oldSize = *oldSize + (*oldSize)/2;
  (*ptr) = handler;
  return 0;
}

// Функция для чтения строк и страндартного ввода.
// Принимает пустой указатель.
// Возвращает количество считанных строк
int readFromStdInput(char ***stringArray) {
  int capacity = 16;
  char *buffer = NULL;
  int sizeOfStringArray = 0;
  if (scanf(" %m[^\n]", &buffer) == -1) {
    printf("[error]");
    return -1;
  }
  (*stringArray) = (char **)calloc(capacity, sizeof(char *));
  if ((*stringArray) == NULL)
    return genMemoryError(NULL, 0);
  while (1) {
    if (sizeOfStringArray == capacity) 
      if(reallocArray(&(*stringArray), &capacity) == -1) {
        return -1;
      }
    (*stringArray)[sizeOfStringArray++] = strdup(buffer);
    if ((*stringArray)[sizeOfStringArray - 1] == NULL)
      return genMemoryError((*stringArray), sizeOfStringArray - 1);
    free(buffer);
    buffer = NULL;
    if (scanf(" %m[^\n]", &buffer) == -1) {
      break;
    }
  }
  return sizeOfStringArray;
}

// Функция для вывода строк в стандартный вывод
// Пинимает указатель на массива указателей и размер массива
void printStringArray(char **stringArray, int sizeOfStringArray) {
  if (stringArray != NULL) {
    for (int i = 0; i < sizeOfStringArray; i++) {
      printf("%s\n", stringArray[i]);
    }
  }
}

int main(void) {
  char **stringArray = NULL;
  char **newStringArray = NULL;
  int sizeOfStringArray = readFromStdInput(&stringArray);
  if(sizeOfStringArray == -1)
    return 0;
  int sizeOfNewStringArray = processString(stringArray,
                                           &newStringArray, sizeOfStringArray);
  if(sizeOfNewStringArray == -1)
    return 0;
  printStringArray(newStringArray, sizeOfNewStringArray);
  freeArray(stringArray, sizeOfStringArray);
  freeArray(newStringArray, sizeOfNewStringArray);
  return 0;
}
