#pragma once

#include "UIList.h"

#include <boost/algorithm/string/predicate.hpp>
using boost::algorithm::iequals;

class UISelector : public UIList
{
public:
	UISelector();
	~UISelector();

	virtual void scrollSelection(const int &diff) {	setSelectionIndex(getSelectionIndex() + diff); }
	virtual void setSelectionIndex(const int &index);
	virtual int getSelectionIndex() { return mSelectedIndex; }

	virtual void setWrapSelection(bool wrap) { mWrapSelection = wrap; }
	virtual bool getWrapSelection() { return mWrapSelection; }

	virtual void selectNextItem(char c);

	virtual UIComponent *getSelectedItem();

protected:

	int mSelectedIndex;
	bool mWrapSelection;

	virtual void updateScroll();

	virtual void renderChild(UIComponent *child, int childIndex, bool overrideFormat);
};
