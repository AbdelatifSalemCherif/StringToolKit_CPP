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

	void _Assignement(const char* SourceBegin, const char* SourceLogicalEnd)
	{

		_HandlePhysicalSpace(_Length(SourceBegin, SourceLogicalEnd), false);

		_LogicalEnd = _Write(_Begin, SourceBegin, SourceLogicalEnd, _Left);
	}

	void _Concatenate(const char* Source1Begin, const char* Source1LogicalEnd, const char* Source2Begin, const char* Source2LogicalEnd)
	{

		_HandlePhysicalSpace(_Length(Source1Begin, Source1LogicalEnd) + _Length(Source2Begin, Source2LogicalEnd) - 1, false);

		if (Source1LogicalEnd > Source1Begin)
		{
			_LogicalEnd = _Write(_Begin, Source1Begin, Source1LogicalEnd - 1, _Left);
		}
		else
		{
			_LogicalEnd = _Begin;
		}

		_LogicalEnd = _Write(_LogicalEnd + 1, Source2Begin, Source2LogicalEnd, _Left);
	}

	void _Append(const char* SourceBegin, const char* SourceLogicalEnd)
	{

		_HandlePhysicalSpace(Length() + _Length(SourceBegin, SourceLogicalEnd) - 1, true);

		_LogicalEnd = _Write(_LogicalEnd, SourceBegin, SourceLogicalEnd, _Left);

	}

	void _Insert(char* StartWritting, const char* SourceBegin, const char* SourceLogicalEnd, const char* StartReading
		, const char* StopReading)
	{

		StopReading = _HandleAddressWithinRange(SourceBegin, SourceLogicalEnd - 1, StopReading);

		if (StartReading <= StopReading)
		{
			StartWritting = (char*)_HandleAddressWithinRange(_Begin, _LogicalEnd, StartWritting);

			unsigned short Offset = StopReading - StartReading + 1;

			_HandlePhysicalSpace(Length() + Offset, true);

			_LogicalEnd = _Write(StartWritting + Offset, StartWritting, _LogicalEnd, _Right);

			_Write(StartWritting, StartReading, StopReading, _Left);

		}
	}

	void _Delete(char* StartDelete, char* StopDelete)
	{
		StopDelete = (char*) _HandleAddressWithinRange(_Begin, _LogicalEnd - 1, StopDelete);

		if (StartDelete <= StopDelete)
		{
			_LogicalEnd = _Write(StartDelete, StopDelete + 1, _LogicalEnd, _Left);
		}

	}

	char _at(const char* Address) const
	{

		return (Address >= _Begin && Address < _LogicalEnd) ? *(Address) : '\0';

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

	bool _IsAddressWithinRange(const char* Begin, const char* End, const char* Address) const 
	{
		return Address >= Begin && Address <= End;
	}

	const char* _HandleAddressWithinRange(const char* Begin, const char* End, const char* Address) const
	{
		return _IsAddressWithinRange(Begin, End, Address) ? Address : End;
	}

	char* _SearchNecklaceFromLeft(const char* StartReadingCoffer, const char* StopReadingCoffer, const char* StartReadingNecklace,
		const char* StopReadingNecklace) const
	{

		const char* CofferReader = StartReadingCoffer, * NecklaceReader = StartReadingNecklace;

		while (CofferReader <= StopReadingCoffer)
		{
			if (*CofferReader == *NecklaceReader)
			{

				if (NecklaceReader == StopReadingNecklace)
				{
					return (char*)CofferReader - (StopReadingNecklace - StartReadingNecklace);
				}
				else
				{
					NecklaceReader++;
				}
			}
			else if (NecklaceReader > StartReadingNecklace)
			{
				CofferReader -= NecklaceReader - StartReadingNecklace;

				NecklaceReader = StartReadingNecklace;

			}

			CofferReader++;

		}

		return nullptr;
	}

	char* _SearchNecklaceFromLeft(const char* StartReadingCoffer, const char* StopReadingCoffer, const char* NecklaceBegin, 
		const char* NecklaceLogicalEnd, const char* StartReadingNecklace, const char* StopReadingNecklace) const
	{

		StopReadingCoffer = _HandleAddressWithinRange(_Begin, _LogicalEnd - 1, StopReadingCoffer);

		StopReadingNecklace = _HandleAddressWithinRange(NecklaceBegin, NecklaceLogicalEnd - 1, StopReadingNecklace);

		if (StartReadingCoffer <= StopReadingCoffer && StartReadingNecklace <= StopReadingNecklace &&
			_Length(StartReadingCoffer, StopReadingCoffer) >= _Length(StartReadingNecklace, StopReadingNecklace))
		{
			return _SearchNecklaceFromLeft(StartReadingCoffer, StopReadingCoffer, StartReadingNecklace, StopReadingNecklace);
		}

		return nullptr;
	}

	char* _SearchNecklaceFromRight(const char* StartReadingCoffer, const char* StopReadingCoffer,  const char* StartReadingNecklace,
		const char* StopReadingNecklace) const
	{

		const char* CofferReader = StopReadingCoffer, * NecklaceReader = StopReadingNecklace;

		while (CofferReader >= StartReadingCoffer)
		{
			if (*CofferReader == *NecklaceReader)
			{

				if (NecklaceReader == StartReadingNecklace)
				{
					return (char*)CofferReader;
				}
				else
				{
					NecklaceReader--;
				}
			}
			else if (NecklaceReader < StopReadingNecklace)
			{
				CofferReader += StopReadingNecklace - NecklaceReader;

				NecklaceReader = StopReadingNecklace;

			}

			CofferReader--;
		}

		return nullptr;
	}


	char* _SearchNecklaceFromRight(const char* StartReadingCoffer, const char* StopReadingCoffer, const char* NecklaceBegin, 
		const char* NecklaceLogicalEnd, const char* StartReadingNecklace, const char* StopReadingNecklace) const
	{

		StopReadingCoffer = _HandleAddressWithinRange(_Begin, _LogicalEnd - 1, StopReadingCoffer);

		StopReadingNecklace = _HandleAddressWithinRange(NecklaceBegin, NecklaceLogicalEnd - 1, StopReadingNecklace);

		if (StartReadingCoffer <= StopReadingCoffer && StartReadingNecklace <= StopReadingNecklace &&
			_Length(StartReadingCoffer, StopReadingCoffer) >= _Length(StartReadingNecklace, StopReadingNecklace))
		{
			return _SearchNecklaceFromRight(StartReadingCoffer, StopReadingCoffer, StartReadingNecklace, StopReadingNecklace);
		}

		return nullptr;
	}

	char* _SearchGemFromLeft(const char* StartReadingCoffer, const char* StopReadingCoffer, const char* StartReadingGems,
		const char* StopReadingGems) const
	{
		const char* CofferReader = StartReadingCoffer, * GemsReader = StartReadingGems;

		while (CofferReader <= StopReadingCoffer)
		{
			while (GemsReader <= StopReadingGems)
			{
				if (*CofferReader == *GemsReader)
				{
					return (char*)CofferReader;
				}

				GemsReader++;
			}

			GemsReader = StartReadingGems;
			CofferReader++;
		}

		return nullptr;
	}

	char* _SearchGemFromLeft(const char* StartReadingCoffer, const char* StopReadingCoffer, const char* GemsBegin
		, const char* GemsLogicalEnd, const char* StartReadingGems, const char* StopReadingGems) const
	{

		StopReadingCoffer = _HandleAddressWithinRange(_Begin, _LogicalEnd - 1, StopReadingCoffer);

		StopReadingGems = _HandleAddressWithinRange(GemsBegin, GemsLogicalEnd - 1, StopReadingGems);

		if (StartReadingCoffer <= StopReadingCoffer && StartReadingGems <= StopReadingGems &&
			_Length(StartReadingCoffer, StopReadingCoffer) >= _Length(StartReadingGems, StopReadingGems))
		{
			return _SearchGemFromLeft(StartReadingCoffer, StopReadingCoffer, StartReadingGems, StopReadingGems);
		}

		return nullptr;
	}

	char* _SearchPebbleFromLeft(const char* StartReadingCoffer, const char* StopReadingCoffer, const char* StartReadingGems,
		const char* StopReadingGems) const
	{
		const char* CofferReader = StartReadingCoffer;

		while (CofferReader <= StopReadingCoffer)
		{
			if (_SearchGemFromLeft(StartReadingGems, StopReadingGems, CofferReader, CofferReader) == nullptr)
			{
				return (char*)CofferReader;
			}

			CofferReader++;
		}

		return nullptr;
	}

	char* _SearchPebbleFromLeft(const char* StartReadingCoffer, const char* StopReadingCoffer, const char* GemsBegin
		, const char* GemsLogicalEnd, const char* StartReadingGems, const char* StopReadingGems) const
	{
		StopReadingCoffer = _HandleAddressWithinRange(_Begin, _LogicalEnd - 1, StopReadingCoffer);

		StopReadingGems = _HandleAddressWithinRange(GemsBegin, GemsLogicalEnd - 1, StopReadingGems);

		if (StartReadingCoffer <= StopReadingCoffer && StartReadingGems <= StopReadingGems &&
			_Length(StartReadingCoffer, StopReadingCoffer) >= _Length(StartReadingGems, StopReadingGems))
		{
			return _SearchPebbleFromLeft(StartReadingCoffer, StopReadingCoffer, StartReadingGems, StopReadingGems);
		}

		return nullptr;
	}

	char* _SearchGemFromRight(const char* StartReadinCoffer, const char* StopReadingCoffer, const char* StartReadingGems,
		const char* StopReadingGems) const
	{
		const char* CofferReader = StopReadingCoffer, * GemsReader = StopReadingGems;

		while (CofferReader >= StartReadinCoffer)
		{
			while (GemsReader >= StopReadingGems)
			{
				if (*CofferReader == *GemsReader)
				{
					return (char*)CofferReader;
				}

				GemsReader--;
			}

			GemsReader = StopReadingGems;
			CofferReader--;
		}

		return nullptr;
	}

	char* _SearchGemFromRight(const char* StartReadingCoffer, const char* StopReadingCoffer, const char* GemsBegin,
		const char* GemsLogicalEnd, const char* StartReadingGems, const char* StopReadingGems) const
	{
		StopReadingCoffer = _HandleAddressWithinRange(_Begin, _LogicalEnd - 1, StopReadingCoffer);

		StopReadingGems = _HandleAddressWithinRange(GemsBegin, GemsLogicalEnd - 1, StopReadingGems);

		if (StartReadingCoffer <= StopReadingCoffer && StartReadingGems <= StopReadingGems &&
			_Length(StartReadingCoffer, StopReadingCoffer) >= _Length(StartReadingGems, StopReadingGems))
		{
			return _SearchGemFromRight(StartReadingCoffer, StopReadingCoffer, StartReadingGems, StopReadingGems);
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

	char* _SearchPebbleFromRight(const char* StartReadingCoffer, const char* StopReadingCoffer, const char* GemsBegin
		, const char* GemsLogicalEnd, const char* StartReadingGems, const char* StopReadingGems) const
	{
		StopReadingCoffer = _HandleAddressWithinRange(_Begin, _LogicalEnd - 1, StopReadingCoffer);

		StopReadingGems = _HandleAddressWithinRange(GemsBegin, GemsLogicalEnd - 1, StopReadingGems);

		if (StartReadingCoffer <= StopReadingCoffer && StartReadingGems <= StopReadingGems &&
			_Length(StartReadingCoffer, StopReadingCoffer) >= _Length(StartReadingGems, StopReadingGems))
		{
			return _SearchPebbleFromRight(StartReadingCoffer, StopReadingCoffer, StartReadingGems, StopReadingGems);
		}

		return nullptr;
	}

public:

	static const unsigned short NoPosition = -1;

	StringKernel()
	{
		_Begin = new char[16];

		_PhysicalEnd = _GetNewPhysicalEnd(16);

		Clear();
	}

	StringKernel(const char Value[])
	{
		const char* ValueLogicalEnd = _GetLogicalEnd(Value);

		const unsigned short Capacity = _GetNewCapacity(_Length(Value, ValueLogicalEnd));

		_Begin = new char[Capacity];

		_PhysicalEnd = _GetNewPhysicalEnd(Capacity);

		_LogicalEnd = _Write(_Begin, Value, ValueLogicalEnd, _Left);

	}

	StringKernel(const StringKernel& Value)
	{
		const unsigned short Capacity = _GetNewCapacity(Value.Length());

		_Begin = new char[Capacity];

		_PhysicalEnd = _GetNewPhysicalEnd(Capacity);

		_LogicalEnd = _Write(_Begin, Value._Begin, Value._LogicalEnd, _Left);

	}

	~StringKernel()
	{
		cout << "Hi Destructor !" << endl;

		delete[] _Begin;
	}

	const char* Value() const
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
		_Assignement(Source, _GetLogicalEnd(Source));
	}

	void Assignment(const StringKernel& Source)
	{
		_Assignement(Source._Begin, Source._LogicalEnd);
	}

	void Copy(const char Source[])
	{
		_Assignement(Source, _GetLogicalEnd(Source));
	}

	void Copy(const StringKernel& Source)
	{
		_Assignement(Source._Begin, Source._LogicalEnd);
	}

	void Concatenate(const char Source1[], const char Source2[])
	{
		_Concatenate(Source1, _GetLogicalEnd(Source1), Source2, _GetLogicalEnd(Source2));
	}

	void Concatenate(const char Source1[], const StringKernel& Source2)
	{
		_Concatenate(Source1, _GetLogicalEnd(Source1), Source2._Begin, Source2._LogicalEnd);
	}

	void Concatenate(const StringKernel& Source1, const char Source2[])
	{
		_Concatenate(Source1._Begin, Source1._LogicalEnd, Source2, _GetLogicalEnd(Source2));
	}

	void Concatenate(const StringKernel& Source1, const StringKernel& Source2)
	{
		_Concatenate(Source1._Begin, Source1._LogicalEnd, Source2._Begin, Source2._LogicalEnd);
	}

	void Append(const char Source[])
	{
		_Append(Source, _GetLogicalEnd(Source));
	}

	void Append(const StringKernel& Source)
	{
		_Append(Source._Begin, Source._LogicalEnd);
	}

	void Insert(unsigned short StartWrittingIndex, const char Source[], unsigned short StartReadingIndex, 
		unsigned short StopReadingIndex)
	{
		_Insert(_Begin + StartWrittingIndex, Source, _GetLogicalEnd(Source), Source + StartReadingIndex, Source + StopReadingIndex);
	}

	void Insert(unsigned short StartWrittingIndex, const char Source[])
	{		
		
		const char* SourceLogicalEnd = _GetLogicalEnd(Source);

		_Insert(_Begin + StartWrittingIndex, Source, SourceLogicalEnd, Source, SourceLogicalEnd - 1);
	}

	void Insert(unsigned short StartWrittingIndex, const StringKernel& Source, unsigned short StartReadingIndex,
		unsigned short StopReadingIndex)
	{
		_Insert(_Begin + StartWrittingIndex, Source._Begin, Source._LogicalEnd, Source._Begin + StartReadingIndex
			, Source._Begin + StopReadingIndex);
			
	}

	void Insert(unsigned short StartWrittingIndex, const StringKernel& Source)
	{
		_Insert(_Begin + StartWrittingIndex, Source._Begin, Source._LogicalEnd, Source._Begin, Source._LogicalEnd - 1);
	}

	void Delete(unsigned short StartDeleteIndex, unsigned short StopDeleteIndex)
	{
		_Delete(_Begin + StartDeleteIndex, _Begin + StopDeleteIndex);

	}

	void Preppend(const char Source[])
	{
		const char* SourceLogicalEnd = _GetLogicalEnd(Source);

		_Insert(_Begin, Source, SourceLogicalEnd, Source, SourceLogicalEnd - 1);
	}

	void Preppend(const StringKernel& Source)
	{
		_Insert(_Begin, Source._Begin, Source._LogicalEnd, Source._Begin, Source._LogicalEnd - 1);
	}

	void Swap(StringKernel& Data2)
	{
		_Swap2Address((const char**)& _Begin, (const char**)& Data2._Begin);
		_Swap2Address((const char**)& _LogicalEnd, (const char**)& Data2._LogicalEnd);
		_Swap2Address((const char**)& _PhysicalEnd, (const char**)& Data2._PhysicalEnd);

	}

	char at(unsigned short Index) const
	{
		return _at(_Begin + Index);
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
	}

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]
		, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char * Position = _SearchNecklaceFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Necklace, _GetLogicalEnd(Necklace)
			,Necklace + NecklaceBeginIndex, Necklace + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);

	}

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]) const
	{

		const char* NecklaceLogicalEnd = _GetLogicalEnd(Necklace);

		char* Position = _SearchNecklaceFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Necklace, NecklaceLogicalEnd,
			Necklace, NecklaceLogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);

	}

	unsigned short SearchNecklaceFromLeft(const char Necklace[], unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char* Position = _SearchNecklaceFromLeft(_Begin, _LogicalEnd - 1, Necklace, _GetLogicalEnd(Necklace)
			, Necklace + NecklaceBeginIndex, Necklace + NecklaceEndIndex);

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
		
		char* Position = _SearchNecklaceFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Necklace._Begin
			, Necklace._LogicalEnd, Necklace._Begin + NecklaceBeginIndex, Necklace._Begin + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Necklace) const
	{

		char* Position = _SearchNecklaceFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Necklace._Begin, 
			Necklace._LogicalEnd, Necklace._Begin, Necklace._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);

	}

	unsigned short SearchNecklaceFromLeft(StringKernel& Necklace, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char* Position = _SearchNecklaceFromLeft(_Begin, _LogicalEnd - 1, Necklace._Begin, Necklace._LogicalEnd
			, Necklace._Begin + NecklaceBeginIndex, Necklace._Begin + NecklaceEndIndex);

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
		
		char* Position = _SearchNecklaceFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Necklace
			, _GetLogicalEnd(Necklace) , Necklace + NecklaceBeginIndex, Necklace + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchNecklaceFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]) const
	{

		const char* NecklaceLogicalEnd = _GetLogicalEnd(Necklace);

		char* Position = _SearchNecklaceFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Necklace, NecklaceLogicalEnd,
			Necklace, NecklaceLogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchNecklaceFromRight(const char Necklace[], unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char* Position = _SearchNecklaceFromRight(_Begin, _LogicalEnd - 1, Necklace, _GetLogicalEnd(Necklace)
			, Necklace + NecklaceBeginIndex, Necklace + NecklaceEndIndex);

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
		
		char* Position = _SearchNecklaceFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Necklace._Begin
			, Necklace._LogicalEnd, Necklace._Begin + NecklaceBeginIndex, Necklace._Begin + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchNecklaceFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Necklace) const
	{

		char* Position = _SearchNecklaceFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Necklace._Begin,
			Necklace._LogicalEnd, Necklace._Begin, Necklace._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchNecklaceFromRight(StringKernel& Necklace, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char* Position = _SearchNecklaceFromRight(_Begin, _LogicalEnd - 1, Necklace._Begin, Necklace._LogicalEnd
			, Necklace._Begin + NecklaceBeginIndex, Necklace._Begin + NecklaceEndIndex);

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

		char* Position = _SearchGemFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems, _GetLogicalEnd(Gems),
			Gems + FirstGemIndex, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]) const
	{
		const char* GemsLogicalEnd = _GetLogicalEnd(Gems);

		char* Position = _SearchGemFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems, GemsLogicalEnd, 
			Gems, GemsLogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromLeft(const char Gems[], unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		
		char* Position = _SearchGemFromLeft(_Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems), Gems + FirstGemIndex
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
		
		char* Position = _SearchGemFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems._Begin, Gems._LogicalEnd
			, Gems._Begin + FirstGemIndex, Gems._Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems) const
	{

		char* Position = _SearchGemFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems._Begin, Gems._LogicalEnd
			, Gems._Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromLeft(StringKernel& Gems, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGemFromLeft(_Begin, _LogicalEnd - 1, Gems._Begin, Gems._LogicalEnd, Gems._Begin + FirstGemIndex,
			Gems._Begin + LastGemIndex);

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
		
		char* Position = _SearchPebbleFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex,Gems, _GetLogicalEnd(Gems),
			Gems + FirstGemIndex, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]) const
	{
		const char* GemsLogicalEnd = _GetLogicalEnd(Gems);

		char* Position = _SearchPebbleFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems, GemsLogicalEnd,
			Gems, GemsLogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromLeft(const char Gems[], unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebbleFromLeft(_Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems), Gems + FirstGemIndex,
			Gems + LastGemIndex);

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

		char* Position = _SearchPebbleFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems._Begin, Gems._LogicalEnd
			, Gems._Begin + FirstGemIndex, Gems._Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems) const
	{

		char* Position = _SearchPebbleFromLeft(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems._Begin, Gems._LogicalEnd
			, Gems._Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromLeft(StringKernel& Gems, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebbleFromLeft(_Begin, _LogicalEnd, Gems._Begin, Gems._LogicalEnd, Gems._Begin + FirstGemIndex,
			Gems._Begin + LastGemIndex);

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

		char* Position = _SearchGemFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems, _GetLogicalEnd(Gems),
			Gems + FirstGemIndex, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]) const
	{
		const char* GemsLogicalEnd = _GetLogicalEnd(Gems);

		char* Position = _SearchGemFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems, GemsLogicalEnd,
			Gems, GemsLogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromRight(const char Gems[], unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGemFromRight(_Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems), Gems + FirstGemIndex,
			Gems + LastGemIndex);

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

		char* Position = _SearchGemFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems._Begin, Gems._LogicalEnd,
			Gems._Begin + FirstGemIndex, Gems._Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems) const
	{

		char* Position = _SearchGemFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems._Begin, Gems._LogicalEnd,
			Gems._Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchGemFromRight(StringKernel& Gems, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGemFromRight(_Begin, _LogicalEnd, Gems._Begin, Gems._LogicalEnd, Gems._Begin + FirstGemIndex,
			Gems._Begin + LastGemIndex);

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

		char* Position = _SearchPebbleFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems, _GetLogicalEnd(Gems),
			Gems + FirstGemIndex, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]) const
	{

		const char* GemsLogicalEnd = _GetLogicalEnd(Gems);

		char* Position = _SearchPebbleFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems, GemsLogicalEnd,
			Gems, GemsLogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromRight(const char Gems[], unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebbleFromRight(_Begin, _LogicalEnd, Gems, _GetLogicalEnd(Gems), Gems + FirstGemIndex, 
			Gems + LastGemIndex);

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
	
		char* Position = _SearchPebbleFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems._Begin, Gems._LogicalEnd,
			Gems._Begin + FirstGemIndex, Gems._Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems) const
	{

		char* Position = _SearchPebbleFromRight(_Begin + CofferBeginIndex, _Begin + CofferEndIndex, Gems._Begin, Gems._LogicalEnd,
			Gems._Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromRight(StringKernel& Gems, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebbleFromRight(_Begin, _LogicalEnd, Gems._Begin, Gems._LogicalEnd, Gems._Begin + FirstGemIndex, 
			Gems._Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	unsigned short SearchPebbleFromRight(StringKernel& Gems) const
	{

		char* Position = _SearchGemFromRight(_Begin, _LogicalEnd - 1, Gems._Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - _Begin);
	}

	void Replace(unsigned short StartWritingIndex, unsigned short StopWritingIndex, const char Source[],
		unsigned short StartReadingIndex, unsigned short StopReadingIndex)
	{
		/*if ()
		{

		}*/
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