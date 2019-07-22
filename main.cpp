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

/**
 * @brief in_order
 * This procedure prints nodes in a binomial tree.
 * @param root the root of the binomial tree.
 */
void in_order(struct node *root)
{
    while (root != nullptr) {
        cout<<root->key<<" ";
        in_order(root->child);
        if (root->parent == nullptr)
            cout<<"\n";
        root = root->sibling;
    }
}

/**
 * @brief print_binomial_heap
 * This procedure prints all the nodes in a binomial heap.
 * @param h the binomial heap.
 */
void print_binomial_heap(struct heap* h) {
    in_order(h->head);   // call lev
}

/**
 * @brief binomial_heap_minimum
 * The following procedure finds the minimum node in a Binomial Heap and return a pointer to the node.
 * @param h the binomial Heap
 * @return a pointer to the minimum node in the heap.
 */
struct node* binomial_heap_minimum(struct heap* h) {
    struct node *y = nullptr;
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
 * @brief binomial_link
 * The following procedure links two binomial heaps with the same degree.
 * Note node z's key is smaller than node y's key.
 * @param y the root of the first binomial heap.
 * @param z the root of the second binomial heap.
 */
void binomial_link(struct node* &y, struct node* &z) {
    y->parent = z;          // make z y's parent.
    y->sibling = z->child;  // make z's children to be y's siblings.
    z->child = y;           // make y z's child.
    z->degree = z->degree + 1;  // increment z's degree (number of children) by 1.
}

/**
 * @brief binomial_heap_merge
 * The following procedure merges two binomial heaps and returns a pointer to the head of the new
 * binomial heap.
 * @param h1 binomial heap1
 * @param h2 binomial heap2
 * @return the head of the new binomial heap.
 */
struct node *binomial_heap_merge(struct heap* h1, struct heap* h2) {
    struct heap *h = make_binomial_heap();
    struct node *curr1 = nullptr;   // Pointer to traverse heap1.
    struct node *curr2 = nullptr;   // Pointer to traverse heap2.
    if (h1->head == nullptr) {  // heap1 is empty, set new head to heap2.
        h->head = h2->head;
        return h->head;
    }
    else if (h2->head == nullptr) { // heap2 is empty, set new head to heap1.
        h->head = h1->head;
        return h->head;
    }

    // At this point, both h1 and h2 are non-empty.
    if (h2->head->key < h1->head->key) {    // heap2 root is smaller than heap1 root.
        h->head = h2->head;
        curr2 = h2->head->sibling;
        curr1 = h1->head;
    }
    else {
        h->head = h1->head;
        curr1 = h1->head->sibling;
        curr2 = h2->head;
    }
    struct node *curr = h->head;
    while (curr1 != nullptr && curr2 != nullptr) {  // do this while none of the pointers is null.
        if (curr1->key > curr2->key) {  // curr2 element is added to new heap.
            curr->sibling = curr2;
            curr = curr->sibling;
            curr2 = curr2->sibling;
        }
        else {                          // curr1 element is added to new heap.
            curr->sibling = curr1;
            curr = curr->sibling;
            curr1 = curr1->sibling;
        }
    }
    // In case of different number of trees.
    struct node *tail = nullptr;
    if (curr1 == nullptr)
        tail = curr2;
    else {
        tail = curr1;
    }
    while (tail != nullptr) {   // add remaining nodes to new heap.
        curr->sibling = tail;
        curr = curr->sibling;
        tail = tail->sibling;
    }
    return h->head;             // return new binomial heap head.
}

/**
 * @brief binomial_heap_union
 * The following procedure unites two binomial heaps and returns a pointer to a single binomial heap
 * destroying the two heaps in the process.
 * @param h1 binomial heap1
 * @param h2 binomial heap2
 * @return new binomial heap with combined contents of h1 & h2.
 */
struct heap *binomial_heap_union(struct heap* &h1, struct heap* &h2) {
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

/**
 * @brief binomial_heap_insert
 * The following procedure inserts a node into a binomial heap H.
 * @param h the Binomial Heap.
 * @param x the node to be inserted.
 */
void binomial_heap_insert(struct heap* &h, struct node* x) {
    struct heap *hp = make_binomial_heap();
    x->parent = nullptr;
    x->child = nullptr;
    x->sibling = nullptr;
    x->degree = 0;
    hp->head = x;   // set x as the head of this 1-node binomial heap.
    h = binomial_heap_union(h, hp); // unite n-node binomial heap and 1-node binomial heap.
}

/**
 * @brief binomial_heap_extract_min
 * The following procedure extracts the node with the minimum key from binomialheap H
 * and returns a pointer to the extracted node.
 * @param h the Binomial heap
 * @return the node with minimum key.
 */
struct node *binomial_heap_extract_min(struct heap* &h) {
    // Find root x with minimum key in root list of h.
    struct node *curr = h->head;
    struct node *x = binomial_heap_minimum(h);
    // Remove x from root list of h.
    curr = h->head;
    struct node *prev = nullptr;
    while (curr != nullptr) {
        if (curr->sibling == x) {   // find x's previous node.
            prev = curr;
        }
        curr = curr->sibling;
    }
    prev->sibling = x->sibling;

    struct heap *hp = make_binomial_heap();
    // Reverse list of x's children.
    struct node *child = x->child;
    struct node *pr = nullptr, *nxt = nullptr;
    while (child != nullptr) {
        nxt = child->sibling;   // store next
        child->sibling = pr;    // reverse current node's pointer.
        // Move pointers one position ahead.
        pr = child;
        child = nxt;
    }
    hp->head = pr;  // Set hp->head to point to head of resulting list.

    h = binomial_heap_union(h, hp);
    return x;
}

/**
 * @brief binomial_heap_decrease_key
 * The following procedure decreases the key of a node x in a binomial heap H to a new value k.
 * It signals an error ifkis greater thanx’s current key
 * @param h the Binomial Heap
 * @param x the node whose key is to be decreased.
 * @param k the new key for node x.
 */
void binomial_heap_decrease_key(struct heap* &h, struct node* x, int k) {

}

/**
 * @brief binomial_heap_delete
 * The following procedure deletes a node from a binomial Heap and modifies the heap in question.
 * @param h the binomial heap
 * @param x the node to be deleted.
 */
void binomial_heap_delete(struct heap* &h, struct node* x) {
    binomial_heap_decrease_key(h, x, numeric_limits<int>::min());   // set node x's key to -ve infinity.
    binomial_heap_extract_min(h);   // remove smallest node from Heap.
}

// Main function.
int main()
{
    cout << "Binomial Heap program!" << endl;
    struct heap *h = make_binomial_heap();
    binomial_heap_insert(h, Node(12));
    binomial_heap_insert(h, Node(7));
    binomial_heap_insert(h, Node(25));
    binomial_heap_insert(h, Node(15));
    binomial_heap_insert(h, Node(28));
    binomial_heap_insert(h, Node(33));
    binomial_heap_insert(h, Node(41));

    cout<<"Minimum node in binomial heap is: "<<binomial_heap_minimum(h)->key<<endl;
    print_binomial_heap(h);

    cout<<"deleting smallest node....."<<endl;
    binomial_heap_delete(h, binomial_heap_minimum(h));
    cout<<"Binomial heap after deleting smallest node: "<<endl;
    print_binomial_heap(h);

    return 0;
}
