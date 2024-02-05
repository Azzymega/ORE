#include "Reflector.h"
#include "Extension/Array.h"
#include "../libDraco/Loader.h"
#include "Extension/Class.h"
#include "../libk/UUID.h"

void World::Initialize(Object Count) {
    this->Count = Count;
    char Buffer[20];

    for (int i = 0; i < Count; ++i) {

        this->Experiences[i].Initialize();
        this->Experiences[i].ID = i;

        printf("[Reflector] Initializing ");
        printf(itoa(i,Buffer,10));
        printf(" block\n");

    }

}

GUID World::Add(Object Value) {
    Experience* Data = nullptr;
    Experience* Definition = nullptr;

    Data = Get();
    Definition = GetDefinition();

    assert(Data == nullptr, "[Reflector] Free matter block not found! OutOfMemory!!!");
    assert(Definition == nullptr, "[Reflector] Free consciousness block not found! OutOfMemory!!!");

    Location Position = Data->Matter.Add(Value);

    assert(Position.Field == BadPointer, "[Reflector] Allocation failed!!!");

    Position.Experience = Data->ID;
    GUID Identifier = Definition->Consciousness.Add(Position);

    return Identifier;
}

GUID World::Add(int Value) {
    Experience* Data = nullptr;
    Experience* Definition = nullptr;

    Data = Get();
    Definition = GetDefinition();

    assert(Data == nullptr, "[Reflector] Free matter block not found! OutOfMemory!!!");
    assert(Definition == nullptr, "[Reflector] Free consciousness block not found! OutOfMemory!!!");

    Location Position = Data->Matter.Add(Value);

    assert(Position.Field == BadPointer, "[Reflector] Allocation failed!!!");

    Position.Experience = Data->ID;
    GUID Identifier = Definition->Consciousness.Add(Position);

    return Identifier;
}

GUID World::Add(long Value) {
    Experience* Data = nullptr;
    Experience* Definition = nullptr;

    Data = Get();
    Definition = GetDefinition();

    assert(Data == nullptr, "[Reflector] Free matter block not found! OutOfMemory!!!");
    assert(Definition == nullptr, "[Reflector] Free consciousness block not found! OutOfMemory!!!");

    Location Position = Data->Matter.Add(Value);

    assert(Position.Field == BadPointer, "[Reflector] Allocation failed!!!");

    Position.Experience = Data->ID;
    GUID Identifier = Definition->Consciousness.Add(Position);

    return Identifier;
}

GUID World::Add(float Value) {
    Experience* Data = nullptr;
    Experience* Definition = nullptr;

    Data = Get();
    Definition = GetDefinition();

    assert(Data == nullptr, "[Reflector] Free matter block not found! OutOfMemory!!!");
    assert(Definition == nullptr, "[Reflector] Free consciousness block not found! OutOfMemory!!!");

    Location Position = Data->Matter.Add(Value);

    assert(Position.Field == BadPointer, "[Reflector] Allocation failed!!!");

    Position.Experience = Data->ID;
    GUID Identifier = Definition->Consciousness.Add(Position);

    return Identifier;
}

GUID World::Add(double Value) {
    Experience* Data = nullptr;
    Experience* Definition = nullptr;

    Data = Get();
    Definition = GetDefinition();

    assert(Data == nullptr, "[Reflector] Free matter block not found! OutOfMemory!!!");
    assert(Definition == nullptr, "[Reflector] Free consciousness block not found! OutOfMemory!!!");

    Location Position = Data->Matter.Add(Value);

    assert(Position.Field == BadPointer, "[Reflector] Allocation failed!!!");

    Position.Experience = Data->ID;
    GUID Identifier = Definition->Consciousness.Add(Position);

    return Identifier;
}

Experience *World::Get(Object BlocksCount) {
    Experience* Chunk = nullptr;

    for (int i = 0; i < this->Count; ++i) {
        if (!this->Experiences[i].Full && this->Experiences[i].Type == Type::TMatter && this->Experiences[i].State == State::Used) {
            Chunk = this->Experiences[i].Check(BlocksCount);
            if (Chunk != nullptr) {
                return Chunk;
            }
        }
    }

    if (Chunk == nullptr) {
        for (int i = 0; i < this->Count; ++i) {
            if (!this->Experiences[i].Full && this->Experiences[i].State == State::Free) {
                Chunk = this->Experiences[i].Initialize(Type::TMatter);
                return Chunk;
            }
        }
    }

    return Chunk;
}

Experience *World::Get() {
    Experience* Chunk = nullptr;

    for (int i = 0; i < this->Count; ++i) {
        if (!this->Experiences[i].Full && this->Experiences[i].Type == Type::TMatter && this->Experiences[i].State == State::Used) {
            Chunk = this->Experiences[i].Check();
            if (Chunk != nullptr) {
                return Chunk;
            }
        }
    }

    if (Chunk == nullptr) {
        for (int i = 0; i < this->Count; ++i) {
            if (!this->Experiences[i].Full && this->Experiences[i].State == State::Free) {
                Chunk = this->Experiences[i].Initialize(Type::TMatter);
                if (Chunk != nullptr) {
                    return Chunk;
                }
            }
        }
    }

    return Chunk;
}

Experience *World::GetDefinition(Object Length) {
    Experience* Chunk = nullptr;

    for (int i = 0; i < this->Count; ++i) {
        if (!this->Experiences[i].Full && this->Experiences[i].Type == Type::TConsciousness && this->Experiences[i].State == State::Used) {
            Chunk = this->Experiences[i].Check(Length);
            if (Chunk != nullptr) {
                break;
            }
        }
    }

    if (Chunk == nullptr) {
        for (int i = 0; i < this->Count; ++i) {
            if (!this->Experiences[i].Full && this->Experiences[i].State == State::Free) {
                Chunk = this->Experiences[i].Initialize(Type::TConsciousness);
                if (Chunk != nullptr) {
                    break;
                }
            }
        }
    }

    return Chunk;
}

Experience *World::GetDefinition() {
    Experience* Chunk = nullptr;

    for (int i = 0; i < this->Count; ++i) {
        if (!this->Experiences[i].Full && this->Experiences[i].Type == Type::TConsciousness && this->Experiences[i].State == State::Used) {
            Chunk = this->Experiences[i].Check();
            if (Chunk != nullptr) {
                break;
            }
        }
    }

    if (Chunk == nullptr) {
        for (int i = 0; i < this->Count; ++i) {
            if (!this->Experiences[i].Full && this->Experiences[i].State == State::Free) {
                Chunk = this->Experiences[i].Initialize(Type::TConsciousness);
                if (Chunk != nullptr) {
                    break;
                }
            }
        }
    }

    return Chunk;
}

GUID World::AddPacked(size_t Length) {
    Experience* Data = nullptr;
    Experience* Definition = nullptr;
    Object BlockCount = 1;

    if (Length > BlockSize) {
        BlockCount = (Length/8)+(Length%8)+15;
    }

    Data = Get(BlockCount);
    Definition = GetDefinition();

    assert(Data == nullptr, "[Reflector] Free matter block not found! OutOfMemory!!!");
    assert(Definition == nullptr, "[Reflector] Free consciousness block not found! OutOfMemory!!!");

    Packed Position = Data->Matter.AddPacked(BlockCount);

    assert(Position.Field == BadPointer, "[Reflector] Packed allocation failed!!!");

    Position.Experience = Data->ID;

    GUID Identifier = Definition->Consciousness.AddPacked(Position);

    GUID Null = GenerateNullGUID1();

    if (memcmp(&Identifier,&Null,sizeof(GUID)) == 0) {
        assert(true,"[Reflector] Packed allocation failed!");
    }

    return Identifier;
}

Object World::Get(GUID Identifier) {
    Location Position;
    Object Value;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            Position = Experiences[i].Consciousness.Get(Identifier);

            assert(Position.Field >= 115000,"[Failure] [Reflector] Native object field out of index!");

            if (Position.Field != BadPointer) {
                break;
            }
        }
    }

    Value = this->Experiences[Position.Experience].Matter.Objects[Position.Field].Object;

    return Value;
}

void *World::GetPacked(GUID Identifier) {
    Packed* Position = nullptr;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {

            Position = Experiences[i].Consciousness.GetPacked(Identifier);

            assert(Position == nullptr,"[Failure] [Reflector] Packed nullptr!!!");
            assert(Position->Field >= 115000,"[Failure] [Reflector] Packed field out of index!");

        }
    }

    return &this->Experiences[Position->Experience].Matter.Objects[Position->Field];
}

GUID World::AddArray(Object Length) {
    Experience* Definition = nullptr;

    Definition = GetDefinition();

    assert(Definition == nullptr, "[Reflector] Free consciousness for array not found! OutOfMemory!!!");

    GUID Identifier = Definition->Consciousness.AddArray(Length);

    return Identifier;
}

Representation *World::GetArray(GUID Array) {
    Representation* ArrayDefinition = nullptr;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            ArrayDefinition = Experiences[i].Consciousness.GetArray(Array);
            if (ArrayDefinition != nullptr) {
                return ArrayDefinition;
            }
        }
    }

    assert(true, "[Reflector] Array bad pointer!!!");

    return ArrayDefinition;
}

void World::SetArrayValue(GUID Array, Object Index, GUID Value) {
    Representation* ObjectDefinition = GetArray(Array);

    assert(ObjectDefinition == nullptr, "[Reflector] Failed to find array representation!");

    Parts* ArrayDefinition = ObjectDefinition->GetParts();

    ArrayDefinition->Set(Index,Value);
}

GUID World::GetArrayValue(GUID Array, Object Index) {
    Representation* ObjectDefinition = GetArray(Array);

    Parts* ArrayDefinition = ObjectDefinition->GetParts();

    assert(ArrayDefinition == nullptr,"[Reflector] Failed to get array definition!");

    return ArrayDefinition->Get(Index);
}

GUID World::AddClass(ClassFile File) {

    Experience* Definition = nullptr;

    Definition = GetDefinition();

    assert(Definition == nullptr, "[Reflector] Failed to find free consciousness block! OutOfMemory!!!");

    GUID Identifier = Definition->Consciousness.AddClass(File);

    return Identifier;
}

Class *World::GetClass(GUID Address) {
    Class* ClassDefinition = nullptr;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            ClassDefinition = Experiences[i].Consciousness.GetClass(Address);
            if (ClassDefinition != nullptr) {
                return ClassDefinition;
            }
        }
    }

    assert(true, "[Reflector] [ClassExtension] Failed to get class!!!");

    return ClassDefinition;
}

GUID World::GetClassReference(const char *ClassName) {
    GUID ClassDefinition = GenerateNullGUID1();

    for (int i = 0; i < this->Count; ++i) {
        // FIXME
        GUID NullGUID = GenerateNullGUID1();
        if (Experiences[i].Type == Type::TConsciousness) {
            ClassDefinition = Experiences[i].Consciousness.GetClassReference(ClassName);
            if (memcmp(&ClassDefinition.Internal,&NullGUID.Internal,sizeof(GUID)) != 0) {
                return ClassDefinition;
            }
        }
    }

    assert(true, "[Reflector] [ClassExtension] Failed to get class!!!");

    return ClassDefinition;
}

Class *World::GetClass(const char *ClassName) {
    Class* ClassDefinition = nullptr;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            ClassDefinition = Experiences[i].Consciousness.GetClass(ClassName);
            if (ClassDefinition != nullptr) {
                return ClassDefinition;
            }
        }
    }

    assert(true, "[Reflector] [ClassExtension] Failed to get class!!!");

    return ClassDefinition;
}

Object World::GetArrayLength(GUID Array) {
    Representation* ObjectDefinition = GetArray(Array);

    Parts* ArrayDefinition = ObjectDefinition->GetParts();

    assert(true, "[Reflector] [ClassExtension] Failed to get array length!!!");
    return ArrayDefinition->GetLength();
}

void World::Set(GUID Identifier, float Value) {
    Location Position;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            Position = Experiences[i].Consciousness.Get(Identifier);
            if (Position.Field != BadPointer) {
                break;
            }
        }
    }

    assert(Position.Field == BadPointer,"[Reflector] Failed to find representation!");
    this->Experiences[Position.Experience].Matter.Set(Position,Value);
}

void World::Set(GUID Identifier, double Value) {
    Location Position;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            Position = Experiences[i].Consciousness.Get(Identifier);
            if (Position.Field != BadPointer) {
                break;
            }
        }
    }

    assert(Position.Field == BadPointer,"[Reflector] Failed to find representation!");
    this->Experiences[Position.Experience].Matter.Set(Position,Value);
}

void World::Set(GUID Identifier, Object Value) {
    Location Position;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            Position = Experiences[i].Consciousness.Get(Identifier);
            if (Position.Field != BadPointer) {
                break;
            }
        }
    }

    assert(Position.Field == BadPointer,"[Reflector] Failed to find representation!");
    this->Experiences[Position.Experience].Matter.Set(Position,Value);
}

float World::GetF(GUID Identifier) {
    Location Position;
    float Value;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            Position = Experiences[i].Consciousness.Get(Identifier);
            if (Position.Field != BadPointer) {
                break;
            }
        }
    }

    Value = this->Experiences[Position.Experience].Matter.GetF(Position);

    return Value;
}

double World::GetD(GUID Identifier) {
    Location Position;
    double Value;

    for (size_t i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            Position = Experiences[i].Consciousness.Get(Identifier);
            if (Position.Field != BadPointer) {
                break;
            }
        }
    }

    Value = this->Experiences[Position.Experience].Matter.GetD(Position);

    return Value;
}

int World::GetI(GUID Identifier) {
    Location Position;
    int Value;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            Position = Experiences[i].Consciousness.Get(Identifier);
            if (Position.Field != BadPointer) {
                break;
            }
        }
    }

    Value = this->Experiences[Position.Experience].Matter.GetI(Position);

    return Value;
}

long World::GetL(GUID Identifier) {
    Location Position;
    long Value;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            Position = Experiences[i].Consciousness.Get(Identifier);
            if (Position.Field != BadPointer) {
                break;
            }
        }
    }

    Value = this->Experiences[Position.Experience].Matter.GetL(Position);

    return Value;
}

void World::Set(GUID Identifier, int Value) {
    Location Position;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            Position = Experiences[i].Consciousness.Get(Identifier);
            if (Position.Field != BadPointer) {
                break;
            }
        }
    }

    assert(Position.Field == BadPointer,"[Reflector] Failed to find representation!");
    this->Experiences[Position.Experience].Matter.Set(Position,Value);
}

void World::Set(GUID Identifier, long Value) {
    Location Position;

    for (int i = 0; i < this->Count; ++i) {
        if (Experiences[i].Type == Type::TConsciousness) {
            Position = Experiences[i].Consciousness.Get(Identifier);
            if (Position.Field != BadPointer) {
                break;
            }
        }
    }

    assert(Position.Field == BadPointer,"[Reflector] Failed to find representation!");
    this->Experiences[Position.Experience].Matter.Set(Position,Value);
}

GUID World::AddObject(GUID Template) {
    Experience* Definition = nullptr;

    Definition = GetDefinition();

    assert(Definition == nullptr, "[Reflector] Free consciousness for class object not found! OutOfMemory!!!");

    Object Length = GetClass(Template)->File.Fields->Count;

    GUID Identifier = Definition->Consciousness.AddObject(Template,Length);

    for (int i = 0; i < Length; ++i) {

        this->SetArrayValue(Identifier,i,this->Add(0));

    }

    return Identifier;
}

Experience* Experience::Initialize() {
    this->Full = false;
    this->State = State::Free;
    this->Type = Type::TMatter;
    return this;
}

Experience* Experience::Initialize(enum Type Type) {
    this->Full = false;
    this->State = State::Used;
    this->Type = Type;
    switch (Type) {
        case TMatter:
            this->Matter.Initialize();
            break;
        case TConsciousness:
            this->Consciousness.Initialize();
            break;
    }
    return this;
}

Experience* Experience::Check() {
    if (this->Type == Type::TMatter) {
        if (this->Matter.Check()) {
            return this;
        }
        else {
            this->Full = true;
            return nullptr;
        }
    } else {
        if (this->Consciousness.Check()) {
            return this;
        }
        else {
            this->Full = true;
            return nullptr;
        }
    }
}

Experience* Experience::Check(Object Length) {
    if (this->Type == Type::TMatter) {
        if (this->Matter.Check(Length)) {
            return this;
        }
        else {
            return nullptr;
        }
    } else {
        if (this->Consciousness.Check(Length)) {
            return this;
        }
        else {
            return nullptr;
        }
    }
}

bool Consciousness::Check() {
    if (this->Allocated + 64 > MB - 50000) {
        return false;
    }
    else {
        return true;
    }
}

bool Consciousness::Check(Object Length) {
    if (this->Allocated + Length > MB - 50000) {
        return false;
    }
    else {
        return true;
    }
}

GUID Consciousness::Add(Location Position) {
    this->Count++;
    Representation* LastRepresentation = GetLast();
    GUID Address = LastRepresentation->Initialize(Position);
    this->Allocated+=Position.Size+sizeof(Representation);
    return Address;
}

Representation *Consciousness::Get(Object Index) {
    Object Pointer = reinterpret_cast<Object>(this->Reflections);

    for (int i = 0; i < Index; ++i) {

        Object Size = ((Representation*)Pointer)->Size;

        assert(Size > MB,"[Reflector] Heap corruption!");

        Pointer+=Size;
    }

    return reinterpret_cast<Representation *>(Pointer);
}

Representation *Consciousness::GetLast() {

    Object Pointer = (Object)&this->Reflections;

    for (size_t i = 1; i < this->Count; ++i) {
        Object Size = ((Representation*)Pointer)->Size;

        assert(Size > MB,"[Reflector] Heap corruption!");

        Pointer+=Size;
    }

    return reinterpret_cast<Representation *>(Pointer);
}

void Consciousness::Initialize() {
    this->Allocated = 16;
    this->Count = 0;
}

GUID Consciousness::AddPacked(Packed Position) {
    this->Count++;
    Representation* LastRepresentation = GetLast();
    GUID Address = LastRepresentation->Initialize(Position);
    this->Allocated+=Position.Size+sizeof(Representation);
    return Address;
}

Location Consciousness::Get(GUID Value) {

    for (int i = 0; i < this->Count; ++i) {
        if (memcmp(Get(i)->Identifier.Internal,Value.Internal,sizeof(GUID)) == 0) {

            return *Get(i)->GetLocation();

        }
    }
    return Location{{sizeof(Location),PropertyType::TLocation},BadPointer,BadPointer};
}

Packed* Consciousness::GetPacked(GUID Value) {

    for (size_t i = 0; i < this->Count; ++i) {

        if (memcmp(Get(i)->Identifier.Internal,Value.Internal,40) == 0) {

            return Get(i)->GetPacked();

        }
    }
    return nullptr;
}

GUID Consciousness::AddArray(Object Length) {
    this->Count++;
    Parts Parts{};
    Parts.Count = Length;
    Parts.Type = TArray;
    this->Allocated+=sizeof(Parts)+(sizeof(GUID)*Length)+sizeof(Representation);
    GUID Data = GetLast()->Initialize(Parts);
    return Data;
}

Representation *Consciousness::GetArray(GUID Array) {

    for (int i = 0; i < this->Count; ++i) {

        if (memcmp(&Get(i)->Identifier,&Array,sizeof(GUID)) == 0) {
            return Get(i);
        }

    }

    return nullptr;

}

GUID Consciousness::AddClass(ClassFile File) {
    this->Count++;
    Class Klass{};
    Klass.File = File;
    Klass.Type = TPTClass;
    GUID Data = GetLast()->Initialize(Klass);
    this->Allocated+=sizeof(Class)+(sizeof(GUID))+sizeof(Representation);
    return Data;
}

Class *Consciousness::GetClass(GUID Address) {
    for (int i = 0; i < this->Count; ++i) {
        if (memcmp(Get(i)->Identifier.Internal,Address.Internal,sizeof(GUID)) == 0) {
            if (Get(i)->GetClass() != nullptr) {
                return Get(i)->GetClass();
            }
        }
    }
    return nullptr;
}

Class *Consciousness::GetClass(const char *Name) {

    Object BufferClassIndex = -1;
    Object BufferClassNameIndex = -1;
    char* BufferClassName = nullptr;
    Class* BufferClass = nullptr;

    for (int i = 0; i < this->Count; ++i) {
        if (Get(i)->GetClass() != nullptr) {

            BufferClass = Get(i)->GetClass();

            BufferClassIndex = Get(i)->GetClass()->File.This;
            BufferClassNameIndex = Get(i)->GetClass()->File.Pool->Elements[BufferClassIndex].Class.Name;
            BufferClassName = Get(i)->GetClass()->File.Pool->Elements[BufferClassNameIndex].UTF.Bytes;

            if (strcmp(Name,BufferClassName) == 0) {

                return Get(i)->GetClass();

            }
        }
    }
    return nullptr;
}

GUID Consciousness::AddObject(GUID Template, Object Length) {
    this->Count++;
    Parts Parts{};
    Parts.Count = Length;
    Parts.Type = PropertyType::TArray;
    ObjectType Type{};
    Type.Class = Template;
    Type.Type = PropertyType::TCObject;
    GUID Data = GetLast()->Initialize(Type,Parts);
    this->Allocated+=sizeof(Parts)+(sizeof(GUID)*Length)+sizeof(Representation);
    return Data;
}

GUID Consciousness::GetClassReference(const char *Name) {
    Object BufferClassIndex = -1;
    Object BufferClassNameIndex = -1;
    char* BufferClassName = nullptr;

    for (int i = 0; i < this->Count; ++i) {
        if (Get(i)->GetClass() != nullptr) {

            BufferClassIndex = Get(i)->GetClass()->File.This;
            BufferClassNameIndex = Get(i)->GetClass()->File.Pool->Elements[BufferClassIndex].Class.Name;
            BufferClassName = Get(i)->GetClass()->File.Pool->Elements[BufferClassNameIndex].UTF.Bytes;

            if (BufferClassName != nullptr) {
                if (strcmp(Name,BufferClassName) == 0) {
                    return Get(i)->Identifier;
                }
            }
        }
    }
    assert(true, "[Reflector] Failed to find class reference!!!");
    return GenerateNullGUID1();
}

bool Matter::Check() {
    for (int i = 0; i < 115000; ++i) {
        if (Field[i] == Free) {
            return true;
        }
    }
    return false;
}

bool Matter::Check(Object Length) {

    Object Free = 0;

    for (int i = 0; i < 115000; ++i) {
        if (Field[i] == State::Free) {
            for (int j = i; j < 115000; ++j) {
                if (Field[j] == State::Free) {
                    Free++;
                }
                else {
                    break;
                }
                if (Free == Length) {
                    return true;
                }
            }
            Free = 0;
        }
    }

    return false;
}

Location Matter::Add(Object Value) {
    for (size_t i = 0; i < 115000; ++i) {
        if (Field[i] == Free) {
            Field[i] = Used;
            Objects[i].Object = Value;
            return Location{sizeof (Location),PropertyType::TLocation,0,i};
        }
    }
    return Location{sizeof (Location),PropertyType::TLocation,BadPointer,BadPointer};
}

Packed Matter::AddPacked(size_t BlockCount) {

    Object Free = 0;

    for (size_t i = 0; i < 115000; ++i) {
        if (Field[i] == State::Free) {
            for (size_t j = i; j < 115000; ++j) {
                if (Field[j] == State::Free) {
                    Free++;
                }
                else {

                    break;

                }
                if (Free == BlockCount) {

                    if (i != 0) {
                        Field[i-1] = Used;
                    }

                    for (size_t k = 0; k < BlockCount; ++k) {
                        Field[i+k] = Used;
                    }

                    // FIXME
                    return Packed{{sizeof(Packed),PropertyType::TPacked},0,i,BlockCount+10};

                }
            }
            Free = 0;
        }
    }

    assert(true,"[Reflector] Failed to allocate Packed!");

    return Packed{sizeof(Packed),PropertyType::TPacked,BadPointer,BadPointer,BadPointer};

}

void Matter::Initialize() {
    for (size_t i = 0; i < 115000; ++i) {
        this->Field[i] = Free;
    }
}

Object Matter::Get(Location Value) {
    return this->Objects[Value.Field].Object;
}

void *Matter::GetPacked(Location Value) {
    return &this->Objects[Value.Field];
}

void Matter::Set(Location Position, Object Value) {
    this->Objects[Position.Field].Object = Value;
}

void Matter::Set(Location Position, float Value) {
    this->Objects[Position.Field].Float = Value;
}

void Matter::Set(Location Position, double Value) {
    this->Objects[Position.Field].Double = Value;
}

float Matter::GetF(Location Value) {
    return this->Objects[Value.Field].Float;
}

double Matter::GetD(Location Value) {
    return this->Objects[Value.Field].Double;
}

int Matter::GetI(Location Value) {
    return this->Objects[Value.Field].Integer;
}

long Matter::GetL(Location Value) {
    return this->Objects[Value.Field].Long;
}

void Matter::Set(Location Position, long Value) {
    this->Objects[Position.Field].Long = Value;
}

void Matter::Set(Location Position, int Value) {
    this->Objects[Position.Field].Integer = Value;
}

Location Matter::Add(long Value) {
    for (size_t i = 0; i < 115000; ++i) {
        if (Field[i] == Free) {
            Field[i] = Used;
            Objects[i].Long = Value;
            return Location{sizeof (Location),PropertyType::TLocation,0,i};
        }
    }
    return Location{sizeof (Location),PropertyType::TLocation,BadPointer,BadPointer};
}

Location Matter::Add(int Value) {
    for (size_t i = 0; i < 115000; ++i) {
        if (Field[i] == Free) {
            Field[i] = Used;
            Objects[i].Integer = Value;
            return Location{sizeof (Location),PropertyType::TLocation,0,i};
        }
    }
    return Location{sizeof (Location),PropertyType::TLocation,BadPointer,BadPointer};
}

Location Matter::Add(double Value) {
    for (size_t i = 0; i < 115000; ++i) {
        if (Field[i] == Free) {
            Field[i] = Used;
            Objects[i].Double = Value;
            return Location{sizeof (Location),PropertyType::TLocation,0,i};
        }
    }
    return Location{sizeof (Location),PropertyType::TLocation,BadPointer,BadPointer};
}

Location Matter::Add(float Value) {
    for (size_t i = 0; i < 115000; ++i) {
        if (Field[i] == Free) {
            Field[i] = Used;
            Objects[i].Float = Value;
            return Location{sizeof (Location),PropertyType::TLocation,0,i};
        }
    }
    return Location{sizeof (Location),PropertyType::TLocation,BadPointer,BadPointer};
}

GUID Representation::Initialize(Location Position) {
    this->Size = sizeof(Representation)+sizeof(Location);
    GenerateGUID(&this->Identifier);
    this->Values.Count++;
    memcpy(this->Values.Values,&Position,sizeof(Location));
    return this->Identifier;
}

GUID Representation::Initialize(Packed Position) {
    // STACK???
    // FIXME
    this->Size = sizeof(Representation)+sizeof(Packed);
    GenerateGUID(&this->Identifier);
    this->Values.Count++;
    memcpy(this->Values.Values,&Position,sizeof(Packed));
    return this->Identifier;
}

GUID Representation::Initialize(Parts PartsSize) {
    // FIXME
    this->Size = sizeof(Representation)+sizeof(Parts)+PartsSize.Size+(PartsSize.Count*sizeof(GUID));
    GenerateGUID(&this->Identifier);
    this->Values.Count++;
    memcpy(this->Values.Values,&PartsSize,sizeof(Parts));
    return this->Identifier;
}

Location *Representation::GetLocation() {
    for (int i = 0; i < this->Values.Count; ++i) {

        Property* Data = this->Values.Get(i);

        if (Data->Type == PropertyType::TLocation) {
            return (Location*)Data;
        }

    }
    return nullptr;
}

Packed *Representation::GetPacked() {
    for (int i = 0; i < this->Values.Count; ++i) {

        Property* Data = this->Values.Get(i);

        if (Data->Type == PropertyType::TPacked) {
            return (Packed*)Data;
        }

    }
    return nullptr;
}

Parts *Representation::GetParts() {
    for (int i = 0; i < this->Values.Count; ++i) {

        Property* Data = this->Values.Get(i);

        if (Data->Type == PropertyType::TArray) {
            return (Parts*)Data;
        }

    }
    return nullptr;
}

GUID Representation::Initialize(Class Class) {
    this->Size = sizeof(Representation)+sizeof(Class)+100;
    GenerateGUID(&this->Identifier);
    this->Values.Count++;
    Class.Size = sizeof(Class);
    memcpy(this->Values.Values,&Class,sizeof(Class));
    return this->Identifier;
}

GUID Representation::Initialize(ObjectType Type, Parts Members) {
    this->Size = sizeof(Representation)+sizeof(ObjectType)+sizeof(Parts)+(sizeof(GUID)*Members.Count);
    GenerateGUID(&this->Identifier);
    this->Values.Count++;
    Type.Size = sizeof(ObjectType);
    memcpy(this->Values.Values,&Type,sizeof(ObjectType));
    memcpy(((char*)this->Values.Values)+sizeof(Type),&Members, sizeof(Parts)+(sizeof(GUID)*Members.Count));
    this->Values.Count++;
    return this->Identifier;
}

Class *Representation::GetClass() {
    for (int i = 0; i < this->Values.Count; ++i) {

        Property* Data = this->Values.Get(i);

        if (Data->Type == PropertyType::TPTClass) {
            return (Class*)Data;
        }

    }
    return nullptr;
}

Property* Properties::Get(Object Index) {

    Object Pointer = (Object)&this->Values[0];

    for (size_t i = 1; i < this->Count; ++i) {

        Object Size = ((Property*)Pointer)->Size;

        assert(Size > MB,"[Reflector] Heap corruption!");

        Pointer+=Size;

    }

    return reinterpret_cast<Property *>(Pointer);

}
