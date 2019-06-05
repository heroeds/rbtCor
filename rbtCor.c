#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 60

enum type {RED,BLACK};

typedef struct rb {
    char key[LENGTH];
    int flag;
    struct rb *left, *right, *parent;
    enum type color;
}node;



static void LeftRotate(node **T,node **x)
{
    node *y = (*x)->right;
    (*x)->right = y->left;

    if(y->left!=NULL)
        y->left->parent = *x;

    y->parent = (*x)->parent;

    if((*x)->parent == NULL)
        *T = y;

    else if(*x == (*x)->parent->left)
        (*x)->parent->left = y;

    else
        (*x)->parent->right = y;

    y->left = *x;

    (*x)->parent = y;
}

static void RightRotate(node **T,node **x)
{
    node *y = (*x)->left;
    (*x)->left = y->right;

    if(y->right!=NULL)
        y->right->parent = *x;

    y->parent = (*x)->parent;

    if((*x)->parent==NULL)
        *T = y;

    else if((*x)== (*x)->parent->left)
        (*x)->parent->left = y;

    else
        (*x)->parent->right = y;

    y->right = *x;
    (*x)->parent = y;
}

static void RB_insert_fixup(node **T,node **z)
{
    node *grandparent = NULL;
    node *parentpt = NULL;

    while(((*z)!=*T) && ((*z)->color!= BLACK) && ((*z)->parent->color == RED))
    {
        parentpt = (*z)->parent;
        grandparent = (*z)->parent->parent;

        if(parentpt == grandparent->left)
        {
            node *uncle = grandparent->right;

            if(uncle!=NULL && uncle->color == RED)
            {
                grandparent->color = RED;
                parentpt->color = BLACK;
                uncle->color = BLACK;
                *z = grandparent;
            }

            else
            {
                if((*z) == parentpt->right)
                {
                    LeftRotate(T,&parentpt);
                    (*z) = parentpt;
                    parentpt = (*z)->parent;
                }

                RightRotate(T,&grandparent);
                parentpt->color = BLACK;
                grandparent->color = RED;
                (*z) = parentpt;
            }
        }

        else
        {
            node *uncle = grandparent->left;

            if(uncle!=NULL && uncle->color == RED)
            {
                grandparent->color = RED;
                parentpt->color = BLACK;
                uncle->color = BLACK;
                (*z) = grandparent;
            }

            else
            {
                if((*z) == parentpt->left)
                {
                    RightRotate(T,&parentpt);
                    (*z) = parentpt;
                    parentpt = (*z)->parent;
                }

                LeftRotate(T,&grandparent);
                parentpt->color = BLACK;
                grandparent->color = RED;
                (*z) = parentpt;
            }
        }
    }
    (*T)->color = BLACK;
}

node *RB_insert(node *T, char k[], int f)
{
    node* z = (node*)malloc(sizeof(node));
    strcpy(z->key, k);
    z->flag = f;
    z->left = NULL;
    z->right = NULL;
    z->parent = NULL;
    z->color = RED;

    node *y = NULL;
    node *x = T;

    while(x!=NULL)
    {
        y = x;
        if(strcmp(z->key, x->key) < 0)
            x = x->left;

        else
            x = x->right;
    }
    z->parent = y;

    if(y==NULL)
        T = z;

    else if(strcmp(z->key, y->key) < 0)
        y->left = z;

    else
        y->right = z;

    RB_insert_fixup(&T,&z);

    return T;
}

node *Tree_minimum(node *n)
{
    while(n->left!=NULL)
        n = n->left;
    return n;
}

static void RB_delete_fixup(node **T, node **x)
{
    while((*x)!=*T && (*x)->color == BLACK)
    {
        if((*x)==(*x)->parent->left)
        {
            node* w = (*x)->parent->right;

            if(w->color==RED)
            {
                w->color = BLACK;
                (*x)->parent->color = BLACK;
                LeftRotate(T,&((*x)->parent));
                w = (*x)->parent->right;
            }

            if(w->left->color==BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                (*x) = (*x)->parent;
            }

            else
            {
                if(w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(T,&w);
                    w = (*x)->parent->right;
                }

                w->color = (*x)->parent->color;
                (*x)->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(T,&((*x)->parent));
                (*x) = *T;
            }
        }

        else
        {
            node* w = (*x)->parent->left;

            if(w->color==RED)
            {
                w->color = BLACK;
                (*x)->parent->color = BLACK;
                RightRotate(T,&((*x)->parent));
                w = (*x)->parent->left;
            }

            if(w->right->color==BLACK && w->left->color == BLACK)
            {
                w->color = RED;
                (*x) = (*x)->parent;
            }

            else
            {
                if(w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(T,&w);
                    w = (*x)->parent->left;
                }

                w->color = (*x)->parent->color;
                (*x)->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotate(T,&((*x)->parent));
                (*x) = *T;
            }
        }
    }
    (*x)->color = BLACK;
}

static void RB_transplat(node **T, node **u, node **v)
{
    if((*u)->parent == NULL)
        *T = *v;

    else if((*u)==(*u)->parent->left)
        (*u)->parent->left = *v;
    else
        (*u)->parent->right = *v;

    if((*v)!=NULL)
        (*v)->parent = (*u)->parent;
}

node *RB_delete(node *T, node *z)
{
    node *y = z;
    enum type yoc;
    yoc = z->color;

    node *x;

    if(z->left==NULL )
    {
        x = z->right;
        RB_transplat(&T,&z,&(z->right));
    }

    else if(z->right==NULL )
    {
        x = z->left;
        RB_transplat(&T,&z,&(z->left));
    }

    else
    {
        y = Tree_minimum(z->right);
        yoc = y->color;
        x = y->right;

        if(y->parent==z)
            x->parent = y;

        else
        {
            RB_transplat(&T,&y,&(y->right));
            y->right = z->right;
            y->right->parent = y;
        }

        RB_transplat(&T,&z,&y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if(yoc==BLACK)
        RB_delete_fixup(&T,&x);

    return T;
}

node *BST_search(node* root, char x[])
{
    if(root==NULL || !strcmp(root->key, x))
        return root;

    if(strcmp(root->key,x) > 0)
        return  BST_search(root->left,x);
    else
        return BST_search(root->right,x);
}

void preorder(node *root)
{
    if(root==NULL)
        return;

    printf("\n[%s %d]", root->key, root->flag);
    preorder(root->left);
    preorder(root->right);
}

void preorderFree(node *root)
{
    if(root==NULL)
        return;

    free(root);
    preorder(root->left);
    preorder(root->right);
}

int main()
{
    node *RBT = NULL;

    char str1[LENGTH] = "abcd";
    char str2[LENGTH] = "abcd";
    char str3[LENGTH] = "abce jg";
    RBT = RB_insert(RBT,str1, 1);
    RBT = RB_insert(RBT,str2, 0);
    RBT = RB_insert(RBT,str3, 0);
    /*RBT = RB_insert(RBT,"dd", 1);
    RBT = RB_insert(RBT,"ff", 0);
    RBT = RB_insert(RBT,"bb", 1);
    RBT = RB_insert(RBT,"ag", 0);
    RBT = RB_insert(RBT,"af", 0);*/

    printf("\nPreorder - ");
    preorder(RBT);

    node *x = BST_search(RBT,"abcd");

    RBT = RB_delete(RBT,x);

    printf("\nAfter deleting gg");

    printf("\nPreorder - ");
    preorder(RBT);

    return 0;
}
