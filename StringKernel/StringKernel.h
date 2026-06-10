#pragma once

#include<iostream>

using namespace std;

class StringKernel
{
private:

	struct _stRange
	{
		char* _Start;
		char* _Stop;
	};

	struct _stBounds
	{
		_stRange _LogicalRange;
		char* _PhysicalEnd;
	};

	_stBounds _Bounds;

	_stRange _RangeAssignement(char* Start, char* Stop) const
	{
		_stRange Range;

		Range._Start = Start;
		Range._Stop = Stop;

		return Range;
	}

	_stBounds _BoundsAssignement(const _stRange& LogicalRange, char* PhysicalEnd) const
	{
		_stBounds Bounds;

		Bounds._LogicalRange = LogicalRange;
		Bounds._PhysicalEnd = PhysicalEnd;

		return Bounds;
	}

	void _AddStopCharacter(_stRange& Range) const
	{
		*Range._Stop = '\0';
	}

	void _Clear(_stRange& Range) const
	{
		Range._Stop = Range._Start;

		_AddStopCharacter(Range);
	}

	unsigned short _Size(const _stRange& Range) const
	{
		return Range._Stop - Range._Start;
	}

	unsigned short _Length(const _stRange& Range) const
	{
		return Range._Stop - Range._Start + 1;
	}

	unsigned short _Capacity(const _stBounds& Bounds) const
	{
		return Bounds._PhysicalEnd - Bounds._LogicalRange._Start + 1;
	}

	unsigned short _GetNewCapacity(const unsigned short& NewLength) const
	{

		return (NewLength + 15) & ~15;
	}

	bool _HaveMoreSpace(const _stBounds& Bounds, unsigned short NewLength) const
	{
		return NewLength <= _Capacity(Bounds);
	}

	char* _GetNewPhysicalEnd(char* Start, unsigned short Capacity) const
	{
		return Start + Capacity - 1;
	}

	enum _enStartFrom { _Left, _Right };

	char* _Write(char* StartWrite, const _stRange& ReadingRange, _enStartFrom StartWriteFrom) const
	{
		char* StropWrite = StartWrite + _Length(ReadingRange) - 1;

		if (StartWriteFrom == _Left)
		{
			char* Writer = StartWrite, * Reader = ReadingRange._Start;

			while (Reader <= ReadingRange._Stop)
			{
				*Writer++ = *Reader++;
			}
		}
		else
		{
			char* Writer = StropWrite , * Reader = ReadingRange._Stop;

			while (Reader >= ReadingRange._Start)
			{
				*Writer-- = *Reader--;
			}
		}

		return StropWrite;
	}

	_stRange _Assignement(_stRange WritingRange, const _stRange& ReadingRange) const
	{

		WritingRange._Stop = _Write(WritingRange._Start, ReadingRange, _Left);

		return WritingRange;
	}

	_stRange _Concatenate(_stRange WritingRange, const _stRange& ReadingRange1, const _stRange& ReadingRange2) const
	{

		WritingRange._Stop = _Write(_Write(WritingRange._Start, ReadingRange1, _Left) + 1, ReadingRange2, _Left);

		return WritingRange;
	}

	_stRange _Append(_stRange WritingRange, const _stRange& ReadingRange) const
	{

		WritingRange._Stop = _Write(WritingRange._Stop + 1, ReadingRange, _Left);

		return WritingRange;
	}

	_stRange _Preppend(_stRange WritingRange, const _stRange& ReadingRange) const
	{

		WritingRange._Stop = _Write(WritingRange._Start + _Length(ReadingRange), WritingRange, _Right);

		_Write(WritingRange._Start, ReadingRange, _Left);

		return WritingRange;

	}

	_stRange _Insert(_stRange WritingRange, char* StartWrite, const _stRange& ReadingRange) const
	{

		char* StopWrite = StartWrite + _Length(ReadingRange) - 1;

		WritingRange._Stop = _Write(StopWrite + 1, WritingRange, _Right);

		_Write(StartWrite, ReadingRange, _Right);

		return WritingRange;
	}

	_stRange _Delete(_stRange WritingRange, const _stRange& DeletingRange) const
	{
		WritingRange._Stop = _Write(DeletingRange._Start, _RangeAssignement(DeletingRange._Stop + 1, WritingRange._Stop), _Left);

		return WritingRange;
	}

	void _Reallocate(_stBounds& Bounds, unsigned short NewLogicalRangeLength, bool DoSaveData) const
	{

		if (!_HaveMoreSpace(Bounds, NewLogicalRangeLength))
		{
			_stRange LastLogicalRange = Bounds._LogicalRange;

			unsigned short Capacity = _GetNewCapacity(NewLogicalRangeLength);

			Bounds._LogicalRange._Start = new char[Capacity];

			Bounds._PhysicalEnd = _GetNewPhysicalEnd(Bounds._LogicalRange._Start, Capacity);

			if (DoSaveData)
			{

				Bounds._LogicalRange._Stop = _Write(Bounds._LogicalRange._Start, LastLogicalRange, _Left);

			}
			else
			{
				_Clear(Bounds._LogicalRange);
			}

			delete[] LastLogicalRange._Start;
		}
		else if (!DoSaveData)
		{
			_Clear(Bounds._LogicalRange);
		}
	}

protected:

	

	char* _GetBegin()
	{
		return _Begin;
	}

	char* _GetLogicalEnd()
	{
		return _LogicalEnd;
	}

	char* _GetPhysicalEnd()
	{
		return _PhysicalEnd;
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

	unsigned short _Size(const _stRange& Range) const
	{

		return (Range._Start < Range._Stop) ? _Size(Range._Start, Range._Stop) : _Size(Range._Stop, Range._Start);
	}

	unsigned short _Length(const _stRange& Range) const
	{
		return (Range._Start < Range._Stop) ? _Length(Range._Start, Range._Stop) : _Length(Range._Stop, Range._Start);
	}

	void _AddStopCharacter()
	{
		*_LogicalEnd = '\0';
	}

	void _Reallocate(unsigned short NewLength, bool DoSaveData)
	{
		_Reallocate(_Begin, _LogicalEnd, _PhysicalEnd, NewLength, DoSaveData);

	}

	void _Assignement(const char* SourceBegin, const char* SourceLogicalEnd)
	{

		_Reallocate(_Length(SourceBegin, SourceLogicalEnd), false);

		_LogicalEnd = _Write(_Begin, SourceBegin, SourceLogicalEnd, _Left);
	}

	void _Concatenate(const char* Source1Begin, const char* Source1LogicalEnd, const char* Source2Begin, const char* Source2LogicalEnd)
	{

		_Reallocate(_Length(Source1Begin, Source1LogicalEnd) + _Length(Source2Begin, Source2LogicalEnd) - 1, false);

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

		_Reallocate(Length() + _Length(SourceBegin, SourceLogicalEnd) - 1, true);

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

			_Reallocate(Length() + Offset, true);

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

	static bool IsSameData(const StringKernel& Data1, const StringKernel& Data2)
	{
		const char* Begin1 = Data1._Begin, * Begin2 = Data2._Begin;

		bool IsSame = Data1.Length() == Data2.Length();

		while (IsSame && (Data1._Begin < Data1._LogicalEnd))
		{
			IsSame = Begin1++ == Begin2++;
		}

		return IsSame;
	}

	bool IsSameData(const StringKernel& Data2) const
	{
		return IsSameData(*this, Data2);
	}

	void Swap(StringKernel& Data2)
	{
		_Swap2Address((const char**)&_Begin, (const char**)&Data2._Begin);
		_Swap2Address((const char**)&_LogicalEnd, (const char**)&Data2._LogicalEnd);
		_Swap2Address((const char**)&_PhysicalEnd, (const char**)&Data2._PhysicalEnd);

	}

	char at(unsigned short Index) const
	{
		return _at(_Begin + Index);
	}

	void PushBack(const char& NewCharacter)
	{
		_Reallocate(Length() + 1, true);

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