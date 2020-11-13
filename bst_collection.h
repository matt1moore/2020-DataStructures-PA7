//----------------------------------------------------------------------
// Name: Matthew Moore
// File: hw7_test.cpp
// Date: Fall 2020
// Desc: Binary Search Tree Implementation 
//---

#ifndef BST_COLLECTION_H
#define BST_COLLECTION_H

#include "array_list.h"
#include "collection.h"

using namespace std;

template<typename K, typename V>
class BSTCollection : public Collection<K,V> 
{
public:
  BSTCollection();
  BSTCollection(const BSTCollection<K,V>& rhs);
  ~BSTCollection();
  BSTCollection& operator=(const BSTCollection<K,V>& rhs);

  void add(const K& a_key, const V& a_val);
  void remove(const K& a_key);
  bool find(const K& search_key, V& the_val) const;
  void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  void keys(ArrayList<K>& all_keys) const;
  void sort(ArrayList<K>& all_keys_sorted) const;
  size_t size() const;  
  size_t height() const;
  
private:
  // tree node
  struct Node {
    K key;
    V value;
    Node* left;
    Node* right;
  };
  // root node of the tree
  Node* root;
  // number of k-v pairs stored in the collection
  size_t node_count;
  // remove all elements in the bst
  void make_empty(Node* subtree_root);
  // copy helper
  void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root);
  // remove helper
  Node* remove(Node* subtree_root, const K& a_key);
  // helper to recursively build up key list
  void find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const;
  // helper to recursively build sorted list of keys
  void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;
  // helper to recursively find height of the tree
  size_t height(const Node* subtree_root) const;
  
};

// Function Definitions
template<typename K, typename V>
BSTCollection<K,V>::BSTCollection()
  : root(nullptr), node_count(0)
{

}
template<typename K, typename V>
BSTCollection<K,V>::BSTCollection(const BSTCollection<K,V>& rhs)
  : root(nullptr), node_count(0)
{
  // Defer to the assignment operator
  *this = rhs;
}
template<typename K, typename V>
BSTCollection<K,V>::~BSTCollection()
{
  make_empty(root); 
  root = nullptr;
}
template<typename K, typename V>
BSTCollection<K,V>& BSTCollection<K,V>::operator=(const BSTCollection<K,V>& rhs)
{
  if (this != &rhs) { // protects against the self assignment case 
    node_count = 0;
	if (root != nullptr) {
	  make_empty(root);
	  root = nullptr;
	}
	// Assignment made to make the binary trees identical, with unique memory addresses
	node_count = rhs.node_count;
	
	if (rhs.root != nullptr) {
	  // Create the root node for the copy assignment
	  root = new Node;
	  root->key = rhs.root->key;
	  root->value = rhs.root->value;
	  root->left = nullptr;
	  root->right = nullptr; 
	  copy(root,rhs.root); 
	}
  }
  return *this; 
    
}
template<typename K, typename V>
void BSTCollection<K,V>::add(const K& a_key, const V& a_val)
{
  // Creating the new node
  Node * curr_ptr = root;
  Node * newNode = new Node;
  newNode->key = a_key;
  newNode->value = a_val;
  
  // Case of first element being added
  if (curr_ptr == NULL) {
    // Root points to NULL so add initial node as the root
	root = newNode;
	root->left = NULL;
	root->right = NULL;
	++node_count;
	return;
  }
  
  // Traversing down a path to the end 
  while (curr_ptr != NULL) {
    if (a_key > curr_ptr->key) {
	  if (curr_ptr->right == NULL) {
	    // leaf node has been reached
		curr_ptr->right = newNode;
		newNode->left = NULL;
		newNode->right = NULL;
		++node_count;
		return;
	  }
	  // The key value being added is larger than the current node in the list, so go right
	  curr_ptr = curr_ptr->right;
	}
    else if (a_key < curr_ptr->key) {
	  if (curr_ptr->left == NULL) {
	    // leaf node has been reached
		curr_ptr->left = newNode;
		newNode->left = NULL;
		newNode->right = NULL;
		++node_count;
		return;
	  }
	  // The key value added is smaller than the current node, so go left
	  curr_ptr = curr_ptr->left;
	}
	else {
	  // Value already exists in the list.. so do nothing
	  return;
	}
  }
}
template<typename K, typename V>
void BSTCollection<K,V>::remove(const K& a_key)
{
  Node * subtree_root = remove(root,a_key); 
  
  if (height(subtree_root) == 2) {
    // CASE 1: leaf node
	if (a_key > subtree_root->key) { 
      // The key removed was to the right
	  subtree_root->right = nullptr;
	}
	else {
	  subtree_root->left = nullptr;
	}
  }
  else if (height(subtree_root) == 1) {
    // CASE 2: Single child node
	if (root == nullptr) {
	  root = subtree_root;
	}
  }
  // The node removed was the leaf node
	
  /*
  Node * curr_ptr = root;
  Node * prev_ptr = nullptr; 
  while (curr_ptr != NULL) {
    // Extends down a path of the binary tree to locate the node
	if (curr_ptr->key == a_key) {
      // Key has been located
	  if (height(curr_ptr) == 1) {
	    // CASE 1: leaf node
		if (curr_ptr == root) {
		  // Case of root being the current pointer 
		  delete curr_ptr;
		  --node_count; 
		  root = nullptr;
		  return;
		}
		else if (prev_ptr->key > curr_ptr->key) {
		  // Tells us that the curr pointer is on the left
		  prev_ptr->left = nullptr;
		}
		else {
		  // Otherwise it will be located on the right
		  prev_ptr->right = nullptr;
		}
        delete curr_ptr;
		--node_count;
        return; 		
	  }
	  else if (curr_ptr->left == nullptr || curr_ptr->right == nullptr) {
	    // CASE 2: Single Child Case 
		// Right Node should always take the parent position, unless it does not exist
		if (curr_ptr == root) {
		  // Current pointer is the root, so no use for the previous pointer
		  if (curr_ptr->right != nullptr) {
		    root = curr_ptr->right;
		  }
		  else {
		    root = curr_ptr->left;
		  }
		}
		else if (prev_ptr > curr_ptr) {
		  // Curr pointer is on the left
		  if (curr_ptr->right != nullptr) {
		    prev_ptr->left = curr_ptr->right;
		  }
		  else {
		    prev_ptr->left = curr_ptr->left;
		  }
		}
		else {
		  // Curr pointer is on the right
		  if (curr_ptr->right != nullptr) {
		    prev_ptr->left = curr_ptr->right;
		  }
		  else {
		    prev_ptr->left = curr_ptr->left;
		  }
		}
		delete curr_ptr;
		--node_count;
		return; 
	  }
	  else if (height(curr_ptr) == 2 && (curr_ptr->left != nullptr && curr_ptr->right != nullptr)) {
	    // CASE 3: Two Children with height of 2
		if (curr_ptr == root) {
		  // Special case of the root being the removed node
		  root = curr_ptr->right;
		  root->left = curr_ptr->left;
		  root->right = nullptr;
		}
		else if (curr_ptr > prev_ptr) {
		  // current pointer is on the right of the previous
		  prev_ptr->right = curr_ptr->right;
		  prev_ptr->right->left = curr_ptr->left;
		  prev_ptr->right->right = nullptr;
		}
		else {
		  // current pointer is on the left of the previous
		  prev_ptr->left = curr_ptr->right;
		  prev_ptr->left->left = curr_ptr->left;
		  prev_ptr->left->right = nullptr;
		}
		delete curr_ptr;
		--node_count;
		return;
	  }
	  else {
	    // CASE 4: Curr pointer has height of more than 2
	    curr_ptr = remove(curr_ptr,a_key);
		delete curr_ptr;
		--node_count;
		return;
	  }
	}
	else if (a_key < curr_ptr->key) {
	  // key being removed is greater than the current key
	  prev_ptr = curr_ptr;
	  curr_ptr = curr_ptr->left;
	}
	else if (a_key > curr_ptr->key) {
	  // key being removed is less than the current key
	  prev_ptr = curr_ptr;
	  curr_ptr = curr_ptr->right;
	}
  }
  
  
  
  
  // base case
    if (root == NULL)
        return root;
 
    // If the key to be deleted is 
    // smaller than the root's
    // key, then it lies in left subtree
    if (key < root->key)
        root->left = deleteNode(root->left, key);
 
    // If the key to be deleted is
    // greater than the root's
    // key, then it lies in right subtree
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
 
    // if key is same as root's key, then This is the node
    // to be deleted
    else {
        // node with only one child or no child
        if (root->left == NULL) {
            struct node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct node* temp = root->left;
            free(root);
            return temp;
        }
 
        // node with two children: Get the inorder successor
        // (smallest in the right subtree)
        struct node* temp = minValueNode(root->right);
 
        // Copy the inorder successor's content to this node
        root->key = temp->key;
 
        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
  */
}
template<typename K, typename V>
bool BSTCollection<K,V>::find(const K& search_key, V& the_val) const
{
  Node * curr_ptr = root;
  
  while (curr_ptr != NULL) {
    // Search down a path in the binary tree to find the node
	if (curr_ptr->key == search_key) {
      // Key has been located
	  the_val = curr_ptr->value;
	  return true;
	}
	else if (search_key < curr_ptr->key) {
	  // key being removed is greater than the current key
	  curr_ptr = curr_ptr->left;
	}
	else if (search_key > curr_ptr->key) {
	  // key being removed is less than the current key
	  curr_ptr = curr_ptr->right;
	}
  }
  // This point is only reached if the node searched for is not found
  return false;
}
template<typename K, typename V>
void BSTCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  find(root,k1,k2,keys);
}
template<typename K, typename V>
void BSTCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  keys(root,all_keys); 
}
template<typename K, typename V>
void BSTCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);
}
template<typename K, typename V>
size_t BSTCollection<K,V>::size() const
{
  return node_count;
}	

template<typename K, typename V>
size_t BSTCollection<K,V>::height() const
{
  return height(root);
}

// HELPER FUNCTIONS

template<typename K, typename V>
void BSTCollection<K,V>::make_empty(Node* subtree_root)
{
  if (!subtree_root) {
    // BASE CASE
	return;
  }
  make_empty(subtree_root->left);
  make_empty(subtree_root->right); 
  delete subtree_root;
}

template<typename K, typename V>
void BSTCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root)
{
  if(!rhs_subtree_root) { 
    // BASE CASE
	return;
  }
  
  if (rhs_subtree_root->left != nullptr) {
	Node * newNode = new Node;
    newNode->key = rhs_subtree_root->left->key;
    newNode->value = rhs_subtree_root->left->value;
    newNode->left = nullptr;
    newNode->right = nullptr; 
    lhs_subtree_root->left = newNode;
	copy(lhs_subtree_root->left,rhs_subtree_root->left);
  }
  if (rhs_subtree_root->right!= nullptr) {
	Node * newNode = new Node;
    newNode->key = rhs_subtree_root->right->key;
    newNode->value = rhs_subtree_root->right->value;
    newNode->left = nullptr;
    newNode->right = nullptr; 
    lhs_subtree_root->right = newNode;
	copy(lhs_subtree_root->right,rhs_subtree_root->right);
  }
  
  
  
}

template<typename K, typename V>
typename BSTCollection<K,V>::Node *
BSTCollection<K,V>::remove(Node* subtree_root, const K& a_key)
{
  if (subtree_root == nullptr) {
    // BASE CASE
	return NULL;
  }
  
  // Locator of the node which needs to be removed
  if (subtree_root != nullptr && (a_key < subtree_root->key)) {
	// Removing element is to the left
    subtree_root->left = remove(subtree_root->left,a_key);
  }
  else if (subtree_root != nullptr && (a_key > subtree_root->key)) {
	// Removing element is to the right
    subtree_root->right = remove(subtree_root->left,a_key);
  }
  else if (subtree_root != nullptr && (subtree_root->key == a_key)) {
    // Key has been located
	if (height(subtree_root) == 1) {
	  // CASE 1: leaf node
      if (subtree_root == root) {
		// Case of root being the current pointer 
		delete subtree_root;
		--node_count; 
		  root = nullptr;
	  }
	  else {
	    // Must set the left or right pointer of the parent to null
		// Allow the recursion to climb back up to the parent
		delete subtree_root;
		--node_count; 
	  }
	}
	else if (subtree_root->right == nullptr || subtree_root->left == nullptr) {
	  // CASE 2: Node with one child node
	  Node * tmp;
	  if (subtree_root->right != nullptr) {
	    tmp = subtree_root->right;
	  }
	  else {
	    tmp = subtree_root->left;
	  }
	  if (subtree_root == root) {
	    root = nullptr;
	  }
	  delete subtree_root;
	  --node_count;
	  return tmp;
	}
	else if (height(subtree_root) == 2 && (subtree_root->right != nullptr && subtree_root->left)) {
	  // CASE 3: 2 Nodes but only a height of two
	  // delete subtree_root;
	  // --node_count;
	}
	else {
	  // delete subtree_root;
	  // --node_count; 
	}
  }
  
  return subtree_root;
  
}
template<typename K, typename V>
void BSTCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const
{
  if (!subtree_root) {
    // Empty tree base case
	return;
  }

  if (k1 > subtree_root->key) {
    find(subtree_root->right,k1,k2,keys);
  }
  else if (k1 <= subtree_root->key && subtree_root->key <= k2) {
	keys.add(subtree_root->key);
    find(subtree_root->left,k1,k2,keys);
	find(subtree_root->right,k1,k2,keys);
  }
  else if (k2 < subtree_root->key) {
    find(subtree_root->left,k1,k2,keys); 
  }
}
template<typename K, typename V>
void BSTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const
{
  if (!subtree_root) {
    // Base case
    return;
  }
  keys(subtree_root->left,all_keys);
  all_keys.add(subtree_root->key);
  keys(subtree_root->right,all_keys);
}
template<typename K, typename V>
size_t BSTCollection<K,V>::height(const Node* subtree_root) const
{
  size_t left, right;
  
  if (subtree_root == NULL) {
    return 0;
  }
  else {
    left = height(subtree_root->left);
    right = height(subtree_root->right);
    return max(left,right) + 1;
  }
}

#endif