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

/**
 * Link two binomial trees with the same degree. node z's key is smaller than node y's key.
*/
void binomial_link(struct node* &y, struct node* &z) {
    y->parent = z;          // make z y's parent.
    y->sibling = z->child;  // make z's children to be y's siblings.
    z->child = y;           // make y z's child.
    z->degree = z->degree + 1;  // increment z's degree (number of children) by 1.
}

/** Merge two binomial heaps and return their new head. */
struct node *binomial_heap_merge(struct heap* h1, struct heap* h2) {

}

/** Merge two binomial heaps and returns a single one destroying the two heaps in the process. */
struct heap *binomial_heap_union(struct heap* h1, struct heap* h2) {
    struct heap *h = make_binomial_heap();  // new binomial heap to hold the union.
    h->head = binomial_heap_merge(h1, h2);
    if (h->head == nullptr)
        return h;
    // Set up linked list of root list with x as the starying node.
    struct node *prev_x = nullptr;
    struct node *x = h->head;
    struct node *next_x = x->sibling;
    while (next_x != nullptr) {
        if (x->degree != next_x->degree ||      // current and next nodes have different degrees.
                (next_x->sibling != nullptr && next_x->sibling->degree == x->degree)) { // case 1 & 2
            prev_x = x;
            x = next_x;
        }
        else {
            if (x->key <= next_x->key) {    // x's key is smaller than x's next key. CASE 3
                x->sibling = next_x->sibling;
                binomial_link(next_x, x);
            }
            else {                          // x's key is greater than x's next key. CASE 4
                if (prev_x == nullptr) {
                    h->head = next_x;
                }
                else {
                    prev_x->sibling = next_x;
                }
                binomial_link(x, next_x);
                x = next_x;
            }
        }
        next_x = x->sibling;
    }
    return h;
}

/** Insert a node into a binomial heap H. */
void binomial_heap_insert(struct heap* &h, struct node* x) {
    struct heap *hp = make_binomial_heap();
    x->parent = nullptr;
    x->child = nullptr;
    x->sibling = nullptr;
    x->degree = 0;
    hp->head = x;   // set x as the head of this 1-node binomial heap.
    h = binomial_heap_union(h, hp); // unite n-node binomial heap and 1-node binomial heap.
}

// Main function.
int main()
{
    cout << "Hello World!" << endl;
    return 0;
}



























