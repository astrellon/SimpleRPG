#pragma once

template <class T>
class GameEntityRef
{
public:
	GameEntityRef() 
	{
		mId = -1; mEntity = NULL;
	}
	GameEntityRef(T *entity)
	{
		setEntity(entity);
	}
	GameEntityRef(int id)
	{
		setEntityId(id);
	}
	~GameEntityRef() {}

	T *getEntity()
	{
		if(mEntity != NULL)
		{
			return mEntity;
		}
		GameEntity *entity = GameEntity::getEntityById(getEntityId());
		return dynamic_cast<T *>(entity);
	}
	void setEntity(T *entity)
	{
		if(entity == NULL)
		{
			setEntityId(-1);
		}
		else
		{
			setEntityId(entity->getId());
		}
		mEntity = entity;
	}

	int getEntityId()
	{
		return mId;
	}
	void setEntityId(int id)
	{
		mEntity = NULL;
		mId = id;
	}


protected:

	int mId;
	T *mEntity;
};
