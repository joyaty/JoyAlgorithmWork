/**
 * 自定义ADT - 红黑树，一种AVL树的特化变种
 * 红黑树的性质：
 * 1.树中的任意节点，要么是红色的，要么是黑色的
 * 2.根节点是黑色的
 * 3.若一个节点是红色，则子节点必须是黑色的
 * 4.从一个节点到一个NULL指针的节点的每条路径都必须包含相同数量的黑色节点
 */

#pragma once

#include <iostream>

namespace Joy
{
	/// <summary>
	/// 自定义自顶向下红黑树的实现                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T> class JoyRedBlackTree
	{
	private:
		/// <summary>
		/// 红黑树节点颜色枚举定义
		/// </summary>
		enum class EnumColor : unsigned char
		{
			BLACK,
			RED,
		};

		/// <summary>
		/// 红黑树的节点数据结构
		/// </summary>
		struct RedBlackNode
		{
			T elementData{};
			RedBlackNode* pLeftChild{ nullptr };
			RedBlackNode* pRightChild{ nullptr };
			EnumColor colorTag{ EnumColor::BLACK };

			RedBlackNode()
			{
			}

			RedBlackNode(const T& element, RedBlackNode* pLeftNode = nullptr, RedBlackNode* pRightNode = nullptr, EnumColor color = EnumColor::BLACK)
				: elementData(element), pLeftChild(pLeftNode), pRightChild(pRightNode), colorTag(color)
			{
			}
		};

	public:
		explicit JoyRedBlackTree(const T& negInf)
		{
			InitStaticNullNode();
			m_Root = new RedBlackNode(negInf, s_NullNode, s_NullNode, EnumColor::BLACK);
			m_Root->pLeftChild = m_Root->pRightChild = s_NullNode;
		}

		JoyRedBlackTree(const JoyRedBlackTree& rhs)
			: JoyRedBlackTree(rhs.m_Root->elementData)
		{
			operator=(rhs);
		}

		~JoyRedBlackTree()
		{
			MakeEmpty();
		}

	public:
		/// <summary>
		/// 打印树
		/// </summary>
		void PrintTree() const
		{
			std::cout << "PreOrder  ================" << std::endl;
			PrintTreePreOrder(m_Root->pRightChild);
			std::cout << "InOrder   ================" << std::endl;
			PrintTreeInOrder(m_Root->pRightChild);
			std::cout << "PostOrder ================" << std::endl;
			PrintTreePostOrder(m_Root->pRightChild);
		}

		/// <summary>
		/// 是否为空树
		/// </summary>
		/// <returns></returns>
		bool IsEmpty() const
		{
			return m_Root->pRightChild == s_NullNode;
		}

		/// <summary>
		/// 清空树
		/// </summary>
		void MakeEmpty()
		{
			MakeEmpty(m_Root);
		}

		/// <summary>
		/// 查找树最小值
		/// </summary>
		/// <returns></returns>
		const T& FindMin() const
		{
			if (m_Root->pRightChild == s_NullNode)
			{
				// 空树，直接返回极小值
				return m_Root->elementData;
			}
			// 循环寻找左子树
			m_Current = m_Root->pRightChild;
			while (m_Current->pLeftChild != s_NullNode)
			{
				m_Current = m_Current->pLeftChild;
			}
			return m_Current->elementData;
		}
		/// <summary>
		/// 查找树最大值
		/// </summary>
		/// <returns></returns>
		const T& FindMax() const
		{
			if (m_Root->pRightChild == s_NullNode)
			{
				// 空树，直接返回极小值
				return m_Root->elementData;
			}
			// 循环寻找右子树
			m_Current = m_Root->pRightChild;
			while (m_Current->pRightChild != s_NullNode)
			{
				m_Current = m_Current->pRightChild;
			}
			return m_Current->elementData;
		}

		/// <summary>
		/// 是否包含值
		/// </summary>
		/// <param name="element"></param>
		/// <returns></returns>
		bool Contain(const T& element) const
		{
			if (m_Root->pRightChild == s_NullNode)
			{
				// 空树，直接返回极小值
				return false;
			}
			m_Current = m_Root->pRightChild;
			while (m_Current != s_NullNode)
			{
				if (element < m_Current->elementData)
				{
					m_Current = m_Current->pLeftChild;
				}
				else if (m_Current->elementData < element)
				{
					m_Current = m_Current->pRightChild;
				}
				else
				{
					break;
				}
			}
			return m_Current != s_NullNode;
		}

		/// <summary>
		/// 向红黑树新增一个元素节点，自顶向下插入
		/// </summary>
		/// <param name="element"></param>
		void Insert(const T& element)
		{
			if (m_Root->pRightChild == s_NullNode)
			{
				// 空树，直接放在根节点
				m_Root->pRightChild = new RedBlackNode(element, s_NullNode, s_NullNode, EnumColor::BLACK);
				return;
			}
			m_Current = m_Parent = m_Grand = m_Great = m_Root;
			// 空节点元素标记为目标值，确保可以找到插入位置
			s_NullNode->elementData = element;
			while (m_Current->elementData != element)
			{
				m_Great = m_Grand;
				m_Grand = m_Parent;
				m_Parent = m_Current;
				m_Current = element < m_Current->elementData ? m_Current->pLeftChild : m_Current->pRightChild;
				// 当前节点的左右子节点皆为红色，需要翻转当前节点与子节点颜色，若父节点同为红色，需要再旋转
				if (m_Current->pLeftChild->colorTag == EnumColor::RED && m_Current->pRightChild->colorTag == EnumColor::RED)
				{
					HandleReorient(element);
				}
			}
			// 当前节点不为空节点，说明元素已在红黑树中，不处理插入
			if (m_Current != s_NullNode) { return; }
			// 位置在叶子(空)节点
			m_Current = new RedBlackNode(element, s_NullNode, s_NullNode, EnumColor::RED);
			if (element < m_Parent->elementData)
			{
				m_Parent->pLeftChild = m_Current;
			}
			else
			{
				m_Parent->pRightChild = m_Current;
			}
			// 插入新节点，检查红黑树，确认是否要翻转和旋转树结构
			HandleReorient(element);
		}

		/// <summary>
		/// 从红黑树删除指定的元素节点
		/// </summary>
		/// <param name="element"></param>
		void Remove(const T& element)
		{
			// 空树，直接返回不处理
			if (m_Root->pRightChild == s_NullNode) { return; }
			// m_Parent为伪根节点，m_Current为真正的根节点
			m_Grand = m_Parent = m_Root;
			m_Current = m_Root->pRightChild;
			m_Sibling = m_Root->pLeftChild;
			// 指向待删除节点的临时指针
			RedBlackNode* toRemove = s_NullNode;

			while (m_Current != s_NullNode)
			{
				if (m_Current->pLeftChild->colorTag == EnumColor::BLACK && m_Current->pRightChild->colorTag == EnumColor::BLACK)
				{
					// 当前节点的两个子节点都是黑色，切换当前节点的颜色为红色，保证下降时，m_Parent为红色
					HandleBothBlack();
				}
				else
				{
					// 当前节点有红色子节点，尝试继续下降
					m_Grand = m_Parent;
					m_Parent = m_Current;
					if (element < m_Parent->elementData)
					{
						m_Current = m_Parent->pLeftChild;
						m_Sibling = m_Parent->pRightChild;
					}
					else
					{
						// 删除的位置在右子树，更新删除目标，继续下移。若等于，则后续，只会向左子树查找，直到叶子节点
						toRemove = m_Parent;
						m_Current = m_Parent->pRightChild;
						m_Sibling = m_Parent->pLeftChild;
					}
					// 下降后，当前节点是黑色，则兄弟节点必定是红色，可以旋转父节点和兄弟节点，使得兄弟节点变为祖父节点，更新父节点为红色，祖父节点(原兄弟节点，为红色)更新为黑色，确保性质4。回到下一次迭代
					if (m_Current->colorTag == EnumColor::BLACK)
					{
						m_Grand = Rotate(m_Sibling->elementData, m_Grand);
						m_Grand->colorTag = EnumColor::BLACK;
						m_Parent->colorTag = EnumColor::RED;
						m_Sibling = element < m_Parent->elementData ? m_Parent->pRightChild : m_Parent->pLeftChild;
						continue;
					}
					// 下降后，当前节点是红色，则不需要处理，可以继续下降，因为确保了再次下降，m_Parent是红色
				}
				m_Grand = m_Parent;
				m_Parent = m_Current;
				if (element < m_Parent->elementData)
				{
					m_Current = m_Parent->pLeftChild;
					m_Sibling = m_Parent->pRightChild;
				}
				else
				{
					toRemove = m_Parent;
					m_Current = m_Parent->pRightChild;
					m_Sibling = m_Parent->pLeftChild;
				}
			}
			// 根节点重置为黑色，确保性质2
			m_Root->pRightChild->colorTag = EnumColor::BLACK;

			if (toRemove == s_NullNode || toRemove->elementData != element)
			{
				// 没有找到删除的节点，不处理
				return;
			}
			// 找到删除节点，此时toRemove指向删除的目标节点，m_Parent指向删除目标节点右子树下的最小节点，将最小子节点数据替换到删除节点，删除最小子节点
			toRemove->elementData = m_Parent->elementData;
			if (m_Grand->pLeftChild == m_Parent)
			{
				m_Grand->pLeftChild = s_NullNode;
			}
			else
			{
				m_Grand->pRightChild = s_NullNode;
			}

			delete m_Parent;
			m_Parent = s_NullNode;
		}

	public:
		/// <summary>
		/// 复制操作符
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		const JoyRedBlackTree& operator= (const JoyRedBlackTree& rhs)
		{
			if (this != &rhs)
			{
				MakeEmpty();
				m_Root = Clone(rhs.m_Root);
			}
			return *this;
		}
	
	private:
		/// <summary>
		/// 前序遍历打印树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePreOrder(RedBlackNode* pNode) const
		{
			if (pNode == s_NullNode) { return; }
			std::cout << pNode->elementData << " " << static_cast<int>(pNode->colorTag) << std::endl;
			PrintTreePreOrder(pNode->pLeftChild);
			PrintTreePreOrder(pNode->pRightChild);
		}
		/// <summary>
		/// 中序遍历打印树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreeInOrder(RedBlackNode* pNode) const
		{
			if (pNode == s_NullNode) { return; }
			PrintTreeInOrder(pNode->pLeftChild);
			std::cout << pNode->elementData << " " << static_cast<int>(pNode->colorTag) << std::endl;
			PrintTreeInOrder(pNode->pRightChild);
		}
		/// <summary>
		/// 后序遍历打印树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePostOrder(RedBlackNode* pNode) const
		{
			if (pNode == s_NullNode) { return; }
			PrintTreePostOrder(pNode->pLeftChild);
			PrintTreePostOrder(pNode->pRightChild);
			std::cout << pNode->elementData << " " << static_cast<int>(pNode->colorTag) << std::endl;
		}

		/// <summary>
		/// 克隆一个节点
		/// </summary>
		/// <param name="pNode"></param>
		/// <param name="pNullNode"></param>
		/// <returns></returns>
		RedBlackNode* Clone(const RedBlackNode* pNode) const
		{
			// 说明是空节点
			if (pNode == pNode->pLeftChild) { return s_NullNode; }
			return new RedBlackNode(pNode->elementData, Clone(pNode->pLeftChild), Clone(pNode->pRightChild), pNode->colorTag);
		}

		/// <summary>
		/// 清空树
		/// </summary>
		/// <param name="pNode"></param>
		void MakeEmpty(RedBlackNode*& pNode)
		{
			if (pNode == s_NullNode) { return; }
			MakeEmpty(pNode->pLeftChild);
			MakeEmpty(pNode->pRightChild);

			delete pNode;
			pNode = nullptr;
		}

		/// <summary>
		/// 处理红黑树的旋转和颜色翻转
		/// </summary>
		/// <param name="value"></param>
		void HandleReorient(const T& value)
		{
			// 当前节点的左右两个子节点都是红色，或当前节点是新插入节点，则颜色翻转，当前节点为红色，子节点为黑色。
			m_Current->colorTag = EnumColor::RED;
			m_Current->pLeftChild->colorTag = EnumColor::BLACK;
			m_Current->pRightChild->colorTag = EnumColor::BLACK;
			// 当前节点的父节点是红色，需要旋转
			if (m_Parent->colorTag == EnumColor::RED)
			{
				// 旋转的祖父节点需要标记为红色
				m_Grand->colorTag = EnumColor::RED;
				// 当前节点与父节点到祖父节点的方向不同，需要双旋转
				if ((value < m_Parent->elementData) != (m_Parent->elementData < m_Grand->elementData))
				{
					// 当前节点与父节点做一次单旋，旋转的子树根节点的父节点是祖父节点。注意，此时，m_Parent更新为旋转子树的新根节点，这里新根节点为m_Current指向的节点
					m_Parent = Rotate(value, m_Grand);
				}
				// 父节点和祖父节点做一次单旋，旋转的子树的根节点的父节点是曾祖父节点。注意，此时，m_Current更新为旋转子树的新根节点，这里新根节点为m_Parent指向的节点
				m_Current = Rotate(value, m_Great);
				// 单旋转下，m_Current指向旧的m_Parent。双旋转下，m_Current与旧m_Current相同
				m_Current->colorTag = EnumColor::BLACK;
			}
			// 根节点保证为黑色
			m_Root->pRightChild->colorTag = EnumColor::BLACK;
		}

		/// <summary>
		/// 当前节点的两个子节点都是黑色节点，根据当前节点的兄弟节点子节点情况做处理
		/// </summary>
		void HandleBothBlack()
		{
			// 当前节点着色为红色
			m_Current->colorTag = EnumColor::RED;
			if (m_Sibling->pLeftChild->colorTag == EnumColor::BLACK && m_Sibling->pLeftChild->colorTag == EnumColor::BLACK)
			{
				// 兄弟节点的两个子节点都是黑色，直接翻转当前节点，兄弟节点，父节点的颜色，兄弟节点是空节点，则不处理
				m_Sibling->colorTag = m_Sibling != s_NullNode ? EnumColor::RED : EnumColor::BLACK;
			}
			else
			{
				// 兄弟节点的两个子节点至少有一个红色节点
				const T& redValue = m_Sibling->pLeftChild->colorTag == EnumColor::RED ? m_Sibling->pLeftChild->elementData : m_Sibling->pRightChild->elementData;
				if ((redValue < m_Sibling->elementData) != (m_Sibling->elementData < m_Parent->elementData))
				{
					// 红色子节点到兄弟节点的方向与兄弟节点到父节点的方向不同，需要双旋
					Rotate(redValue, m_Parent);
				}
				// 旋转完毕，新根节点根为红色，两个子节点更新为黑色
				m_Grand = Rotate(redValue, m_Grand);
				m_Grand->colorTag = EnumColor::RED;
				m_Grand->pLeftChild->colorTag = m_Grand->pRightChild->colorTag = EnumColor::BLACK;
			}
		}

		/// <summary>
		/// 一次单旋转操作，返回旋转子树的新根节点
		/// 参数为目标值和旋转子树根节点的父节点，根据这两个参数可以方便的判断应该的旋转方向。
		/// </summary>
		/// <param name="value">目标值</param>
		/// <param name="theParent">旋转子树根节点的父节点</param>
		/// <returns></returns>
		RedBlackNode* Rotate(const T& value, RedBlackNode* theParent)
		{
			if (value < theParent->elementData)
			{
				value < theParent->pLeftChild->elementData
					? RotateWithLeftChild(theParent->pLeftChild) 
					: RotateWithRightChild(theParent->pLeftChild);
				return theParent->pLeftChild;
			}
			else
			{
				value < theParent->pRightChild->elementData
					? RotateWithLeftChild(theParent->pRightChild)
					: RotateWithRightChild(theParent->pRightChild);
				return theParent->pRightChild;
			}
		}

		/// <summary>
		/// 单旋转右旋
		/// </summary>
		/// <param name="pNode"></param>
		void RotateWithLeftChild(RedBlackNode*& pNode)
		{
			RedBlackNode* pTempNode = pNode->pLeftChild;
			pNode->pLeftChild = pTempNode->pRightChild;
			pTempNode->pRightChild = pNode;
			pNode = pTempNode;
		}

		/// <summary>
		/// 单旋转左旋
		/// </summary>
		/// <param name="pNode"></param>
		void RotateWithRightChild(RedBlackNode*& pNode)
		{
			RedBlackNode* pTempNode = pNode->pRightChild;
			pNode->pRightChild = pTempNode->pLeftChild;
			pTempNode->pLeftChild = pNode;
			pNode = pTempNode;
		}

	private:
		static void InitStaticNullNode()
		{
			if (s_NullNode != nullptr) { return; }
			s_NullNode = new RedBlackNode();
			s_NullNode->pLeftChild = s_NullNode->pRightChild = s_NullNode;
		}
		
	private:
		/// <summary>
		/// 红黑树根节点，假根节点，包含一个负无穷节点，空左节点，和真实的根右节点
		/// </summary>
		RedBlackNode* m_Root{ nullptr };

		/// <summary>
		/// 当前遍历的节点
		/// </summary>
		mutable RedBlackNode* m_Current{ nullptr };
		/// <summary>
		/// 当前遍历的兄弟节点
		/// </summary>
		RedBlackNode* m_Sibling{ nullptr };
		/// <summary>
		/// 当前遍历的父节点
		/// </summary>
		RedBlackNode* m_Parent{ nullptr };
		/// <summary>
		/// 当前遍历的祖父节点
		/// </summary>
		RedBlackNode* m_Grand{ nullptr };
		/// <summary>
		/// 当前遍历的曾祖父节点
		/// </summary>
		RedBlackNode* m_Great{ nullptr };

	private:
		/// <summary>
		/// 空节点，指代nullptr，所有叶子节点都应该指向s_NullNode，定义此节点可以简化程序，避免过多的nullptr异常处理，也方便按引用返回不存在的节点
		/// </summary>
		static JoyRedBlackTree<T>::RedBlackNode* s_NullNode;
	};

	template<typename T> typename JoyRedBlackTree<T>::RedBlackNode* JoyRedBlackTree<T>::s_NullNode = nullptr;
}

