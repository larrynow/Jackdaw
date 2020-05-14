#pragma once
#ifndef JKENTITY_H_
#define JKENTITY_H_

#include"Jackdaw.h"
#include"Types.h"
#include"Model.h"
#include"Animator.h"
#include"Transform.h"
#include"EntityFactory.h"

#

/*
Class jkEntity : An entity is object that exist in a map.
jkEntity have a model and a transform.
*/

class jkEntity
{
	// Temp.
	friend class jkContent;
	// Temp.
	friend class jkBackendRenderer;

public:

	// Constructor with no resource.
	jkEntity(const VEC3& position) : m_pModel(nullptr), mTransform(position){}
	jkEntity() : jkEntity({0.f, 0.f, 0.f}) {}

	// Constructor with a model.
	jkEntity(const VEC3& position, std::shared_ptr <jkModel> model) :
		m_pModel(model), mTransform(position), m_pAnimator(nullptr)
	{
		if (m_pModel->IsWithAnimation())
		{
			m_pAnimator = new jkAnimator(*model);
		}
	}

	virtual ~jkEntity() { if (m_pAnimator) delete m_pAnimator; };

	DISALLOW_COPY_AND_ASSIGN(jkEntity);

	virtual void Update(const double delta_time) {};

	inline std::shared_ptr<jkModel> GetModel() { return m_pModel; }

	inline void PlayAnimation(std::string anim_name) { if(m_pAnimator) m_pAnimator->Play(anim_name); }

	inline jkTransform& GetTransform() { return mTransform; }
	inline VEC3 GetPosition() { return mTransform.GetPosition(); }

protected:

	std::shared_ptr<jkModel> m_pModel;
	
	jkAnimator* m_pAnimator;

	jkTransform mTransform;

};

//#ifndef REG_ENTITY(jkEntity);


//#endif // !REG_ENTITY(jkEntity)

#endif // !JKENTITY_H_



