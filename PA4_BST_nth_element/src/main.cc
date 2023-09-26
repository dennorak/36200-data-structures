#include <iostream>

// count is used to track depth in n-th element searches
int count = 0;
// tree node structure, self-explanatory
struct Node
{
    Node* left;
    Node* right;
    int data;
};

// Build a tree using the node + data provided
void buildTreeNodes(Node* root, int* data, size_t s_data)
{
    // if a tree can't be made, set the current node to data
    if (s_data < 3)
    {
        root->data = *data;
        return;
    }
    // set the node to the median value
    root->data = data[s_data/2];
    // create nodes for left/right, recursively build them
    root->left =  new Node();
    root->right = new Node();
    buildTreeNodes(root->left, data, s_data/2); // gets first half of array
    buildTreeNodes(root->right, &data[s_data/2+1], s_data/2); // gets second half
};

// gets the n-th smallest elements
Node* nthTreeElement(Node* node, int& n)
{
    // if left
    if (node->left != nullptr)
    {
        // check the left first, it's smaller
        Node* left = nthTreeElement(node->left, n);
        if (left != nullptr) return left;
    }

    // update count + check if we're already on the
    //      n-th smallest element
    count++;
    if (count == n)
        return node;

    // if right
    if (node->right != nullptr)
    {
        // check the right
        Node* right = nthTreeElement(node->right, n);
        if (right != nullptr) return right;
    }
    // we're all the way right, and we 
    //      didn't find it; go up one layer
    return nullptr;
}

// print the tree
void printTree(Node* node)
{
    if (node->left  != nullptr) printTree(node->left);
    std::cout << node->data << " ";
    if (node->right != nullptr) printTree(node->right);
};

// print the tree (annotated with path)
void printTreeAnnotated(Node* node, std::string path="root")
{
    if (node->left  != nullptr) printTreeAnnotated(node->left, path + "->left");
    std::cout << node->data << " Path:" << path << '\n';
    if (node->right != nullptr) printTreeAnnotated(node->right, path + "->right");
};

int main()
{
    // define elements
    int elements[]
        { 6, 17, 20, 41, 45, 52, 57, 65, 
        71, 76, 79, 87, 92, 95, 99 };
    // define sizeof elements
    int s_elements = sizeof(elements) / sizeof(int);

    // create root node + start building
    Node root;
    buildTreeNodes(&root, &elements[0], s_elements);

    // print each sorted array
    std::cout << "Tree with path:\n";
    printTreeAnnotated(&root);
    std::cout << "Tree (inline for selection):\n";
    printTree(&root);

    // get n from user
    int n;
    std::cout << "\nn = ";
    std::cin >> n;

    // print out n-th element
    Node* nth = nthTreeElement(&root, n);
    std::cout << "e = " << nth->data << "\n";
}