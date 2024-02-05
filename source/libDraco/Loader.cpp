#include "Loader.h"

extern World* Reflector;

ClassFile Draco::Load(void *Binary) {
    Reader.Initialize(Binary);
    ClassFile File{};

    Buffer = &File;

    File.Magic = Reader.GetI();
    File.Minor = Reader.GetS();
    File.Major = Reader.GetS();
    File.Pool = LoadPool();
    //Reader.RawSet(&File.Flags,2);
    File.Flags = static_cast<OFlag>(Reader.GetS());
    File.This = Reader.GetS();
    File.Super = Reader.GetS();
    printf("[Draco] Loading interfaces!\n");
    printf("[Draco] Interface count: ");
    File.Interfaces = LoadInterfaces();
    printf("[Draco] Interfaces loaded!\n");
    printf("[Draco] Loading fields!\n");
    printf("[Draco] Fields count: ");
    File.Fields = LoadFields();
    printf("[Draco] Fields loaded!\n");
    printf("[Draco] Loading methods!\n");
    printf("[Draco] Methods count: ");
    File.Methods = LoadMethods();
    printf("[Draco] Methods loaded!\n");
    printf("[Draco] Loading attributes!\n");
    printf("[Draco] Attributes count: ");
    File.Attributes = LoadAttributes();
    printf("[Draco] Attributes loaded!\n");

    Buffer = nullptr;

    return File;
}

ConstantPool *Draco::LoadPool() {
    u2 Count = Reader.GetS();
    // STACK???
    // FIXME!!!
    auto* Pool = static_cast<ConstantPool *>(Reflector->GetPacked(
            Reflector->AddPacked(sizeof(ConstantPool)+((Count+3)*sizeof(PElement)))));
    Pool->Count = Count;
    for (int Counter = 1; Counter < Pool->Count; ++Counter) {
        PElement Element{};
        Element.Tag = static_cast<OType>(Reader.GetB());
        switch (Element.Tag) {
            default:
                Counter--;
                break;
            case TClass:
                Element.Class.Name = Reader.GetS();
                Pool->Elements[Counter] = Element;
                break;
            case TField:
                Element.Field.Class = Reader.GetS();
                Element.Field.NAT = Reader.GetS();
                Pool->Elements[Counter] = Element;
                break;
            case TMethod:
                Element.Method.Class = Reader.GetS();
                Element.Method.NAT = Reader.GetS();
                Pool->Elements[Counter] = Element;
                break;
            case TInterfaceMethod:
                Element.InterfaceMethod.Class = Reader.GetS();
                Element.InterfaceMethod.NAT = Reader.GetS();
                Pool->Elements[Counter] = Element;
                break;
            case TString:
                Element.String.UTFIndex = Reader.GetS();
                Pool->Elements[Counter] = Element;
                break;
            case TInteger:
                Element.Integer.Bytes = Reader.GetI();
                Element.Integer.Data = Reflector->Add((int)Element.Integer.Bytes);
                Pool->Elements[Counter] = Element;
                break;
            case TFloat:
                Element.Float.Bytes = Reader.GetI();
                Element.Float.Data = Reflector->Add((float)Element.Integer.Bytes);
                Pool->Elements[Counter] = Element;
                break;
            case TLong:
                Element.Long.High = Reader.GetI();
                Element.Long.Low = Reader.GetI();
                Element.Long.Data = Reflector->Add((long)(Element.Long.Low | Element.Long.High));
                Pool->Elements[Counter] = Element;
                break;
            case TDouble:
                Element.Double.High = Reader.GetI();
                Element.Double.Low = Reader.GetI();
                Element.Double.Data = Reflector->Add((double)(Element.Double.Low | Element.Double.High));
                Pool->Elements[Counter] = Element;
                break;
            case TNAT:
                Element.NAT.Name = Reader.GetS();
                Element.NAT.Descriptor = Reader.GetS();
                Pool->Elements[Counter] = Element;
                break;
            case TUTF:

                // FIXME
                // TODO FIX KERNEL COMPILES ONLY WITH -O3 WTF

                Object StringIntegerLength = Reader.GetS();

                Element.UTF.Length = StringIntegerLength; // FIXME

                GUID PackedString = Reflector->AddPacked(Element.UTF.Length+10);

                char* Bytes = static_cast<char *>(Reflector->GetPacked(PackedString));

                for (int j = 0; j < Element.UTF.Length; ++j) {
                    Bytes[j] = Reader.GetB();
                }

                Bytes[Element.UTF.Length] = 0;

                if (strlen(Bytes) == 0) {
                    printf("[Draco] Failure! Data: ");
                    printi(StringIntegerLength);
                    printf("\n");
                    assert(true,"Ultimate failure!");
                }

                printf("[Draco] String table: ");
                printf(Bytes);
                printf("\n");

                Element.UTF.Bytes = Bytes;

                Object StringLength = strlen(Element.UTF.Bytes);

                /*

                GUID String = Reflector->AddArray(StringLength);

                for (size_t i = 0; i < StringLength; ++i) {
                    // FIXME
                    // BAD PERFORMANCE!!!
                    Reflector->SetArrayValue(String,i,Reflector->Add(Element.UTF.Bytes[i]));
                }

                 */

                //Element.UTF.Address = String;

                Pool->Elements[Counter] = Element;

                break;

        }
    }
    return Pool;
}

Interfaces *Draco::LoadInterfaces() {

    u2 Count = Reader.GetS();

    printi(Count);
    printf("\n");

    auto* Interfaces = static_cast<struct Interfaces *>(Reflector->GetPacked(
            Reflector->AddPacked(sizeof(struct Interfaces) + (sizeof(u2) * Count))));
    Interfaces->Count = Count;
    for (int i = 0; i < Count; ++i) {
        Interfaces->Values[i] = Reader.GetS();
    }
    return Interfaces;
}

Fields *Draco::LoadFields() {
    u2 Count = Reader.GetS();

    printi(Count);
    printf("\n");

    auto* Fields = static_cast<struct Fields *>(Reflector->GetPacked(
            Reflector->AddPacked(sizeof(struct Interfaces) + (sizeof(Field) * Count))));
    Fields->Count = Count;
    for (int i = 0; i < Count; ++i) {
        Fields->Values[i].Flag = static_cast<OField>(Reader.GetS());
        Fields->Values[i].Data.Name = Reader.GetS();
        Fields->Values[i].Data.Descriptor = Reader.GetS();
        Fields->Values[i].Attributes = LoadAttributes();
        // FIXME
        if (Fields->Values[i].Flag == OTStatic) {
            Fields->Values[i].Static = Reflector->Add(0);
        }
    }
    return Fields;
}

Attributes *Draco::LoadAttributes() {
    u2 Count = Reader.GetS();

    auto* Attrs = static_cast<struct Attributes *>(Reflector->GetPacked(
            Reflector->AddPacked(sizeof(struct Attributes) + (sizeof(Attribute) * Count))));
    Attrs->Count = Count;
    for (int i = 0; i < Count; ++i) {

        Attribute Attr{};
        Attr.Name = Reader.GetS();
        Attr.Length = Reader.GetI();

        char* CPName = Buffer->Pool->Elements[Attr.Name].UTF.Bytes;

        if (strcmp(CPName,"SourceFile") == 0) {
            Attr.SourceFile.Index = Reader.GetS();
        }
        else if (strcmp(CPName,"ConstantValue") == 0) {
            Attr.ConstantValue.Index = Reader.GetS();
        }
        else if (strcmp(CPName,"Code") == 0) {
            Attr.Code.Stack = Reader.GetS();
            Attr.Code.Locals = Reader.GetS();
            Attr.Code.Length = Reader.GetI();

            Attr.Code.Code = static_cast<u1 *>(Reflector->GetPacked(Reflector->AddPacked(Attr.Code.Length)));
            for (int j = 0; j < Attr.Code.Length; ++j) {
                Attr.Code.Code[j] = Reader.GetB();
            }

            Attr.Code.ExceptionTableLength = Reader.GetS();

            Attr.Code.ETable = static_cast<AExceptionTable *>(Reflector->GetPacked(
                    Reflector->AddPacked(sizeof(AExceptionTable) * Attr.Code.ExceptionTableLength)));
            for (int j = 0; j < Attr.Code.ExceptionTableLength; ++j) {
                Attr.Code.ETable[j].StartPC = Reader.GetS();
                Attr.Code.ETable[j].EndPC = Reader.GetS();
                Attr.Code.ETable[j].HandlerPC = Reader.GetS();
                Attr.Code.ETable[j].CatchType = Reader.GetS();
            }

            Attr.Code.Values = LoadAttributes();
        }
        else if (strcmp(CPName,"LineNumberTable") == 0) {
            u2 Length = Reader.GetS();
            auto* Table = static_cast<ALineNumberTable *>(Reflector->GetPacked(
                    Reflector->AddPacked(sizeof(ALineNumberTable) + (sizeof(LineNumberTable) * Count))));

            Table->Length = Length;

            for (int j = 0; j < Table->Length; ++j) {
                Table->Table[j].StartPC = Reader.GetS();
                Table->Table[j].LineNumber = Reader.GetS();
            }

            Attr.LNTable = Table;
        }
        else if (strcmp(CPName,"Exceptions") == 0) {
            u2 Length = Reader.GetS();
            auto* Exceptions = static_cast<AExceptions *>(Reflector->GetPacked(
                    Reflector->AddPacked(sizeof(AExceptions) + (sizeof(u2) * Count))));

            Exceptions->Number = Length;

            for (int j = 0; j < Exceptions->Number; ++j) {
                Exceptions->ExceptionTablesIndexes[j] = Reader.GetS();
            }

            Attr.Exceptions = Exceptions;
        }
        else if (strcmp(CPName,"LocalVariableTable") == 0) {
            u2 Length = Reader.GetS();
            auto* Table = static_cast<ALocalVariableTable *>(Reflector->GetPacked(
                    Reflector->AddPacked(sizeof(ALocalVariableTable) + (sizeof(LocalVariableTable) * Count))));

            Table->Length = Length;

            for (int j = 0; j < Table->Length; ++j) {
                Table->Table[j].StartPC = Reader.GetS();
                Table->Table[j].Length = Reader.GetS();
                Table->Table[j].Name = Reader.GetS();
                Table->Table[j].Descriptor = Reader.GetS();
                Table->Table[j].Index = Reader.GetS();
            }

            Attr.LVTable = Table;
        }
        Attrs->Values[i] = Attr;
    }
    return Attrs;
}

Methods *Draco::LoadMethods() {

    u2 Count = Reader.GetS();

    printi(Count);
    printf("\n");

    auto* Methods = static_cast<struct Methods *>(Reflector->GetPacked(
            Reflector->AddPacked(sizeof(struct Interfaces) + (sizeof(Field) * Count))));
    Methods->Count = Count;
    for (int i = 0; i < Count; ++i) {
        Reader.RawSet(&Methods->Values[i].Flag,2);
        //Methods->Values[i].Flag = static_cast<OMethod>(Reader.GetS());
        Methods->Values[i].Data.Name = Reader.GetS();
        Methods->Values[i].Data.Descriptor = Reader.GetS();
        Methods->Values[i].Attributes = LoadAttributes();
    }
    return Methods;
}
