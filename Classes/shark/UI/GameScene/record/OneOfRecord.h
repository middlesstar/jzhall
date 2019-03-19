#ifndef  _OneOfRecord_H_
#define  _OneOfRecord_H_

#include "shark/Base/ViewBase.h"

/* ´°¿Ú»ùÀà */
class  OneOfRecord : public ViewBase
{
public:
	OneOfRecord();
	virtual ~OneOfRecord();

	CREATE_FUNC(OneOfRecord);

protected:
	void setContent();
public:
	void setImage(int index);

protected:
	ImageView* logo;
	
};

#endif 

