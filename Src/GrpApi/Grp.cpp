//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Math.hpp>
#include <stdio.h >

#include "Grp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
long double Conv(long double cs, int numb)
{
   AnsiString A = FloatToStrF(cs, ffFixed, 18, 2);
   cs = StrToFloat(A);
	return cs;
}

LD sign(LD Val)
{
	if(Val >= 0) return Val; else return 0.0;
}

void __fastcall CSer::AddPset(LD X, LD Y, DWORD Color,
                              AnsiString Caption,
                              AnsiString BMPsetResName,
                              AnsiString BMCaptionResName,
                              THandle HI)
{
	TGrpPset *GP = new TGrpPset;
   GP->X = X;
   GP->Y = Y;
   GP->Color = Color;
   GP->Caption = Caption;
   if(BMPsetResName != "")
   {
   	GP->BMPset = new Graphics::TBitmap;
      GP->BMPset->LoadFromResourceName(HI, BMPsetResName);
      GP->BMPset->Transparent = true;
      GP->BMPset->TransparentColor =
      GP->BMPset->Canvas->Pixels[0][0];
   }
   else GP->BMPset = NULL;
   if(BMCaptionResName != "")
   {
   	GP->BMCaption = new Graphics::TBitmap;
      GP->BMCaption->LoadFromResourceName(HI, BMCaptionResName);
      GP->BMCaption->Transparent = true;
      GP->BMCaption->TransparentColor =
      GP->BMCaption->Canvas->Pixels[0][0];
   }
   else GP->BMCaption = NULL;
   Add(GP);
}
//---------------------------------------------------------------------------
LD __fastcall CSer::GetMinX(void)
{
   if(Count)
   {
		LD MinX = ((TGrpPset*)Items[0])->X;
   	for(int ci = 1; ci < Count; ci++)
      	if(MinX > ((TGrpPset*)Items[ci])->X)
         	MinX = ((TGrpPset*)Items[ci])->X;
   	return MinX;
   }
   return 0;
}
//---------------------------------------------------------------------------
LD __fastcall CSer::GetMinY(void)
{
   if(Count)
   {
		LD MinY = ((TGrpPset*)Items[0])->Y;
   	for(int ci = 1; ci < Count; ci++)
      	if(MinY > ((TGrpPset*)Items[ci])->Y)
         	MinY = ((TGrpPset*)Items[ci])->Y;
   	return MinY;
   }
   return 0;
}
//---------------------------------------------------------------------------
LD __fastcall CSer::GetMaxX(void)
{
   if(Count)
   {
		LD MaxX = ((TGrpPset*)Items[0])->X;
   	for(int ci = 1; ci < Count; ci++)
      	if(MaxX < ((TGrpPset*)Items[ci])->X)
         	MaxX = ((TGrpPset*)Items[ci])->X;
   	return MaxX;
   }
   return 0;
}
//---------------------------------------------------------------------------
LD __fastcall CSer::GetMaxY(void)
{
   if(Count)
   {
		LD MaxY = ((TGrpPset*)Items[0])->Y;
   	for(int ci = 1; ci < Count; ci++)
      	if(MaxY < ((TGrpPset*)Items[ci])->Y)
         	MaxY = ((TGrpPset*)Items[ci])->Y;
   	return MaxY;
   }
   return 0;
}
//---------------------------------------------------------------------------
void __fastcall CSer::Clr(void)
{
   for(int ci = 0; ci < Count; ci++)
   {
   	if(((TGrpPset*)Items[ci])->BMPset)
      	((TGrpPset*)Items[ci])->BMPset->Free();
   	if(((TGrpPset*)Items[ci])->BMCaption)
      	((TGrpPset*)Items[ci])->BMCaption->Free();
   	delete (TGrpPset*)Items[ci];
   }
	Clear();
}
//---------------------------------------------------------------------------
__fastcall CSer::~CSer(void)
{
	Clr();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall CSerManager::AddValue(void *&Data, DWORD &DataSize,
                                      void *Value, int ValSize)
{
   DataSize += ValSize;
   Data = (byte*)realloc(Data, DataSize);
   memcpy((void*)((DWORD)Data + DataSize - ValSize), Value, ValSize);
}
//---------------------------------------------------------------------------
void __fastcall CSerManager::GetValue(void *&Data, DWORD &DataSize,
                                      void *Value, int ValSize)
{
   memcpy(Value, (void*)((DWORD)Data + DataSize), ValSize);
   DataSize += ValSize;
}
//---------------------------------------------------------------------------
bool CSerManager::CheckSer(CSer *Ser)
{
	bool SerAgane = false;
   CSer *TmpSer = NULL; TGrpPset *TmpGrpPset = NULL, *TmpGrpPsetSrc = NULL;
   for(int ci = 0; ci < Count; ci++)
   {
   	TmpSer = (CSer*)Items[ci];
      if(TmpSer->Count == Ser->Count)
      {
      	for(int cj = 0; cj < TmpSer->Count; cj++)
      	{
         	TmpGrpPset = (TGrpPset*)TmpSer->Items[cj];
         	TmpGrpPsetSrc = (TGrpPset*)Ser->Items[cj];
         	if(TmpGrpPset->X == TmpGrpPsetSrc->X &&
            	TmpGrpPset->Y == TmpGrpPsetSrc->Y)
            {
            	SerAgane = true;
            }
            else
            {
            	SerAgane = false;
               break;
            }
      	}
         if(SerAgane) return SerAgane;
      }
      else SerAgane = false;
   }
	return SerAgane;
}
//---------------------------------------------------------------------------
bool __fastcall CSerManager::AddSer(CSer *Ser, AnsiString SerName)
{
   if(CheckSer(Ser)) return false;
   Ser->DefName = SerName;
	Add(Ser);
   SetSersName();
   return true;
}
//---------------------------------------------------------------------------
void __fastcall CSerManager::DelPrevSers(long int NumNotDel)
{
   if(NumNotDel == -1) return;
	if(Count - 1 - NumNotDel < 0) return;
   for(long int ci = Count - 2 - NumNotDel; ci >= 0; ci--)
   {
      delete (CSer*)Items[ci];
   	Delete(ci);
   }
   SetSersName();
}
//---------------------------------------------------------------------------
void __fastcall CSerManager::SetSersName(void)
{
	for(int ci = 0; ci < Count; ci++)
   	((CSer*)Items[ci])->Name = ((CSer*)Items[ci])->DefName + ci;
}
//---------------------------------------------------------------------------
AnsiString __fastcall CSerManager::GetTextData(TList *Sers)
{
	if(!Count || !Sers) return "";
   if(!Sers->Count || Count < Sers->Count) return "";
	AnsiString TextData = "";
   int Max = 0;
   for(int ci = 0; ci < Sers->Count; ci++)
   {
   	CSer *Ser = (CSer*)Sers->Items[ci];
      TextData += Ser->Name + "\t\t\t";
      if(Max < Ser->Count) Max = Ser->Count;
   }
   TextData += "\r\n";
   for(int ci = 0; ci < Max; ci++)
   {
      for(int cj = 0; cj < Sers->Count; cj++)
      {
   		CSer *Ser = (CSer*)Sers->Items[cj];
         if(ci < Ser->Count)
         {
         	TextData += FloatToStr(((TGrpPset*)Ser->Items[ci])->X);
            TextData += "\t";
         	TextData += FloatToStr(((TGrpPset*)Ser->Items[ci])->Y);
         	TextData += "\t\t";
         }
         else TextData += "\t\t\t";
      }
      TextData += "\r\n";
   }
   TextData += "\r\n";
   return TextData;
}
//---------------------------------------------------------------------------
AnsiString __fastcall CSerManager::GetTextData(void)
{
	return GetTextData(this);
}
//---------------------------------------------------------------------------
bool __fastcall CSerManager::SetTextData(AnsiString TextData)
{
	if(TextData == "") return true;
   AnsiString Name = ""; int cj = 1;
   Name.SetLength(TextData.Length());
   TList *TmpList = new TList;
   for(int ci = 1; ci <= TextData.Length(); ci++)
   {
      if(TextData[ci] == '\r') break;
   	if(TextData[ci] != '\t')
      {
         Name[cj] = TextData[ci];
         cj++;
      }
      else
      {
         if(cj)
         {
            CSer *Ser = new CSer;
            Ser->Name = Name;
            Ser->Color = random(0xFFFFFF);
            TmpList->Add(Ser);
            Add(Ser);
      		cj = 0;
         	Name = "";
   			Name.SetLength(TextData.Length());
         }
      }
   }
   if(!TmpList->Count)
   {
   	TmpList->Clear();
   	delete TmpList;
      return false;
   }

   TmpList->Clear();
   delete TmpList;
   return true;
}
//---------------------------------------------------------------------------
void __fastcall CSerManager::DelSers(TList *Sers)
{
	if(!Count || !Sers) return;
	if(!Sers->Count || Count < Sers->Count) return;
   for(int ci = 0; ci < Sers->Count; ci++) Remove(Sers->Items[ci]);
}
//---------------------------------------------------------------------------
void __fastcall CSerManager::DelAllSers(void)
{
   for(int ci = 0; ci < Count; ci++) delete (CSer*)Items[ci];
	Clear();
}
//---------------------------------------------------------------------------
TRectLD CSerManager::GetGrpReg(void)
{
   TRectLD Rect;
   if(Count)
   {
   	LD MinX = ((CSer*)Items[0])->GetMinX(), TMinX;
   	LD MinY = ((CSer*)Items[0])->GetMinY(), TMinY;
   	LD MaxX = ((CSer*)Items[0])->GetMaxX(), TMaxX;
   	LD MaxY = ((CSer*)Items[0])->GetMaxY(), TMaxY;
   	for(int ci = 1; ci < Count; ci++)
   	{
      	if(MinX > (TMinX = ((CSer*)Items[ci])->GetMinX()))
         	MinX = TMinX;
      	if(MinY > (TMinY = ((CSer*)Items[ci])->GetMinY()))
         	MinY = TMinY;
      	if(MaxX < (TMaxX = ((CSer*)Items[ci])->GetMaxX()))
         	MaxX = TMaxX;
      	if(MaxY < (TMaxY = ((CSer*)Items[ci])->GetMaxY()))
         	MaxY = TMaxY;
   	}
      if(MinX == MaxX)
      {
      	if(MinX >= 0)
         	MinX = 0;
         else
         	MaxX = 0;
         if(!MinX && !MaxX){MinX = -10.0; MaxX = 10.0;}
      }
      if(MinY == MaxY)
      {
      	if(MinY >= 0)
         	MinY = 0;
         else
         	MaxY = 0;
         if(!MinY && !MaxY){MinY = -10.0; MaxY = 10.0;}
      }
      Rect = RectLD(MinX, MinY, MaxX, MaxY);
   }
   else Rect = RectLD(-10.0, -10.0, 10.0, 10.0);
   return Rect;
}
//---------------------------------------------------------------------------
TPointLD CSerManager::GetOffsetLogicCenter(void)
{
   TPointLD Point;
   TRectLD Rect = GetGrpReg();
   Point.X = -Rect.Left;
   Point.Y = -Rect.Top;
	return Point;
}
//---------------------------------------------------------------------------
TPointLD CSerManager::GetLogicCenter(void)
{
   TPointLD Point;
   TRectLD Rect = GetGrpReg();
   if(Rect.Left*Rect.Right < 0)
   	Point.X = 0.0;
   else
   	if(Rect.Left < 0) Point.X = Rect.Right; else Point.X = Rect.Left;
   if(Rect.Top*Rect.Bottom < 0)
   	Point.Y = 0.0;
   else
   	if(Rect.Top < 0) Point.Y = Rect.Bottom; else Point.Y = Rect.Top;
   return Point;
}
//---------------------------------------------------------------------------
void __fastcall CSerManager::GetDump(void *&Dump, DWORD &DumpSize)
{
	int C = Count;
   AddValue(Dump, DumpSize, &C, sizeof(int));
   for(int ci = 0; ci < Count; ci++)
   {
   	CSer *Ser = (CSer*)Items[ci];
   	AddValue(Dump, DumpSize, &Ser->Selected, sizeof(int));
   	AddValue(Dump, DumpSize, Ser->DefName.c_str(), Ser->DefName.Length());
   	AddValue(Dump, DumpSize, Ser->Name.c_str(), Ser->Name.Length());
   	AddValue(Dump, DumpSize, &Ser->Color, sizeof(DWORD));
      C = Ser->Count;
   	AddValue(Dump, DumpSize, &C, sizeof(int));
      for(int cj = 0; cj < Ser->Count; cj++)
      {
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall CSerManager::SetDump(void *&Dump, DWORD &DumpSize)
{
   //GetValue(Dump, DumpSize, &Left, sizeof(int));
}
//---------------------------------------------------------------------------
__fastcall CSerManager::~CSerManager(void)
{
	DelAllSers();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall CGrps::CalcVNames(void)
{
	LD DMF = GrpDesc.POFY*GrpDesc.NumIntermedFis;
   SIZE TS;
   AnsiString StrTmp = FloatToStr(Conv(LogicReg.Top, 5));
   GetTextExtentPoint32(BackBuf, StrTmp.c_str(),
                        StrTmp.Length(), &TS);
   LD Max = TS.cx;
	for(LD ci = LogicReg.Top; ci < LogicReg.Bottom; ci += DMF)
   {
      StrTmp = FloatToStr(Conv(ci, 5));
   	GetTextExtentPoint32(BackBuf, StrTmp.c_str(),
                           StrTmp.Length(), &TS);
      if(Max < TS.cx) Max = TS.cx;
   }
   YVNames.Right = Max + GrpDesc.MainFisHeight + Max*0.05;
   if(LogicCenter.X <= 0)
   {
   	if(fabs(LogicReg.Left) - YVNames.Right/DW >= 0)
      	YVNames.Right = 0;
   }
   XVNames.Bottom = TS.cy;
}
//---------------------------------------------------------------------------
void __fastcall CGrps::CalcStMetrix(void)
{
	OffsetLogicCenter = SerManager->GetOffsetLogicCenter();
   LogicCenter = SerManager->GetLogicCenter();
   LogicReg = SerManager->GetGrpReg();
   GrpDesc.POFX = LogicReg.Width()/LD(GrpDesc.NumMainFis*GrpDesc.NumIntermedFis);
   GrpDesc.POFY = LogicReg.Height()/LD(GrpDesc.NumMainFis*GrpDesc.NumIntermedFis);
   CalcVNames();
}
//---------------------------------------------------------------------------
void __fastcall CGrps::CalcDinMetrix(void)
{
	LD LogicWidth = LogicReg.Width();
	LD LogicHeight = LogicReg.Height();
   PhizReg = RectLD(ClientWidth*0.1 + YVNames.Right,
                    ClientHeight*0.1 + XVNames.Bottom,
                    ClientWidth*0.9, ClientHeight*0.9);
   if(!LogicWidth) LogicWidth = PhizReg.Width();
   if(!LogicHeight) LogicHeight = PhizReg.Height();
   DW = PhizReg.Width()/LogicWidth;
   DH = PhizReg.Height()/LogicHeight;
   PhizCenter = PointLD(OffsetLogicCenter.X*DW + PhizReg.Left,
                        OffsetLogicCenter.Y*DH + PhizReg.Top);
}
//---------------------------------------------------------------------------
LD __fastcall CGrps::InvertPhizX(LD X)
{
	return ClientWidth - X;
}
//---------------------------------------------------------------------------
LD __fastcall CGrps::InvertPhizY(LD Y)
{
	return ClientHeight - Y;
}
//---------------------------------------------------------------------------
LD __fastcall CGrps::GetPhizX(LD LogicX)
{
	LD Result = PhizCenter.X + DW*LogicX;
   if(GrpDesc.InvertX)
   	return InvertPhizX(Result);
   else
   	return Result;
}
//---------------------------------------------------------------------------
LD __fastcall CGrps::GetPhizY(LD LogicY)
{
	LD Result = PhizCenter.Y + DH*LogicY;
   if(!GrpDesc.InvertY)
   	return InvertPhizY(Result);
   else
   	return Result;
}
//---------------------------------------------------------------------------
void CGrps::CreateBuf(void* &Buff, DWORD BufWidth, DWORD BufHeight)
{
	HDC BuffDC = (HDC)Buff, WndDC = GetDC(Handle);
   DeleteDC(BuffDC);
   HBITMAP hbm;
   BuffDC = CreateCompatibleDC(WndDC);
   hbm = CreateCompatibleBitmap(WndDC, BufWidth, BufHeight);
   SelectObject(BuffDC, hbm);
   DeleteObject(hbm);
   Buff = (void*)BuffDC;
}
//---------------------------------------------------------------------------
void CGrps::TransparentBlt(HDC hdc, HDC SrcDC,
                           short xStart, short yStart,
                           int Width, int Height,
                           COLORREF cTransparentColor)
{
   BITMAP bm;
   COLORREF cColor;
   HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
   HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
   HDC hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
   POINT ptSize;
   hdcTemp = SrcDC;
   ptSize.x = Width;
   ptSize.y = Height;
   DPtoLP(hdcTemp, &ptSize, 1);
   hdcBack = CreateCompatibleDC(hdc);
   hdcObject = CreateCompatibleDC(hdc);
   hdcMem = CreateCompatibleDC(hdc);
   hdcSave = CreateCompatibleDC(hdc);
   bmAndBack = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
   bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
   bmAndMem = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
   bmSave = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
   bmBackOld = SelectObject(hdcBack, bmAndBack);
   bmObjectOld = SelectObject(hdcObject, bmAndObject);
   bmMemOld = SelectObject(hdcMem, bmAndMem);
   bmSaveOld = SelectObject(hdcSave, bmSave);
   SetMapMode(hdcTemp, GetMapMode(hdc));
   BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
   cColor = SetBkColor(hdcTemp, cTransparentColor);
   BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
          SRCCOPY);
   SetBkColor(hdcTemp, cColor);
   BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
          NOTSRCCOPY);
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
          SRCCOPY);
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);
   BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
          SRCCOPY);
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);
   DeleteObject(SelectObject(hdcBack, bmBackOld));
   DeleteObject(SelectObject(hdcObject, bmObjectOld));
   DeleteObject(SelectObject(hdcMem, bmMemOld));
   DeleteObject(SelectObject(hdcSave, bmSaveOld));
   DeleteDC(hdcMem);
   DeleteDC(hdcBack);
   DeleteDC(hdcObject);
   DeleteDC(hdcSave);
}
//---------------------------------------------------------------------------
void __fastcall CGrps::Pset(LD X, LD Y, DWORD Color,
                            TRendStyle RendStyle,
                            AnsiString Caption,
                            Graphics::TBitmap *BMPset,
                            Graphics::TBitmap *BMCaption)
{
   int BoundW = 0;
   int BoundH = 0;
   if(!BMPset)
   	SetPixel(BackBuf, GetPhizX(X), GetPhizY(Y), Color);
   else
   {
   	TransparentBlt(BackBuf, BMPset->Canvas->Handle,
                     GetPhizX(X) - BMPset->Width/2,
                     GetPhizY(Y) - BMPset->Height/2,
                     BMPset->Width, BMPset->Height,
                     ColorToRGB(BMPset->TransparentColor));
      BoundW += BMPset->Width/2;
      BoundH += BMPset->Height/2;
   }
   if(!BMCaption)
   {
      SIZE TS;
      SetBkMode(BackBuf, TRANSPARENT);
      GetTextExtentPoint32(BackBuf, Caption.c_str(),
                           Caption.Length(), &TS);
      /*
      BackBuf->Canvas->TextOut(GetPhizX(X) - TS.cx - BoundW,
                               GetPhizY(Y) - TS.cy - BoundH,
                               Caption);
      */
      SetBkMode(BackBuf, OPAQUE);
   }
   else
   {
      /*
   	BackBuf->Canvas->Draw(GetPhizX(X) - BMCaption->Width - BoundW,
                            GetPhizY(Y) - BMCaption->Height - BoundH,
                            BMCaption);
   	*/
   }
}
//---------------------------------------------------------------------------
void __fastcall CGrps::Line(HDC hdc, LD X1, LD Y1, LD X2, LD Y2,
                            DWORD PenColor, int PenStyle, int PenWidth)
{
   TCanvas *Cvs = new TCanvas;
   Cvs->Handle = hdc;
   Cvs->Pen->Color = (TColor)PenColor;
   Cvs->Pen->Width = PenWidth;
   Cvs->Pen->Style = (TPenStyle)PenStyle;
   Cvs->MoveTo(GetPhizX(X1), GetPhizY(Y1));
   Cvs->LineTo(GetPhizX(X2), GetPhizY(Y2));
   delete Cvs;
}
//---------------------------------------------------------------------------
void __fastcall CGrps::DTextOut(LD X, LD Y, AnsiString Text,
                                TFont *Font,
                                TTextRend TextRend)
{
   TCanvas *Cvs = new TCanvas;
   Cvs->Handle = BackBuf;
   //if(Font) BackBuf->Canvas->Font = Font;
   SIZE TS;
   GetTextExtentPoint32(Cvs->Handle, Text.c_str(),
                        Text.Length(), &TS);
   int TASv = GetTextAlign(Cvs->Handle);
   SetTextAlign(Cvs->Handle, (int)TextRend);
   SetBkMode(Cvs->Handle, TRANSPARENT);
   Cvs->TextOut(GetPhizX(X), GetPhizY(Y), Text);
   SetBkMode(Cvs->Handle, OPAQUE);
   SetTextAlign(Cvs->Handle, TASv);
   delete Cvs;
}
//---------------------------------------------------------------------------
void __fastcall CGrps::DrawSers(void)
{
   CSer *TmpSer;
   TGrpPset *TmpGrpPset1, *TmpGrpPset2;
   int WD = 1;
	for(int ci = 0; ci < SerManager->Count; ci++)
   {
   	TmpSer = (CSer*)SerManager->Items[ci];
      if(TmpSer->Selected) WD = 2; else WD = 1;
      for(int cj = 0; cj < TmpSer->Count; cj += 10)
      {
      	TmpGrpPset1 = (TGrpPset*)TmpSer->Items[cj];
         if(cj + 10 < TmpSer->Count)
         {
      		TmpGrpPset2 = (TGrpPset*)TmpSer->Items[cj + 10];
         	Line(BackBuf, TmpGrpPset1->X, TmpGrpPset1->Y,
                 TmpGrpPset2->X, TmpGrpPset2->Y,
                 TmpSer->Color, PS_SOLID, WD);
         }
         Pset(TmpGrpPset1->X, TmpGrpPset1->Y, TmpGrpPset1->Color,
              rsLogic, TmpGrpPset1->Caption, TmpGrpPset1->BMPset,
              TmpGrpPset1->BMCaption);
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall CGrps::DrawAxis(void)
{
   LD DMF, Obtaned;
   SIZE TS;
   AnsiString StrTmp;
   TTextRend TR, TRL;
   if(GrpDesc.XAxVisible)
   {
   	Line(BackBuf, LogicReg.Left, LogicCenter.Y,
           LogicReg.Right, LogicCenter.Y, ColorToRGB(GrpDesc.AxisColor));
   	DMF = GrpDesc.POFX*GrpDesc.NumIntermedFis;
      if(!DMF) DMF = 1.0;
      for(LD ci = LogicReg.Left; ci <= LogicReg.Right; ci += DMF)
      {
      	Line(BackBuf, ci, LogicCenter.Y + GrpDesc.MainFisHeight/DH, ci,
              LogicCenter.Y - GrpDesc.MainFisHeight/DH, ColorToRGB(GrpDesc.AxisColor));
         if(ci + DMF <= LogicReg.Right)
         	for(LD cj = ci + GrpDesc.POFX; cj < ci + DMF; cj += GrpDesc.POFX)
      			Line(BackBuf, cj, LogicCenter.Y + GrpDesc.IntermedFisHeight/DH, cj,
                    LogicCenter.Y - GrpDesc.IntermedFisHeight/DH, ColorToRGB(GrpDesc.AxisColor));
         Obtaned = ci;
      	StrTmp = FloatToStr(Conv(ci, 5));
      	if(GrpDesc.InvertY) TR = trCenterTop; else TR = trCenterBottom;
      	DTextOut(ci,
               	LogicCenter.Y - (GrpDesc.MainFisHeight + 2)/DH,
               	StrTmp, NULL, TR);
      }
      if(Obtaned < LogicReg.Right)
      	for(LD cj = Obtaned + GrpDesc.POFX; cj < LogicReg.Right; cj += GrpDesc.POFX)
         	Line(BackBuf, cj, LogicCenter.Y + GrpDesc.IntermedFisHeight/DH, cj,
                LogicCenter.Y - GrpDesc.IntermedFisHeight/DH, ColorToRGB(GrpDesc.AxisColor));
      if(GrpDesc.InvertY) TRL = trCenterBottom; else TRL = trCenterTop;
      Obtaned = fabs(LogicReg.Right) >= fabs(LogicReg.Left) ? LogicReg.Right:LogicReg.Left;
   	DTextOut(Obtaned,
               LogicCenter.Y + (GrpDesc.MainFisHeight + 2)/DH,
               XName, NULL, TRL);
   }
   if(GrpDesc.YAxVisible)
   {
   	Line(BackBuf, LogicCenter.X, LogicReg.Top,
           LogicCenter.X, LogicReg.Bottom, ColorToRGB(GrpDesc.AxisColor));
   	DMF = GrpDesc.POFY*GrpDesc.NumIntermedFis;
      if(!DMF) DMF = 1.0;
      for(LD ci = LogicReg.Top; ci <= LogicReg.Bottom; ci += DMF)
      {
      	Line(BackBuf, LogicCenter.X + GrpDesc.MainFisHeight/DW, ci,
              LogicCenter.X - GrpDesc.MainFisHeight/DW, ci, ColorToRGB(GrpDesc.AxisColor));
         StrTmp = FloatToStr(Conv(ci, 5));
         if(GrpDesc.InvertX) TR = trRightMed; else TR = trLeftMed;
   		DTextOut(LogicCenter.X - (GrpDesc.MainFisHeight + 2)/DW,
                  ci, StrTmp, NULL, TR);
         if(ci + DMF <= LogicReg.Bottom)
         	for(LD cj = ci + GrpDesc.POFY; cj < ci + DMF; cj += GrpDesc.POFY)
      			Line(BackBuf, LogicCenter.X + GrpDesc.IntermedFisHeight/DW, cj,
                    LogicCenter.X - GrpDesc.IntermedFisHeight/DW, cj, ColorToRGB(GrpDesc.AxisColor));
         Obtaned = ci;
      }
      if(Obtaned < LogicReg.Bottom)
      	for(LD cj = Obtaned + GrpDesc.POFY; cj < LogicReg.Bottom; cj += GrpDesc.POFY)
         	Line(BackBuf, LogicCenter.X + GrpDesc.IntermedFisHeight/DW, cj,
                 LogicCenter.X - GrpDesc.IntermedFisHeight/DW, cj, ColorToRGB(GrpDesc.AxisColor));
      if(!GrpDesc.InvertX) TRL = trRightMed; else TRL = trLeftMed;
      Obtaned = fabs(LogicReg.Top) > fabs(LogicReg.Bottom) ? LogicReg.Top:LogicReg.Bottom;
      DTextOut(LogicCenter.X + (GrpDesc.MainFisHeight + 2)/DW,
               Obtaned, YName, NULL, TRL);
   }
}
//---------------------------------------------------------------------------
void __fastcall CGrps::DrawBackGround()
{
   RECT rect;
   SetRect(&rect, 0, 0, ClientWidth, ClientHeight);
  	FillRect(BackBuf, &rect, CreateSolidBrush(GrpDesc.BackColor));
}
//---------------------------------------------------------------------------
void __fastcall CGrps::ShowHintCoord(int MX, int MY)
{
   if(HintActive) return;
   SIZE TS;
   AnsiString ResultStr = "( " + FloatToStr(CurrentXY.X);
   ResultStr += " );( ";
   ResultStr += FloatToStr(CurrentXY.Y);
   ResultStr += " )";
   if(!HW)
   {
		HW = new THintWindow(this);
   	HW->Parent = this;
   	HW->Color = clInfoBk;
   	GetTextExtentPoint32(HW->Canvas->Handle, ResultStr.c_str(),
                           ResultStr.Length() + 2, &TS);
   	TRect rect(ClientToScreen(Point(MX, MY - TS.cy - 4)),
                          ClientToScreen(Point(MX + TS.cx, MY - 3)));
		HW->ActivateHint(rect, ResultStr);
   }
   HintActive = true;
}
//---------------------------------------------------------------------------
void __fastcall CGrps::ReleaseHC(void)
{
	if(!HintActive) return;
   HW->ReleaseHandle();
   delete HW;
   HW = NULL;
   HintActive = false;
}
//---------------------------------------------------------------------------
void __fastcall CGrps::Draw(void)
{
   DrawBackGround();
	DrawAxis();
   DrawSers();
   InvalidateRect(Handle, NULL, false);
}
//---------------------------------------------------------------------------
void __fastcall CGrps::Render(HDC hdc)
{
	if(!BackBuf) return;
	BitBlt(hdc, 0, 0, ClientWidth, ClientHeight, BackBuf, 0, 0, SRCCOPY);
}
//---------------------------------------------------------------------------
CSer* __fastcall CGrps::GetSer(int X, int Y)
{
   CSer *TmpSer;
   TGrpPset *TmpGrpPset;
	for(int ci = 0; ci < SerManager->Count; ci++)
   {
   	TmpSer = (CSer*)SerManager->Items[ci];
      for(int cj = 0; cj < TmpSer->Count; cj++)
      {
      	TmpGrpPset = (TGrpPset*)TmpSer->Items[cj];
         if((int)GetPhizX(TmpGrpPset->X) - 2 <= X &&
            (int)GetPhizY(TmpGrpPset->Y) - 2 <= Y &&
            (int)GetPhizX(TmpGrpPset->X) + 2 >= X &&
            (int)GetPhizY(TmpGrpPset->Y) + 2 >= Y)
         {
         	return TmpSer;
			}
      }
   }
   return NULL;
}
//---------------------------------------------------------------------------
void __fastcall CGrps::SelectSer(CSer *Ser)
{
   if(!SelSers || !Ser) return;
   if(SelSers->IndexOf(Ser) != -1) return;
   Ser->Selected = true;
   SelSers->Add(Ser);
   Draw();
}
//---------------------------------------------------------------------------
void __fastcall CGrps::UnSelectSer(void)
{
   if(!SelSers || !SelSers->Count) return;
	for(int ci = 0; ci < SelSers->Count; ci++)
      ((CSer*)SelSers->Items[ci])->Selected = false;
   SelSers->Clear();
   Draw();
}
//---------------------------------------------------------------------------
void __fastcall CGrps::GrpPsetHitTest(TGPHTData GPHTData)
{
	if(GPHTDataEvent) GPHTDataEvent(GPHTData);
}
//---------------------------------------------------------------------------
void __fastcall CGrps::OnGrpResize(TObject *Sender)
{
   if(!BackBuf) return;
   CreateBuf(BackBuf, ClientWidth, ClientHeight);
   CalcDinMetrix();
	Draw();
}
//---------------------------------------------------------------------------
void __fastcall CGrps::OnMD(TObject *Sender, TMouseButton Button,
                            TShiftState Shift, int X, int Y)
{
   if(Button == mbLeft)
   {
   	if(!HitTest)
      {
   		LD X1, Y1;
      	if(GrpDesc.InvertX)
         	X1 = ClientWidth - X - PhizReg.Left;
      	else
      		X1 = X - PhizReg.Left;
      	if(!GrpDesc.InvertY)
      		Y1 = ClientHeight - Y - PhizReg.Top;
      	else
      		Y1 = Y - PhizReg.Top;
			if(LogicReg.InRect(X1/DW - OffsetLogicCenter.X,
                            Y1/DH - OffsetLogicCenter.Y))
   		{
   			Line(GetDC(Handle), X1/DW - OffsetLogicCenter.X,
                 Y1/DH - OffsetLogicCenter.Y,
                 LogicCenter.X, Y1/DH - OffsetLogicCenter.Y,
                 RGB(0, 0, 0), psDashDot);
   		   Line(GetDC(Handle), X1/DW - OffsetLogicCenter.X,
                 Y1/DH - OffsetLogicCenter.Y,
                 X1/DW - OffsetLogicCenter.X, LogicCenter.Y,
                 RGB(0, 0, 0), psDashDot);
            CurrentXY.X = X1/DW - OffsetLogicCenter.X;
         	CurrentXY.Y = Y1/DH - OffsetLogicCenter.Y;
         	ShowHintCoord(X, Y);
         	HintActive = false;
         	PointLineActive = true;
   		}
         UnSelectSer();
      }
      else
      {
      	if(!Shift.Contains(ssShift)) UnSelectSer();
         SelectSer(GetSer(X, Y));
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall CGrps::OnMM(TObject *Sender, TShiftState Shift,
                            int X, int Y)
{
   CSer *TmpSer;
   TGrpPset *TmpGrpPset;
   bool NotData = true;
	for(int ci = 0; ci < SerManager->Count; ci++)
   {
   	TmpSer = (CSer*)SerManager->Items[ci];
      for(int cj = 0; cj < TmpSer->Count; cj++)
      {
      	TmpGrpPset = (TGrpPset*)TmpSer->Items[cj];
         if((int)GetPhizX(TmpGrpPset->X) - 2 <= X &&
            (int)GetPhizY(TmpGrpPset->Y) - 2 <= Y &&
            (int)GetPhizX(TmpGrpPset->X) + 2 >= X &&
            (int)GetPhizY(TmpGrpPset->Y) + 2 >= Y)
         {
            TGPHTData GPHTData;
            GPHTData.X = TmpGrpPset->X;
            GPHTData.Y = TmpGrpPset->Y;
            GPHTData.SerName = TmpSer->Name;
            GrpPsetHitTest(GPHTData);
            NotData = false;
            HitTest = true;
            return;
			}
      }
   }
   if(NotData)
   {
   	if(OnReleaseHitTest) OnReleaseHitTest();
      HitTest = false;
   }
}
//---------------------------------------------------------------------------
void __fastcall CGrps::OnMU(TObject *Sender, TMouseButton Button,
                            TShiftState Shift, int X, int Y)
{
	if(PointLineActive)
   {
   	InvalidateRect(Handle, NULL, false);
      HintActive = true;
      ReleaseHC();
   	PointLineActive = false;
   }
}
//---------------------------------------------------------------------------
void __fastcall CGrps::OnMIPropertyClick(TObject *Sender)
{
	PropDlgExe();
}
//---------------------------------------------------------------------------
void __fastcall CGrps::WMPaint(TWMPaint &Message)
{
	PAINTSTRUCT ps;
   BeginPaint(Handle, &ps);
   HDC hdc = GetDC(Handle);
	Render(hdc);
   ReleaseDC(Handle, hdc);
   EndPaint(Handle, &ps);
}
//---------------------------------------------------------------------------
__fastcall CGrps::CGrps(Classes::TComponent *AOwner)
                 :TWinControl(AOwner)
{
   PointLineActive = false;
   HintActive = false;
   HitTest = false;
   CBFormat = 0;
   DW = 1.0;
   DH = 1.0;
   OffsetLogicCenter = PointLD(0.0, 0.0);
   LogicCenter = PointLD(0.0, 0.0);
   PhizCenter = PointLD(0.0, 0.0);
   CurrentXY = PointLD(0.0, 0.0);
   LogicReg = RectLD(0.0, 0.0, 0.0, 0.0);
   XVNames = RectLD(0.0, 0.0, 0.0, 0.0);
   YVNames = RectLD(0.0, 0.0, 0.0, 0.0);
   PhizReg = RectLD(0.0, 0.0, 200.0, 200.0);
   ZeroMemory(&GrpDesc, sizeof(TGrpDesc));
   GrpDesc.InvertX = false;
   GrpDesc.InvertY = false;
   GrpDesc.XAxVisible = true;
   GrpDesc.YAxVisible = true;
   GrpDesc.MainFisHeight = 4;
   GrpDesc.IntermedFisHeight = 2;
   GrpDesc.NumMainFis = 10;
   GrpDesc.NumIntermedFis = 5;
   GrpDesc.PropAcPage = 0;
   GrpDesc.SaveSers = false;
   GrpDesc.NumSers = 0;
   GrpDesc.AxisColor = clBlack;
   GrpDesc.BackColor = clWhite;
   GrpDesc.POFX = 0.0;
   GrpDesc.POFY = 0.0;
   BackBuf = NULL;
   BMXName = NULL;
   BMYName = NULL;
   SerManager = new CSerManager;
   ClipBoard = new CClipBoard;
   ClipBoard->RegisterFormat("CB_SER", CBFormat);

   const TMenuItem **items = new const TMenuItem*[1];
   items[0] = NewItem("��������", TextToShortCut(""), false, true, OnMIPropertyClick, 0, "MIProperty");
   PopupMenu = NewPopupMenu(this, "PMGrp", paLeft, true, items, 0);
   delete []items;

	//HW = new THintWindow(this);
   //HW->Parent = this;
   //HW->Color = clInfoBk;
   HW = NULL;

   SelSers = new TList;
   Parent = (TWinControl*)AOwner;
   ControlStyle << csOpaque;
   OnResize = OnGrpResize;
	OnReleaseHitTest = NULL;
	GPHTDataEvent = NULL;
   OnMouseMove = OnMM;
   OnMouseDown = OnMD;
   OnMouseUp = OnMU;
   CreateBuf(BackBuf, 200, 200);
}
//---------------------------------------------------------------------------
void __fastcall CGrps::AxisName(AnsiString XName, AnsiString YName,
                                AnsiString XResName, AnsiString YResName,
                                THandle HI)
{
	if(XResName == "")
   {
   	this->XName = XName;
      if(BMXName) BMXName->Free();
      BMXName = NULL;
   }
   else
   {
      BMXName = new Graphics::TBitmap;
      BMXName->LoadFromResourceName(HI, XResName);
      BMXName->Transparent = true;
      BMXName->TransparentColor = BMXName->Canvas->Pixels[0][0];
      this->XName = "";
   }
	if(YResName == "")
   {
   	this->YName = YName;
      if(BMYName) BMYName->Free();
      BMYName = NULL;
   }
   else
   {
      BMYName = new Graphics::TBitmap;
      BMYName->LoadFromResourceName(HI, YResName);
      BMYName->Transparent = true;
      BMYName->TransparentColor = BMYName->Canvas->Pixels[0][0];
      this->YName = "";
   }
}
//---------------------------------------------------------------------------
bool __fastcall CGrps::AddSer(CSer *Ser, AnsiString SerName)
{
	if(!SerManager->AddSer(Ser, SerName)) return false;
   if(GrpDesc.SaveSers)
   	SerManager->DelPrevSers(GrpDesc.NumSers);
   else
   	SerManager->DelPrevSers(0);
   CalcStMetrix();
   CalcDinMetrix();
   Draw();
   return true;
}
//---------------------------------------------------------------------------
bool __fastcall CGrps::CheckSers(void)
{
	if(!SerManager) return false;
	return SerManager->Count;
}
//---------------------------------------------------------------------------
bool __fastcall CGrps::CheckCBText(void)
{
   return ClipBoard->CheckFormat(CF_TEXT);
}
//---------------------------------------------------------------------------
bool __fastcall CGrps::CheckSelSers(void)
{
	if(!SelSers) return false;
   return SelSers->Count;
}
//---------------------------------------------------------------------------
bool __fastcall CGrps::CheckCBBitmap(void)
{
	return ClipBoard->CheckFormat(CF_BITMAP);
}
//---------------------------------------------------------------------------
void __fastcall CGrps::PropDlgExe(void)
{
	FProperty = new TFProperty(Application);
   FProperty->ChBxAXVisible->Checked = GrpDesc.XAxVisible;
   FProperty->ChBxAYVisible->Checked = GrpDesc.YAxVisible;
   FProperty->ChBxAXInvert->Checked = GrpDesc.InvertX;
   FProperty->ChBxAYInvert->Checked = GrpDesc.InvertY;
   FProperty->PCProp->ActivePageIndex = GrpDesc.PropAcPage;
   FProperty->EAxColor->Text =
   	FProperty->EAxColor->Text.sprintf("%X", GrpDesc.AxisColor);
   FProperty->EFColor->Text =
   	FProperty->EFColor->Text.sprintf("%X", GrpDesc.BackColor);
   FProperty->CBxSaveSers->Checked = GrpDesc.SaveSers;
   FProperty->CBxSaveSersClick(FProperty->CBxSaveSers);
   FProperty->ENumSers->Text = GrpDesc.NumSers;
   FProperty->ShowModal();
   if(FProperty->ResultOk)
   {
   	GrpDesc.XAxVisible = FProperty->ChBxAXVisible->Checked;
  	 	GrpDesc.YAxVisible = FProperty->ChBxAYVisible->Checked;
   	GrpDesc.InvertX = FProperty->ChBxAXInvert->Checked;
   	GrpDesc.InvertY = FProperty->ChBxAYInvert->Checked;
   	GrpDesc.PropAcPage = FProperty->PCProp->ActivePageIndex;
      GrpDesc.AxisColor =
      	(TColor)FProperty->StrHexToInt(FProperty->EAxColor->Text, 1);
      GrpDesc.BackColor =
      	(TColor)FProperty->StrHexToInt(FProperty->EFColor->Text, 1);
   	GrpDesc.SaveSers = FProperty->CBxSaveSers->Checked;
      GrpDesc.NumSers = FProperty->NumSers;
   }
   FProperty->Free();
   SerManager->DelPrevSers(GrpDesc.NumSers);
   CalcStMetrix();
   OnGrpResize(this);
}
//---------------------------------------------------------------------------
void __fastcall CGrps::Print(void)
{
}
//---------------------------------------------------------------------------
void __fastcall CGrps::ClearGrp(void)
{
   UnSelectSer();
	SerManager->DelAllSers();
   CalcStMetrix();
   CalcDinMetrix();
   Draw();
}
//---------------------------------------------------------------------------
void __fastcall CGrps::CopySelected(void)
{
   if(!ClipBoard->Open(Handle)) return;
	if(!ClipBoard->Empty()) return;
   AnsiString TextDataA = SerManager->GetTextData(SelSers);
   LPSTR TextData = TextDataA.c_str();
   if(!ClipBoard->SetTextData(CF_TEXT, TextData)) return;
   delete []TextData;
	if(!ClipBoard->Close()) return;
}
//---------------------------------------------------------------------------
void __fastcall CGrps::PastSers(void)
{
   if(!CheckCBText()) return;
   OpenClipboard(Handle);
   HANDLE hMem = GetClipboardData(CF_TEXT);
   if(!hMem){CloseClipboard(); return;}
   LPSTR TextData = (LPSTR)malloc(GlobalSize(hMem));
   if(!TextData){delete []TextData; return;}
   LPSTR MemAddr = (LPSTR)GlobalLock(hMem);
   strcpy(TextData, MemAddr);
	GlobalUnlock(hMem);
	CloseClipboard();
   AnsiString TextDataA = TextData;
   if(!SerManager->SetTextData(TextDataA))
   	MessageBoxA(Handle,
                  "�������� ������ ������",
                  "Clip Board Error",
                  MB_OK | MB_ICONERROR);
   delete []TextData;
}
//---------------------------------------------------------------------------
void __fastcall CGrps::DelSelected(void)
{
	if(!SelSers) return;
   SerManager->DelSers(SelSers);
   CalcStMetrix();
   CalcDinMetrix();
   UnSelectSer();
}
//---------------------------------------------------------------------------
void __fastcall CGrps::SelectAllSers(void)
{
	if(!SerManager->Count) return;
   for(int ci = 0; ci < SerManager->Count; ci++) SelectSer((CSer*)SerManager->Items[ci]);
}
//---------------------------------------------------------------------------
void __fastcall CGrps::ClearCB(void)
{
   if(!ClipBoard->Open(Handle)) return;
	if(!ClipBoard->Empty()) return;
	if(!ClipBoard->Close()) return;
}
//---------------------------------------------------------------------------
void __fastcall CGrps::CopyToCB(void)
{
   if(!ClipBoard->Open(Handle)) return;
	if(!ClipBoard->Empty()) return;
   if(!ClipBoard->SetDCData(CF_BITMAP, BackBuf, 0, 0, ClientWidth, ClientHeight)) return;
   AnsiString TextDataA = SerManager->GetTextData();
   LPSTR TextData = TextDataA.c_str();
   if(!ClipBoard->SetTextData(CF_TEXT, TextData)) return;
   //delete []TextData;
	if(!ClipBoard->Close()) return;
}
//---------------------------------------------------------------------------
__fastcall CGrps::~CGrps(void)
{
   if(BMXName) {delete BMXName; BMXName = NULL;}
   if(BMYName) {delete BMYName; BMYName = NULL;}
   if(BackBuf) {DeleteDC(BackBuf); BackBuf = NULL;}
   if(SelSers) {SelSers->Clear(); delete SelSers; SelSers = NULL;}
   if(SerManager) {delete SerManager; SerManager = NULL;}
   if(ClipBoard) {delete ClipBoard; ClipBoard = NULL;}
}
//---------------------------------------------------------------------------
