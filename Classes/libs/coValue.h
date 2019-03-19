#ifndef _coValue_H_
#define _coValue_H_

// 加密值
template<typename T>
class coValue
{
public:
	coValue(T val = 0, T defaltValue=0)
	{
		mDefValue = defaltValue;
		set(val);
	}

	template<typename T2>
	coValue(const T2& val)
	{
		mDefValue = 0;
		set((T)val);
	}

	template<typename T2>
	coValue(const coValue<T2>& val)
	{
		mDefValue = 0;
		set((T)val.get());
	}

	T operator()() const
	{
		return get();
	}

	template<typename T2>
	coValue<T>& operator= (const T2& val)
	{
		set((T)val);
		return *this;
	}

	template<typename T2>
	coValue<T>& operator= (const coValue<T2>& val)
	{
		set((T)val.get());
		return *this;
	}

	template<typename T2>
	T operator+ (const coValue<T2>& value) const
	{
		return (T)(get() + value.get());
	}

	template<typename T2>
	T operator+ (const T2& val) const
	{
		return (T)(get() + val);
	}

	template<typename T2>
	T operator- (const coValue<T2>& value) const
	{
		return (T)(get() - value.get());
	}

	template<typename T2>
	T operator- (const T2& val) const
	{
		return (T)(get() - val);
	}


	template<typename T2>
	coValue<T>& operator+= (const coValue<T2>& value)
	{
		set(get()+value.get());
		return *this;
	}

	template<typename T2>
	coValue<T>& operator+= (const T2& val)
	{
		set(get()+val);
		return *this;
	}

	template<typename T2>
	coValue<T>& operator-= (const coValue<T2>& value)
	{
		set(get()-value.get());
		return *this;
	}

	template<typename T2>
	coValue<T>& operator-= (const T2& val)
	{
		set(get()-val);
		return *this;
	}

public:
	T get() const
	{
		if (mCheck != (mReal^mCheckKey))
			return mDefValue;

		return mReal ^ mRealKey;
	}

	void set(T value)
	{
		mRealKey	= rand();
		mReal		= mRealKey^value;

		mCheckKey	= rand();
		mCheck		= mCheckKey^mReal;
	}

	void setDefaultValue(T value)
	{
		mDefValue = value;
	}

public:
	T mReal;
	T mCheck;
	T mDefValue;
public:
	int mRealKey;
	int mCheckKey;
};

typedef coValue<int>				coInt;
typedef coValue<unsigned int>		coUInt;
typedef coValue<float>				coFloat;
typedef coValue<bool>				coBool;
typedef coValue<double>				coDouble;
typedef coValue<long long>			coInt64;
typedef coValue<unsigned long long>	coUInt64;

#endif // _coValue_H_