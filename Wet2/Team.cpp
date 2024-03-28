#include "Team.h"

#include "StackNode.h"

Team::Team() {
    this->teamId = 0;
    this->teamStr = 0;
    this->playersCounter = 0;
    this->nextPlayerId = 1;
    this->medals = 0;
}

Team::Team(int teamId) {
    this->teamId = teamId;
    this->teamStr = 0;
    this->playersCounter = 0;
    this->nextPlayerId = 1;
    this->medals = 0;
}

int Team::getTeamStr() { return this->teamStr; }

void Team::calculateTeamStr() {
    if (this->playersCounter == 0) {
        this->teamStr = 0;
        return;
    }
    int median = (this->playersCounter / 2) + 1;
    AVLNode<TwoKeysInt, int>* player =
        this->playersTree.getNodeByRank(median);
    this->teamStr = playersCounter * player->data;
}

void Team::insertPlayer(int str) {
    TwoKeysInt playerKey(str, this->nextPlayerId);
    this->playersStack.push(playerKey);
    this->playersTree.insert(playerKey, str);

    this->nextPlayerId += 1;
    this->playersCounter += 1;

    this->calculateTeamStr();
}

void Team::removeNewestPlayer() {
    if (playersCounter == 0) {
        return;
    }

    TwoKeysInt playerKey = this->playersStack.top();
    this->playersTree.remove(playerKey);
    this->playersStack.pop();

    this->playersCounter -= 1;
    this->nextPlayerId -= 1;

    this->calculateTeamStr();
}

bool Team::isEmpty() {
    if (this->playersCounter == 0) {
        return true;
    }
    return false;
}

void Team::purchaseTeam(Team* otherTeam) {
    int playersCounterBefore = playersCounter;
    // Combine stacks
    combineStacks(otherTeam);
    // Combine trees
    mergeTeamsTrees(&(otherTeam->playersTree), playersCounterBefore);
    // Update team parameters ?
    calculateTeamStr();
}

void Team::combineStacks(Team* otherTeam) {
    StackNode<TwoKeysInt>* tmpNode = otherTeam->playersStack.getStartNode();
    for (int i = 0; i < otherTeam->playersCounter; i++) {
        TwoKeysInt playerKey(tmpNode->data.key1, this->nextPlayerId);
        this->playersStack.push(playerKey);

        this->nextPlayerId += 1;
        this->playersCounter += 1;

        tmpNode = tmpNode->nextNode;
    }
}

void Team::mergeTeamsTrees(AVLTree<TwoKeysInt, int>* otherTree,
                           int playersCounterBefore) {
    AVLNode<TwoKeysInt, int>* playersArray = mergeTeamAVLTrees(
        &this->playersTree, otherTree, playersCounterBefore);
    // Create new empty tree and insert from array into the tree
    int newTreeSize = this->playersCounter;
    int height =
        std::max((int)(std::ceil(std::log2(newTreeSize + 1)) - 1), -1);
    AVLTree<TwoKeysInt, int> newTree(height);
    int toDelete = (newTree.getTreeSize() - newTreeSize);
    newTree.adjustTreeSize(newTree.getRoot(), &toDelete);
    newTree.updateTreePostOrder(newTree.getRoot());
    newTree.inorderToTree(newTree.getRoot(), playersArray, newTreeSize, 0);
    // Delete my tree
    playersTree.destroy(playersTree.getRoot());
    playersTree.setRoot(newTree.getRoot());
    playersTree.setTreeSize(newTreeSize);
    newTree.disconnectRoot();

    delete[] playersArray;
}

/*
 * Merge AVL trees into AVLNode array
 * @param tree1
 * @param tree2
 * @param tree3
 * @return
 *          AVLNode<dataType1,dataType2>*
 */
AVLNode<TwoKeysInt, int>* Team::mergeTeamAVLTrees(
    AVLTree<TwoKeysInt, int>* tree1, AVLTree<TwoKeysInt, int>* tree2,
    int playersCounterBefore) {
    int tree1Size = tree1->getTreeSize();
    AVLNode<TwoKeysInt, int>* tree1Players =
        new AVLNode<TwoKeysInt, int>[tree1Size];

    AVLTree<TwoKeysInt, int>::inorderToArray(tree1->getRoot(), tree1Players,
                                             tree1Size, 0);

    int tree2Size = tree2->getTreeSize();
    AVLNode<TwoKeysInt, int>* tree2Players =
        new AVLNode<TwoKeysInt, int>[tree2Size];

    AVLTree<TwoKeysInt, int>::inorderToArray(tree2->getRoot(), tree2Players,
                                             tree2Size, 0);
    for (int i = 0; i < tree2Size; i++) {
        tree2Players[i].key.key2 += playersCounterBefore;
    }

    AVLNode<TwoKeysInt, int>* newArr =
        merge(tree1Players, tree2Players, tree1Size, tree2Size);

    delete[] tree1Players;

    delete[] tree2Players;

    return newArr;
}

AVLNode<TwoKeysInt, int>* Team::merge(AVLNode<TwoKeysInt, int>* arrTree1,
                                      AVLNode<TwoKeysInt, int>* arrTree2,
                                      int size1, int size2) {
    AVLNode<TwoKeysInt, int>* newArr =
        new AVLNode<TwoKeysInt, int>[size1 + size2];

    int i = 0, j = 0, k = 0;
    for (i = 0, j = 0, k = 0; i < size1 && j < size2 && k < size1 + size2;) {
        if (arrTree1[i].key <= arrTree2[j].key) {
            newArr[k++] = arrTree1[i++];
        } else if (arrTree1[i].key > arrTree2[j].key) {
            newArr[k++] = arrTree2[j++];
        }
    }
    if (i < size1) {
        for (; i < size1; i++, k++) newArr[k] = arrTree1[i];
    } else if (j < size2) {
        for (; j < size2; j++, k++) newArr[k] = arrTree2[j];
    }
    return newArr;
}

void Team::setMedals(int medals) { this->medals = medals; }
int Team::getMedals() { return this->medals; }