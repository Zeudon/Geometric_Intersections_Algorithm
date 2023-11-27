#include<bits/stdc++.h>
#define ll long long
using namespace std;

/* Nived Damodaran - 2018B5A71003H
Yash Amin - 2018B5A71044H
Harshit Bansal - 2018B5A70601H
Geet Sahitya Dulam - 2018B4A70797H*/


/*input
3
0 0 2 2
0 10 2 12
5 15 10 10
*/


int points=0,remaining=0,counter=0;

struct point
{
    double x;
    double y;
    int left;
    int right;
    int intersection;
    int ind1;
    int ind2;
};

 
struct segment
{
    double x1;
    double y1;
    double x2;
    double y2;
    point p1;
    point p2;
    double y_inter;
    //m=slope of segment
    //double m;
};
 

 
class StatusNode
{
    public:
    segment line;
    StatusNode *left;
    StatusNode *right;
 
    //intersection coordinates with sweep line
    //double x_intersect,y_intersect;
    int depth;
 
    //int counter;
    //int isLeft;
};

StatusNode* inserted;
StatusNode* deleted;

class Node
{
    public:
    point p;
    Node *left;
    Node *right;
    int depth;
    int counter;
    //int isLeft;
};

StatusNode *root=NULL;
Node *root1=NULL;

double finder(segment s,double x)
{
    double y_intercept;
    if(s.x1==s.x2)
    {
        y_intercept=s.y1;
    }
    else
    {
        double m=(s.y2-s.y1)/(s.x2-s.x1);
        double c=s.y1-(m*s.x1);
        y_intercept=(m*x)+c;
    }
    return y_intercept;
}

int depth(Node *t)
{
    if(t==NULL)
    {
        return 0;
    }
    else
    {
        return t->depth;
    }
}
 
Node* create(point p)
{
    Node* temp=new Node();
    temp->p=p;
    temp->depth=1;
    temp->counter=1;
    temp->left=NULL;
    temp->right=NULL;
    points+=1;
    return temp;
}
 
Node* LeftRotate(Node *t)
{
    Node *temp1=t->right;
    Node *temp2=t->right->left;
 
    temp1->left=t;
    t->right=temp2;
 
    t->depth = max(1+depth(t->right),1+depth(t->left));
    temp1->depth = max(depth(temp1->left)+1,depth(temp1->right)+1);
    return temp1;
}
 
Node* RotateRight(Node *t)
{
    Node *temp1= t->left;
    Node *temp2= t->left->right;
 
    temp1->right = t;
    t->left = temp2;
 
    t->depth = max(1+depth(t->right),1+depth(t->left));
    temp1->depth = max(depth(temp1->left)+1,depth(temp1->right)+1);
    return temp1;
}
 
int check( Node *t)
{
    if(t==NULL)
    {
        return 0;
    }
    else
    {
        return depth(t->left)-depth(t->right);
    }
}
 
Node* insert(Node *t,point p)
{
    if(t==NULL)
    {
        return create(p);
    }
    if(p.x<t->p.x)
    {
        t->left = insert(t->left,p);
    }
    else if(p.x==t->p.x)
    {
        if(p.y==t->p.y)
        {
            //t->counter++;
            if(t->p.intersection==0)
            {
                t->p.intersection=1;
                t->p.ind2=p.ind1;
                t->p.left=(t->p.left || p.left);
                t->p.right=(t->p.right || p.right);
            }
        }
        else
        {
            if(p.y>t->p.y)
            {
                t->left = insert(t->left,p);
            }
            else
            {
                t->right = insert(t->right,p);
            }
        }
        return t;
    }
    else
    {
        t->right = insert(t->right,p);
    }
 
    t->depth = max(depth(t->left),depth(t->right))+1;
 
    int balanced = check(t);
    //cout<<value<<" "<<balanced<<"\n";
    if(balanced>1 && p.x<t->left->p.x)
    {
        return RotateRight(t);
    }
 
    if(balanced<-1 && p.x>t->right->p.x)
    {
        return LeftRotate(t);
    }
 
    if(balanced>1 && p.x>t->left->p.x)
    {
        t->left=LeftRotate(t->left);
        return RotateRight(t);
    }
 
    if(balanced<-1 && p.x<t->right->p.x)
    {
        t->right=RotateRight(t->right);
        return LeftRotate(t);
    }
    return t;
}
 
Node* minNode(Node* t)
{
    Node* temp=t;
    while(temp->left!=NULL)
    {
        temp=temp->left;
    }
    return temp;
}
 
Node* maxNode(Node* t)
{
    Node* temp=t;
    while(temp->right!=NULL)
    {
        temp=temp->right;
    }
    return temp;
}
 
Node* deleteNode(Node* root,point p)
{
    if(root==NULL)
    {
        return root;
    }
    if(p.x<root->p.x)
    {
        root->left=deleteNode(root->left,p);
    }
    else if(p.x>root->p.x)
    {
        root->right=deleteNode(root->right,p);
    }
    else
    {
        if(p.y==root->p.y)
        {
            //root->counter--;
            if(root->left==NULL || root->right==NULL)
            {
                Node* temp=NULL;
                if(root->left!=NULL)
                {
                    temp=root->left;
                }
                if(root->right!=NULL)
                {
                    temp=root->right;
                }
                if(temp==NULL)
                {
                    temp=root;
                    root=NULL;
                }
                else
                {
                    *root=*temp;
                }
                free(temp);
            }
            else
            {
                Node *temp=minNode(root->right);
                root->p=temp->p;
                root->counter=temp->counter;
                root->right=deleteNode(root->right,temp->p);
            }
        }
        else
        {
            if(p.y>root->p.y)
            {
                root->left=deleteNode(root->left,p);
            }
            else
            {
                root->right=deleteNode(root->right,p);
            }
        }
    }
    if(root==NULL)
    {
        return root;
    }
    root->depth=1+max(depth(root->left),depth(root->right));
    int balanced=check(root);
 
    if(balanced>1 && check(root->left)>=0)
    {
        return RotateRight(root);
    }
    if(balanced>1 && check(root->left)<0)
    {
        root->left=LeftRotate(root->left);
        return RotateRight(root);   
    }
    if(balanced<-1 && check(root->right)<=0)
    {
        return LeftRotate(root);
    }
    if(balanced<-1 && check(root->right)>0)
    {
        root->right=RotateRight(root->right);
        return LeftRotate(root);
    }
    return root;
}
 
Node* successor(Node* root,Node* t)
{
    if(root==NULL)
    {
        return root;
    }
    if(t->right!=NULL)
    {
        return minNode(t->right);
    }
    Node* temp=NULL;
    while(root!=NULL)
    {
        if(t->p.x<root->p.x)
        {
            temp=root;
            root=root->left;
        }
        else if(t->p.x>root->p.x)
        {
            root=root->right;
        }
        else
        {
            break;
        }
    }
    return temp;
}
 
Node* predecessor(Node* root, Node* t,Node * pred)
{
    if(root==NULL)
    {
        return pred;
    }
    if(root->p.x==t->p.x)
    {
        if(root->left!=NULL)
        {
            return maxNode(root->left);
        }
    }
    else if(t->p.x<root->p.x)
    {
        return predecessor(root->left,t,pred);
    }
    else
    {
        pred=root;
        return predecessor(root->right,t,pred);
    }
    return pred;
}
 
int depth1(StatusNode *t)
{
    if(t==NULL)
    {
        return 0;
    }
    else
    {
        return t->depth;
    }
}


void inter(segment s1, segment s2)
{
    point p;
    if(s1.x1==s1.x2 && s2.x1==s2.x2)
    {
        if(s1.x1==s2.x1)
        {
            if(s1.y2==s2.y1)
            {
                p.x=s1.x1;
                p.y=s1.y2;
                p.intersection=1;
                p.ind1=s1.p1.ind1;
                p.ind2=s2.p1.ind1;
                root1=insert(root1,p);
            }
        }
    }
    else if(s1.x1==s1.x2)
    {
        double m=(s2.y2-s2.y1)/(s2.x2-s2.x1);
        double c=(s2.y1)-(m*s2.x1);
        double y=(m*s1.x1)+c;
        double mi1,mi2,ma1,ma2;
        if(s1.y1<s1.y2)
        {
            mi1=s1.y1;
            ma1=s1.y2;
        }
        else
        {
            mi1=s1.y2;
            ma1=s1.y1;
        }
        if(s2.y1<s2.y2)
        {
            mi2=s2.y1;
            ma2=s2.y2;
        }
        else
        {
            mi2=s2.y2;
            ma2=s2.y1;
        }
        if((y<=ma1 && y>=mi1) && (y<=ma2 && y>=mi2))
        {
            p.x=s1.x1;
            p.y=y;
            p.intersection=1;
            p.ind1=s1.p1.ind1;
            p.ind2=s2.p1.ind1;
            root1=insert(root1,p);
        }
    }
    else if(s2.x1==s2.x2)   
    {
        double m=(s1.y2-s1.y1)/(s1.x2-s1.x1);
        double c=(s1.y1)-(m*s1.x1);
        double y=(m*s2.x1)+c;
        double mi1,mi2,ma1,ma2;
        if(s1.y1<s1.y2)
        {
            mi1=s1.y1;
            ma1=s1.y2;
        }
        else
        {
            mi1=s1.y2;
            ma1=s1.y1;
        }
        if(s2.y1<s2.y2)
        {
            mi2=s2.y1;
            ma2=s2.y2;
        }
        else
        {
            mi2=s2.y2;
            ma2=s2.y1;
        }
        if((y<=ma1 && y>=mi1) && (y<=ma2 && y>=mi2))
        {
            p.x=s2.x1;
            p.y=y;
            p.intersection=1;
            p.ind1=s1.p1.ind1;
            p.ind2=s2.p1.ind1;
            root1=insert(root1,p);
        }
    }
    else
    {
        double m1=(s1.y2-s1.y1)/(s1.x2-s1.x1);
        double m2=(s2.y2-s2.y1)/(s2.x2-s2.x1);
        double c1=s1.y1-(m1*s1.x1);
        double c2=s2.y1-(m2*s2.x1);
        cout<<m1<<" inter "<<m2<<endl;
        if(m1!=m2)
        {
            double x=(c2-c1)/(m1-m2);
            double y=(m1*x)+c1;
            double mi1,mi2,mi3,mi4,ma1,ma2,ma3,ma4;
            if(s1.y1<s1.y2)
            {
                mi1=s1.y1;
                ma1=s1.y2;
            }
            else
            {
                mi1=s1.y2;
                ma1=s1.y1;
            }
            if(s2.y1<s2.y2)
            {
                mi2=s2.y1;
                ma2=s2.y2;
            }
            else
            {
                mi2=s2.y2;
                ma2=s2.y1;
            }
            if(s1.x1<s1.x2)
            {
                mi3=s1.x1;
                ma3=s1.x2;
            }
            else
            {
                mi3=s1.x2;
                ma3=s1.x1;
            }
            if(s2.x1<s2.x2)
            {
                mi4=s2.x1;
                ma4=s2.x2;
            }
            else
            {
                mi4=s2.x2;
                ma4=s2.x1;
            }
            if((y<=ma1 && y>=mi1) && (y<=ma2 && y>=mi2) && (x<=ma3 && x>=mi3) && (x<=ma4 && x>=mi4))
            {
                p.x=x;
                p.y=y;
                p.intersection=1;
                p.ind1=s1.p1.ind1;
                p.ind2=s2.p1.ind1;
                root1=insert(root1,p);
            }
        }
    }
    return;  
}

 
StatusNode* create1(segment ls,double x,double y)
{
    //cout<<"YES1"<<endl;
    StatusNode* temp=new StatusNode();
    temp->line=ls;
    temp->depth=1;
    /*temp->x_intersect=x;
    temp->y_intersect=y;
    temp->line.x_inter=x;
    temp->line.y_inter=y;
    temp->counter=1;*/
    temp->left=NULL;
    temp->right=NULL;
    return temp;
}
 
StatusNode* LeftRotate1(StatusNode *t)
{
    StatusNode *temp1=t->right;
    StatusNode *temp2=t->right->left;
 
    temp1->left=t;
    t->right=temp2;
 
    t->depth = max(1+depth1(t->right),1+depth1(t->left));
    temp1->depth = max(depth1(temp1->left)+1,depth1(temp1->right)+1);
    return temp1;
}
 
StatusNode* RotateRight1(StatusNode *t)
{
    StatusNode *temp1= t->left;
    StatusNode *temp2= t->left->right;
 
    temp1->right = t;
    t->left = temp2;
 
    t->depth = max(1+depth1(t->right),1+depth1(t->left));
    temp1->depth = max(depth1(temp1->left)+1,depth1(temp1->right)+1);
    return temp1;
}
 
int check1(StatusNode *t)
{
    if(t==NULL)
    {
        return 0;
    }
    else
    {
        return depth1(t->left)-depth1(t->right);
    }
}
 
StatusNode* insert1(StatusNode *t,segment ls,double x,double y)
{
    if(t==NULL)
    {
        //cout<<"YES1"<<endl;
        inserted=create1(ls,x,y);
        return inserted;
    }
    if(t->line.x1==t->line.x2)
    {
        t->line.y_inter=t->line.y1;
        if(ls.x1==ls.x2)
        {
            cout<<"YES1"<<endl;
            t->left=insert1(t->left,ls,x,y);
        }
        else
        {
            if(y>=t->line.y1)
            {
                t->right=insert1(t->right,ls,x,y);
            }
            else
            {
                t->left=insert1(t->left,ls,x,y);
            }
        }
    }
    else
    {
        double y_intersecter=finder(t->line,x);
        t->line.y_inter=y_intersecter;
        if(y<y_intersecter)
        {
            t->left = insert1(t->left,ls,x,y);
        }
        //If the y coordinates of intersection are same, then the segment which has lesser x coord of the segment is towards the left
        else if(y==y_intersecter)
        {
            if(ls.x1==ls.x2)
            {
                //cout<<"YES1"<<endl;
                t->left=insert1(t->left,ls,x,y);
            }
            else
            {
                double m1=(ls.y1-ls.y2)/(ls.x1-ls.x2);
                double m2=(t->line.y1-t->line.y2)/(t->line.x1-t->line.x2);
                int right=0;
                if(t->line.y2==y && t->line.x2==x)
                {
                    right=1;
                }
                if(right==0)
                {
                    if(m1>m2)
                    {
                        t->right=insert1(t->right,ls,x,y);
                    }
                    else
                    {
                        t->left=insert1(t->left,ls,x,y);
                    }
                }
                else
                {
                    if(m1<m2)
                    {
                        t->right=insert1(t->right,ls,x,y);
                    }
                    else
                    {
                        t->left=insert1(t->left,ls,x,y);
                    }
                }
            }
            return t;
        }
        else
        {
            if(ls.x1==ls.x2)
            {
                if(t->line.x2==x && t->line.y2==y)
                {
                    t->left=insert1(t->left,ls,x,y);
                }
                else
                {
                    t->right=insert1(t->right,ls,x,y);
                }
            }
            else
            {
                t->right = insert1(t->right,ls,x,y);
            }
        }
    }

    //cout<<"YES1"<<endl;
    t->depth = max(depth1(t->left),depth1(t->right))+1;
    //cout<<"YES2"<<endl;
 
    int balanced = check1(t);
    //cout<<value<<" "<<balanced<<"\n";
    double y1,y2;
    //cout<<"YES2"<<endl;
    if(balanced>1)
    {
        if(t->left->line.x1==t->left->line.x2)
        {
            y1=t->left->line.y1;
        }
        else
        {
            y1=finder(t->left->line,x);
        }
        if(y<y1)
        {
            return RotateRight1(t);
        }
        else if(y>y1)
        {
            t->left=LeftRotate1(t->left);
            return RotateRight1(t);
        }
    }
 
    if(balanced<-1)
    {
        if(t->right->line.x1==t->right->line.x2)
        {
            //cout<<"YES2"<<endl;
            y2=t->left->line.y2;
        }
        else
        {
            if(t->right!=NULL)
            {
                y2=finder(t->right->line,x);
            }
        }
        if(y>y2)
        {
            return LeftRotate1(t);
        }
        else if(y<y2)
        {
            t->right=RotateRight1(t->right);
            return LeftRotate1(t);
        }
    }
    return t;
}
 
//to update x intersection and y intersection points of the line segments as the sweep lines are updated
/*void update1(StatusNode* root,double x){
    if(root){
        update1(root->left,x);
        root->x_intersect=x;
        //y=mx+c to find y intersection of all segments with sweep line
        root->y_intersect= (x-root->line.x1)*(root->line.m) + root->line.y1;
        root->line.y_inter=root->y_intersect;
        update1(root->right,x);
    }
    else
        return;
}*/

StatusNode* minNode1(StatusNode* t)
{
    StatusNode* temp=t;
    while(temp->left!=NULL)
    {
        temp=temp->left;
    }
    return temp;
}
 
StatusNode* maxNode1(StatusNode* t)
{
    StatusNode* temp=t;
    while(temp->right!=NULL)
    {
        temp=temp->right;
    }
    return temp;
}


StatusNode* successor1(StatusNode* root,StatusNode* t)
{
    if(root==NULL)
    {
        return root;
    }
    if(t->right!=NULL)
    {
        return minNode1(t->right);
    }
    StatusNode* temp=NULL;
    while(root!=NULL)
    {
        if(t->line.y_inter<root->line.y_inter)
        {
            temp=root;
            root=root->left;
        }
        else if(t->line.y_inter>root->line.y_inter)
        {
            root=root->right;
        }
        else
        {
            break;
        }
    }
    return temp;
}
 
StatusNode* predecessor1(StatusNode* root, StatusNode* t,StatusNode * pred)
{
    if(root==NULL)
    {
        return pred;
    }
    if(root->line.y_inter==t->line.y_inter)
    {
        if(root->left!=NULL)
        {
            return maxNode1(root->left);
        }
    }
    else if(t->line.y_inter<root->line.y_inter)
    {
        return predecessor1(root->left,t,pred);
    }
    else
    {
        pred=root;
        return predecessor1(root->right,t,pred);
    }
    return pred;
}

void inOrder1(StatusNode *root)
{
    if(root!=NULL)
    {
        inOrder1(root->left);
        cout<<"Inorder1 "<<root->line.x1<<" "<<root->line.y1<<" "<<root->line.x2<<" "<<root->line.y2<<"\n";
        inOrder1(root->right);
    }
}
 
void inOrder(Node *root)
{
    if(root!=NULL)
    {
        inOrder(root->left);
        cout<<"Inorder "<<root->p.x<<" "<<root->p.y<<" "<<root->counter<<"\n";
        inOrder(root->right);
    }
}


StatusNode* deleteStatusNode1(StatusNode* root,segment ls, double x, double y)
{
    if(root==NULL)
    {
        return root;
    }
    if(root->line.x1==root->line.x2)
    {
        if(ls.x1==root->line.x1 && ls.y1==root->line.y1 && ls.x2==root->line.x2 && ls.y2==root->line.y2)
        {
            if(root->left==NULL || root->right==NULL)
            {
                StatusNode* temp=NULL;
                if(root->left!=NULL)
                {
                    temp=root->left;
                }
                if(root->right!=NULL)
                {
                    temp=root->right;
                }
                if(temp==NULL)
                {
                    //cout<<"YES1"<<endl;
                    temp=root;
                    root=NULL;
                }
                else
                {
                    *root=*temp;
                }
                deleted=temp;
                StatusNode* pred=predecessor1(root,deleted,NULL);
                StatusNode* succ=successor1(root,deleted);
                if(pred!=NULL && succ!=NULL)
                {
                    inter(pred->line,succ->line);
                }
                //cout<<"YES1 "<<temp<<endl;
                free(temp);
            }
            else
            {
                StatusNode *temp=minNode1(root->right);
                root->line=temp->line;
                //root->counter=temp->counter;
                //root->x_intersect=x;
                //root->y_intersect=y;
                root->right=deleteStatusNode1(root->right,temp->line,x,y);
            }
        }
        if(root!=NULL)
        {
            if(y>=root->line.y2)
            {
                root->right=deleteStatusNode1(root->right,ls,x,y);
            }
            else
            {
                root->left=deleteStatusNode1(root->left,ls,x,y);
            }
        }
        else
        {
            return root;
        }
    }
    else
    {
        double y_intersecter=finder(root->line,x);
        if(y<y_intersecter)
        {
            root->left=deleteStatusNode1(root->left,ls,x,y);
        }
        else if(y>y_intersecter)
        {
            root->right=deleteStatusNode1(root->right,ls,x,y);
        }
        else
        {
            if(ls.x1==root->line.x1 && ls.y1==root->line.y1 && ls.x2==root->line.x2 && ls.y2==root->line.y2)
            {
                if(root->left==NULL || root->right==NULL)
                {
                    StatusNode* temp=NULL;
                    if(root->left!=NULL)
                    {
                        temp=root->left;
                    }
                    if(root->right!=NULL)
                    {
                        temp=root->right;
                    }
                    if(temp==NULL)
                    {
                        temp=root;
                        root=NULL;
                    }
                    else
                    {
                        *root=*temp;
                    }
                    deleted=temp;
                    StatusNode* pred=predecessor1(root,deleted,NULL);
                    StatusNode* succ=successor1(root,deleted);
                    if(pred!=NULL && succ!=NULL)
                    {
                        inter(pred->line,succ->line);
                    }
                    free(temp);
                }
                else
                {
                    StatusNode *temp=minNode1(root->right);
                    root->line=temp->line;
                    //root->counter=temp->counter;
                    //root->x_intersect=ls.x_inter;
                    //root->y_intersect=ls.y_inter;
                    root->right=deleteStatusNode1(root->right,temp->line,x,y);
                }
            }
            else
            {
                if(ls.x1==ls.x2)
                {
                    if(root->line.x2==x && root->line.y2==y)
                    {
                        root->left=deleteStatusNode1(root->left,ls,x,y);
                    }
                    else
                    {
                        root->right=deleteStatusNode1(root->right,ls,x,y);
                    }
                }
                else
                {
                    double m1=(root->line.y1-root->line.y2)/(root->line.x1-root->line.x2);
                    double m2=(ls.y1-ls.y2)/(ls.x1-ls.x2);
                    if((m1>0 && m2>0 && m1<m2) || (m1==0 && m2>0) || (m1<0 && m2>=0))
                    {
                        root->left=deleteStatusNode1(root->left,ls,x,y);
                    }
                    else
                    {
                        root->right=deleteStatusNode1(root->right,ls,x,y);
                    }
                }
            }
        }
    }
    if(root==NULL)
    {
        return root;
    }
    root->depth=1+max(depth1(root->left),depth1(root->right));
    int balanced=check1(root);
 
    if(balanced>1 && check1(root->left)>=0)
    {
        return RotateRight1(root);
    }
    if(balanced>1 && check1(root->left)<0)
    {
        root->left=LeftRotate1(root->left);
        return RotateRight1(root);   
    }
    if(balanced<-1 && check1(root->right)<=0)
    {
        return LeftRotate1(root);
    }
    if(balanced<-1 && check1(root->right)>0)
    {
        root->right=RotateRight1(root->right);
        return LeftRotate1(root);
    }
    return root;
}




int main()
{
    ofstream output_file("output_file.txt");
    cout<<"GG"<<endl;
    int n,i;
    cout<<"Enter no of line segments:-";
    cin>>n;
    output_file<<n<<endl;
    int a[n];
    segment line_list[n];
    for(i=0;i<n;i++){
        cin>>line_list[i].x1>>line_list[i].y1>>line_list[i].x2>>line_list[i].y2;
        output_file<<line_list[i].x1<<" "<<line_list[i].y1<<" "<<line_list[i].x2<<" "<<line_list[i].y2;
        if(line_list[i].x1>line_list[i].x2)
        {
            double temp=line_list[i].x1;
            double temp2=line_list[i].y1;
            line_list[i].x1=line_list[i].x2;
            line_list[i].y1=line_list[i].y2;
            line_list[i].x2=temp;
            line_list[i].y2=temp2;
        }
        else if(line_list[i].x1==line_list[i].x2)
        {
            if(line_list[i].y1<line_list[i].y2)
            {
                double temp=line_list[i].y1;
                line_list[i].y1=line_list[i].y2;
                line_list[i].y2=temp;
            }
        }
        //root=insert1(root,line_list[i],i*0.2,i*0.3);
        point p1,p2;
        p1.x=line_list[i].x1;
        p1.y=line_list[i].y1;
        p1.left=1;
        p1.right=0;
        p1.intersection=0;
        p1.ind1=i;
        line_list[i].p1=p1;
        p2.x=line_list[i].x2;
        p2.y=line_list[i].y2;
        p2.left=0;
        p2.right=1;
        p2.intersection=0;
        p2.ind1=i;
        line_list[i].p2=p2;
        root1=insert(root1,p1);
        root1=insert(root1,p2);
    }
    remaining=points;
    cout<<root1->p.x<<" "<<root1->p.y<<endl;
    while(remaining>0)
    {
        remaining-=1;
        Node* temp=minNode(root1);
        cout<<temp->p.x<<" "<<temp->p.y<<" "<<remaining<<endl;
        //cout<<temp->p.left<<" "<<temp->p.right<<" "<<temp->p.intersection<<endl;
        if(temp->p.left==1)
        {
            if(temp->p.intersection==1)
            {
                if(line_list[temp->p.ind1].x1==temp->p.x && line_list[temp->p.ind1].y1==temp->p.y)
                {
                    line_list[temp->p.ind1].y_inter=temp->p.y;
                    root=insert1(root,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                    StatusNode* pred=predecessor1(root,inserted,NULL);
                    StatusNode* succ=successor1(root,inserted);
                    if(pred!=NULL)
                    {
                        inter(pred->line,inserted->line);
                    }
                    if(succ!=NULL)
                    {
                        inter(succ->line,inserted->line);
                    }
                }
                if(line_list[temp->p.ind2].x1==temp->p.x && line_list[temp->p.ind2].y1==temp->p.y)
                {
                    line_list[temp->p.ind2].y_inter=temp->p.y;
                    //cout<<"YES1"<<endl;
                    inOrder1(root);
                    root=insert1(root,line_list[temp->p.ind2],temp->p.x,temp->p.y);
                    StatusNode* pred=predecessor1(root,inserted,NULL);
                    StatusNode* succ=successor1(root,inserted);
                    if(pred!=NULL)
                    {
                        inter(pred->line,inserted->line);
                    }
                    if(succ!=NULL)
                    {
                        inter(succ->line,inserted->line);
                    }
                }
            }
            else
            {
                line_list[temp->p.ind1].y_inter=temp->p.y;
                root=insert1(root,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                StatusNode* pred=predecessor1(root,inserted,NULL);
                StatusNode* succ=successor1(root,inserted);
                if(pred!=NULL)
                {
                    cout<<pred->line.x1<<" "<<pred->line.y1<<endl;
                    inter(pred->line,inserted->line);
                }
                if(succ!=NULL)
                {
                    inter(succ->line,inserted->line);
                }
            }
        }
        if(temp->p.right==1)
        {
            //cout<<"YES1"<<endl;
            if(temp->p.intersection==1)
            {
                if(line_list[temp->p.ind1].x2==temp->p.x && line_list[temp->p.ind1].y2==temp->p.y)
                {
                    //cout<<"YES1"<<endl;
                    root=deleteStatusNode1(root,line_list[temp->p.ind1],temp->p.x,temp->p.y);

                }
                if(line_list[temp->p.ind2].x2==temp->p.x && line_list[temp->p.ind2].y2==temp->p.y)
                {
                    //cout<<"YES2"<<endl;
                    root=deleteStatusNode1(root,line_list[temp->p.ind2],temp->p.x,temp->p.y);
                }
            }
            else
            {
                root=deleteStatusNode1(root,line_list[temp->p.ind1],temp->p.x,temp->p.y);
            }
        }
        if(temp->p.intersection==1)
        {
            int c1=0,c2=0;
            if((line_list[temp->p.ind2].x1!=temp->p.x || line_list[temp->p.ind2].y1!=temp->p.y) && (line_list[temp->p.ind2].x2!=temp->p.x || line_list[temp->p.ind2].y2!=temp->p.y))
            {
                c1=1;
            }
            if((line_list[temp->p.ind1].x1!=temp->p.x || line_list[temp->p.ind1].y1!=temp->p.y) && (line_list[temp->p.ind1].x2!=temp->p.x || line_list[temp->p.ind1].y2!=temp->p.y))
            {
                c2=1;
            }
            if(c1==1 && c2==1)
            {
                remaining+=1;
                root=deleteStatusNode1(root,line_list[temp->p.ind1],line_list[temp->p.ind1].x2,line_list[temp->p.ind1].y2);
                root=deleteStatusNode1(root,line_list[temp->p.ind2],line_list[temp->p.ind2].x2,line_list[temp->p.ind2].y2);
                root=insert1(root,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                root=insert1(root,line_list[temp->p.ind2],temp->p.x,temp->p.y);   
            }
            counter+=1;
            cout<<"Lines "<<temp->p.ind1+1<<" and "<<temp->p.ind2+1<<" intersect at ( "<<temp->p.x<<" , "<<temp->p.y<<" )"<<endl;
            output_file<<"Lines "<<temp->p.ind1+1<<" and "<<temp->p.ind2+1<<" intersect at ( "<<temp->p.x<<" , "<<temp->p.y<<" )"<<endl;
        }
        //cout<<"YES"<<endl;
        root1=deleteNode(root1,temp->p);
        inOrder(root1);
        inOrder1(root);
    }
    if(counter==1)
    {
        cout<<"There is 1 intersection point."<<endl;
        output_file<<"There is 1 intersection point."<<endl;
    }
    else
    {
        cout<<"There are "<<counter<<" intersection points."<<endl;
        output_file<<"There are "<<counter<<" intersection points."<<endl;
    }
    cout<<points<<endl;
    return 0;
}