/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/
#include "stdafx.h"
#include "FxBuddy.h"
#include "convert_charset.h"
#include "FxDatabase.h"

#ifdef WIN32_PLATFORM_WFSP
#include "Resourcesp.h"
#else
#include "Resourceppc.h"
#endif

#ifdef BUDDY_INFO_TABLE
extern CFxDatabase * g_pFxDB;
#endif

#define NoNameGroupID	-2
#define StrangerGroupID	-3

BuddyOpt::BuddyOpt(CTreeCtrl * widget)
{
	QunItem = NULL;
	markedCount = 0;
	treeWidget = widget;
	init_icon();
}

BuddyOpt::~BuddyOpt()
{
	freeAllGroupdata();    
}

void BuddyOpt::addItemToTree()
{
#ifdef BUDDY_INFO_TABLE
	g_pFxDB->ReadBuddyInfoBegin();
#endif
#if DEBUG_GUI
	addGroupToTree();
#else
	addGroupToTree();
	addAccountToTree();
	addQunToTree();
#endif
#ifdef BUDDY_INFO_TABLE
	g_pFxDB->ReadBuddyInfoEnd();
#endif
}

void BuddyOpt::freeAllGroupdata()
{
	HTREEITEM hItem = treeWidget->GetRootItem();
	while (hItem != NULL)
	{
		if (isQunItem(hItem))
			freeAllQundata(hItem);
		else
		{
			freeAllAccountdata(hItem);
			Group_Info *group_info =(Group_Info *)treeWidget->GetItemData(hItem);
			if( group_info) 
				delete group_info;
		}
		hItem = treeWidget->GetNextSiblingItem(hItem);
	}
}

void BuddyOpt::freeAllAccountdata(HTREEITEM groupItem)    
{
	if (!groupItem)
		return;
	HTREEITEM hItem = treeWidget->GetChildItem(groupItem);

	while (hItem != NULL)
	{
		Account_Info *ac_info =(Account_Info*)treeWidget->GetItemData(hItem);
		if (ac_info)
			delete ac_info;
		hItem = treeWidget->GetNextSiblingItem(hItem);
	}
}

void BuddyOpt::freeAllQundata(HTREEITEM groupItem)
{
	if (!groupItem)
		return;
	HTREEITEM hItem = treeWidget->GetChildItem(groupItem);

	while (hItem != NULL)
	{
		Qun_Info *qun_info = (Qun_Info*)treeWidget->GetItemData(hItem);
		if (qun_info)
			delete qun_info;
		hItem = treeWidget->GetNextSiblingItem(hItem);
	}
}

Account_Info *BuddyOpt::fetchNoUpdateAccount()
{
	HTREEITEM groupItem = treeWidget->GetRootItem();
	CString strAccountID;
	while (groupItem != NULL)
	{
		if (!isQunItem(groupItem)) //not the qun item
		{
			HTREEITEM hItem = treeWidget->GetChildItem(groupItem);
			while (hItem != NULL)
			{
				Account_Info *ac_info =(Account_Info*)treeWidget->GetItemData(hItem);
				if (ac_info && ac_info->isUpdate == FALSE)
				{
					if(fx_is_pc_user_by_id(ac_info->accountID))
					{
						strAccountID.Format(_T("%d"), ac_info->accountID);
						if(0 == strAccountID.Compare(ac_info->accountName))
						{
							//优先更新只显示出飞信ID的好友
							return ac_info;
						}
					}
					else
					{
						//手机好友不需要更新
						ac_info->isUpdate = TRUE;
					}
				}
				hItem = treeWidget->GetNextSiblingItem(hItem);
			}

		}
		groupItem = treeWidget->GetNextSiblingItem(groupItem);
	}
	groupItem = treeWidget->GetRootItem();
	while (groupItem != NULL)
	{
		if (!isQunItem(groupItem)) //not the qun item
		{
			HTREEITEM hItem = treeWidget->GetChildItem(groupItem);
			while (hItem != NULL)
			{
				Account_Info *ac_info =(Account_Info*)treeWidget->GetItemData(hItem);
				if (ac_info && ac_info->isUpdate == FALSE)
				{
					if(fx_is_pc_user_by_id(ac_info->accountID))
					{
						return ac_info;
					}
					else
					{
						//手机好友不需要更新
						ac_info->isUpdate = TRUE;
					}
				}
				hItem = treeWidget->GetNextSiblingItem(hItem);
			}

		}
		groupItem = treeWidget->GetNextSiblingItem(groupItem);
	}
	return NULL;
}

void BuddyOpt::setHaveUpdateAccount(Account_Info *ac_info)
{
	if (!ac_info)
		return;
	ac_info->isUpdate = TRUE;
}

void BuddyOpt::addQunToTree()
{
	Fetion_Qun *qun = NULL;
	Qun_Info *qun_info = NULL;

	DList *tmp_qun = (DList *)fx_get_qun();
	while(tmp_qun)
	{
		qun = (Fetion_Qun *) tmp_qun->data;
		if(qun) {
			if (!QunItem) {					
				//notify: set the qun icon
				QunItem = treeWidget->InsertItem(_T("飞信群"), I_QUN, I_QUN);
				treeWidget->SortChildren(TVI_ROOT);
			}

			qun_info = new Qun_Info;
			char * qun_name = fx_get_qun_show_name(qun); 
			qun_info->qunName = ConvertUtf8ToUtf16(qun_name);
			if(qun_name)
				free(qun_name);
			qun_info->qunID = qun->id; 

			HTREEITEM item = treeWidget->InsertItem(qun_info->qunName, I_QUN, I_QUN, QunItem);
			treeWidget->SetItemData(item,(DWORD)qun_info); 
			treeWidget->SortChildren(QunItem);
		}
		tmp_qun = d_list_next(tmp_qun);
	}
}

BOOL BuddyOpt::isQunItem(HTREEITEM item) 
{
	if(QunItem)
		return item == QunItem;
	else
		return false;
}

//add Group info to tree widget
void BuddyOpt::addGroupToTree()
{
#if DEBUG_GUI
	HTREEITEM  item = treeWidget->InsertItem(CString("aaa"), I_QUN, I_QUN);
	 
	Account_Info *ac_info = new Account_Info;
	ac_info->accountName = _T("ddd");
	ac_info->accountID = 123456;
	ac_info->onlinestate = 1;

	//notify: set the group icon
	HTREEITEM accountItem = treeWidget->InsertItem(CString("bbb"), I_QUN, 0, item);
	treeWidget->SetItemData(accountItem,(DWORD)ac_info); 

	setOnlineState(accountItem, 1);
	 treeWidget->InsertItem(CString("aaabbb"), 0, 0, item);
	 treeWidget->InsertItem(CString("ccc"), 1, 0);
	 treeWidget->InsertItem(CString("ddd"), 1, 0);
	 treeWidget->InsertItem(CString("aaabbb"), 0, 0, item);
	 treeWidget->InsertItem(CString("eee"), 1, 0);
	 treeWidget->InsertItem(CString("eee"), 1, 0);
#if 0
	 treeWidget->InsertItem(CString("eee"), 1, 0);
	 treeWidget->InsertItem(CString("eee"), 1, 0);
	 treeWidget->InsertItem(CString("eee"), 1, 0);
	 treeWidget->InsertItem(CString("eee"), 1, 0);
	 treeWidget->InsertItem(CString("eee"), 1, 0);
	 treeWidget->InsertItem(CString("eee"), 1, 0);
#endif
	 item = treeWidget->InsertItem(CString("eee"), 1, 0);
	 treeWidget->InsertItem(CString("bbb"), I_QUN, 0, item);
	 treeWidget->InsertItem(CString("bbb"), I_QUN, 0, item);
#if 0
	//treeWidget->SetItemData(item,(DWORD)groupinfo); 
	 HTREEITEM  RootItem = treeWidget->GetRootItem( );
	 if (!RootItem)
	 {
		 ::AfxMessageBox(_T("no root item"));
		 return;
	 } else
	 {
		 if (RootItem == item)
			 ::AfxMessageBox(_T("root item is item"));
	 }

	HTREEITEM hItem1 = treeWidget->GetNextSiblingItem(RootItem);
	 while (hItem1 != NULL)
	{
		::AfxMessageBox(treeWidget->GetItemText(hItem1));
		hItem1 = treeWidget->GetNextSiblingItem(hItem1);
	}

	HTREEITEM hItem = treeWidget->GetChildItem(RootItem);

	while (hItem != NULL)
	{
		::AfxMessageBox(treeWidget->GetItemText(hItem));
		hItem = treeWidget->GetNextSiblingItem(hItem);
	}
#endif
#else
	Group_Info *groupinfo = NULL; 
	Fetion_Group *group = NULL;

	DList *tmp_group = (DList *)fx_get_group();
	while(tmp_group)
	{
		group = (Fetion_Group *) tmp_group->data;
		if(group) {
			CString str = ConvertUtf8ToUtf16(group->name);		
			groupinfo = new Group_Info;
			groupinfo->groupName = str;
			groupinfo->groupID = group->id;
			groupinfo->online_no = 0;

			//notify: set the group icon
			HTREEITEM item = treeWidget->InsertItem(str, I_QUN, I_QUN);
			treeWidget->SetItemData(item,(DWORD)groupinfo); 
			treeWidget->SortChildren(TVI_ROOT);
		}
		tmp_group = d_list_next(tmp_group);
	}
#endif
}

void BuddyOpt::addGroup(const char* groupname, long id)
{
	Group_Info *groupinfo = new Group_Info;
	groupinfo->groupName = ConvertUtf8ToUtf16((char*)groupname);
	groupinfo->groupID = id;
	groupinfo->online_no = 0;

	//notify: set the group icon
	HTREEITEM item = treeWidget->InsertItem(groupinfo->groupName + _T("(0/0)"), I_QUN, I_QUN);
	treeWidget->SetItemData(item,(DWORD)groupinfo);
	treeWidget->SortChildren(TVI_ROOT);
}

//add account info to tree widget
void BuddyOpt::addAccountToTree()
{
	const Fetion_Account *account = fx_get_first_account();
	while(account)
	{
		addAccountToGroup(account);
		account = fx_get_next_account(account);
	}
}

//add account to it's group item 
void BuddyOpt::addAccountToGroup(const Fetion_Account *account)  
{
	//remove the user's id on account from list...
	if (!account || account->id == (long)strtol(fx_get_usr_uid(), NULL, 10))
		return;

	int	group_no = fx_get_account_group_id(account) ;
	if(group_no < 0)
	{
		group_no = NoNameGroupID;
		if(fx_is_InChatlist_by_account(account))
		{
			group_no = StrangerGroupID;
		}
	}
	HTREEITEM groupItem = findGroupItemByID(group_no);
	if(!groupItem)
	{
		CString strGroupName;
		if(StrangerGroupID == group_no)
		{
			strGroupName = _T("陌生人");
		}
		else
		{
			strGroupName = _T("未分组");
		}
		Group_Info *groupinfo = new Group_Info;
		groupinfo->groupName = strGroupName;
		groupinfo->groupID = group_no;
		groupinfo->online_no = 0;
		HTREEITEM item = treeWidget->InsertItem(strGroupName, I_QUN, I_QUN);
		treeWidget->SetItemData(item,(DWORD)groupinfo); 
		treeWidget->SortChildren(TVI_ROOT);
	}

	BOOL bFromDB = FALSE;
#ifdef BUDDY_INFO_TABLE
	BUDDYINFODB BuddyInfo;
	BuddyInfo.lID = account->id;
	g_pFxDB->ReadBuddyInfo(&BuddyInfo);
	if(!BuddyInfo.strNickName.IsEmpty())
	{
		fx_set_buddy_nickname_ex(account->id, ConvertUtf16ToUtf8(BuddyInfo.strNickName));
		bFromDB = TRUE;
	}
	if(!BuddyInfo.strImpresa.IsEmpty())
	{
		fx_set_buddy_impresa_ex(account->id, ConvertUtf16ToUtf8(BuddyInfo.strImpresa));
		bFromDB = TRUE;
	}
#endif

	char *showname = fx_get_account_show_name_with_state(account, TRUE, TRUE);
	CString show_name = ConvertUtf8ToUtf16(showname);
	int online_state = fx_get_online_status_by_account(account);

	addAccountToGroup(account, show_name, online_state, group_no, bFromDB);

	if(showname)
		free(showname);
}

void BuddyOpt::addAccountToGroup(const Fetion_Account *account, CString & name, int online_state, int group_id, BOOL bFromDB)
{
	//remove the user's id on account from list... fixed: it maybe have redundance
	if (!account || account->id == (long)strtol(fx_get_usr_uid(), NULL, 10))
		return;

	HTREEITEM groupItem = findGroupItemByID(group_id);
	if (!groupItem)
		return;

	Account_Info *ac_info = new Account_Info;
	ac_info->accountName = name;
	ac_info->accountID = account->id;
	ac_info->bFromDB = bFromDB;

	if(bFromDB)
	{
		ac_info->isUpdate = TRUE;
	}
	else
	{
		ac_info->isUpdate = FALSE;
	}
	ac_info->BuddyInfo.lID = account->id;
	ac_info->BuddyInfo.strLocalName =  ConvertUtf8ToUtf16(account->local_name);
	if(account->personal)
	{
		ac_info->BuddyInfo.strNickName = ConvertUtf8ToUtf16(account->personal->nickname);
		ac_info->BuddyInfo.strImpresa = ConvertUtf8ToUtf16(account->personal->impresa);
	}
	else
	{
		ac_info->BuddyInfo.strNickName = _T("");
		ac_info->BuddyInfo.strImpresa = _T("");
	}
     
	ac_info->onlinestate = online_state;

	//notify: set the group icon
	HTREEITEM accountItem = treeWidget->InsertItem(name + _T("  "), 0, 0, groupItem);
	treeWidget->SetItemData(accountItem,(DWORD)ac_info); 
	setOnlineState(accountItem, ac_info->onlinestate);

//	setTipsOfAccount(accountItem, account);

	Group_Info *group_info = (Group_Info *)treeWidget->GetItemData(groupItem);
	if (!group_info)
		return;

	if (fx_is_on_line_by_account (account))
	{
		//set the online to frist
		//groupItem->removeChild(accountItem);
		//groupItem->insertChild(group_info->online_no, accountItem);
		group_info->online_no ++;
	}
    updateGroupInfo(groupItem, true);
	//CString online;
	//online.Format(_T("(%d"),group_info->online_no);
	//CString BuddyCount;
	//BuddyCount.Format(_T("/%d)"), GetChildCount(groupItem));
	//CString groupShowName = group_info->groupName + online + BuddyCount;

	//treeWidget->SetItemText(groupItem, groupShowName);
	//treeWidget->Expand(groupItem, TVE_EXPAND);
}

HTREEITEM BuddyOpt::findGroupItemByID(int group_id)
{
	HTREEITEM  hItem = treeWidget->GetRootItem( );

	while (hItem != NULL)
	{
		Group_Info *group_info = (Group_Info *)treeWidget->GetItemData(hItem);
		if (group_info && group_id == group_info->groupID)
			return hItem;
		hItem = treeWidget->GetNextSiblingItem(hItem);
	}
	return NULL;
}

HTREEITEM BuddyOpt::findAccountItemFromAllGroup(const Fetion_Account *account)
{
	if (!account)
		return NULL;

	long account_id = (long)account->id;

	HTREEITEM hGroupItem = treeWidget->GetRootItem();
    while(hGroupItem != NULL)
    {
		if (!isQunItem(hGroupItem)) //not the qun item
		{
			HTREEITEM hItem = treeWidget->GetChildItem(hGroupItem);
			while (hItem != NULL)
			{
				Account_Info *ac_info =(Account_Info*)treeWidget->GetItemData(hItem);
				if(ac_info && account_id == ac_info->accountID)
					return hItem;
				hItem = treeWidget->GetNextSiblingItem(hItem);
			}
		}
		hGroupItem = treeWidget->GetNextSiblingItem(hGroupItem);
    }
	return NULL;
}

void BuddyOpt::WantUpdateAllAccountInfo()
{
	HTREEITEM hGroupItem = treeWidget->GetRootItem();
    while(hGroupItem != NULL)
    {
		if (!isQunItem(hGroupItem)) //not the qun item
		{
			HTREEITEM hItem = treeWidget->GetChildItem(hGroupItem);
			while (hItem != NULL)
			{
				Account_Info *ac_info =(Account_Info*)treeWidget->GetItemData(hItem);
				if(ac_info)
				{
					ac_info->isUpdate = FALSE;
				}
				hItem = treeWidget->GetNextSiblingItem(hItem);
			}
		}
		hGroupItem = treeWidget->GetNextSiblingItem(hGroupItem);
    }
}

//this function will add to libfetion impl...
// return false should not changed
// true should changed
// state is 0 sub the online number
//       is 1 add the online number
BOOL BuddyOpt::isOnlineStateChanged(int old_state, int new_state, int* state)
{
	if(old_state == new_state)
		return false;

	if (fx_is_online_status(old_state))//old_state is online state
	{
		if( !fx_is_online_status(new_state)) { //new state is offline state
			*state = 0;
			return true;
		}
	} else { //old_state is offline state
		if( fx_is_online_status(new_state)) { //new state is online state
			*state = 1;
			return true;
		}
	}
	return false;
}

void BuddyOpt::updateAccountInfo(long account_id)
{
	const Fetion_Account * account = fx_get_account_by_id (account_id);
	if (!account)
		return;

	HTREEITEM accountItem = findAccountItemFromAllGroup(account);

	//not find this account, so add it to Group...
	if (!accountItem) 
	{
		printf("not find the item , i will add to the group\n");
		addAccountToGroup (account);

		accountItem = findAccountItemFromAllGroup(account);
		if (!accountItem) 
		{
			return;
		}
	}

	//update the account info
	//setTipsOfAccount(accountItem, account);

	char * showname = fx_get_account_show_name_with_state(account, TRUE, TRUE);
	CString show_name = ConvertUtf8ToUtf16(showname);
	if(showname)
		free(showname);

	Account_Info *ac_info =(Account_Info*)treeWidget->GetItemData(accountItem);

	if(!ac_info)
		return;

	BUDDYINFODB BuddyInfo;
#if 0
	BuddyInfo.lID = account->id;
	BuddyInfo.strLocalName =  ConvertUtf8ToUtf16(account->local_name);
	if(account->personal)
	{
		BuddyInfo.strNickName = ConvertUtf8ToUtf16(account->personal->nickname);
		BuddyInfo.strImpresa = ConvertUtf8ToUtf16(account->personal->impresa);
	}
	else
	{
		BuddyInfo.strNickName = _T("");
		BuddyInfo.strImpresa = _T("");
	}
#endif
	if((!ac_info->bFromDB) || ChangeBuddyInfo(&ac_info->BuddyInfo, &BuddyInfo))
	{
		ac_info->bFromDB = TRUE;
#ifdef BUDDY_INFO_TABLE
		if(!g_pFxDB->UpdateBuddyInfo(&BuddyInfo))
		{
			//假如写失败
		}
#endif
	}
	ac_info->accountName = show_name;

	int old_online_state = ac_info->onlinestate;
	int new_online_state = fx_get_online_status_by_account(account);
    
	ac_info->onlinestate = new_online_state;

	CString old_show_name = treeWidget->GetItemText(accountItem);
	show_name += _T("  ");
	if (old_show_name.CompareNoCase(show_name))
	{
		treeWidget->SetItemText(accountItem, show_name);
	}
	setOnlineState(accountItem, ac_info->onlinestate);

	if (old_show_name.CompareNoCase(show_name) || (old_online_state != new_online_state))
	{
		HTREEITEM groupItem = treeWidget->GetParentItem(accountItem);
		if(groupItem)
		{
			Group_Info *group_info =(Group_Info *)treeWidget->GetItemData(groupItem);
			if(!group_info)
				return ;
			int state = 0;
			if(isOnlineStateChanged(old_online_state , new_online_state , &state))
			{
				if (state)			
					group_info->online_no ++;
				else 
					group_info->online_no --;
			}
			updateGroupInfo(groupItem, true);
		}
	}
	//printf("Updata new buddy.... \n");
}

void BuddyOpt::delAccount_direct(long uid)
{
	const Fetion_Account * account = fx_get_account_by_id (uid);
    HTREEITEM  hItem = findAccountItemFromAllGroup(account);

    if(hItem != NULL)
    {
        HTREEITEM groupItem = treeWidget->GetParentItem(hItem);
        Group_Info *group_info =(Group_Info *)treeWidget->GetItemData(groupItem);

	    if(!group_info)
		    return ;

        treeWidget->DeleteItem(hItem);

	    if (fx_is_on_line_by_account (account))
	    {
		    group_info->online_no --;
	    }

        updateGroupInfo(groupItem, true);
    }

}

#if 0
void BuddyOpt::delAccount(HTREEITEM accountItem)  
{
	if (!accountItem)
		return;

#if MS_VC6
	Account_Info *ac_info =(Account_Info*)(accountItem->data(0, Qt::UserRole).toUInt());
#else
	Account_Info *ac_info =accountItem->data(0, Qt::UserRole).value<Account_Info*>() ;
#endif
	if (!ac_info)
		return;

	const Fetion_Account * account = fx_get_account_by_id (ac_info->accountID);

	if (ac_info )	
		delete ac_info;


	HTREEITEM groupItem = accountItem->parent();
	if (!groupItem)
		return;

	groupItem->removeChild(accountItem);

	{
#if MS_VC6
		Group_Info *group_info =(Group_Info *)( groupItem->data(0, Qt::UserRole).toUInt() );
#else
		Group_Info *group_info = groupItem->data(0, Qt::UserRole).value<Group_Info *>();
#endif
		if (!group_info)
			return ;
		if (fx_is_on_line_by_account(account))
			group_info->online_no --;

		CString online;
		online.Format(_T("(%d"),group_info->online_no);
		CString groupShowName = group_info->groupName + online + _T("在线)");		
		groupItem->setText(0, groupShowName);
	}

}

void BuddyOpt::delAccount(long uid)  
{
	const Fetion_Account * account = fx_get_account_by_id (uid);
	if(!account)  return;

	delAccount(findAccountItem(account)); 
}

void BuddyOpt::delGroup(long id)  
{
	HTREEITEM hItem = treeWidget->GetRootItem();
	while (hItem != NULL)
	{
		if (!isQunItem(hItem))
		{
			Group_Info *group_info =(Group_Info *)treeWidget->GetItemData(hItem);
			if( group_info && group_info->groupID == id) 
			{
				//free all account data of this group
				freeAllAccountdata(hItem);    
				delete group_info;
				treeWidget->DeleteItem(hItem);  
				return;
			}
		}

		hItem = treeWidget->GetNextSiblingItem(hItem);
	}
}
#endif

int BuddyOpt::init_icon(void)
{
#if 0
	//CBitmap pBitmap[13]; 
	m_imagelist.Create(IDB_BITMAP2, 16, 0, RGB(0,128,128));
	treeWidget->SetImageList (&m_imagelist, TVSIL_NORMAL); 
#else
	//CBitmap pBitmap[13]; 
	
	//m_imagelist.Create(IDB_BITMAP2, 16, 0, RGB(0,128,128));
	//m_imagelist.Create(13,13,ILC_COLORDDB,13,18); 
	//m_imagelist.Create(16,16,0,13,18); 
	//m_imagelist.SetBkColor (RGB(1,1,1));

	m_imagelist.Create(16,16,0,20,21);
	pBitmap[I_OFFLINE].LoadBitmapW(IDB_BITMAP_OFFLINE);
	pBitmap[I_BLACK].LoadBitmapW(IDB_BITMAP_BLACK);
	pBitmap[I_MOBILE].LoadBitmapW(IDB_BITMAP_MOBILE);
	pBitmap[I_WAITING_AUTH].LoadBitmapW(IDB_BITMAP_WAIT);
	pBitmap[I_REFUSE].LoadBitmapW(IDB_BITMAP_REFUSE);
	pBitmap[I_DINNER].LoadBitmapW(IDB_BITMAP_DINNER);
	pBitmap[I_AWAY].LoadBitmapW(IDB_BITMAP_AWAY);
	pBitmap[I_ONLINE].LoadBitmapW(IDB_BITMAP_ONLINE);
	pBitmap[I_PHONE].LoadBitmapW(IDB_BITMAP_PHONE);
	pBitmap[I_BUSY].LoadBitmapW(IDB_BITMAP_BUSY);
	pBitmap[I_MEETING].LoadBitmapW(IDB_BITMAP_MEET); 
	pBitmap[I_EXAWAY].LoadBitmapW(IDB_BITMAP_EXAWAY);
	pBitmap[I_NUM_PRIMITIVES].LoadBitmapW(IDB_BITMAP_NUMP);
	pBitmap[I_ONLINE_M].LoadBitmapW(IDB_BITMAP_ONLINE_M);
	pBitmap[I_AWAY_M].LoadBitmapW(IDB_BITMAP_AWAY_M);
	pBitmap[I_BUSY_M].LoadBitmapW(IDB_BITMAP_BUSY_M);
	pBitmap[I_QUN].LoadBitmapW(IDB_BITMAP_QUN);
	pBitmap[I_FLICK].LoadBitmapW(IDB_BITMAP_FLICK);
	pBitmap[I_REFUSE_SMS].LoadBitmapW(IDB_BITMAP_REFUSE_SMS);
	
	for(int i = 0; i < I_END; i++) 
		m_imagelist.Add(&pBitmap[i], RGB(255,255,255)); 
	treeWidget->SetImageList (&m_imagelist, TVSIL_NORMAL); 
#endif
	return 1;
}

int BuddyOpt::getOnlineIcon(int flag)
{
	int res = 0;
	switch(flag)
	{
	case 0:  //pc offline
	case FX_STATUS_OFFLINE:
		res = I_OFFLINE;
		break;
	case FX_STATUS_BLACK:
		res = I_BLACK;
		break;
	case FX_STATUS_MOBILE: //mobile user
		res = I_MOBILE;
		break;
	case FX_STATUS_WAITING_AUTH:  //the account waiting   
		res = I_WAITING_AUTH;
		break;
	case FX_STATUS_REFUSE:   //the account is refuse make friends with you
		res = I_REFUSE;
		break;
	case FX_STATUS_DINNER:
		res = I_DINNER;
		break;
	case FX_STATUS_AWAY:
		res = I_AWAY;
		break;
	case FX_STATUS_ONLINE:
		res = I_ONLINE;
		break;
	case FX_STATUS_PHONE:
		res = I_PHONE;
		break;
	case FX_STATUS_BUSY:
		res = I_BUSY;
		break;
	case FX_STATUS_MEETING :
		res = I_MEETING;
		break;
	case FX_STATUS_EXTENDED_AWAY:
		res = I_EXAWAY;
		break;
    case FX_STATUS_UNSET:
	case FX_STATUS_NUM_PRIMITIVES:
		res = I_NUM_PRIMITIVES;
		break;

	case FX_STATUS_MOBILE_OUT_OF_SERIVCE:
	case FX_STATUS_CLOSE_FETION_SERIVCE:
		res = I_REFUSE;
		break;
	case FX_STATUS_MOBILE_ONLINE:
		res = I_ONLINE_M;
		break;
	case FX_STATUS_MOBILE_DINNER:
	case FX_STATUS_MOBILE_AWAY:
	case FX_STATUS_MOBILE_EXTENDED_AWAY:
	case FX_STATUS_MOBILE_NUM_PRIMITIVES:
		res = I_AWAY_M;
		break;
	case FX_STATUS_MOBILE_PHONE:
	case FX_STATUS_MOBILE_MEETING:
	case FX_STATUS_MOBILE_BUSY:
		res = I_BUSY_M;
		break;
	case FX_STATUS_REFUSE_SMS:
		res = I_REFUSE_SMS;
		break;
	default:
		break;
	}
	return res;
}

void BuddyOpt::setOnlineState(HTREEITEM hItem)
{
	if(!hItem)
		return;
	Account_Info *ac_info =(Account_Info*)treeWidget->GetItemData(hItem);
	if(!ac_info)
		return;
	setOnlineState(hItem, ac_info->onlinestate);
}

void BuddyOpt::setOnlineState(HTREEITEM hItem, int flag)
{
	if(!hItem)
		return;
	int res = getOnlineIcon(flag);
	treeWidget->SetItemImage(hItem, res, res);
}


#if 0
CString BuddyOpt::createAccountTipsInfo(const Fetion_Account *account)
{
	CString tips;
	CString tmp;
	BOOL hP = false;
	if(account->personal)
		hP = true;

	CString info;
	info += tr("mobile_no:");
	if(hP)
	{
		tmp = ConvertUtf8ToUtf16(account->personal->mobile_no);
		if (!tmp.isEmpty())
		{
			info += "<b style=\"color:red; \">" + tmp +"</b>";
			tips += info +"<br>";
		}
	}
	else 
	{
		if (!fx_is_pc_user_by_account(account)){
			long head, body;
			int prefix = 0;
			char mobile_no[12];
			sprintf(mobile_no, "%d%08ld", prefix, body);
			mobile_no[11] = '\0';
			info += "<b style=\"color:red; \">"+CString(mobile_no) + "</b>"; 
		}

		info += "<b style=\"color:red; \"> </b>"; 
	tips += info +"<br>";
	}


	if (fx_is_pc_user_by_account(account)){
		info = tr("fetion_no:");
		info += "<b style=\"color:red; \">"+CString("%1").arg(account->id)+"</b>";
		tips += info +"<br>";
	}

	if(hP)
	{
		tmp = ConvertUtf8ToUtf16(account->personal->nickname);
		if (!tmp.isEmpty())
		{
			info = tr("nickname:");
			info += "<b style=\"color:red; \">" + tmp +"</b>";
			tips += info +"<br>";
		}

		tmp = ConvertUtf8ToUtf16(account->personal->name);
		if (!tmp.isEmpty())
		{
			info = tr("name:");
			info += "<b style=\"color:red; \">" + tmp +"</b>";
			tips += info +"<br>";
		}

		info = tr("gender:");
		switch(account->personal->gender)
		{
			case 2:
				info += "<b style=\"color:red; \">" + tr("girl") +"</b>";
				tips += info +"<br>";
				break;
			case 1:
				info += "<b style=\"color:red; \">" + tr("boy") +"</b>";
				tips += info +"<br>";
				break;
			case 0:
				info += "<b style=\"color:red; \">" + tr("unknow") +"</b>";
				break;
		}

		tmp = ConvertUtf8ToUtf16(account->personal->impresa);
		if (!tmp.isEmpty())
		{
			info = tr("impresa:");
			info += "<b style=\"color:red; \">" + tmp +"</b>";
			tips += info +"<br>";
		}
	}

	//remove the last "<br>"
	tips = tips.remove( tips.size() - strlen("<br>"), strlen("<br>"));
	return tips;
}

//fix: need to....
void BuddyOpt::setTipsOfAccount(HTREEITEM  *accountItem, const Fetion_Account *account)
{
	if (!accountItem || !account)
		return;
	CString tips = createAccountTipsInfo(account);
	accountItem->setToolTip(0, tips); 
}


void BuddyOpt::expandTree()
{
	HTREEITEM  *RootItem = this->treeWidget->invisibleRootItem();
	if(!RootItem)
		return ;

	for(int i =0;  i< RootItem-> childCount (); i++)
		this->treeWidget->expandItem (RootItem->child(i));
}
#endif

int BuddyOpt::GetChildCount(HTREEITEM groupItem)
{
	int count = 0;
	if (treeWidget->ItemHasChildren(groupItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = treeWidget->GetChildItem(groupItem);

		while (hChildItem != NULL)
		{
			count ++;
			hNextItem = treeWidget->GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
		}
	}
	return count;
}
// 更新组信息
void BuddyOpt::updateGroupInfo(HTREEITEM hGroupItem, bool bAnyway)
{

	if (bAnyway || treeWidget->ItemHasChildren(hGroupItem))
    {
        Group_Info *group_info =(Group_Info *)treeWidget->GetItemData(hGroupItem);

	    if(!group_info)
		    return ;

        CString online;
        online.Format(_T("(%d"),group_info->online_no);
        CString BuddyCount;
        BuddyCount.Format(_T("/%d)"), GetChildCount(hGroupItem));
        CString groupShowName = group_info->groupName + online + BuddyCount;
        treeWidget->SetItemText(hGroupItem, groupShowName + _T("  "));

        SortBuddy(hGroupItem);
    }
	if(!treeWidget->ItemHasChildren(hGroupItem))
	{
		Group_Info *group_info =(Group_Info *)treeWidget->GetItemData(hGroupItem);

		if(!group_info)
			return;
		if(group_info->groupID < 0)
		{
			treeWidget->DeleteItem(hGroupItem);
		}
	}
}

// 做排序用，值越小则应该排的越前
static int GetSortValue(int iOnlineState)
{
    switch(iOnlineState)
    {
    case FX_STATUS_ONLINE:
        return 0;
	case FX_STATUS_MOBILE_ONLINE:
		return 5;
    case FX_STATUS_PHONE:
        return 10;
    case FX_STATUS_BUSY:
    case FX_STATUS_MEETING:
        return 20;
	case FX_STATUS_MOBILE_PHONE:
	case FX_STATUS_MOBILE_MEETING:
	case FX_STATUS_MOBILE_BUSY:	
		return 25;
	case FX_STATUS_AWAY:
    case FX_STATUS_EXTENDED_AWAY:
    case FX_STATUS_DINNER:
        return 30;
    case FX_STATUS_UNSET:
    case FX_STATUS_NUM_PRIMITIVES:
        return 40;
	case FX_STATUS_MOBILE_DINNER:
	case FX_STATUS_MOBILE_AWAY:
	case FX_STATUS_MOBILE_EXTENDED_AWAY:
	case FX_STATUS_MOBILE_NUM_PRIMITIVES:
		return 45;
	case FX_STATUS_OFFLINE:
        return 50;
    case FX_STATUS_MOBILE:
        return 60;
	case FX_STATUS_REFUSE_SMS:
		return 65;
    case FX_STATUS_WAITING_AUTH:
        return 70;
	case FX_STATUS_MOBILE_OUT_OF_SERIVCE:
        return 85;
	case FX_STATUS_CLOSE_FETION_SERIVCE:
        return 86;
    case FX_STATUS_REFUSE:
        return 90;
    case FX_STATUS_BLACK:
        return 100;
    default :
        return 80;
    }
}
static int CALLBACK SortBuddyFunc(LPARAM lbuddy1, LPARAM lbuddy2, LPARAM lParamSort)
{
    CTreeCtrl* pTreeCtrl = (CTreeCtrl*) lParamSort;

    
    Account_Info *ac_info1 =(Account_Info*)lbuddy1;
    Account_Info *ac_info2 =(Account_Info*)lbuddy2;

	if(!ac_info1 || !ac_info2)
		return 0;
    int iSortValue1 = GetSortValue(ac_info1->onlinestate);
    int iSortValue2 = GetSortValue(ac_info2->onlinestate);
	int iRet = iSortValue1 - iSortValue2;
	if(iRet == 0)
	{    
		iRet = ac_info1->accountName.CompareNoCase(ac_info2->accountName);
	}
    return iRet;
}

void BuddyOpt::SortBuddy(HTREEITEM hGroupItem)
{
    TVSORTCB tvs;
    tvs.hParent = hGroupItem;
    tvs.lpfnCompare = SortBuddyFunc;
    tvs.lParam = (LPARAM)treeWidget;

    treeWidget->SortChildrenCB(&tvs);
}

HTREEITEM BuddyOpt::findQunItem(const Fetion_Qun * qun)
{
	if (!qun)
		return NULL;

	long qun_id = (long)qun->id;

	if(NULL != QunItem)
	{
		HTREEITEM hItem = treeWidget->GetChildItem(QunItem);
		while (NULL != hItem)
		{
			Qun_Info *qun_info =(Qun_Info*)treeWidget->GetItemData(hItem);
			if(qun_info && (qun_id == qun_info->qunID))
			{
				return hItem;
			}
			hItem = treeWidget->GetNextSiblingItem(hItem);
		}
	}
	return NULL;
}

BOOL BuddyOpt::ChangeBuddyInfo(BUDDYINFODB *pBuddyInfoOld, BUDDYINFODB *pBuddyInfoNew)
{
	BOOL bChanged = FALSE;
	if(pBuddyInfoOld->strLocalName != pBuddyInfoNew->strLocalName)
	{
		bChanged = TRUE;
		pBuddyInfoOld->strLocalName = pBuddyInfoNew->strLocalName;
	}
	if(pBuddyInfoOld->strNickName != pBuddyInfoNew->strNickName)
	{
		bChanged = TRUE;
		pBuddyInfoOld->strNickName = pBuddyInfoNew->strNickName;
	}
	if(pBuddyInfoOld->strImpresa != pBuddyInfoNew->strImpresa)
	{
		bChanged = TRUE;
		pBuddyInfoOld->strImpresa = pBuddyInfoNew->strImpresa;
	}
	return bChanged;
}