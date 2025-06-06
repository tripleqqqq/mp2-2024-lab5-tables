#pragma once
#include "table_interface.h"
#include <utility>

class RBTreeTable : public ITable {
private:
    enum class Color { RED, BLACK };

    struct Node {

        KeyValuePair data;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(const KeyType& key, const ValueType& value, Color c = Color::RED,
            Node* p = nullptr, Node* l = nullptr, Node* r = nullptr)
            : data(key, value), color(c), left(l), right(r), parent(p) {}
    };

    Node* root;
    Node* nil;

    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nil) y->left->parent = x;
        y->parent = x->parent;

        if (x->parent == nil) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;

        y->left = x;
        x->parent = y;

        operationsCount += 8;
    }

    void rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != nil) x->right->parent = y;
        x->parent = y->parent;

        if (y->parent == nil) root = x;
        else if (y == y->parent->right) y->parent->right = x;
        else y->parent->left = x;

        x->right = y;
        y->parent = x;

        operationsCount += 8;
    }

    Node* minimum(Node* node) const {
        while (node->left != nil) {
            node = node->left;

            operationsCount++;
        }
        return node;
    }

    void transplant(Node* u, Node* v) {
        if (u->parent == nil) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;

        operationsCount += 4;
    }

    Node* findNode(const KeyType& key) const {
        Node* current = root;
        while (current != nil) {
            if (equal(key, current->data.first)) return current;
            current = less(key, current->data.first) ? current->left : current->right;

            operationsCount++;
        }
        return nullptr;
    }

    void fixInsert(Node* z) {

        while (z->parent->color == Color::RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;

                    operationsCount += 4;
                }
                else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotateRight(z->parent->parent);

                    operationsCount += 3;
                }
            }
            else {
                Node* y = z->parent->parent->left;
                if (y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;

                    operationsCount += 4;
                }
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotateLeft(z->parent->parent);

                    operationsCount += 3;
                }
            }
        }
        root->color = Color::BLACK;
    }

    void fixDelete(Node* x) {
        while (x != root && x->color == Color::BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;

                    operationsCount += 4;
                }
                if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->parent;

                    operationsCount += 2;
                }
                else {
                    if (w->right->color == Color::BLACK) {
                        w->left->color = Color::BLACK;
                        w->color = Color::RED;
                        rotateRight(w);
                        w = x->parent->right;

                        operationsCount += 4;
                    }
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->right->color = Color::BLACK;
                    rotateLeft(x->parent);
                    x = root;

                    operationsCount += 5;
                }
            }
            else {
                Node* w = x->parent->left;
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rotateRight(x->parent);
                    w = x->parent->left;

                    operationsCount += 4;
                }
                if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->parent;

                    operationsCount += 2;
                }
                else {
                    if (w->left->color == Color::BLACK) {
                        w->right->color = Color::BLACK;
                        w->color = Color::RED;
                        rotateLeft(w);
                        w = x->parent->left;

                        operationsCount += 4;
                    }
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->left->color = Color::BLACK;
                    rotateRight(x->parent);
                    x = root;

                    operationsCount += 5;
                }
            }
        }
        x->color = Color::BLACK;
    }

    void clear(Node* node) {

        if (node != nil) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

public:
    RBTreeTable() {
        nil = new Node(KeyType(), ValueType(), Color::BLACK);

        root = nil;
    }

    ~RBTreeTable() {
        clear(root);
        delete nil;
    }

    void insert(const KeyType& key, const ValueType& value) override {
        Node* z = new Node(key, value);
        Node* y = nil;
        Node* x = root;

        while (x != nil) {
            y = x;
            x = less(z->data.first, x->data.first) ? x->left : x->right;

            operationsCount++;
        }

        z->parent = y;
        if (y == nil) root = z;
        else if (less(z->data.first, y->data.first)) y->left = z;
        else y->right = z;

        z->left = nil;
        z->right = nil;
        z->color = Color::RED;
        fixInsert(z);
    }

    bool contains(const KeyType& key) const override {
        return findNode(key) != nullptr;
    }

    ValueType get(const KeyType& key) const override {
        Node* node = findNode(key);
        if (node == nullptr) throw std::runtime_error("Key not found");
        return node->data.second;
    }

    void remove(const KeyType& key) override {
        Node* z = findNode(key);
        if (z == nullptr) return;

        Node* y = z;
        Node* x;
        Color y_original_color = y->color;

        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        }
        else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            }
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        operationsCount += 10;

        if (y_original_color == Color::BLACK) {
            fixDelete(x);
        }
    }
};