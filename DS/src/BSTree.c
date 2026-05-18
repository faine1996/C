#include <stdlib.h> /* malloc, free */
#include "../inc/BSTree.h"

typedef struct Node Node;

struct Node
{
    void* m_item;
    Node* m_left;
    Node* m_right;
    Node* m_parent;
};

struct BSTree
{
    Node m_sentinel;
    TreeComparator m_less;
};

/* --- Helper Function Declarations --- */

static Node* CreateNode(void* _item, Node* _parent);
static void DestroySubtree(Node* _root, void (*_destroyer)(void*));
static Node* GetMostLeft(Node* _root);
static Node* GetMostRight(Node* _root);
static void RemoveNode(Node* _node);
static BSTreeItr ForEachPreOrder(Node* _root, TreeActionFunction _action, void* _context);
static BSTreeItr ForEachInOrder(Node* _root, TreeActionFunction _action, void* _context);
static BSTreeItr ForEachPostOrder(Node* _root, TreeActionFunction _action, void* _context);

/* --- Core Functions --- */

BSTree* BSTreeCreate(TreeComparator _less)
{
    BSTree* tree = NULL;

    if (NULL == _less)
    {
        return NULL;
    }

    tree = (BSTree*)malloc(sizeof(BSTree));
    if (NULL == tree)
    {
        return NULL;
    }

    tree->m_less = _less;
    tree->m_sentinel.m_left = NULL;
    tree->m_sentinel.m_right = NULL;
    tree->m_sentinel.m_parent = NULL;
    tree->m_sentinel.m_item = NULL;

    return tree;
}

void BSTreeDestroy(BSTree** _tree, void (*_destroyer)(void*))
{
    if (NULL == _tree || NULL == *_tree)
    {
        return;
    }

    DestroySubtree((*_tree)->m_sentinel.m_left, _destroyer);
    free(*_tree);
    *_tree = NULL;
}

BSTreeItr BSTreeInsert(BSTree* _tree, void* _item)
{
    Node* current = NULL;
    Node* parent = NULL;
    int res = 0;

    if (NULL == _tree)
    {
        return NULL;
    }

    current = _tree->m_sentinel.m_left;
    parent = &(_tree->m_sentinel);

    while (NULL != current)
    {
        parent = current;
        res = _tree->m_less(_item, current->m_item);
        if (1 == res) /* _item < current->m_item */
        {
            current = current->m_left;
        }
        else if (-1 == res) /* current->m_item < _item */
        {
            current = current->m_right;
        }
        else /* Duplicate */
        {
            return (BSTreeItr)&(_tree->m_sentinel);
        }
    }

    current = CreateNode(_item, parent);
    if (NULL == current)
    {
        return (BSTreeItr)&(_tree->m_sentinel);
    }

    if (parent == &(_tree->m_sentinel) || 1 == _tree->m_less(_item, parent->m_item))
    {
        parent->m_left = current;
    }
    else
    {
        parent->m_right = current;
    }

    return (BSTreeItr)current;
}

BSTreeItr BSTreeItrBegin(const BSTree* _tree)
{
    Node* node = NULL;
    if (NULL == _tree)
    {
        return NULL;
    }

    node = GetMostLeft((Node*)_tree->m_sentinel.m_left);
    return (NULL == node) ? (BSTreeItr)&(_tree->m_sentinel) : (BSTreeItr)node;
}

BSTreeItr BSTreeItrEnd(const BSTree* _tree)
{
    if (NULL == _tree)
    {
        return NULL;
    }
    return (BSTreeItr)&(_tree->m_sentinel);
}

BSTreeItr BSTreeItrNext(BSTreeItr _it)
{
    Node* node = (Node*)_it;
    Node* parent = NULL;

    if (NULL == node)
    {
        return NULL;
    }

    if (NULL != node->m_right)
    {
        return (BSTreeItr)GetMostLeft(node->m_right);
    }

    parent = node->m_parent;
    while (NULL != parent && node == parent->m_right)
    {
        node = parent;
        parent = parent->m_parent;
    }

    return (BSTreeItr)parent;
}

BSTreeItr BSTreeItrPrev(BSTreeItr _it)
{
    Node* node = (Node*)_it;
    Node* parent = NULL;

    if (NULL == node)
    {
        return NULL;
    }

    if (NULL != node->m_left)
    {
        return (BSTreeItr)GetMostRight(node->m_left);
    }

    parent = node->m_parent;
    while (NULL != parent && node == parent->m_left)
    {
        node = parent;
        parent = parent->m_parent;
    }

    return (NULL == parent) ? _it : (BSTreeItr)parent;
}

void* BSTreeItrGet(BSTreeItr _it)
{
    Node* node = (Node*)_it;
    if (NULL == node || NULL == node->m_parent) /* Sentinel has NULL parent */
    {
        return NULL;
    }
    return node->m_item;
}

void* BSTreeItrRemove(BSTreeItr _it)
{
    Node* node = (Node*)_it;
    void* item = NULL;

    if (NULL == node || NULL == node->m_parent)
    {
        return NULL;
    }

    item = node->m_item;
    RemoveNode(node);
    return item;
}

BSTreeItr BSTreeForEach(const BSTree* _tree, BSTreeTraversalMode _mode, TreeActionFunction _action, void* _context)
{
    if (NULL == _tree || NULL == _action)
    {
        return NULL;
    }

    switch (_mode)
    {
        case BSTREE_TRAVERSAL_PREORDER:
            return ForEachPreOrder(_tree->m_sentinel.m_left, _action, _context);
        case BSTREE_TRAVERSAL_INORDER:
            return ForEachInOrder(_tree->m_sentinel.m_left, _action, _context);
        case BSTREE_TRAVERSAL_POSTORDER:
            return ForEachPostOrder(_tree->m_sentinel.m_left, _action, _context);
        default:
            return (BSTreeItr)&(_tree->m_sentinel);
    }
}

/* --- Helper Function Definitions --- */

static Node* CreateNode(void* _item, Node* _parent)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (NULL == node)
    {
        return NULL;
    }
    node->m_item = _item;
    node->m_parent = _parent;
    node->m_left = NULL;
    node->m_right = NULL;
    return node;
}

static void DestroySubtree(Node* _root, void (*_destroyer)(void*))
{
    if (NULL == _root)
    {
        return;
    }
    DestroySubtree(_root->m_left, _destroyer);
    DestroySubtree(_root->m_right, _destroyer);
    if (NULL != _destroyer)
    {
        _destroyer(_root->m_item);
    }
    free(_root);
}

static Node* GetMostLeft(Node* _root)
{
    if (NULL == _root)
    {
        return NULL;
    }
    while (NULL != _root->m_left)
    {
        _root = _root->m_left;
    }
    return _root;
}

static Node* GetMostRight(Node* _root)
{
    if (NULL == _root)
    {
        return NULL;
    }
    while (NULL != _root->m_right)
    {
        _root = _root->m_right;
    }
    return _root;
}

static void RemoveNode(Node* _node)
{
    Node* parent = _node->m_parent;
    Node* child = NULL;
    Node* successor = NULL;

    if (NULL != _node->m_left && NULL != _node->m_right)
    {
        successor = GetMostLeft(_node->m_right);
        _node->m_item = successor->m_item;
        RemoveNode(successor);
        return;
    }

    child = (NULL != _node->m_left) ? _node->m_left : _node->m_right;

    if (parent->m_left == _node)
    {
        parent->m_left = child;
    }
    else
    {
        parent->m_right = child;
    }

    if (NULL != child)
    {
        child->m_parent = parent;
    }

    free(_node);
}

static BSTreeItr ForEachPreOrder(Node* _root, TreeActionFunction _action, void* _context)
{
    BSTreeItr res = NULL;
    if (NULL == _root)
    {
        return NULL;
    }
    if (0 == _action(_root->m_item, _context))
    {
        return (BSTreeItr)_root;
    }
    res = ForEachPreOrder(_root->m_left, _action, _context);
    if (NULL != res)
    {
        return res;
    }
    return ForEachPreOrder(_root->m_right, _action, _context);
}

static BSTreeItr ForEachInOrder(Node* _root, TreeActionFunction _action, void* _context)
{
    BSTreeItr res = NULL;
    if (NULL == _root)
    {
        return NULL;
    }
    res = ForEachInOrder(_root->m_left, _action, _context);
    if (NULL != res)
    {
        return res;
    }
    if (0 == _action(_root->m_item, _context))
    {
        return (BSTreeItr)_root;
    }
    return ForEachInOrder(_root->m_right, _action, _context);
}

static BSTreeItr ForEachPostOrder(Node* _root, TreeActionFunction _action, void* _context)
{
    BSTreeItr res = NULL;
    if (NULL == _root)
    {
        return NULL;
    }
    res = ForEachPostOrder(_root->m_left, _action, _context);
    if (NULL != res)
    {
        return res;
    }
    res = ForEachPostOrder(_root->m_right, _action, _context);
    if (NULL != res)
    {
        return res;
    }
    if (0 == _action(_root->m_item, _context))
    {
        return (BSTreeItr)_root;
    }
    return NULL;
}