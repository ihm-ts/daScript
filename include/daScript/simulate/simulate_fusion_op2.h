#pragma once

/*
 TODO:
    Op2ArgAny
    Op2AnyArg
 */

#define IMPLEMENT_ANY_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE,RTYPE,RCTYPE,CONSTTYPE) \
    struct Op2FusionPoint_##OPNAME##_##CTYPE : FusionPoint { \
        Op2FusionPoint_##OPNAME##_##CTYPE ( ) {} \
        struct SimNode_Op2R2VR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VR2V ( const LineInfo & at, uint32_t sp_l, uint32_t sp_r ) \
                : SimNode(at),stackTop_l(sp_l), stackTop_r(sp_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VR2V", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop_l); \
                V_SP(stackTop_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                CTYPE lv =  *(CTYPE *)(context.stack.sp() + stackTop_l); \
                CTYPE rv =  *(CTYPE *)(context.stack.sp() + stackTop_r); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_Op2ArgArg : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ArgArg ( const LineInfo & at, int32_t i_l, int32_t i_r ) \
                : SimNode(at),index_l(i_l), index_r(i_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgArg", sizeof(CTYPE), #CTYPE); \
                V_ARG(index_l); \
                V_ARG(index_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                CTYPE lv =  cast<CTYPE>::to(context.abiArguments()[index_l]); \
                CTYPE rv =  cast<CTYPE>::to(context.abiArguments()[index_r]); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t index_l; \
            int32_t index_r; \
        }; \
        struct SimNode_Op2R2VAny : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VAny ( const LineInfo & at, SimNode * rn, uint32_t sp ) \
                : SimNode(at), r(rn), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VAny", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                CTYPE lv =  *(CTYPE *)(context.stack.sp() + stackTop); \
                auto rv =  r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            SimNode * r; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2AnyR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2AnyR2V ( const LineInfo & at, SimNode * ln, uint32_t sp ) \
                : SimNode(at), l(ln), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "AnyR2V", sizeof(CTYPE), #CTYPE); \
                V_SUB(l); \
                V_SP(stackTop); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = l->eval##TYPE(context); \
                CTYPE rv =  *(CTYPE *)(context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            SimNode * l; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2ArgR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ArgR2V ( const LineInfo & at, int32_t i, uint32_t sp ) \
                : SimNode(at), index(i), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgR2V", sizeof(CTYPE), #CTYPE); \
                V_ARG(index); \
                V_SP(stackTop); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                CTYPE lv =  cast<CTYPE>::to(context.abiArguments()[index]); \
                CTYPE rv =  *(CTYPE *)(context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t  index; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2R2VArg : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VArg ( const LineInfo & at, int32_t i, uint32_t sp ) \
                : SimNode(at), index(i), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VArg", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop); \
                V_ARG(index); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                CTYPE lv =  *(CTYPE *)(context.stack.sp() + stackTop); \
                CTYPE rv =  cast<CTYPE>::to(context.abiArguments()[index]); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t  index; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2R2VConst : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VConst ( const LineInfo & at, CONSTTYPE cv, uint32_t sp ) \
                : SimNode(at), c(cv), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VConst", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop); \
                V_ARG(c); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                CTYPE lv =  *(CTYPE *)(context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (lv,c,context); \
            } \
            CONSTTYPE c; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2ArgConst : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ArgConst ( const LineInfo & at, CONSTTYPE cv, int32_t i ) \
                : SimNode(at), c(cv), index(i) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgConst", sizeof(CTYPE), #CTYPE); \
                V_ARG(index); \
                V_ARG(c); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                CTYPE lv =  cast<CTYPE>::to(context.abiArguments()[index]); \
                return SimPolicy<CTYPE>:: OPNAME (lv,c,context); \
            } \
            CONSTTYPE c; \
            int32_t index; \
        }; \
        struct SimNode_Op2AnyConst : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2AnyConst ( const LineInfo & at, SimNode * ll, CONSTTYPE cv ) \
                : SimNode(at), l(ll), c(cv) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "AnyConst", sizeof(CTYPE), #CTYPE); \
                V_SUB(l); \
                V_ARG(c); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = l->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,c,context); \
            } \
            SimNode * l; \
            CONSTTYPE c; \
        }; \
        struct SimNode_Op2ConstAny : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ConstAny ( const LineInfo & at, SimNode * rr, CONSTTYPE cv ) \
                : SimNode(at), r(rr), c(cv) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ConstAny", sizeof(CTYPE), #CTYPE); \
                V_ARG(c); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto rv = r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (c,rv,context); \
            } \
            SimNode * r; \
            CONSTTYPE c; \
        }; \
        struct SimNode_Op2ConstR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ConstR2V ( const LineInfo & at, CONSTTYPE cv, uint32_t sp ) \
                : SimNode(at), c(cv), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ConstR2V", sizeof(CTYPE), #CTYPE); \
                V_ARG(c); \
                V_SP(stackTop); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                CTYPE rv =  *(CTYPE *)(context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (c,rv,context); \
            } \
            CONSTTYPE c; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2ConstArg : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ConstArg ( const LineInfo & at, CONSTTYPE cv, int32_t i ) \
                : SimNode(at), c(cv), index(i) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ConstArg", sizeof(CTYPE), #CTYPE); \
                V_ARG(c); \
                V_ARG(index); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                CTYPE rv = cast<CTYPE>::to(context.abiArguments()[index]); \
                return SimPolicy<CTYPE>:: OPNAME (c,rv,context); \
            } \
            CONSTTYPE c; \
            int32_t index; \
        }; \
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) override { \
            auto tnode = static_cast<SimNode_Op2 *>(node); \
            /* OP(*,ConstValue) */ \
            if ( is(info,tnode->r,"ConstValue") ) { \
                auto cnode = static_cast<SimNode_ConstValue *>(tnode->r); \
                auto cvalue = cast<CONSTTYPE>::to(cnode->value); \
                /* OP(GetLocalR2V,ConstValue) */ \
                if ( is(info,tnode->l,"GetLocalR2V") ) { \
                    auto r2vnode_l = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2R2VConst>(node->debugInfo, cvalue, r2vnode_l->stackTop); \
                /* OP(GetArgument,ConstValue) */ \
                } else if ( is(info,tnode->l,"GetArgument") ) { \
                    auto argnode_l = static_cast<SimNode_GetArgument *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2ArgConst>(node->debugInfo, cvalue, argnode_l->index); \
                } else { \
                    return context->code->makeNode<SimNode_Op2AnyConst>(node->debugInfo, tnode->l, cvalue); \
                } \
            /* OP(ConstValue,*) */ \
            } else if ( is(info,tnode->l,"ConstValue") ) { \
                auto cnode = static_cast<SimNode_ConstValue *>(tnode->l); \
                auto cvalue = cast<CONSTTYPE>::to(cnode->value); \
                /* OP(ConstValue,GetLocalR2V) */ \
                if ( is(info,tnode->r,"GetLocalR2V") ) { \
                    auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2ConstR2V>(node->debugInfo, cvalue, r2vnode_r->stackTop); \
                /* OP(ConstValue,GetArgument) */ \
                } else if ( is(info,tnode->r,"GetArgument") ) { \
                    auto argnode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2ConstArg>(node->debugInfo, cvalue, argnode_r->index); \
                } else { \
                    return context->code->makeNode<SimNode_Op2ConstAny>(node->debugInfo, tnode->r, cvalue); \
                } \
            /* OP(GetLocalR2V,*) */ \
            } else if ( is(info,tnode->l,"GetLocalR2V") ) { \
                auto r2vnode_l = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->l); \
                /* OP(GetLocalR2V,GetLocalR2V) */ \
                if ( is(info,tnode->r,"GetLocalR2V") ) { \
                    auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2R2VR2V>(node->debugInfo, r2vnode_l->stackTop, r2vnode_r->stackTop); \
                /* OP(GetLocalR2V,GetLocalArg) */ \
                } else if ( is(info,tnode->r,"GetLocalArg") ) { \
                    auto argnode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2R2VArg>(node->debugInfo, argnode_r->index, r2vnode_l->stackTop); \
                } else { \
                    return context->code->makeNode<SimNode_Op2R2VAny>(node->debugInfo, tnode->r, r2vnode_l->stackTop); \
                } \
            /* OP(*,GetLocalR2V) */ \
            } else if ( is(info,tnode->r, "GetLocalR2V") ) { \
                auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                /* OP(GetArgument,GetLocalR2V) */ \
                if ( is(info,tnode->l,"GetArgument") ) { \
                    auto argnode_l = static_cast<SimNode_GetArgument *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2ArgR2V>(node->debugInfo, argnode_l->index, r2vnode_r->stackTop); \
                } else { \
                    return context->code->makeNode<SimNode_Op2AnyR2V>(node->debugInfo, tnode->l, r2vnode_r->stackTop); \
                } \
            /* OP(GetArgument,* */ \
            } else if ( is(info,tnode->l,"GetArgument") ) { \
                auto argnode_l = static_cast<SimNode_GetArgument *>(tnode->l); \
                /* OP(GetArgument,GetArgument) */ \
                if ( is(info,tnode->r,"GetArgument") ) { \
                    auto argnode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2ArgArg>(node->debugInfo, argnode_l->index, argnode_r->index); \
                } \
            } \
            return node; \
        } \
    };

#define IMPLEMENT_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    IMPLEMENT_ANY_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE,TYPE,CTYPE,CTYPE)

#define IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    IMPLEMENT_ANY_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE,Bool,bool,CTYPE)

#define IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,CTYPE) \
    IMPLEMENT_ANY_OP2_FUSION_POINT(OPNAME,,CTYPE,,vec4f,vec4f)

#define IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,CTYPE) \
    IMPLEMENT_ANY_OP2_FUSION_POINT(OPNAME,,CTYPE,Bool,bool,vec4f)

#ifndef FUSION_OP2_EVAL_CAST
#define FUSION_OP2_EVAL_CAST(expr)     (expr)
#endif

#define IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    struct Op2FusionPoint_##OPNAME##_##CTYPE : FusionPoint { \
        Op2FusionPoint_##OPNAME##_##CTYPE () {} \
        struct SimNode_Op2LocR2V : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2LocR2V ( const LineInfo & at, uint32_t sp_l, uint32_t sp_r ) \
                : SimNode(at),stackTop_l(sp_l), stackTop_r(sp_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "LocR2V", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop_l); \
                V_SP(stackTop_r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                CTYPE & lv =  *(CTYPE *)(context.stack.sp() + stackTop_l); \
                CTYPE rv =  *(CTYPE *)(context.stack.sp() + stackTop_r); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP2_EVAL_CAST(lv),rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_Op2LocAny : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2LocAny ( const LineInfo & at, SimNode * rn, uint32_t sp ) \
                : SimNode(at), r(rn), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "LocAny", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                CTYPE & lv =  *(CTYPE *)(context.stack.sp() + stackTop); \
                auto rv =  r->eval##TYPE(context); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP2_EVAL_CAST(lv),rv,context); \
            } \
            SimNode * r; \
            uint32_t stackTop; \
        }; \
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) override { \
            auto tnode = static_cast<SimNode_Op2 *>(node); \
            /* OP(GetLocal,*) */ \
            if ( is(info,tnode->l,"GetLocal") ) { \
                auto r2vnode_l = static_cast<SimNode_GetLocal *>(tnode->l); \
                /* OP(GetLocal,GetLocalR2V) */ \
                if ( is(info,tnode->r,"GetLocalR2V") ) { \
                    auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2LocR2V>(node->debugInfo, r2vnode_l->stackTop, r2vnode_r->stackTop); \
                } else { \
                    return context->code->makeNode<SimNode_Op2LocAny>(node->debugInfo, tnode->r, r2vnode_l->stackTop); \
                } \
            } \
            return node; \
        } \
    };

#define IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,CTYPE) \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,,CTYPE)

#define REGISTER_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    (*g_fusionEngine)[fuseName(#OPNAME,#CTYPE)].push_back(make_shared<Op2FusionPoint_##OPNAME##_##CTYPE>());

#define REGISTER_OP2_VEC_FUSION_POINT(OPNAME,CTYPE) \
    (*g_fusionEngine)[fuseName(#OPNAME,#CTYPE)].push_back(make_shared<Op2FusionPoint_##OPNAME##_##CTYPE>());

#define IMPLEMENT_OP2_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,Int,int32_t); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,UInt,uint32_t); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,Int64,int64_t); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define IMPLEMENT_OP2_NUMERIC_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP2_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,Float,float); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,Double,double);

#define IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,Int,int32_t); \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,UInt,uint32_t); \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,Int64,int64_t); \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define IMPLEMENT_SET_OP2_NUMERIC_FUSION_POINT(OPNAME) \
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,Float,float); \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,Double,double);

#define IMPLEMENT_BOOL_OP2_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,Int,int32_t); \
    IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,UInt,uint32_t); \
    IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,Int64,int64_t); \
    IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define IMPLEMENT_BOOL_OP2_NUMERIC_FUSION_POINT(OPNAME) \
    IMPLEMENT_BOOL_OP2_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,Float,float); \
    IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,Double,double);

#define REGISTER_OP2_INTEGER_FUSION_POINT(OPNAME) \
    REGISTER_OP2_FUSION_POINT(OPNAME,Int,int32_t); \
    REGISTER_OP2_FUSION_POINT(OPNAME,UInt,uint32_t); \
    REGISTER_OP2_FUSION_POINT(OPNAME,Int64,int64_t); \
    REGISTER_OP2_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define REGISTER_OP2_NUMERIC_FUSION_POINT(OPNAME) \
    REGISTER_OP2_INTEGER_FUSION_POINT(OPNAME); \
    REGISTER_OP2_FUSION_POINT(OPNAME,Float,float); \
    REGISTER_OP2_FUSION_POINT(OPNAME,Double,double);

// vectors

#define IMPLEMENT_OP2_VEC_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,int2 ); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,uint2); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,int3 ); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,uint3); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,int4 ); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,uint4);

#define IMPLEMENT_OP2_VEC_NUMERIC_FUSION_POINT(OPNAME)  \
    IMPLEMENT_OP2_VEC_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,float2 ); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,float3 ); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,float4 );

#define IMPLEMENT_BOOL_OP2_VEC_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,int2 ); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,uint2); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,int3 ); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,uint3); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,int4 ); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,uint4);

#define IMPLEMENT_BOOL_OP2_VEC_NUMERIC_FUSION_POINT(OPNAME)  \
    IMPLEMENT_BOOL_OP2_VEC_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,float2 ); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,float3 ); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,float4 );

#define IMPLEMENT_SET_OP2_VEC_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,int2 ); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,uint2); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,int3 ); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,uint3); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,int4 ); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,uint4);

#define IMPLEMENT_SET_OP2_VEC_NUMERIC_FUSION_POINT(OPNAME)  \
    IMPLEMENT_SET_OP2_VEC_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,float2 ); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,float3 ); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,float4 );

#define REGISTER_OP2_VEC_INTEGER_FUSION_POINT(OPNAME) \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,int2 ); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,uint2); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,int3 ); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,uint3); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,int4 ); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,uint4);

#define REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(OPNAME) \
    REGISTER_OP2_VEC_INTEGER_FUSION_POINT(OPNAME); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,float2 ); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,float3 ); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,float4 );