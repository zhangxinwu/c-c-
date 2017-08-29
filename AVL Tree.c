#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define High(p) (p == NULL ? 0 : p->high)
#define true 1
#define false 0

struct node
{
    int value;
    int high;
    struct node *left;
    struct node *right;
} * root;

typedef struct node Node;
typedef int Value;

Node *LeftLeft(Node *p);
Node *LeftRight(Node *p);
Node *RightRight(Node *p);
Node *RightLeft(Node *p);

Node *InsertNode(Node *p, Value value);
Node *DeleteNode(Node *p, Value value);
Node *QueryNode(Node *p, Value value);
Node *QueryMaxNode(Node *p);
Node *QueryMinNode(Node *p);

void pB(Node *p);
void pM(Node *p);
void pA(Node *p);

void FreeAllNode(Node *p);
void welcome();

int main()
{
    int n, m;
    welcome();
    while (true)
    {
        scanf("%d", &n);
        if (n == 9)
            break;
        switch (n)
        {
        case 0:
            scanf("%d", &m);
            root = InsertNode(root, m);
            break;
        case 1:
            scanf("%d", &m);
            root = DeleteNode(root, m);
            break;
        case 2:
            printf("%d\n", QueryMaxNode(root)->value);
            break;
        case 3:
            printf("%d\n", QueryMinNode(root)->value);
            break;
        case 4:
            pB(root);
            puts("");
            break;
        case 5:
            pM(root);
            puts("");
            break;
        case 6:
            pA(root);
            puts("");
            break;
        case 7:
            scanf("%d", &m);
            if (QueryNode(root, m) == NULL)
                puts("No this number.");
            else
                puts("Has this number.");
            break;
        case 8:
            FreeAllNode(root);
            root = NULL;
            puts("Finshed to Free All Node!");
            break;
        default:
            break;
        }
    }
    return 0;
}

void welcome()
{
    puts("0:Insert a number.");
    puts("1:Delete a number.");
    puts("2:Get the max number.");
    puts("3:Get the min number.");
    puts("4:Print all B node.");
    puts("5:Print all M node.");
    puts("6:Print all A node.");
    puts("7:Judge has this number.");
    puts("8:Free all numbers.");
    puts("9:Exit this.");
}

Node *LeftLeft(Node *p)
{
    Node *f = p->left;
    Node *c = f->right;
    f->right = p;
    p->left = c;
    p->high = max(High(p->left), High(p->right)) + 1;
    f->high = max(High(f->left), High(f->right)) + 1;
    return f;
}

Node *LeftRight(Node *p)
{
    p->left = RightRight(p->left);
    return LeftLeft(p);
}

Node *RightRight(Node *p)
{
    Node *f = p->right;
    Node *c = f->left;
    f->left = p;
    p->right = c;
    p->high = max(High(p->left), High(p->right)) + 1;
    f->high = max(High(f->left), High(f->right)) + 1;
    return f;
}

Node *RightLeft(Node *p)
{
    p->right = LeftLeft(p->right);
    return RightRight(p);
}

Node *InsertNode(Node *p, Value value)
{
    if (p == NULL)
    {
        p = (Node *)malloc(sizeof(Node));
        p->value = value;
        return p;
    }
    if (value < p->value)
    {
        p->left = InsertNode(p->left, value);
        if (High(p->left) - High(p->right) == 2)
            if (value < p->left->value)
                return LeftLeft(p);
            else
                return LeftRight(p);
    }
    if (value > p->value)
    {
        p->right = InsertNode(p->right, value);
        if (High(p->right) - High(p->left) == 2)
            if (value < p->right->value)
                return RightLeft(p);
            else
                return RightRight(p);
    }
    p->high = max(High(p->left), High(p->right)) + 1;
    return p;
}

Node *DeleteNode(Node *p, Value value)
{
    if (p == NULL)
        return NULL;
    if (value < p->value)
        p->left = DeleteNode(p->left, value);
    else if (value > p->value)
        p->right = DeleteNode(p->right, value);
    else
    {
        Node *f = NULL;
        if (p->left != NULL)
        {
            f = QueryMaxNode(p->left);
            p->value = f->value;
            p->left = DeleteNode(p->left, f->value);
        }
        else if (p->right != NULL)
        {
            f = QueryMinNode(p->right);
            p->value = f->value;
            p->right = DeleteNode(p->right, f->value);
        }
        else
        {
            free(p);
            return NULL;
        }
    }
    if (High(p->left) - High(p->right) == 2)
    {
        Node *f = p->left;
        if (High(f->left) - High(f->right) == 1)
            p = LeftLeft(p);
        else
            p = LeftRight(p);
    }
    if (High(p->right) - High(p->left) == 2)
    {
        Node *f = p->right;
        if (High(f->left) - High(f->right) == 1)
            p = RightLeft(p);
        else
            p = RightRight(p);
    }
    p->high = max(High(p->left), High(p->right)) + 1;
    return p;
}

Node *QueryNode(Node *p, Value value)
{
    if (p == NULL)
        return NULL;
    if (value == p->value)
        return p;
    if (value < p->value)
        return QueryNode(p->left, value);
    if (value > p->value)
        return QueryNode(p->right, value);
}

Node *QueryMaxNode(Node *p)
{
    if (p == NULL)
        return NULL;
    if (p->right == NULL)
        return p;
    return QueryMaxNode(p->right);
}

Node *QueryMinNode(Node *p)
{
    if (p == NULL)
        return NULL;
    if (p->left == NULL)
        return p;
    return QueryMinNode(p->left);
}

void FreeAllNode(Node *p)
{
    if (p == NULL)
        return;
    FreeAllNode(p->left);
    FreeAllNode(p->right);
    free(p);
    return;
}

void pB(Node *p)
{
    if (p == NULL)
        return;
    printf("%d ", p->value);
    pB(p->left);
    pB(p->right);
}

void pM(Node *p)
{
    if (p == NULL)
        return;
    pB(p->left);
    printf("%d ", p->value);
    pB(p->right);
}

void pA(Node *p)
{
    if (p == NULL)
        return;
    pB(p->left);
    pB(p->right);
    printf("%d ", p->value);
}
