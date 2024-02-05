#include "Engine.h"
#include "../libReflector/Extension/Class.h"
#include "../libc/math.hpp"
#include "../libk/UUID.h"
#include "Native.h"

// TODO NOT FORGET ABOUT PLATFORM-DEPENDENT FLOATING POINT VALUES
#define NAN 0x7F800001
#define INF 0x7F800000

extern World* Reflector;
// TODO WRITE ENGINE

void Stack::Initialize(u4 Length) {
    this->AllocatedCount = Length;
    this->Count = 0;
    this->Values = Reflector->AddArray(this->AllocatedCount);
}

void Stack::Push(GUID Value) {
    assert(this->Count+1 >= AllocatedCount,"[Executive] [Reflector] StackOverflow!");
    Reflector->SetArrayValue(this->Values,this->Count,Value);
    this->Count++;
}

void Stack::Pop() {
    this->Count--;
}

GUID Stack::Top() const {
    return Reflector->GetArrayValue(this->Values,this->Count-1);
}

GUID Stack::TopAndPop() {
    GUID Reference = Top();
    Pop();
    return Reference;
}

void Locals::Initialize(u4 Length) {
    this->Count = Length;
    this->Values = Reflector->AddArray(this->Count+10);
}

GUID Locals::Get(u4 Index) const {
    return Reflector->GetArrayValue(this->Values,Index);
}

void Locals::Set(u4 Index, GUID Value) const {
    Reflector->SetArrayValue(this->Values,Index,Value);
}

// TODO DEBUG EXTENSIONS, GLOBAL STATE VARIABLE, STACK TRACE
// TODO ABI, RETURNADDRESS AND IP AND CPool IS GUID THAT LOADED IN STACK!!!

void Executive::Execute(ACode *Code, Object IP, Stack *SP, Locals *Registers,  Class* ClassReference) {

    for (; IP < Code->Length; IP++) {

        /*

        printf("[Executive] Emittion opcode: ");
        printi(Code->Code[IP]);
        printf("\n");

        */

        switch (Code->Code[IP]) {

            case op_sipush: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = (FirstIndexByte << 8) | SecondIndexByte;

                GUID Value = Reflector->Add(IndexByte);

                SP->Push(Value);

                break;
            }

            case op_bipush: {
                char Byte = (char)Code->Code[++IP];

                GUID Value = Reflector->Add(Byte);

                SP->Push(Value);

                break;
            }

            case op_aaload: {
                GUID ArrayReference = SP->TopAndPop();
                GUID ArrayElementIndex = SP->TopAndPop();

                Object NumericalArrayElementIndex = Reflector->Get(ArrayElementIndex);

                GUID Object = Reflector->GetArrayValue(ArrayReference,NumericalArrayElementIndex);

                SP->Push(Object);

                break;
            }

            case op_aastore: {
                GUID ArrayReference = SP->TopAndPop();
                GUID ArrayElementIndex = SP->TopAndPop();
                GUID ArrayElementNewItem = SP->TopAndPop();

                Object NumericalArrayElementIndex = Reflector->Get(ArrayElementIndex);

                Reflector->SetArrayValue(ArrayReference,NumericalArrayElementIndex,ArrayElementNewItem);

                break;
            }

            case op_swap: {
                GUID First = SP->TopAndPop();
                GUID Second = SP->TopAndPop();

                SP->Push(Second);
                SP->Push(First);

                break;
            }

            case op_wide: {
                assert(true, "[Executive] Wide opcode not implemented yet!");

                break;
            }

            case op_tableswitch: {
                assert(true, "[Executive] Switch not implemented yet!");

                break;
            }

            case op_aconst_null: {
                SP->Push(GenerateNullGUID());

                break;
            }

            case op_aload: case op_dload: case op_fload: case op_iload: case op_lload: {
                Object Index = Code->Code[++IP];

                SP->Push(Registers->Get(Index));

                break;
            }

            case op_aload_0: case op_dload_0: case op_fload_0: case op_iload_0: case op_lload_0: {
                SP->Push(Registers->Get(0));

                break;
            }

            case op_aload_1: case op_dload_1: case op_fload_1: case op_iload_1: case op_lload_1: {
                SP->Push(Registers->Get(1));

                break;
            }

            case op_aload_2: case op_dload_2: case op_fload_2: case op_iload_2: case op_lload_2: {
                SP->Push(Registers->Get(2));

                break;
            }

            case op_aload_3: case op_dload_3: case op_fload_3: case op_iload_3: case op_lload_3: {
                SP->Push(Registers->Get(3));

                break;
            }

            case op_anewarray: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = (FirstIndexByte << 8) | SecondIndexByte;

                GUID ArrayLength = SP->TopAndPop();

                Object NumericalArrayLength = Reflector->Get(ArrayLength);

                SP->Push(Reflector->AddArray(NumericalArrayLength));
            }

            case op_return: {

                GUID LocalsPointer = SP->TopAndPop();
                GUID StackPointer = SP->TopAndPop();
                GUID ClassPointer = SP->TopAndPop();
                GUID InstructionPointer = SP->TopAndPop();
                GUID ReturnAddress = SP->TopAndPop();

                Object NumericalReturnAddress = Reflector->Get(ReturnAddress);
                Object NumericalInstructionPointer = Reflector->Get(InstructionPointer);
                Object NumericalClassReference = Reflector->Get(ClassPointer);
                Object NumericalStackPointer = Reflector->Get(StackPointer);
                Object NumericalLocalsPointer = Reflector->Get(LocalsPointer);

                IP = NumericalInstructionPointer;
                ClassReference = reinterpret_cast<Class *>(NumericalClassReference);
                Code = reinterpret_cast<ACode *>(NumericalReturnAddress);
                SP = reinterpret_cast<Stack *>(NumericalStackPointer);
                Registers = reinterpret_cast<Locals *>(NumericalLocalsPointer);

                break;
            }

            case op_areturn: case op_dreturn: case op_freturn: case op_ireturn: case op_lreturn: {

                GUID FunctionResult = SP->TopAndPop();
                GUID LocalsPointer = SP->TopAndPop();
                GUID StackPointer = SP->TopAndPop();
                GUID ClassPointer = SP->TopAndPop();
                GUID InstructionPointer = SP->TopAndPop();
                GUID ReturnAddress = SP->TopAndPop();

                Object NumericalReturnAddress = Reflector->Get(ReturnAddress);
                Object NumericalInstructionPointer = Reflector->Get(InstructionPointer);
                Object NumericalClassReference = Reflector->Get(ClassPointer);
                Object NumericalStackPointer = Reflector->Get(StackPointer);
                Object NumericalLocalsPointer = Reflector->Get(LocalsPointer);

                IP = NumericalInstructionPointer;
                ClassReference = reinterpret_cast<Class *>(NumericalClassReference);
                Code = reinterpret_cast<ACode *>(NumericalReturnAddress);
                SP = reinterpret_cast<Stack *>(NumericalStackPointer);
                Registers = reinterpret_cast<Locals *>(NumericalLocalsPointer);

                SP->Push(FunctionResult);

                break;
            }

            case op_arraylength: {
                GUID ArrayReference = SP->TopAndPop();

                Object Length = Reflector->GetArrayLength(ArrayReference);

                SP->Push(Reflector->Add(Length));

                break;
            }

            case op_astore: case op_dstore: case op_fstore: case op_istore: case op_lstore: {
                Object NumericalRegisterIndex = Code->Code[++IP];
                GUID ObjectReference = SP->TopAndPop();

                Registers->Set(NumericalRegisterIndex,ObjectReference);

                break;
            }

            case op_astore_0: case op_dstore_0: case op_fstore_0: case op_istore_0: case op_lstore_0: {
                GUID ObjectReference = SP->TopAndPop();

                Registers->Set(0,ObjectReference);

                break;
            }

            case op_astore_1: case op_dstore_1: case op_fstore_1: case op_istore_1: case op_lstore_1: {
                GUID ObjectReference = SP->TopAndPop();

                Registers->Set(1,ObjectReference);

                break;
            }

            case op_astore_2: case op_dstore_2: case op_fstore_2: case op_istore_2: case op_lstore_2: {
                GUID ObjectReference = SP->TopAndPop();

                Registers->Set(2,ObjectReference);

                break;
            }

            case op_astore_3: case op_dstore_3: case op_fstore_3: case op_istore_3: case op_lstore_3: {
                GUID ObjectReference = SP->TopAndPop();

                Registers->Set(3,ObjectReference);

                break;
            }
            // FIXME
            case op_athrow: {
                assert(true,"[Executive] Exceptions is not implemented yet!");

                break;
            }

            case op_saload: case op_baload: case op_caload: case op_daload: case op_faload: case op_iaload: case op_laload: {
                GUID ArrayReference = SP->TopAndPop();
                GUID ArrayElementIndexReference = SP->TopAndPop();

                Object ArrayElementIndex = Reflector->Get(ArrayElementIndexReference);

                SP->Push(Reflector->GetArrayValue(ArrayReference,ArrayElementIndex));

                break;
            }

            case op_sastore: case op_bastore: case op_castore: case op_dastore: case op_fastore: case op_iastore: case op_lastore: {
                GUID ArrayReference = SP->TopAndPop();
                GUID ArrayElementIndexReference = SP->TopAndPop();
                GUID ArrayElementValueReference = SP->TopAndPop();

                Object ArrayElementIndex = Reflector->Get(ArrayElementIndexReference);

                Reflector->SetArrayValue(ArrayReference,ArrayElementIndex,ArrayElementValueReference);

                break;
            }
            // FIXME
            // TODO EXPLICIT CONVERSION!!!
            case op_checkcast: {
                assert(true,"[Executive] Cast check is not implemented yet!");

                break;
            }

            case op_d2f: {
                GUID Double = SP->TopAndPop();

                double DoubleValue = Reflector->GetD(Double);

                float FloatValue = (float)DoubleValue;

                Reflector->Set(Double,FloatValue);

                SP->Push(Double);

                break;
            }

            case op_d2i: {
                GUID Double = SP->TopAndPop();

                double DoubleValue = Reflector->GetD(Double);

                int IntegerValue = (int)DoubleValue;

                Reflector->Set(Double,IntegerValue);

                SP->Push(Double);

                break;
            }

            case op_d2l: {
                GUID Double = SP->TopAndPop();

                double DoubleValue = Reflector->GetD(Double);

                long LongValue = (long)DoubleValue;

                Reflector->Set(Double,LongValue);

                SP->Push(Double);

                break;
            }

            case op_dadd: {
                GUID SecondDoubleReference = SP->TopAndPop();
                GUID FirstDoubleReference = SP->TopAndPop();

                double SecondValue = Reflector->GetD(SecondDoubleReference);
                double FirstValue = Reflector->GetD(FirstDoubleReference);

                double FinalValue = FirstValue+SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondDoubleReference,FinalValue);

                SP->Push(SecondDoubleReference);

                break;
            }
            // TODO IEE 754 NAN???
            case op_dcmpg: case op_dcmpl: {
                GUID SecondDoubleReference = SP->TopAndPop();
                GUID FirstDoubleReference = SP->TopAndPop();

                double SecondValue = Reflector->GetD(SecondDoubleReference);
                double FirstValue = Reflector->GetD(FirstDoubleReference);
                int Result;

                if (FirstValue > SecondValue) {
                    Result = 1;
                }
                else if (FirstValue == SecondValue) {
                    Result = 0;
                }
                else {
                    Result = -1;
                }

                SP->Push(Reflector->Add(Result));

                break;
            }

            case op_dconst_0: {
                double Constant = 0.0;

                SP->Push(Reflector->Add(Constant));

                break;
            }

            case op_dconst_1: {
                double Constant = 1.0;

                SP->Push(Reflector->Add(Constant));

                break;
            }

            case op_ddiv: {
                GUID SecondDoubleReference = SP->TopAndPop();
                GUID FirstDoubleReference = SP->TopAndPop();

                double SecondValue = Reflector->GetD(SecondDoubleReference);
                double FirstValue = Reflector->GetD(FirstDoubleReference);

                double FinalValue = FirstValue/SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondDoubleReference,FinalValue);

                SP->Push(SecondDoubleReference);

                break;
            }

            case op_dmul: {
                GUID SecondDoubleReference = SP->TopAndPop();
                GUID FirstDoubleReference = SP->TopAndPop();

                double SecondValue = Reflector->GetD(SecondDoubleReference);
                double FirstValue = Reflector->GetD(FirstDoubleReference);

                double FinalValue = FirstValue*SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondDoubleReference,FinalValue);

                SP->Push(SecondDoubleReference);

                break;
            }

            case op_dneg: {
                GUID DoubleReference = SP->TopAndPop();

                double Double = Reflector->GetD(DoubleReference);

                Double = -Double;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(DoubleReference,Double);

                SP->Push(DoubleReference);

                break;
            }
            // TODO REMOVE ALL LIBC FUNCTION INTO ANOTHER LIB
            case op_drem: {
                GUID SecondDoubleReference = SP->TopAndPop();
                GUID FirstDoubleReference = SP->TopAndPop();

                double SecondValue = Reflector->GetD(SecondDoubleReference);
                double FirstValue = Reflector->GetD(FirstDoubleReference);

                double FinalValue = fmod(FirstValue,SecondValue);

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondDoubleReference,FinalValue);

                SP->Push(SecondDoubleReference);

                break;
            }

            case op_dsub: {
                GUID SecondDoubleReference = SP->TopAndPop();
                GUID FirstDoubleReference = SP->TopAndPop();

                double SecondValue = Reflector->GetD(SecondDoubleReference);
                double FirstValue = Reflector->GetD(FirstDoubleReference);

                double FinalValue = FirstValue-SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondDoubleReference,FinalValue);

                SP->Push(SecondDoubleReference);

                break;
            }

            case op_dup: {
                SP->Push(SP->Top());

                break;
            }

            case op_dup_x1: {
                GUID FirstReference = SP->TopAndPop();
                GUID SecondReference = SP->TopAndPop();

                SP->Push(FirstReference);
                SP->Push(SecondReference);
                SP->Push(FirstReference);

                break;
            }

            case op_dup_x2: {
                GUID FirstReference = SP->TopAndPop();
                GUID SecondReference = SP->TopAndPop();
                GUID ThirdReference = SP->TopAndPop();

                SP->Push(FirstReference);
                SP->Push(ThirdReference);
                SP->Push(SecondReference);
                SP->Push(FirstReference);

                break;
            }

            case op_dup2: {
                GUID FirstReference = SP->TopAndPop();
                GUID SecondReference = SP->TopAndPop();

                SP->Push(FirstReference);
                SP->Push(SecondReference);
                SP->Push(FirstReference);
                SP->Push(SecondReference);

                break;
            }

            case op_dup2_x1: {
                GUID FirstReference = SP->TopAndPop();
                GUID SecondReference = SP->TopAndPop();
                GUID ThirdReference = SP->TopAndPop();

                SP->Push(SecondReference);
                SP->Push(FirstReference);
                SP->Push(ThirdReference);
                SP->Push(SecondReference);
                SP->Push(FirstReference);

                break;
            }

            case op_dup2_x2: {
                GUID FirstReference = SP->TopAndPop();
                GUID SecondReference = SP->TopAndPop();
                GUID ThirdReference = SP->TopAndPop();
                GUID FourthReference = SP->TopAndPop();

                SP->Push(SecondReference);
                SP->Push(FirstReference);
                SP->Push(FourthReference);
                SP->Push(ThirdReference);
                SP->Push(SecondReference);
                SP->Push(FirstReference);

                break;
            }

            case op_f2d: {
                GUID Float = SP->TopAndPop();

                float FloatValue = Reflector->GetF(Float);

                double DoubleValue = FloatValue;

                Reflector->Set(Float,DoubleValue);

                SP->Push(Float);

                break;
            }

            case op_f2i: {
                GUID Float = SP->TopAndPop();

                float FloatValue = Reflector->GetF(Float);

                int IntegerValue = (int)FloatValue;

                Reflector->Set(Float,IntegerValue);

                SP->Push(Float);

                break;
            }

            case op_f2l: {
                GUID Float = SP->TopAndPop();

                float FloatValue = Reflector->GetF(Float);

                long LongValue = (long)FloatValue;

                Reflector->Set(Float,LongValue);

                SP->Push(Float);

                break;
            }

            case op_fadd: {
                GUID SecondFloatReference = SP->TopAndPop();
                GUID FirstFloatReference = SP->TopAndPop();

                float SecondValue = Reflector->GetF(SecondFloatReference);
                float FirstValue = Reflector->GetF(FirstFloatReference);

                float FinalValue = FirstValue+SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondFloatReference,FinalValue);

                SP->Push(SecondFloatReference);

                break;
            }

            case op_lcmp: {
                GUID SecondDoubleReference = SP->TopAndPop();
                GUID FirstDoubleReference = SP->TopAndPop();

                long SecondValue = Reflector->GetL(SecondDoubleReference);
                long FirstValue = Reflector->GetL(FirstDoubleReference);
                int Result;

                if (FirstValue > SecondValue) {
                    Result = 1;
                }
                else if (FirstValue == SecondValue) {
                    Result = 0;
                }
                else {
                    Result = -1;
                }

                SP->Push(Reflector->Add(Result));

                break;
            }

            case op_fcmpg: case op_fcmpl: {
                GUID SecondDoubleReference = SP->TopAndPop();
                GUID FirstDoubleReference = SP->TopAndPop();

                float SecondValue = Reflector->GetF(SecondDoubleReference);
                float FirstValue = Reflector->GetF(FirstDoubleReference);
                int Result;

                if (FirstValue > SecondValue) {
                    Result = 1;
                }
                else if (FirstValue == SecondValue) {
                    Result = 0;
                }
                else {
                    Result = -1;
                }

                SP->Push(Reflector->Add(Result));

                break;
            }

            case op_fconst_0: {
                float Constant = 0.0;

                SP->Push(Reflector->Add(Constant));

                break;
            }

            case op_fconst_1: {
                float Constant = 1.0;

                SP->Push(Reflector->Add(Constant));

                break;
            }

            case op_fconst_2: {
                float Constant = 2.0;

                SP->Push(Reflector->Add(Constant));

                break;
            }

            case op_fdiv: {
                GUID SecondFloatReference = SP->TopAndPop();
                GUID FirstFloatReference = SP->TopAndPop();

                float SecondValue = Reflector->GetF(SecondFloatReference);
                float FirstValue = Reflector->GetF(FirstFloatReference);

                float FinalValue = FirstValue/SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondFloatReference,FinalValue);

                SP->Push(SecondFloatReference);

                break;
            }

            case op_fmul: {
                GUID SecondFloatReference = SP->TopAndPop();
                GUID FirstFloatReference = SP->TopAndPop();

                float SecondValue = Reflector->GetF(SecondFloatReference);
                float FirstValue = Reflector->GetF(FirstFloatReference);

                float FinalValue = FirstValue*SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondFloatReference,FinalValue);

                SP->Push(SecondFloatReference);

                break;
            }

            case op_lookupswitch: {
                // FIXME
                // TODO IMPLEMENT ME
                Object OffsetTemplate =  (4 - ((IP - Code->Length) % 4));

                Object Offset = IP+OffsetTemplate;

                assert(true, "[Executive] Switch not implemented yet!");

                break;
            }

            case op_lneg: {
                GUID LongReference = SP->TopAndPop();

                long Long = Reflector->GetL(LongReference);

                Long = -Long;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(LongReference,Long);

                SP->Push(LongReference);

                break;
            }

            case op_fneg: {
                GUID FloatReference = SP->TopAndPop();

                float Float = Reflector->GetF(FloatReference);

                Float = -Float;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(FloatReference,Float);

                SP->Push(FloatReference);

                break;
            }
                // TODO REMOVE ALL LIBC FUNCTION INTO ANOTHER LIB
            case op_frem: {
                GUID SecondFloatReference = SP->TopAndPop();
                GUID FirstFloatReference = SP->TopAndPop();

                float SecondValue = Reflector->GetF(SecondFloatReference);
                float FirstValue = Reflector->GetF(FirstFloatReference);

                float FinalValue = fmod(FirstValue,SecondValue);

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondFloatReference,FinalValue);

                SP->Push(SecondFloatReference);

                break;
            }

            case op_fsub: {
                GUID SecondFloatReference = SP->TopAndPop();
                GUID FirstFloatReference = SP->TopAndPop();

                float SecondValue = Reflector->GetF(SecondFloatReference);
                float FirstValue = Reflector->GetF(FirstFloatReference);

                float FinalValue = FirstValue-SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondFloatReference,FinalValue);

                SP->Push(SecondFloatReference);

                break;
            }

            case op_putfield: {
                bool Success = false;

                GUID Value = SP->TopAndPop();
                GUID ObjectReference = SP->TopAndPop();

                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                Object FieldClassIndex = ClassReference->File.Pool->Elements[IndexByte].Field.Class;
                Object FieldNATIndex = ClassReference->File.Pool->Elements[IndexByte].Field.NAT;
                Object FieldNameIndex = ClassReference->File.Pool->Elements[FieldNATIndex].NAT.Name;
                Object FieldSignatureIndex = ClassReference->File.Pool->Elements[FieldNATIndex].NAT.Descriptor;
                Object FieldClassNameIndex = ClassReference->File.Pool->Elements[FieldClassIndex].Class.Name;

                char* ClassName = ClassReference->File.Pool->Elements[FieldClassNameIndex].UTF.Bytes;
                char* FieldName = ClassReference->File.Pool->Elements[FieldNameIndex].UTF.Bytes;
                char* FieldDescriptor = ClassReference->File.Pool->Elements[FieldSignatureIndex].UTF.Bytes;

                Class* ReferenceClass = Reflector->GetClass(ClassName);
                Field* ReferenceField = ReferenceClass->GetField(FieldName,ClassName,FieldDescriptor);

                for (int i = 0; i < ReferenceClass->File.Fields->Count; ++i) {
                    if (&ReferenceClass->File.Fields->Values[i] == ReferenceField) {
                        Reflector->SetArrayValue(ObjectReference,i,Value);
                        Success = true;
                        break;
                    }
                }

                assert(!Success,"[Executive] [Reflector] Failed to getField from reference!");

                break;
            }

            case op_getfield: {
                bool Success = false;

                GUID ObjectReference = SP->TopAndPop();

                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                Object FieldClassIndex = ClassReference->File.Pool->Elements[IndexByte].Field.Class;
                Object FieldNATIndex = ClassReference->File.Pool->Elements[IndexByte].Field.NAT;
                Object FieldNameIndex = ClassReference->File.Pool->Elements[FieldNATIndex].NAT.Name;
                Object FieldSignatureIndex = ClassReference->File.Pool->Elements[FieldNATIndex].NAT.Descriptor;
                Object FieldClassNameIndex = ClassReference->File.Pool->Elements[FieldClassIndex].Class.Name;

                char* ClassName = ClassReference->File.Pool->Elements[FieldClassNameIndex].UTF.Bytes;
                char* FieldName = ClassReference->File.Pool->Elements[FieldNameIndex].UTF.Bytes;
                char* FieldDescriptor = ClassReference->File.Pool->Elements[FieldSignatureIndex].UTF.Bytes;

                Class* ReferenceClass = Reflector->GetClass(ClassName);
                Field* ReferenceField = ReferenceClass->GetField(FieldName,ClassName,FieldDescriptor);

                for (int i = 0; i < ReferenceClass->File.Fields->Count; ++i) {
                    if (&ReferenceClass->File.Fields->Values[i] == ReferenceField) {
                        SP->Push(Reflector->GetArrayValue(ObjectReference,i));
                        Success = true;
                        break;
                    }
                }

                assert(!Success,"[Executive] [Reflector] Failed to getField from reference!");

                break;
            }

            case op_putstatic: {
                bool Success = false;

                GUID Value = SP->TopAndPop();

                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                Object FieldClassIndex = ClassReference->File.Pool->Elements[IndexByte].Field.Class;
                Object FieldNATIndex = ClassReference->File.Pool->Elements[IndexByte].Field.NAT;
                Object FieldNameIndex = ClassReference->File.Pool->Elements[FieldNATIndex].NAT.Name;
                Object FieldSignatureIndex = ClassReference->File.Pool->Elements[FieldNATIndex].NAT.Descriptor;
                Object FieldClassNameIndex = ClassReference->File.Pool->Elements[FieldClassIndex].Class.Name;

                char* ClassName = ClassReference->File.Pool->Elements[FieldClassNameIndex].UTF.Bytes;
                char* FieldName = ClassReference->File.Pool->Elements[FieldNameIndex].UTF.Bytes;
                char* FieldDescriptor = ClassReference->File.Pool->Elements[FieldSignatureIndex].UTF.Bytes;

                Class* ReferenceClass = Reflector->GetClass(ClassName);
                Field* ReferenceField = ReferenceClass->GetField(FieldName,ClassName,FieldDescriptor);

                assert(ReferenceField == nullptr,"[Executive] [Reflector] Failed to getField from reference!");

                ReferenceField->Static = Value;

                break;
            }

            case op_getstatic: {
                bool Success = false;

                GUID ObjectReference = SP->TopAndPop();

                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                Object FieldClassIndex = ClassReference->File.Pool->Elements[IndexByte].Field.Class;
                Object FieldNATIndex = ClassReference->File.Pool->Elements[IndexByte].Field.NAT;
                Object FieldNameIndex = ClassReference->File.Pool->Elements[FieldNATIndex].NAT.Name;
                Object FieldSignatureIndex = ClassReference->File.Pool->Elements[FieldNATIndex].NAT.Descriptor;
                Object FieldClassNameIndex = ClassReference->File.Pool->Elements[FieldClassIndex].Class.Name;

                char* ClassName = ClassReference->File.Pool->Elements[FieldClassNameIndex].UTF.Bytes;
                char* FieldName = ClassReference->File.Pool->Elements[FieldNameIndex].UTF.Bytes;
                char* FieldDescriptor = ClassReference->File.Pool->Elements[FieldSignatureIndex].UTF.Bytes;

                Class* ReferenceClass = Reflector->GetClass(ClassName);
                Field* ReferenceField = ReferenceClass->GetField(FieldName,ClassName,FieldDescriptor);

                assert(ReferenceField == nullptr,"[Executive] [Reflector] Failed to getField from reference!");

                SP->Push(ReferenceField->Static);

                break;
            }

            case op_goto: {

                // FIXME

                u1 FirstIndexByte = Code->Code[++IP];
                u1 SecondIndexByte = Code->Code[++IP];

                u2 FinalPointer = (FirstIndexByte | SecondIndexByte) >> 3;
                // FIXME DO NORMAL CHECK, NOT MAGIC!!!
                IP = FinalPointer-2;

                break;
            }

            case op_jsr: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;
                // FIXME DO NORMAL CHECK, NOT MAGIC!!!
                SP->Push(Reflector->Add(IndexByte));

                IP = IndexByte-1;

                break;
            }

            case op_goto_w: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];
                Object ThirdIndexByte = Code->Code[++IP];
                Object FourthIndexByte = Code->Code[++IP];

                Object IndexByte = (FirstIndexByte) | (SecondIndexByte) | (ThirdIndexByte) | FourthIndexByte;

                IP = IndexByte-1;

                break;
            }

            case op_ret: {
                // TODO STRANGE WITH JSR
                Object RegisterIndex = Code->Code[++IP];

                GUID ReturnAddress = Registers->Get(RegisterIndex);

                Object NumericalReturnAddress = Reflector->Get(ReturnAddress);

                IP = NumericalReturnAddress;
                break;
            }

            case op_jsr_w: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];
                Object ThirdIndexByte = Code->Code[++IP];
                Object FourthIndexByte = Code->Code[++IP];

                Object IndexByte = (FirstIndexByte) | (SecondIndexByte) | (ThirdIndexByte) | FourthIndexByte;

                SP->Push(Reflector->Add(IndexByte));

                IP = IndexByte-1;

                break;
            }

            case op_i2b: {
                GUID Integer = SP->TopAndPop();

                int IntegerValue = Reflector->GetI(Integer);

                char ByteValue = (char)IntegerValue;

                Reflector->Set(Integer,ByteValue);

                SP->Push(Integer);

                break;
            }

            case op_i2c: case op_i2s: {
                GUID Integer = SP->TopAndPop();

                int IntegerValue = Reflector->GetI(Integer);

                wchar_t CharValue = (wchar_t)IntegerValue;

                Reflector->Set(Integer,CharValue);

                SP->Push(Integer);

                break;
            }

            case op_i2d: {
                GUID Integer = SP->TopAndPop();

                int IntegerValue = Reflector->GetI(Integer);

                double DoubleValue = (double)IntegerValue;

                Reflector->Set(Integer,DoubleValue);

                SP->Push(Integer);

                break;
            }

            case op_i2f: {
                GUID Integer = SP->TopAndPop();

                int IntegerValue = Reflector->GetI(Integer);

                float FloatValue = (float)IntegerValue;

                Reflector->Set(Integer,FloatValue);

                SP->Push(Integer);

                break;
            }

            case op_l2f: {
                GUID Long = SP->TopAndPop();

                long LongValue = Reflector->GetL(Long);

                float FloatValue = (float)LongValue;

                Reflector->Set(Long,FloatValue);

                SP->Push(Long);

                break;
            }

            case op_l2i: {
                GUID Long = SP->TopAndPop();

                long LongValue = Reflector->GetL(Long);

                int IntegerValue = (int)LongValue;

                Reflector->Set(Long,IntegerValue);

                SP->Push(Long);

                break;
            }

            case op_l2d: {
                GUID Long = SP->TopAndPop();

                long LongValue = Reflector->GetL(Long);

                double DoubleValue = (double)LongValue;

                Reflector->Set(Long,DoubleValue);

                SP->Push(Long);

                break;
            }

            case op_i2l: {
                GUID Integer = SP->TopAndPop();

                int IntegerValue = Reflector->GetI(Integer);

                long LongValue = (long)IntegerValue;

                Reflector->Set(Integer,LongValue);

                SP->Push(Integer);

                break;
            }

            case op_iadd: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                int FinalValue = FirstValue+SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_ladd: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                long SecondValue = Reflector->GetL(SecondValueReference);
                long FirstValue = Reflector->GetL(FirstValueReference);

                long FinalValue = FirstValue+SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_lsub: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                long SecondValue = Reflector->GetL(SecondValueReference);
                long FirstValue = Reflector->GetL(FirstValueReference);

                long FinalValue = FirstValue-SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_ldiv : {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                long SecondValue = Reflector->GetL(SecondValueReference);
                long FirstValue = Reflector->GetL(FirstValueReference);

                long FinalValue = FirstValue/SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_lconst_0: {
                SP->Push(Reflector->Add((long)0));

                break;
            }

            case op_ldc_w: case op_ldc2_w: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                switch (ClassReference->GetPE(IndexByte)->Tag) {

                    case TString:
                        SP->Push(ClassReference->GetPE(ClassReference->GetPE(IndexByte)->String.UTFIndex)->UTF.Address);
                        break;
                    case TInteger:
                        SP->Push(ClassReference->GetPE(IndexByte)->Integer.Data);
                        break;
                    case TFloat:
                        SP->Push(ClassReference->GetPE(IndexByte)->Float.Data);
                        break;
                        // FIXME
                    case TLong:
                        SP->Push(ClassReference->GetPE(IndexByte)->Long.Data);
                        break;
                        // FIXME
                    case TDouble:
                        SP->Push(ClassReference->GetPE(IndexByte)->Double.Data);
                        break;
                    default:
                        assert(true,"[Executive] [Reflector] Wrong constant pool element LDC parameter!!!");
                        break;
                }

                break;
            }

            case op_ldc: {
                Object IndexByte = Code->Code[++IP];

                switch (ClassReference->GetPE(IndexByte)->Tag) {

                    case TString:
                        SP->Push(ClassReference->GetPE(ClassReference->GetPE(IndexByte)->String.UTFIndex)->UTF.Address);
                        break;
                    case TInteger:
                        SP->Push(ClassReference->GetPE(IndexByte)->Integer.Data);
                        break;
                    case TFloat:
                        SP->Push(ClassReference->GetPE(IndexByte)->Float.Data);
                        break;
                    default:
                        assert(true,"[Executive] [Reflector] Wrong constant pool element LDC parameter!!!");
                        break;
                }

                break;
            }

            case op_lconst_1: {
                SP->Push(Reflector->Add((long)1));

                break;
            }

            case op_lrem: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                long SecondValue = Reflector->GetL(SecondValueReference);
                long FirstValue = Reflector->GetL(FirstValueReference);

                long FinalValue = FirstValue - (FirstValue/SecondValue) * SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_irem: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                int FinalValue = FirstValue - (FirstValue/SecondValue) * SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_ishl: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                FirstValue <<= SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FirstValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_lshl: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                long SecondValue = Reflector->GetL(SecondValueReference);
                long FirstValue = Reflector->GetL(FirstValueReference);

                FirstValue <<= SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FirstValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_monitorenter: {
                // TODO IMPLEMENT ME
                assert(true, "[Executive] Monitor enter not implemented yet!");

                break;
            }

            case op_monitorexit: {
                // TODO IMPLEMENT ME
                assert(true, "[Executive] Monitor exit not implemented yet!");

                break;
            }

            case op_lxor: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                long SecondValue = Reflector->GetL(SecondValueReference);
                long FirstValue = Reflector->GetL(FirstValueReference);

                FirstValue ^= SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FirstValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_ixor: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                FirstValue ^= SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FirstValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_ishr: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                FirstValue >>= SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FirstValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_lshr: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                long SecondValue = Reflector->GetL(SecondValueReference);
                long FirstValue = Reflector->GetL(FirstValueReference);

                FirstValue >>= SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FirstValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_iushr: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                // FIXME
                u4 FirstValue = Reflector->GetI(FirstValueReference);

                FirstValue >>= SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,(int)FirstValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_lushr: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetL(SecondValueReference);
                // FIXME
                u8 FirstValue = Reflector->GetL(FirstValueReference);

                FirstValue >>= SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,(int)FirstValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_isub: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                int FinalValue = FirstValue - SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_lor: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                long SecondValue = Reflector->GetL(SecondValueReference);
                long FirstValue = Reflector->GetL(FirstValueReference);

                long FinalValue = FirstValue | SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_ior: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                int FinalValue = FirstValue | SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_lmul: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                long SecondValue = Reflector->GetL(SecondValueReference);
                long FirstValue = Reflector->GetL(FirstValueReference);

                long FinalValue = FirstValue*SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_land: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                long SecondValue = Reflector->GetL(SecondValueReference);
                long FirstValue = Reflector->GetL(FirstValueReference);

                long FinalValue = FirstValue&SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_iand: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                int FinalValue = FirstValue&SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_iconst_m1: {
                SP->Push(Reflector->Add(-1));

                break;
            }

            case op_iconst_0: {
                SP->Push(Reflector->Add(0));

                break;
            }

            case op_iconst_1: {
                SP->Push(Reflector->Add(1));

                break;
            }

            case op_iconst_2: {
                SP->Push(Reflector->Add(2));

                break;
            }

            case op_iconst_3: {
                SP->Push(Reflector->Add(3));

                break;
            }

            case op_iconst_4: {
                SP->Push(Reflector->Add(4));

                break;
            }

            case op_iconst_5: {
                SP->Push(Reflector->Add(5));

                break;
            }

            case op_idiv: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                int FinalValue = FirstValue/SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_if_acmpeq: case op_if_acmpne: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];
                // TODO IMPLEMENT
                Object IndexByte = FirstIndexByte | SecondIndexByte;
                assert(true,"[Executive] equals for objects is not implemented yet!");
                break;
            }

            case op_if_icmpeq: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                if (FirstValue == SecondValue) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_multianewarray: {
                // TODO IMPLEMENT ME
                assert(true, "[Executive] Multidimensional arrays not implemented yet!");

                break;
            }

            case op_if_icmpne: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                if (FirstValue != SecondValue) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_if_icmplt: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                if (FirstValue < SecondValue) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_if_icmple: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                if (FirstValue <= SecondValue) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_if_icmpgt: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                if (FirstValue > SecondValue) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_if_icmpge: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                if (FirstValue >= SecondValue) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_ifeq: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID Value = SP->TopAndPop();

                int FirstValue = Reflector->GetI(Value);

                if (FirstValue == 0) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_ifne: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID Value = SP->TopAndPop();

                int FirstValue = Reflector->GetI(Value);

                if (FirstValue != 0) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_iflt: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID Value = SP->TopAndPop();

                int FirstValue = Reflector->GetI(Value);

                if (FirstValue < 0) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_ifle: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID Value = SP->TopAndPop();

                int FirstValue = Reflector->GetI(Value);

                if (FirstValue <= 0) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_ifgt: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID Value = SP->TopAndPop();

                int FirstValue = Reflector->GetI(Value);

                if (FirstValue > 0) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_ifge: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID Value = SP->TopAndPop();

                int FirstValue = Reflector->GetI(Value);

                if (FirstValue >= 0) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_ifnonnull: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID Value = SP->TopAndPop();
                GUID Null = GenerateNullGUID();

                if (memcmp(&Value,&Null,sizeof(GUID)) != 0) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_ifnull: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                GUID Value = SP->TopAndPop();
                GUID Null = GenerateNullGUID();

                if (memcmp(&Value,&Null,sizeof(GUID)) == 0) {
                    IP = IndexByte-1;
                }

                break;
            }

            case op_iinc: {
                u1 RegisterIndex = Code->Code[++IP];
                char IncrementConstant = Code->Code[++IP];

                GUID Local = Registers->Get(RegisterIndex);
                // TODO Is normal? Maybe type check?
                Reflector->Set(Local,Reflector->Get(Local)+IncrementConstant);

                break;
            }

            case op_imul: {
                GUID SecondValueReference = SP->TopAndPop();
                GUID FirstValueReference = SP->TopAndPop();

                int SecondValue = Reflector->GetI(SecondValueReference);
                int FirstValue = Reflector->GetI(FirstValueReference);

                int FinalValue = FirstValue*SecondValue;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(SecondValueReference,FinalValue);

                SP->Push(SecondValueReference);

                break;
            }

            case op_ineg: {
                GUID IntegerReference = SP->TopAndPop();

                int Value = Reflector->GetI(IntegerReference);

                Value = -Value;

                // FIXME
                // TODO MAKE STACK VARIABLES!!!
                Reflector->Set(IntegerReference,Value);

                SP->Push(IntegerReference);

                break;
            }

            case op_instanceof: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                Object ClassReferenceIndex = ClassReference->GetPE(IndexByte)->Class.Name;
                char* ClassName = ClassReference->GetPE(ClassReferenceIndex)->UTF.Bytes;
                // TODO IMPLEMENT!!!
                GUID ObjectReference = SP->TopAndPop();
                assert(true,"[Executive] Type checking for objects is not implemented yet!");
            }

            case op_invokeinterface: {

                //
                // Resolving method
                //

                GUID ObjectReference = SP->TopAndPop();

                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];
                Object Unused = Code->Code[++IP];
                Object SecondUnused = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                Object MethodClassIndex = ClassReference->File.Pool->Elements[IndexByte].Field.Class;
                Object MethodNATIndex = ClassReference->File.Pool->Elements[IndexByte].Field.NAT;
                Object MethodNameIndex = ClassReference->File.Pool->Elements[MethodNATIndex].NAT.Name;
                Object MethodSignatureIndex = ClassReference->File.Pool->Elements[MethodNATIndex].NAT.Descriptor;
                Object MethodClassNameIndex = ClassReference->File.Pool->Elements[MethodClassIndex].Class.Name;

                char* ClassName = ClassReference->File.Pool->Elements[MethodClassNameIndex].UTF.Bytes;
                char* MethodName = ClassReference->File.Pool->Elements[MethodNameIndex].UTF.Bytes;
                char* MethodDescriptor = ClassReference->File.Pool->Elements[MethodSignatureIndex].UTF.Bytes;

                Class* ReferenceClass = Reflector->GetClass(ClassName);
                Method* ReferenceMethod = ReferenceClass->GetMethod(MethodName,ClassName,MethodDescriptor);

                //
                //  Signature parsing
                //

                Object ArgumentCounter = 0;
                bool End = false;
                // TODO Verifier there
                for (size_t i = 1; i < strlen(MethodDescriptor); ++i) {
                    switch (MethodDescriptor[i]) {
                        case ')': {
                            End = true;
                            break;
                        }
                        case 'B': case 'C': case 'D': case 'F': case 'I': case 'J': case 'S': case 'Z': {
                            ArgumentCounter++;
                            break;
                        }
                        case '[': {
                            break;
                        }
                        case 'L': {
                            ArgumentCounter++;

                            while (MethodDescriptor[i] != ';') {
                                i++;
                            }

                            break;
                        }
                    }
                    if (End) {
                        break;
                    }
                }

                //
                // Pushing arguments
                //

                GUID Arguments[ArgumentCounter];

                for (size_t i = 0; i < ArgumentCounter; ++i) {
                    Arguments[ArgumentCounter-i-1] = SP->TopAndPop();
                }

                Object NumericalReturnAddress = reinterpret_cast<Object>(Code);
                // FIXME
                Object NumericalInstructionPointer = IP;
                Object NumericalClassReference = reinterpret_cast<Object>(ClassReference);
                Object NumericalStackPointer = reinterpret_cast<Object>(SP);
                Object NumericalLocalsPointer = reinterpret_cast<Object>(Registers);

                assert(ReferenceMethod == nullptr,"[Executive] [Reflector] Failed to getField from reference!");

                //
                // ABI PART
                //

                if (ReferenceMethod->Flag == MTNative) {
                    assert(true, "[Executive] Native!!!");
                }

                GUID NewRegisters = Reflector->AddPacked(sizeof(Locals));

                Locals* NewRegistersPointer = (Locals*)Reflector->GetPacked(NewRegisters);

                NewRegistersPointer->Initialize(ReferenceMethod->Attributes->Values[0].Code.Locals);

                NewRegistersPointer->Set(0,ObjectReference);

                GUID NewStack = Reflector->AddPacked(sizeof(Stack));

                Stack* NewStackPointer = (Stack*)Reflector->GetPacked(NewStack);

                NewStackPointer->Initialize(ReferenceMethod->Attributes->Values[0].Code.Stack+ABISpace);

                // TODO MAKE IT MORE VIEWABLE
                IP = -1;
                ClassReference = ReferenceClass;
                SP = NewStackPointer;
                Registers = NewRegistersPointer;
                // FIXME
                Code = &ReferenceMethod->Attributes->Values[0].Code;

                //
                // Pushing arguments to destination stack
                //

                SP->Push(Reflector->Add(NumericalReturnAddress));
                SP->Push(Reflector->Add(NumericalInstructionPointer));
                SP->Push(Reflector->Add(NumericalClassReference));
                SP->Push(Reflector->Add(NumericalStackPointer));
                SP->Push(Reflector->Add(NumericalLocalsPointer));

                for (size_t i = 0; i < ArgumentCounter; ++i) {
                    SP->Push(Arguments[i]);
                }

                break;
            }


            case op_new: {
                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                Object ClassIndex = ClassReference->GetPE(IndexByte)->Class.Name;

                char* ClassName = ClassReference->GetPE(ClassIndex)->UTF.Bytes;

                GUID ReferenceClass = Reflector->GetClassReference(ClassName);

                SP->Push(Reflector->AddObject(ReferenceClass));

                break;
            }

            case op_newarray: {
                Object Type = Code->Code[++IP];

                GUID Length = SP->TopAndPop();

                int NumericalLength = Reflector->GetI(Length);

                SP->Push(Reflector->AddArray(NumericalLength));

                break;
            }

            case op_nop: {
                break;
            }

            case op_pop: {
                SP->Pop();

                break;
            }

            case op_pop2: {
                SP->Pop();
                SP->Pop();

                break;
            }

            case op_invokespecial: {

                //
                // Resolving method
                //

                GUID ObjectReference = SP->TopAndPop();

                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                Object MethodClassIndex = ClassReference->File.Pool->Elements[IndexByte].Field.Class;
                Object MethodNATIndex = ClassReference->File.Pool->Elements[IndexByte].Field.NAT;
                Object MethodNameIndex = ClassReference->File.Pool->Elements[MethodNATIndex].NAT.Name;
                Object MethodSignatureIndex = ClassReference->File.Pool->Elements[MethodNATIndex].NAT.Descriptor;
                Object MethodClassNameIndex = ClassReference->File.Pool->Elements[MethodClassIndex].Class.Name;

                char* ClassName = ClassReference->File.Pool->Elements[MethodClassNameIndex].UTF.Bytes;
                char* MethodName = ClassReference->File.Pool->Elements[MethodNameIndex].UTF.Bytes;
                char* MethodDescriptor = ClassReference->File.Pool->Elements[MethodSignatureIndex].UTF.Bytes;

                Class* ReferenceClass = Reflector->GetClass(ClassName);
                Method* ReferenceMethod = ReferenceClass->GetMethod(MethodName,ClassName,MethodDescriptor);

                //
                //  Signature parsing
                //

                Object ArgumentCounter = 0;

                bool End = false;
                // TODO Verifier there
                for (size_t i = 1; i < strlen(MethodDescriptor); ++i) {
                    switch (MethodDescriptor[i]) {
                        case ')': {
                            End = true;
                            break;
                        }
                        case 'B': case 'C': case 'D': case 'F': case 'I': case 'J': case 'S': case 'Z': {
                            ArgumentCounter++;
                            break;
                        }
                        case '[': {
                            break;
                        }
                        case 'L': {
                            ArgumentCounter++;

                            while (MethodDescriptor[i] != ';') {
                                i++;
                            }

                            break;
                        }
                    }
                    if (End) {
                        break;
                    }
                }

                //
                // Pushing arguments
                //

                GUID Arguments[ArgumentCounter];

                for (size_t i = 0; i < ArgumentCounter; ++i) {
                    Arguments[ArgumentCounter-i-1] = SP->TopAndPop();
                }

                Object NumericalReturnAddress = reinterpret_cast<Object>(Code);
                Object NumericalInstructionPointer = IP;
                Object NumericalClassReference = reinterpret_cast<Object>(ClassReference);
                Object NumericalStackPointer = reinterpret_cast<Object>(SP);
                Object NumericalLocalsPointer = reinterpret_cast<Object>(Registers);

                if (ReferenceMethod == nullptr) {

                    printf("[Executive] [Reflector] Failed to find: \"");
                    printf(ClassName);
                    printf("::");
                    printf(MethodName);
                    printf(".");
                    printf(MethodDescriptor);
                    printf("\n");
                    assert(true,"[Executive] Halt!");

                }

                //
                //  Resolving superclass method
                //

                // FIXME

                while (true) {
                    if (ReferenceClass->File.Flags & OSuper) {
                        break;
                    }
                    else {
                        Object SuperClassIndex = ReferenceClass->File.Super;
                        Object SuperClassNameIndex = ReferenceClass->GetPE(SuperClassIndex)->Class.Name;

                        char* SuperClassName = ReferenceClass->GetPE(SuperClassNameIndex)->UTF.Bytes;

                        ReferenceClass = Reflector->GetClass(SuperClassName);

                        if (ReferenceClass->File.Flags != OSuper) {
                            continue;
                        }

                        ReferenceMethod = ReferenceClass->GetMethod(MethodName,MethodDescriptor);
                        break;
                    }
                    break;
                }

                //
                // ABI PART
                //

                printf("[Executive] [Reflector] Calling: ");
                printf(ClassName);
                printf("::");
                printf(MethodName);
                printf(".");
                printf(MethodDescriptor);
                printf("\n");

                if (ReferenceMethod->Flag & MTNative != 0) {
                    assert(true, "[Executive] Native!!!");
                }

                GUID NewRegisters = Reflector->AddPacked(sizeof(Locals));

                Locals* NewRegistersPointer = (Locals*)Reflector->GetPacked(NewRegisters);

                NewRegistersPointer->Initialize(ReferenceMethod->Attributes->Values[0].Code.Locals);

                NewRegistersPointer->Set(0,ObjectReference);

                GUID NewStack = Reflector->AddPacked(sizeof(Stack));

                Stack* NewStackPointer = (Stack*)Reflector->GetPacked(NewStack);

                NewStackPointer->Initialize(ReferenceMethod->Attributes->Values[0].Code.Stack+ABISpace);

                // TODO MAKE IT MORE VIEWABLE
                IP = -1;
                ClassReference = ReferenceClass;
                SP = NewStackPointer;
                Registers = NewRegistersPointer;
                // FIXME
                Code = &ReferenceMethod->Attributes->Values[0].Code;

                //
                // Pushing arguments to destination stack
                //

                SP->Push(Reflector->Add(NumericalReturnAddress));
                SP->Push(Reflector->Add(NumericalInstructionPointer));
                SP->Push(Reflector->Add(NumericalClassReference));
                SP->Push(Reflector->Add(NumericalStackPointer));
                SP->Push(Reflector->Add(NumericalLocalsPointer));

                for (size_t i = 0; i < ArgumentCounter; ++i) {
                    SP->Push(Arguments[i]);
                }

                break;
            }

            case op_invokestatic: {

                //
                // Resolving method
                //

                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                Object MethodClassIndex = ClassReference->File.Pool->Elements[IndexByte].Field.Class;
                Object MethodNATIndex = ClassReference->File.Pool->Elements[IndexByte].Field.NAT;
                Object MethodNameIndex = ClassReference->File.Pool->Elements[MethodNATIndex].NAT.Name;
                Object MethodSignatureIndex = ClassReference->File.Pool->Elements[MethodNATIndex].NAT.Descriptor;
                Object MethodClassNameIndex = ClassReference->File.Pool->Elements[MethodClassIndex].Class.Name;

                char* ClassName = ClassReference->File.Pool->Elements[MethodClassNameIndex].UTF.Bytes;
                char* MethodName = ClassReference->File.Pool->Elements[MethodNameIndex].UTF.Bytes;
                char* MethodDescriptor = ClassReference->File.Pool->Elements[MethodSignatureIndex].UTF.Bytes;

                Class* ReferenceClass = Reflector->GetClass(ClassName);
                Method* ReferenceMethod = ReferenceClass->GetMethod(MethodName,ClassName,MethodDescriptor);

                //
                //  Signature parsing
                //

                Object ArgumentCounter = 0;

                bool End = false;
                // TODO Verifier there
                for (size_t i = 1; i < strlen(MethodDescriptor); ++i) {
                    switch (MethodDescriptor[i]) {
                        case ')': {
                            End = true;
                            break;
                        }
                        case 'B': case 'C': case 'D': case 'F': case 'I': case 'J': case 'S': case 'Z': {
                            ArgumentCounter++;
                            break;
                        }
                        case '[': {
                            break;
                        }
                        case 'L': {
                            ArgumentCounter++;

                            while (MethodDescriptor[i] != ';') {
                                i++;
                            }

                            break;
                        }
                    }
                    if (End) {
                        break;
                    }
                }

                //
                // Pushing arguments
                //

                GUID Arguments[ArgumentCounter];

                for (size_t i = 0; i < ArgumentCounter; ++i) {
                    Arguments[ArgumentCounter-i-1] = SP->TopAndPop();
                }

                Object NumericalReturnAddress = reinterpret_cast<Object>(Code);
                Object NumericalInstructionPointer = IP;
                Object NumericalClassReference = reinterpret_cast<Object>(ClassReference);
                Object NumericalStackPointer = reinterpret_cast<Object>(SP);
                Object NumericalLocalsPointer = reinterpret_cast<Object>(Registers);

                assert(ReferenceMethod == nullptr,"[Executive] [Reflector] Failed to getField from reference!");

                //
                // ABI PART
                //

                /*

                printf("[Executive] [Reflector] Calling: ");
                printf(ClassName);
                printf("::");
                printf(MethodName);
                printf(".");
                printf(MethodDescriptor);
                printf("\n");

                 */

                if (ReferenceMethod->Flag & MTNative != 0) {

                    for (size_t i = 0; i < ArgumentCounter; ++i) {
                        SP->Push(Arguments[i]);
                    }

                    if (strcmp(MethodName, "peekS") == 0) {

                        short Value = Reflector->GetI(SP->TopAndPop());
                        long Address = Reflector->GetL(SP->TopAndPop());

                        peekS(Address, Value);

                    } else if (strcmp(MethodName, "peekB") == 0) {

                        char Value = Reflector->GetI(SP->TopAndPop());
                        long Address = Reflector->GetL(SP->TopAndPop());

                        peekB(Address, Value);

                    } else if (strcmp(MethodName, "peekI") == 0) {

                        int Value = Reflector->GetI(SP->TopAndPop());
                        long Address = Reflector->GetL(SP->TopAndPop());

                        peekI(Address, Value);

                    } else if (strcmp(MethodName, "peekL") == 0) {

                        long Value = Reflector->GetL(SP->TopAndPop());
                        long Address = Reflector->GetL(SP->TopAndPop());

                        peekL(Address, Value);

                    } else {

                        assert(true, "[Executive] Native!!!");

                    }
                }
                else {

                    GUID NewRegisters = Reflector->AddPacked(sizeof(Locals));

                    Locals* NewRegistersPointer = (Locals*)Reflector->GetPacked(NewRegisters);

                    NewRegistersPointer->Initialize(ReferenceMethod->Attributes->Values[0].Code.Locals);

                    GUID NewStack = Reflector->AddPacked(sizeof(Stack));

                    Stack* NewStackPointer = (Stack*)Reflector->GetPacked(NewStack);

                    NewStackPointer->Initialize(ReferenceMethod->Attributes->Values[0].Code.Stack+ABISpace);

                    // TODO MAKE IT MORE VIEWABLE
                    IP = -1;
                    ClassReference = ReferenceClass;
                    SP = NewStackPointer;
                    Registers = NewRegistersPointer;
                    // FIXME
                    Code = &ReferenceMethod->Attributes->Values[0].Code;

                    //
                    // Pushing arguments to destination stack
                    //

                    SP->Push(Reflector->Add(NumericalReturnAddress));
                    SP->Push(Reflector->Add(NumericalInstructionPointer));
                    SP->Push(Reflector->Add(NumericalClassReference));
                    SP->Push(Reflector->Add(NumericalStackPointer));
                    SP->Push(Reflector->Add(NumericalLocalsPointer));

                    for (size_t i = 0; i < ArgumentCounter; ++i) {
                        SP->Push(Arguments[i]);
                    }

                }

                break;

            }

            case op_invokevirtual: {
                //
                // Resolving method
                //

                GUID ObjectReference = SP->TopAndPop();

                Object FirstIndexByte = Code->Code[++IP];
                Object SecondIndexByte = Code->Code[++IP];

                Object IndexByte = FirstIndexByte | SecondIndexByte;

                Object MethodClassIndex = ClassReference->File.Pool->Elements[IndexByte].Field.Class;
                Object MethodNATIndex = ClassReference->File.Pool->Elements[IndexByte].Field.NAT;
                Object MethodNameIndex = ClassReference->File.Pool->Elements[MethodNATIndex].NAT.Name;
                Object MethodSignatureIndex = ClassReference->File.Pool->Elements[MethodNATIndex].NAT.Descriptor;
                Object MethodClassNameIndex = ClassReference->File.Pool->Elements[MethodClassIndex].Class.Name;

                char* ClassName = ClassReference->File.Pool->Elements[MethodClassNameIndex].UTF.Bytes;
                char* MethodName = ClassReference->File.Pool->Elements[MethodNameIndex].UTF.Bytes;
                char* MethodDescriptor = ClassReference->File.Pool->Elements[MethodSignatureIndex].UTF.Bytes;

                Class* ReferenceClass = Reflector->GetClass(ClassName);
                Method* ReferenceMethod = ReferenceClass->GetMethod(MethodName,ClassName,MethodDescriptor);

                //
                //  Signature parsing
                //

                Object ArgumentCounter = 0;

                bool End = false;
                // TODO Verifier there
                for (size_t i = 1; i < strlen(MethodDescriptor); ++i) {
                    switch (MethodDescriptor[i]) {
                        case ')': {
                            End = true;
                            break;
                        }
                        case 'B': case 'C': case 'D': case 'F': case 'I': case 'J': case 'S': case 'Z': {
                            ArgumentCounter++;
                            break;
                        }
                        case '[': {
                            break;
                        }
                        case 'L': {
                            ArgumentCounter++;

                            while (MethodDescriptor[i] != ';') {
                                i++;
                            }

                            break;
                        }
                    }
                    if (End) {
                        break;
                    }
                }

                //
                // Pushing arguments
                //

                GUID Arguments[ArgumentCounter];

                for (size_t i = 0; i < ArgumentCounter; ++i) {
                    Arguments[ArgumentCounter-i-1] = SP->TopAndPop();
                }

                Object NumericalReturnAddress = reinterpret_cast<Object>(Code);
                // FIXME
                Object NumericalInstructionPointer = IP+1;
                Object NumericalClassReference = reinterpret_cast<Object>(ClassReference);
                Object NumericalStackPointer = reinterpret_cast<Object>(SP);
                Object NumericalLocalsPointer = reinterpret_cast<Object>(Registers);

                //
                //  Resolving superclass method
                //

                while (true) {
                    if (ReferenceMethod != nullptr) {
                        break;
                    }
                    else {
                        Object SuperClassIndex = ReferenceClass->File.Super;
                        Object SuperClassNameIndex = ReferenceClass->GetPE(SuperClassIndex)->Class.Name;

                        char* SuperClassName = ReferenceClass->GetPE(SuperClassNameIndex)->UTF.Bytes;

                        ReferenceClass = Reflector->GetClass(SuperClassName);
                        ReferenceMethod = ReferenceClass->GetMethod(MethodName,MethodDescriptor);

                        if (ReferenceMethod == nullptr) {
                            continue;
                        }

                        break;
                    }
                    break;
                }

                //
                // ABI PART
                //

                printf("[Executive] [Reflector] Calling: ");
                printf(ClassName);
                printf("::");
                printf(MethodName);
                printf(".");
                printf(MethodDescriptor);
                printf("\n");


                if (ReferenceMethod->Flag & MTNative != 0) {
                    assert(true, "[Executive] Native!!!");
                }

                Locals NewRegisters{};
                NewRegisters.Initialize(ReferenceMethod->Attributes->Values[0].Code.Locals);

                NewRegisters.Set(0,ObjectReference);

                Stack NewStack{};
                NewStack.Initialize(ReferenceMethod->Attributes->Values[0].Code.Stack+ABISpace);

                // TODO MAKE IT MORE VIEWABLE
                IP = -1;
                ClassReference = ReferenceClass;
                SP = &NewStack;
                Registers = &NewRegisters;
                // FIXME
                Code = &ReferenceMethod->Attributes->Values[0].Code;

                //
                // Pushing arguments to destination stack
                //

                SP->Push(Reflector->Add(NumericalReturnAddress));
                SP->Push(Reflector->Add(NumericalInstructionPointer));
                SP->Push(Reflector->Add(NumericalClassReference));
                SP->Push(Reflector->Add(NumericalStackPointer));
                SP->Push(Reflector->Add(NumericalLocalsPointer));

                for (size_t i = 0; i < ArgumentCounter; ++i) {
                    SP->Push(Arguments[i]);
                }

                break;
            }

            default: {
                assert(true, "[Executive] Unknown opcode!");

                break;
            }
        }
    }
}
