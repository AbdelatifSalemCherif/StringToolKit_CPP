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

	void _Swap2Address(const char** Address1, const char** Address2) const
	{
		const char* TempAddress = *Address1;
		*Address1 = *Address2;
		*Address2 = TempAddress;
	}

	void _Swap2Indices(unsigned short& Index1, unsigned short& Index2) const
	{
		unsigned short TempIndex = Index1;
		Index1 = Index2;
		Index2 = TempIndex;
	}

	unsigned short _HandleEndIndex(const char* Begin, const char* End, unsigned short EndIndex) const
	{
		return (EndIndex > End - Begin) ? End - Begin: EndIndex; 
	}

	char* _SearchNecklaceFromLeft(const char* CofferBegin, const char* CofferEnd, const char* NecklaceBegin
		, const char* NecklaceEnd) const
	{
		const char* CofferReader = CofferBegin, * NecklaceReader = NecklaceBegin;

		while (CofferReader <= CofferEnd)
		{
			if (*CofferReader == *NecklaceReader)
			{

				if (NecklaceReader == NecklaceEnd)
				{
					return (char*) CofferReader - (NecklaceEnd - NecklaceBegin);
				}
				else
				{
					NecklaceReader++;
				}
			}
			else if (NecklaceReader > NecklaceBegin)
			{
				CofferReader -= NecklaceReader - NecklaceBegin;

				NecklaceReader = NecklaceBegin;

			}

			CofferReader++;
		}

		return nullptr;
	}

	char* _SearchNecklaceFromRight(const char* CofferBegin, const char* CofferEnd, const char* NecklaceBegin
		, const char* NecklaceEnd) const
	{
		const char* CofferReader = CofferEnd, * NecklaceReader = NecklaceEnd;

		while (CofferReader >= CofferBegin)
		{
			if (*CofferReader == *NecklaceReader)
			{

				if (NecklaceReader == NecklaceBegin)
				{
					return (char*) CofferReader;
				}
				else
				{
					NecklaceReader--;
				}
			}
			else if (NecklaceReader < NecklaceEnd)
			{
				CofferReader += NecklaceEnd - NecklaceReader;

				NecklaceReader = NecklaceEnd;

			}

			CofferReader--;
		}

		return nullptr;
	}

	char* _SearchGemFromLeft(const char* CofferBegin, const char* CofferEnd, const char* FirstGem, const char* LastGem) const
	{
		const char* CofferReader = CofferBegin, * GemsReader = FirstGem;

		while (CofferReader <= CofferEnd)
		{
			while (GemsReader <= LastGem)
			{
				if (*CofferReader == *GemsReader)
				{
					return (char*) CofferReader;
				}

				GemsReader++;
			}

			GemsReader = FirstGem;
			CofferReader++;
		}
	

		return nullptr;
	}

	char* _SearchPebbleFromLeft(const char* CofferBegin, const char* CofferEnd, const char* FirstGem, const char* LastGem) const
	{
		const char* CofferReader = CofferBegin;

		while (CofferReader <= CofferEnd)
		{
			if (_SearchGemFromLeft(FirstGem, LastGem, CofferReader, CofferReader) == nullptr)
			{
				return (char*) CofferReader;
			}

			CofferReader++;
		}

		return nullptr;
	}

	char* _SearchGemFromRight(const char* CofferBegin, const char* CofferEnd, const char* FirstGem, const char* LastGem) const
	{
		const char* CofferReader = CofferEnd, * GemsReader = LastGem;

		while (CofferReader >= CofferBegin)
		{
			while (GemsReader >= LastGem)
			{
				if (*CofferReader == *GemsReader)
				{
					return (char*)CofferReader;
				}

				GemsReader--;
			}

			GemsReader = LastGem;
			CofferReader--;
		}

		return nullptr;
	}

	char* _SearchPebbleFromRight(const char* CofferBegin, const char* CofferEnd, const char* FirstGem, const char* LastGem) const
	{
		const char* CofferReader = CofferEnd, * GemsReader = LastGem;

		while (CofferReader >= CofferBegin)
		{
			if (_SearchGemFromLeft(FirstGem, LastGem, CofferReader, CofferReader) == nullptr)
			{
				return (char*)CofferReader;
			}

			CofferReader--;
		}

		return nullptr;
	}

public:

	const unsigned short NoPosition = -1;

	StringKernel()
	{
		_Begin = new char[16];

		_PhysicalEnd = _GetNewPhysicalEnd(16);

		Clear();
	}

	StringKernel(const char Value[], unsigned short ValueLength)
	{
		if (ValueLength == 0)
		{
			ValueLength = 1;
		}

		const unsigned short Capacity = _GetNewCapacity(ValueLength);

		_Begin = new char[Capacity];

		_PhysicalEnd = _GetNewPhysicalEnd(Capacity);

		if (ValueLength == 1)
		{
			Clear();
		}
		else
		{
			_LogicalEnd = _Write(_Begin, Value, _GetLogicalEnd(Value, ValueLength), _Left);
		}
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

	void Assignment(const char Source[])
	{
		const char* SourceLogicalEnd = _GetLogicalEnd(Source);

		_HandlePhysicalSpace(_Length(Source, SourceLogicalEnd), false);

		_LogicalEnd = _Write(_Begin, Source, SourceLogicalEnd, _Left);
	}

	void Assignment(const StringKernel& Source)
	{
		_HandlePhysicalSpace(Source.Length(), false);

		_LogicalEnd = _Write(_Begin, Source._Begin, Source._LogicalEnd, _Left);
	}

	void Copy(const char Source[])
	{
		Assignment(Source);
	}

	void Copy(const StringKernel& Source)
	{
		Assignment(Source);
	}

	void Concatenate(const char Source1[], const char Source2[])
	{
		const char* Source1LogicalEnd = _GetLogicalEnd(Source1), * Source2LogicalEnd = _GetLogicalEnd(Source2);

		_HandlePhysicalSpace(_Length(Source1, Source1LogicalEnd) + _Length(Source2, Source2LogicalEnd) - 1, false);

		_LogicalEnd = _Write(_Begin, Source1, Source1LogicalEnd - 1, _Left);

		_LogicalEnd = _Write(_LogicalEnd + 1, Source2, Source2LogicalEnd, _Left);

	}

	void Concatenate(const char Source1[], const StringKernel& Source2)
	{
		const char* Source1LogicalEnd = _GetLogicalEnd(Source1);

		_HandlePhysicalSpace(_Length(Source1, Source1LogicalEnd) + Source2.Length() - 1, false);

		_LogicalEnd = _Write(_Begin, Source1, Source1LogicalEnd - 1, _Left);

		_LogicalEnd = _Write(_LogicalEnd + 1, Source2._Begin, Source2._LogicalEnd, _Left);
	}

	void Concatenate(const StringKernel& Source1, const char Source2[])
	{
		Concatenate(Source2, Source1);
	}

	void Concatenate(const StringKernel& Source1, const StringKernel& Source2)
	{
		_HandlePhysicalSpace(Source1.Length() + Source2.Length() - 1, false);

		_LogicalEnd = _Write(_Begin, Source1._Begin, Source1._LogicalEnd - 1, _Left);

		_LogicalEnd = _Write(_LogicalEnd + 1, Source2._Begin, Source2._LogicalEnd, _Left);
	}

	void Append(const char Source[])
	{
		const char* SourceLogicalEnd = _GetLogicalEnd(Source);

		_HandlePhysicalSpace(Length() + _Length(Source, SourceLogicalEnd) - 1, true);

		_LogicalEnd = _Write(_LogicalEnd, Source, SourceLogicalEnd, _Left);

	}

	void Append(const StringKernel& Source)
	{
		_HandlePhysicalSpace(Length() + Source.Length() - 1, true);

		_LogicalEnd = _Write(_LogicalEnd, Source._Begin, Source._LogicalEnd, _Left);
	}

	void Insert(const unsigned short & StartWrittingIndex, const char Source[], const unsigned short& StartReadingIndex, 
		const unsigned short& StopReadingIndex)
	{
		const char* SourceLogicalEnd = _GetLogicalEnd(Source);




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

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]
		, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		if (CofferBeginIndex > CofferEndIndex || NecklaceBeginIndex > NecklaceEndIndex || 
			(NecklaceEndIndex - NecklaceBeginIndex) > (CofferEndIndex - CofferBeginIndex))
		{
			return NoPosition;
		}

		CofferEndIndex = _HandleEndIndex(_Begin, _LogicalEnd - 1, CofferEndIndex);

		NecklaceEndIndex = _HandleEndIndex(Necklace, _GetLogicalEnd(Necklace) - 1, NecklaceEndIndex);

		char * Position = _SearchNecklaceFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Necklace + NecklaceBeginIndex
			, Necklace + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);

	}

	unsigned short SearchNecklaceFromLeft(const char Necklace[]) const
	{
		char* Position = _SearchNecklaceFromLeft(_Begin, _LogicalEnd - 1, Necklace, _GetLogicalEnd(Necklace) - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Necklace
	, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{
		if (CofferBeginIndex > CofferEndIndex || NecklaceBeginIndex > NecklaceEndIndex ||
			(NecklaceEndIndex - NecklaceBeginIndex) > (CofferEndIndex - CofferBeginIndex))
		{
			return NoPosition;
		}

		CofferEndIndex = _HandleEndIndex(_Begin, _LogicalEnd - 1, CofferEndIndex);

		NecklaceEndIndex = _HandleEndIndex(Necklace._Begin, Necklace._LogicalEnd - 1, NecklaceEndIndex);

		char* Position = _SearchNecklaceFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Necklace._Begin + NecklaceBeginIndex
			, Necklace._LogicalEnd + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchNecklaceFromLeft(StringKernel& Necklace) const
	{
		
		char* Position = _SearchNecklaceFromLeft(_Begin, _LogicalEnd - 1, Necklace._Begin, Necklace._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchNecklaceFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]
		, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{
		if (CofferBeginIndex > CofferEndIndex || NecklaceBeginIndex > NecklaceEndIndex ||
			(NecklaceEndIndex - NecklaceBeginIndex) > (CofferEndIndex - CofferBeginIndex))
		{
			return NoPosition;
		}

		CofferEndIndex = _HandleEndIndex(_Begin, _LogicalEnd - 1, CofferEndIndex);

		NecklaceEndIndex = _HandleEndIndex(Necklace, _GetLogicalEnd(Necklace) - 1, NecklaceEndIndex);

		char* Position = _SearchNecklaceFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Necklace + NecklaceBeginIndex
			, Necklace + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchNecklaceFromRight(const char Necklace[]) const
	{
		char* Position = _SearchNecklaceFromRight(_Begin, _LogicalEnd - 1, Necklace, _GetLogicalEnd(Necklace) - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchNecklaceFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Necklace
		, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{
		if (CofferBeginIndex > CofferEndIndex || NecklaceBeginIndex > NecklaceEndIndex ||
			(NecklaceEndIndex - NecklaceBeginIndex) > (CofferEndIndex - CofferBeginIndex))
		{
			return NoPosition;
		}

		CofferEndIndex = _HandleEndIndex(_Begin, _LogicalEnd - 1, CofferEndIndex);

		NecklaceEndIndex = _HandleEndIndex(Necklace._Begin, Necklace._LogicalEnd - 1, NecklaceEndIndex);

		char* Position = _SearchNecklaceFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Necklace._Begin + NecklaceBeginIndex
			, Necklace._LogicalEnd + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchNecklaceFromRight(StringKernel& Necklace) const
	{

		char* Position = _SearchNecklaceFromRight(_Begin, _LogicalEnd - 1, Necklace._Begin, Necklace._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		if (CofferBeginIndex > CofferEndIndex || FirstGemIndex > LastGemIndex ||
			(LastGemIndex - FirstGemIndex) > (CofferEndIndex - CofferBeginIndex))
		{
			return NoPosition;
		}

		CofferEndIndex = _HandleEndIndex(_Begin, _LogicalEnd - 1, CofferEndIndex);

		LastGemIndex = _HandleEndIndex(Gems, _GetLogicalEnd(Gems) - 1, LastGemIndex);

		char* Position = _SearchGemFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems + FirstGemIndex
			, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromLeft(const char Gems[]) const
	{
		char* Position = _SearchGemFromLeft(_Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems) - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		if (CofferBeginIndex > CofferEndIndex || FirstGemIndex > LastGemIndex ||
			(LastGemIndex - FirstGemIndex) > (CofferEndIndex - CofferBeginIndex))
		{
			return NoPosition;
		}

		CofferEndIndex = _HandleEndIndex(_Begin, _LogicalEnd - 1, CofferEndIndex);

		LastGemIndex = _HandleEndIndex(Gems._Begin, Gems._LogicalEnd - 1, LastGemIndex);

		char* Position = _SearchGemFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems._Begin + FirstGemIndex
			, Gems._LogicalEnd + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromLeft(StringKernel& Gems) const
	{

		char* Position = _SearchGemFromLeft(_Begin, _LogicalEnd - 1, Gems._Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		if (CofferBeginIndex > CofferEndIndex || FirstGemIndex > LastGemIndex ||
			(LastGemIndex - FirstGemIndex) > (CofferEndIndex - CofferBeginIndex))
		{
			return NoPosition;
		}

		CofferEndIndex = _HandleEndIndex(_Begin, _LogicalEnd - 1, CofferEndIndex);

		LastGemIndex = _HandleEndIndex(Gems, _GetLogicalEnd(Gems) - 1, LastGemIndex);

		char* Position = _SearchPebbleFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems + FirstGemIndex
			, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromLeft(const char Gems[]) const
	{
		char* Position = _SearchPebbleFromLeft(_Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems) - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		if (CofferBeginIndex > CofferEndIndex || FirstGemIndex > LastGemIndex ||
			(LastGemIndex - FirstGemIndex) > (CofferEndIndex - CofferBeginIndex))
		{
			return NoPosition;
		}

		CofferEndIndex = _HandleEndIndex(_Begin, _LogicalEnd - 1, CofferEndIndex);

		LastGemIndex = _HandleEndIndex(Gems._Begin, Gems._LogicalEnd - 1, LastGemIndex);

		char* Position = _SearchPebbleFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems._Begin + FirstGemIndex
			, Gems._LogicalEnd + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromLeft(StringKernel& Gems) const
	{

		char* Position = _SearchPebbleFromLeft(_Begin, _LogicalEnd - 1, Gems._Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		if (CofferBeginIndex > CofferEndIndex || FirstGemIndex > LastGemIndex ||
			(LastGemIndex - FirstGemIndex) > (CofferEndIndex - CofferBeginIndex))
		{
			return NoPosition;
		}

		CofferEndIndex = _HandleEndIndex(_Begin, _LogicalEnd - 1, CofferEndIndex);

		LastGemIndex = _HandleEndIndex(Gems, _GetLogicalEnd(Gems) - 1, LastGemIndex);

		char* Position = _SearchGemFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems + FirstGemIndex
			, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromRight(const char Gems[]) const
	{
		char* Position = _SearchGemFromRight(_Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems) - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		if (CofferBeginIndex > CofferEndIndex || FirstGemIndex > LastGemIndex ||
			(LastGemIndex - FirstGemIndex) > (CofferEndIndex - CofferBeginIndex))
		{
			return NoPosition;
		}

		CofferEndIndex = _HandleEndIndex(_Begin, _LogicalEnd - 1, CofferEndIndex);

		LastGemIndex = _HandleEndIndex(Gems._Begin, Gems._LogicalEnd - 1, LastGemIndex);

		char* Position = _SearchGemFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems._Begin + FirstGemIndex
			, Gems._LogicalEnd + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromRight(StringKernel& Gems) const
	{

		char* Position = _SearchGemFromRight(_Begin, _LogicalEnd - 1, Gems._Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		if (CofferBeginIndex > CofferEndIndex || FirstGemIndex > LastGemIndex ||
			(LastGemIndex - FirstGemIndex) > (CofferEndIndex - CofferBeginIndex))
		{
			return NoPosition;
		}

		CofferEndIndex = _HandleEndIndex(_Begin, _LogicalEnd - 1, CofferEndIndex);

		LastGemIndex = _HandleEndIndex(Gems, _GetLogicalEnd(Gems) - 1, LastGemIndex);

		char* Position = _SearchPebbleFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems + FirstGemIndex
			, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromRight(const char Gems[]) const
	{
		char* Position = _SearchPebbleFromRight(_Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems) - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		if (CofferBeginIndex > CofferEndIndex || FirstGemIndex > LastGemIndex ||
			(LastGemIndex - FirstGemIndex) > (CofferEndIndex - CofferBeginIndex))
		{
			return NoPosition;
		}

		CofferEndIndex = _HandleEndIndex(_Begin, _LogicalEnd - 1, CofferEndIndex);

		LastGemIndex = _HandleEndIndex(Gems._Begin, Gems._LogicalEnd - 1, LastGemIndex);

		char* Position = _SearchPebbleFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems._Begin + FirstGemIndex
			, Gems._LogicalEnd + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromRight(StringKernel& Gems) const
	{

		char* Position = _SearchGemFromRight(_Begin, _LogicalEnd - 1, Gems._Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	void Replace(unsigned short StartWritingIndex, unsigned short StopWritingIndex, unsigned short StartReadingIndex,
		unsigned short StopReadingIndex)
	{

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