#ifndef RANKTREE_H
#define RANKTREE_H

template<class Key, class Val>
class RankTree {
public:
    typedef struct Node {
        Node *daddy;
        Node *left;
        Node *right;
        Key key;
        Val val;
        int height;
        int rank;
    } Node;

    // Tree functions
    RankTree() : m_root(nullptr), m_size(0) {};
    RankTree &operator=(const RankTree<Key, Val> &other) = default;
    ~RankTree();
    Node *Find(const Key &key) const;
    void SetRoot(Node *node);
    Node *GetRoot();
    bool IsEmpty() const;
    int GetSize() const;
    void IncSize();
    void DecSize();
    Node *successor(Node *node);

    // Node functions
    Node *InitNode(const Key &key, const Val &val);
    void DeleteNode(Node *node);
    static int getHeight(const Node *node);
    static int CalcBF(const Node *node);
    Node *GetClosestFromAbove(const Node *node) const;
    Node *GetClosestFromBelow(const Node *node) const;
    bool IsLeaf(const Node *node) const;
    bool IsDaddyOfTwo(const Node *node) const;
    Node *GetSmallestVal() const;
    Node *GetBiggestVal() const;
    Node* GetClosestFromBelow(const Key& key) const;
    void InitRanks(Node* node);
    void AddToRanks(Node* node, int amount);
    int GetSumOfRanks(Node* node);

    // Roll functions
    void RollLL(Node *grandpa);
    void RollRR(Node *grandpa);
    void RollLR(Node *grandpa);
    void RollRL(Node *grandpa);

    // Insert functions
    Node *Insert(const Key &key, const Val &val);
    void CheckNRoll(Node *node);
    void PickRoll(Node *daddy, int BF);

    // Delete functions
    void DeleteFullTree(Node *node);
    void Delete(const Key &key);
    //void Delete(const Key &key);
    void BalanceTreeAfterDeletion(Node *node);

private:
    Node *m_root;
    int m_size;
};

template<class Key, class Val>
void RankTree<Key, Val>::Delete(const Key &key) {
    Node *node = Find(key);
    if (!node) {
        return;
    }
    //case 1: node is a leaf
    if (IsLeaf(node)) {
        if (!node->daddy) { //meaning there is only root
            m_root = nullptr;
            DeleteNode(node);
            DecSize();
            return;
        } else { //leaf and not the root
            Node *dad = node->daddy;
            if (dad->left == node) { //set dad son to null
                dad->left = nullptr;
            } else {
                dad->right = nullptr;
            }
            DeleteNode(node);
            BalanceTreeAfterDeletion(dad);
            DecSize();
            return;
        }
    }
    //case 2: node has right son and no left son
    if (!node->left && node->right) {
        if (!node->daddy) { //if node is root
            m_root = node->right;
            node->right->daddy = nullptr;
            m_root->rank = node->rank;
            DeleteNode(node);
            DecSize();
            return;
        } else { //node is not the root
            Node *dad = node->daddy;
            node->right->daddy = dad;
            if (dad->left == node) { //set dad son to null
                dad->left = node->right;
            } else {
                dad->right = node->right;
            }
            dad->rank = node->rank;
            DeleteNode(node);
            BalanceTreeAfterDeletion(dad);
            DecSize();
            return;
        }
    }
    //case 3: node has left son and no right son
    if (node->left && !node->right) {
        if (!node->daddy) { //if node is root
            m_root = node->left;
            node->left->daddy = nullptr;
            m_root->rank = node->rank;
            DeleteNode(node);
            DecSize();
            return;
        } else { //node is not the root
            Node *dad = node->daddy;
            node->left->daddy = dad;
            if (dad->left == node) { //set dad son to null
                dad->left = node->left;
            } else {
                dad->right = node->left;
            }
            dad->rank = node->rank;
            DeleteNode(node);
            BalanceTreeAfterDeletion(dad);
            DecSize();
            return;
        }
    }
        //case 4: node has two sons
    else { // need to check deeply
        Node *closest = successor(node);
        Node *dadOfClosest = closest->daddy;
        if (dadOfClosest->right == closest && closest->right) {
            dadOfClosest->right = closest->right;
            closest->right->daddy = dadOfClosest;
        } else if (dadOfClosest->right == closest && !closest->right) {
            dadOfClosest->right = nullptr;
        } else if (dadOfClosest->left == closest && closest->right) {
            dadOfClosest->left = closest->right;
            closest->right->daddy = dadOfClosest;
        } else {
            dadOfClosest->left = nullptr;
        }
        if (node == GetRoot()) {
            SetRoot(closest);
        }
        closest->right = node->right;
        if (closest->right) {
            closest->right->daddy = closest;
        }
        closest->left = node->left;
        if (closest->left) {
            closest->left->daddy = closest;
        }
        closest->daddy = node->daddy;
        if (closest->daddy) {
            if (closest->daddy->left == node) {
                closest->daddy->left = closest;
            } else {
                closest->daddy->right = closest;
            }
        }
        closest->height = node->height;
        closest->rank = node->rank;
        if (dadOfClosest == node) {
            dadOfClosest = nullptr;
        }
        DeleteNode(node);
        BalanceTreeAfterDeletion(dadOfClosest);
        DecSize();
    }
}

template<class Key, class Val>
void RankTree<Key, Val>::DecSize() {
    this->m_size--;
}

template<class Key, class Val>
void RankTree<Key, Val>::IncSize() {
    this->m_size++;
}

template<class Key, class Val>
typename RankTree<Key, Val>::Node *RankTree<Key, Val>::successor(RankTree::Node *node) {
    if (node->right) {
        // If the node has a right child, find the minimum node in the right subtree
        return GetClosestFromAbove(node);
    } else {
        // Find the nearest ancestor for which the given node is in the left subtree
        Node *successor = nullptr;
        Node *current = m_root;
        while (current != node) {
            if (node->key < current->key) {
                successor = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return successor;
    }
}

template<class Key, class Val>
int RankTree<Key, Val>::GetSize() const {
    return this->m_size;
}

template<class Key, class Val>
typename RankTree<Key, Val>::Node *RankTree<Key, Val>::GetBiggestVal() const {
    Node *node = m_root;
    if (node == nullptr) {
        return nullptr;
    }
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}

template<class Key, class Val>
typename RankTree<Key, Val>::Node *RankTree<Key, Val>::GetSmallestVal() const {
    Node *node = m_root;
    if (node == nullptr) {
        return nullptr;
    }
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

template<class Key, class Val>
void RankTree<Key, Val>::DeleteNode(RankTree::Node *node) {
    if (node == nullptr) {
        return;
    }
    if (node == m_root) {
        m_root = nullptr;
    }
    node->val = nullptr;
    node->daddy = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    delete node;
}

template<class Key, class Val>
int RankTree<Key, Val>::getHeight(const RankTree::Node *node) {
    if (node == nullptr) {
        return -1;
    }
    return 1 + max(getHeight(node->left), getHeight(node->right));
}

template<class Key, class Val>
bool RankTree<Key, Val>::IsDaddyOfTwo(const RankTree::Node *node) const {
    if (node == nullptr) {
        return false;
    }
    return (node->left && node->right);
}

template<class Key, class Val>
RankTree<Key, Val>::~RankTree() {
    DeleteFullTree(m_root);
}

template<class Key, class Val>
void RankTree<Key, Val>::DeleteFullTree(Node *node) {
    if (node != nullptr) {
        DeleteFullTree(node->left);
        DeleteFullTree(node->right);
        if (node == m_root) {
            m_root = nullptr;
        } else if (node->daddy->left == node) {
            node->daddy->left = nullptr;
        } else {
            node->daddy->right = nullptr;
        }
        DeleteNode(node);
        DecSize();
    }
}

template<class Key, class Val>
typename RankTree<Key, Val>::Node *RankTree<Key, Val>::InitNode(const Key &key, const Val &val) {
    Node *node = new Node;
    node->daddy = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    node->key = key;
    node->val = val;
    node->height = 0;
    node->rank = 0;
    return node;
}

template<class Key, class Val>
int RankTree<Key, Val>::CalcBF(const Node *node) {
    int leftHeight = (node->left) ? node->left->height : -1;
    int rightHeight = (node->right) ? node->right->height : -1;
    return leftHeight - rightHeight;
}

template<class Key, class Val>
typename RankTree<Key, Val>::Node *RankTree<Key, Val>::GetClosestFromAbove(const Node *node) const {
    if (node == nullptr) {
        return nullptr;
    }
    Node *curr = node->right;
    if (curr == nullptr) {
        return nullptr;
    }
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr;
}

template<class Key, class Val>
typename RankTree<Key, Val>::Node *RankTree<Key, Val>::GetClosestFromBelow(const Node *node) const {
    if (node == nullptr) {
        return nullptr;
    }
    Node *curr = node->left;
    if (curr == nullptr) {
        return nullptr;
    }
    while (curr->right != nullptr) {
        curr = curr->right;
    }
    return curr;
}

template<class Key, class Val>
typename RankTree<Key, Val>::Node *RankTree<Key, Val>::Find(const Key &key) const {
    if (m_root == nullptr) {
        return nullptr;
    }
    Node *curr = m_root;
    if (curr->key == key) {
        return curr;
    }
    while (curr != nullptr) {
        if (curr->key < key) {
            curr = curr->right;
        } else if (curr->key > key) {
            curr = curr->left;
        } else {
            return curr;
        }
    }
    return nullptr;
}

template<class Key, class Val>
void RankTree<Key, Val>::SetRoot(Node *node) {
    if (!m_root) {
        m_size++;
    }
    this->m_root = node;
}

template<class Key, class Val>
typename RankTree<Key, Val>::Node *RankTree<Key, Val>::GetRoot() {
    return this->m_root;
}

template<class Key, class Val>
bool RankTree<Key, Val>::IsEmpty() const {
    return this->m_root == nullptr;
}

template<class Key, class Val>
void RankTree<Key, Val>::RollLL(Node *grandpa) {
    Node *papa = grandpa->left;
    Node *otherSon = papa->right;
    int papa_rank = papa->rank;
    int grandpa_rank = grandpa->rank;
    papa->left->rank += papa_rank + grandpa_rank;
    papa->rank += grandpa_rank;
    grandpa->rank -= papa_rank;
    papa->right = grandpa;
    if (grandpa->daddy == nullptr) {
        m_root = papa;
        papa->daddy = nullptr;
    } else {
        papa->daddy = grandpa->daddy;
        if (papa->daddy->left == grandpa) {
            grandpa->daddy->left = papa;
        } else {
            grandpa->daddy->right = papa;
        }
    }
    grandpa->daddy = papa;
    if (otherSon) {
        grandpa->left = otherSon;
        otherSon->daddy = grandpa;
    } else {
        grandpa->left = nullptr;
    }
    grandpa->height = getHeight(grandpa);
    papa->height = getHeight(papa);
    if (grandpa == m_root) {
        m_root = papa;
    }
}

template<class Key, class Val>
void RankTree<Key, Val>::RollRR(Node *grandpa) {
    Node *papa = grandpa->right;
    Node *otherSon = papa->left;
    int papa_rank = papa->rank;
    int grandpa_rank = grandpa->rank;
    papa->right->rank -= grandpa_rank;
    papa->rank += grandpa_rank;
    grandpa->rank -= papa_rank;
    papa->left = grandpa;
    if (grandpa->daddy == nullptr) {
        m_root = papa;
        papa->daddy = nullptr;
    } else {
        papa->daddy = grandpa->daddy;
        if (grandpa->daddy->left == grandpa) {
            grandpa->daddy->left = papa;
        } else {
            grandpa->daddy->right = papa;
        }
    }
    grandpa->daddy = papa;
    if (otherSon) {
        grandpa->right = otherSon;
        otherSon->daddy = grandpa;
    } else {
        grandpa->right = nullptr;
    }
    grandpa->height = getHeight(grandpa);
    papa->height = getHeight(papa);
}

template<class Key, class Val>
void RankTree<Key, Val>::RollLR(Node *grandpa) {
    Node *papa = grandpa->left;
    Node *son = papa->right;
    int son_rank = son->rank;
    int papa_rank = papa->rank;
    int grandpa_rank = grandpa->rank;
    son->rank += papa_rank + grandpa_rank;
    papa->rank += grandpa_rank - son_rank;
    grandpa->rank -= son_rank;
    if (son->left) {
        papa->right = son->left;
        papa->right->daddy = papa;
    } else {
        papa->right = nullptr;
    }
    if (son->right) {
        grandpa->left = son->right;
        grandpa->left->daddy = grandpa;
    } else {
        grandpa->left = nullptr;
    }
    son->left = papa;
    papa->daddy = son;
    son->right = grandpa;
    son->daddy = grandpa->daddy;
    if (grandpa->daddy) {
        if (grandpa->daddy->left == grandpa) {
            grandpa->daddy->left = son;
        } else {
            grandpa->daddy->right = son;
        }
    }
    grandpa->daddy = son;
    if (grandpa == m_root) {
        m_root = son;
    }
    papa->height = getHeight(papa);
    grandpa->height = getHeight(grandpa);
    son->height = papa->height > grandpa->height ? papa->height + 1 : grandpa->height + 1;
}

template<class Key, class Val>
void RankTree<Key, Val>::RollRL(Node *grandpa) {
    Node *papa = grandpa->right;
    Node *son = papa->left;
    int son_rank = son->rank;
    int papa_rank = papa->rank;
    int grandpa_rank = grandpa->rank;
    son->rank += papa_rank + grandpa_rank;
    papa->rank += grandpa_rank - son_rank;
    grandpa->rank -= son_rank;
    if (son->right) {
        papa->left = son->right;
        papa->left->daddy = papa;
    } else {
        papa->left = nullptr;
    }
    if (son->left) {
        grandpa->right = son->left;
        grandpa->right->daddy = grandpa;
    } else {
        grandpa->right = nullptr;
    }
    son->right = papa;
    papa->daddy = son;
    son->left = grandpa;
    son->daddy = grandpa->daddy;
    if (grandpa->daddy) {
        if (grandpa->daddy->left == grandpa) {
            grandpa->daddy->left = son;
        } else {
            grandpa->daddy->right = son;
        }
    }
    grandpa->daddy = son;
    if (grandpa == m_root) {
        m_root = son;
    }
    papa->height = getHeight(papa);
    grandpa->height = getHeight(grandpa);
    son->height = papa->height > grandpa->height ? papa->height + 1 : grandpa->height + 1;
}

template<class Key, class Val>
typename RankTree<Key, Val>::Node *RankTree<Key, Val>::Insert(const Key &key, const Val &val) {
    if (Find(key)) {
        return nullptr;
    }
    Node *newNode = InitNode(key, val);
    if (IsEmpty()) {
        m_root = newNode;
        m_size++;
        return newNode;
    }
    // find the place to insert the new node and insert it
    Node *curr = m_root;
    int sum_ranks = 0;
    while (curr != nullptr) {
        if (key < curr->key) {
            if (curr->left == nullptr) {
                curr->left = newNode;
                curr->left->rank = -sum_ranks;
                break;
            }
            sum_ranks += curr->rank;
            curr = curr->left;
        } else if (key > curr->key) {
            if (curr->right == nullptr) {
                curr->right = newNode;
                curr->right->rank = -sum_ranks;
                break;
            }
            sum_ranks += curr->rank;
            curr = curr->right;
        }
    }
    newNode->daddy = curr;
    // check the balance factor of the tree and rock n' roll if needed
    CheckNRoll(newNode);
    this->m_size++;
    return newNode;
}

template<class Key, class Val>
void RankTree<Key, Val>::CheckNRoll(Node *node) {
    Node *curr = node;
    while (curr != m_root) {
        Node *daddy = curr->daddy;
        if (!daddy) {
            return;
        }
        if (daddy->height > curr->height) {
            return;
        }
        daddy->height = curr->height + 1;
        int BF = CalcBF(daddy);
        if (BF == 2 || BF == -2) {
            PickRoll(daddy, BF);
            return;
        }
        curr = daddy;
    }
}

template<class Key, class Val>
void RankTree<Key, Val>::PickRoll(Node *daddy, int BF) {
    if (BF == 2) {
        CalcBF(daddy->left) >= 0 ? RollLL(daddy) : RollLR(daddy);
    } else {
        CalcBF(daddy->right) <= 0 ? RollRR(daddy) : RollRL(daddy);
    }
}

template<class Key, class Val>
bool RankTree<Key, Val>::IsLeaf(const Node *node) const {
    if (node->left == nullptr && node->right == nullptr) {
        return true;
    }
    return false;
}

template<class Key, class Val>
void RankTree<Key, Val>::BalanceTreeAfterDeletion(Node *node) {
    if (node == nullptr) {
        return;
    }
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    int BF = CalcBF(node);
    if (BF == 2 || BF == -2) {
        PickRoll(node, BF);
    }
    if (node->daddy != nullptr) {
        BalanceTreeAfterDeletion(node->daddy);
    }
}

template<class Key, class Val>
typename RankTree<Key, Val>::Node* RankTree<Key, Val>::GetClosestFromBelow(const Key& key) const {
    Node* node_of_key = Find(key);
    if (node_of_key) {
        return GetClosestFromBelow(node_of_key);
    }
    Node* curr = this->m_root;
    while (curr) {
        if ((curr->key < key && !curr->right) || (curr->key < key && curr->right->key > key)) {
            return curr;
        } else if (curr->key < key) {
            curr = curr->right;
        } else if (curr->key > key) {
            curr = curr->left;
        }
    }
    return nullptr;
}

template<class Key, class Val>
void RankTree<Key, Val>::InitRanks(Node* node) {
    if (node != nullptr) {
        InitRanks(node->left);
        InitRanks(node->right);
        node->rank = 0;
    }
}

template<class Key, class Val>
void RankTree<Key, Val>::AddToRanks(Node* node, int amount) {
    if (node == nullptr || m_root == nullptr) {
        return;
    }
    Node* curr = m_root;
    bool row_of_right = false;
    while (curr) {
        if (curr->key == node->key) {
            if (!row_of_right) {
                node->rank += amount;
            }
            if (curr->right) {
                curr->right->rank -= amount;
            }
            return;
        } else if (curr->key < node->key) {
            if (!row_of_right) {
                curr->rank += amount;
            }
            curr = curr->right;
            row_of_right = true;
        } else if (curr->key > node->key) {
            if (row_of_right) {
                curr->rank -= amount;
            }
            curr = curr->left;
            row_of_right = false;
        }
    }
}

template<class Key, class Val>
int RankTree<Key, Val>::GetSumOfRanks(Node* node) {
    if (node == nullptr || m_root == nullptr) {
        return 0;
    }
    int sum_ranks = 0;
    Node* curr = m_root;
    while (curr->key != node->key) {
        sum_ranks += curr->rank;
        if (curr->key < node->key) {
            curr = curr->right;
        }
        else {
            curr = curr->left;
        }
    }
    return sum_ranks + node->rank;
}

#endif // RANKTREE_H