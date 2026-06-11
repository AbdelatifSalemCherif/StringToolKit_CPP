#pragma once

#include<iostream>

using namespace std;


class StringKernel
{
private:
	
	class _clsRange
	{
	public:

		char* Begin;
		char* End;

		_clsRange(char* Begin_, char* End_)
		{
			Begin = Begin_;
			End = End_;
		}

		static _clsRange NewRange(char* Begin_, char* End_)
		{
			_clsRange Range(Begin_, End_);

			return Range;
		}

		_clsRange HandleRangeToBeInside(_clsRange Range) const
		{
			if (Range.Begin < Begin)
			{
				Range.Begin = Begin;
			}

			if (Range.End > End)
			{
				Range.End = End;
			}

			return Range;
		}

		enum enStartFrom { Left, Right };

		char* Write(char* StartWrite, const _clsRange& ReadingRange, enStartFrom StartWriteFrom) const
		{
			char* StropWrite = StartWrite + ReadingRange.Length() - 1;

			if (StartWriteFrom == Left)
			{
				char* Writer = StartWrite, * Reader = ReadingRange.Begin;

				while (Reader <= ReadingRange.End)
				{
					*Writer++ = *Reader++;
				}
			}
			else
			{
				char* Writer = StropWrite, * Reader = ReadingRange.End;

				while (Reader >= ReadingRange.Begin)
				{
					*Writer-- = *Reader--;
				}
			}

			return StropWrite;
		}

		void AddStopCharacter() const
		{
			*End = '\0';
		}

		void Clear()
		{
			End = Begin;

			AddStopCharacter();
		}

		char at(const char* At) const
		{

			return (At >= Begin && At < End) ? *(At) : '\0';

		}

		unsigned short Size() const
		{
			return End - Begin;
		}

		unsigned short Length() const
		{
			return End - Begin + 1;
		}

		void Assignement(const _clsRange& New)
		{

			End = Write(Begin, New, Left);

		}

		void Concatenate(const _clsRange& New1, const _clsRange& New2)
		{

			End = Write(Write(Begin, New1, Left) + 1, New2, Left);

		}

		void Append(const _clsRange& New)
		{

			End = Write(End + 1, New, Left);

		}

		void Preppend(const _clsRange& New)
		{

			End = Write(Begin + New.Length(), *this, Right);

			Write(Begin, New, Left);

		}

		void Insert(char* StartWrite, const _clsRange& New)
		{

			char* StopWrite = StartWrite + New.Length() - 1;

			End = Write(StopWrite + 1, *this, Right);

			Write(StartWrite, New, Right);

		}

		void Delete(const _clsRange& DeleteRange)
		{

			End = Write(DeleteRange.Begin, NewRange(DeleteRange.End + 1, End), Left);

		}

		char* SearchNecklace(const _clsRange& Necklace, enStartFrom StratSearchFrom) const
		{

			if (StratSearchFrom == Left)
			{
				char* CofferReader = Begin, * NecklaceReader = Necklace.Begin;

				while (CofferReader <= End)
				{
					if (*CofferReader == *NecklaceReader)
					{

						if (NecklaceReader == Necklace.End)
						{
							return CofferReader - (Necklace.End - Necklace.Begin);
						}
						else
						{
							NecklaceReader++;
						}
					}
					else if (NecklaceReader > Necklace.Begin)
					{
						CofferReader -= NecklaceReader - Necklace.Begin;

						NecklaceReader = Necklace.Begin;

					}

					CofferReader++;

				}
			}
			else
			{
				char* CofferReader = End, * NecklaceReader = Necklace.End;

				while (CofferReader >= Begin)
				{
					if (*CofferReader == *NecklaceReader)
					{

						if (NecklaceReader == Necklace.Begin)
						{
							return CofferReader;
						}
						else
						{
							NecklaceReader--;
						}
					}
					else if (NecklaceReader < Necklace.End)
					{
						CofferReader += Necklace.End - NecklaceReader;

						NecklaceReader = Necklace.End;

					}

					CofferReader--;
				}
			}

			return nullptr;
		}

		char* SearchGem(const _clsRange& Gems, enStartFrom StartSearchFrom) const
		{

			if (StartSearchFrom == Left)
			{
				for (char* CofferReader = Begin; CofferReader <= End; CofferReader++)
				{
					for (char* GemsReader = Gems.Begin; GemsReader <= Gems.End; GemsReader++)
					{
						if (*CofferReader == *GemsReader)
						{
							return CofferReader;
						}
					}
				}
			}
			else
			{
				for (char* CofferReader = End; CofferReader >= Begin; CofferReader--)
				{
					for (char* GemsReader = Gems.End; GemsReader >= Gems.Begin; GemsReader--)
					{
						if (*CofferReader == *GemsReader)
						{
							return CofferReader;
						}
					}
				}
			}

			return nullptr;
		}

		char* SearchPebble(const _clsRange& Gems, enStartFrom StartSearchFrom) const
		{
			
			if (StartSearchFrom == Left)
			{
				for (char* CofferReader = Begin; CofferReader <= End; CofferReader++)
				{
					if (Gems.SearchGem(NewRange(CofferReader, CofferReader), Left) == nullptr)
					{
						return CofferReader;
					}

				}
			}
			else
			{
				for (char* CofferReader = End; CofferReader >= Begin; CofferReader--)
				{
					if (Gems.SearchGem(NewRange(CofferReader, CofferReader), Left) == nullptr)
					{
						return CofferReader;
					}
				}
			}

			return nullptr;
		}


	};

	_clsRange _LogicalRange;

	char* _PhysicalEnd;
	
	unsigned short _GetNewCapacity(const unsigned short& NewLength) const
	{

		return (NewLength + 15) & ~15;
	}

	bool _HaveMoreSpace(unsigned short NewLength) const
	{
		return NewLength <= _Capacity(clsBounds);
	}

	char* _GetNewPhysicalEnd(char* Start, unsigned short Capacity) const
	{
		return Start + Capacity - 1;
	}
protected:

	char* _GetBegin() const
	{
		return _LogicalRange.Begin;
	}

	char* _GetLogicalEnd() const
	{
		return _LogicalRange.End;
	}

	char* _GetPhysicalEnd() const
	{
		return _PhysicalEnd;
	}

	_clsRange GetLogicalRange() const
	{
		return _LogicalRange;
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

	unsigned short _Capacity(const clsBounds& clsBounds) const
	{
		return clsBounds._PhysicalEnd - clsBounds._LogicalRange.Begin + 1;
	}

	void _Reallocate(clsBounds& clsBounds, unsigned short NewLogicalRangeLength, bool DoSaveData) const
	{

		if (!_HaveMoreSpace(clsBounds, NewLogicalRangeLength))
		{
			_clsRange LastLogicalRange = clsBounds._LogicalRange;

			unsigned short Capacity = _GetNewCapacity(NewLogicalRangeLength);

			clsBounds._LogicalRange.Begin = new char[Capacity];

			clsBounds._PhysicalEnd = _GetNewPhysicalEnd(clsBounds._LogicalRange.Begin, Capacity);

			if (DoSaveData)
			{

				clsBounds._LogicalRange.End = Write(clsBounds._LogicalRange.Begin, LastLogicalRange, Left);

			}
			else
			{
				_Clear(clsBounds._LogicalRange);
			}

			delete[] LastLogicalRange.Begin;
		}
		else if (!DoSaveData)
		{
			_Clear(clsBounds._LogicalRange);
		}
	}

	void _Assignement(const char* SourceBegin, const char* SourceLogicalEnd)
	{

		_Reallocate(_Length(SourceBegin, SourceLogicalEnd), false);

		_LogicalEnd = Write(Begin, SourceBegin, SourceLogicalEnd, Left);
	}

	void _Concatenate(const char* Source1Begin, const char* Source1LogicalEnd, const char* Source2Begin, const char* Source2LogicalEnd)
	{

		_Reallocate(_Length(Source1Begin, Source1LogicalEnd) + _Length(Source2Begin, Source2LogicalEnd) - 1, false);

		if (Source1LogicalEnd > Source1Begin)
		{
			_LogicalEnd = Write(Begin, Source1Begin, Source1LogicalEnd - 1, Left);
		}
		else
		{
			_LogicalEnd = Begin;
		}

		_LogicalEnd = Write(_LogicalEnd + 1, Source2Begin, Source2LogicalEnd, Left);
	}

	void _Append(const char* SourceBegin, const char* SourceLogicalEnd)
	{

		_Reallocate(Length() + _Length(SourceBegin, SourceLogicalEnd) - 1, true);

		_LogicalEnd = Write(_LogicalEnd, SourceBegin, SourceLogicalEnd, Left);

	}

	void _Insert(char* StartWritting, const char* SourceBegin, const char* SourceLogicalEnd, const char* StartReading
		, const char* StopReading)
	{

		StopReading = _HandleAddressWithinRange(SourceBegin, SourceLogicalEnd - 1, StopReading);

		if (StartReading <= StopReading)
		{
			StartWritting = (char*)_HandleAddressWithinRange(Begin, _LogicalEnd, StartWritting);

			unsigned short Offset = StopReading - StartReading + 1;

			_Reallocate(Length() + Offset, true);

			_LogicalEnd = Write(StartWritting + Offset, StartWritting, _LogicalEnd, Right);

			Write(StartWritting, StartReading, StopReading, Left);

		}
	}

	void _Delete(char* StartDelete, char* StopDelete)
	{
		StopDelete = (char*) _HandleAddressWithinRange(Begin, _LogicalEnd - 1, StopDelete);

		if (StartDelete <= StopDelete)
		{
			_LogicalEnd = Write(StartDelete, StopDelete + 1, _LogicalEnd, Left);
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

		StopReadingCoffer = _HandleAddressWithinRange(Begin, _LogicalEnd - 1, StopReadingCoffer);

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

		StopReadingCoffer = _HandleAddressWithinRange(Begin, _LogicalEnd - 1, StopReadingCoffer);

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

		StopReadingCoffer = _HandleAddressWithinRange(Begin, _LogicalEnd - 1, StopReadingCoffer);

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
		StopReadingCoffer = _HandleAddressWithinRange(Begin, _LogicalEnd - 1, StopReadingCoffer);

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
		StopReadingCoffer = _HandleAddressWithinRange(Begin, _LogicalEnd - 1, StopReadingCoffer);

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
		StopReadingCoffer = _HandleAddressWithinRange(Begin, _LogicalEnd - 1, StopReadingCoffer);

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
		Begin = new char[16];

		_PhysicalEnd = _GetNewPhysicalEnd(16);

		Clear();
	}

	StringKernel(const char Value[])
	{
		const char* ValueLogicalEnd = _GetLogicalEnd(Value);

		const unsigned short Capacity = _GetNewCapacity(_Length(Value, ValueLogicalEnd));

		Begin = new char[Capacity];

		_PhysicalEnd = _GetNewPhysicalEnd(Capacity);

		_LogicalEnd = Write(Begin, Value, ValueLogicalEnd, Left);

	}

	StringKernel(const StringKernel& Value)
	{
		const unsigned short Capacity = _GetNewCapacity(Value.Length());

		Begin = new char[Capacity];

		_PhysicalEnd = _GetNewPhysicalEnd(Capacity);

		_LogicalEnd = Write(Begin, Value.Begin, Value._LogicalEnd, Left);

	}

	~StringKernel()
	{
		cout << "Hi Destructor !" << endl;

		delete[] Begin;
	}

	const char* Value() const
	{
		return Begin;
	}

	void Reallocate(unsigned short NewLength, bool DoSaveData)
	{
		_Reallocate(_Bounds, NewLength, DoSaveData);

	}

	void Clear()
	{

		_LogicalEnd = Begin;

		_AddStopCharacter();
	}

	bool IsEmpty() const
	{
		return _LogicalEnd == Begin;
	}

	bool IsFull() const
	{
		return _LogicalEnd == _PhysicalEnd;
	}

	unsigned short Size() const
	{
		return _Size(Begin, _LogicalEnd);
	}

	unsigned short Length() const
	{
		return _Length(Begin, _LogicalEnd);
	}

	unsigned short Capacity() const
	{
		return _PhysicalEnd - Begin + 1;
	}

	static bool IsSameData(const StringKernel& Data1, const StringKernel& Data2)
	{
		const char* Begin1 = Data1.Begin, * Begin2 = Data2.Begin;

		bool IsSame = Data1.Length() == Data2.Length();

		while (IsSame && (Data1.Begin < Data1._LogicalEnd))
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
		_Swap2Address((const char**)&Begin, (const char**)&Data2.Begin);
		_Swap2Address((const char**)&_LogicalEnd, (const char**)&Data2._LogicalEnd);
		_Swap2Address((const char**)&_PhysicalEnd, (const char**)&Data2._PhysicalEnd);

	}

	char at(unsigned short Index) const
	{
		return _at(Begin + Index);
	}

	void PushBack(const char& NewCharacter)
	{
		_Reallocate(Length() + 1, true);

		*_LogicalEnd++ = NewCharacter;

		_AddStopCharacter();
	}

	void PopBack()
	{
		if (_LogicalEnd > Begin)
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
		_Assignement(Source.Begin, Source._LogicalEnd);
	}

	void Copy(const char Source[])
	{
		_Assignement(Source, _GetLogicalEnd(Source));
	}

	void Copy(const StringKernel& Source)
	{
		_Assignement(Source.Begin, Source._LogicalEnd);
	}

	void Concatenate(const char Source1[], const char Source2[])
	{
		_Concatenate(Source1, _GetLogicalEnd(Source1), Source2, _GetLogicalEnd(Source2));
	}

	void Concatenate(const char Source1[], const StringKernel& Source2)
	{
		_Concatenate(Source1, _GetLogicalEnd(Source1), Source2.Begin, Source2._LogicalEnd);
	}

	void Concatenate(const StringKernel& Source1, const char Source2[])
	{
		_Concatenate(Source1.Begin, Source1._LogicalEnd, Source2, _GetLogicalEnd(Source2));
	}

	void Concatenate(const StringKernel& Source1, const StringKernel& Source2)
	{
		_Concatenate(Source1.Begin, Source1._LogicalEnd, Source2.Begin, Source2._LogicalEnd);
	}

	void Append(const char Source[])
	{
		_Append(Source, _GetLogicalEnd(Source));
	}

	void Append(const StringKernel& Source)
	{
		_Append(Source.Begin, Source._LogicalEnd);
	}

	void Insert(unsigned short StartWrittingIndex, const char Source[], unsigned short StartReadingIndex, 
		unsigned short StopReadingIndex)
	{
		_Insert(Begin + StartWrittingIndex, Source, _GetLogicalEnd(Source), Source + StartReadingIndex, Source + StopReadingIndex);
	}

	void Insert(unsigned short StartWrittingIndex, const char Source[])
	{		
		
		const char* SourceLogicalEnd = _GetLogicalEnd(Source);

		_Insert(Begin + StartWrittingIndex, Source, SourceLogicalEnd, Source, SourceLogicalEnd - 1);
	}

	void Insert(unsigned short StartWrittingIndex, const StringKernel& Source, unsigned short StartReadingIndex,
		unsigned short StopReadingIndex)
	{
		_Insert(Begin + StartWrittingIndex, Source.Begin, Source._LogicalEnd, Source.Begin + StartReadingIndex
			, Source.Begin + StopReadingIndex);
			
	}

	void Insert(unsigned short StartWrittingIndex, const StringKernel& Source)
	{
		_Insert(Begin + StartWrittingIndex, Source.Begin, Source._LogicalEnd, Source.Begin, Source._LogicalEnd - 1);
	}

	void Delete(unsigned short StartDeleteIndex, unsigned short StopDeleteIndex)
	{
		_Delete(Begin + StartDeleteIndex, Begin + StopDeleteIndex);

	}

	void Preppend(const char Source[])
	{
		const char* SourceLogicalEnd = _GetLogicalEnd(Source);

		_Insert(Begin, Source, SourceLogicalEnd, Source, SourceLogicalEnd - 1);
	}

	void Preppend(const StringKernel& Source)
	{
		_Insert(Begin, Source.Begin, Source._LogicalEnd, Source.Begin, Source._LogicalEnd - 1);
	}

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]
		, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char * Position = _SearchNecklaceFromLeft(Begin + CofferBeginIndex, Begin + CofferEndIndex, Necklace, _GetLogicalEnd(Necklace)
			,Necklace + NecklaceBeginIndex, Necklace + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);

	}

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]) const
	{

		const char* NecklaceLogicalEnd = _GetLogicalEnd(Necklace);

		char* Position = _SearchNecklaceFromLeft(Begin + CofferBeginIndex, Begin + CofferEndIndex, Necklace, NecklaceLogicalEnd,
			Necklace, NecklaceLogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);

	}

	unsigned short SearchNecklaceFromLeft(const char Necklace[], unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char* Position = _SearchNecklaceFromLeft(Begin, _LogicalEnd - 1, Necklace, _GetLogicalEnd(Necklace)
			, Necklace + NecklaceBeginIndex, Necklace + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);

	}

	unsigned short SearchNecklaceFromLeft(const char Necklace[]) const
	{

		char* Position = _SearchNecklaceFromLeft(Begin, _LogicalEnd - 1, Necklace, _GetLogicalEnd(Necklace) - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Necklace
	, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{
		
		char* Position = _SearchNecklaceFromLeft(Begin + CofferBeginIndex, Begin + CofferEndIndex, Necklace.Begin
			, Necklace._LogicalEnd, Necklace.Begin + NecklaceBeginIndex, Necklace.Begin + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Necklace) const
	{

		char* Position = _SearchNecklaceFromLeft(Begin + CofferBeginIndex, Begin + CofferEndIndex, Necklace.Begin, 
			Necklace._LogicalEnd, Necklace.Begin, Necklace._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);

	}

	unsigned short SearchNecklaceFromLeft(StringKernel& Necklace, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char* Position = _SearchNecklaceFromLeft(Begin, _LogicalEnd - 1, Necklace.Begin, Necklace._LogicalEnd
			, Necklace.Begin + NecklaceBeginIndex, Necklace.Begin + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);

	}

	unsigned short SearchNecklaceFromLeft(StringKernel& Necklace) const
	{
		
		char* Position = _SearchNecklaceFromLeft(Begin, _LogicalEnd - 1, Necklace.Begin, Necklace._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchNecklaceFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]
		, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{
		
		char* Position = _SearchNecklaceFromRight(Begin + CofferBeginIndex, Begin + CofferEndIndex, Necklace
			, _GetLogicalEnd(Necklace) , Necklace + NecklaceBeginIndex, Necklace + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchNecklaceFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]) const
	{

		const char* NecklaceLogicalEnd = _GetLogicalEnd(Necklace);

		char* Position = _SearchNecklaceFromRight(Begin + CofferBeginIndex, Begin + CofferEndIndex, Necklace, NecklaceLogicalEnd,
			Necklace, NecklaceLogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchNecklaceFromRight(const char Necklace[], unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char* Position = _SearchNecklaceFromRight(Begin, _LogicalEnd - 1, Necklace, _GetLogicalEnd(Necklace)
			, Necklace + NecklaceBeginIndex, Necklace + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchNecklaceFromRight(const char Necklace[]) const
	{

		char* Position = _SearchNecklaceFromRight(Begin, _LogicalEnd - 1, Necklace, _GetLogicalEnd(Necklace) - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchNecklaceFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Necklace
		, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{
		
		char* Position = _SearchNecklaceFromRight(Begin + CofferBeginIndex, Begin + CofferEndIndex, Necklace.Begin
			, Necklace._LogicalEnd, Necklace.Begin + NecklaceBeginIndex, Necklace.Begin + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchNecklaceFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Necklace) const
	{

		char* Position = _SearchNecklaceFromRight(Begin + CofferBeginIndex, Begin + CofferEndIndex, Necklace.Begin,
			Necklace._LogicalEnd, Necklace.Begin, Necklace._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchNecklaceFromRight(StringKernel& Necklace, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char* Position = _SearchNecklaceFromRight(Begin, _LogicalEnd - 1, Necklace.Begin, Necklace._LogicalEnd
			, Necklace.Begin + NecklaceBeginIndex, Necklace.Begin + NecklaceEndIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchNecklaceFromRight(StringKernel& Necklace) const
	{

		char* Position = _SearchNecklaceFromRight(Begin, _LogicalEnd - 1, Necklace.Begin, Necklace._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGemFromLeft(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems, _GetLogicalEnd(Gems),
			Gems + FirstGemIndex, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]) const
	{
		const char* GemsLogicalEnd = _GetLogicalEnd(Gems);

		char* Position = _SearchGemFromLeft(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems, GemsLogicalEnd, 
			Gems, GemsLogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromLeft(const char Gems[], unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		
		char* Position = _SearchGemFromLeft(Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems), Gems + FirstGemIndex
			, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromLeft(const char Gems[]) const
	{

		char* Position = _SearchGemFromLeft(Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems) - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		
		char* Position = _SearchGemFromLeft(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems.Begin, Gems._LogicalEnd
			, Gems.Begin + FirstGemIndex, Gems.Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems) const
	{

		char* Position = _SearchGemFromLeft(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems.Begin, Gems._LogicalEnd
			, Gems.Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromLeft(StringKernel& Gems, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGemFromLeft(Begin, _LogicalEnd - 1, Gems.Begin, Gems._LogicalEnd, Gems.Begin + FirstGemIndex,
			Gems.Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromLeft(StringKernel& Gems) const
	{

		char* Position = _SearchGemFromLeft(Begin, _LogicalEnd - 1, Gems.Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		
		char* Position = _SearchPebbleFromLeft(Begin + CofferBeginIndex, Begin + CofferEndIndex,Gems, _GetLogicalEnd(Gems),
			Gems + FirstGemIndex, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]) const
	{
		const char* GemsLogicalEnd = _GetLogicalEnd(Gems);

		char* Position = _SearchPebbleFromLeft(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems, GemsLogicalEnd,
			Gems, GemsLogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromLeft(const char Gems[], unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebbleFromLeft(Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems), Gems + FirstGemIndex,
			Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromLeft(const char Gems[]) const
	{
		char* Position = _SearchPebbleFromLeft(Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems) - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebbleFromLeft(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems.Begin, Gems._LogicalEnd
			, Gems.Begin + FirstGemIndex, Gems.Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems) const
	{

		char* Position = _SearchPebbleFromLeft(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems.Begin, Gems._LogicalEnd
			, Gems.Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromLeft(StringKernel& Gems, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebbleFromLeft(Begin, _LogicalEnd, Gems.Begin, Gems._LogicalEnd, Gems.Begin + FirstGemIndex,
			Gems.Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromLeft(StringKernel& Gems) const
	{

		char* Position = _SearchPebbleFromLeft(Begin, _LogicalEnd - 1, Gems.Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGemFromRight(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems, _GetLogicalEnd(Gems),
			Gems + FirstGemIndex, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]) const
	{
		const char* GemsLogicalEnd = _GetLogicalEnd(Gems);

		char* Position = _SearchGemFromRight(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems, GemsLogicalEnd,
			Gems, GemsLogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromRight(const char Gems[], unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGemFromRight(Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems), Gems + FirstGemIndex,
			Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromRight(const char Gems[]) const
	{
		char* Position = _SearchGemFromRight(Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems) - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGemFromRight(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems.Begin, Gems._LogicalEnd,
			Gems.Begin + FirstGemIndex, Gems.Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems) const
	{

		char* Position = _SearchGemFromRight(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems.Begin, Gems._LogicalEnd,
			Gems.Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromRight(StringKernel& Gems, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGemFromRight(Begin, _LogicalEnd, Gems.Begin, Gems._LogicalEnd, Gems.Begin + FirstGemIndex,
			Gems.Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchGemFromRight(StringKernel& Gems) const
	{

		char* Position = _SearchGemFromRight(Begin, _LogicalEnd - 1, Gems.Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebbleFromRight(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems, _GetLogicalEnd(Gems),
			Gems + FirstGemIndex, Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]) const
	{

		const char* GemsLogicalEnd = _GetLogicalEnd(Gems);

		char* Position = _SearchPebbleFromRight(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems, GemsLogicalEnd,
			Gems, GemsLogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromRight(const char Gems[], unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebbleFromRight(Begin, _LogicalEnd, Gems, _GetLogicalEnd(Gems), Gems + FirstGemIndex, 
			Gems + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromRight(const char Gems[]) const
	{
		char* Position = _SearchPebbleFromRight(Begin, _LogicalEnd - 1, Gems, _GetLogicalEnd(Gems) - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
	
		char* Position = _SearchPebbleFromRight(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems.Begin, Gems._LogicalEnd,
			Gems.Begin + FirstGemIndex, Gems.Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems) const
	{

		char* Position = _SearchPebbleFromRight(Begin + CofferBeginIndex, Begin + CofferEndIndex, Gems.Begin, Gems._LogicalEnd,
			Gems.Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromRight(StringKernel& Gems, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebbleFromRight(Begin, _LogicalEnd, Gems.Begin, Gems._LogicalEnd, Gems.Begin + FirstGemIndex, 
			Gems.Begin + LastGemIndex);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
	}

	unsigned short SearchPebbleFromRight(StringKernel& Gems) const
	{

		char* Position = _SearchGemFromRight(Begin, _LogicalEnd - 1, Gems.Begin, Gems._LogicalEnd - 1);

		return (Position == nullptr) ? NoPosition : (Position - Begin);
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
		char* BeginCopy = Begin;

		while (BeginCopy < _LogicalEnd)
		{
			cout << *BeginCopy++;
		}
	}








};