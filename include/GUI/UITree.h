#pragma once
#include <string>
#include <Util/UI.h>

typedef struct TreeNode* pTreeNode;
typedef UI* TreeDataType;
struct TreeNode {
	UI* data;
	pTreeNode pFirstChild;
	pTreeNode pNextBrother;
	TreeNode(TreeDataType value);
};

class UITree
{
public:
	UITree();
	UITree(TreeDataType Value);
	~UITree();

	void Insert(std::string parentName, TreeDataType Value);
	void InsertBrother(pTreeNode pBrotherNode, TreeDataType Value);

	pTreeNode Search(pTreeNode pNode, std::string targetName);
	
	void Preorder(pTreeNode pNode);
	void Inorder(pTreeNode pNode); 
	void postorder(pTreeNode pNode);

	void PrintNode(pTreeNode pNode);
	void FreeMemory(pTreeNode pNode);

	pTreeNode MouseClick(pTreeNode pNode, float xpos, float ypos);

public:
	pTreeNode pRoot;
};





