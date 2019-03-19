#ifndef GetHouseIndex_H_
#define GetHouseIndex_H_
class GetHouseIndex
{
private:
	static GetHouseIndex *m_Instance;

public:
	~GetHouseIndex()
	{
		if (m_Instance != nullptr)
			delete m_Instance;
	}
	static GetHouseIndex* getInstance();


	void setIndex(int n);
	int getIndex();

	int roomIndex;
	
};


#endif // _GetHouseIndex_H_