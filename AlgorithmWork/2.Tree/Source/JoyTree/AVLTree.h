/**
 * 自定义ADT - 带有平衡的二叉搜索树
 * 二叉搜索树的定义：二叉搜索树是有特殊性质的二叉树
 * 特殊性质：对于树中的每个节点X，左子树上的每个节点都比节点X小，右子树上的每个节点都比节点X大。即，中序遍历下，节点元素是有序的。
 * 平衡条件：对于树中的每个节点X，左子树的深度与右子树的深度相差不超过1
 */

#pragma once

namespace Joy
{
	template<typename T> class JoyAVLTree
	{
	private:
		struct AVLTreeNode
		{
			T elementData;
			AVLTreeNode* pLeftChild;
			AVLTreeNode* pRightChild;
			int deep;

			AVLTreeNode(const T& element, AVLTreeNode* pLeftNode, AVLTreeNode* pRightNode, int height)
				: elementData(element), pLeftChild(pLeftNode), pRightChild(pRightNode), deep(height)
			{}
		};

	private:
		AVLTreeNode* m_Root{ nullptr };
	};
}
