#ifndef __ConfirmDialogDelegate_H__
#define __ConfirmDialogDelegate_H__

class ConfirmDialogDelegate
{
public:
	virtual void onOkClick(int nData,void* pData) {};
	virtual void onCancelClick(int nData, void* pData) {};
};


#endif // __ConfirmDialogDelegate_H__
