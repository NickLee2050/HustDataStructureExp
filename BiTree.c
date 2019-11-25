#include <stdio.h>
#include <stdlib.h>
//Preliminaries
typedef int Status;
#define TRUE 1
#define OK 1
#define FALSE 0
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAXLEN 100
#define INIT_SIZE 100
#define NAME_LEN 20
#define SIZE_INCREAMENT 20

static int tree_mk = 0;

//Data Structure Design
typedef struct
{
    unsigned int ID;
    char name[NAME_LEN];
    unsigned int score;
} ElemType;
typedef struct BiTreeNode
{
    ElemType *data;
    struct BiTreeNode *lchild;
    struct BiTreeNode *rchild;
} btnode;
typedef struct BiTree
{
    btnode *head;
    int count;
} btm;

//Additional Data Structure Design
typedef btnode StackElem;
typedef btnode QueueElem;
typedef struct QNode
{
    QueueElem *data;
    struct QNode *next;
} QNode;
typedef struct
{
    QNode *head;
    QNode *tail;
    int length;
} Queue;
typedef struct Stack
{
    StackElem **data;
    int top;
    int size;
} Stack;

//Functions
int max(int a, int b);
Status InitBiTree(btm **index);
Status CreateBiTree(btm **index, FILE *def);
Status DestroyBiTree(btm **index, int i);
Status ClearBiTree(btm *T);
Status BiTreeEmpty(btm *T);
int BiTreeDepth(btnode *head);
btnode *LocateNode(btnode *head, unsigned int e);
Status Assign(btnode *head, unsigned int e, ElemType value);
btnode *GetSibling(btnode *head, unsigned int e);
btnode *GetParent(btnode *head, unsigned int e);
Status InsertNode(btm *T, unsigned int e, int LR, ElemType c);
Status DeleteNode(btm *T, unsigned int e);
Status PreOrderTraverse(btnode *head, Status visit(btnode *));
Status InOrderTraverse(btnode *head, Status visit(btnode *));
Status PostOrderTraverse(btnode *head, Status visit(btnode *));
Status LevelOrderTraverse(btnode *head, Status visit(btnode *));
Status SaveBiTrees(btm **index, FILE *DSS);
Status visit(btnode *);
Stack *InitStack(void)
{
    Stack *S = (Stack *)malloc(sizeof(Stack));
    if (!S)
        exit(OVERFLOW);
    S->data = (StackElem **)malloc(sizeof(StackElem *) * INIT_SIZE);
    if (!S->data)
        exit(OVERFLOW);
    S->top = -1;
    S->size = INIT_SIZE;
    return S;
}
Status DestroyStack(Stack *S)
{
    if (!S)
        return ERROR;
    free(S->data);
    free(S);
    return OK;
}
Status ClearStack(Stack *S)
{
    S->top = -1;
    return OK;
}
Status StackPush(Stack *S, StackElem *e)
{
    if (S->top + 1 > S->size)
    {
        StackElem **new = (StackElem **)realloc(S->data, sizeof(StackElem *) * (S->size + SIZE_INCREAMENT));
        if (!new)
            exit(OVERFLOW);
        S->data = new;
        S->size += SIZE_INCREAMENT;
    }
    S->data[++(S->top)] = e;
    return OK;
}
Status StackPop(Stack *S, StackElem **e)
{
    if (S->top < 0)
        return ERROR;
    *e = S->data[(S->top)--];
    return OK;
}
Queue *InitQueue(void)
{
    Queue *Q = (Queue *)malloc(sizeof(Queue));
    Q->head = NULL;
    Q->tail = NULL;
    Q->length = 0;
    return Q;
}
Status DestroyQueue(Queue *Q)
{
    if (!Q)
        return ERROR;
    if (!Q->length)
    {
        free(Q);
        return OK;
    }
    QNode *p1 = Q->head;
    QNode *p2 = Q->head->next;
    while (1)
    {
        free(p1);
        p1 = p2;
        if (!p1)
            break;
        p2 = p2->next;
    }
    free(Q);
    return OK;
}
Status QueuePush(Queue *Q, QueueElem *e)
{
    QNode *new = (QNode *)malloc(sizeof(QNode));
    if (!new)
        exit(OVERFLOW);
    new->data = e;
    new->next = NULL;
    if (!Q->length)
    {
        Q->head = new;
        Q->tail = new;
    }
    else
    {
        Q->tail->next = new;
        Q->tail = new;
    }
    Q->length++;
    return OK;
}
Status QueuePop(Queue *Q, QueueElem **e)
{
    if (Q->length <= 0)
        return ERROR;
    *e = Q->head->data;
    QNode *t = Q->head;
    Q->head = t->next;
    free(t);
    Q->length--;
    return OK;
}
int main()
{
    FILE *Fdef = fopen("./PreOrderDefWithNULL.txt", "r");
    btm *index[MAXLEN] = {NULL};
    //Necessary variables
    int mark = -1;
    int idx_pos = 0, LR = -1;
    unsigned int ident = 0;
    ElemType e;
    do
    {
        {
            printf("                Menu for Binary Tree On Linked List\n");
            printf("----------------------------------------------------------------------\n");
            printf("    1. CreateBiTree         6. LocateNode          11. PreOrderTrav\n");
            printf("    2. DestroyBiTree        7. Assign              12. InOrderTrav\n");
            printf("    3. ClearBiTree          8. GetSibling          13. PostOrderTrav\n");
            printf("    4. BiTreeEmpty          9. InsertNode          14. LevelOrderTrav\n");
            printf("    5. BiTreeDepth          10. DeleteNode         15. SaveBiTrees\n");
            printf("               16.ShowAllBiTree          0. Exit\n");
            printf("----------------------------------------------------------------\n");
            printf("Input a Number for your operation in range [0, 16], end with <Enter>:\n");
        } //Table printing
        {
            while (1)
            {
                scanf("%d", &mark);
                if (mark >= 0 && mark <= 16)
                    break;
                printf("Illegal input, please re-input a Number in range [0, 16], end with <Enter>:\n");
            }
            if (!mark)
                break;
            if (mark != 1)
            {
                if (!tree_mk)
                {
                    printf("WARNING: No Bi-Tree! Unable to do any manipulations! Create Bi-Trees FIRST!\n");
                    continue;
                }
                else if (mark < 15)
                {
                    printf("Please input the Number of Bi-Tree in range [1, %d], end with <Enter>\n", tree_mk);
                    while (1)
                    {
                        scanf("%d", &idx_pos);
                        if (idx_pos >= 1 && idx_pos <= tree_mk)
                            break;
                        printf("Illegal input, please re-input a Number in range [1, %d], end with <Enter>:\n", tree_mk);
                    }
                }
            }
        } //Value Judging
        switch (mark)
        {
        case 1:
            if (CreateBiTree(index, Fdef))
                printf("Created.\n");
            else
                printf("ERROR.\n");
            break;
        case 2:
            if (DestroyBiTree(index, idx_pos))
                printf("Destroyed.\n");
            else
                printf("ERROR.\n");
            break;
        case 3:
            if (ClearBiTree(index[idx_pos]))
                printf("Clear.\n");
            else
                printf("ERROR.\n");
            break;
        case 4:
            switch (BiTreeEmpty(index[idx_pos]))
            {
            case TRUE:
                printf("Empty.\n");
                break;
            case FALSE:
                printf("Not empty.\n");
                break;
            case INFEASIBLE:
                printf("Not a tree.\n");
                break;
            }
            break;
        case 5:
            printf("The depth of Bi-Tree ID:%u is '%d'\n", index[idx_pos], BiTreeDepth(index[idx_pos]->head));
            break;
        case 6:
            printf("Please input the ID: ");
            if (scanf("%u", &ident))
                if (LocateNode(index[idx_pos]->head, ident))
                    printf("Found.\n");
                else
                    printf("Not found.\n");
            else
                printf("Error.\n");
            break;
        case 7:
            printf("Please input the ID:");
            scanf("%u", &(e.ID));
            printf("Please input the name:");
            scanf("%s", e.name);
            printf("Please input the score:");
            scanf("%u", &(e.score));
            if (Assign(index[idx_pos]->head, e.ID, e))
                printf("Assigned.\n");
            else
                printf("Error.\n");
            break;
        case 8:
            printf("Please input the ID:");
            btnode *t = GetSibling(index[idx_pos]->head, ident);
            if (t)
                printf("The sibling ID is: %u", t->data->ID);
            break;
        case 9:
            printf("Please input the ID:");
            scanf("%u", &(e.ID));
            printf("Please input the name:");
            scanf("%s", e.name);
            printf("Please input the score:");
            scanf("%u", &(e.score));
            printf("Please input the position DIGIT, Left(0) or Right(1):");
            scanf("%d", &LR);
            if (InsertNode(index[idx_pos], e.ID, LR, e))
                printf("Done.\n");
            else
                printf("Error.\n");
            break;
        case 10:
            printf("Please input the ID:");
            scanf("%u", &ident);
            if (DeleteNode(index[idx_pos], ident))
                printf("Done.\n");
            else
                printf("Error.\n");
            break;
        case 11:
            PreOrderTraverse(index[idx_pos]->head, &visit);
            break;
        case 12:
            if (BiTreeEmpty(index[idx_pos]))
                InOrderTraverse(index[idx_pos]->head, &visit);
            else
                printf("<Empty>\n");
            break;
        case 13:
            if (BiTreeEmpty(index[idx_pos]))
                PostOrderTraverse(index[idx_pos]->head, &visit);
            else
                printf("<Empty>\n");
            break;
        case 14:
            LevelOrderTraverse(index[idx_pos]->head, &visit);
            break;
        case 15:
        {
            FILE *DSS = fopen("./BiTreeSave.txt", "w");
            setbuf(DSS, NULL);
            fflush(DSS);
            if (SaveBiTrees(index, DSS))
                printf("Saved.\n");
            else
                printf("Error.\n");
            break;
        }
        }
    } while (mark);
    return 0;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}
Status InitBiTree(btm **index)
{
    if (!(index[tree_mk + 1] = (btm *)malloc(sizeof(btm))))
        exit(OVERFLOW);
    //Manage node memory allocation
    index[tree_mk + 1]->head = NULL;
    index[tree_mk + 1]->count = 0;
    tree_mk++;
    return OK;
}
Status PreOrderCreate(btnode *head, int *count, FILE *def)
{
    char c;
    unsigned int d;
    while (1)
    {
        c = fgetc(def);
        if (c == '^') //Empty node detected, compatible with empty tree.
            return INFEASIBLE;
        if (c == ' ')
            continue;
        if (c == '\n')
            return OK;
        ungetc(c, def);
        fscanf(def, "%u", &d);
        break;
    }
    head->data->ID = d;
    if (!(head->lchild = (btnode *)malloc(sizeof(btnode))))
        exit(OVERFLOW);
    if (!(head->lchild->data = (ElemType *)malloc(sizeof(ElemType))))
        exit(OVERFLOW);
    (*count)++;
    //Left child memory allocation
    if (PreOrderCreate(head->lchild, count, def) == INFEASIBLE)
    {
        free(head->lchild->data);
        free(head->lchild);
        head->lchild = NULL;
        (*count)--;
    }

    if (!(head->rchild = (btnode *)malloc(sizeof(btnode))))
        exit(OVERFLOW);
    if (!(head->rchild->data = (ElemType *)malloc(sizeof(ElemType))))
        exit(OVERFLOW);
    (*count)++;
    //Right child memory allocation
    if (PreOrderCreate(head->rchild, count, def) == INFEASIBLE)
    {
        free(head->rchild->data);
        free(head->rchild);
        head->rchild = NULL;
        (*count)--;
    }
    return OK;
}
Status CreateBiTree(btm **index, FILE *def)
{
    char c;
    while (1)
    {
        c = fgetc(def);
        if (c == '\n')
            continue;
        if (c == EOF)
            return OK;
        ungetc(c, def);
        InitBiTree(index);
        if (!(index[tree_mk]->head = (btnode *)malloc(sizeof(btnode))))
            exit(OVERFLOW);
        if (!(index[tree_mk]->head->data = (ElemType *)malloc(sizeof(ElemType))))
            exit(OVERFLOW);
        //Head node memory allocation
        (index[tree_mk]->count)++;
        index[tree_mk]->head->lchild = NULL;
        index[tree_mk]->head->rchild = NULL;
        //Basic info & connection establishment
        if (PreOrderCreate(index[tree_mk]->head, &(index[tree_mk]->count), def) == INFEASIBLE)
        {
            free(index[tree_mk]->head);
            index[tree_mk]->head = NULL;
            index[tree_mk]->count = 0;
        }
    }
    return OK;
}
Status ClearBiTree(btm *T)
{
    if (T->count)
    {
        Queue *bitr = InitQueue();
        btnode *tp = NULL;
        QueuePush(bitr, T->head);
        while (T->count)
        {
            QueuePop(bitr, &tp);
            if (tp->lchild)
                QueuePush(bitr, tp->lchild);
            if (tp->rchild)
                QueuePush(bitr, tp->rchild);
            free(tp);
            T->count--;
        }
        T->head = NULL;
        DestroyQueue(bitr);
    }
    return OK;
}
Status DestroyBiTree(btm **index, int i)
{
    if (index && i <= tree_mk && i >= 1)
    {
        ClearBiTree(index[i]);
        free(index[i]);
        for (int j = i; j <= tree_mk; j++)
            index[j] = index[j + 1];
        tree_mk--;
        return OK;
    }
    return ERROR;
}
Status BiTreeEmpty(btm *T)
{
    if (!T) //Incompatible situation
        return INFEASIBLE;
    if (!T->head) //Definition of an empty tree
        return TRUE;
    return FALSE;
}
int BiTreeDepth(btnode *head)
{
    if (!head)
        return 0;
    return max(BiTreeDepth(head->lchild), BiTreeDepth(head->rchild)) + 1;
}
btnode *LocateNode(btnode *head, unsigned int e)
{
    if (head->data->ID == e)
        return head;
    btnode *tmp;
    if (head->lchild)
    {
        tmp = LocateNode(head->lchild, e);
        if (tmp)
            return tmp;
    }
    if (head->rchild)
    {
        tmp = LocateNode(head->rchild, e);
        if (tmp)
            return tmp;
    }
    return NULL;
}
Status Assign(btnode *head, unsigned int e, ElemType value)
{
    btnode *target = LocateNode(head, e);
    if (!target)
        return ERROR;
    *(target->data) = value;
    return OK;
}
btnode *GetSibling(btnode *head, unsigned int e)
{
    if (!head)
        return NULL;
    if (head->lchild && head->rchild)
    {
        if (head->lchild->data->ID == e)
            return head->rchild;
        if (head->rchild->data->ID == e)
            return head->lchild;
    }
    btnode *tmp;
    if (head->lchild)
        if (tmp = GetSibling(head->lchild, e))
            return tmp;
    if (head->rchild)
        if (tmp = GetSibling(head->rchild, e))
            return tmp;
    return NULL;
}
btnode *GetParent(btnode *head, unsigned int e)
{
    if (!head)
        return ERROR;
    btnode *tmp;
    if (head->lchild)
    {
        if (head->lchild->data->ID == e)
            return head;
        tmp = GetParent(head->lchild, e);
        if (tmp)
            return tmp;
    }
    if (head->rchild)
    {
        if (head->rchild->data->ID == e)
            return head;
        tmp = GetParent(head->rchild, e);
        if (tmp)
            return tmp;
    }
    return NULL;
}
int ConnectionJudge(btnode *parent, btnode *child)
{
    if (!parent || !child)
        return -1;
    if (parent->lchild == child)
        return 0;
    if (parent->rchild == child)
        return 1;
    return -1;
}
Status InsertNode(btm *T, unsigned int e, int LR, ElemType c)
{
    if (LR != 0 || LR != 1)
        return ERROR;
    btnode *target = LocateNode(T->head, e);
    if (!target)
        return ERROR;
    btnode *new = (btnode *)malloc(sizeof(btnode));
    if (!new)
        exit(OVERFLOW);
    new->data = &c;
    new->lchild = NULL;
    if (LR)
    {
        new->rchild = target->rchild;
        target->rchild = new;
    }
    else
    {
        new->rchild = target->lchild;
        target->lchild = new;
    }
    T->count++;
    return OK;
}
Status DeleteNode(btm *T, unsigned int e)
{
    if (!T)
        return ERROR;
    if (BiTreeEmpty(T))
        return ERROR;
    if (T->head->data->ID == e) //Special case
    {
        if (T->head->lchild && T->head->rchild)
        {
            btnode *p1 = T->head;
            btnode *p2 = T->head->lchild;
            T->head = T->head->lchild;
            while (p2->rchild)
                p2 = p2->rchild;
            p2->rchild = p1->rchild;
            free(p1);
        }
        else if (T->head->lchild && !(T->head->rchild))
        {
            btnode *p1 = T->head;
            T->head = T->head->lchild;
            free(p1);
        }
        else if (!(T->head->lchild) && T->head->rchild)
        {
            btnode *p1 = T->head;
            T->head = T->head->rchild;
            free(p1);
        }
        else
        {
            free(T->head);
            T->head = NULL;
        }
        T->count--;
        return OK;
    }
    btnode *target = LocateNode(T->head, e);
    if (target)
    {
        btnode *parent = GetParent(T->head, target->data->ID);
        int LR = ConnectionJudge(parent, target);
        if (LR == -1)
            return ERROR;
        if (target->lchild && target->rchild)
        {
            btnode *p = target->lchild;
            if (!LR)
                parent->lchild = target->lchild;
            else
                parent->rchild = target->lchild;
            while (p->rchild)
                p = p->rchild;
            p->rchild = target->rchild;
            free(target);
        }
        else if (target->lchild && !(target->rchild))
        {
            if (!LR)
                parent->lchild = target->lchild;
            else
                parent->rchild = target->lchild;
            free(target);
        }
        else if (target->rchild && !(target->lchild))
        {
            if (!LR)
                parent->lchild = target->rchild;
            else
                parent->rchild = target->rchild;
            free(target);
        }
        else
        {
            if (!LR)
                parent->lchild = NULL;
            else
                parent->rchild = NULL;
            free(target);
        }
        T->count--;
        return OK;
    }
}
Status PreOrderTraverse(btnode *head, Status visit(btnode *))
{
    if (!head)
        return ERROR;
    if ((!head->lchild) && !(head->rchild))
    {
        printf("<Empty>\n");
        return OK;
    }
    Stack *bitr = InitStack();
    StackPush(bitr, head);
    btnode *tp = NULL;
    while (bitr->top >= 0)
    {
        StackPop(bitr, &tp);
        visit(tp);
        if (tp)
        {
            StackPush(bitr, tp->rchild);
            StackPush(bitr, tp->lchild);
        }
    }
    printf("\n");
    DestroyStack(bitr);
    return OK;
}
Status InOrderTraverse(btnode *head, Status visit(btnode *))
{
    if (!head)
        return ERROR;
    InOrderTraverse(head->lchild, visit);
    visit(head);
    InOrderTraverse(head->rchild, visit);
    return OK;
}
Status PostOrderTraverse(btnode *head, Status visit(btnode *))
{
    if (!head)
        return ERROR;
    PostOrderTraverse(head->lchild, visit);
    PostOrderTraverse(head->rchild, visit);
    visit(head);
    return OK;
}
Status LevelOrderTraverse(btnode *head, Status visit(btnode *))
{
    if (!head)
        return ERROR;
    if ((!head->lchild) && !(head->rchild))
    {
        printf("<Empty>\n");
        return OK;
    }
    Queue *bitr = InitQueue();
    QueuePush(bitr, head);
    btnode *tp = NULL;
    while (bitr->length)
    {
        QueuePop(bitr, &tp);
        visit(tp);
        if (tp)
        {
            QueuePush(bitr, tp->lchild);
            QueuePush(bitr, tp->rchild);
        }
    }
    printf("\n");
    return OK;
}
Status SaveBiTrees(btm **index, FILE *DSS)
{
    if (!index || !tree_mk)
        return ERROR;
    Stack *bitr = InitStack();
    for (int i = 1; i <= tree_mk; i++)
    {
        if (!index[i])
            return ERROR;
        if (BiTreeEmpty(index[i]))
        {
            fprintf(DSS, "^\n");
            continue;
        }
        StackPush(bitr, index[i]->head);
        btnode *tp = NULL;
        while (bitr->top >= 0)
        {
            StackPop(bitr, &tp);
            if (!tp)
                fprintf(DSS, "^ ");
            else
                fprintf(DSS, "%u ", tp->data->ID);
            if (tp)
            {
                StackPush(bitr, tp->rchild);
                StackPush(bitr, tp->lchild);
            }
        }
        fprintf(DSS, "\n");
        ClearStack(bitr);
    }
    DestroyStack(bitr);
    return OK;
}
Status visit(btnode *node)
{
    if (!node)
        return ERROR;
    printf("%u ", node->data->ID);
    return OK;
}