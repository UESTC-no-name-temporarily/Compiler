#include <iostream>

class Module;
class BasicBlock;
class Function;
class Inst;

class Inst
{
    enum OpID
    {
        None,
        // Terminators
        Ret,
        Br,
        Switch,
        IndirectBr,
        Invoke,
        Resume,
        Unreachable,
        // Standard binary operators...
        Add,
        FAdd,
        Sub,
        FSub,
        Mul,
        FMul,
        UDiv,
        SDiv,
        FDiv,
        URem,
        SRem,
        FRem,
        // Logical operators...
        And,
        Or,
        Xor,
        // Memory operators...
        Alloca,
        Load,
        Store,
        GetElementPtr,
        Fence,
        AtomicCmpXchg,
        AtomicRMW,
        // Cast operators...
        Trunc,
        ZExt,
        SExt,
        FPToUI,
        FPToSI,
        UIToFP,
        SIToFP,
        FPTrunc,
        FPExt,
        PtrToInt,
        IntToPtr,
        BitCast,
        AddrSpaceCast,
        // Other operators...
        ICmp,
        FCmp,
        PHI,
        Call,
        Select,
        UserOp1,
        UserOp2,
        VAArg,
        ExtractElement,
        InsertElement,
        ShuffleVector,
        ExtractValue,
        InsertValue,
        LandingPad,
        // Vector operators
        ExtractElementInst,
        InsertElementInst,
        ShuffleVectorInst,
        ExtractValueInst,
        InsertValueInst,
        // Atomic operators
        AtomicRMWInst,
        FenceInst,
        AtomicCmpXchgInst,
        // Other operators
        VAArgInst,
        LandingPadInst,
        // Call operators
        CallInst,
        InvokeInst,
        // Terminator operators
        ReturnInst,
        BrInst,
        SwitchInst,
        IndirectBrInst,
        ResumeInst,
        UnreachableInst,
        // Binary operators
        AddInst,
        FAddInst,
        SubInst,
        FSubInst,
        MulInst,
        FMulInst,
        UDivInst,
        SDivInst,
        FDivInst,
        URemInst,
        SRemInst,
        FRemInst,
        // Logical operators
        AndInst,
        OrInst,
        XorInst,
        // Cast operators
        TruncInst,
        ZExtInst,
        SExtInst,
        FPToUIInst,
        FPToSIInst,
        UIToFPInst
    };
    bool HasSideEffect();
    
    public:
    
};