#pragma once

#include<iostream>


using namespace std;

class StringKernel
{
protected:

	char* Begin;
	char* LogicalEnd;
	char* PhysicalEnd;

	unsigned short CalculateSize(const char* const& FirstAddress, const char* const& LastAddress)
	{
		return LastAddress - FirstAddress;
	}

	unsigned short CalculateLength(const char* const& FirstAddress, const char* const& LastAddress)
	{
		return LastAddress - FirstAddress + 1;
	}

	const char* GetLogicalEnd(const char* FirstAddress)
	{
		while (*FirstAddress)
		{
			FirstAddress++;
		}

		return FirstAddress;
	}

	const char* GetLogicalEnd(const char* )

	bool IsSameData(StringKernel* Data1, StringKernel* Data2)
	{
		bool IsSame = Data1->Length() == Data2->Length();

		while (IsSame && (Data1->Begin < Data1->LogicalEnd))
		{
			IsSame = Data1->Begin++ == Data2->Begin++;
		}

		return IsSame;
	}

	bool IsSameData(StringKernel* Data2)
	{
		return IsSameData(this, Data2);
	}

	enum enStartFrom { Left, Right };

	char* Write(char* StartWritingAddress, const char* StartReadingAddress, const char* StopReadingAddress
		, enStartFrom StartWritingFrom = Left)
	{
		char* StopWritingAddress = StartWritingAddress + CalculateLength(StartReadingAddress, StopReadingAddress) - 1;

		if (StartWritingFrom == Left)
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

	bool HaveMoreSpace(const unsigned short& NewLength)
	{
		return NewLength <= Capacity();
	}

	unsigned short GetNewCapacity(const unsigned short& NewLength)
	{

		return (NewLength + 15) & ~15;
	}

	char* GetNewPhysicalEnd(const unsigned short& Capacity)
	{
		return Begin + Capacity - 1;
	}

	void HandlePhysicalSpace(const unsigned short& NewLength, const bool& DoSaveData)
	{

		if (!HaveMoreSpace(NewLength))
		{
			char* BeginCopy = Begin;

			unsigned short Capacity = GetNewCapacity(NewLength);

			Begin = new char[Capacity];
			PhysicalEnd = GetNewPhysicalEnd(Capacity);

			if (DoSaveData)
			{
				LogicalEnd = Write(Begin, BeginCopy, LogicalEnd);
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

public:

	StringKernel(const char Value[])
	{
		
	}







	void AddStopCharacter()
	{
		*LogicalEnd = '\0';
	}

	void Clear()
	{

		LogicalEnd = Begin;

		AddStopCharacter();
	}

	bool IsEmpty()
	{
		return LogicalEnd == Begin;
	}

	bool IsFull()
	{
		return LogicalEnd == PhysicalEnd;
	}

	unsigned short Size()
	{
		CalculateSize(Begin, LogicalEnd);
	}

	unsigned short Length()
	{
		CalculateLength(Begin, LogicalEnd);
	}

	unsigned short Capacity()
	{
		return PhysicalEnd - Begin + 1;
	}

	void Assignment(const char Source[], const unsigned short& SourceLength)
	{

		HandlePhysicalSpace(SourceLength, false);

		LogicalEnd = Write(Begin, Source, );
	}

	static char* Concatenate(char*& DestinationFirstAddress, char*& DestinationEndAddress,
		const char* const Source1FirstAddress, const char* const Source1LastAddress, const char* const Source2FirstAddress,
		const char* const Source2LastAddress)
	{
		//213472275300

		HandleArraySpace(DestinationFirstAddress, CalculateLength(Source1FirstAddress, Source1LastAddress)
			+ CalculateLength(Source2FirstAddress, Source2LastAddress) - 1, DestinationEndAddress, false);

		char* DestinationLastAdress = Write(DestinationFirstAddress, Source1FirstAddress, Source1LastAddress - 1);

		DestinationLastAdress = Write(DestinationLastAdress + 1, Source2FirstAddress, Source2LastAddress);

		return DestinationLastAdress;
	}

	static char* Concatenate(char*& DestinationFirstAddress, char*& DestinationEndAddress,const char* const Source1FirstAddress
		, const char* const Source1LastAddress, const char* const Source2FirstAddress)
	{
		return Concatenate(DestinationFirstAddress, DestinationEndAddress, Source1FirstAddress, Source1LastAddress,
			Source2FirstAddress, GetLogicalEnd(Source2FirstAddress));
	}

	static char* Concatenate(char*& DestinationFirstAddress, char*& DestinationEndAddress,const char* const Source1FirstAddress
		, const char* const Source2FirstAddress)
	{
		return Concatenate(DestinationFirstAddress, DestinationEndAddress, Source1FirstAddress
			, GetLogicalEnd(Source1FirstAddress), Source2FirstAddress, GetLogicalEnd(Source2FirstAddress));
	}

	static void Append(char*& DestinationFirstAddress, char*& DestinationLastAddress, char*& DestinationEndAddress,
		const char* const SourceFirstAddress, const char* const SourceLastAddress)
	{

		HandleArraySpace(DestinationFirstAddress, DestinationLastAddress, CalculateLength(SourceFirstAddress, SourceLastAddress) - 1
			, DestinationEndAddress, true);

		DestinationLastAddress = Write(DestinationLastAddress, SourceFirstAddress, SourceLastAddress);

	}

	static void Append(char*& DestinationFirstAddress, char*& DestinationLastAddress, char*& DestinationEndAddress,
		const char* const SourceFirstAddress)
	{
		Append(DestinationFirstAddress, DestinationLastAddress, DestinationEndAddress, SourceFirstAddress
			, GetLogicalEnd(SourceFirstAddress));
	}

	static char* Insert(char*& DestinationFirstAddress, char*& DestinationLastAddress, char*& DestinationEndAddress,
		char*& StartWritingAddress, const char* const SourceFirstAddress, const char* const SourceLastAddress)
	{
		unsigned short LengthToStartWritingAddress = CalculateLength(DestinationFirstAddress, StartWritingAddress),
			SourceLength = CalculateLength(SourceFirstAddress, SourceLastAddress);

		HandleArraySpace(DestinationFirstAddress, DestinationLastAddress, SourceLength - 1, DestinationEndAddress, true);

		StartWritingAddress = DestinationFirstAddress + LengthToStartWritingAddress - 1;
		char* EndWritingAddress = StartWritingAddress + SourceLength - 2;

		DestinationLastAddress = Write(EndWritingAddress + 1, StartWritingAddress, DestinationLastAddress, Right);

	
		return Write(StartWritingAddress, SourceFirstAddress, SourceLastAddress - 1);
	}

	static char* Insert(char*& DestinationFirstAddress, char*& DestinationLastAddress, char*& DestinationEndAddress,
		char*& StartWritingAddress, const char* const SourceFirstAddress)
	{
		return Insert(DestinationFirstAddress, DestinationLastAddress, DestinationEndAddress, StartWritingAddress, SourceFirstAddress
			, GetLogicalEnd(SourceFirstAddress));
	}

	static void Delete(char*& LastAddress, char* const StartDeleteAddress, char* const EndDeleteAddress)
	{
		Write(StartDeleteAddress, LastAddress, EndDeleteAddress + 1, LastAddress, Left);

	}

	static void Delete(char*& LastAddress, char* const StartDeleteAddress, const unsigned short& Offset)
	{
		Delete(LastAddress, StartDeleteAddress, StartDeleteAddress + Offset);
	}

	static char* Preppend(char*& DestinationFirstAddress, char*& DestinationLastAddress, char*& DestinationEndAddress,
		const char* const SourceFirstAddress, const char* const SourceLastAddress)
	{
		unsigned short LengthSource = CalculateLength(SourceFirstAddress, SourceLastAddress);

		HandleArraySpace(DestinationFirstAddress, DestinationLastAddress, LengthSource - 1, DestinationEndAddress, true);

		char* StartWritingAddress = DestinationFirstAddress;
		char* EndWritingAddress = DestinationFirstAddress + LengthSource - 2;

		Write(EndWritingAddress + 1, DestinationLastAddress, DestinationFirstAddress, DestinationLastAddress, Right);

		Write(DestinationFirstAddress, EndWritingAddress, SourceFirstAddress, SourceLastAddress - 1);

		return EndWritingAddress;
	}

	static char* Preppend(char*& DestinationFirstAddress, char*& DestinationLastAddress, char*& DestinationEndAddress,
		const char* const SourceFirstAddress)
	{
		return Preppend(DestinationFirstAddress, DestinationLastAddress, DestinationEndAddress, SourceFirstAddress
			, GetLogicalEnd(SourceFirstAddress));
	}

	static char* SwapAddress(const char*& Address1, const char*& Address2)
	{
		const char* TempAddress = Address1;
		Address1 = Address2;
		Address2 = TempAddress;
	}

	static void SwapData(const char*& FirstAddress1, const char*& LastAddress1, const char*& EndAddress1, const char*& FirstAddress2,
		const char*& LastAddress2, const char*& EndAddress2)
	{
		SwapAddress(FirstAddress1, FirstAddress2);
		SwapAddress(LastAddress1, LastAddress2);
		SwapAddress(EndAddress1, EndAddress2);

	}

	static void PushBack(char*& FirstAddress, char*& LastAddress, char*& EndAddress, const char& Ch_)
	{
		HandleArraySpace(FirstAddress, LastAddress, 1, EndAddress, true);

		*LastAddress = Ch_;

		AddStopCharacter(++LastAddress);
	}

	static void PopBack(char*& LastAddress)
	{
		AddStopCharacter(--LastAddress);
	}

	static char* Find(const char* TextFirstAddress, const char* TextLastAddress, const char* WordFirstAddress,
		const char* WordLastAddress, const enStartFrom& StartReadingFrom = Left)
	{
		bool IsFound = false;
		--WordLastAddress;

		if (StartReadingFrom == Left)
		{
			while ((TextFirstAddress < TextLastAddress) && !IsFound)
			{
				WordFirstAddress += *TextFirstAddress++ == *WordFirstAddress ? 1 : 0;

				IsFound = WordFirstAddress == WordLastAddress;
			}

		}
		else
		{

		}


	}



	static void Print(const char* FirstAddress, const char* LastAddress)
	{
		while (FirstAddress < LastAddress)
		{
			cout << *FirstAddress++;
		}
		cout << endl;
	}

	static void Print(const char* FirstAddress)
	{
		Print(FirstAddress, GetLogicalEnd(FirstAddress));
	}







};