#pragma once
#include<assert.h>

// Free list for Object Pool.
template<class ObjectT>
class jkFreeListNode
{
public:
	jkFreeListNode() : mObject() {}
	void SetNext(jkFreeListNode* pNode)
	{
		pNext = pNode;
	}
	jkFreeListNode* GetNext()
	{
		return pNext;
	}

	ObjectT& GetData()
	{
		return mObject;
	}
	ObjectT* GetDataPtr()
	{
		return &mObject;
	}

private:

	union
	{
		ObjectT mObject;//Data held by this node.
		jkFreeListNode* pNext;
	};
};

template<class ObjectT, int POOL_SIZE>
class jkObjectPool
{
	typedef jkFreeListNode<ObjectT> NodeT;//Node define.

public:

	jkObjectPool() : mPoolSize(POOL_SIZE), mFirstAvailable(nullptr)
	{
		mFirstAvailable = &mNodes[0];

		for (size_t i = 0; i < POOL_SIZE - 1; i++)
		{
			mNodes[i].SetNext(&mNodes[i + 1]);
		}
		mNodes[POOL_SIZE - 1].SetNext(nullptr);
	}

	// Return an Node which is available.
	NodeT* Create()
	{
		assert(mFirstAvailable != nullptr);

		NodeT* newNode = mFirstAvailable;
		mFirstAvailable = newNode->GetNext();

		return newNode;
	}

	// Retrieve a Node Created from Obj pool.
	void Retrieve(NodeT* pNode)
	{
		pNode->SetNext(mFirstAvailable);
		mFirstAvailable = pNode;
	}

	// Retrieve an Object Created from Obj pool.
	// If an obj is not created by obj pool, should not be retrive.
	/*void Retrieve(ObjectT* pObj)
	{
		auto ptr = reinterpret_cast<NodeT*>(pObj);
		ptr->SetNext(mFirstAvailable);
		mFirstAvailable = ptr;
	}*/

private:

	int mPoolSize;
	NodeT* mFirstAvailable;
	NodeT mNodes[POOL_SIZE];
};
