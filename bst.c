/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Kaden "ator" Fugate
 * Email: fugateka@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>

#include "bst.h"
#include "stack.h"

/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` field stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node {
  int key;
  void* value;
  struct bst_node* left;
  struct bst_node* right;
};


/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst {
  struct bst_node* root;
};

/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
struct bst* bst_create() {
  
  struct bst *bst = malloc( sizeof( struct bst ) );
  bst->root = NULL;

  return bst;

}

/*****************************************************************************
 **
 ** BST_FREE FUNCTIONS
 **
 *****************************************************************************/


/*
 * helper function to free tree. uses recursion to delete end first
 * 
 * Params:
 *      node - root
 *
 */
void free_recurse(struct bst_node *node)
{

  if(node->left){ free_recurse(node->left); }
  if(node->right){ free_recurse(node->right); }

  free(node);
  node = NULL;

}

/*
 * This function should free the memory associated with a BST.  While this
 * function should up all memory used in the BST itself, it should not free
 * any memory allocated to the pointer values stored in the BST.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   bst - the BST to be destroyed.  May not be NULL.
 */
void bst_free(struct bst* bst) {

  free_recurse(bst->root);

  free(bst);
  bst = NULL;

}

/*****************************************************************************
 **
 ** BST_SIZE FUNCTIONS
 **
 *****************************************************************************/


/*
 * helper function to count size of tree.
 * 
 * Params:
 *      node - root node
 *      *size - address of size
 *
 */
void size_recurse(struct bst_node *node, int *size)
{

  if(!node){ return; } // if node doesnt exist, end recursion

  (*size)++; // if exists, increment size

  size_recurse(node->left, size);
  size_recurse(node->right, size);

}

/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 */
int bst_size(struct bst* bst) {

  if(!bst){ return -1; } // tree doesnt exist, -1
  else if(!bst->root){ return 0; } // tree exist, no root, 0

  int size = 0;
  size_recurse(bst->root, &size); 

  return size; // size is changed in recursive function
}

/*****************************************************************************
 **
 ** BST_INSERT FUNCTIONS
 **
 *****************************************************************************/


/*
 * helper function to make recursion possible for insertion. If node doesnt exist,
 * node will be inserted. If does not exist, will use recursion until finds 
 * spot for new node.
 * 
 * Params:
 *      **node - address of root
 *      key - key to be inserted
 *      value - value to be inserted
 *
 */
void insert_recurse(struct bst_node **node, int key, void* value)
{

  if( !(*node) ){ // if node doesnt exist, insert and end recursion 

    (*node) = malloc( sizeof( struct bst_node ) ); 
    (*node)->key = key, (*node)->value = value, (*node)->left = NULL, (*node)->right = NULL; // <---- init vals for new node
    return; 

  } 

  if( (*node)->key > key ){ insert_recurse( &((*node)->left), key, value ); } // continue to left or right
  else{ insert_recurse( &((*node)->right), key, value ); }

}

/*
 * This function should insert a new key/value pair into the BST.  The key
 * should be used to order the key/value pair with respect to the other data
 * stored in the BST.  The value should be stored along with the key, once the
 * right location in the tree is found.
 *
 * Params:
 *   bst - the BST into which a new key/value pair is to be inserted.  May not
 *     be NULL.
 *   key - an integer value that should be used to order the key/value pair
 *     being inserted with respect to the other data in the BST.
 *   value - the value being inserted into the BST.  This should be stored in
 *     the BST alongside the key.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void bst_insert(struct bst* bst, int key, void* value) {

  insert_recurse(&(bst->root), key, value);

}

/*****************************************************************************
 **
 ** BST_REMOVE FUNCTIONS
 **
 *****************************************************************************/


/*
 * this is a recursive function that finds the minimum of a subtree.
 * 
 * Params:
 *      node - root node
 *
 */
struct bst_node* predecessor_min(struct bst_node *node)
{

  if( node->left ){ return predecessor_min(node->left); }
  else{ return node; }

}

/*
 * helper function to remove a node from the tree. This function first finds the node being removed.
 * after this, it determines how many children the tree has. if leaf, make node at address null.
 * if one child, make node at address its child. if two children, swap next greatest node. after swapping
 * node, recursively call function to remove swapped value from the tree. if this process is 
 * repeated enough times, eventually it will delete a node with one or zero children.
 * 
 * Params:
 *      **node - address of root
 *      key - key to be removed
 *
 */
void remove_recurse(struct bst_node **node, int key)
{

  if(!(*node)){ return; } 

  if((*node)->key < key){ remove_recurse( &((*node)->right), key ); } // finding node with key
  else if((*node)->key > key){ remove_recurse(&((*node)->left), key); }

  else{ // if node = key

    if(!(*node)->left && !(*node)->right){ free(*node); (*node) = NULL; } // if leaf node

    else if(!(*node)->left){ // one child (right)
      struct bst_node *temp = *node; 
      (*node) = (*node)->right; 

      free(temp); 
      temp = NULL; 
    }

    else if(!(*node)->right){ // one child (left)
      struct bst_node *temp = *node; 
      (*node) = (*node)->left; 

      free(temp); 
      temp = NULL; 
    }

    else{ // two children

      struct bst_node *temp = predecessor_min((*node)->right);
      temp->left = (*node)->left, temp->right = (*node)->right;

      node = &temp;

      remove_recurse(&((*node)->right), (*node)->key); // call recurse to remove swapped val
    }

  }

}
/* 16 lines, very spaced out. was going to make everything inside of the 
statement where the value is found into a seperate function however,
i ran into a circular dependancy where one function always needed the other to compile.
was one line over so i figured it was alright */

/*
 * This function should remove a key/value pair with a specified key from a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should remove the first one it encounters (i.e. the one closest to
 * the root of the tree).
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair to be removed from the BST.
 */
void bst_remove(struct bst* bst, int key) {
  
  if(!bst){ return; }

  remove_recurse(&(bst->root), key); // remove recurse will do everything

}

/*****************************************************************************
 **
 ** BST_GET FUNCTIONS
 **
 *****************************************************************************/


/*
 * helper function for get function. If node exist, check key. If equal, return it
 * if not, recurse down proper tree. If node not found, return NULL.
 * 
 * Params:
 *      node - root node
 *      key - key searching for
 *
 */
void* get_recurse(struct bst_node *node, int key)
{

  if( node ){ // recursion will go until node == NULL or value exists

    if(node->key == key){ return node->value; } // return when key found

    else if(node->key > key){ return get_recurse(node->left, key); } // traverse proper tree

    else{ return get_recurse(node->right, key); }

  }
  
  return NULL; // if not node, return NULL

}

/*
 * This function should return the value associated with a specified key in a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should return the first one it encounters (i.e. the one closest to
 * the root of the tree).  If the BST does not contain the specified key, this
 * function should return NULL.
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair whose value is to be returned.
 *
 * Return:
 *   Should return the value associated with the key `key` in `bst` or NULL,
 *   if the key `key` was not found in `bst`.
 */
void* bst_get(struct bst* bst, int key) {
  
  if(!bst || !bst->root){ return NULL; }

  return get_recurse(bst->root, key);

}

/*****************************************************************************
 **
 ** BST puzzle functions
 **
 *****************************************************************************/

/*****************************************************************************
 **
 ** BST_HEIGHT FUNCTIONS
 **
 *****************************************************************************/


/*
 * helper function to make recursion possible for height function. If height > max,
 * save max. use recursion to repeat until all nodes checked.
 * 
 * Params:
 *      node - root node
 *      *max - address of max
 *      cur - cur depth
 *
 */
void height_recurse(struct bst_node *node, int *max, int cur)
{

  if( cur > *max ){ *max = cur; } // save max

  if(node->left){ height_recurse(node->left, max, cur + 1); }
  if(node->right){ height_recurse(node->right, max, cur + 1); }

}

/*
 * This function should return the height of a given BST, which is the maximum
 * depth of any node in the tree (i.e. the number of edges in the path from
 * the root to that node).  Note that the height of an empty tree is -1 by
 * convention.
 *
 * Params:
 *   bst - the BST whose height is to be computed
 *
 * Return:
 *   Should return the height of bst.
 */
int bst_height(struct bst* bst) {
  
  if(!bst){ return -1; } // no tree, -1
  if(!bst->root || (!bst->root->left && !bst->root->right)){ return 0; } // empty or size 1, 0
  
  int max = 1; // max must be atleast 1 now
  height_recurse(bst->root, &max, 0); 

  return max;

}

/*****************************************************************************
 **
 ** BST_PATH_SUM FUNCTIONS
 **
 *****************************************************************************/


/*
 * This is a helper function to make recursion easier for the path puzzle problem.
 * if the total is less than the sum, it increments. If total is equal to sum when
 * not leaf node, make total not equal to sum. if nodes exist and total is less than sum,
 * use recursion. if total is less than sum and path did not add to sum, try next smallest
 * subtree.
 * 
 * Params:
 *      node - root node
 *      *total - address of total
 *      sum - path sum
 *
 */
void path_recurse(struct bst_node *node, int *total, int sum)
{
  // all if statements

  if((*total) < sum){ (*total) += node->key; } // if total less than sum, add to total

  if( (*total) == sum && (  node->left || node->right ) ){ (*total) += 1; } // if total = sum but not leaf, make total > sum 

  if( node->left && (*total) < sum ){ path_recurse(node->left, total, sum); } // check left to right

  if( node->right && (*total) < sum ){ path_recurse(node->right, total, sum); } /* dont check any further right 
  if total is greater than sum*/

  if((*total) < sum ){ (*total) -= node->key; } /* once done with node in path and still less than sum, 
  remove node from total*/

}

/*
 * This function should determine whether a specified value is a valid path
 * sum within a given BST.  In other words, this function should check whether
 * the given BST contains any path from the root to a leaf in which the keys
 * sum to the specified value.
 *
 * Params:
 *   bst - the BST whose paths sums to search
 *   sum - the value to search for among the path sums of `bst`
 *
 * Return:
 *   Should return 1 if `bst` contains any path from the root to a leaf in
 *   which the keys add up to `sum`.  Should return 0 otherwise.
 */
int bst_path_sum(struct bst* bst, int sum) {
  
  if(!bst){ return 0; } // no tree or empty tree false
  if(!bst->root){return 0;}

  int total = 0;
  path_recurse(bst->root, &total, sum);
  
  if(total == sum){ return 1; } // total will only equal sum when path true
  return 0;
   
}

/*****************************************************************************
 **
 ** BST_RANGE_SUM FUNCTIONS
 **
 *****************************************************************************/


/*
 * This function is a helper function so we can use recursion to do the
 * range sum problem. Adds values if theyre in range, traverses paths
 * that are in correct range. Breaks when node null
 * 
 * Params:
 *      node - root node
 *      *sum - address of sum
 *      lower - lower bound
 *      upper - upper bound
 *
 */
void range_recurse(struct bst_node *node, int *sum, int lower, int upper)
{

  if( !node ){ return; } // break recursion when node doesnt exist
  else if( lower <= node->key && node->key <= upper ){ (*sum) += node->key; } // add when in range

  if( node->left && node->key > lower  ){ range_recurse(node->left, sum, lower, upper); } // when left node exists && is in range
  if( node->right && node->key < upper  ){ range_recurse(node->right, sum, lower, upper); } // when left node exists && is in range

}

/*
 * This function should compute a range sum in a given BST.  Specifically, it
 * should compute the sum of all keys in the BST between a given lower bound
 * and a given upper bound.  For full credit, you should not process any subtree
 * whose keys cannot be included in the range sum.
 *
 * Params:
 *   bst - the BST within which to compute a range sum
 *   lower - the lower bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *   upper - the upper bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *
 * Return:
 *   Should return the sum of all keys in `bst` between `lower` and `upper`.
 */
int bst_range_sum(struct bst* bst, int lower, int upper) {
  
  if(!bst || !bst->root){ return 0; } // no nodes, no sum

  int sum = 0;
  range_recurse(bst->root, &sum, lower, upper); // call recurse passing address of sum

  return sum;
}

/*****************************************************************************
 **
 ** BST iterator definition (extra credit only)
 **
 *****************************************************************************/

/*
 * Structure used to represent a binary search tree iterator.  It contains
 * only a reference to a stack to be used to implement the iterator.
 *
 * You should not modify this structure.
 */
struct bst_iterator {
  struct stack* stack;
};

/*
 * This function is meant to find the left_subtree in order to push it onto
 * the stack.
 * 
 * Params:
 *      node - root node of tree
 *      stack - stack being pushed onto
 *
 */
void left_subtree(struct bst_node *node, struct stack *stack)
{

  stack_push( stack, (void*) node );

  if(node->left){ left_subtree(node->left, stack); }

}

/*
 * This function should allocate and initialize an iterator over a specified
 * BST and return a pointer to that iterator.
 *
 * Params:
 *   bst - the BST for over which to create an iterator.  May not be NULL.
 */
struct bst_iterator* bst_iterator_create(struct bst* bst) {
  
  struct bst_iterator *iter = malloc( sizeof( struct bst_iterator ) ); // mem for iter

  iter->stack = stack_create(); // mem for stack

  if(bst){ left_subtree( bst->root, (iter->stack) ); } // fill stack with left subtrees

  return iter;
}

/*
 * This function should free all memory allocated to a given BST iterator.
 * It should NOT free any memory associated with the BST itself.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   iter - the BST iterator to be destroyed.  May not be NULL.
 */
void bst_iterator_free(struct bst_iterator* iter) {
  
  stack_free(iter->stack);

  free(iter);
  iter = NULL;

}

/*
 * This function should indicate whether a given BST iterator has more nodes
 * to visit.  It should specifically return 1 (true) if the iterator has at
 * least one more node to visit or 0 (false) if it does not have any more
 * nodes to visit.
 *
 * Param:
 *   iter - the BST iterator to be checked for remaining nodes to visit.  May
 *     not be NULL.
 */
int bst_iterator_has_next(struct bst_iterator* iter) {
  
  if(stack_isempty(iter->stack)){ return 0; }

  return 1;

}

/*
 * This function should return both the value and key associated with the
 * current node pointed to by the specified BST iterator and advnce the
 * iterator to point to the next node in the BST (in in-order order).
 *
 * Because a function can't return two things, the key associated with the
 * current node should be returned the normal way, while its value should be
 * returned via the argument `value`.  Specifically, the argument `value`
 * is a pointer to a void pointer.  The current BST node's value (a void
 * pointer) should be stored at the address represented by `value` (i.e. by
 * dereferencing `value`).  This will look something like this:
 *
 *   *value = current_node->value;
 *
 * Parameters:
 *   iter - BST iterator.  The key and value associated with this iterator's
 *     current node should be returned, and the iterator should be updated to
 *     point to the next node in the BST (in in-order order).  May not be NULL.
 *   value - pointer at which the current BST node's value should be stored
 *     before this function returns.
 *
 * Return:
 *   This function should return the key associated with the current BST node
 *   pointed to by `iter`.
 */
int bst_iterator_next(struct bst_iterator* iter, void** value) {
  
  if(!bst_iterator_has_next(iter)){ return -1; }

  struct bst_node *temp = stack_pop(iter->stack); // store val to return

  if(temp->right){ left_subtree(temp->right, iter->stack); } /* when popping node
  next smallest left subtree is pushed to stack */

  if (temp->value) {
    *value = temp->value;
  }
  else{ *value = NULL; }

  return temp->key;
}
