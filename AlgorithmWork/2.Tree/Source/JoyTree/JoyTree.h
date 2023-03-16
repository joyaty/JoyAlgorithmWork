/**
 * 自定义ADT - 二叉搜索树
 */

#include <iostream>

namespace Joy
{
	/// <summary>
	/// 二叉搜索树
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T> class BinarySearchTree
	{
	public:
		BinarySearchTree()
			: m_Root(nullptr)
		{
		}

		BinarySearchTree(const BinarySearchTree& rhs)
		{
			operator=(rhs);
		}

		~BinarySearchTree() 
		{
			MakeEmpty();
		}

		const T& FindMax() const
		{
			return FindMax(m_Root)->elementData;
		}

		const T& FindMin() const
		{
			return FindMin(m_Root)->elementData;
		}

		bool Contain(const T& element) const
		{
			return Contain(element, m_Root);
		}
		bool IsEmpty() const
		{
			return m_Root == nullptr;
		}

		void PrintTree() const
		{
			std::cout << "InOrder==========" << std::endl;
			PrintTreeInOrder(m_Root);
			std::cout << "PreOrder==========" << std::endl;
			PrintTreePreOrder(m_Root);
			std::cout << "PostOrder==========" << std::endl;
			PrintTreePostOrder(m_Root);
		}

		void MakeEmpty()
		{
			MakeEmpty(m_Root);
		}

		void Insert(const T& element)
		{
			Insert(element, m_Root);
		}

		void Remove(const T& element)
		{
			Remove(element, m_Root);
		}

		const BinarySearchTree& operator=(const BinarySearchTree& rhs)
		{
			// TODO 深拷贝树
			return *this;
		}

	private:
		/// <summary>
		/// 二叉树组成节点
		/// </summary>
		struct BinaryTreeNode
		{
			/// <summary>
			/// 节点数据
			/// </summary>
			T elementData;
			/// <summary>
			/// 左子节点
			/// </summary>
			BinaryTreeNode* leftChild{ nullptr };
			/// <summary>
			/// 右子节点
			/// </summary>
			BinaryTreeNode* rightChild{ nullptr };

			BinaryTreeNode(const T& element, BinaryTreeNode* leftNode, BinaryTreeNode* rightNode)
				: elementData(element), leftChild(leftNode), rightChild(rightNode)
			{
			}
		};

	private:
		/// <summary>
		/// 检查二叉搜索树是否存在数据节点element
		/// </summary>
		/// <param name="element"></param>
		/// <param name="pNode"></param>
		/// <returns></returns>
		bool Contain(const T& element, BinaryTreeNode* pNode) const
		{
			if (pNode == nullptr)
			{
				return false;
			}
			if (pNode->elementData < element)
			{
				return Contain(element, pNode->rightChild);
			}
			if (element < pNode->elementData)
			{
				return Contain(element, pNode->leftChild);
			}
			return true;
		}

		/// <summary>
		/// 查找二叉搜索树的最小节点，一直遍历左子树，递归方法
		/// </summary>
		/// <param name="pNode"></param>
		/// <returns></returns>
		BinaryTreeNode* FindMin(BinaryTreeNode* pNode) const
		{
			if (pNode == nullptr)
			{
				return nullptr;
			}
			if (pNode->leftChild == nullptr)
			{
				return pNode;
			}
			return FindMin(pNode->leftChild);
		}

		/// <summary>
		/// 查找二叉搜索树的最大节点，一直遍历右子树，非递归方法
		/// </summary>
		/// <param name="pNode"></param>
		/// <returns></returns>
		BinaryTreeNode* FindMax(BinaryTreeNode* pNode) const
		{
			if (pNode == nullptr)
			{
				return nullptr;
			}
			while (pNode->rightChild != nullptr)
			{
				pNode = pNode->rightChild;
			}
			return pNode;
		}

		/// <summary>
		/// 插入新元素节点到二叉搜索树
		/// </summary>
		/// <param name="element">新元素数据</param>
		/// <param name="pNode">当前位置节点的引用</param>
		void Insert(const T& element, BinaryTreeNode*& pNode)
		{
			if (pNode == nullptr)
			{
				pNode = new BinaryTreeNode(element, nullptr, nullptr);
			}
			else if (pNode->elementData < element)
			{
				Insert(element, pNode->rightChild);
			}
			else if(element < pNode->elementData)
			{
				Insert(element, pNode->leftChild);
			}
		}

		/// <summary>
		/// 移除二叉搜索树的某个元素节点
		/// </summary>
		/// <param name="element">待删除的元素</param>
		/// <param name="pNode">当前节点</param>
		void Remove(const T& element, BinaryTreeNode*& pNode)
		{
			if (pNode == nullptr)
			{
				return;
			}

			if (pNode->elementData < element)
			{
				Remove(element, pNode->rightChild);
			}
			else if (element < pNode->elementData)
			{
				Remove(element, pNode->leftChild);
			}
			else
			{
				if (pNode->leftChild != nullptr && pNode->rightChild != nullptr)
				{
					// 右子树最小节点上提到删除节点，然后删除右子树的最小节点
					pNode->elementData = RemoveMin(pNode->rightChild);
				}
				else
				{
					BinaryTreeNode* pTempNode = pNode;
					pNode = pNode->leftChild != nullptr ? pNode->leftChild : pNode->rightChild;
					delete pTempNode;
				}
			}
		}

		/// <summary>
		/// 删除二叉搜索树最小节点，返回最小节点的元素
		/// </summary>
		/// <param name="pNode"></param>
		/// <returns></returns>
		T RemoveMin(BinaryTreeNode*& pNode)
		{
			// 找到最小节点
			if (pNode->leftChild != nullptr)
			{
				return RemoveMin(pNode->leftChild);
			}
			else
			{
				BinaryTreeNode* pTempNode = pNode;
				T element = pTempNode->elementData;
				pNode = pNode->rightChild;
				delete pTempNode;

				return element;
			}
		}

		/// <summary>
		/// 清空二叉搜索树
		/// </summary>
		/// <param name="pNode"></param>
		void MakeEmpty(BinaryTreeNode*& pNode)
		{
			if (pNode == nullptr)
			{
				return;
			}
			// 移除右节点
			MakeEmpty(pNode->rightChild);
			// 移除左节点
			MakeEmpty(pNode->leftChild);
			// 移除自身节点
			delete pNode;
		}

		/// <summary>
		/// 打印二叉搜索树 - 中序遍历
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreeInOrder(BinaryTreeNode* pNode) const
		{
			if (pNode == nullptr) { return; }
			PrintTreeInOrder(pNode->leftChild);
			std::cout << pNode->elementData << std::endl;
			PrintTreeInOrder(pNode->rightChild);
		}
		/// <summary>
		/// 打印二叉搜索树 - 前序遍历
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePreOrder(BinaryTreeNode* pNode) const
		{
			if (pNode == nullptr) { return; }
			std::cout << pNode->elementData << std::endl;
			PrintTreePreOrder(pNode->leftChild);
			PrintTreePreOrder(pNode->rightChild);
		}
		/// <summary>
		/// 打印二叉搜索树 - 后序遍历
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePostOrder(BinaryTreeNode* pNode) const
		{
			if (pNode == nullptr) { return; }
			PrintTreePostOrder(pNode->leftChild);
			PrintTreePostOrder(pNode->rightChild);
			std::cout << pNode->elementData << std::endl;
		}

	private:
		/// <summary>
		/// 根节点
		/// </summary>
		BinaryTreeNode* m_Root{ nullptr };
		/// <summary>
		/// 树节点数量
		/// </summary>
		int m_Count{ 0 };
		/// <summary>
		/// 树的深度
		/// </summary>
		int m_Deep{ 0 };
	};
}