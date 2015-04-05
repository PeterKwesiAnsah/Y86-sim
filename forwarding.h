typedef struct {
    unsigned int e_dstE;
    unsigned int e_valE;
    unsigned int E_dstM;
    unsigned int e_Cnd;
    unsigned int E_icode;

    unsigned int d_srcA;
    unsigned int d_srcB;
    unsigned int D_icode;

    unsigned int M_valE;
    unsigned int m_valM;
    unsigned int M_dstE;
    unsigned int M_dstM;
    unsigned int M_Cnd;
    unsigned int M_icode;
    unsigned int M_valA;
    unsigned int m_stat;

    unsigned int W_valE;
    unsigned int W_dstE;
    unsigned int W_icode;
    unsigned int W_valM;
    unsigned int W_dstM;
    unsigned int W_stat;
} forwardType;
