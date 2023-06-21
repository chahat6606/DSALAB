#include "../include/common.h"
#include "pa2.h"

// don't remove these globals
static struct record *bst_root = NULL;
static struct record *avl_root = NULL;
static size_t num_bst_nodes = 0;
static size_t num_avl_nodes = 0;

// don't modify this function
struct record *get_bst_root()
{
  return bst_root;
}

// don't modify this function
struct record *get_avl_root()
{
  return avl_root;
}

// don't modify this function
// compare two uids
// return -1 if uid1 < uid2
// return 0 if uid1 == uid2
// return 1 if uid1 > uid2
static int cmp_uid(char *uid1, char *uid2)
{
  int i;

  for (i = 0; i < MAX_LEN; i++) {
    if (uid1[i] > uid2[i]) {
      return 1;
    }
    else if (uid1[i] < uid2[i]) {
      return -1;
    }
  }
  return 0;
}

// don't modify this function
// compare two records
// we assume that uid is unique and
// there can't be two records with the 
// same uid
// return -1 if record1 < record2
// return 0 if record1 == record2
// return 1 if record1 > record2
static int cmp_record(struct record *r1, struct record *r2)
{
  return cmp_uid(r1->uid, r2->uid);
}

// don't modify this function
static void *allocate_memory(size_t size)
{
  return __mymalloc(size);
}

// don't modify this function
static void free_memory(void *ptr)
{
  __myfree(ptr);
}

// ---------------- Add your code below this line ----------------- //

// Return the total number of records in the
// BST rooted at bst_root.
size_t get_num_bst_records() {
  return num_bst_nodes;
}
// Return the total number of records in the
// AVL tree rooted at avl_root.
size_t get_num_avl_records(){
  return num_avl_nodes;
}
// Insert record r in the BST rooted at bst_root.
void insert_record_bst(struct record r){
  struct record *a=(struct record*)allocate_memory(sizeof(struct record));
  *a=r;
  if (bst_root==NULL){

  bst_root=a;
  num_bst_nodes=1;
  return;}
  int e=1;
  struct record *temp=bst_root;
  while(e==1){
    if (cmp_record(temp,a)==1){
      if(temp->left==NULL){
        temp->left=a;
        num_bst_nodes+=1;
        break;}
      temp=temp->left;}
    else{
      if (temp->right==NULL){
        temp->right=a;
        num_bst_nodes+=1;
        break;
      }
      temp=temp->right;}
    }
}
// Insert record r in the AVL tree rooted at avl_root.
// defining balance and rotation functions;
//set height
void set_height(struct record *n) {
int l_height = (n->left) ? n->left->height : -1;
int r_height = (n->right) ? n->right->height : -1;
n->height = (l_height > r_height) ? l_height + 1: r_height + 1;
}
//get_balace
int get_balance(struct record *n) {
int l_height = (n->left) ? n->left->height : -1;
int r_height = (n->right) ? n->right->height : -1;
return l_height - r_height;}
//rotations
struct record* LL(struct record *n) {
  struct record *new_root = n->left;
  n->left = new_root->right;
  new_root->right = n;
  set_height(n);
  set_height(new_root);
  return new_root;}
struct record* RR(struct record*n) {
  struct record *new_root = n->right;
  n->right = new_root->left;
  new_root->left = n;
  set_height(n);
  set_height(new_root);
  return new_root;}
 //LR ROTATION
static struct record* LR(struct record *n) {
  struct record *mid = n->left;
  struct record *new_root = mid->right;
  mid->right = new_root->left;
  n->left = new_root->right;
  new_root->left = mid;
  new_root->right = n;
  set_height(n);
  set_height(mid);
  set_height(new_root);
  return new_root;}
//RR rotation
static struct record* RL(struct record *n) {
  struct record *mid = n->right;
  struct record *new_root = mid->left;
  mid->left = new_root->right;
  n->right = new_root->left;
  new_root->left = n;
  new_root->right = mid;
  set_height(n);
  set_height(mid);
  set_height(new_root);
  return new_root;}
//rotationn
struct record* try_rotate(struct record *n) {
  int bal = get_balance(n);
  if (bal < -1) {
    bal = get_balance(n->right);
    if (bal > 0) { 
      n = RL(n);}
    else {
      n = RR(n);}
}
  else if (bal > 1) {
    bal = get_balance(n->left);
    if (bal < 0) {
      n = LR(n);
    }
    else {
      n = LL(n);
    }
  }
  return n;
}

struct record *allocate_node(struct record r)
{struct record *ni = allocate_memory(sizeof(struct record));
  *ni = r;
  ni->left = NULL;
  ni->right = NULL; ni->friends = NULL;

  return ni;}

struct record *insert_avl(struct record *root, struct record r)
{ if (root == NULL)
  {
    num_avl_nodes++;
    return allocate_node(r);
  }

  else
  {
    if (cmp_uid(r.uid, root->uid) >= 0)
    {
      root->right = insert_avl(root->right, r);
    }
    else
    {
      root->left = insert_avl(root->left, r);
    }

    set_height(root);
    root = try_rotate(root);
    return root;
  }
}
void insert_record_avl(struct record r){
  avl_root=insert_avl(avl_root,r);
  return; }
// Search the record corresponding to uid in the BST
// rooted at bst_root.
// If the record is not present, return a dummy record
// with -1 in the status field.
struct record search_record_bst(char uid[MAX_LEN])
{  struct record *temp=bst_root;
   while(temp!=NULL) {
  if (cmp_uid(temp->uid,uid)==0){
    return *temp ;}
  if (cmp_uid(temp->uid,uid)==-1){
    temp=temp->right;
    continue;
  }
  temp=temp->left;}

  struct record dummy_record;
  dummy_record.status=-1;
  return dummy_record;
} 

// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// Return the head of the linked list of friends
// of the user with uid uid in the BST rooted at bst_root.
// If the corresponding record doesn't exist, return NULL.
struct list_records *get_friends_list_bst(char uid[MAX_LEN])
{while(bst_root!=NULL) {
  if (cmp_uid(bst_root->uid,uid)==0){
    return (bst_root->friends) ;}
  if (cmp_uid(bst_root->uid,uid)==-1){
    bst_root=bst_root->right;
    continue;
  }
  bst_root=bst_root->left;}
  return NULL;
} 


// Delete the record (say n) corresponding to uid from the BST.
// Remove n from the lists of friends of other records
// and release the memory for the linked list nodes.
// Release memory for all the nodes in the list of friends of n.
// Return a copy of the value of the deleted node.
// If the node is not present, return a dummy record
// with -1 in the status field.
struct record delete_record_bst(char uid[MAX_LEN]){}

// Search the record corresponding to uid in the AVL tree
// rooted at avl_root.
// If the record is not present, return a dummy record 
// with -1 in the status field.
struct record search_record_avl(char uid[MAX_LEN]){
  struct record *temp=avl_root;
   while(temp!=NULL) {
  if (cmp_uid(temp->uid,uid)==0){
    return *temp ;}
  if (cmp_uid(temp->uid,uid)==-1){
    temp=temp->right;
    continue;
  }
  temp=temp->left;}

  struct record dummy_record;
  dummy_record.status=-1;
  return dummy_record;
}
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// Return the head of the linked list of friends
// of the user with uid uid in the AVL tree rooted at avl_root.
// If the corresponding record doesn't exist, return NULL.
struct list_records *get_friends_list_avl(char uid[MAX_LEN])
{while(avl_root!=NULL) {
  if (cmp_uid(avl_root->uid,uid)==0){
    return (avl_root->friends) ;}
  if (cmp_uid(avl_root->uid,uid)==-1){
    avl_root=avl_root->right;
    continue;
  }
  avl_root=avl_root->left;}
  return NULL;
} 

// Delete the record (say n) corresponding to uid from the AVL tree.
// Remove n from the lists of friends of other records
// and release the memory for the linked list nodes.
// Release memory for all the nodes in the list of friends of n.
// Return a copy of the value of the deleted node.
// If the node is not present, return a dummy record
// with -1 in the status field.
struct record delete_record_avl(char uid[MAX_LEN])
{

}//  DIFFRENTTTTTTTTTTTTTTTT THAN BSTTT
// function to destroyy node 
void dnode(struct record* rec){
  if (rec==NULL){
    return;
  }
  struct list_records* temp=rec->friends;
  while (temp != NULL) {
    struct list_records* next = temp->next;
    free_memory(temp);
    temp = next;
  }
  dnode(rec->left);dnode(rec->right);
  free_memory(rec);
}
// Release memory for all BST nodes and their lists of friends.
// Make bst_root points to an empty tree.
void destroy_bst(){
dnode(bst_root);
bst_root=NULL;
num_bst_nodes=0;
}

// Release memory for all AVL nodes and their lists of friends.
// Make avl_root points to an empty tree.
void destroy_avl(){
  dnode(avl_root);
  avl_root=NULL;
  num_avl_nodes=0;
}

// Make users with uids uid1 and uid2 in the BST rooted at bst_root
// friends of each other if they aren't already friends.
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// To make the user with record A a friend of the user with record B, 
// add A to B's list of friends and add B to A's list of friends.
// Return 1 if uid1 and uid2 are already friends before this call.
// Return 0 if they become friends during this call.
struct record* sbsbst(char uid[MAX_LEN]) {
  struct record *temp = bst_root;
  while (temp != NULL) {
    if (cmp_uid(temp->uid, uid) == 0) {
      return temp;
    } else if (cmp_uid(temp->uid, uid) < 0) {
      temp = temp->right;
    } else {
      temp = temp->left;
    }
  }
  return NULL; // add this line to handle the case when uid is not found
}

int make_friends_bst(char uid1[MAX_LEN], char uid2[MAX_LEN]) {
  struct record *a, *b;
  a = sbsbst(uid1);
  b = sbsbst(uid2);

  if (a == NULL || b == NULL) {
    return -1;
  }

  struct list_records *temp = b->friends;
  while (temp != NULL) {
    if (cmp_record(temp->record, a) == 0) {
      return 1;
    }
    temp = temp->next;
  }

  struct list_records *nf = (struct list_records*) allocate_memory(sizeof(struct list_records));
  nf->record = a;
  nf->next = b->friends;
  b->friends = nf;

  nf = (struct list_records*) allocate_memory(sizeof(struct list_records));
  nf->record = b;
  nf->next = a->friends;
  a->friends = nf;

  return 0;
}


// Make users with uids uid1 and uid2 in the AVL tree rooted at avl_root
// friends of each other if they aren't already friends.
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// To make the user with record A a friend of the user with record B, 
// add A to B's list of friends and add B to A's list of friends.
// Return 1 if uid1 and uid2 are already friends before this call.
// Return 0 if they become friends during this call.
struct record* sbsavl(char uid[MAX_LEN]) {
  struct record *temp = avl_root;
  while (temp != NULL) {
    if (cmp_uid(temp->uid, uid) == 0) {
      return temp;
    } else if (cmp_uid(temp->uid, uid) < 0) {
      temp = temp->right;
    } else {
      temp = temp->left;
    }
  }
  return NULL; // add this line to handle the case when uid is not found
}

int make_friends_avl(char uid1[MAX_LEN], char uid2[MAX_LEN]) {
  struct record *a, *b;
  a = sbsavl(uid1);
  b = sbsavl(uid2);

  if (a == NULL || b == NULL) {
    return -1;
  }

  struct list_records *temp = b->friends;
  while (temp != NULL) {
    if (cmp_record(temp->record, a) == 0) {
      return 1;
    }
    temp = temp->next;
  }

  struct list_records *nf = (struct list_records*) allocate_memory(sizeof(struct list_records));
  nf->record = a;
  nf->next = b->friends;
  b->friends = nf;

  nf = (struct list_records*) allocate_memory(sizeof(struct list_records));
  nf->record = b;
  nf->next = a->friends;
  a->friends = nf;

  return 0;
}
