//---------------------------------------------------------------------------
#ifndef MC1PH
#define MC1PH

#include "MCStdElement.h"
#include "PPropFrm.h"
//---------------------------------------------------------------------------
class C1P : public CStdElement
{
private:
   long double N;//������� ��������
	long double K;//����������� ������������������
   std::complex <long double> *YN_1;
   TF1PProp *F1PProp;
public:
   C1P(void);
   virtual void __fastcall SetData(void);
   virtual void FUNKCIJA(int index, int end, long double DT);
   virtual void __fastcall GetDump(void *&Dump, DWORD &DumpSize);
   virtual void __fastcall SetDump(void *&Dump, DWORD &DumpSize);
   ~C1P();
};
//---------------------------------------------------------------------------
#endif
