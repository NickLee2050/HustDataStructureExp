#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

//Preliminaries
typedef int Status;
#define TRUE 1
#define OK 1
#define FALSE 0
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define LIST_INIT_SIZE 100
#define LIST_INCREMENT 10

//Data Structure design
typedef int ElemType;
typedef struct
{
    ElemType *elem;
    int length;
    int size;
} SqList;

//Functions
Status compare(ElemType a, ElemType b);
Status InitList(SqList *L);
Status DestroyList(SqList *L);
Status ClearList(SqList *L);
Status ListEmpty(SqList *L);
int ListLength(SqList *L);
Status GetElem(SqList *L, int i, ElemType *e);
Status LocateElem(SqList *L, int *i, ElemType e, Status cmp(ElemType, ElemType));
Status PriorElem(SqList *L, ElemType cur_e, ElemType *pre_e);
Status NextElem(SqList *L, ElemType cur_e, ElemType *next_e);
Status ListInsert(SqList *L, int i, ElemType e);
Status ListDelete(SqList *L, int i, ElemType *e);
Status ListTraverse(SqList *L, Status vt(SqList *L, int i, ElemType *e));
Status ShowIndex(SqList **index);
Status ReadLists(FILE *DSR, SqList **index);
Status SaveLists(FILE *DSS, SqList **idnex);
static int init_mk = 0; //Count the number of existing lists
int main(void)
{
    //Necessary variables
    int mark = -1;
    int len, position, idx_pos;
    ElemType element;
    ElemType get;
    SqList *index[LIST_INIT_SIZE] = {NULL};

    do
    {
        printf("          Menu for Linear Table On Sequence Structure\n");
        printf("----------------------------------------------------------------\n");
        printf("    1. InitList         6. GetElem          11. ListDelete\n");
        printf("    2. DestroyList      7. LocateElem       12. ListTraverse\n");
        printf("    3. ClearList        8. PriorElem        13. ShowIndex\n");
        printf("    4. ListEmpty        9. NextElem         14. ReadLists\n");
        printf("    5. ListLength       10. ListInsert      15. SaveLists\n");
        printf("                        0. Exit\n");
        printf("----------------------------------------------------------------\n");
        printf("\nInput a Number for your operation in range [0, 15], end with <Enter>:\n");
        while (1)
        {
            scanf("%d", &mark);
            if (mark >= 0 && mark <= 15)
                break;
            printf("Illegal input, please re-input a Number in range [0, 15], end with <Enter>:\n");
        }
        if (mark >= 2 && mark <= 12)
        {
            if (!init_mk)
            {
                printf("WARNING: No List! Unable to do any manipulations! Initialize a List FIRST!\n");
                continue;
            }
            else
            {
                printf("Please input the Number of List in range [1, %d], end with <Enter>\n", init_mk);
                while (1)
                {
                    scanf("%d", &idx_pos);
                    if (idx_pos >= 1 && idx_pos <= init_mk)
                        break;
                    printf("Illegal input, please re-input a Number in range [1, %d], end with <Enter>:\n", init_mk);
                }
            }
        }

        switch (mark)
        {
        case 0:
            break;
        case 1:
            index[init_mk + 1] = (SqList *)malloc(sizeof(SqList));
            if (InitList(index[init_mk + 1]))
            {
                init_mk++;
                printf("Initialized!\n");
            }
            else
            {
                index[init_mk + 1] = NULL;
                printf("Failed!\n");
            }
            break;
        case 2:
            if (!init_mk)
            {
                printf("No List!\n");
                break;
            }
            if (DestroyList(index[idx_pos]))
            {
                free(index[idx_pos]);
                for (int i = idx_pos; i < init_mk; i++)
                {
                    index[i] = index[i + 1];
                }
                index[init_mk] = NULL;
                init_mk--;
                printf("Destroyed!\n");
            }
            else
                printf("Failed!\n");
            break;
        case 3:
            if (!init_mk)
            {
                printf("No List!\n");
                break;
            }
            if (ClearList(index[idx_pos]))
                printf("Clear!\n");
            else
                printf("Failed!\n");
            break;
        case 4:
            if (!init_mk)
            {
                printf("No List!\n");
                break;
            }
            switch (ListEmpty(index[idx_pos]))
            {
            case TRUE:
                printf("Empty!\n");
                break;
            case FALSE:
                printf("Not Empty!\n");
                break;
            case INFEASIBLE:
                printf("Failed!\n");
                break;
            }
            break;
        case 5:
            if (!init_mk)
            {
                printf("No List!\n");
                break;
            }
            len = ListLength(index[idx_pos]);
            if (len == INFEASIBLE)
                printf("Not a legal Sequential List.\n");
            else
                printf("Length:%d\n", len);
            break;
        case 6:
            if (!init_mk)
            {
                printf("No List!\n");
                break;
            }
            if (!index[idx_pos]->length)
            {
                printf("Empty List!\n");
                break;
            }
            printf("Please input the position in range [1, %d], end with <Enter>\n", index[idx_pos]->length);
            while (1)
            {
                scanf("%d", &position);
                if (position >= 1 && position <= index[idx_pos]->length)
                    break;
                printf("Illegal input, please re-input a Number in range [1, %d], end with <Enter>\n", index[idx_pos]->length);
            }
            if (!GetElem(index[idx_pos], position, &element))
                printf("Failed!\n");
            else
                printf("The element at position %d is %d\n", position, element);
            break;
        case 7:
            if (!init_mk)
            {
                printf("No List!\n");
                break;
            }
            if (!index[idx_pos]->length)
            {
                printf("Empty List!\n");
                break;
            }
            printf("Please input the element to find, end with <Enter>\n");
            scanf("%d", &element);
            switch (LocateElem(index[idx_pos], &position, element, compare))
            {
            case OK:
                printf("The element '%d' is at position %d\n", element, position + 1);
                break;
            case ERROR:
                printf("Failed!\n");
                break;
            case INFEASIBLE:
                printf("Not Found!\n");
                break;
            }
            break;
        case 8:
            if (!init_mk)
            {
                printf("No List!\n");
                break;
            }
            if (!index[idx_pos]->length)
            {
                printf("Empty List!\n");
                break;
            }
            printf("Please input the current element, end with <Enter>\n");
            scanf("%d", &element);
            if (!PriorElem(index[idx_pos], element, &get))
                printf("Failed!\n");
            else
                printf("Last element is '%d'\n", get);
            break;
        case 9:
            if (!init_mk)
            {
                printf("No List!\n");
                break;
            }
            if (!index[idx_pos]->length)
            {
                printf("Empty List!\n");
                break;
            }
            printf("Please input the current element, end with <Enter>\n");
            scanf("%d", &element);
            if (!NextElem(index[idx_pos], element, &get))
                printf("Failed!\n");
            else
                printf("Next element is '%d'\n", get);
            break;
        case 10:
            if (!init_mk)
            {
                printf("No List!\n");
                break;
            }
            if (!index[idx_pos]->length)
            {
                position = 1;
                printf("Please input the element, end with <Enter>\n");
                scanf("%d", &element);
            }
            else
            {
                printf("Please input the element, end with <Enter>\n");
                scanf("%d", &element);
                printf("Please input the position in range [1, %d], end with<Enter>\n", index[idx_pos]->length + 1);
                while (1)
                {
                    scanf("%d", &position);
                    if (position >= 1 && position <= index[idx_pos]->length + 1)
                        break;
                    printf("Illegal input, please re-input a number in range [1, %d], end with <Enter>\n", index[idx_pos]->length + 1);
                }
            }
            if (!ListInsert(index[idx_pos], position, element))
                printf("Failed!\n");
            else
                printf("Inserted!\n");
            break;
        case 11:
            if (!init_mk)
            {
                printf("No List!\n");
                break;
            }
            if (!index[idx_pos]->length)
            {
                printf("Empty List!\n");
                break;
            }
            printf("Please input the position in range [1, %d], end with<Enter>\n", index[idx_pos]->length);
            while (1)
            {
                scanf("%d", &position);
                if (position >= 0 && position <= index[idx_pos]->length)
                    break;
                printf("Illegal input, please re-input a number in range [1, %d], end with <Enter>\n", index[idx_pos]->length);
            }
            if (!ListDelete(index[idx_pos], position, &element))
                printf("Failed!\n");
            else
                printf("Deleted!\n");
            break;
        case 12:
            if (!init_mk)
            {
                printf("No List!\n");
                break;
            }
            if (!index[idx_pos]->length)
            {
                printf("Empty List!\n");
                break;
            }
            if (!ListTraverse(index[idx_pos], &GetElem))
                printf("Failed!\n");
            break;
        case 13:
            if (!init_mk)
            {
                printf("No List!\n");
                break;
            }
            else if (!ShowIndex(index))
            {
                printf("Failed!\n");
                break;
            }

            break;
        case 14:
            if (init_mk)
                printf("Lists exist, save them first.\n");
            else
            {
                FILE *DSR = fopen("./SqListData.txt", "r");
                if (ReadLists(DSR, index))
                    printf("Read successful!\n");
                else
                    printf("Failed!\n");
                fclose(DSR);
            }
            break;
        case 15:
            if (!init_mk)
                printf("No List!\n");
            else
            {
                FILE *DSS = fopen("./SqListSave.txt", "w");
                if (SaveLists(DSS, index))
                    printf("Saved!\n");
                else
                    printf("Failed!\n");
                fclose(DSS);
            }
            break;
        }
    } while (mark);
    return 0;
}

Status compare(ElemType a, ElemType b)
{
    if (a != b)
        return FALSE;
    return TRUE;
}
Status InitList(SqList *L)
{
    if (!L)
        return ERROR;
    L->elem = (ElemType *)malloc(sizeof(ElemType) * LIST_INIT_SIZE);
    if (!L->elem)
        return ERROR;
    L->length = 0;
    L->size = LIST_INIT_SIZE;
    return OK;
}
Status DestroyList(SqList *L)
{
    if (!L)
        return ERROR;
    free(L->elem);
    L->elem = NULL;
    return OK;
}
Status ClearList(SqList *L)
{
    if (!L)
        return ERROR;
    L->length = 0;
    return OK;
}
Status ListEmpty(SqList *L)
{
    if (!L)
        return INFEASIBLE; //Make a difference from the values of TRUE and FALSE.
    if (!(L->length))
        return TRUE;
    return FALSE;
}
int ListLength(SqList *L)
{
    if (!L)
        return INFEASIBLE; //The minimum length of a list is 0, using INFEASIBLE to mark illegal pointers of L.
    return L->length;
}
Status GetElem(SqList *L, int i, ElemType *e)
{
    if (!L || !L->elem)
        return ERROR;
    if (i < 1 || i > L->length)
        return ERROR;
    *e = L->elem[i - 1];
    return OK;
}
Status LocateElem(SqList *L, int *i, ElemType e, Status cmp(ElemType, ElemType))
{
    if (!L || !L->elem)
        return ERROR; //Wrong parameters.
    for (int k = 0; k < L->length; k++)
    {
        if (cmp(L->elem[k], e))
        {
            *i = k;
            return OK;
        }
    }
    return INFEASIBLE; //Not found.
}
Status PriorElem(SqList *L, ElemType cur_e, ElemType *pre_e)
{
    if (!L || !L->elem)
        return ERROR;
    int i;
    Status mark;
    mark = LocateElem(L, &i, cur_e, &compare);
    if ((mark != OK) || !i)
        return ERROR; //Not found, Wrong parameters, First position.
    *pre_e = L->elem[i - 1];
    return OK;
}
Status NextElem(SqList *L, ElemType cur_e, ElemType *next_e)
{
    if (!L || !L->elem)
        return ERROR;
    int i, mark;
    mark = LocateElem(L, &i, cur_e, &compare);
    if ((mark != OK) || (i == L->length - 1))
        return ERROR; //Not found, Wrong parameters, Last position.
    *next_e = L->elem[i + 1];
    return OK;
}
Status ListInsert(SqList *L, int i, ElemType e)
{
    if (!L || !L->elem)
        return ERROR;
    if (i < 1 || i > L->length + 1) //User input domain is [1, L->length + 1].
        return ERROR;
    if (L->length >= L->size)
    {
        ElemType *newbase = realloc(L->elem, (L->size + LIST_INCREMENT) * sizeof(ElemType));
        if (!newbase)
            return OVERFLOW;
        L->size += LIST_INCREMENT;
    }
    for (int k = L->length - 1; k >= i - 1; k--) //Consider the gap.
        L->elem[k + 1] = L->elem[k];
    L->elem[i - 1] = e;
    ++L->length;
    return OK;
}
Status ListDelete(SqList *L, int i, ElemType *e)
{
    if (!L || !L->elem)
        return ERROR;
    if (i < 1 || i > L->length) //User input domain is [1, L->length].
        return ERROR;
    *e = L->elem[i - 1];
    for (int k = i - 1; k < L->length - 1; k++) //Consider the gap to allow natural input for users
        L->elem[k] = L->elem[k + 1];
    --L->length;
    return OK;
}
Status ListTraverse(SqList *L, Status vt(SqList *L, int i, ElemType *e))
{
    if (!L || !L->elem)
        return ERROR;
    int j = 0;
    ElemType e;
    if (!L->length)
    {
        printf("<Empty>\n");
        return OK;
    }
    for (; j < L->length - 1; j++)
    {
        if (!vt(L, j + 1, &e))
            return ERROR;
        printf("%d ", e);
    }
    if (!vt(L, j + 1, &e))
        return ERROR;
    printf("%d\n", e);
    return OK;
}
Status ShowIndex(SqList **index)
{
    if (!init_mk || !index)
        return ERROR;
    for (int i = 1; i <= init_mk; i++)
    {
        printf("List %d:\n", i);
        if (!ListTraverse(index[i], &GetElem))
            return ERROR;
    }
    return OK;
}
Status ReadLists(FILE *DSR, SqList **index)
{
    if (!DSR || !index)
        return ERROR;
    ElemType d;
    char c;
    while (1)
    {
        index[init_mk + 1] = (SqList *)malloc(sizeof(SqList));
        if (InitList(index[init_mk + 1]))
            init_mk++;
        else
            return ERROR;
        while (1)
        {
            c = fgetc(DSR);
            if (c == EOF)
            {
                free(index[init_mk]);
                index[init_mk] = NULL;
                init_mk--;
                return OK;
            }
            if (c == '\n')
                break;
            if (c != ' ')
                ungetc(c, DSR);
            fscanf(DSR, "%d", &d);
            ListInsert(index[init_mk], index[init_mk]->length + 1, d);
        }
    }
}
Status SaveLists(FILE *DSS, SqList **index)
{
    if (!DSS || !index)
        return ERROR;
    int j;
    for (int i = 1; i <= init_mk; i++)
    {
        if (!index[i]->length)
        {
            fprintf(DSS, "\n");
            continue;
        }
        for (j = 0; j < index[i]->length - 1; j++)
            fprintf(DSS, "%d ", index[i]->elem[j]);
        fprintf(DSS, "%d\n", index[i]->elem[j]);
    }
    for (int i = 1; i <= init_mk; i++)
    {
        if (!DestroyList(index[i]))
            return ERROR;
        index[i] = NULL;
    }
    init_mk = 0;
    return OK;
}