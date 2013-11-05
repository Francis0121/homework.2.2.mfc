
// GameView.cpp : CGameView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"
#include "GameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameView

IMPLEMENT_DYNCREATE(CGameView, CView)

BEGIN_MESSAGE_MAP(CGameView, CView)
	// ǥ�� �μ� �����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CGameView ����/�Ҹ�

CGameView::CGameView()
{
	nMatchCount = 0;
}

CGameView::~CGameView()
{
}

BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CGameView �׸���

void CGameView::OnDraw(CDC* pDC)
{
	CGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	int nCount = 0;
	
	CDC memDC;
	CBitmap* pOldBmp;

	if(pDoc->m_bRandom){
		nMatchCount=0;
		pDoc->OnRandom();
		pDoc->m_bRandom = false;
	}

	// IDB_BITMAP1�� ����� ī���� ���� �Ͽ� nCount�� �ʱⰪ 1�� �ش�.

	memDC.CreateCompatibleDC(pDC);

	for(int n=0; n< pDoc->m_nRow;n++)
	{
		for(int m = 0; m<pDoc->m_nCol;m++){
			
			if(pDoc->m_bShow[n][m] == true){
				pDoc->m_bmp[n][m].LoadBitmap(IDB_BITMAP1 + pDoc->m_nRnd[nCount]);
				pOldBmp = memDC.SelectObject(&pDoc->m_bmp[n][m]);

				//���ʰ� ���ʿ� ������ �ֱ� ���ؼ� 1�� ���ߴ�.
				pDC->BitBlt(pDoc->m_bmCell.cx*(m+1), pDoc->m_bmCell.cy*(n+1), 
						pDoc->m_bmCell.cx, pDoc->m_bmCell.cy, &memDC, 0 ,0, SRCCOPY);
				pDoc->m_bmp[n][m].DeleteObject();
			}
			
			nCount++;

			if(pDoc->m_bShow[n][m] == false){
				CBitmap bmp;
				bmp.LoadBitmap(IDB_BITMAP1);
				pOldBmp = memDC.SelectObject(&bmp);
				pDC->BitBlt(pDoc->m_bmCell.cx*(m+1), pDoc->m_bmCell.cy*(n+1), 
					pDoc->m_bmCell.cx, pDoc->m_bmCell.cy, &memDC, 0 ,0, SRCCOPY);
				bmp.DeleteObject();
			}
		}
	}
	//memDC.SelectObject(pOldBmp);
	pDoc->ResizeWindow();
}


// CGameView �μ�

BOOL CGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CGameView ����

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CView::AssertValid();
}

void CGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameDoc* CGameView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameDoc)));
	return (CGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameView �޽��� ó����


void CGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CGameDoc* pDoc = GetDocument();

	//���� ���콺�� ������ �ִ� �ε���
	pDoc->m_nRowIndex = point.y / pDoc->m_bmCell.cy -1;
	pDoc->m_nColIndex = point.x / pDoc->m_bmCell.cx -1;

	//���� Ŭ���� ���� �ٽ� Ŭ��, �ι� �̻� Ŭ��, ���� �ۿ� Ŭ���ߴٸ� ����
	if(pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] == true
		|| pDoc ->m_nBmpSecondID != 0
		|| pDoc -> m_nRowIndex > pDoc->m_nRow -1
		|| pDoc -> m_nColIndex > pDoc->m_nCol -1
		|| pDoc -> m_nRowIndex < 0
		|| pDoc -> m_nColIndex < 0 )
		return;

	//���콺�� ������ �ִ� �͵��� ���� �׸��� ������
	pDoc ->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = true;

	CRect rect;
	SetRect(&rect, 
		pDoc->m_nRowIndex * (pDoc->m_bmCell.cx +1),
		pDoc->m_nColIndex * (pDoc->m_bmCell.cy +1),
		pDoc->m_nRowIndex * (pDoc->m_bmCell.cx +2),
		pDoc->m_nColIndex * (pDoc->m_bmCell.cy +2));
	InvalidateRect(rect);

	if(pDoc->m_nBmpFirstID == 0){
		pDoc->m_bMouse = true;

		pDoc->m_nBmpFirstID = pDoc->m_nBmpID[pDoc->m_nRowIndex][pDoc->m_nColIndex];
		
		m_nRowTempIndex = pDoc->m_nRowIndex;
		m_nColTempIndex = pDoc->m_nColIndex;
		
	}else{
		pDoc->m_bMouse = false;
		pDoc->m_nBmpSecondID = 
			pDoc->m_nBmpID[pDoc->m_nRowIndex][pDoc->m_nColIndex];
	
	}
	OnMatching();
	CView::OnLButtonDown(nFlags, point);
}



void CGameView::OnMatching(void){
	CGameDoc* pDoc = GetDocument();

	if(pDoc->m_nBmpFirstID == pDoc->m_nBmpSecondID && pDoc->m_bMouse == false){
		//Ŭ���̺�Ʈ �ʱ�ȭ
		pDoc->m_bShow[m_nRowTempIndex][m_nColTempIndex] = true;
		pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = true;
		pDoc->m_nBmpFirstID = pDoc -> m_nBmpSecondID = 0;
		m_nRowTempIndex = m_nColTempIndex =0;

		nMatchCount++;
	}else if(pDoc->m_nBmpFirstID != pDoc->m_nBmpSecondID && pDoc->m_bMouse == false){
		CDC* pDC= GetDC();
		CDC memDC;
		CBitmap bmp;

		memDC.CreateCompatibleDC(pDC);
		bmp.LoadBitmapW(IDB_BITMAP1 + pDoc->m_nBmpSecondID);

		CBitmap* pOldBmp = memDC.SelectObject(&bmp);
		pDC->BitBlt(
			pDoc->m_bmCell.cx*(pDoc->m_nColIndex+1),
			pDoc->m_bmCell.cy*(pDoc->m_nRowIndex+1), 
			pDoc->m_bmCell.cx, 
			pDoc->m_bmCell.cy, &memDC, 0 ,0, SRCCOPY);
		bmp.DeleteObject();
		memDC.SelectObject(pOldBmp);
		Sleep(400);

		pDoc->m_bShow[m_nRowTempIndex][m_nColTempIndex] = false;
		pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = false;
		pDoc->m_nBmpFirstID = pDoc -> m_nBmpSecondID = 0;
		m_nRowTempIndex = m_nColTempIndex =0;
	}

	Invalidate();

	if(nMatchCount == (pDoc->m_nRow*pDoc->m_nCol)/2){
		OnSuccess();	
	}
}

void CGameView::OnSuccess(void){
	int res = AfxMessageBox(_T("���ӿ���! �ٽý����Ͻðڽ��ϱ�?"), MB_YESNO);
	if(res == IDYES){
		CGameDoc* pDoc = GetDocument();
		pDoc->m_bRandom = true;

		for(int n=0; n<pDoc->m_nRow; n++){
			for(int m=0; m<pDoc->m_nCol; m++){
				pDoc->m_bShow[n][m] = false;	
			}
		}

		pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID =0;
		pDoc->m_bMouse = false;
		Invalidate();
	}
}
