#ifndef __DownTo_H__
#define __DownTo_H__

#include "slwh/Base/ModelBase.h"

/*
ָ��ָ��İ���
*/

class DownTo :
	public ModelBase
{
private:
	int  m_index;
	bool m_bIsRunning;
	//�洢24�����ӵ���ɫ
	enumAnimalColor m_color[Animal_Max];
	//�洢24������
	Sprite3D* m_downTo[Animal_Max];

public:
	bool LoadModel(std::string const &path);
	//ͨ��·����������Դ
	bool AttachSprite3D(std::string const &path);
	void ChangeColor(enumAnimalColor* const &arr);
	void ChangeColor(int* const &arr);
	DownTo();
	~DownTo();

protected:
	//�ı������ɫ�������̺���
	void ChgeColorRun(float t);
	void Start();

	//void update(float t);
};

#endif

