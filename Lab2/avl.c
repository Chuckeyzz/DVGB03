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
static AVL new_AVL(int val);
//static AVL handleRC(AVL T, int val);
//static AVL handleLC(AVL T, int val);
//=============================================================================
// Public functions, exported via .h-file
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// avl_add: adds the value val to AVL T in a balanced fashion
//-----------------------------------------------------------------------------
static AVL new_AVL(int val){
	return new_BT(val);
}
AVL avl_add(AVL T, int val) {

//	int innerBalance = 0;
//	int outerBalance = 0; //om höger ++ vänster om root --
	
	return	!T              ?	new_AVL(val)                                        :
		val < get_val(T)	?	cons(avl_add(get_LC(T), val), T, get_RC(T))         :
		val > get_val(T)	?	cons(get_LC(T), T, avl_add(get_RC(T), val))         :
		T;	//for when we have a duplicate
	
	if(DEBUG)printf("avl_add (%d)\n",val);
	//TODO

	return T;
	}


//-----------------------------------------------------------------------------
// avl_rem: removes the value val from AVL T in a balanced fashion
//-----------------------------------------------------------------------------
AVL avl_rem(AVL T, int val)
{
	if(DEBUG)printf("avl_rem (%d)\n",val);
	// TODO
	return T;
}
//-----------------------------------------------------------------------------
// balance: balances the AVL tree T if needed
//-----------------------------------------------------------------------------
AVL balance(AVL T)
{
	return srr(slr(drr(dlr(T))));
}
//=============================================================================
// Private functions, for local use only
//-----------------------------------------------------------------------------
static AVL srr(AVL T)                               //SRR (+outside left)
{
	if(DEBUG)printf("srr\n");
	
	AVL temp = get_LC(T);
	set_LC(temp, get_LC(T));                       //set takes two parameters  (1 who wants the child, and 2 who gives it)
	set_RC(get_RC(temp), T);                       //set_LC (taker, giver)
	return temp;
	
	//return T; ????
}
static AVL slr(AVL T)                              //SLR (+outside right)
{
	if(DEBUG)printf("slr\n");
	
	AVL temp = get_LC(T);
	set_RC(temp, get_RC(T));                       //set takes two parameters  (1 who wants the child, and 2 who gives it)
	set_LC(get_LC(temp), T);                       //set_LC (taker, giver)
	return temp;
	
	//return T;
}
static AVL drr(AVL T)
{
	if(DEBUG)printf("drr\n");

	set_LC(get_LC(T), slr(get_LC(T)));                    
	T = srr(T);                                           //perform right Single Right Rotation on T
	
	return T;
}
static AVL dlr(AVL T)
{
	if(DEBUG)printf("drr\n");
	
	set_RC(get_RC(T), srr(get_RC(T)));                    
	T = slr(T);                                           //perform right Single Left Rotation on T
	
	return T;
}
