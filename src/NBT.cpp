#include <iostream>
#include <sstream>
#include <zlib.h>
#include "NBT.hpp"
#include "Utilities.hpp"

//////////////
// Base Tag //
//////////////
nbt::Tag::Tag(const std::string &Name){
	TagName = Name;
}

const std::string &nbt::Tag::GetName() const{
	return TagName;
}

int8_t nbt::Tag::GetType() const{
	return 0;
}

void nbt::Tag::SetName(const std::string &Name){
	TagName = Name;
}

nbt::Tag::~Tag(){
}

//////////////
// Byte Tag //
//////////////
nbt::Byte::Byte(const std::string &Name, const int8_t &Value) : nbt::Tag(Name){
	Payload = Value;
}

const int8_t &nbt::Byte::GetPayload() const{
	return Payload;
}

int8_t nbt::Byte::GetType() const{
	return 1;
}

bool nbt::Byte::Load(std::istream &Stream, bool LoadName){
	if (!Stream.good())
		return false;
	if (LoadName){
		nbt::String Name;
		if (!Name.Load(Stream, false))
			return false;
		SetName(Name.GetPayload());
	}
	Payload = Stream.get();
	if (!Stream.good())
		return false;
	return true;
}

bool nbt::Byte::Save(std::ostream &Stream, bool WriteName){
	if (!Stream.good())
		return false;
	if (WriteName){
		Stream.put(GetType());
		if (!Stream.good())
			return false;
		nbt::String Name("", GetName());
		if (!Name.Save(Stream, false))
			return false;
	}
	Stream.put(Payload);
	if (!Stream.good())
		return false;
	return true;
}

void nbt::Byte::SetPayload(const int8_t &Value){
	Payload = Value;
}

nbt::Byte::~Byte(){
}

///////////////
// Short Tag //
///////////////
nbt::Short::Short(const std::string &Name, const int16_t &Value) : nbt::Tag(Name){
	Payload = Value;
}

const int16_t &nbt::Short::GetPayload() const{
	return Payload;
}

int8_t nbt::Short::GetType() const{
	return 2;
}

bool nbt::Short::Load(std::istream &Stream, bool LoadName){
	if (!Stream.good())
		return false;
	if (LoadName){
		nbt::String Name;
		if (!Name.Load(Stream, false))
			return false;
		SetName(Name.GetPayload());
	}
	Stream.read(reinterpret_cast<char*>(&Payload), 2);
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 2);
	if (!Stream.good())
		return false;
	return true;
}

bool nbt::Short::Save(std::ostream &Stream, bool WriteName){
	if (!Stream.good())
		return false;
	if (WriteName){
		Stream.put(GetType());
		if (!Stream.good())
			return false;
		nbt::String Name("", GetName());
		if (!Name.Save(Stream, false))
			return false;
	}
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 2);
	Stream.write(reinterpret_cast<char*>(&Payload), 2);
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 2);
	if (!Stream.good())
		return false;
	return true;
}

void nbt::Short::SetPayload(const int16_t &Value){
	Payload = Value;
}

nbt::Short::~Short(){
}

/////////////
// Int Tag //
/////////////
nbt::Int::Int(const std::string &Name, const int32_t &Value) : nbt::Tag(Name){
	Payload = Value;
}

const int32_t &nbt::Int::GetPayload() const{
	return Payload;
}

int8_t nbt::Int::GetType() const{
	return 3;
}

bool nbt::Int::Load(std::istream &Stream, bool LoadName){
	if (!Stream.good())
		return false;
	if (LoadName){
		nbt::String Name;
		if (!Name.Load(Stream, false))
			return false;
		SetName(Name.GetPayload());
	}
	Stream.read(reinterpret_cast<char*>(&Payload), 4);
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 4);
	if (!Stream.good())
		return false;
	return true;
}

bool nbt::Int::Save(std::ostream &Stream, bool WriteName){
	if (!Stream.good())
		return false;
	if (WriteName){
		Stream.put(GetType());
		if (!Stream.good())
			return false;
		nbt::String Name("", GetName());
		if (!Name.Save(Stream, false))
			return false;
	}
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 4);
	Stream.write(reinterpret_cast<char*>(&Payload), 4);
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 4);
	if (!Stream.good())
		return false;
	return true;
}

void nbt::Int::SetPayload(const int32_t &Value){
	Payload = Value;
}

nbt::Int::~Int(){
}

//////////////
// Long Tag //
//////////////
nbt::Long::Long(const std::string &Name, const int64_t &Value) : nbt::Tag(Name){
	Payload = Value;
}

const int64_t &nbt::Long::GetPayload() const{
	return Payload;
}

int8_t nbt::Long::GetType() const{
	return 4;
}

bool nbt::Long::Load(std::istream &Stream, bool LoadName){
	if (!Stream.good())
		return false;
	if (LoadName){
		nbt::String Name;
		if (!Name.Load(Stream, false))
			return false;
		SetName(Name.GetPayload());
	}
	Stream.read(reinterpret_cast<char*>(&Payload), 8);
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 8);
	if (!Stream.good())
		return false;
	return true;
}

bool nbt::Long::Save(std::ostream &Stream, bool WriteName){
	if (!Stream.good())
		return false;
	if (WriteName){
		Stream.put(GetType());
		if (!Stream.good())
			return false;
		nbt::String Name("", GetName());
		if (!Name.Save(Stream, false))
			return false;
	}
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 8);
	Stream.write(reinterpret_cast<char*>(&Payload), 8);
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 8);
	if (!Stream.good())
		return false;
	return true;
}

void nbt::Long::SetPayload(const int64_t &Value){
	Payload = Value;
}

nbt::Long::~Long(){
}

///////////////
// Float Tag //
///////////////
nbt::Float::Float(const std::string &Name, const float &Value) : nbt::Tag(Name){
	Payload = Value;
}

const float &nbt::Float::GetPayload() const{
	return Payload;
}

int8_t nbt::Float::GetType() const{
	return 5;
}

bool nbt::Float::Load(std::istream &Stream, bool LoadName){
	if (!Stream.good())
		return false;
	if (LoadName){
		nbt::String Name;
		if (!Name.Load(Stream, false))
			return false;
		SetName(Name.GetPayload());
	}
	Stream.read(reinterpret_cast<char*>(&Payload), 4);
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 4);
	if (!Stream.good())
		return false;
	return true;
}

bool nbt::Float::Save(std::ostream &Stream, bool WriteName){
	if (!Stream.good())
		return false;
	if (WriteName){
		Stream.put(GetType());
		if (!Stream.good())
			return false;
		nbt::String Name("", GetName());
		if (!Name.Save(Stream, false))
			return false;
	}
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 4);
	Stream.write(reinterpret_cast<char*>(&Payload), 4);
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 4);
	if (!Stream.good())
		return false;
	return true;
}

void nbt::Float::SetPayload(const float &Value){
	Payload = Value;
}

nbt::Float::~Float(){
}

////////////////
// Double Tag //
////////////////
nbt::Double::Double(const std::string &Name, const double &Value) : nbt::Tag(Name){
	Payload = Value;
}

const double &nbt::Double::GetPayload() const{
	return Payload;
}

int8_t nbt::Double::GetType() const{
	return 6;
}

bool nbt::Double::Load(std::istream &Stream, bool LoadName){
	if (!Stream.good())
		return false;
	if (LoadName){
		nbt::String Name;
		if (!Name.Load(Stream, false))
			return false;
		SetName(Name.GetPayload());
	}
	Stream.read(reinterpret_cast<char*>(&Payload), 8);
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 8);
	if (!Stream.good())
		return false;
	return true;
}

bool nbt::Double::Save(std::ostream &Stream, bool WriteName){
	if (!Stream.good())
		return false;
	if (WriteName){
		Stream.put(GetType());
		if (!Stream.good())
			return false;
		nbt::String Name("", GetName());
		if (!Name.Save(Stream, false))
			return false;
	}
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 8);
	Stream.write(reinterpret_cast<const char*>(&Payload), 8);
	if (!terra::IsBigEndian())
		terra::SwapEndianness(reinterpret_cast<char*>(&Payload), 8);
	if (!Stream.good())
		return false;
	return true;
}

void nbt::Double::SetPayload(const double &Value){
	Payload = Value;
}

nbt::Double::~Double(){
}

////////////////////
// Byte Array Tag //
////////////////////
nbt::ByteArray::ByteArray(const std::string &Name, const std::vector<char> &Value) : nbt::Tag(Name){
	Payload = Value;
}

const std::vector<char> &nbt::ByteArray::GetPayload() const{
	return Payload;
}

int8_t nbt::ByteArray::GetType() const{
	return 7;
}

bool nbt::ByteArray::Load(std::istream &Stream, bool LoadName){
	if (!Stream.good())
		return false;
	if (LoadName){
		nbt::String Name;
		if (!Name.Load(Stream, false))
			return false;
		SetName(Name.GetPayload());
	}
	nbt::Int Length;
	if (!Length.Load(Stream, false))
		return false;
	for (int32_t i = 0; i < Length.GetPayload(); ++i){
		char Buffer;
		Stream.get(Buffer);
		if (!Stream.good())
			return false;
		Payload.push_back(Buffer);
	}
	if (!Stream.good())
		return false;
	return true;
}

bool nbt::ByteArray::Save(std::ostream &Stream, bool WriteName){
	if (!Stream.good())
		return false;
	if (WriteName){
		Stream.put(GetType());
		if (!Stream.good())
			return false;
		nbt::String Name("", GetName());
		if (!Name.Save(Stream, false))
			return false;
	}
	nbt::Int Length("", Payload.size());
	if (!Length.Save(Stream, false))
		return false;
	for (int i = 0; i < Length.GetPayload(); ++i){
		Stream.put(Payload.at(i));
		if (!Stream.good())
			return false;
	}
	if (!Stream.good())
		return false;
	return true;
}

void nbt::ByteArray::SetPayload(const std::vector<char> &Value){
	Payload = Value;
}

nbt::ByteArray::~ByteArray(){
}

////////////////
// String Tag //
////////////////
nbt::String::String(const std::string &Name, const std::string &Value) : nbt::Tag(Name){
	Payload = Value;
}

const std::string &nbt::String::GetPayload() const{
	return Payload;
}

int8_t nbt::String::GetType() const{
	return 8;
}

bool nbt::String::Load(std::istream &Stream, bool LoadName){
	if (!Stream.good())
		return false;
	if (LoadName){
		nbt::String Name;
		if (!Name.Load(Stream, false))
			return false;
		SetName(Name.GetPayload());
	}
	nbt::Short Length;
	if (!Length.Load(Stream, false))
		return false;
	Payload = "";
	for (int16_t i = 0; i < Length.GetPayload(); ++i){
		Payload += Stream.get();
		if (!Stream.good())
			return false;
	}
	if (!Stream.good())
		return false;
	return true;
}

bool nbt::String::Save(std::ostream &Stream, bool WriteName){
	if (!Stream.good())
		return false;
	if (WriteName){
		Stream.put(GetType());
		if (!Stream.good())
			return false;
		nbt::String Name("", GetName());
		if (!Name.Save(Stream, false))
			return false;
	}
	nbt::Short Length("", Payload.size());
	if (!Length.Save(Stream, false))
		return false;
	Stream.write(Payload.c_str(), Length.GetPayload());
	if (!Stream.good())
		return false;
	return true;
}

void nbt::String::SetPayload(const std::string &Value){
	Payload = Value;
}

nbt::String::~String(){
}

//////////////
// List Tag //
//////////////
nbt::List::List(const std::string &Name, const std::vector<std::shared_ptr<nbt::Tag>> &Value) : nbt::Tag(Name){
	Payload = Value;
	Type = 0;
	ClearMismatchedTypes();
}

void nbt::List::ClearMismatchedTypes(){
	if (Type == 0 && Payload.size() > 0)
		Type = Payload.at(0)->GetType();
	std::vector<std::shared_ptr<nbt::Tag>> Value;
	for (auto i = Payload.begin(); i != Payload.end(); ++i){
		if ((*i)->GetType() == Type)
			Value.push_back(*i);
	}
	Payload = Value;
	if (Payload.size() == 0)
		Type = 0;
}

const std::vector<std::shared_ptr<nbt::Tag>> &nbt::List::GetPayload() const{
	return Payload;
}

int8_t nbt::List::GetTagType() const{
	return Type;
}

int8_t nbt::List::GetType() const{
	return 9;
}

bool nbt::List::Load(std::istream &Stream, bool LoadName){
	if (!Stream.good())
		return false;
	if (LoadName){
		nbt::String Name;
		if (!Name.Load(Stream, false))
			return false;
		SetName(Name.GetPayload());
	}
	nbt::Byte TagType;
	if (!TagType.Load(Stream, false))
		return false;
	Type = TagType.GetPayload();
	if (Type <= 0 || Type > 10){
		Type = 0;
		return false;
	}
	nbt::Int Length;
	if (!Length.Load(Stream, false))
		return false;
	std::vector<std::shared_ptr<nbt::Tag>> Values;
	for (int32_t i = 0; i < Length.GetPayload(); ++i){
		std::shared_ptr<nbt::Tag> NewTag;
		switch (Type){
			case 1:{
				NewTag.reset(new nbt::Byte());
				break;
			}
			case 2:{
				NewTag.reset(new nbt::Short());
				break;
			}
			case 3:{
				NewTag.reset(new nbt::Int());
				break;
			}
			case 4:{
				NewTag.reset(new nbt::Long());
				break;
			}
			case 5:{
				NewTag.reset(new nbt::Float());
				break;
			}
			case 6:{
				NewTag.reset(new nbt::Double());
				break;
			}
			case 7:{
				NewTag.reset(new nbt::ByteArray());
				break;
			}
			case 8:{
				NewTag.reset(new nbt::String());
				break;
			}
			case 9:{
				NewTag.reset(new nbt::List());
				break;
			}
			case 10:{
				NewTag.reset(new nbt::Compound());
				break;
			}
		}
		if (!NewTag->Load(Stream, false))
			return false;
		Values.push_back(NewTag);
	}
	Payload = Values;
	return true;
}

bool nbt::List::Save(std::ostream &Stream, bool WriteName){
	if (!Stream.good())
		return false;
	if (WriteName){
		Stream.put(GetType());
		if (!Stream.good())
			return false;
		nbt::String Name("", GetName());
		if (!Name.Save(Stream, false))
			return false;
	}
	nbt::Byte TagType("", Type);
	if (!TagType.Save(Stream, false))
		return false;
	nbt::Int Length("", Payload.size());
	if (!Length.Save(Stream, false))
		return false;
	for (unsigned int i = 0; i < Payload.size(); ++i){
		if (!Payload.at(i)->Save(Stream, false))
			return false;
	}
	return true;
}

void nbt::List::SetPayload(const std::vector<std::shared_ptr<Tag>> &Value){
	Payload = Value;
	Type = 0;
	ClearMismatchedTypes();
}

nbt::List::~List(){
}

//////////////////
// Compound Tag //
//////////////////
nbt::Compound::Compound(const std::string &Name, const std::vector<std::shared_ptr<nbt::Tag>> &Value) : nbt::Tag(Name){
	Payload = Value;
}

std::vector<std::shared_ptr<nbt::Tag>>::iterator nbt::Compound::FirstChild(std::string Name){
	for (auto i = Payload.begin(); i != Payload.end(); ++i){
		if ((*i)->GetName() == Name)
			return i;
	}
	return Payload.end();
}

const std::vector<std::shared_ptr<nbt::Tag>> &nbt::Compound::GetPayload() const{
	return Payload;
}

int8_t nbt::Compound::GetType() const{
	return 10;
}

bool nbt::Compound::Load(std::istream &Stream, bool LoadName){
	if (!Stream.good())
		return false;
	if (LoadName){
		nbt::String Name;
		if (!Name.Load(Stream, false))
			return false;
		SetName(Name.GetPayload());
	}
	std::vector<std::shared_ptr<nbt::Tag>> Value;
	bool EndFound = false;
	while (!EndFound){
		nbt::Byte TagType;
		TagType.Load(Stream, false);
		if (TagType.GetPayload() < 0 || TagType.GetPayload() > 10)
			return false;
		std::shared_ptr<nbt::Tag> NewTag;
		switch (TagType.GetPayload()){
			case 0:{
				EndFound = true;
				break;
			}
			case 1:{
				NewTag.reset(new nbt::Byte());
				break;
			}
			case 2:{
				NewTag.reset(new nbt::Short());
				break;
			}
			case 3:{
				NewTag.reset(new nbt::Int());
				break;
			}
			case 4:{
				NewTag.reset(new nbt::Long());
				break;
			}
			case 5:{
				NewTag.reset(new nbt::Float());
				break;
			}
			case 6:{
				NewTag.reset(new nbt::Double());
				break;
			}
			case 7:{
				NewTag.reset(new nbt::ByteArray());
				break;
			}
			case 8:{
				NewTag.reset(new nbt::String());
				break;
			}
			case 9:{
				NewTag.reset(new nbt::List());
				break;
			}
			case 10:{
				NewTag.reset(new nbt::Compound());
				break;
			}
		}
		if (EndFound)
			break;
		if (!NewTag->Load(Stream))
			return false;
		Value.push_back(NewTag);
	}
	Payload = Value;
	return true;
}

std::vector<std::shared_ptr<nbt::Tag>>::iterator nbt::Compound::NextChild(std::string Name, std::vector<std::shared_ptr<Tag>>::iterator Current){
	for (auto i = Current+1; i != Payload.end(); ++i){
		if ((*i)->GetName() == Name)
			return i;
	}
	return Payload.end();
}

bool nbt::Compound::Save(std::ostream &Stream, bool WriteName){
	if (!Stream.good())
		return false;
	if (WriteName){
		Stream.put(GetType());
		if (!Stream.good())
			return false;
		nbt::String Name("", GetName());
		if (!Name.Save(Stream, false))
			return false;
	}
	for (unsigned int i = 0; i < Payload.size(); ++i){
		if (!Payload.at(i)->Save(Stream))
			return false;
	}
	Stream.put(0);
	if (!Stream.good())
		return false;
	return true;
}

void nbt::Compound::SetPayload(const std::vector<std::shared_ptr<nbt::Tag>> &Value){
	Payload = Value;
}

nbt::Compound::~Compound(){
}

//////////
// File //
//////////
nbt::File::File(){
}

nbt::File::File(const std::string &Filename){
	Load(Filename);
}

std::shared_ptr<nbt::Compound> &nbt::File::GetRoot(){
	return Root;
}

bool nbt::File::Load(const std::string &Filename){
	gzFile NBTFile = gzopen(Filename.c_str(), "rb");
	if (!NBTFile){
		std::cerr << "Unable to open NBT file " << Filename << " for reading\n";
		return false;
	}
	if (gzbuffer(NBTFile, 65536) == -1){
		std::cerr << "Unable to allocate 64KB buffer for reading NBT file " << Filename << '\n';
		gzclose(NBTFile);
		return false;
	}
	std::string FileContents;
	while (true){
		char Buffer;
		int Code = gzread(NBTFile, &Buffer, 1);
		if (Code == -1){
			std::cerr << "Unable to read NBT file " << Filename << '\n';
			gzclose(NBTFile);
			return false;
		}
		if (Code == 0)
			break;
		FileContents += Buffer;
	}
	gzclose(NBTFile);
	std::istringstream Stream(FileContents, std::istringstream::in | std::istringstream::binary);
	if (Stream.get() != 10){
		std::cerr << "No root compound in NBT file " << Filename << '\n';
		return false;
	}
	Root.reset(new nbt::Compound());
	if (!Root->Load(Stream)){
		std::cerr << "Unable to parse NBT file " << Filename << '\n';
		Root.reset();
		return false;
	}
	return true;
}

bool nbt::File::Save(const std::string &Filename){
	if (!Root){
		std::cerr << "No root compound exists to be written to NBT file " << Filename << '\n';
		return false;
	}
	gzFile NBTFile = gzopen(Filename.c_str(), "wb");
	if (!NBTFile){
		std::cerr << "Unable to open NBT file " << Filename << " for writing\n";
		return false;
	}
	if (gzbuffer(NBTFile, 65536) == -1){
		std::cerr << "Unable to allocate 64KB buffer for writing NBT file " << Filename << '\n';
		gzclose(NBTFile);
		return false;
	}
	std::ostringstream Stream(std::ostringstream::out | std::ostringstream::binary);
	if (!Root->Save(Stream)){
		std::cerr << "Unable to write NBT file " << Filename << '\n';
		gzclose(NBTFile);
		return false;
	}
	std::string FileContents = Stream.str();
	if (gzwrite(NBTFile, FileContents.c_str(), FileContents.size()) == 0){
		std::cerr << "Unable to write NBT file " << Filename << '\n';
		gzclose(NBTFile);
		return false;
	}
	gzclose(NBTFile);
	return true;
}

void nbt::File::SetRoot(const std::shared_ptr<nbt::Compound> &NewRoot){
	if (NewRoot.get())
		Root = NewRoot;
}

nbt::File::~File(){
}
