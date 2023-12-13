#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#define M 5

// the node struct itself
struct Node
{
    std::vector<int> keys;
    std::vector<Node*> children;
    bool is_leaf;
    Node(bool leaf = true) : is_leaf(leaf) {}
};

// class to manage the root node pointer (mainly)
class BTree
{
  public:
    BTree() : root(nullptr) {}

    // inserts a key in the tree, accounting
    //  for splits as needed
    void Insert(int key)
    {
        if (!root)
        {
            // no root, make a new one
            root = new Node(true);
            root->keys.push_back(key);
        } else
        {
            // if we need space, split
            if (root->keys.size() == M - 1)
            {
                Node* new_root = new Node(false);
                new_root->children.push_back(root);
                split(new_root, 0);
                root = new_root;
            }
            // insert the key
            insert(root, key);
        }
    }

    // Writes a DOT file for visualizing the tree
    //  to debug, and ensure structure is correct
    void WriteFile(const std::string& filename)
    {
        std::ofstream file(filename);
        file << "digraph BTree {\n";
        file << "\tnode [shape=record];\n";
        std::queue<Node*> nodes;
        nodes.push(root);
        while (!nodes.empty())
        {
            Node* current = nodes.front();
            nodes.pop();
            writeNode(file, current);
            for (Node* child : current->children) { nodes.push(child); }
        }
        file << "}\n";
        file.close();
    }

    // serches a given range recursively, starting from the root
    std::vector<int> SearchRange(int start, int end)
    {
        std::vector<int> result;
        searchRangeRecurse(root, start, end, result);
        return result;
    }

  private:
    Node* root;

    void insert(Node* node, int key)
    {
        int i = node->keys.size() - 1;

        // if we're a leaf, insert here
        if (node->is_leaf)
        {
            node->keys.push_back(0); // make space for the new key
            // find the key's position
            while (i >= 0 && key < node->keys[ i ])
            {
                node->keys[ i + 1 ] = node->keys[ i ];
                i--;
            }
            node->keys[ i + 1 ] = key;
        } else
        {   // find the child node to insert into, splitting when needed
            while (i >= 0 && key < node->keys[ i ]) { i--; }
            i++;
            if (node->children[ i ]->keys.size() == M - 1)
            {
                split(node, i);
                if (key > node->keys[ i ]) { i++; }
            }
            // insert the key in the child
            insert(node->children[ i ], key);
        }
    }

    // Splits a node
    void split(Node* parent, int index)
    {
        // get child to split + new node
        Node* node = parent->children[ index ];
        Node* new_node = new Node(node->is_leaf);
        // move median to the parent
        parent->keys.insert(parent->keys.begin() + index, node->keys[ M / 2 ]);
        parent->children.insert(parent->children.begin() + index + 1, new_node);
        // split the keys into the children
        new_node->keys.assign(node->keys.begin() + (M / 2 + 1), node->keys.end());
        node->keys.resize(M / 2);
        // if we're not a leaf, split the children too
        if (!node->is_leaf)
        {
            new_node->children.assign(
                node->children.begin() + (M / 2 + 1), node->children.end());
            node->children.resize(M / 2 + 1);
        }
    }

    void writeNode(std::ofstream& file, Node* node)
    {
        file << "\tNode" << node << " [label=\"";
        for (size_t i = 0; i < node->keys.size(); i++)
        {
            file << "<" << i << "> " << node->keys[ i ];
            if (i < node->keys.size() - 1) { file << " | "; }
        }
        file << "\"];\n";

        for (size_t i = 0; i < node->children.size(); i++)
        {
            file << "\tNode" << node << ":" << i << " -> Node"
                    << node->children[ i ] << ";\n";
        }
    }

    void searchRangeRecurse(Node* node, int start, int end, std::vector<int>& result)
    {
        if (!node) return;

        ulong i = 0;
        // find the first position within range
        while (i < node->keys.size() && start > node->keys[ i ]) { i++; }

        // if we're still within the range of the node's keys
        if (i < node->keys.size())
        {   // append children while they're within the range
            while (i < node->keys.size() && node->keys[ i ] <= end)
            {
                result.push_back(node->keys[ i ]);
                i++;
            }
        }

        // if there's children, search those too
        if (!node->is_leaf)
        {
            for (ulong j = 0; j <= i; j++)
            {
                searchRangeRecurse(node->children[ j ], start, end, result);
            }
        }
    }
};

int main()
{
    BTree btree;

    // get n from user
    int n;
    std::cout << "\nn = ";
    std::cin >> n;

    // generate numbers + insert
    for (int i = 0; i < n; i++)
    {
        int key = rand() % (3 * n + 1);
        btree.Insert(key);
    }

    // write DOT file (for debugging)
    btree.WriteFile("btree.dot");

    // search range
    std::vector<int> keysInRange = btree.SearchRange(n, 2 * n);
    std::cout << "Keys in the range [" << n << ", " << 2 * n << "]: ";
    for (int key : keysInRange) std::cout << key << " ";
    std::cout << "\n";

    return 0;
}
