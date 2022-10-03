#include "AVL.hpp"


///////////////////////////////////////////////////////////////////////////////////////
//Node class
//////////////////////////////////////////////////////////////////////////////////////
AVL::Node::Node(const string& e, Node *p, Node *l, Node *r){
    element = e;
    parent = p;
    left = l;
    right = r;
    height = 1;
}

AVL::Node* AVL::Node::getParent() const {return parent;}
AVL::Node* AVL::Node::getLeft() const {return left;}
AVL::Node* AVL::Node::getRight() const {return right;}
string AVL::Node::getElement() const { return element; }
int AVL::Node::getHeight() const {return height;}

void AVL::Node::setLeft(Node* l) { left = l; }
void AVL::Node::setRight(Node* r) { right = r; }
void AVL::Node::setParent(Node* p) { parent = p; }
void AVL::Node::setElement(string e) { element = e; }

bool AVL::Node::isLeft() const{
    if (getParent() == nullptr) {
        return false;
    }
    else if (getParent()->getLeft() == this) {
        return true;
    }
    else {
        return false;
    }
}

bool AVL::Node::isRight() const {
    if (getParent() == nullptr) {
        return false;
    }
    else if (getParent()->getRight() == this) {
        return true;
    }
    else {
        return false;
    }
}

int AVL::Node::rightChildHeight() const {
    if (getRight() == nullptr) {
        return 0;
    }
    else {
        return getRight()->getHeight();
    }
}

int AVL::Node::leftChildHeight() const{
    if (getLeft() == nullptr) {
        return 0;
    }
    else {
        return getLeft()->getHeight();
    }
}

int AVL::Node::updateHeight() {

    if (leftChildHeight() >= rightChildHeight()) {
        height = leftChildHeight() + 1;
        return leftChildHeight() + 1;
    }
    else {
        height = rightChildHeight() + 1;
        return rightChildHeight() + 1;
    }
}

bool AVL::Node::isBalanced() {
    if (abs(leftChildHeight() - rightChildHeight()) > 1) {
        return false;
    }
    else {
        return true;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
//AVL class
//////////////////////////////////////////////////////////////////////////////////////////
AVL::AVL() {
    size = 0;
    root = nullptr;
}

AVL::AVL(const AVL& tree) {
    size = 0;
    root = nullptr;
    AVL::Iterator final = tree.end();
    for (AVL::Iterator it = tree.begin(); it != final; ++it) {
        add(it.operator*());
    }
}

bool AVL::contains(string e) {
    Node* curr = root;
    int res;

    while (curr != nullptr) {

        res = e.compare(curr->getElement());
        if (res > 0) {
            curr = curr->getRight();
        }
        else if (res < 0) {
            curr = curr->getLeft();
        }
        else {
            return true;
        }
    }

    return false;
}

bool AVL::add(string e) {
    Node* curr = root;
    Node* currParent = nullptr;
    int res;
    
    if (contains(e)) {
        return false;
    }

    while (curr != nullptr) {
        currParent = curr;
        res = e.compare(curr->getElement());
        if (res > 0) {
            curr = curr->getRight();
        }
        else {
            curr = curr->getLeft();
        }
    }
    
    size++;
    curr = new Node(e, currParent, nullptr, nullptr);
    if (size == 1) {
        root = curr;
    }
    //briskw an to neo paidi einai aristero h deksi kai to syndew me ton patera
    if (currParent != nullptr) {
        if (res > 0) {
            currParent->setRight(curr);
        }
        else {
            currParent->setLeft(curr);
        }
    }
    //elegxo thn isorropia tou dentrou
    balanceFunction(currParent);

    return true;
}

bool AVL::rmv(string e) {
    Node* curr = root;
    Node* swapNode = nullptr;
    Node* currParent = nullptr;
    int res;

    if (!contains(e)) {
        return false;
    }
    
    while (1) {
        res = e.compare(curr->getElement());
        if (res > 0) {
            curr = curr->getRight();
        }
        else if (res < 0) {
            curr = curr->getLeft();
        }
        else {
            break;
        }
    }

    //an o komvos exei deksi upodentro, vriskei to aristerotero komvo tou upodentrou
    if (curr->getRight() != nullptr) {
        swapNode = curr->getRight();
        while (swapNode->getLeft() != nullptr) {
            swapNode = swapNode->getLeft();
        }

        currParent = swapNode->getParent();
        
        if (swapNode->isLeft()) {
            //orizei neo paidi ston patera tou, to deksi tou paidi giati den exei aristero
            swapNode->getParent()->setLeft(swapNode->getRight());
        }
        else {
            //sthn periptwsh pou to deksi upodentro exei mono deksia paidia
            swapNode->getParent()->setRight(swapNode->getRight());
        }
        if (swapNode->getRight() != nullptr) {
            swapNode->getRight()->setParent(swapNode->getParent());
        }

        swapNode->setParent(curr->getParent());
        if (curr->getParent() != nullptr) {
            if (curr->isLeft()) {
                curr->getParent()->setLeft(swapNode);
            }
            else {
                curr->getParent()->setRight(swapNode);
            }
        }
        swapNode->setLeft(curr->getLeft());
        if (curr->getLeft() != nullptr) {
            curr->getLeft()->setParent(swapNode);
        }
        swapNode->setRight(curr->getRight());
        if (curr->getRight() != nullptr) {
            curr->getRight()->setParent(swapNode);
        }

        if (curr == root) {
            root = swapNode;
        }
        if (currParent == curr) {
            currParent = swapNode;
        }
    }
    //an o komvos exei mono aristero paidi
    else if (curr->getLeft() != nullptr) {
        swapNode = curr->getLeft();

        swapNode->setParent(curr->getParent());
        if (swapNode->getParent() != nullptr) {
            if (curr->isLeft()) {
                swapNode->getParent()->setLeft(swapNode);
            }
            else {
                swapNode->getParent()->setRight(swapNode);
            }
        }
        currParent = swapNode->getParent();
        if (curr == root) {
            root = swapNode;
        }
    }
    //an o komvos den exei paidia
    else {
        currParent = curr->getParent();
        if (size != 1) {
            if (curr->isRight()) {
                curr->getParent()->setRight(nullptr);
            }
            else {
                curr->getParent()->setLeft(nullptr);
            }
        }
        else{
            root = nullptr;
        }
    }
    //elegxei thn isorropia tou dentrou
    balanceFunction(currParent);

    size--;
    delete curr;
    return true;
}

void AVL::balanceFunction(Node* currParent) {
    Node* top;
    Node* medium;
    Node* low;
    stack<Node*> temp;
    stack<Node*> SearchForBalance;
    
    //bazw se mia stoiva tous komvous pou pithanon na exei allaksei to upsos tous
    while (currParent != nullptr) {
        currParent->updateHeight();
        temp.push(currParent);
        currParent = currParent->getParent();
    }

    //anapodogurizw thn stoiva gia na elegxo tous komvous gia isorropia apo katw pros ta panw
    while (!temp.empty()) {
        SearchForBalance.push(temp.top());
        temp.pop();
    }

    //elegxo kathe komvo sth stoiva an isorropei
    //an den isorropei vriskw tous treis komvous pou tha summetexoun sthn peristrofh
    //kai ektelw thn katallhlh peristrofh
    while (!SearchForBalance.empty()) {
        top = SearchForBalance.top();
        SearchForBalance.pop();

        if (!top->isBalanced()) {
            currParent = top->getParent();
            if (top->leftChildHeight() > top->rightChildHeight()) {
                medium = top->getLeft();
            }
            else {
                medium = top->getRight();
            }

            if (medium->leftChildHeight() > medium->rightChildHeight()) {
                low = medium->getLeft();
            }
            else if(medium->leftChildHeight() < medium->rightChildHeight()){
                low = medium->getRight();
            }
            else {
                //ama ki oi duo peristrofes einai dynates, epilegw thn aplh peristrofh
                if (medium->isLeft()) {
                    low = medium->getLeft();
                }
                else {
                    low = medium->getRight();
                }
            }

            //uparxoun 4 diaforetikes periptwseis peristrofwn
            //2 aples(medium-left,low-left kai medium-right,low-right)
            //2 diples(medium-left,low-right kai medium-right,low-left)
            if (medium->isRight() && low->isRight()) {
                medium->setParent(top->getParent());
                if (top->getParent() != nullptr) {
                    if (top->isLeft()) {
                        top->getParent()->setLeft(medium);
                    }
                    else {
                        top->getParent()->setRight(medium);
                    }
                }
                if (top == root) {
                    root = medium;
                }
                top->setParent(medium);
                top->setRight(medium->getLeft());
                if (medium->getLeft() != nullptr) {
                    medium->getLeft()->setParent(top);
                }
                medium->setLeft(top);

                low->updateHeight();
                top->updateHeight();
                medium->updateHeight();
            }
            else if (medium->isRight() && low->isLeft()) {
                low->setParent(top->getParent());
                if (top->getParent() != nullptr) {
                    if (top->isLeft()) {
                        top->getParent()->setLeft(low);
                    }
                    else {
                        top->getParent()->setRight(low);
                    }
                }
                if (top == root) {
                    root = low;
                }
                top->setParent(low);
                top->setRight(low->getLeft());
                if (low->getLeft() != nullptr) {
                    low->getLeft()->setParent(top);
                }
                low->setLeft(top);
                medium->setParent(low);
                medium->setLeft(low->getRight());
                if (low->getRight() != nullptr) {
                    low->getRight()->setParent(medium);
                }
                low->setRight(medium);

                top->updateHeight();
                medium->updateHeight();
                low->updateHeight();
            }
            else if (medium->isLeft() && low->isLeft()) {
                medium->setParent(top->getParent());
                if (top->getParent() != nullptr) {
                    if (top->isLeft()) {
                        top->getParent()->setLeft(medium);
                    }
                    else {
                        top->getParent()->setRight(medium);
                    }
                }
                if (top == root) {
                    root = medium;
                }
                top->setParent(medium);
                top->setLeft(medium->getRight());
                if (medium->getRight() != nullptr) {
                    medium->getRight()->setParent(top);
                }
                medium->setRight(top);

                low->updateHeight();
                top->updateHeight();
                medium->updateHeight();
            }
            else {
                low->setParent(top->getParent());
                if (top->getParent() != nullptr) {
                    if (top->isLeft()) {
                        top->getParent()->setLeft(low);
                    }
                    else {
                        top->getParent()->setRight(low);
                    }
                }
                if (top == root) {
                    root = low;
                }
                top->setParent(low);
                top->setLeft(low->getRight());
                if (low->getRight() != nullptr) {
                    low->getRight()->setParent(top);
                }
                low->setRight(top);
                medium->setParent(low);
                medium->setRight(low->getLeft());
                if (low->getLeft() != nullptr) {
                    low->getLeft()->setParent(medium);
                }
                low->setLeft(medium);

                top->updateHeight();
                medium->updateHeight();
                low->updateHeight();
            }
            //diorthwnei to upsos olwn twn parapanw komvwn sto dentro pou einai
            //pithano na tous metavlhthei to upsos
            while (currParent != nullptr) {
                currParent->updateHeight();
                currParent = currParent->getParent();
            }
        }
    }
}

void AVL::nextNode(Node* curr, ofstream& DotFile) {
    if (curr != nullptr) {
        DotFile << "\t" << curr->getParent()->getElement() << " -- " << curr->getElement() << endl;
        DotFile << "\t" << curr->getElement() << " [label = \"" << curr->getElement() << "\", shape = circle, color = black]\n";
        nextNode(curr->getLeft(), DotFile);
        nextNode(curr->getRight(), DotFile);
    }
}

void AVL::print2DotFile(char* filename) {
    ofstream DotFile;
    DotFile.open(filename);
    DotFile << "graph AVL {\n";
    if (root != nullptr) {
        DotFile << "\t" << root->getElement() << " [label = \"" << root->getElement() << "\", shape = circle, color = black]\n";
        nextNode(root->getLeft(), DotFile);
        nextNode(root->getRight(), DotFile);
    }
    DotFile << "}\n";
    DotFile.close();
}

void AVL::pre_order(ostream& out) {
    AVL::Iterator final = end();
    for (AVL::Iterator it = begin(); it != final; ++it) {
        out << it.operator*() <<" ";
    }
}

ostream& operator<<(std::ostream& out, const AVL& tree) {
    AVL::Iterator final = tree.end();
    for (AVL::Iterator it = tree.begin(); it != final; ++it) {
        out << it.operator*() << " ";
    }
    return out;
}

AVL& AVL::operator=(const AVL& avl) {
    AVL::Iterator final = end();
    for (AVL::Iterator it = begin(); it != final; ++it) {
        rmv(it.operator*());
    }
    for (AVL::Iterator it = avl.begin(); it != final; ++it) {
        add(it.operator*());
    }
    return *this;
}

AVL  AVL::operator+(const AVL& avl) {
    AVL new_avl;
    AVL::Iterator final = end();

    for (AVL::Iterator it = begin(); it != final; ++it) {
        new_avl.add(it.operator*());
    }
    
    for (AVL::Iterator it = avl.begin(); it != final; ++it) {
        new_avl.add(it.operator*());
    }
    
    return new_avl;
}

AVL&  AVL::operator+=(const AVL& avl) {
    AVL::Iterator final = end();

    for (AVL::Iterator it = avl.begin(); it != final; ++it) {
        add(it.operator*());
    }

    return *this;
}

AVL& AVL::operator+=(const string& e) {
    add(e);
    return *this;
}

AVL& AVL::operator-=(const string& e) {
    rmv(e);
    return *this;
}

AVL AVL::operator+(const string& e) {
    AVL new_avl;
    AVL::Iterator final = end();

    for (AVL::Iterator it = begin(); it != final; ++it) {
        new_avl.add(it.operator*());
    }
    new_avl.add(e);

    return new_avl;
}

AVL  AVL::operator-(const string& e) {
    AVL new_avl;
    AVL::Iterator final = end();

    for (AVL::Iterator it = begin(); it != final; ++it) {
        new_avl.add(it.operator*());
    }
    new_avl.rmv(e);

    return new_avl;
}

AVL::Iterator AVL::begin() const {
    Iterator it(root);
    return it;
}

AVL::Iterator AVL::end() const {
    Iterator it;
    return it;
}

/////////////////////////////////////////////////////////////////////////////
//iterator class
////////////////////////////////////////////////////////////////////////////
AVL::Iterator::Iterator(Node* root) {
    stack<Node*> temp;
    int i, stackSize;
    Node* curr = root;

    temp = fillStack(curr, temp);

    stackSize = temp.size();
    for (i = 0; i < stackSize; i++) {
        curr = temp.top();
        temp.pop();
        st.push(curr);
    }
    currNode = root;
}

AVL::Iterator::Iterator() {
    currNode = nullptr;
}

AVL::Iterator::Iterator(const Iterator& iter) {
    currNode = iter.currNode;
    st = iter.st;
}

stack<AVL::Node*> AVL::Iterator::fillStack(Node* curr, stack<Node*> temp) {
    if (curr != nullptr) {
        temp.push(curr);
        temp = fillStack(curr->getLeft(), temp);
        temp = fillStack(curr->getRight(), temp);
    }
    return temp;
}

AVL::Iterator& AVL::Iterator::operator++() {
    st.pop();
    if (st.empty()) {
        currNode = nullptr;
    }
    else {
        currNode = st.top();
    }
    return *this;
}

AVL::Iterator AVL::Iterator::operator++(int a) {
    Iterator it(*this);
    st.pop();
    if (st.empty()) {
        currNode = nullptr;
    }
    else {
        currNode = st.top();
    }
    return it;
}

string AVL::Iterator::operator*() {
    return currNode->getElement();
}

bool AVL::Iterator::operator==(Iterator it) {
    if (currNode != it.currNode || st != it.st) {
        return false;
    }
    return true;
}

bool AVL::Iterator::operator!=(Iterator it) {
    if (currNode == it.currNode && st == it.st) {
        return false;
    }
    return true;
}



