#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

//Preliminaries
#define TRUE 1
#define OK 1
#define FALSE 0
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAXLEN 100
typedef int Status;
static int init_mk = 0; //Count the number of existing lists

//Data Structure design
typedef int ElemType;
struct LNode
{
    ElemType data;
    struct LNode *next;
} typedef LNode;
struct LinkedListPointer
{
    LNode *head;
    int length;
} typedef Lp;

//Functions
Status compare(ElemType a, ElemType b);
Status InitList(Lp **index);
Status DestroyList(Lp **index, int pos);
Status ClearList(Lp *lp);
Status ListEmpty(Lp *lp);
int ListLength(Lp *lp);
Status GetElem(Lp *lp, int i, ElemType *e);
Status LocateElem(Lp *lp, int *i, ElemType e, Status cmp(ElemType, ElemType));
Status PriorElem(Lp *lp, ElemType cur_e, ElemType *pre_e, Status cmp(ElemType, ElemType));
Status NextElem(Lp *lp, ElemType cur_e, ElemType *next_e, Status cmp(ElemType, ElemType));
Status ListInsert(Lp *lp, int i, ElemType e);
Status ListDelete(Lp *lp, int i, ElemType *e);
Status ListTraverse(Lp *lp);
Status ShowIndex(Lp **index);
Status ReadLists(FILE *DSR, Lp **index);
Status SaveLists(FILE *DSS, Lp **index);
Status ShowIndex(Lp **index);
Status ReadLists(FILE *DSR, Lp **index);
Status SaveLists(FILE *DSS, Lp **idnex);
int main(void)
{
    //Necessary variables
    int mark = -1;
    int len, position, idx_pos;
    ElemType element;
    ElemType get;
    Lp *index[MAXLEN] = {NULL};
    do
    {
        printf("          Menu for Linear Table On Linked Structure\n");
        printf("----------------------------------------------------------------\n");
        printf("    1. InitList         6. GetElem          11. ListDelete\n");
        printf("    2. DestroyList      7. LocateElem       12. ListTraverse\n");
        printf("    3. ClearList        8. PriorElem        13. ShowIndex\n");
        printf("    4. ListEmpty        9. NextElem         14. ReadLists\n");
        printf("    5. ListLength       10. ListInsert      15. SaveLists\n");
        printf("                        0. Exit\n");
        printf("----------------------------------------------------------------\n");
        printf("Input a Number for your operation in range [0, 15], end with <Enter>:\n");
        while (1)
        {
            scanf("%d", &mark);
            if (mark >= 0 && mark <= 15)
                break;
            printf("Illegal input, please re-input a Number in range [0, 15], end with <Enter>:\n");
        }
        if (!mark)
            break;
        if (mark != 1 && mark != 14)
        {
            if (!init_mk)
            {
                printf("WARNING: No List! Unable to do any manipulations! Initialize or read Lists FIRST!\n");
                continue;
            }
            else if (mark != 13 && mark != 15)
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
        case 1:
            if (InitList(index))
                printf("Initialized!\n");
            else
                printf("Failed!\n");
            break;
        case 2:
            if (DestroyList(index, idx_pos))
            {
                printf("Destroyed!\n");
                for (int i = idx_pos; i < init_mk + 1; i++)
                {
                    index[i] = index[i + 1];
                }
                index[init_mk + 1] = NULL;
            }
            else
                printf("Failed!\n");
            break;
        case 3:
            if (ClearList(index[idx_pos]))
                printf("Clear!\n");
            else
                printf("Failed!\n");
            break;
        case 4:
            switch (ListEmpty(index[idx_pos]))
            {
            case TRUE:
                printf("Empty!\n");
                break;
            case FALSE:
                printf("Not Empty!\n");
                break;
            case INFEASIBLE:
                printf("Not a legal List.\n");
                break;
            }
            break;
        case 5:
            len = ListLength(index[idx_pos]);
            if (len == INFEASIBLE)
                printf("Not a legal List.\n");
            else
                printf("Length:%d\n", len);
            break;
        case 6:
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
                printf("The element at position %d is '%d'\n", position, element);
            break;
        case 7:
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
                printf("The element '%d' is at position %d\n", element, position);
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
            if (!index[idx_pos]->length)
            {
                printf("Empty List!\n");
                break;
            }
            printf("Please input the current element, end with <Enter>\n");
            scanf("%d", &element);
            if (!PriorElem(index[idx_pos], element, &get, &compare))
                printf("Failed!\n");
            else
                printf("Last element is '%d'\n", get);
            break;
        case 9:
            if (!index[idx_pos]->length)
            {
                printf("Empty List!\n");
                break;
            }
            printf("Please input the current element, end with <Enter>\n");
            scanf("%d", &element);
            if (!NextElem(index[idx_pos], element, &get, &compare))
                printf("Failed!\n");
            else
                printf("Next element is '%d'\n", get);
            break;
        case 10:
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
            if (!ListTraverse(index[idx_pos]))
                printf("Failed!\n");
            break;
        case 13:
            if (!ShowIndex(index))
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
                FILE *DSR = fopen("./LinkedListData.txt", "r");
                if (ReadLists(DSR, index))
                    printf("Read successful!\n");
                else
                    printf("Failed!\n");
                fclose(DSR);
            }
            break;
        case 15:
        {
            FILE *DSS = fopen("./LinkedListSave.txt", "w");
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
Status InitList(Lp **index)
{
    if (!(index[init_mk + 1] = (Lp *)malloc(sizeof(Lp))))
        return ERROR;
    if (!(index[init_mk + 1]->head = (LNode *)malloc(sizeof(LNode))))
        return ERROR;
    index[init_mk + 1]->head->next = NULL;
    index[init_mk + 1]->length = 0;
    init_mk++;
    return OK;
}
Status DestroyList(Lp **index, int pos)
{
    if (!index[pos])
        return ERROR;
    LNode *p1 = index[pos]->head;
    LNode *p2;
    do
    {
        p2 = p1->next;
        free(p1);
        p1 = p2;
    } while (p1);
    free(index[pos]);
    index[pos] = NULL;
    init_mk--;
    return OK;
}
Status ClearList(Lp *lp)
{
    if (!lp || !lp->head)
        return ERROR;
    LNode *p1 = lp->head->next;
    LNode *p2;
    while (p1)
    {
        p2 = p1->next;
        free(p1);
        p1 = p2;
    }
    lp->head->next = NULL;
    lp->length = 0;
    return OK;
}
Status ListEmpty(Lp *lp)
{
    if (!lp || !lp->head)
        return INFEASIBLE; //Make a difference from the values of TRUE and FALSE.
    if (!lp->length)
        return TRUE;
    return FALSE;
}
int ListLength(Lp *lp)
{
    if (!lp || !lp->head)
        return INFEASIBLE; //The minimum length of a list is 0, using INFEASIBLE to mark illegal pointers of L.
    return lp->length;
}
Status GetElem(Lp *lp, int i, ElemType *e)
{
    if (!lp || !lp->head)
        return ERROR;
    if (i < 1 || i > lp->length)
        return ERROR;
    LNode *p = lp->head->next;
    for (int count = 1; count < i; count++)
        p = p->next;
    *e = p->data;
    return OK;
}
Status LocateElem(Lp *lp, int *i, ElemType e, Status cmp(ElemType, ElemType))
{
    if (!lp || !lp->head)
        return ERROR; //Wrong parameters.
    LNode *p = lp->head->next;
    for (int count = 1; count <= lp->length; count++)
    {
        if (cmp(p->data, e))
        {
            *i = count;
            return OK;
        }
        p = p->next;
    }
    return INFEASIBLE; //Not found.
}
Status PriorElem(Lp *lp, ElemType cur_e, ElemType *pre_e, Status cmp(ElemType, ElemType))
{
    if (!lp || !lp->head)
        return ERROR; //Wrong parameters.
    if (lp->length < 2)
        return ERROR; //No prior element.
    LNode *p1 = lp->head->next;
    LNode *p2 = lp->head->next->next;
    for (int count = 2; count <= lp->length; count++)
    {
        if (cmp(p2->data, cur_e))
        {
            *pre_e = p1->data;
            return OK;
        }
        p1 = p1->next;
        p2 = p2->next;
    }
    return ERROR;
}
Status NextElem(Lp *lp, ElemType cur_e, ElemType *next_e, Status cmp(ElemType, ElemType))
{
    if (!lp || !lp->head)
        return ERROR; //Wrong parameters.
    if (lp->length < 2)
        return ERROR; //No next element.
    LNode *p1 = lp->head->next;
    LNode *p2 = lp->head->next->next;
    for (int count = 2; count <= lp->length; count++)
    {
        if (cmp(p1->data, cur_e))
        {
            *next_e = p2->data;
            return OK;
        }
        p1 = p1->next;
        p2 = p2->next;
    }
    return ERROR;
}
Status ListInsert(Lp *lp, int i, ElemType e)
{
    if (!lp || !lp->head)
        return ERROR;                //Wrong parameters.
    if (i < 1 || i > lp->length + 1) //User input domain is [1, L->length + 1].
        return ERROR;
    LNode *new = (LNode *)malloc(sizeof(LNode));
    if (!new)
        return ERROR; //Memory allocation failure.
    LNode *p1 = lp->head;
    LNode *p2 = lp->head->next;
    for (int count = 0; count < i - 1; count++)
    {
        p1 = p1->next;
        p2 = p2->next;
    }
    p1->next = new;
    new->next = p2;
    new->data = e;
    lp->length++;
    return OK;
}
Status ListDelete(Lp *lp, int i, ElemType *e)
{
    if (!lp || !lp->head)
        return ERROR;            //Wrong parameters.
    if (i < 1 || i > lp->length) //User input domain is [1, L->length].
        return ERROR;
    LNode *p1 = lp->head;
    LNode *p2 = lp->head->next;
    for (int count = 0; count < i - 1; count++)
    {
        p1 = p1->next;
        p2 = p2->next;
    }
    p1->next = p2->next;
    *e = p2->data;
    free(p2);
    lp->length--;
    return OK;
}
Status ListTraverse(Lp *lp)
{
    if (!lp || !lp->head)
        return ERROR; //Wrong parameters.
    LNode *p = lp->head->next;
    if (!p)
    {
        printf("<Empty>\n");
        return OK;
    }
    while (p->next)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("%d\n", p->data);
    return OK;
}
Status ShowIndex(Lp **index)
{
    if (!init_mk || !index)
        return ERROR;
    for (int i = 1; i <= init_mk; i++)
    {
        printf("List %d:\n", i);
        if (!ListTraverse(index[i]))
            return ERROR;
    }
    return OK;
}
Status ReadLists(FILE *DSR, Lp **index)
{
    if (!DSR || !index)
        return ERROR;
    ElemType d;
    char c;
    while (1)
    {
        if (!InitList(index))
            return ERROR;
        while (1)
        {
            c = fgetc(DSR);
            if (c == EOF)
            {
                DestroyList(index, init_mk);
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
Status SaveLists(FILE *DSS, Lp **index)
{
    if (!DSS || !index)
        return ERROR;
    LNode *p;
    for (int i = 1; i <= init_mk; i++)
    {
        p = index[i]->head->next;
        if (!index[i]->length)
        {
            fprintf(DSS, "\n");
            continue;
        }
        while (p->next)
        {
            fprintf(DSS, "%d ", p->data);
            p = p->next;
        }
        fprintf(DSS, "%d\n", p->data);
    }
    init_mk = 0;
    return OK;
}