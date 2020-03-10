#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//Preliminaries
typedef int Status;
#define TRUE 1
#define OK 1
#define FALSE 0
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAXLEN 10
#define INIT_SIZE 10
#define SIZE_INCREAMENT 5
#define MAX_VEX_NUM 20
static int init_mk = 0;
static int visited[MAX_VEX_NUM];
//Data Structure Design
typedef int InfoType; //To show the weight of an arc
typedef struct
{
    int key;
    int value;
} VertexType;
typedef struct ArcNode
{
    int adjvex;
    struct ArcNode *nextarc;
    InfoType *info;
} ArcNode;
typedef struct VNode
{
    VertexType *data;
    ArcNode *firstarc;
} VNode;
typedef struct
{
    VNode *vert[MAX_VEX_NUM];
    int vexnum, arcnum;
    int kind;
} ALGraph;

//Additional Data Structure Design
typedef VNode QueueElem;
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
Queue *InitQueue(void)
{
    Queue *Q = (Queue *)malloc(sizeof(Queue));
    if (!Q)
        exit(OVERFLOW);
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
Status EnQueue(Queue *Q, QueueElem *e)
{
    if (!Q)
        return ERROR;
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
Status DeQueue(Queue *Q, QueueElem **e)
{
    if (!Q)
        return ERROR;
    if (Q->length <= 0)
        return ERROR;
    *e = Q->head->data;
    QNode *t = Q->head;
    Q->head = t->next;
    free(t);
    Q->length--;
    return OK;
}
//Function Declaration
Status ReadGraph(ALGraph **index, FILE *DSR);
ALGraph *InitGraph(int kind);
Status CreateCraph(ALGraph *G, VNode **V, ArcNode **VR);
Status DestroyGraph(ALGraph **index, int pos);
VNode *LocateVex(ALGraph *G, int key);
Status PutVex(ALGraph *G, int key, int v);
int FirstAdjVex(ALGraph *G, int pos);
int NextAdjVex(ALGraph *G, int vpos, int wpos);
Status InsertVex(ALGraph *G, VNode *V);
Status DeleteVex(ALGraph *G, int key);
Status InsertArc(ALGraph *G, int vkey, int wkey);
Status DeleteArc(ALGraph *G, int vkey, int wkey);
Status DFSTraverse(ALGraph *G, Status visit(VNode *));
Status DFS(ALGraph *G, int i, Status visit(VNode *));
Status BFSTraverse(ALGraph *G, Status visit(VNode *));
Status SaveGraph(ALGraph **index, FILE *DSS);
Status visit(VNode *v);
int main()
{
    ALGraph *index[MAXLEN] = {NULL};
    int mark;
    int pos;
    int key, value, t1, t2, p;
    FILE *DSR = fopen("./GraphDef.txt", "r");
    FILE *DSS = fopen("./GraphSave.txt", "w+");
    setbuf(DSS, NULL);
    fflush(DSS);
    do
    {
        {
            printf("               Menu for Graph on Adjacent Linked List\n");
            printf("----------------------------------------------------------------------\n");
            printf("    1. CreateGraph       5. FirstAdjVex         9. InsertArc\n");
            printf("    2. DestroyGraph      6. NextAdjVex          10. DeleteArc\n");
            printf("    3. LocateVex         7. InsertVex           11. DFSTraverse\n");
            printf("    4. PutVex            8. DeleteVex           12. BFSTraverse\n");
            printf("                13. SaveGraph                0.exit\n");
            printf("----------------------------------------------------------------------\n");
        } //Table printing
        mark = -1;
        pos = -1;
        key = 0;
        value = 0;
        t1 = 0;
        t2 = 0;
        p = 0;
        //Necessary variables
        {
            printf("Input a Number for your operation in range [0, 13], end with <Enter>:\n");
            {
                while (1)
                {
                    scanf("%d", &mark);
                    if (mark >= 0 && mark <= 13)
                        break;
                    printf("Illegal input, please re-input a Number in range [0, 13], end with <Enter>:\n");
                }
                if (!mark)
                    break;
                if (mark != 1)
                {
                    if (!init_mk)
                    {
                        printf("WARNING: No Graph! Unable to do any manipulations! Read ALGraphs FIRST!\n");
                        continue;
                    }
                    else if (mark != 13)
                    {
                        printf("Please input the Number of ALGraph in range [0, %d], end with <Enter>\n", init_mk - 1);
                        while (1)
                        {
                            scanf("%d", &pos);
                            if (pos >= 0 && pos <= init_mk - 1)
                                break;
                            printf("Illegal input, please re-input a Number in range [0, %d], end with <Enter>:\n", init_mk - 1);
                        }
                    }
                }
            } //Value Judging
            switch (mark)
            {
            case 1:
                if (ReadGraph(index, DSR))
                    printf("Created.\n");
                else
                    printf("ERROR.\n");
                break;
            case 2:
                if (DestroyGraph(index, pos))
                    printf("Destroyed.\n");
                else
                    printf("ERROR.\n");
                break;
            case 3:
                printf("Please input the key of the node:");
                scanf("%d", &key);
                VNode *t = LocateVex(index[pos], key);
                if (t)
                    printf("Found.\n");
                else
                    printf("Not Found.\n");
                break;
            case 4:
                printf("Please input the key of the node:");
                scanf("%d", &key);
                printf("Please input the value:");
                scanf("%d", &value);
                if (PutVex(index[pos], key, value))
                    printf("Done.\n");
                else
                    printf("Error.\n");
                break;
            case 5:
                printf("Please input the position of the vex:");
                scanf("%d", &t1);
                p = FirstAdjVex(index[pos], t1);
                if (p == INFEASIBLE)
                    printf("Error.\n");
                else
                    printf("The first adjacent vex is at the location: %d\n", p);
                break;
            case 6:
                printf("Please input the positions of the two vexs:");
                scanf("%d%d", &t1, &t2);
                p = NextAdjVex(index[pos], t1, t2);
                if (p == INFEASIBLE)
                    printf("Error.\n");
                else
                    printf("The next adjacent vex is at the location: %d\n", p);
                break;
            case 7:
                printf("Please input the key of the new node:");
                scanf("%d", &key);
                printf("Please input the value:");
                scanf("%d", &value);
                VNode *V = (VNode *)malloc(sizeof(VNode));
                if (!V)
                    exit(OVERFLOW);
                V->data = (VertexType *)malloc(sizeof(VertexType));
                if (!(V->data))
                    exit(OVERFLOW);
                V->firstarc = NULL;
                V->data->key = key;
                V->data->value = value;
                if (index[pos] == NULL)
                {
                    printf("Empty graph, input the graph catagory for initialization.\n");
                    while (p != 0 || p != 1)
                    {
                        printf("Illegal input, input again!\n");
                        printf("(0)Simple Directed (1)Simple Undirected\n");
                        scanf("%d", &p);
                    }
                    index[pos] = InitGraph(p);
                }
                if (InsertVex(index[pos], V))
                    printf("Inserted.\n");
                else
                    printf("Error.\n");
                break;
            case 8:
                printf("Please input the key of the node to delete:");
                scanf("%d", &key);
                if (DeleteVex(index[pos], key))
                    printf("Deleted.\n");
                else
                    printf("Error.\n");
                break;
            case 9:
                printf("Please input the keys of the two vexs:");
                scanf("%d%d", &t1, &t2);
                if (InsertArc(index[pos], t1, t2))
                    printf("Inserted.\n");
                else
                    printf("Error.\n");
                break;
            case 10:
                printf("Please input the keys of the two vexs:");
                scanf("%d%d", &t1, &t2);
                if (DeleteArc(index[pos], t1, t2))
                    printf("Deleted.\n");
                else
                    printf("Error.\n");
                break;
            case 11:
                DFSTraverse(index[pos], &visit);
                break;
            case 12:
                BFSTraverse(index[pos], &visit);
                break;
            case 13:
                if (SaveGraph(index, DSS))
                    printf("Saved.\n");
                else
                    printf("Error.\n");
                break;
            }
        }
    } while (mark);
    return 0;
}
Status ReadGraph(ALGraph **index, FILE *DSR)
{
    char c;
    int d1, d2;
    int i = 0, j = 0;
    for (; init_mk < MAXLEN; init_mk++, i = 0, j = 0)
    {
        c = fgetc(DSR);
        if (c == EOF)
            break;
        if (c == '\n')
        {
            index[init_mk] = InitGraph(0);
            continue;
        }
        ungetc(c, DSR);
        fscanf(DSR, "%d", &d1);
        index[init_mk] = InitGraph(d1);
        VNode *V[MAX_VEX_NUM] = {NULL};
        ArcNode *VR[MAX_VEX_NUM * (MAX_VEX_NUM - 1)] = {NULL};
        while (1) //Create V and VR
        {
            c = fgetc(DSR);
            if (c == '\n')
                break;
            if (c == '(')
            {
                fscanf(DSR, "%d,%d", &d1, &d2);
                V[i] = (VNode *)malloc(sizeof(VNode));
                if (!(V[i]))
                    exit(OVERFLOW);
                V[i]->data = (VertexType *)malloc(sizeof(VertexType));
                if (!(V[i]->data))
                    exit(OVERFLOW);
                V[i]->data->key = d1;
                V[i]->data->value = d2;
                V[i]->firstarc = NULL;
                i++;
            }
            if (c == '<')
            {
                fscanf(DSR, "%d,%d", &d1, &d2);
                ArcNode *tp = (ArcNode *)malloc(sizeof(ArcNode));
                if (!tp)
                    exit(OVERFLOW);
                tp->adjvex = d2;
                tp->nextarc = NULL;
                tp->info = NULL;
                j++; //Initialization of an ArcNode
                if (!(VR[d1]))
                    VR[d1] = tp;
                else
                {
                    ArcNode *end = VR[d1];
                    while (end->nextarc)
                        end = end->nextarc;
                    end->nextarc = tp;
                } //Establish correct links
            }
        }
        index[init_mk]->vexnum = i;
        index[init_mk]->arcnum = j;
        CreateCraph(index[init_mk], V, VR);
    }
    return OK;
}
ALGraph *InitGraph(int kind)
{
    ALGraph *G = (ALGraph *)malloc(sizeof(ALGraph));
    if (!G)
        exit(OVERFLOW);
    for (int i = 0; i < MAX_VEX_NUM; i++)
        G->vert[i] = NULL;
    G->arcnum = 0;
    G->vexnum = 0;
    G->kind = kind;
    return G;
}
Status CreateCraph(ALGraph *G, VNode **V, ArcNode **VR)
{
    if (!G)
        return ERROR;
    for (int i = 0; i < G->vexnum; i++)
    {
        G->vert[i] = V[i];
        G->vert[i]->firstarc = VR[i];
    }
    return OK;
}
Status DestroyGraph(ALGraph **index, int pos)
{
    if (index && pos < init_mk && pos >= 0) //Validation
    {
        ALGraph *G = index[pos];
        for (int i = 0; i < G->vexnum; i++)
        {
            ArcNode *p1 = G->vert[i]->firstarc;
            ArcNode *p2 = NULL;
            while (p1)
            {
                p2 = p1->nextarc;
                free(p1);
                p1 = p2;
                free(G->vert[i]);
            }
            free(G);
        }
        for (int i = pos; i < init_mk; i++) //index order reconstruct
            index[i] = index[i + 1];
        init_mk--;
        return OK;
    }
    return ERROR;
}
VNode *LocateVex(ALGraph *G, int key)
{
    if (!G)
        return NULL;
    for (int i = 0; i < G->vexnum; i++)
    {
        if (key == G->vert[i]->data->key)
            return G->vert[i];
    }
    return NULL;
}
Status PutVex(ALGraph *G, int key, int v)
{
    if (!G)
        return ERROR;
    VNode *p = LocateVex(G, key);
    if (!p)
        return ERROR;
    p->data->value = v;
    return OK;
}
int FirstAdjVex(ALGraph *G, int pos)
{
    if (!G || pos >= G->vexnum)
        return INFEASIBLE;
    if (!G->vert[pos]->firstarc)
        return INFEASIBLE;
    return G->vert[pos]->firstarc->adjvex;
}
int NextAdjVex(ALGraph *G, int vpos, int wpos)
{
    if (!G || vpos >= G->vexnum || wpos >= G->vexnum)
        return INFEASIBLE;
    ArcNode *p = G->vert[vpos]->firstarc;
    while (p)
    {
        if (p->adjvex == wpos)
        {
            p = p->nextarc;
            break;
        }
        p = p->nextarc;
    }
    if (p == NULL)
        return INFEASIBLE;
    return p->adjvex;
}
Status InsertVex(ALGraph *G, VNode *V)
{
    if (!G || !V)
        return ERROR;
    if (G->vexnum == MAX_VEX_NUM)
        return OVERFLOW;
    for (int i = 0; i < G->vexnum; i++)
    {
        if (V->data->key == G->vert[i]->data->key)
            return ERROR;
    }
    G->vert[G->vexnum] = V;
    (G->vexnum)++;
    return OK;
}
Status DeleteVex(ALGraph *G, int key)
{
    if (!G)
        return ERROR;
    VNode *p = LocateVex(G, key);
    if (!p)
        return ERROR;
    int count = 0;
    while (p != G->vert[count])
        count++;
    for (int i = 0; i < G->vexnum; i++)
    {
        if (i == count)
            continue;
        ArcNode *t1 = G->vert[i]->firstarc;
        if (!t1)
            continue;
        if (t1->adjvex == count)
        {
            G->vert[i]->firstarc = t1->nextarc;
            free(t1);
            (G->arcnum)--;
        } //special case
        else
        {
            ArcNode *t2 = t1; //t2 points to the previous arc of t1
            t1 = t1->nextarc;
            while (t1)
            {
                if (t1->adjvex == count)
                {
                    t2->nextarc = t1->nextarc; //reconstruct links
                    free(t1);
                    (G->arcnum)--;
                    break; //found
                }
                t2 = t1;
                t1 = t1->nextarc;
            } //end without break, meaning not found
        }
    } //free arcs to vex
    ArcNode *p1 = p->firstarc;
    ArcNode *p2 = NULL;
    while (p1)
    {
        p2 = p1->nextarc;
        free(p1);
        (G->arcnum)--;
        p1 = p2;
    }
    free(p); //free arcs from vex
    for (int j = 0; j < G->vexnum; j++)
    {
        if (j == count)
            continue;
        ArcNode *t = G->vert[j]->firstarc;
        while (t)
        {
            if (t->adjvex > count)
                (t->adjvex)--;
            t = t->nextarc;
        }
    } //redirect before sorting
    for (int k = count; k < G->vexnum; k++)
        G->vert[k] = G->vert[k + 1]; //sorting
    (G->vexnum)--;
    return OK;
}
Status InsertArc(ALGraph *G, int vkey, int wkey)
{
    if (!G)
        return ERROR;
    VNode *v = LocateVex(G, vkey);
    VNode *w = LocateVex(G, wkey);
    if (!v || !w)
        return ERROR;
    int vcount = 0, wcount = 0;
    while (v != G->vert[vcount])
        vcount++;
    while (w != G->vert[wcount])
        wcount++;
    ArcNode *t = v->firstarc;
    while (t)
    {
        if (t->adjvex == wcount) //arc existing
            return ERROR;
        t = t->nextarc;
    }
    {
        if (v->firstarc == NULL)
        {
            v->firstarc = (ArcNode *)malloc(sizeof(ArcNode));
            if (!v->firstarc)
                exit(OVERFLOW);
            (G->arcnum)++;
            v->firstarc->adjvex = wcount;
            v->firstarc->nextarc = NULL;
        }
        else
        {
            t = v->firstarc;
            while (t->nextarc)
                t = t->nextarc;
            t->nextarc = (ArcNode *)malloc(sizeof(ArcNode));
            (G->arcnum)++;
            if (!(t->nextarc))
                exit(OVERFLOW);
            t->nextarc->adjvex = wcount;
            t->nextarc->nextarc = NULL;
        }
    }
    if (G->kind)
    {
        if (w->firstarc == NULL)
        {
            w->firstarc = (ArcNode *)malloc(sizeof(ArcNode));
            (G->arcnum)++;
            if (!w->firstarc)
                exit(OVERFLOW);
            w->firstarc->adjvex = wcount;
            w->firstarc->nextarc = NULL;
        }
        else
        {
            t = w->firstarc;
            while (t->nextarc)
                t = t->nextarc;
            t->nextarc = (ArcNode *)malloc(sizeof(ArcNode));
            (G->arcnum)++;
            if (t->nextarc)
                exit(OVERFLOW);
            t->nextarc->adjvex = wcount;
            t->nextarc->nextarc = NULL;
        }
    }
    return OK;
}
Status DeleteArc(ALGraph *G, int vkey, int wkey)
{
    if (!G)
        return ERROR;
    VNode *v = LocateVex(G, vkey);
    VNode *w = LocateVex(G, wkey);
    if (!v || !w)
        return ERROR;
    int vcount = 0, wcount = 0;
    while (v != G->vert[vcount])
        vcount++;
    while (w != G->vert[wcount])
        wcount++;
    if (G->kind)
    {
        ArcNode *t1 = v->firstarc;
        if (!t1)
            return ERROR;
        if (t1->adjvex == wcount)
        {
            v->firstarc = t1->nextarc;
            free(t1);
            (G->arcnum)--;
        } //special case
        else
        {
            ArcNode *t2 = t1; //t2 points to the previous arc of t1
            t1 = t1->nextarc;
            while (t1)
            {
                if (t1->adjvex == wcount)
                {
                    t2->nextarc = t1->nextarc; //reconstruct links
                    free(t1);
                    (G->arcnum)--;
                    break; //found
                }
                t2 = t1;
                t1 = t1->nextarc;
            } //end without break, meaning not found
        }
        t1 = w->firstarc;
        if (!t1)
            return ERROR;
        if (t1->adjvex == vcount)
        {
            w->firstarc = t1->nextarc;
            free(t1);
            (G->arcnum)--;
        } //special case
        else
        {
            ArcNode *t2 = t1; //t2 points to the previous arc of t1
            t1 = t1->nextarc;
            while (t1)
            {
                if (t1->adjvex == vcount)
                {
                    t2->nextarc = t1->nextarc; //reconstruct links
                    free(t1);
                    (G->arcnum)--;
                    break; //found
                }
                t2 = t1;
                t1 = t1->nextarc;
            } //end without break, meaning not found
        }
    }
    else
    {
        ArcNode *t1 = v->firstarc;
        if (!t1)
            return ERROR;
        if (t1->adjvex == wcount)
        {
            v->firstarc = t1->nextarc;
            free(t1);
            (G->arcnum)--;
        } //special case
        else
        {
            ArcNode *t2 = t1; //t2 points to the previous arc of t1
            t1 = t1->nextarc;
            while (t1)
            {
                if (t1->adjvex == wcount)
                {
                    t2->nextarc = t1->nextarc; //reconstruct links
                    free(t1);
                    (G->arcnum)--;
                    break; //found
                }
                t2 = t1;
                t1 = t1->nextarc;
            } //end without break, meaning not found
        }
    }
    return OK;
}
Status DFSTraverse(ALGraph *G, Status visit(VNode *))
{
    if (!G)
        return ERROR;
    if (!(G->vexnum))
    {
        printf("<Empty>\n");
        return OK;
    }
    for (int i = 0; i < G->vexnum; i++)
        visited[i] = 0;
    for (int i = 0; i < G->vexnum; i++)
        if (!visited[i])
            DFS(G, i, visit);
    printf("\n");
    return OK;
}
Status DFS(ALGraph *G, int i, Status visit(VNode *))
{
    visited[i] = 1;
    visit(G->vert[i]);
    for (int wpos = FirstAdjVex(G, i); wpos >= 0; wpos = NextAdjVex(G, i, wpos))
        if (!visited[wpos])
            DFS(G, wpos, visit);
        else
            break;
    return OK;
}
Status BFSTraverse(ALGraph *G, Status visit(VNode *))
{
    if (!G)
        return ERROR;
    if (!(G->vexnum))
    {
        printf("<Empty>\n");
        return OK;
    }
    for (int i = 0; i < G->vexnum; i++)
        visited[i] = 0;
    Queue *Q = InitQueue();
    for (int i = 0; i < G->vexnum; i++)
    {
        if (!visited[i])
        {
            visited[i] = 1;
            visit(G->vert[i]);
            EnQueue(Q, G->vert[i]);
            VNode *t = NULL;
            while (Q->length)
            {
                DeQueue(Q, &t);
                for (int wpos = FirstAdjVex(G, i); wpos >= 0; wpos = NextAdjVex(G, i, wpos))
                {
                    if (!visited[wpos])
                    {
                        visited[wpos] = 1;
                        visit(G->vert[wpos]);
                        EnQueue(Q, G->vert[wpos]);
                    }
                }
            }
        }
    }
    printf("\n");
    return OK;
}
Status SaveGraph(ALGraph **index, FILE *DSS)
{
    if (!index || !DSS)
        return ERROR;
    for (int i = 0; i < init_mk; i++)
    {
        if (index[i]->vexnum == 0)
        {
            fprintf(DSS, "\n");
            continue;
        }
        fprintf(DSS, "%d", index[i]->kind);
        for (int j = 0; j < index[i]->vexnum; j++)
            fprintf(DSS, "(%d,%d)", index[i]->vert[j]->data->key, index[i]->vert[j]->data->value);
        for (int j = 0; j < index[i]->vexnum; j++)
        {
            ArcNode *t = index[i]->vert[j]->firstarc;
            while (t)
            {
                fprintf(DSS, "<%d,%d>", j, t->adjvex);
                t = t->nextarc;
            }
        }
        fprintf(DSS, "\n");
    }
    return OK;
}
Status visit(VNode *v)
{
    if (!v)
        return ERROR;
    printf("%d ", v->data->key);
    return OK;
}