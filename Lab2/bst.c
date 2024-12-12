//=============================================================================
// bst.c - binary search tree, based on binary tree (bt.h/bt.c)
//=============================================================================
#include "global.h"
#include "bst.h"

//-----------------------------------------------------------------------------
// local function prototypes
//-----------------------------------------------------------------------------
typedef struct Queue {
    BST* nodes;
    int front;
    int rear;
    int size;
} Queue;
static void _preorder(BST T, int* pos, int* a);
static void _inorder(BST T, int* pos, int* a);
static void _postorder(BST T, int* pos, int* a);
static void _bfs(BST T, int* pos, int* a, int max);
static Queue* createQueue(int capacity);
static void enqueue(Queue* q, BST node);
static BST dequeue(Queue* q);
static int isQueueEmpty(Queue* q);
static BST remove_root(BST T);
static BST findMin(BST T);
static BST findMax(BST T);

//-----------------------------------------------------------------------------
// public functions, exported through bst.h
//-----------------------------------------------------------------------------
// new_BST: creates a new BST
BST new_BST(int val)
{
	return new_BT(val);
}
//-----------------------------------------------------------------------------
// bst_add: adds the value v to the BST T (unless duplicate)
//-----------------------------------------------------------------------------
BST bst_add(BST T, int v)
{
	return	!T          ?	new_BST(v)                                    :
		v < get_val(T)	?	cons(add(get_LC(T), v), T, get_RC(T))         :
		v > get_val(T)	?	cons(get_LC(T), T, add(get_RC(T), v))         :
		T;	//for when we have a duplicate
}
//-----------------------------------------------------------------------------
// bst_rem: removes the value val from the BST (if it exists)
//-----------------------------------------------------------------------------


BST bst_rem(BST T, int val)
{

	return  !T                 ? T                                           :
			(val < get_val(T)) ? cons(bst_rem(get_LC(T), val), T, get_RC(T)) :
			(val > get_val(T)) ? cons(get_LC(T), T, bst_rem(get_RC(T), val)) :
			remove_root(T);
} 


//-----------------------------------------------------------------------------
// preorder: puts the BST T values into array a in preorder
//-----------------------------------------------------------------------------
//   2                    2
//  / \	--> [2,1,3]      / \  --> [2,3]
// 1  3                 *  3
//-----------------------------------------------------------------------------
void preorder(BST T, int* a)
{
	int pos = 0;
	_preorder(T, &pos, a);
}
//-----------------------------------------------------------------------------
// inorder: puts the BST T values into array a in inorder
//-----------------------------------------------------------------------------
//   2                    2
//  / \	--> [1,2,3]      / \  --> [2,3]
// 1  3                 *  3
//-----------------------------------------------------------------------------
void inorder(BST T, int* a)
{
	int pos = 0;
	_inorder(T, &pos, a);
	
}
//-----------------------------------------------------------------------------
// postorder: puts the BST T values into array a in postorder
//-----------------------------------------------------------------------------
//   2                    2
//  / \	--> [1,3,2]      / \  --> [3,2]
// 1  3                 *  3
//-----------------------------------------------------------------------------
void postorder(BST T, int* a)
{
	int pos = 0;
	_postorder(T, &pos, a);
}
//-----------------------------------------------------------------------------
// bfs: puts the BST T values into array a in bfs-order, non-nodes
// are indicated by X, as defined in global.h
//-----------------------------------------------------------------------------
//   2                    2
//  / \	--> [2,1,3]      / \  --> [2,X,3]
// 1  3                 *  3
//-----------------------------------------------------------------------------
void bfs(BST T, int* a, int max)
{
	int pos = 0;
	_bfs(T, &pos, a, max);
}
//-----------------------------------------------------------------------------
// is_member: checks if value val is member of BST T
//-----------------------------------------------------------------------------
bool is_member(BST T, int val)
{
	if(!T)
		return false;
	
	return (val > get_val(T))    ?     is_member(get_RC(T), val)     :
	       (val < get_val(T))    ?     is_member(get_LC(T), val)     :
		   (val == get_val(T))   ?     true                          :
		   false;

//	if (val < get_val(T)){
//		is_member(get_RC(T));
//	}
//	else if(val > get_val(T)){
//		is_member(get_LC(T));
//	}
//	else if(val == get_val(T)){
//		return true;
//	}
//	return false;
}
//-----------------------------------------------------------------------------
// height: returns height of BST T
//-----------------------------------------------------------------------------
int height(BST T)
{
	if(!T) return 0;

	//Vi räknar ut höjden på den vänstra sidan och den högra separat
	int leftHeight = height(get_LC(T));
    int rightHeight = height(get_RC(T));
	
	// Returnerar den längsta av ovanstående + startnoden
	if (leftHeight > rightHeight)
		return leftHeight + 1;
	else
		return rightHeight + 1;
}
//-----------------------------------------------------------------------------
// size: returns size of BST T
//-----------------------------------------------------------------------------
int size(BST T)
{
	if(!T) return 0;
	
	return 1 + size(T->LC) + size(T->RC); //Kalkulerar size rekursivt genom att gå igenom varje nods vänstra och högra sida tills dem returnerar 0
}
//-----------------------------------------------------------------------------
// private helper functions, not exported
//-----------------------------------------------------------------------------
static void _preorder(BST T, int* pos, int* a)
{
	if (T)
	{
		a[(*pos)++] = get_val(T);
		_preorder(get_LC(T), pos, a);
		_preorder(get_RC(T), pos, a);
	}
}
static void _inorder(BST T, int* pos, int* a) {
	if(T) {
		_inorder(get_LC(T), pos, a);
		a[(*pos)] = get_val(T);
		(*pos)++;
		_inorder(get_RC(T), pos, a);
	}
}
static void _postorder(BST T, int* pos, int* a){
	if(T){
		_postorder(get_LC(T), pos, a);
		_postorder(get_RC(T), pos, a);
		a[(*pos)] = get_val(T);
		(*pos)++;
	}	
}

static void _bfs(BST T, int* pos, int* a, int max) {
    if (T) {
        Queue* q = createQueue(max);
        enqueue(q, T);

        while (!isQueueEmpty(q) && *pos < max) {
            BST node = dequeue(q);

            if (node != NULL) {
                // Add the current node's value
                a[(*pos)++] = node->val;

                // Enqueue left and right children (even if NULL)
                enqueue(q, node->LC);
                enqueue(q, node->RC);
            } else {
                // Add placeholder ('*') for missing children
                a[(*pos)++] = X;

                // Still enqueue NULLs to maintain tree structure
                if (*pos + 1 < max) {
                    enqueue(q, NULL); // Placeholder left
                    enqueue(q, NULL); // Placeholder right
                }
            }
        }
        free(q->nodes);
        free(q);
    }
}

static Queue* createQueue(int capacity) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->nodes = (BST*)malloc(capacity * sizeof(BST));
    q->front = q->rear = 0;
    q->size = capacity;
    return q;
}

// Dequeue function
static BST dequeue(Queue* q) {
    return q->nodes[q->front++];
}

// Check if the queue is empty
static int isQueueEmpty(Queue* q) {
    return q->front == q->rear;
}

static void enqueue(Queue* q, BST node) {
	if (q->rear < q->size) { // Check for queue overflow
		q->nodes[q->rear++] = node;
	}
}

static BST remove_root(BST T){
	if(!get_LC(T) && !get_RC(T)){
		return NULL;
	}else if(get_LC(T) && !get_RC(T)){
		return get_LC(T);
	}else if(!get_LC(T) && get_RC(T)){
		return get_RC(T);
	}else{
		BST temp;
		if(height(get_RC(T)) > height(get_LC(T))){
			temp = findMin(get_RC(T));
			set_val(T, get_val(temp));
			set_RC(T, bst_rem(get_RC(T), get_val(temp)));
		}else{
			temp = findMax(get_LC(T));
            set_val(T, get_val(temp));
            set_LC(T, bst_rem(get_LC(T), get_val(temp)));
		}
	}
	return T;
}

static BST findMin(BST T){

	if(!T) return NULL;
	while(get_LC(T) != NULL){
		T = get_LC(T);
	}
	return T;
}

static BST findMax(BST T) {
    if (!T) return NULL;

    while (get_RC(T) != NULL) {
        T = get_RC(T);
    }
    return T;
}
