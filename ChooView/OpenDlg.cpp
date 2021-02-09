// OpenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChooView.h"
#include "OpenDlg.h"
#include "afxdialogex.h"
#include "ChildView.h"
#include "ModifyCategoryDlg.h"
#include "DBManager.h"
#include "DataBaseStruct.h"
class CChildView;

// COpenDlg dialog

IMPLEMENT_DYNAMIC(COpenDlg, CDialogEx)

	COpenDlg::COpenDlg(CWnd* pParent) :
		CDialogEx(COpenDlg::IDD, pParent),
		m_pParent(pParent)
{

}

COpenDlg::~COpenDlg()
{
}

void COpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(COpenDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &COpenDlg::OnTvnSelchangedTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &COpenDlg::OnNMDblclkTree1)
	ON_BN_CLICKED(IDC_BUTTON1, &COpenDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_Modify, &COpenDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_BUTTON2, &COpenDlg::OnBnClickedButton2)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, &COpenDlg::OnTvnItemexpandingTree1)
END_MESSAGE_MAP()


// COpenDlg message handlers


BOOL COpenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitTree();
	InitList();

	return TRUE;
}


int COpenDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void COpenDlg::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM selectedItem = m_TreeCtrl.GetSelectedItem();
	HTREEITEM child = m_TreeCtrl.GetChildItem(selectedItem);
	HTREEITEM parent = m_TreeCtrl.GetParentItem(selectedItem);
	m_ListCtrl.DeleteAllItems();
	//선택학 Item이 Category인 경우
	if (parent == NULL)
	{
		CString idxText = m_TreeCtrl.GetItemText(child);
		int flag = idxText.ReverseFind(_T(':'));
		int index = _ttoi(idxText.Mid(flag + 2));
		std::vector<int> ImageIdList;
		CDBManager::SelectImageIdInCategory(index, &ImageIdList);
		for (int i : ImageIdList)
		{
			AddMemoList(i);
		}
	}
	//선택한 Item이 Image인 경우
	else if (m_TreeCtrl.GetItemText(parent).Left(10) == _T("Image List"))
	{
		HTREEITEM imageID = m_TreeCtrl.GetNextSiblingItem(child);
		CString idxText = m_TreeCtrl.GetItemText(imageID);
		int flag = idxText.ReverseFind(_T(':'));
		int index = _ttoi(idxText.Mid(flag + 2));
		AddMemoList(index);
	}
}


void COpenDlg::OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	HTREEITEM selectedItem = m_TreeCtrl.GetSelectedItem();
	HTREEITEM child = m_TreeCtrl.GetChildItem(selectedItem);
	HTREEITEM root = m_TreeCtrl.GetParentItem(selectedItem);

	//선택학 Item이 Category인 경우
	if (root == NULL)
	{
		CString idxText = m_TreeCtrl.GetItemText(child);
		int flag = idxText.ReverseFind(_T(':'));
		int index = _ttoi(idxText.Mid(flag + 2));
		((CChildView*)m_pParent)->LoadCategory(index);
		OnOK();
	}
	//선택한 Item이 Image인 경우
	else if (m_TreeCtrl.GetItemText(root).Left(10) == _T("Image List"))
	{
		HTREEITEM imageID = m_TreeCtrl.GetNextSiblingItem(child);
		CString idxText = m_TreeCtrl.GetItemText(imageID);
		int flag = idxText.ReverseFind(_T(':'));
		int index = _ttoi(idxText.Mid(flag + 2));
		((CChildView*)m_pParent)->LoadImageInDataBase(index);
	}
}


void COpenDlg::OnBnClickedDelete()
{
	HTREEITEM selectedItem = m_TreeCtrl.GetSelectedItem();
	if (selectedItem == NULL)
	{
		return;
	}
	HTREEITEM child = m_TreeCtrl.GetChildItem(selectedItem);
	HTREEITEM root = m_TreeCtrl.GetParentItem(selectedItem);
	//선택된 Item이 Category인 경우
	if (root == NULL)
	{
		CString idxText = m_TreeCtrl.GetItemText(child);
		int flag = idxText.ReverseFind(_T(':'));
		int index = _ttoi(idxText.Mid(flag + 2));

		CChildView* childView = dynamic_cast<CChildView*>(m_pParent);
		if (childView->GetCategoryID() == index)
		{
			AfxMessageBox(_T("열려 있는 Category는 삭제할 수 없습니다."));
			return;
		}

		std::vector<int> imageList;
		CDBManager::SelectImageIdInCategory(index, &imageList);
		for (unsigned int i = 0; i < imageList.size(); i++)
		{
			std::vector<int> AnnotationList;
			CDBManager::SelectAnnotationIdInImage(imageList[i], &AnnotationList);
			for (unsigned int j = 0; j < AnnotationList.size(); j++)
			{
				CDBManager::DeletePointInAnnotation(AnnotationList[j]);
			}
			CDBManager::DeleteAnnotationInImage(imageList[i]);
			CDBManager::DeleteImageByID(imageList[i]);
		}
		CDBManager::DeleteCategoryById(index);
	}
	//선택된 Item이 Image인 경우
	else if (m_TreeCtrl.GetItemText(root) == _T("Image List"))
	{
		HTREEITEM imageID = m_TreeCtrl.GetNextSiblingItem(child);
		CString idxText = m_TreeCtrl.GetItemText(imageID);
		int flag = idxText.ReverseFind(_T(':'));
		int index = _ttoi(idxText.Mid(flag + 2));
		std::vector<int> AnnotationList;
		CDBManager::SelectAnnotationIdInImage(index, &AnnotationList);
		for (unsigned int i = 0; i < AnnotationList.size(); i++)
		{
			CDBManager::DeletePointInAnnotation(AnnotationList[i]);
		}
		CDBManager::DeleteAnnotationInImage(index);
		CDBManager::DeleteImageByID(index);
	}
	m_TreeCtrl.DeleteItem(selectedItem);
	m_ListCtrl.DeleteAllItems();
}

/**
@Tree 초기화
*/
void COpenDlg::InitTree()
{
	m_TreeCtrl.DeleteAllItems();
	std::vector<CategoryData> categoryList;
	CDBManager::SelectAllCategory(&categoryList);
	for (unsigned int i = 0; i < categoryList.size(); i++)
	{
		HTREEITEM root = m_TreeCtrl.InsertItem(categoryList[i].m_strCategoryName,
																				   0, 0, TVI_ROOT, TVI_LAST);

		CString strCategoryID;
		strCategoryID.Format(_T("CategoryID : %d"), categoryList[i].m_nCategoryID);
		m_TreeCtrl.InsertItem(strCategoryID, 0, 0, root, TVI_LAST);

		CString strCategoryComment;
		strCategoryComment.Format(_T("Category Comment : %s"),
														  categoryList[i].m_strComment);
		m_TreeCtrl.InsertItem(strCategoryComment, 0, 0, root, TVI_LAST);

		CString strCategoryCreateDate;
		strCategoryCreateDate.Format(_T("Create Date : %s"),
																 categoryList[i].m_strCreateDate);
		HTREEITEM CreateDateItem = m_TreeCtrl.InsertItem(strCategoryCreateDate,
																										 0, 0, root, TVI_LAST);
		std::vector<ImageData> imageList;
		CDBManager::SelectImageInCategory(categoryList[i].m_nCategoryID, &imageList);
		CString strImageList;
		strImageList.Format(_T("Image List(%d)"), imageList.size());
		HTREEITEM imageListItem = m_TreeCtrl.InsertItem(strImageList, 0, 0, root,
			TVI_LAST);
		for (unsigned int j = 0; j < imageList.size(); j++)
		{
			int flag = imageList[j].m_strFileName.ReverseFind(_T('\\'));
			CString fileName = imageList[j].m_strFileName.Mid(flag + 1);
			CString filePath = imageList[j].m_strFileName.Left(flag + 1);
			HTREEITEM fileNameItem = m_TreeCtrl.InsertItem(fileName, 1, 1, imageListItem,
																										 TVI_LAST);
			HTREEITEM filePathItem = m_TreeCtrl.InsertItem(filePath, 1, 1, fileNameItem,
																										 TVI_LAST);
			CString imageID;
			imageID.Format(_T("Image_ID : %d"), imageList[j].m_nImageID);
			HTREEITEM imageIDItem = m_TreeCtrl.InsertItem(imageID, 1, 1, fileNameItem,
																									  TVI_LAST);
		}
	}
}

/**
@List 초기화
*/
void COpenDlg::InitList()
{
	CRect rt;
	m_ListCtrl.GetWindowRect(&rt);
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrl.InsertColumn(0, _T("Text"), LVCFMT_LEFT,
												  static_cast<int>(rt.Width() * 0.5));
	m_ListCtrl.InsertColumn(1, _T("CreateDate"), LVCFMT_LEFT,
													static_cast<int>(rt.Width() * 0.25));
	m_ListCtrl.InsertColumn(2, _T("UpdateDate"), LVCFMT_LEFT,
												  static_cast<int>(rt.Width() * 0.25));
}


void COpenDlg::OnBnClickedModify()
{
	// TODO: Add your control notification handler code here
	HTREEITEM selectedItem = m_TreeCtrl.GetSelectedItem();
	HTREEITEM child = m_TreeCtrl.GetChildItem(selectedItem);
	HTREEITEM root = m_TreeCtrl.GetParentItem(selectedItem);
	if (root == NULL)
	{
		CString idxText = m_TreeCtrl.GetItemText(child);
		int flag = idxText.ReverseFind(_T(':'));
		int index = _ttoi(idxText.Mid(flag + 2));
		CategoryData result;
		if (!CDBManager::SelectCategoryById(index, &result))
		{
			return;
		}
		CModifyCategoryDlg dlg(index, result.m_strCategoryName, result.m_strComment);

		dlg.DoModal();
		InitTree();
		InitList();
	}
}


void COpenDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if (((CChildView*)m_pParent)->LoadImage())
	{
		OnOK();
	}
}

void COpenDlg::OnTvnItemexpandingTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CPoint p;
	GetCursorPos(&p);
	::ScreenToClient(m_TreeCtrl.m_hWnd, &p);
	HTREEITEM hitm = m_TreeCtrl.HitTest(p);
	if (m_TreeCtrl.GetChildItem(hitm) == NULL)
	{
		m_TreeCtrl.Select(m_TreeCtrl.GetParentItem(hitm), TVGN_CARET);
	}
	else
	{
		m_TreeCtrl.Select(hitm, TVGN_CARET);
	}
}

void COpenDlg::AddMemoList(const int& imageID)
{
	std::vector<AnnotationData> memoList;
	CDBManager::SelectMemoInImage(imageID, &memoList);

	for (unsigned int i = 0; i < memoList.size(); i++)
	{
		m_ListCtrl.InsertItem(0, memoList[i].m_strMemoString);
		m_ListCtrl.SetItemText(0, 1, memoList[i].m_strCreateDate);
		m_ListCtrl.SetItemText(0, 2, memoList[i].m_strUpdateDate);
	}
}

