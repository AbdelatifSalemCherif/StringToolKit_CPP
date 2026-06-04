#pragma once

#include<iostream>

using namespace std;

class StringKernel
{
protected:

	char* _Begin;
	char* _LogicalEnd;
	char* _PhysicalEnd;

	unsigned short _Size(const char* const& FirstAddress, const char* const& LastAddress) const
	{
		return LastAddress - FirstAddress;
	}

	unsigned short _Length(const char* const& FirstAddress, const char* const& LastAddress) const
	{
		return LastAddress - FirstAddress + 1;
	}

	const char* _GetLogicalEnd(const char Data[]) const
	{
		while (*Data)
		{
			Data++;
		}

		return Data;
	}

	const char* _GetLogicalEnd(const char Data[], const unsigned short& Length) const
	{
		return Data + Length - 1;
	}

	bool _IsSameData(const StringKernel& Data1, const StringKernel& Data2) const
	{
		const char* Begin1 = Data1._Begin, * Begin2 = Data2._Begin;

		bool IsSame = Data1.Length() == Data2.Length();

		while (IsSame && (Data1._Begin < Data1._LogicalEnd))
		{
			IsSame = Begin1++ == Begin2++;
		}

		return IsSame;
	}

	bool _IsSameData(const StringKernel& Data2) const
	{
		return _IsSameData(*this, Data2);
	}

	void _AddStopCharacter()
	{
		*_LogicalEnd = '\0';
	}

	enum _enStartFrom { _Left, _Right };

	char* _Write(char* StartWritingAddress, const char* StartReadingAddress, const char* StopReadingAddress
		, _enStartFrom StartWritingFrom) const
	{
		char* StopWritingAddress = StartWritingAddress + _Length(StartReadingAddress, StopReadingAddress) - 1;

		if (StartWritingFrom == _Left)
		{
			while (StartReadingAddress <= StopReadingAddress)
			{
				*StartWritingAddress++ = *StartReadingAddress++;
			}
		}
		else
		{
			StartWritingAddress = StopWritingAddress;

			while (StartReadingAddress <= StopReadingAddress)
			{
				*StartWritingAddress-- = *StopReadingAddress--;
			}
		}

		return StopWritingAddress;
	}

	bool _HaveMoreSpace(const unsigned short& NewLength) const
	{
		return NewLength <= Capacity();
	}

	unsigned short _GetNewCapacity(const unsigned short& NewLength) const
	{

		return (NewLength + 15) & ~15;
	}

	char* _GetNewPhysicalEnd(const unsigned short& Capacity) const
	{
		return _Begin + Capacity - 1;
	}

	void _HandlePhysicalSpace(const unsigned short& NewLength, const bool& DoSaveData)
	{

		if (!_HaveMoreSpace(NewLength))
		{
			char* BeginCopy = _Begin;

			unsigned short Capacity = _GetNewCapacity(NewLength);

			_Begin = new char[Capacity];
			_PhysicalEnd = _GetNewPhysicalEnd(Capacity);

			if (DoSaveData)
			{
				_LogicalEnd = _Write(_Begin, BeginCopy, _LogicalEnd, _Left);
			}
			else
			{
				Clear();
			}

			delete[] BeginCopy;
		}
		else if (!DoSaveData)
		{
			Clear();
		}
	}

	void _Swap2Address(const char** Address1, const char** Address2)
	{
		const char* TempAddress = *Address1;
		*Address1 = *Address2;
		*Address2 = TempAddress;
	}

	enum _enThreading {_Necklace, _Beads};

	enum _enSearchFor {_Sheep, _Wolf};

	enum _enCaseSensitivity {_Sensitive, _Insensitive};

	char* _Search(const char* HaystackBegin, const char* HaystackEnd, const char* NeedleBegin, const char* NeedleEnd,
		_enStartFrom StartSearchFrom = _Left, _enThreading NeedleThreading = _Necklace, _enSearchFor SearchFor = _Sheep
		, _enCaseSensitivity CaseSensitivity = _Sensitive) const
	{
		
		const char* HaystackReader = HaystackBegin, * NeedleReader = NeedleBegin;

		while (HaystackReader < HaystackEnd)
		{
			if (*HaystackReader == *NeedleReader)
			{
				
				if (NeedleReader == NeedleEnd)
				{
					return (SearchFor == _Sheep) ? (char*)HaystackReader - _Length(NeedleBegin, NeedleEnd) + 1 :
						(char*)HaystackReader + 1;
				}
				else
				{
					NeedleReader++;
				}
			}
			else if (NeedleReader > NeedleBegin)
			{
				HaystackReader -= NeedleReader - NeedleBegin;

				NeedleReader = NeedleBegin;
				
			}

			HaystackReader++;
		}

		return nullptr;
	}

public:

	StringKernel()
	{
		_Begin = new char[16];

		_PhysicalEnd = _GetNewPhysicalEnd(16);

		Clear();
	}

	StringKernel(const char Value[], const unsigned short& ValueLength)
	{
		const unsigned short Capacity = _GetNewCapacity(ValueLength);

		_Begin = new char[Capacity];

		_PhysicalEnd = _GetNewPhysicalEnd(Capacity);

		_LogicalEnd = _Write(_Begin, Value, _GetLogicalEnd(Value, ValueLength), _Left);
	}

	StringKernel(const char Value[]) : StringKernel(Value, _Length(Value, _GetLogicalEnd(Value)))
	{

	}

	StringKernel(const StringKernel& Value) : StringKernel(Value._Begin, Value.Length())
	{
		

	}

	~StringKernel()
	{
		cout << "Hi Destructor !" << endl;

		delete[] _Begin;
	}

	const char* Value()
	{
		return _Begin;
	}

	void Clear()
	{

		_LogicalEnd = _Begin;

		_AddStopCharacter();
	}

	bool IsEmpty() const
	{
		return _LogicalEnd == _Begin;
	}

	bool IsFull() const
	{
		return _LogicalEnd == _PhysicalEnd;
	}

	unsigned short Size() const
	{
		return _Size(_Begin, _LogicalEnd);
	}

	unsigned short Length() const
	{
		return _Length(_Begin, _LogicalEnd);
	}

	unsigned short Capacity() const
	{
		return _PhysicalEnd - _Begin + 1;
	}

	void Assignment(const char Source[], const unsigned short& SourceLength)
	{

		_HandlePhysicalSpace(SourceLength, false);

		_LogicalEnd = _Write(_Begin, Source, _GetLogicalEnd(Source, SourceLength), _Left);
	}

	void Assignment(const char Source[])
	{
		Assignment(Source, _Length(Source, _GetLogicalEnd(Source)));
	}

	void Copy(const StringKernel& Source)
	{
		Assignment(Source._Begin, Source.Length());
	}

	void Concatenate(const char Source1[], const unsigned short & Source1Length, const char Source2[]
		, const unsigned short& Source2Length)
	{
		//213472275300

		_HandlePhysicalSpace(Source1Length + Source2Length - 1, false);

		_LogicalEnd = _Write(_Begin, Source1, _GetLogicalEnd(Source1, Source1Length) - 1, _Left);

		_LogicalEnd = _Write(_LogicalEnd + 1, Source2, _GetLogicalEnd(Source2, Source2Length), _Left);

	}

	void Concatenate(const char Source1[], const char Source2[])
	{
		Concatenate(Source1, _Length(Source1, _GetLogicalEnd(Source1)), Source2
			, _Length(Source2, _GetLogicalEnd(Source2)));
	}

	void Concatenate(const char Source1[], const StringKernel& Source2)
	{
		Concatenate(Source1, _Length(Source1, _GetLogicalEnd(Source1)), Source2._Begin, Source2.Length());
	}

	void Concatenate(const StringKernel& Source1, const char Source2[])
	{
		Concatenate(Source2, Source1);
	}

	void Concatenate(const StringKernel& Source1, const StringKernel& Source2)
	{
		Concatenate(Source1._Begin, Source1.Length(), Source2._Begin, Source2.Length());
	}

	void Append(const char Source[], const unsigned short& SourceLength)
	{

		_HandlePhysicalSpace(Length() + SourceLength - 1, true);

		_LogicalEnd = _Write(_LogicalEnd, Source, _GetLogicalEnd(Source, SourceLength), _Left);

	}

	void Append(const char Source[])
	{
		Append(Source, _Length(Source, _GetLogicalEnd(Source)));
	}

	void Append(const StringKernel& Source)
	{
		Append(Source._Begin, Source.Length());
	}

	void Insert(const unsigned short & StartWrittingIndex, const char Source[], const unsigned short& StartReadingIndex, 
		const unsigned short& StopReadingIndex)
	{
		unsigned short Offset = StopReadingIndex - StartReadingIndex + 1;

		_HandlePhysicalSpace(Length() + Offset, true);

		_LogicalEnd = _Write(_Begin + StartWrittingIndex + Offset, _Begin + StartWrittingIndex, _LogicalEnd, _Right);

		_Write(_Begin + StartWrittingIndex, Source + StartReadingIndex, Source + StopReadingIndex, _Left);

	}

	void Insert(const unsigned short& StartWrittingIndex, const char Source[])
	{		
		Insert(StartWrittingIndex, Source, 0, _GetLogicalEnd(Source) - Source - 1);
	}

	void Insert(const unsigned short& StartWrittingIndex, const StringKernel& Source, const unsigned short& StartReadingIndex,
		const unsigned short& StopReadingIndex)
	{
		Insert(StartWrittingIndex, Source._Begin, StartReadingIndex, StopReadingIndex);
	}

	void Insert(const unsigned short& StartWrittingIndex, const StringKernel& Source)
	{
		Insert(StartWrittingIndex, Source._Begin, 0, Source.Length() - 2);
	}

	void Delete(const unsigned short& StartDeleteIndex, const unsigned short& EndDeleteIndex)
	{
		_LogicalEnd = _Write(_Begin + StartDeleteIndex, _Begin + EndDeleteIndex + 1, _LogicalEnd, _Left);

	}

	void Preppend(const char Source[], const unsigned short& SourceLength)
	{
		_HandlePhysicalSpace(Length() + SourceLength - 1, true);

		_LogicalEnd = _Write(_Begin + SourceLength - 1, _Begin, _LogicalEnd, _Right);

		_Write(_Begin, Source, _GetLogicalEnd(Source, SourceLength - 1), _Left);

	}

	void Preppend(const char Source[])
	{
		Preppend(Source, _Length(Source, _GetLogicalEnd(Source)));
	}

	void Preppend(const StringKernel& Source)
	{
		Preppend(Source._Begin, Source.Length());
	}

	void Swap(StringKernel& Data2)
	{
		_Swap2Address((const char**)& _Begin, (const char**)& Data2._Begin);
		_Swap2Address((const char**)& _LogicalEnd, (const char**)& Data2._LogicalEnd);
		_Swap2Address((const char**)& _PhysicalEnd, (const char**)& Data2._PhysicalEnd);

	}

	char at(const unsigned short& Index) const
	{
		char* AtAddress = _Begin + Index;

		return (AtAddress < _LogicalEnd) ? *(AtAddress) : *(_LogicalEnd - 1);
	}

	void PushBack(const char& NewCharacter)
	{
		_HandlePhysicalSpace(Length() + 1, true);

		*_LogicalEnd++ = NewCharacter;

		_AddStopCharacter();
	}

	void PopBack()
	{
		if (_LogicalEnd > _Begin)
		{
			_LogicalEnd--;
			_AddStopCharacter();
		}
		else
		{
			Clear();
		}
	}

	unsigned short SearchSheepMasslyFromLeft(const unsigned short& HaystackBeginIndex, const unsigned short& HaystackEndIndex
		, const char* Needle, const unsigned short& NeedleBeginIndex, const unsigned short& NeedleEndIndex)
	{
		char * Position = _Search(_Begin + HaystackBeginIndex, _Begin + HaystackEndIndex, Needle + NeedleBeginIndex
			, Needle + NeedleEndIndex);

		return (Position == nullptr) ? -1 : (Position - _Begin);
	}

	void Print() const
	{
		char* BeginCopy = _Begin;

		while (BeginCopy < _LogicalEnd)
		{
			cout << *BeginCopy++;
		}
	}








};