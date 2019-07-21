#include <iostream>
#include <limits>

using namespace std;

/** Node for Binomial Heap. Each node has a key, pointers to its parent, leftmost child,
 * its sibling (to its right) and a degree(number of children of x).
  */
struct node
{
    int key, degree;
    struct node *child, *sibling, *parent;
};

/** A utility function to create a new node */
struct node *Node(int value)
{
    struct node *temp =  (struct node *)malloc(sizeof(struct node));
    temp->key = value;
    temp->degree = 0;
    temp->child = temp->sibling = temp->parent = nullptr;
    return temp;
}

/** A utility function to do level_order traversal of a bonimial tree. */
void level_order(struct node *root)
{
    if (root != nullptr)
    {
        printf("%d ", root->key);
        level_order(root->child);
        level_order(root->sibling);
    }
}

/** Structure of a binomial heap. */
struct heap {
    struct node *head;
};

/** Make an empty Binomial Heap and return it. */
struct heap *make_binomial_heap() {
    struct heap *h = (struct heap *)malloc(sizeof(struct heap));
    h->head = nullptr;
    return h; // return an empty heap.
}

/** Find minimum node in Binomial Heap and return it. */
struct node* binomial_heap_minimum(struct heap* h) {
    struct node* y = nullptr;
    struct node *x = h->head;
    int minimum = numeric_limits<int>::max();
    while (x != nullptr) {
        if (x->key < minimum) {
            minimum = x->key;
            y = x;
        }
        x = x->sibling;
    }
    return y;
}

/** Link two binomial trees with the same degree. */
void binomial_link(struct node* &y, struct node* &z) {
    y->parent = z;          // make z y's parent.
    y->sibling = z->child;  // make z's children to be y's siblings.
    z->child = y;           // make y z's child.
    z->degree = z->degree + 1;  // increment z's degree (number of children) by 1.
}


// Main function.
int main()
{
    cout << "Hello World!" << endl;
    return 0;
}
