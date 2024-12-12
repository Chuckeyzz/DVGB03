//=============================================================================
// avl.c - AVL-tree based on binary search tree (bst.h/bst.c)
//=============================================================================
#include "avl.h"
#define DEBUG 0
//=============================================================================
// local prototypes
//-----------------------------------------------------------------------------
static AVL srr(AVL T);
static AVL slr(AVL T);
static AVL drr(AVL T);
static AVL dlr(AVL T);
static int isBalanced(AVL T);

//=============================================================================
// Public functions, exported via .h-file
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// avl_add: adds the value val to AVL T in a balanced fashion
//-----------------------------------------------------------------------------
AVL avl_add(AVL T, int val) {

	if(DEBUG)printf("avl_add (%d)\n",val);
	T = bst_add(T, val);
	T = balance(T);
	return T;
}


//-----------------------------------------------------------------------------
// avl_rem: removes the value val from AVL T in a balanced fashion
//-----------------------------------------------------------------------------
AVL avl_rem(AVL T, int val)
{
	if(DEBUG)printf("avl_rem (%d)\n",val);
	T = bst_rem(T, val);
	T = balance(T);
	return T;
}
//-----------------------------------------------------------------------------
// balance: balances the AVL tree T if needed
//-----------------------------------------------------------------------------
AVL balance(AVL T)
{
	int bCount = 0;
	bCount = isBalanced(T);

	//left heavy
	if(bCount > 1){
		//left left 
		if(isBalanced(get_LC(T)) >= 0){
			return srr(T);		
		}
		//left right
		else {
			return drr(T);	
		}
	}
	//right heavy
	if(bCount < -1){
		//right right
		if(isBalanced(get_RC(T)) <= 0){
			return slr(T);
		}
		else{
			//right left
			return dlr(T);
		}
	}
	return T;
}
//=============================================================================
// Private functions, for local use only
//-----------------------------------------------------------------------------
static AVL slr(AVL T)                              //SLR (+outside right)
{
	if(DEBUG)printf("slr\n");
	
	AVL temp = get_RC(T);
	set_RC(T, get_LC(temp));                       //set takes two parameters  (1 who wants the child, and 2 who gives it)
	set_LC(temp, T);                       //set_LC (taker, giver)
	return temp;
	
	//return T;
}
static AVL srr(AVL T)                               //SRR (+outside left)
{
	if(DEBUG)printf("srr\n");
	
	AVL temp = get_LC(T);
	set_LC(T, get_RC(temp));                       //set takes two parameters  (1 who wants the child, and 2 who gives it)
	set_RC(temp, T);                       //set_LC (taker, giver)
	return temp;
	
	//return T; ????
}
static AVL drr(AVL T)
{
	if(DEBUG)printf("drr\n");

	set_LC(T, slr(get_LC(T)));                    
	T = srr(T);                                           //perform right Single Right Rotation on T
	
	return T;
}
static AVL dlr(AVL T)
{
	if(DEBUG)printf("drr\n");
	
	set_RC(T, srr(get_RC(T)));                    
	T = slr(T);                                           //perform right Single Left Rotation on T
	
	return T;
}

static int isBalanced(AVL T){
	return  T ? (height(get_LC(T)) - height(get_RC(T))) :
			0;
}

