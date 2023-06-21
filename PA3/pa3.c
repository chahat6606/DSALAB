#include "../include/common.h"
#include "pa3.h"

// don't remove these globals
static struct trie_node *trie_root = NULL;
size_t num_msgs = 0;

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

// return the list that contains the history of
// events on msg.
// return NULL if the msg is not present in the trie.
struct list_events* get_history(char msg[MAX_MSG_LEN])
{
  struct trie_node* node = trie_root;
  for(int i = 0; msg[i] != '\0'; i++) {
    struct trie_node* c = node->child;
    while(c != NULL && c->val!= msg[i]) {
      c = c->next;
    }
    if(c == NULL)
      return NULL;
    node = c;
  }
  return node->history;
}

// remove all the delete and the corresponding 
// post events from the history of msg, before returning
// the head of the linked list that contains the history.
// return NULL if the msg is not present in the trie.
// For example, if the following events occur for the msg
// in the given order, where rightmost is the most recent event
// (u1, P), (u1, P), (u2, P), (u1, D), (u1, P), (u3, P), (u4, P), (u3, D), (u1, D)
// Here, u1, u2, u3 are users, P and D corresponds to post and delete
// events
// Then after the cleanup the sequence of events looks like
// (u1, P), (u2, P), (u4, P)
// Notice that the history list only contains the post events
// after the cleanup
// the order of the post events remains the same during the cleanup
struct list_events* get_clean_history(char msg[MAX_MSG_LEN])
{
  struct trie_node* node = trie_root;
  for(int i = 0; msg[i] != '\0'; i++) {
    struct trie_node* c = node->child;
    while(c != NULL && c->val!= msg[i]) {
      c = c->next;
    }
    if(c == NULL)
      return NULL;
    node = c;
  }
  struct list_events *head= node->history;
  if(head == NULL){
    return NULL;
  }
  struct list_events* prev = NULL;
  struct list_events* curr = head;
  struct list_events* node_head=head;
  while(curr!=NULL)
  {
    if(curr->action==DELETED)
    {
      struct list_events* p2=curr;
      struct list_events* cur=curr->next;
      while(cur!=NULL)
      {
        if(curr->record==cur->record && cur->action==POSTED)
        {
          p2->next=cur->next;
          break;
        }
        p2=cur;
        cur=cur->next;
      }
      if(prev==NULL)
      {
        node_head=curr->next;
        curr=curr->next;
      }
      else 
      {
        prev->next=curr->next;
        curr=curr->next;
      }
    }
    else {
    prev=curr;
    curr=curr->next;
    }
  }
  // else{
  // struct list_events* prev = NULL;
  // struct list_events* curr = head;
  // while(curr->next != NULL) {
  //   if(curr->action == DELETED) {
  //     struct list_events* prev2 = curr;
  //     struct list_events* curr2 = curr->next;
  //     while(curr2->next != NULL) {
  //       if(curr2->action==POSTED && curr->record == curr2->record) {
  //         prev2->next = curr2->next;
  //         // printf("(%s, %d) ", curr2->record->name, curr2->action);
  //         // free_memory(curr2);
  //         // free_memory(prev2);
  //         break;
  //       }else{
  //         prev2 = curr2;
  //         curr2 = curr2->next;
  //       }
  //     }
  //     if(prev == NULL) {
  //       head = curr->next;
  //       // printf("(%s, %d)\n", curr->record->name, curr->action);
  //       free_memory(curr);
  //       curr = head;
  //     }
  //     else {
  //       prev->next = curr->next;
  //       free_memory(curr);
  //       curr = prev->next;
  //     }
  //   }
  //   else {
  //     prev = curr;
  //     curr = curr->next;
  //   }
  // }
  // }
  // free_memory(prev);
  // free_memory(curr);
  // c = head;
  // p = 0; d = 0;
  // while(c != NULL) {
  //   printf("(%s %d)", c->record->name, c->action == POSTED);
  //   c = c->next;
  // }
  // printf("\n");
  return node_head;

  
}
// }


// the maximum length of the '\0' terminated message 
// stored in msg is MAX_MSG_LEN
// Insert the msg in the trie rooted at trie_root
// store the reference of the square node
// corresponding to the msg in the linked list r->posts,
// where r is "struct record*"
// A user can post a message several times, say k times, 
// in that case r->posts contain k entries corresponding to
// the message
// r->posts is a stack. The first node in the linked-list
// r->posts corresponds to the lasted post by the user
// A square node in trie also contains the history of post and delete
// events on the corresponding message. The history is stored in
// a linked list n->history, where n is "struct trie_node*"
// n->history is also a stack. The first node in the linked
// list corresponds to the most recent event. The linked list
// node is of type struct event the contains the type of event
// and a reference to the user's record that posted or deleted
// the message
struct trie_node *createNode(char val)
{
  struct trie_node *node = (struct trie_node *)allocate_memory(sizeof(struct trie_node));
  node->val = val;
  node->child = NULL;
  node->next = NULL;
  node->parent = NULL;
  node->history = NULL;
  return node;
}


void post_a_msg(char msg[MAX_MSG_LEN], struct record *r)
{
  if (trie_root == NULL)
  {
    trie_root = createNode('\0');
  }

  struct trie_node *current = trie_root;

  for (int i = 0; msg[i] != '\0'; i++)
  {
    char c = msg[i];
    struct trie_node *child = current->child;
    struct trie_node *prev = NULL;

    while (child != NULL && child->val != c)
    {
      prev = child;
      child = child->next;
    }

    if (child == NULL)
    {
      struct trie_node *newNode = createNode(c);
      newNode->parent = current;
      // printf("%c",c);

      if (prev == NULL)
      {
        current->child = newNode;
      }
      else
      {
        // if(strcmp(msg, "00h ") == 0){
        //   printf("yee\n");
        // }
        prev->next = newNode;
      }

      current = newNode;
    }
    else
    {
      current = child;
    }
  }

  struct list_posts* post = r->posts;
  if(post == NULL) {
    post = (struct list_posts*)allocate_memory(sizeof(struct list_posts));
    post->node = current;
    post->next = NULL;
    r->posts = post;
  }else{
    struct list_posts* newPost = (struct list_posts*)allocate_memory(sizeof(struct list_posts));
    newPost->node = current;
    newPost->next = post;
    r->posts = newPost;
  }

  struct list_events* history = current->history;
  if(history == NULL) {
    history = (struct list_events*)allocate_memory(sizeof(struct list_events));
    history->action = POSTED;
    history->next = NULL;
    history->record = r;
  }else{
    struct list_events* newHistory = (struct list_events*)allocate_memory(sizeof(struct list_events));
    newHistory->action = POSTED;
    newHistory->next = history;
    newHistory->record = r;
    history = newHistory;
  }
  current->history = history;
}
// delete the latest post by a given user
// the first node in linked list r->posts corresponds to
// the latest post
// To delete the latest post the first node in the r->posts
// is removed
// In addition the history of the corresponding square node
// in the trie also gets updated
// a square node also contains the history of post and delete
// events on that message. The history is stored in
// a linked list n->history, where n is "struct trie_node*"
// n->history is also a stack. The first node in the linked
// list corresponds to the most recent event. The linked list
// node is of type struct event the contains the type of event
// and a reference to the user's record that posted or deleted
// the message
// return 1 if a message was deleted
// return 0 id there is nothing to delete
int delete_latest_post(struct record *r)
{
   struct list_posts* post = r->posts;
  if(post == NULL) {
    return 0;
  }
  struct trie_node* node = post->node;
  struct list_events* newHistory = (struct list_events*)allocate_memory(sizeof(struct list_events));
  newHistory->next = node->history;
  newHistory->action = DELETED;
  newHistory->record = r;
  node->history = newHistory;
  r->posts = post->next;
  free_memory(post);
  return 1;
}

void delete_all_posts(struct record *r)
{ 
  struct list_posts* post = r->posts;
  while(post != NULL) {
    delete_latest_post(r);
    post = r->posts;
  }
}

// store the latest post (null terminated) by the user (r) in msg
// return 0 if there is no post by r
// return 1 if msg contains the latest post
// "struct record" contains references to square nodes of
// trie in the linked list posts
// To facilitate the finding of corresponding message from 
// a trie node, a trie node contains a reference to its
// parent node, that is updated when a message is inserted
// in the trie
int read_latest_post(struct record *r, char msg[MAX_MSG_LEN])
{
  struct list_posts* post = r->posts;
  if(post == NULL) {
    return 0;
  }
  struct trie_node* node = post->node;
  int i = 0;
  while(node->parent != NULL) {
    msg[i] = node->val;
    node = node->parent;
    i++;
  }
  // reverse the string msg
  int j = 0;
  int k = i - 1;
  while(j < k) {
    char temp = msg[j];
    msg[j] = msg[k];
    msg[k] = temp;
    j++;
    k--;
  }
  msg[i] = '\0';
  return 1;
}

// Delete all the memory allocated for the trie and
// the linked list that contains the history
void destroy(struct trie_node* t){
  struct trie_node* child = t->child;
  while(child != NULL) {
    // if(child->parent != t){
    //   printf("%d %d\n",child->parent->val, t->val);
    // }
    destroy(child);
    child = child->next;
  }
  
  struct list_events* h = t->history;
  while(h != NULL) {
    struct list_events* next = h->next;
    free_memory(h);
    h = next;
  }
  free_memory(t);
}

void destroy_trie()
{
  destroy(trie_root);
}
