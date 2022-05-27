/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     leilong  <leilong@uniontech.com>
*
* Maintainer: leilong  <leilong@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include "dbus/dbusaccountmanagerrequest.h"
#include "accountitem.h"

//所有账户管理类
class AccountManager : public QObject
{
    Q_OBJECT
public:
    typedef std::function<void(bool)> CallbackFunc;

    static AccountManager* getInstance();
    QList<QSharedPointer<AccountItem>> getAccountList();
    QSharedPointer<AccountItem> getLocalAccountItem();
    QSharedPointer<AccountItem> getUnionAccountItem();

    //重新获取账户信息
    void resetAccount();

    //日程提醒
    void remindJob(const QString &scheduleID, const qint64 recurID, CallbackFunc callback = nullptr);
    //根据帐户ID下拉数据
    void downloadByAccountID(const QString &accountID, CallbackFunc callback = nullptr);
    //更新网络帐户数据
    void uploadNetWorkAccountData(CallbackFunc callback = nullptr);

    //等待数据获取完成的事件
    void waitingData(CallbackFunc callback);

signals:

public slots:
    //获取账户信息完成事件
    void slotGetAccountListFinish(DAccount::List accountList);
    //获取通用设置完成事件
    void slotGetGeneralSettingsFinish(DCalendarGeneralSettings::Ptr ptr);


protected:
    explicit AccountManager(QObject *parent = nullptr);

private:
    void initConnect();

    //运行等待数据完成的回调函数
    void execWaitingCall();

private:
    static AccountManager* m_accountManager;
    QSharedPointer<AccountItem>  m_localAccountItem;
    QSharedPointer<AccountItem>  m_unionAccountItem;
    DCalendarGeneralSettings::Ptr m_settings;

    DbusAccountManagerRequest* m_dbusRequest;

    QList<CallbackFunc> m_waitingCallList;

    bool m_dataFinished = false;
};
#define gAccounManager AccountManager::getInstance()
#define gLocalAccountItem AccountManager::getLocalAccountItem()
#define gUosAccountItem AccountManager::getUosAccountItem()
#endif // ACCOUNTMANAGER_H
