#pragma once

#include "UIComponent.h"

class UISpacer : public UIComponent
{
public:
	UISpacer() : UIComponent()
	{
		mMeasuredWidth = 1;
		mMeasuredHeight = 1;
	}
	UISpacer(int width, int height) : UIComponent()
	{
		mMeasuredWidth = width;
		mMeasuredHeight = height;
	}
	~UISpacer() {}
};
