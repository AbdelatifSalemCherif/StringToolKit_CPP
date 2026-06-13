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

		_clsRange()
		{
			Begin = nullptr;
			End = nullptr;
		}

		_clsRange(char* Begin_, char* End_)
		{
			Begin = Begin_;
			End = End_;
		}

		_clsRange(const char* Begin_)
		{
			Begin = (char*)Begin_;
			End = GetLogicalEnd(Begin_);
		}

		static _clsRange NewRange(char* Begin_, char* End_)
		{
			_clsRange Range(Begin_, End_);

			return Range;
		}

		static _clsRange NewRange(const char* Begin_)
		{
			return NewRange((char*) Begin_, GetLogicalEnd(Begin_));
		}

		_clsRange RawRange() const
		{
			return NewRange(Begin, End - 1);
		}

		static char* GetLogicalEnd(const char* Begin_)
		{
			while (*Begin_)
			{
				Begin_++;
			}

			return (char*) Begin_;
		}

		bool IsAddressInside(const char* Address) const
		{
			return Address >= Begin && Address <= End;
		}

		bool IsIndexInside(unsigned short Index) const
		{
			return IsAddressInside(Begin + Index);
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

		bool IsEmpty() const
		{
			return End == Begin;
		}

		bool IsSameData(const _clsRange& Range) const
		{
			
			bool IsSame = Length() == Range.Length();

			for (const char* Reader1 = Begin, *Reader2 = Range.Begin; IsSame && (Begin < Range.End); Reader1++,Reader2++)
			{
				IsSame = *Reader1 == *Reader2;
			}

			return IsSame;
		}

		static void Swap2Address(const char** Address1, const char** Address2)
		{
			const char* TempAddress = *Address1;
			*Address1 = *Address2;
			*Address2 = TempAddress;
		}

		void Swap(_clsRange& Range)
		{
			Swap2Address((const char**)& Begin, (const char**)& Range.Begin);

			Swap2Address((const char**)& End, (const char**)& Range.End);
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

		void PushStopCharacter()
		{
			PushBack('\n');
		}

		void AssigneStopCharacter()
		{
			*End = '\n';
		}

		void Clear()
		{
			End = Begin;

			AssigneStopCharacter();
		}

		char at(unsigned short Index) const
		{

			return IsAddressInside(Begin + Index) ? *(Begin + Index) : '\0';

		}

		void PushBack(char NewCharacter)
		{

			*++End = NewCharacter;

		}

		void PopBack()
		{
			if (End > Begin)
			{
				End--;
			}

		}

		_clsRange SubRange(unsigned short From, unsigned short To) const
		{

			return HandleRangeToBeInside(NewRange(Begin + From, Begin + To));

		}

		unsigned short Size() const
		{
			return End - Begin;
		}

		unsigned short Length() const
		{
			return End - Begin + 1;
		}

		void Assignment(const _clsRange& New)
		{
			if (!IsSameData(New))
			{
				End = Write(Begin, New, Left);
			}

		}

		void Concatenate(const _clsRange& New1, const _clsRange& New2)
		{

			End = Write(Write(Begin, New1, Left) + 1, New2, Left);

		}

		void Append(const _clsRange& New)
		{

			End = Write(End + 1, New, Left);

		}

		void Prepend(const _clsRange& New)
		{

			End = Write(Begin + New.Length(), *this, Right);

			Write(Begin, New, Left);

		}

		void Insert(char* StartWrite, const _clsRange& New)
		{

			char* StopWrite = StartWrite + New.Length() - 1;

			End = Write(StopWrite + 1, NewRange(StartWrite, End), Right);

			Write(StartWrite, New, Right);

		}

		void Insert(unsigned short StartWrite, const _clsRange& New)
		{
			Insert(Begin + StartWrite, New);
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

		void Replace(_clsRange DestinationRange, const _clsRange& SourceRange)
		{
			unsigned short WriteLength = DestinationRange.Length(), ReadLength = SourceRange.Length();

			if (WriteLength == ReadLength)
			{
				DestinationRange.Assignment(SourceRange);
			}
			else if (WriteLength > ReadLength)
			{
				DestinationRange.Assignment(SourceRange);

				Delete(NewRange(DestinationRange.End + 1, DestinationRange.End + WriteLength - ReadLength));
			}
			else
			{
				char* StopWrite = DestinationRange.End + ReadLength - WriteLength;

				End = Write(StopWrite + 1, NewRange(DestinationRange.End + 1, End), Right);

				NewRange(DestinationRange.Begin, StopWrite).Assignment(SourceRange);
			}

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
		return NewLength <= Capacity();
	}

	char* _GetNewPhysicalEnd(char* Start, unsigned short Capacity) const
	{
		return Start + Capacity - 1;
	}

	void _Assignment(const _clsRange& SourceRange)
	{

		_Reallocate(SourceRange.Length(), false);

		_LogicalRange.Assignment(SourceRange);

	}

	void _Concatenate(_clsRange Source1Range, const _clsRange& Source2Range)
	{

		Source1Range.PopBack();

		_Reallocate(Source1Range.Length() + Source2Range.Length(), false);

		_LogicalRange.Concatenate(Source1Range, Source2Range);
	}

	void _Append(const _clsRange& SourceRange)
	{

		_Reallocate(Length() - 1 + SourceRange.Length(), true);

		_LogicalRange = _LogicalRange.RawRange();

		_LogicalRange.Append(SourceRange);
	}

	void _Prepend(_clsRange SourceRange)
	{

		_Reallocate(SourceRange.Length() - 1 + Length(), true);

		_LogicalRange.Prepend(SourceRange.RawRange());

	}

	void _Insert(unsigned short StartWrittingIndex, _clsRange SourceRange)
	{

		if (_LogicalRange.IsIndexInside(StartWrittingIndex))
		{

			SourceRange.PopBack();

			_Reallocate(Length() + SourceRange.Length(), true);

			_LogicalRange.Insert(StartWrittingIndex, SourceRange);

		}

	}

	void _Insert(unsigned short StartWrittingIndex, _clsRange SourceRange, unsigned short StartReadingIndex,
		unsigned short StopReadingIndex)
	{

		if (_LogicalRange.IsIndexInside(StartWrittingIndex))
		{

			SourceRange.PopBack();

			SourceRange = SourceRange.SubRange(StartReadingIndex, StopReadingIndex);

			_Reallocate(Length() + SourceRange.Length(), true);

			_LogicalRange.Insert(StartWrittingIndex, SourceRange);

		}

	}

	void _Delete(unsigned short StartDeleteIndex, unsigned short StopDeleteIndex)
	{
		
		_LogicalRange.Delete(_LogicalRange.RawRange().SubRange(StartDeleteIndex, StopDeleteIndex));
	}

	char* _SearchNecklace(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, _clsRange Necklace
		, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex, _clsRange::enStartFrom StartSearchFrom) const
	{

		return _LogicalRange.RawRange().SubRange(CofferBeginIndex, CofferEndIndex).
			SearchNecklace(Necklace.RawRange().SubRange(NecklaceBeginIndex, NecklaceEndIndex), StartSearchFrom);

	}

	char* _SearchNecklace(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, _clsRange Necklace
		, _clsRange::enStartFrom StartSearchFrom) const
	{

		return _LogicalRange.RawRange().SubRange(CofferBeginIndex, CofferEndIndex).
			SearchNecklace(Necklace.RawRange(), StartSearchFrom);

	}

	char* _SearchNecklace(_clsRange Necklace, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex,
		_clsRange::enStartFrom StartSearchFrom) const
	{

		return _LogicalRange.RawRange().
			SearchNecklace(Necklace.RawRange().SubRange(NecklaceBeginIndex, NecklaceEndIndex), StartSearchFrom);

	}

	char* _SearchNecklace(_clsRange Necklace, _clsRange::enStartFrom StartSearchFrom) const
	{

		return _LogicalRange.RawRange().SearchNecklace(Necklace.RawRange(), StartSearchFrom);

	}

	char* _SearchGem(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, _clsRange Gems
		, unsigned short GemsBeginIndex, unsigned short GemsEndIndex, _clsRange::enStartFrom StartSearchFrom) const
	{

		return _LogicalRange.RawRange().SubRange(CofferBeginIndex, CofferEndIndex).
			SearchGem(Gems.RawRange().SubRange(GemsBeginIndex, GemsEndIndex), StartSearchFrom);

	}

	char* _SearchGem(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, _clsRange Gems
		, _clsRange::enStartFrom StartSearchFrom) const
	{

		return _LogicalRange.RawRange().SubRange(CofferBeginIndex, CofferEndIndex).
			SearchGem(Gems.RawRange(), StartSearchFrom);

	}

	char* _SearchGem( _clsRange Gems, unsigned short GemsBeginIndex, unsigned short GemsEndIndex, 
		_clsRange::enStartFrom StartSearchFrom) const
	{

		return _LogicalRange.RawRange().
			SearchGem(Gems.RawRange().SubRange(GemsBeginIndex, GemsEndIndex), StartSearchFrom);

	}

	char* _SearchGem( _clsRange Gems, _clsRange::enStartFrom StartSearchFrom) const
	{

		return _LogicalRange.RawRange().SearchGem(Gems.RawRange(), StartSearchFrom);

	}

	char* _SearchPebble(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, _clsRange Gems
		, unsigned short GemsBeginIndex, unsigned short GemsEndIndex, _clsRange::enStartFrom StartSearchFrom) const
	{

		return _LogicalRange.RawRange().SubRange(CofferBeginIndex, CofferEndIndex).
			SearchPebble(Gems.RawRange().SubRange(GemsBeginIndex, GemsEndIndex), StartSearchFrom);

	}

	char* _SearchPebble(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, _clsRange Gems
		, _clsRange::enStartFrom StartSearchFrom) const
	{

		return _LogicalRange.RawRange().SubRange(CofferBeginIndex, CofferEndIndex).
			SearchPebble(Gems.RawRange(), StartSearchFrom);

	}

	char* _SearchPebble(_clsRange Gems, unsigned short GemsBeginIndex, unsigned short GemsEndIndex, 
		_clsRange::enStartFrom StartSearchFrom) const
	{

		return _LogicalRange.RawRange().
			SearchPebble(Gems.RawRange().SubRange(GemsBeginIndex, GemsEndIndex), StartSearchFrom);

	}

	char* _SearchPebble(_clsRange Gems, _clsRange::enStartFrom StartSearchFrom) const
	{

		return _LogicalRange.RawRange().SearchPebble(Gems.RawRange(), StartSearchFrom);

	}

	void _Replace(unsigned short StartWriteIndex, unsigned short StopWriteIndex, const _clsRange& SourceRange)
	{
		_Reallocate(_LogicalRange.Length() + SourceRange.Length() - (StopWriteIndex - StartWriteIndex + 1), true);

		_LogicalRange.Replace(_LogicalRange.RawRange().SubRange(StartWriteIndex, StopWriteIndex), SourceRange);
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

	const char* _GetLogicalEnd(const char Data[], const unsigned short& Length) const
	{
		return Data + Length - 1;
	}

	void _Reallocate(unsigned short NewLogicalLength, bool DoSaveData)
	{

		if (!_HaveMoreSpace(NewLogicalLength))
		{
			_clsRange LastLogicalRange = _LogicalRange;

			unsigned short Capacity = _GetNewCapacity(NewLogicalLength);

			_LogicalRange.Begin = new char[Capacity];

			_PhysicalEnd = _GetNewPhysicalEnd(_LogicalRange.Begin, Capacity);

			if (DoSaveData)
			{

				_LogicalRange.Assignment(LastLogicalRange);

			}
			else
			{
				_LogicalRange.Clear();
			}

			delete[] LastLogicalRange.Begin;
		}
		else if (!DoSaveData)
		{
			_LogicalRange.Clear();
		}
	}

	void _Swap2Indices(unsigned short& Index1, unsigned short& Index2) const
	{
		unsigned short TempIndex = Index1;
		Index1 = Index2;
		Index2 = TempIndex;
	}


public:

	static const unsigned short NoPosition = -1;

	StringKernel()
	{

		_LogicalRange.Begin = new char[16];

		_LogicalRange.Clear();

		_PhysicalEnd = _GetNewPhysicalEnd(_LogicalRange.Begin, 16);

	}

	StringKernel(const char Value[])
	{

		_clsRange ValueRange(Value);

		const unsigned short Capacity = _GetNewCapacity(ValueRange.Length());

		_LogicalRange.Begin = new char[Capacity];

		_LogicalRange.Assignment(ValueRange);

		_PhysicalEnd = _GetNewPhysicalEnd(_LogicalRange.Begin, Capacity);

	}

	StringKernel(const StringKernel& Value)
	{
		const unsigned short Capacity = _GetNewCapacity(Value._LogicalRange.Length());

		_LogicalRange.Begin = new char[Capacity];

		_PhysicalEnd = _GetNewPhysicalEnd(_LogicalRange.Begin, Capacity);

		_LogicalRange.Assignment(Value._LogicalRange);

	}

	~StringKernel()
	{
		delete[] _LogicalRange.Begin;

		cout << "Hi Destructor !" << endl;
	}

	const char* Value() const
	{
		return _LogicalRange.Begin;
	}

	void Clear()
	{
		_LogicalRange.Clear();

	}

	bool IsEmpty() const
	{
		return _LogicalRange.IsEmpty();
	}

	bool IsFull() const
	{
		return _LogicalRange.End == _PhysicalEnd;
	}

	unsigned short Size() const
	{
		return _LogicalRange.Size();
	}

	unsigned short Length() const
	{
		return _LogicalRange.Length();
	}

	unsigned short Capacity() const
	{
		return _PhysicalEnd - _LogicalRange.Begin + 1;
	}

	bool IsSameData(const StringKernel& Data) const
	{
		return _LogicalRange.IsSameData(Data._LogicalRange);
	}

	void Swap(StringKernel& Data2)
	{
		_LogicalRange.Swap(Data2._LogicalRange);

		_clsRange::Swap2Address((const char**)& _PhysicalEnd, (const char**)& Data2._PhysicalEnd);

	}

	char at(unsigned short Index) const
	{
		return _LogicalRange.at(Index);
	}

	void PushBack(char NewCharacter)
	{
		_Reallocate(_LogicalRange.Length() + 1, true);

		*_LogicalRange.End = NewCharacter;

		_LogicalRange.PushStopCharacter();
	}

	void PopBack()
	{
		_LogicalRange.PopBack();

		_LogicalRange.AssigneStopCharacter();
	}

	void Assignment(const char Source[])
	{

		_Assignment(_clsRange::NewRange(Source));

	}

	void Assignment(const StringKernel& Source)
	{
		_Assignment(Source._LogicalRange);
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
		_Concatenate(_clsRange::NewRange(Source1), _clsRange::NewRange(Source2));
	}

	void Concatenate(const char Source1[], const StringKernel& Source2)
	{
		_Concatenate(_clsRange::NewRange(Source1), Source2._LogicalRange);
	}

	void Concatenate(const StringKernel& Source1, const char Source2[])
	{
		_Concatenate(Source1._LogicalRange, _clsRange::NewRange(Source2));
	}

	void Concatenate(const StringKernel& Source1, const StringKernel& Source2)
	{
		_Concatenate(Source1._LogicalRange, Source2._LogicalRange);
	}

	void Append(const char Source[])
	{
		_Append(_clsRange::NewRange(Source));
	}

	void Append(const StringKernel& Source)
	{
		_Append(Source._LogicalRange);
	}

	void Prepend(const char Source[])
	{
		_Prepend(_clsRange::NewRange(Source));

	}

	void Prepend(const StringKernel& Source)
	{
		_Prepend(Source._LogicalRange);
	}

	void Insert(unsigned short StartWrittingIndex, const char Source[], unsigned short StartReadingIndex, 
		unsigned short StopReadingIndex)
	{

		_Insert(StartWrittingIndex, _clsRange::NewRange(Source), StartReadingIndex, StopReadingIndex);

	}

	void Insert(unsigned short StartWrittingIndex, const char Source[])
	{		
		
		_Insert(StartWrittingIndex, _clsRange::NewRange(Source));

	}

	void Insert(unsigned short StartWrittingIndex, const StringKernel& Source, unsigned short StartReadingIndex,
		unsigned short StopReadingIndex)
	{
		_Insert(StartWrittingIndex, Source._LogicalRange, StartReadingIndex, StopReadingIndex);
			
	}

	void Insert(unsigned short StartWrittingIndex, const StringKernel& Source)
	{
		_Insert(StartWrittingIndex, Source._LogicalRange);
	}

	void Delete(unsigned short StartDeleteIndex, unsigned short StopDeleteIndex)
	{
		_Delete(StartDeleteIndex, StopDeleteIndex);

	}

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]
		, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char * Position = _SearchNecklace(CofferBeginIndex, CofferEndIndex, _clsRange::NewRange(Necklace), NecklaceBeginIndex, 
			NecklaceEndIndex, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);

	}

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]) const
	{

		char* Position = _SearchNecklace(CofferBeginIndex, CofferEndIndex, _clsRange::NewRange(Necklace), _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);

	}

	unsigned short SearchNecklaceFromLeft(const char Necklace[], unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char* Position = _SearchNecklace(_clsRange::NewRange(Necklace), NecklaceBeginIndex,NecklaceEndIndex, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);

	}

	unsigned short SearchNecklaceFromLeft(const char Necklace[]) const
	{

		char* Position = _SearchNecklace(_clsRange::NewRange(Necklace), _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Necklace
	, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{
		
		char* Position = _SearchNecklace(CofferBeginIndex, CofferEndIndex, Necklace._LogicalRange, NecklaceBeginIndex,
			NecklaceEndIndex, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchNecklaceFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Necklace) const
	{

		char* Position = _SearchNecklace(CofferBeginIndex, CofferEndIndex, Necklace._LogicalRange, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);

	}

	unsigned short SearchNecklaceFromLeft(StringKernel& Necklace, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char* Position = _SearchNecklace(Necklace._LogicalRange, NecklaceBeginIndex, NecklaceEndIndex, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);

	}

	unsigned short SearchNecklaceFromLeft(StringKernel& Necklace) const
	{
		
		char* Position = _SearchNecklace(Necklace._LogicalRange, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchNecklaceFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]
		, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{
		
		char* Position = _SearchNecklace(CofferBeginIndex, CofferEndIndex, _clsRange::NewRange(Necklace), NecklaceBeginIndex,
			NecklaceEndIndex, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchNecklaceFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Necklace[]) const
	{

		char* Position = _SearchNecklace(CofferBeginIndex, CofferEndIndex, _clsRange::NewRange(Necklace), _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchNecklaceFromRight(const char Necklace[], unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char* Position = _SearchNecklace(_clsRange::NewRange(Necklace), NecklaceBeginIndex,
			NecklaceEndIndex, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchNecklaceFromRight(const char Necklace[]) const
	{

		char* Position = _SearchNecklace(_clsRange::NewRange(Necklace), _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchNecklaceFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Necklace
		, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{
		
		char* Position = _SearchNecklace(CofferBeginIndex, CofferEndIndex, Necklace._LogicalRange, NecklaceBeginIndex,
			NecklaceEndIndex, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchNecklaceFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Necklace) const
	{

		char* Position = _SearchNecklace(CofferBeginIndex, CofferEndIndex, Necklace._LogicalRange, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchNecklaceFromRight(StringKernel& Necklace, unsigned short NecklaceBeginIndex, unsigned short NecklaceEndIndex) const
	{

		char* Position = _SearchNecklace(Necklace._LogicalRange, NecklaceBeginIndex, NecklaceEndIndex, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchNecklaceFromRight(StringKernel& Necklace) const
	{

		char* Position = _SearchNecklace(Necklace._LogicalRange, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGem(CofferBeginIndex, CofferEndIndex, _clsRange::NewRange(Gems), FirstGemIndex,
			LastGemIndex, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]) const
	{
		char* Position = _SearchGem(CofferBeginIndex, CofferEndIndex, _clsRange::NewRange(Gems), _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromLeft(const char Gems[], unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		
		char* Position = _SearchGem(_clsRange::NewRange(Gems), FirstGemIndex, LastGemIndex, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromLeft(const char Gems[]) const
	{

		char* Position = _SearchGem(_clsRange::NewRange(Gems), _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		
		char* Position = _SearchGem(CofferBeginIndex, CofferEndIndex, Gems._LogicalRange, FirstGemIndex, LastGemIndex, 
			_clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems) const
	{

		char* Position = _SearchGem(CofferBeginIndex, CofferEndIndex, Gems._LogicalRange, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromLeft(StringKernel& Gems, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGem(Gems._LogicalRange, FirstGemIndex, LastGemIndex, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromLeft(StringKernel& Gems) const
	{

		char* Position = _SearchGem(Gems._LogicalRange, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
		
		char* Position = _SearchPebble(CofferBeginIndex, CofferEndIndex, _clsRange::NewRange(Gems), FirstGemIndex, LastGemIndex,
			_clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]) const
	{
		char* Position = _SearchPebble(CofferBeginIndex, CofferEndIndex, _clsRange::NewRange(Gems), _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromLeft(const char Gems[], unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebble(_clsRange::NewRange(Gems), FirstGemIndex, LastGemIndex, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromLeft(const char Gems[]) const
	{
		char* Position = _SearchPebble(_clsRange::NewRange(Gems), _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebble(CofferBeginIndex, CofferEndIndex, Gems._LogicalRange, FirstGemIndex, LastGemIndex,
			_clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromLeft(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems) const
	{

		char* Position = _SearchPebble(CofferBeginIndex, CofferEndIndex, Gems._LogicalRange, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromLeft(StringKernel& Gems, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebble(Gems._LogicalRange, FirstGemIndex, LastGemIndex, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromLeft(StringKernel& Gems) const
	{

		char* Position = _SearchPebble(Gems._LogicalRange, _clsRange::Left);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGem(CofferBeginIndex, CofferEndIndex, _clsRange::NewRange(Gems), FirstGemIndex, LastGemIndex,
			_clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]) const
	{
		char* Position = _SearchGem(CofferBeginIndex, CofferEndIndex, _clsRange::NewRange(Gems), _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromRight(const char Gems[], unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGem(_clsRange::NewRange(Gems), FirstGemIndex, LastGemIndex, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromRight(const char Gems[]) const
	{
		char* Position = _SearchGem(_clsRange::NewRange(Gems), _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGem(CofferBeginIndex, CofferEndIndex, Gems._LogicalRange, FirstGemIndex, LastGemIndex,
			_clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems) const
	{

		char* Position = _SearchGem(CofferBeginIndex, CofferEndIndex, Gems._LogicalRange, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromRight(StringKernel& Gems, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchGem(Gems._LogicalRange, FirstGemIndex, LastGemIndex, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchGemFromRight(StringKernel& Gems) const
	{

		char* Position = _SearchGem(Gems._LogicalRange, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebble(CofferBeginIndex, CofferEndIndex, _clsRange::NewRange(Gems), FirstGemIndex, LastGemIndex,
			_clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, const char Gems[]) const
	{

		char* Position = _SearchPebble(CofferBeginIndex, CofferEndIndex, _clsRange::NewRange(Gems), _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromRight(const char Gems[], unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebble(_clsRange::NewRange(Gems), FirstGemIndex, LastGemIndex, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromRight(const char Gems[]) const
	{
		char* Position = _SearchPebble(_clsRange::NewRange(Gems), _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems
		, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{
	
		char* Position = _SearchPebble(CofferBeginIndex, CofferEndIndex, Gems._LogicalRange, FirstGemIndex, LastGemIndex,
			_clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromRight(unsigned short CofferBeginIndex, unsigned short CofferEndIndex, StringKernel& Gems) const
	{

		char* Position = _SearchPebble(CofferBeginIndex, CofferEndIndex, Gems._LogicalRange, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromRight(StringKernel& Gems, unsigned short FirstGemIndex, unsigned short LastGemIndex) const
	{

		char* Position = _SearchPebble(Gems._LogicalRange, FirstGemIndex, LastGemIndex, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	unsigned short SearchPebbleFromRight(StringKernel& Gems) const
	{

		char* Position = _SearchPebble(Gems._LogicalRange, _clsRange::Right);

		return (Position == nullptr) ? NoPosition : (Position - _LogicalRange.Begin);
	}

	void Replace(unsigned short StartWritingIndex, unsigned short StopWritingIndex, const char Source[],
		unsigned short StartReadingIndex, unsigned short StopReadingIndex)
	{
		_Replace(StartWritingIndex, StopWritingIndex, 
			_clsRange::NewRange(Source).RawRange().SubRange(StartReadingIndex, StopReadingIndex));
	}

	void Replace(unsigned short StartWritingIndex, unsigned short StopWritingIndex, const char Source[])
	{
		_Replace(StartWritingIndex, StopWritingIndex, _clsRange::NewRange(Source).RawRange());
	}

	void Replace(unsigned short StartWritingIndex, unsigned short StopWritingIndex, const StringKernel& Source,
		unsigned short StartReadingIndex, unsigned short StopReadingIndex)
	{
		_Replace(StartWritingIndex, StopWritingIndex,
			Source._LogicalRange.RawRange().SubRange(StartReadingIndex, StopReadingIndex));
	}

	void Replace(unsigned short StartWritingIndex, unsigned short StopWritingIndex, const StringKernel& Source)
	{
		_Replace(StartWritingIndex, StopWritingIndex, Source._LogicalRange.RawRange());
	}

	void Print() const
	{
		char* Reader = _LogicalRange.Begin;

		while (Reader < _LogicalRange.End)
		{
			cout << *Reader++;
		}
	}
	

};