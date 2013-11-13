
// GameView.cpp : CGameView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"
#include "GameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Dialog
#include "WelcomeDlg.h"
#include "StaticDlg.h"

// CGameView

IMPLEMENT_DYNCREATE(CGameView, CView)

	BEGIN_MESSAGE_MAP(CGameView, CView)
		// 표준 인쇄 명령입니다.
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
		ON_WM_LBUTTONDOWN()
		ON_WM_CREATE()
		ON_COMMAND(ID_GRADE_TOP, &CGameView::OnGradeTop)
		ON_COMMAND(ID_GRADE_MIDDEL, &CGameView::OnGradeMiddel)
		ON_COMMAND(ID_GRADE_LOW, &CGameView::OnGradeLow)
		ON_UPDATE_COMMAND_UI(ID_GRADE_TOP, &CGameView::OnUpdateGradeTop)
		ON_UPDATE_COMMAND_UI(ID_GRADE_MIDDEL, &CGameView::OnUpdateGradeMiddel)
		ON_UPDATE_COMMAND_UI(ID_GRADE_LOW, &CGameView::OnUpdateGradeLow)
		ON_WM_TIMER()
		ON_COMMAND(ID_NEW_GAME, &CGameView::OnNewGame)
		ON_COMMAND(ID_MODE_RANKING, &CGameView::OnModeRanking)
		ON_UPDATE_COMMAND_UI(ID_MODE_RANKING, &CGameView::OnUpdateModeRanking)
		ON_COMMAND(ID_STATICS_RANKING, &CGameView::OnStaticsRanking)
		ON_COMMAND(ID_TYPE_DOOLY, &CGameView::OnTypeDooly)
		ON_UPDATE_COMMAND_UI(ID_TYPE_DOOLY, &CGameView::OnUpdateTypeDooly)
		ON_COMMAND(ID_TYPE_POCKETMON, &CGameView::OnTypePocketmon)
		ON_UPDATE_COMMAND_UI(ID_TYPE_POCKETMON, &CGameView::OnUpdateTypePocketmon)
		ON_COMMAND(ID_TYPE_ONEPIECE, &CGameView::OnTypeOnepiece)
		ON_UPDATE_COMMAND_UI(ID_TYPE_ONEPIECE, &CGameView::OnUpdateTypeOnepiece)
	END_MESSAGE_MAP()

	// CGameView 생성/소멸

	CGameView::CGameView()
	{
		nMatchCount = 0;
		isStatus = READY;
		before = clock();
		m_strIntro= _T("");
	}

	CGameView::~CGameView()
	{
	}

	BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: CREATESTRUCT cs를 수정하여 여기에서
		//  Window 클래스 또는 스타일을 수정합니다.

		return CView::PreCreateWindow(cs);
	}

	// CGameView 그리기

	void CGameView::OnDraw(CDC* pDC)
	{
		CGameDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
		int nCount = 0;

		CDC memDC;
		CBitmap* pOldBmp;

		if(pDoc->m_bRandom){
			nMatchCount=0;
			pDoc->OnRandom();
			pDoc->m_bRandom = false;
		}

		// IDB_BITMAP20 이 노란색 카드라는 가정 하에 nCount에 초기값 1을 준다.

		memDC.CreateCompatibleDC(pDC);

		for(int n = 0; n < pDoc->m_nRow ; n++)
		{
			for(int m = 0; m < pDoc->m_nCol ; m++){

				if(pDoc->m_bShow[n][m] == true || isStatus == READY){
					pDoc->m_bmp[n][m].LoadBitmap(pDoc->GetType() + pDoc->m_nRnd[nCount]);
					pOldBmp = memDC.SelectObject(&pDoc->m_bmp[n][m]);

					pDC->BitBlt(
						pDoc->m_bmCell.cx*(m+1),
						pDoc->m_bmCell.cy*(n+1), 
						pDoc->m_bmCell.cx, 
						pDoc->m_bmCell.cy, &memDC, 0 ,0, SRCCOPY);
					pDoc->m_bmp[n][m].DeleteObject();
				}

				nCount++;

				if(pDoc->m_bShow[n][m] == false && isStatus != READY){
					CBitmap bmp;
					bmp.LoadBitmap(pDoc->GetType());
					pOldBmp = memDC.SelectObject(&bmp);
					pDC->BitBlt(
						pDoc->m_bmCell.cx*(m+1), 
						pDoc->m_bmCell.cy*(n+1), 
						pDoc->m_bmCell.cx, 
						pDoc->m_bmCell.cy, &memDC, 0 ,0, SRCCOPY);
					bmp.DeleteObject();
				}
			}
		}
		//memDC.SelectObject(pOldBmp);
		pDoc->ResizeWindow();

		pDC->TextOutW(100, 10, pDoc->GetName());
	}


	// CGameView 인쇄

	BOOL CGameView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// 기본적인 준비
		return DoPreparePrinting(pInfo);
	}

	void CGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
	}

	void CGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: 인쇄 후 정리 작업을 추가합니다.
	}


	// CGameView 진단

#ifdef _DEBUG
	void CGameView::AssertValid() const
	{
		CView::AssertValid();
	}

	void CGameView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	CGameDoc* CGameView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameDoc)));
		return (CGameDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CGameView 메시지 처리기


	void CGameView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if(isStatus == READY)
			return;

		CGameDoc* pDoc = GetDocument();

		//현재 마우스가 가지고 있는 인덱스
		pDoc->m_nRowIndex = point.y / pDoc->m_bmCell.cy -1;
		pDoc->m_nColIndex = point.x / pDoc->m_bmCell.cx -1;

		//이전 클릭한 곳에 다시 클릭, 두번 이상 클릭, 격자 밖에 클릭했다면 리턴
		if(pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] == true
			|| pDoc ->m_nBmpSecondID != 0
			|| pDoc -> m_nRowIndex > pDoc->m_nRow -1
			|| pDoc -> m_nColIndex > pDoc->m_nCol -1
			|| pDoc -> m_nRowIndex < 0
			|| pDoc -> m_nColIndex < 0 )
			return;

		//마우스가 가지고 있는 익덱스 값에 그림을 보여줌
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
			pDoc->m_nBmpSecondID = pDoc->m_nBmpID[pDoc->m_nRowIndex][pDoc->m_nColIndex];
		}
		OnMatching();
		CView::OnLButtonDown(nFlags, point);
	}



	void CGameView::OnMatching(void){
		CGameDoc* pDoc = GetDocument();

		if(pDoc->m_nBmpFirstID == pDoc->m_nBmpSecondID && pDoc->m_bMouse == false){
			//클릭이벤트 초기화
			pDoc->m_bShow[m_nRowTempIndex][m_nColTempIndex] = true;
			pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = true;
			pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID = 0;
			m_nRowTempIndex = m_nColTempIndex =0;

			nMatchCount++;
		}else if(pDoc->m_nBmpFirstID != pDoc->m_nBmpSecondID && pDoc->m_bMouse == false){
			CDC* pDC= GetDC();
			CDC memDC;
			CBitmap bmp;

			memDC.CreateCompatibleDC(pDC);
			bmp.LoadBitmapW(pDoc->GetType() + pDoc->m_nBmpSecondID);

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
			pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID = 0;
			m_nRowTempIndex = m_nColTempIndex = 0;
		}

		Invalidate();

		if(nMatchCount == (pDoc->m_nRow * pDoc->m_nCol)/2){
			OnSuccess();	
		}
	}

	void CGameView::OnSuccess(void){
		CGameDoc* pDoc = GetDocument();
		
		if(pDoc->GetMode() == PRACTICE){
			OnTimerStop();
			CString str;
			str.Format(_T("걸린시간은 %d:%d:%d:%d입니다.\n게임끝! 다시 시작하시겠습니까?"), 
				pDoc->GetHour(), pDoc->GetMinute(), pDoc->GetSecond(), pDoc->GetTimeset());
		
			int res = AfxMessageBox(str, MB_YESNO);
			if(res == IDYES){
				pDoc->InitializeGame();
				Invalidate();
			
				OnTimerReset();
				OnTimerStart();
			}
		}else{
			OnTimerStop();

			if(pDoc->m_nCol < 6){
				pDoc->m_nCol++;
				pDoc->m_nRow++;
				pDoc->InitializeGame();
				Invalidate();

				OnTimerReset();
				OnTimerStart();
			}else{
				CString str;
				str.Format(_T("걸린시간은 %d:%d:%d:%d입니다.\n게임끝! 다시 시작하시겠습니까?"), 
					pDoc->GetHour(), pDoc->GetMinute(), pDoc->GetSecond(), pDoc->GetTimeset());

				int res = AfxMessageBox(str, MB_YESNO);
				pDoc->OnWriteScoreFile();

				if(res == IDYES){
					pDoc->m_nCol = 3;
					pDoc->m_nRow = 2;
					pDoc->InitializeGame();
					Invalidate();

					OnTimerReset();
					OnTimerStart();
				}
			}
		}
	}

	void CGameView::OnTimerStart(void){
		before = clock();
		SetTimer(100, 100, NULL);
	}

	void CGameView::OnTimerStop(void){
		KillTimer(100);
	}

	void CGameView::OnTimerReset(void){
		isStatus = READY;
	}

	int CGameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  여기에 특수화된 작성 코드를 추가합니다.
		CWelcomeDlg dlg;
		int res = dlg.DoModal();

		CGameDoc *pDoc = GetDocument();	
		if(res == IDOK){
			pDoc->SetGrade(dlg.m_nGrade);
			pDoc->UpdateGrade();
			OnTimerStart();
		}else{
			pDoc->SetGrade(LOW); // 확인을 누르지 않는다면 무조건 LOW
			OnTimerStart();
		}

		pDoc->SetName(dlg.m_strName);
		pDoc->SetMode(dlg.m_nMode);
		if(dlg.m_nMode == RANKING){
			pDoc->m_nCol = 3;
			pDoc->m_nRow = 2;
		}

	}

	void CGameView::OnTimer(UINT_PTR nIDEvent){

		switch (isStatus)
		{
		case READY:
			IntroReady();
			break;
		case START:
			StopWatch();
			break;
		case END:
			break;
		}

		CView::OnTimer(nIDEvent);
	}
	
	void CGameView::IntroReady(void){
		double time = ((double) clock()-before) / CLOCKS_PER_SEC;
		

		if(time < 1){
			m_strIntro = _T("Ready");
		}else if(time < 2){
			m_strIntro = _T("3               ");
		}else if(time < 3){
			m_strIntro = _T("2");
		}else if(time < 4){
			m_strIntro = _T("1");
		}else{
			isStatus = START;
			m_strIntro = _T("    ");
			Invalidate();
		}		
		CClientDC dc(this);
		dc.TextOutW(100, 30, m_strIntro);
	}
	
	void CGameView::StopWatch(void){
		
		CGameDoc *pDoc = GetDocument();
		CString str;
		int hour = pDoc->GetHour();
		int minute = pDoc->GetMinute();
		int second = pDoc->GetSecond();
		int timeset = pDoc->GetTimeset();
		str.Format(_T("%d:%d:%d:%d"), hour, minute, second, ++timeset);
		pDoc->SetTimeset(timeset);

		if(minute == 60){
			pDoc->SetHour(++hour);
			pDoc->SetMinute(0);
		}

		if(second == 60){
			pDoc->SetMinute(++minute);
			pDoc->SetSecond(0);
		}

		if(timeset == 10){
			pDoc->SetSecond(++second);
			pDoc->SetTimeset(0);
		}

		CClientDC dc(this);
		dc.TextOutW(10, 10, str);
	}

	// ~ Menu Function
	void CGameView::OnNewGame(){
		CGameDoc *pDoc = GetDocument();
		if(pDoc->GetMode() == RANKING){
			pDoc->m_nRow = 2;
			pDoc->m_nCol = 3;
		}
		pDoc->InitializeGame();
		Invalidate();
		
		OnTimerStop();
		OnTimerReset();
		OnTimerStart();
	}
	
	void CGameView::OnStaticsRanking()
	{
		// Dialog 가 켜지면 Game Stop
		OnTimerStop();	
		CStaticDlg dlg;
		dlg.DoModal();
		
		// Dialog가 꺼지면 Game 다시 시작
		CGameDoc *pDoc = GetDocument();
		if(pDoc->GetMode() == RANKING){
			pDoc->m_nRow = 2;
			pDoc->m_nCol = 3;
		}
		pDoc->InitializeGame();

		Invalidate();
		OnTimerReset();
		OnTimerStart();
	}

	// Grade 난이도 선택 Menu Event
	void CGameView::OnModeRanking(){
		CGameDoc *pDoc = GetDocument();
		pDoc->SetMode(RANKING);
		pDoc->UpdateGrade();

		Invalidate();
		
		OnTimerStop();
		OnTimerReset();
		OnTimerStart();
	}

	void CGameView::OnGradeTop(){
		CGameDoc *pDoc = GetDocument();

		pDoc->SetMode(PRACTICE);
		pDoc->SetGrade(TOP);
		pDoc->UpdateGrade();

		Invalidate();
		
		OnTimerStop();
		OnTimerReset();
		OnTimerStart();
	}

	void CGameView::OnGradeMiddel(){
		CGameDoc *pDoc = GetDocument();

		pDoc->SetMode(PRACTICE);
		pDoc->SetGrade(MIDDLE);
		pDoc->UpdateGrade();

		Invalidate();

		OnTimerStop();
		OnTimerReset();
		OnTimerStart();
	}

	void CGameView::OnGradeLow(){
		CGameDoc *pDoc = GetDocument();

		pDoc->SetMode(PRACTICE);
		pDoc->SetGrade(LOW);
		pDoc->UpdateGrade();

		Invalidate();
		
		OnTimerStop();
		OnTimerReset();
		OnTimerStart();
	}

	// Grade 난이도 선택 Update 
	void CGameView::OnUpdateModeRanking(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		pCmdUI->SetCheck(pDoc->GetMode() == RANKING);
	}

	void CGameView::OnUpdateGradeTop(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		if(pDoc->GetMode() == PRACTICE)
			pCmdUI->SetCheck(pDoc->GetGrade() == TOP);
	}

	void CGameView::OnUpdateGradeMiddel(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		if(pDoc->GetMode() == PRACTICE)
			pCmdUI->SetCheck(pDoc->GetGrade() == MIDDLE);	
	}

	void CGameView::OnUpdateGradeLow(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		if(pDoc->GetMode() == PRACTICE)
			pCmdUI->SetCheck(pDoc->GetGrade() == LOW);
	}

	// 모양변경
	void CGameView::OnTypeDooly(){
		CGameDoc *pDoc = GetDocument();
		pDoc->SetType(DOOLY);
		
		Invalidate();
		
		OnTimerStop();
		OnTimerReset();
		OnTimerStart();
	}
	
	void CGameView::OnTypePocketmon(){
		CGameDoc *pDoc = GetDocument();
		pDoc->SetType(POCKETMON);

		Invalidate();
		
		OnTimerStop();
		OnTimerReset();
		OnTimerStart();
	}

	void CGameView::OnTypeOnepiece(){
		CGameDoc *pDoc = GetDocument();
		pDoc->SetType(ONEPIECE);

		Invalidate();
		
		OnTimerStop();
		OnTimerReset();
		OnTimerStart();
	}

	void CGameView::OnUpdateTypeDooly(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		pCmdUI->SetCheck(pDoc->GetType() == DOOLY);
	}

	void CGameView::OnUpdateTypePocketmon(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		pCmdUI->SetCheck(pDoc->GetType() == POCKETMON);
	}

	void CGameView::OnUpdateTypeOnepiece(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		pCmdUI->SetCheck(pDoc->GetType() == ONEPIECE);
	}
