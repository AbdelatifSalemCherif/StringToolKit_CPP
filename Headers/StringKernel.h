#pragma once

#include<iostream>
#include <vector>

using namespace std;


class StringKernel
{
private:
	
	class _clsRange
	{
	private:

		char* _Begin;
		char* _End;

	public:

		_clsRange(char* Begin, char* End)
		{
			_Begin = Begin;
			_End = End;
		}

		_clsRange(const char* Begin)
		{
			_Begin = (char*)Begin;
			_End = GetLogicalEnd(Begin);

		}

		void SetBegin(char* Begin)
		{
			_Begin = Begin;
		}

		char* GetBegin() const
		{
			return _Begin;
		}

		__declspec(property(get = GetBegin, put = SetBegin)) char* Begin;

		void SetEnd(char* End)
		{
			_End = End;
		}

		char* GetEnd() const
		{
			return _End;
		}

		__declspec(property(get = GetEnd, put = SetEnd)) char* End;

		static _clsRange NewRange(char* Begin, char* End)
		{

			return _clsRange(Begin, End);
		}

		static _clsRange NewRange(const char* Begin)
		{
			return _clsRange(Begin);
		}

		_clsRange RawRange() const
		{
			return (*_End == '\0') ? NewRange(_Begin, _End - 1) : *this;
		}

		static char* GetLogicalEnd(const char* Begin)
		{
			while (*Begin)
			{
				Begin++;
			}

			return (char*) Begin;
		}

		bool IsAddressInside(const char* Address) const
		{
			return Address >= _Begin && Address <= _End;
		}

		bool IsIndexInside(unsigned short Index) const
		{
			return IsAddressInside(_Begin + Index);
		}

		_clsRange HandleRangeToBeInside(_clsRange Range) const
		{
			if (Range._Begin > Range._End)
			{
				Swap2Address((const char**)& Range._Begin, (const char**)& Range._End);
			}

			if (Range._Begin < _Begin)
			{
				Range._Begin = _Begin;
			}
			else if (Range._Begin > _End)
			{
				Range._Begin = _End;
			}

			if (Range._End > _End || Range._End < _Begin)
			{
				Range._End = _End;
			}

			return Range;
		}

		bool IsEmpty() const
		{
			return _End == _Begin && *_End == '\0';
		}

		bool IsSameData(const _clsRange& Range) const
		{
			
			bool IsSame = Length() == Range.Length();

			for (const char* Reader1 = _Begin, *Reader2 = Range._Begin; IsSame && (_Begin < Range._End); Reader1++,Reader2++)
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
			Swap2Address((const char**)& _Begin, (const char**)& Range._Begin);

			Swap2Address((const char**)& _End, (const char**)& Range._End);
		}

		enum enStartFrom { Left, Right };

		char* Write(char* StartWrite, const _clsRange& ReadingRange, enStartFrom StartWriteFrom) const
		{
			char* StropWrite = StartWrite + ReadingRange.Length() - 1;

			if (StartWriteFrom == Left)
			{
				char* Writer = StartWrite, * Reader = ReadingRange._Begin;

				while (Reader <= ReadingRange._End)
				{
					*Writer++ = *Reader++;
				}
			}
			else
			{
				char* Writer = StropWrite, * Reader = ReadingRange._End;

				while (Reader >= ReadingRange._Begin)
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
			*_End = '\n';
		}

		void Clear()
		{
			_End = _Begin;

			AssigneStopCharacter();
		}

		char at(unsigned short Index) const
		{

			return IsAddressInside(_Begin + Index) ? *(_Begin + Index) : '\0';

		}

		void PushBack(char NewCharacter)
		{

			*++_End = NewCharacter;

		}

		void PopBack()
		{
			if (_End > _Begin)
			{
				_End--;
			}

		}

		_clsRange SubRange(unsigned short From, unsigned short To)
		{

			return HandleRangeToBeInside(NewRange(_Begin + From, _Begin + To));

		}

		unsigned short Size() const
		{
			return _End - _Begin;
		}

		unsigned short Length() const
		{
			return _End - _Begin + 1;
		}

		void Assignment(const _clsRange& New)
		{
			if (!IsSameData(New))
			{
				_End = Write(_Begin, New, Left);
			}

		}

		void Concatenate(const _clsRange& New1, const _clsRange& New2)
		{

			_End = Write(Write(_Begin, New1, Left) + 1, New2, Left);

		}

		void Append(const _clsRange& New)
		{

			_End = Write(_End + 1, New, Left);

		}

		void Prepend(const _clsRange& New)
		{

			_End = Write(_Begin + New.Length(), *this, Right);

			Write(_Begin, New, Left);

		}

		void Insert(char* StartWrite, const _clsRange& New)
		{

			char* StopWrite = StartWrite + New.Length() - 1;

			_End = Write(StopWrite + 1, NewRange(StartWrite, _End), Right);

			Write(StartWrite, New, Right);

		}

		void Insert(unsigned short StartWrite, const _clsRange& New)
		{
			Insert(_Begin + StartWrite, New);
		}

		void Delete(const _clsRange& DeleteRange)
		{

			_End = Write(DeleteRange._Begin, NewRange(DeleteRange._End + 1, _End), Left);

		}

		char* SearchNecklace(const _clsRange& Necklace, enStartFrom StratSearchFrom) const
		{

			if (StratSearchFrom == Left)
			{
				char* CofferReader = _Begin, * NecklaceReader = Necklace._Begin;

				while (CofferReader <= _End)
				{
					if (*CofferReader == *NecklaceReader)
					{

						if (NecklaceReader == Necklace._End)
						{
							return CofferReader - (Necklace._End - Necklace._Begin);
						}
						else
						{
							NecklaceReader++;
						}
					}
					else if (NecklaceReader > Necklace._Begin)
					{
						CofferReader -= NecklaceReader - Necklace._Begin;

						NecklaceReader = Necklace._Begin;

					}

					CofferReader++;

				}
			}
			else
			{
				char* CofferReader = _End, * NecklaceReader = Necklace._End;

				while (CofferReader >= _Begin)
				{
					if (*CofferReader == *NecklaceReader)
					{

						if (NecklaceReader == Necklace._Begin)
						{
							return CofferReader;
						}
						else
						{
							NecklaceReader--;
						}
					}
					else if (NecklaceReader < Necklace._End)
					{
						CofferReader += Necklace._End - NecklaceReader;

						NecklaceReader = Necklace._End;

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
				for (char* CofferReader = _Begin; CofferReader <= _End; CofferReader++)
				{
					for (char* GemsReader = Gems._Begin; GemsReader <= Gems._End; GemsReader++)
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
				for (char* CofferReader = _End; CofferReader >= _Begin; CofferReader--)
				{
					for (char* GemsReader = Gems._End; GemsReader >= Gems._Begin; GemsReader--)
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
				for (char* CofferReader = _Begin; CofferReader <= _End; CofferReader++)
				{
					if (Gems.SearchGem(NewRange(CofferReader, CofferReader), Left) == nullptr)
					{
						return CofferReader;
					}

				}
			}
			else
			{
				for (char* CofferReader = _End; CofferReader >= _Begin; CofferReader--)
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

				Delete(NewRange(DestinationRange._End + 1, DestinationRange._End + WriteLength - ReadLength));
			}
			else
			{
				char* StopWrite = DestinationRange._End + ReadLength - WriteLength;

				_End = Write(StopWrite + 1, NewRange(DestinationRange._End + 1, _End), Right);

				NewRange(DestinationRange._Begin, StopWrite).Assignment(SourceRange);
			}

		}

		static bool IsUpper(char Character)
		{
			return Character >= 65 && Character <= 90;
		}

		static bool IsLower(char Character)
		{
			return Character >= 97 && Character <= 122;
		}

		static bool IsVowel(char Character)
		{
			return Character == 65 || Character == 69 || Character == 73 || Character == 79 || Character == 85
				|| Character == 97 || Character == 101 || Character == 105 || Character == 111 || Character == 117;
		}

		static bool IsDigit(char Character)
		{
			return Character >= 48 && Character <= 57;
		}

		static char ToUpper(char Letter)
		{

			return IsLower(Letter) ? Letter & ~0x20 : Letter ;
		}

		static char ToLower(char Letter)
		{
			return IsUpper(Letter) ? Letter | 0x20 : Letter;
		}

		unsigned short CountAllUpperCase() const
		{
			unsigned short UppersCounter = 0;

			for (char* Reader = _Begin; Reader <= _End; Reader++)
			{
				if (IsUpper(*Reader))
				{
					UppersCounter++;
				}
			}

			return UppersCounter;
		}

		unsigned short CountAllLowerCase() const
		{
			unsigned short LowersCounter = 0;

			for (char* Reader = _Begin; Reader <= _End; Reader++)
			{
				if (IsLower(*Reader))
				{
					LowersCounter++;
				}
			}

			return LowersCounter;
		}

		unsigned short CountAllVowels() const
		{
			unsigned short VowelsCounter = 0;

			for (char* Reader = _Begin; Reader <= _End; Reader++)
			{
				if (IsVowel(*Reader))
				{
					VowelsCounter++;
				}
			}

			return VowelsCounter;
		}

		unsigned short CountAllLetters(char Letter, bool MatchCase)
		{
			unsigned short LettersCounter = 0;

			if (MatchCase)
			{
				for (char* Reader = _Begin; Reader <= _End; Reader++)
				{
					if (*Reader == Letter)
					{
						LettersCounter++;
					}

				}
			}
			else
			{
				Letter = ToUpper(Letter);

				for (char* Reader = _Begin; Reader <= _End; Reader++)
				{
					if (ToUpper(*Reader) == Letter)
					{
						LettersCounter++;
					}
				}
			}

			return LettersCounter;
		}

		unsigned short CountAllWords(const _clsRange& Separator) const
		{
			if (IsEmpty())
			{
				return 0;
			}

			unsigned short SeparatorLength = Separator.Length();

			if (Length() < SeparatorLength)
			{
				return 1;
			}
			else if (Length() == SeparatorLength)
			{
				return IsSameData(Separator) ? 0 : 1;
			}

			unsigned short WordsCounter = 0;

			char* SeparatorLastPosition = _End + 2, *SeparatorNextPosition = _End + 1;

			do
			{				
				if (SeparatorLastPosition - SeparatorNextPosition > SeparatorLength)
				{
					WordsCounter++;
				}

				SeparatorLastPosition = SeparatorNextPosition;

				SeparatorNextPosition = NewRange(_Begin, SeparatorLastPosition - 1).SearchNecklace(Separator, Right);
						

			} while (SeparatorNextPosition != nullptr);


			return (SeparatorLastPosition > _Begin) ? WordsCounter + 1: WordsCounter;
		}

		


	};

	_clsRange _LogicalRange = _clsRange(nullptr, nullptr);

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

	void _Allocate(unsigned short LogicalLength)
	{
		unsigned short Capacity = _GetNewCapacity(LogicalLength);

		_LogicalRange.Begin = new char[Capacity];

		_PhysicalEnd = _GetNewPhysicalEnd(_LogicalRange.Begin, Capacity);

		Clear();

	}

	StringKernel(unsigned short LogicalLength)
	{
		_Allocate(LogicalLength);
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

	void _Deallocate()
	{
		delete[] _LogicalRange.Begin;
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

	StringKernel _SubString(const _clsRange& Range)
	{
		StringKernel Word(Range.Length());

		Word._LogicalRange.Assignment(Range);

		return Word;
	}

	vector <StringKernel> _Split(const _clsRange& Separitor) const
	{
		vector <StringKernel> vWords;





		return vWords;
	}

	void _Split(vector <StringKernel>& vPhrase, const _clsRange& Separitor) const
	{
		vPhrase.clear();

		unsigned short SeparatorLength = Separitor.Length();

		char* SeparatorPosition = _LogicalRange.Begin - SeparatorLength;

		while (SeparatorPosition != nullptr)
		{
			SeparatorPosition = _clsRange::NewRange(SeparatorPosition + SeparatorLength, _LogicalRange.End).
				SearchNecklace(Separitor, _clsRange::Left);



		}

	}

protected:

	char* _GetBegin() const
	{
		return _LogicalRange.Begin;
	}

	char* _GetLogicalEnd()  const
	{
		return _LogicalRange.End;
	}

	char* _GetPhysicalEnd() const
	{
		return _PhysicalEnd;
	}

	const char* _GetLogicalEnd(const char Data[], const unsigned short& Length) const
	{
		return Data + Length - 1;
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

		_Allocate(1);

	}

	StringKernel(const char Value[])
	{

		_clsRange ValueRange(Value);

		_Allocate(ValueRange.Length());

		_LogicalRange.Assignment(ValueRange);

	}

	StringKernel(const StringKernel& Value)
	{
		
		_Allocate(Value._LogicalRange.Length());

		_LogicalRange.Assignment(Value._LogicalRange);

	}

	~StringKernel()
	{
		_Deallocate();

		cout << "Hi Destructor !" << endl;
	}

	const char* GetValue() const
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

	StringKernel SubString(unsigned short From, unsigned short To)
	{
		return _SubString(_LogicalRange.SubRange(From, To));		// SubRange can handle From and To if they're outside LogicalRange
	}

	static bool IsUpper(char Character)
	{
		return _clsRange::IsUpper(Character);
	}

	static bool IsLower(char Character)
	{
		return _clsRange::IsLower(Character);
	}

	static bool IsVowel(char Character)
	{
		return _clsRange::IsVowel(Character);
	}

	static bool IsDigit(char Character)
	{
		return _clsRange::IsDigit(Character);
	}

	static char ToUpper(char Letter)
	{

		return _clsRange::ToUpper(Letter);
	}

	static char ToLower(char Letter)
	{
		return _clsRange::ToLower(Letter);
	}

	unsigned short CountAllUpperCase() const
	{
		return _LogicalRange.RawRange().CountAllUpperCase();
	}

	unsigned short CountAllLowerCase() const
	{
		return _LogicalRange.RawRange().CountAllLowerCase();
	}

	unsigned short CountAllVowels() const
	{
		return _LogicalRange.RawRange().CountAllVowels();
	}

	unsigned short CountAllLetters(char Letter, bool MatchCase)
	{
		return _LogicalRange.RawRange().CountAllLetters(Letter, MatchCase);
	}

	unsigned short CountAllWords(const char Separator[]) const
	{
		return _LogicalRange.RawRange().CountAllWords(_clsRange::NewRange(Separator).RawRange());
	}

	unsigned short CountAllWords(const StringKernel& Separator) const
	{
		return _LogicalRange.RawRange().CountAllWords(Separator._LogicalRange.RawRange());
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