#ifndef RBT_HPP
#define RBT_HPP

#include <functional>
#include <fstream>

namespace RBT {

enum class color_t { RED, BLACK };

template <typename KeyT>
struct Node
{
private:
    KeyT key_;
    color_t color_;
    Node *parent_;
    Node *left_;
    Node *right_;

public:
    Node(KeyT key, color_t color = color_t::RED, Node *parent = nullptr, Node *left = nullptr, Node *right = nullptr)
        : key_(key), color_(color), parent_(parent), left_(left), right_(right) {}

    ~Node()
    {
        delete left_;
        delete right_;
    }
    
    template <typename, typename>
    friend class SearchTree;
};


template <typename KeyT, typename Comp = std::less<KeyT>>
class SearchTree {

    using iterator = Node<KeyT>*;
    Node<KeyT> *top_ = nullptr;

    void balance(Node<KeyT> *node);
    void gdumpNode(std::ofstream &ofs, Node<KeyT> *node) const;
    void LLRot(Node<KeyT> *node);
    void RRRot(Node<KeyT> *node);

public:
    iterator lower_bound(KeyT key) const;
    iterator upper_bound(KeyT key) const;
    int distance(iterator fst, iterator snd) const;

    void insert(KeyT key);
    void erase(KeyT key);
    void gdump() const;
};

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::insert(KeyT key)
{
    // std::cout << "here1" << std::endl;
    if (!top_) {
        std::cout << "top created" << std::endl;
        top_ = new Node<KeyT>{key, color_t::BLACK};
        return;
    }
    // std::cout << "here2" << std::endl;

    Node<KeyT> *current = top_;
    Node<KeyT> *parent = nullptr;

    while (current)
    {
        parent = current;

        if (Comp{}(key, current->key_))
            current = current->left_;
        else
            current = current->right_;
    }

    // std::cout << "here3" << std::endl;
    if (Comp{}(key, parent->key_))
    {
        // std::cout << "here4" << parent << std::endl;
        parent->left_ = new Node<KeyT>{key, color_t::RED, parent};
        std::cout << "here5" << parent << std::endl;
        balance(parent->left_);
        //  std::cout << "here6" << parent << std::endl;
    }
    else
    {
        parent->right_ = new Node<KeyT>{key, color_t::RED, parent};
        balance(parent->right_);
    }
}

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::LLRot(Node<KeyT> *node)
{
    Node<KeyT> *p = node->parent_;
    Node<KeyT> *g = p->parent_;

    if (g->parent_)
    {
        if (g == g->parent_->left_)
            g->parent_->left_ = p;
        else
            g->parent_->right_ = p;
            
        g->left_ = p->right_;
        p->right_ = g;
        p->color_ = color_t::BLACK;
        g->color_ = color_t::RED;
    }
}

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::RRRot(Node<KeyT> *node)
{
    Node<KeyT> *p = node->parent_;
    Node<KeyT> *g = p->parent_;

    if (g->parent_)
    {
        if (g == g->parent_->left_)
            g->parent_->left_ = p;
        else
            g->parent_->right_ = p;
            
        g->right_ = p->left_;
        p->left_ = g;
        p->color_ = color_t::BLACK;
        g->color_ = color_t::RED;
    }
}

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::balance(Node<KeyT> *node)
{
    Node<KeyT> *p = node->parent_; // parent
    if (!p) return;
    if (p->color_ == color_t::BLACK) return;

    Node<KeyT> *g = p->parent_; // grandparent

    Node<KeyT> *u = nullptr;

    if (g->left_ != p)
        u = g->left_; // uncle
    else
        u = g->right_; // uncle

    if (u && u->color_ == color_t::RED)
    {
        p->color_ = color_t::BLACK;
        u->color_ = color_t::BLACK;

        if (g->parent_)
            g->color_ = color_t::RED;
        else
            g->color_ = color_t::BLACK;

        std::cout << "here7" << g << std::endl;
        balance(g);
    }

    else if (!u || u->color_ == color_t::BLACK)
    {
        if ((p == g->left_ && node == p->left_))
        {
            LLRot(node);
        }
        else if (p == g->right_ && node == p->right_)
        {
            RRRot(node);
        }
        else if (p == g->left_ && node == p->right_)
        {
            node->parent_ = g;
            g->left_ = node;

            p->right_ = node->left_;

            p->parent_ = node;
            node->left_ = p;

            LLRot(p);

        }
        else if (p == g->right_ && node == p->left_)
        {
            node->parent_ = g;
            g->right_ = node;

            p->left_ = node->right_;

            p->parent_ = node;
            node->right_ = p;

            RRRot(p);
        }
    }
}

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::gdump() const
{
    static int counter = 0;
    std::string graphviz_file = "graphviz/tree" + std::to_string(counter) + ".dot";
    std::ofstream ofs(graphviz_file);
    if (!ofs) throw std::runtime_error("Failed to open graphviz_file file");
    ofs << "digraph G {\n";
    ofs << "    node [shape=circle, style=filled];\n";
    gdumpNode(ofs, top_);
    ofs << "}\n";
    ofs.close();

    std::string dot_cmd = "dot -Tsvg graphviz/tree" + std::to_string(counter) + ".dot -o graphviz/tree" + std::to_string(counter + 1) + ".svg";
    system(dot_cmd.c_str());
    std::string rm_cmd = "rm graphviz/tree" + std::to_string(counter) + ".dot";
    system(rm_cmd.c_str());
    ++counter;
}

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::gdumpNode(std::ofstream &ofs, Node<KeyT> *node) const
{
    if (!node) return;

    std::string color_attr = (node->color_ == color_t::RED) ? 
        "fillcolor=lightcoral, fontcolor=white" : 
        "fillcolor=lightgray, fontcolor=black";

    ofs << "    \"" << node << "\" [label=\"" << node->key_ << "\", " << color_attr << "];\n";
    
    if (node->left_) {
        ofs << "    \"" << node << "\" -> \"" << node->left_ << "\";\n";
        gdumpNode(ofs, node->left_);
    }
    if (node->right_) {
        ofs << "    \"" << node << "\" -> \"" << node->right_ << "\";\n";
        gdumpNode(ofs, node->right_);
    }
}

} // namespace RBT



#endif // RBT_HPP