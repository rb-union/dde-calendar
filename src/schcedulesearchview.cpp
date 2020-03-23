﻿/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
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
#include "schcedulesearchview.h"
#include <QAction>
#include <QMenu>
#include <QListWidget>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include <QStylePainter>
#include <QRect>
#include "schceduledlg.h"
#include "scheduledatamanage.h"
#include <DMessageBox>
#include <DPushButton>
#include <DHiDPIHelper>
#include <DPalette>
#include "schcedulectrldlg.h"
#include "myschceduleview.h"
DGUI_USE_NAMESPACE
CSchceduleSearchItem::CSchceduleSearchItem( QWidget *parent /*= nullptr*/ ): DLabel(parent)
{
    //setMargin(0);
    m_editAction = new QAction(tr("Edit"), this);
    m_deleteAction = new QAction(tr("Delete"), this);
    connect(m_editAction, SIGNAL(triggered(bool)), this, SLOT(slotEdit()));
    connect(m_deleteAction, SIGNAL(triggered(bool)), this, SLOT(slotDelete()));
    //setFlat(true);

}

void CSchceduleSearchItem::setBackgroundColor(QColor color1)
{
    m_Backgroundcolor = color1;
}

void CSchceduleSearchItem::setSplitLineColor(QColor color1)
{
    m_splitlinecolor = color1;
}

void CSchceduleSearchItem::setText( QColor tcolor, QFont font)
{
    m_ttextcolor = tcolor;
    m_tfont = font;
}

void CSchceduleSearchItem::setTimeC(QColor tcolor, QFont font)
{
    m_timecolor = tcolor;
    m_timefont = font;
}

void CSchceduleSearchItem::setData( ScheduleDtailInfo vScheduleInfo, QDate date)
{
    m_ScheduleInfo = vScheduleInfo;
    m_date = date;
    //setToolTip(m_ScheduleInfo.titleName);
    update();
}

void CSchceduleSearchItem::setRoundtype(int rtype)
{
    m_roundtype = rtype;
    update();
}
void CSchceduleSearchItem::slotEdit()
{
    emit signalViewtransparentFrame(1);
    CSchceduleDlg dlg(0, this);
    dlg.setData(m_ScheduleInfo);
    if (dlg.exec() == DDialog::Accepted) {
        emit signalsEdit(this);
    }
    emit signalViewtransparentFrame(0);
}

void CSchceduleSearchItem::slotDelete()
{
    emit signalViewtransparentFrame(1);
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();

    if (m_ScheduleInfo.rpeat == 0) {
        CSchceduleCtrlDlg msgBox;

        msgBox.setText(tr("You are deleting an event."));
        msgBox.setInformativeText(tr("Are you sure you want to delete this event?"));
        DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
        DPushButton *yesButton = msgBox.addPushButton(tr("Delete"), 1);
        msgBox.updatesize();
        DPalette pa = yesButton->palette();
        if (themetype == 0 || themetype == 1) {
            pa.setColor(DPalette::ButtonText, Qt::red);

        } else {
            pa.setColor(DPalette::ButtonText, "#FF5736");

        }
        yesButton->setPalette(pa);
        msgBox.exec();

        if (msgBox.clickButton() == noButton) {
            emit signalViewtransparentFrame(0);
            return;
        } else if (msgBox.clickButton() == yesButton) {
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_ScheduleInfo.id);
        }
    } else {
        if (m_ScheduleInfo.RecurID == 0) {
            CSchceduleCtrlDlg msgBox;

            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete all occurrences of this event, or only the selected occurrence?"));
            DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
            DPushButton *yesallbutton = msgBox.addPushButton(tr("Delete All"));
            DPushButton *yesButton = msgBox.addPushButton(tr("Delete Only This Event"));


            msgBox.updatesize();
            DPalette pa = yesButton->palette();
            if (themetype == 0 || themetype == 1) {
                pa.setColor(DPalette::ButtonText, Qt::white);
                pa.setColor(DPalette::Dark, QColor("#25B7FF"));
                pa.setColor(DPalette::Light, QColor("#0098FF"));
            } else {
                pa.setColor(DPalette::ButtonText, "#B8D3FF");
                pa.setColor(DPalette::Dark, QColor("#0056C1"));
                pa.setColor(DPalette::Light, QColor("#004C9C"));
            }
            yesButton->setPalette(pa);
            msgBox.exec();

            if (msgBox.clickButton() == noButton) {
                emit signalViewtransparentFrame(0);
                return;
            } else if (msgBox.clickButton() == yesallbutton) {
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_ScheduleInfo.id);
            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_ScheduleInfo.id, newschedule);
                newschedule.ignore.append(m_ScheduleInfo.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        } else {
            CSchceduleCtrlDlg msgBox;
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete this and all future occurrences of this event, or only the selected occurrence?"));
            DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
            DPushButton *yesallbutton = msgBox.addPushButton(tr("Delete All Future Events"));
            DPushButton *yesButton = msgBox.addPushButton(tr("Delete Only This Event"));
            msgBox.updatesize();
            DPalette pa = yesButton->palette();
            if (themetype == 0 || themetype == 1) {
                pa.setColor(DPalette::ButtonText, Qt::white);
                pa.setColor(DPalette::Dark, QColor("#25B7FF"));
                pa.setColor(DPalette::Light, QColor("#0098FF"));
            } else {
                pa.setColor(DPalette::ButtonText, "#B8D3FF");
                pa.setColor(DPalette::Dark, QColor("#0056C1"));
                pa.setColor(DPalette::Light, QColor("#004C9C"));
            }
            yesButton->setPalette(pa);
            msgBox.exec();

            if (msgBox.clickButton() == noButton) {
                emit signalViewtransparentFrame(0);
                return;
            } else if (msgBox.clickButton() == yesallbutton) {
                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_ScheduleInfo.id, newschedule);
                newschedule.enddata.type = 2;
                newschedule.enddata.date = m_ScheduleInfo.beginDateTime.addDays(-1);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);

            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_ScheduleInfo.id, newschedule);
                newschedule.ignore.append(m_ScheduleInfo.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        }
    }
    emit signalViewtransparentFrame(0);
    emit signalsDelete(this);
    //ScheduleDbManager::deleteScheduleInfoById(m_ScheduleInfo.id);
}

void CSchceduleSearchItem::slotDoubleEvent(int type)
{
    emit signalsDelete(this);
}

void CSchceduleSearchItem::paintEvent( QPaintEvent *e )
{
    int labelwidth = width();
    int labelheight = height();

    QPainter painter(this);
    QRect fillRect = QRect(0, 0, labelwidth, labelheight);
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    QColor bcolor = m_Backgroundcolor;
    if (m_hoverflag) {
        bcolor.setAlphaF(0.2);
    }
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setBrush(QBrush(bcolor));
    painter.setPen(Qt::NoPen);
    QPainterPath painterPath;
    painterPath.moveTo(m_radius, m_borderframew);
    if (m_roundtype == 1 || m_roundtype == 3) {
        painterPath.arcTo(QRect(m_borderframew, m_borderframew, m_radius * 2, m_radius * 2), 90, 90);
    } else {
        painterPath.lineTo(m_borderframew, m_borderframew);
        painterPath.lineTo(m_borderframew, m_radius);
    }
    painterPath.lineTo(0, labelheight - m_radius);
    if (m_roundtype == 1 || m_roundtype == 2) {
        painterPath.arcTo(QRect(m_borderframew, labelheight - m_radius * 2, m_radius * 2, m_radius * 2), 180, 90);
    } else {
        painterPath.lineTo(m_borderframew, labelheight);
        painterPath.lineTo(m_radius, labelheight);
    }
    painterPath.lineTo(labelwidth - m_radius, labelheight);
    if (m_roundtype == 1 || m_roundtype == 2) {
        painterPath.arcTo(QRect(labelwidth - m_radius * 2, labelheight - m_radius * 2, m_radius * 2, m_radius * 2), 270, 90);
    } else {
        painterPath.lineTo(labelwidth, labelheight);
        painterPath.lineTo(labelwidth, labelheight - m_radius);
    }
    painterPath.lineTo(labelwidth, m_radius);
    //painterPath.moveTo(labelwidth, m_radius);
    if (m_roundtype == 1 || m_roundtype == 3) {

        painterPath.arcTo(QRect(labelwidth - m_radius * 2, m_borderframew, m_radius * 2, m_radius * 2), 0, 90);

    } else {
        painterPath.lineTo(labelwidth, m_borderframew);
        painterPath.lineTo(labelwidth - m_radius, m_borderframew);
    }
    painterPath.lineTo(m_radius, m_borderframew);
    painterPath.closeSubpath();
    painter.drawPath(painterPath);
    painter.restore();
    bcolor = m_timecolor;
    if (m_selectflag) {
        bcolor.setAlphaF(0.6);
    }
    painter.setFont(m_timefont);
    painter.setPen(bcolor);

    QDate begindate  = m_ScheduleInfo.beginDateTime.date();
    QDate enddate = m_ScheduleInfo.endDateTime.date();
    QString datestr;
    if (begindate  == enddate) {
        datestr = m_ScheduleInfo.beginDateTime.toString("hh:mm")
                  + "-" + m_ScheduleInfo.endDateTime.toString("hh:mm");
    } else {
        datestr = m_ScheduleInfo.beginDateTime.toString("hh:mm")
                  + "-" + m_ScheduleInfo.endDateTime.toString("hh:mm");
    }

    painter.drawText(QRect(12, 8, 65, labelheight - 16), Qt::AlignLeft, datestr);
    painter.save();
    bcolor = m_splitlinecolor;
    if (m_selectflag) {
        bcolor.setAlphaF(0.6);
    }
    QPen pen(bcolor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(82, 0, 82, labelheight);
    painter.restore();

    painter.setFont(m_tfont);
    bcolor = m_ttextcolor;
    if (m_selectflag) {
        bcolor.setAlphaF(0.6);
    }
    painter.setPen(bcolor);
    int tilenameW = labelwidth - 91;
    QFontMetrics fm = painter.fontMetrics();
    QString tStitlename = m_ScheduleInfo.titleName;
    tStitlename.replace("\n", "");
    QString str = tStitlename;
    QString tstr;
    for (int i = 0; i < str.count(); i++) {
        tstr.append(str.at(i));
        int widthT = fm.width(tstr) + 5;
        if (widthT >= tilenameW) {
            tstr.chop(1);
            break;
        }
    }
    if (tstr != str) {
        tstr = tstr + "...";
    }

    painter.drawText(QRect(91, 6, tilenameW, labelheight), Qt::AlignLeft, tstr);
    painter.end();
}
void CSchceduleSearchItem::contextMenuEvent( QContextMenuEvent *event )
{
    DMenu Context(this);
    Context.addAction(m_editAction);
    Context.addAction(m_deleteAction);
    Context.exec(QCursor::pos());
}

void CSchceduleSearchItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit signalViewtransparentFrame(1);
    CMySchceduleView dlg(this);
    dlg.setSchedules(m_ScheduleInfo);
    connect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CSchceduleSearchItem::slotDoubleEvent);
    dlg.exec();
    disconnect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CSchceduleSearchItem::slotDoubleEvent);
    emit signalViewtransparentFrame(0);

}

void CSchceduleSearchItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit signalSelectDate(m_date);
        emit signalSelectID(m_ScheduleInfo.id);
        m_selectflag = true;
        update();
    }
}

void CSchceduleSearchItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_selectflag = false;
        update();
    }
}

void CSchceduleSearchItem::enterEvent(QEvent *event)
{
    m_hoverflag = true;
    update();
}

void CSchceduleSearchItem::leaveEvent(QEvent *event)
{
    m_hoverflag = false;
    update();
}
CSchceduleSearchView::CSchceduleSearchView(QWidget *parent) : DWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    m_gradientItemList = new DListWidget(parent);
    m_gradientItemList->setAlternatingRowColors(true);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_gradientItemList);
    //m_gradientItemList->setSpacing(1);
    // set default row
    m_gradientItemList->setCurrentRow(0);
    setLayout(layout);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    m_bBackgroundcolor.setAlphaF(0.03);
    m_gradientItemList->setLineWidth(0);
    m_labellist.clear();
}

CSchceduleSearchView::~CSchceduleSearchView()
{

}

void CSchceduleSearchView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        m_bBackgroundcolor = "#000000";
        m_bBackgroundcolor.setAlphaF(0.05);
        m_btimecolor = "#526A7F";
        m_bttextcolor = "#414D68";
        m_lBackgroundcolor = Qt::white;
        m_ltextcolor = "#001A2E";
    } else if (type == 2) {
        m_bBackgroundcolor = "#FFFFFF";
        m_bBackgroundcolor.setAlphaF(0.05);
        m_btimecolor = "#6D7C88";
        m_bttextcolor = "#C0C6D4";
        m_lBackgroundcolor = "#FFFFFF";
        m_lBackgroundcolor.setAlphaF(0.0);
        m_ltextcolor = "#C0C6D4";
    }
    updateDateShow();
}

void CSchceduleSearchView::clearSearch()
{
    m_vlistData.clear();

    //remove
    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->takeItem(i);
        m_gradientItemList->removeItemWidget(item11);
    }
    m_labellist.clear();
    m_gradientItemList->clear();
    QVector<ScheduleDateRangeInfo> vScheduleInfo;
    CScheduleDataManage::getScheduleDataManage()->setSearchResult(vScheduleInfo);
}

void CSchceduleSearchView::updateDateShow()
{
    m_currentItem = nullptr;
    //remove
    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->takeItem(i);
        m_gradientItemList->removeItemWidget(item11);
    }
    m_gradientItemList->clear();
    m_labellist.clear();
    //找最近日程
    bool flag = false;
    QDate tcurrentdata = QDate::currentDate();
    for (int i = 0; i < m_vlistData.size(); ++i) {
        if (m_vlistData[i].date == tcurrentdata) {
            if (!m_vlistData.at(i).vData.isEmpty()) {
                flag = true;
                break;
            }
        }
    }
    if (!flag && !m_vlistData.isEmpty()) {
        QDate topdate = tcurrentdata;
        QDate mindate = topdate;
        for (int i = 0; i < m_vlistData.size(); ++i) {
            if (m_vlistData[i].date > mindate) {
                mindate = m_vlistData[i].date;
            }
        }
        while (!flag) {
            topdate = topdate.addDays(-1);
            for (int i = 0; i < m_vlistData.size(); ++i) {
                if (m_vlistData[i].date == topdate) {
                    if (!m_vlistData.at(i).vData.isEmpty()) {
                        flag = true;
                        break;
                    }
                }
            }
            if (topdate < mindate) break;
        }
        tcurrentdata = topdate;
    }
    QVector<ScheduleDateRangeInfo> m_showData;
    ScheduleDateRangeInfo showData;
    for (int i = 0; i < m_vlistData.size(); ++i) {
        showData.date = m_vlistData.at(i).date;
        for (int j = 0 ; j < m_vlistData.at(i).vData.size(); ++j) {
            if (m_vlistData.at(i).vData.at(j).beginDateTime.date() == m_vlistData.at(i).date) {
                showData.vData.append(m_vlistData.at(i).vData.at(j));
            }
        }
        if (showData.vData.count() > 0) {
            m_showData.append(showData);
        }

        showData.vData.clear();
    }

    for (int i = 0; i < m_showData.size(); ++i) {
        QListWidgetItem *titem = createItemWidget(m_showData[i].date);
        if (m_showData[i].date == tcurrentdata) {
            m_currentItem = titem;
        }
        if (m_showData.at(i).vData.isEmpty()) continue;
        if (m_showData.at(i).vData.count() == 1) {
            createItemWidget(m_showData.at(i).vData.at(0), m_showData[i].date, 1);
        } else if (m_showData.at(i).vData.count() == 2) {
            createItemWidget(m_showData.at(i).vData.at(0), m_showData[i].date, 3);
            createItemWidget(m_showData.at(i).vData.at(1), m_showData[i].date, 2);
        } else {
            createItemWidget(m_showData.at(i).vData.at(0), m_showData[i].date, 3);
            for (int j = 1; j < m_showData.at(i).vData.count() - 1; j++) {
                createItemWidget(m_showData.at(i).vData.at(j), m_showData[i].date, 0);
            }
            createItemWidget(m_showData.at(i).vData.at(m_showData.at(i).vData.count() - 1), m_showData[i].date, 2);
        }
    }
    if (m_gradientItemList->count() == 0) {
        QListWidgetItem *listItem = new QListWidgetItem;
        DLabel *gwi = new DLabel();
        QFont font("SourceHanSansSC-Normal");
        font.setPixelSize(20);
        gwi->setAlignment(Qt::AlignCenter );
        DPalette daypa = gwi->palette();
        QColor textcolor = DPalette::ToolTipText;
        int themtype = CScheduleDataManage::getScheduleDataManage()->getTheme();
        if (themtype == 2) {
            textcolor = "#C0C6D4";
            textcolor.setAlphaF(0.4);
        } else {
            textcolor = "#000000";
            textcolor.setAlphaF(0.3);
        }
        daypa.setColor(DPalette::WindowText, textcolor);
        daypa.setColor(DPalette::Window, m_lBackgroundcolor);
        gwi->setPalette(daypa);
        gwi->setForegroundRole(DPalette::WindowText);
        //gwi->setForegroundRole(DPalette::Window);
        gwi->setFont(font);
        gwi->setText(tr("No search results"));
        gwi->setFixedSize(m_gradientItemList->width() - 20, 450);
        listItem->setSizeHint(QSize(m_gradientItemList->width(), 450)); //每次改变Item的高度
        //listItem->setBackgroundColor(Qt::white);
        listItem->setFlags(Qt::ItemIsTristate );
        m_gradientItemList->addItem(listItem);
        m_gradientItemList->setItemWidget(listItem, gwi);
        m_labellist.append(gwi);
    }
    if (m_currentItem != nullptr) {
        m_gradientItemList->scrollToItem(m_currentItem, QAbstractItemView::PositionAtTop);
    }
}

void CSchceduleSearchView::createItemWidget(ScheduleDtailInfo info, QDate date, int rtype)
{
    ScheduleDtailInfo &gd = info;
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(gd.type.ID);

    CSchceduleSearchItem *gwi = new CSchceduleSearchItem();
    QFont font("SourceHanSansSC-Normal");
    font.setPixelSize(14);
    gwi->setBackgroundColor(m_bBackgroundcolor);
    //gwi->setBackgroundColor(Qt::white);
    QColor scolor = gdcolor.Purecolor;
    scolor.setAlphaF(1.0);
    gwi->setSplitLineColor(gdcolor.splitColor);
    gwi->setText(m_bttextcolor, font);
    font.setPixelSize(12);
    gwi->setTimeC(m_btimecolor, font);
    gwi->setFixedSize(m_gradientItemList->width() - 20, 35);
    gwi->setData(gd, date);
    gwi->setRoundtype(rtype);
    connect(gwi, &CSchceduleSearchItem::signalsDelete, this, &CSchceduleSearchView::slotdeleteitem);
    connect(gwi, &CSchceduleSearchItem::signalsEdit, this, &CSchceduleSearchView::slotedititem);
    connect(gwi, &CSchceduleSearchItem::signalSelectDate, this, &CSchceduleSearchView::slotSelectDate);
    connect(gwi, &CSchceduleSearchItem::signalSelectID, this, &CSchceduleSearchView::slotSelectID);
    connect(gwi, &CSchceduleSearchItem::signalViewtransparentFrame, this, &CSchceduleSearchView::signalViewtransparentFrame);

    //connect(gwi, SIGNAL(signalsDelete(QDate )), this, SIGNAL(signalDate(QDate )));

    QListWidgetItem *listItem = new QListWidgetItem;
    listItem->setSizeHint(QSize(m_gradientItemList->width() - 5, 36)); //每次改变Item的高度
    //listItem->setBackgroundColor(Qt::white);
    listItem->setFlags(Qt::ItemIsTristate );
    m_gradientItemList->addItem(listItem);
    m_gradientItemList->setItemWidget(listItem, gwi);
    m_labellist.append(gwi);
}

QListWidgetItem *CSchceduleSearchView::createItemWidget(QDate date)
{
    CSchceduleSearchDateItem *gwi = new CSchceduleSearchDateItem();
    QFont font("SourceHanSansSC");
    font.setWeight(QFont::Medium);
    font.setPixelSize(16);
    gwi->setBackgroundColor(m_lBackgroundcolor);
    gwi->setText(m_ltextcolor, font);
    if (date == QDate::currentDate()) {
        int themtype = CScheduleDataManage::getScheduleDataManage()->getTheme();
        if (themtype == 2) {
            gwi->setText("#0059D2", font);
        } else {
            gwi->setText("#0081FF", font);
        }

    }
    gwi->setFixedSize(m_gradientItemList->width() - 20, 35);
    gwi->setDate(date);
    QListWidgetItem *listItem = new QListWidgetItem;
    listItem->setSizeHint(QSize(m_gradientItemList->width() - 5, 36)); //每次改变Item的高度
    //listItem->setBackgroundColor(Qt::white);
    listItem->setFlags(Qt::ItemIsTristate );
    m_gradientItemList->addItem(listItem);
    m_gradientItemList->setItemWidget(listItem, gwi);
    m_labellist.append(gwi);
    return listItem;
}

void CSchceduleSearchView::slotdeleteitem( CSchceduleSearchItem *item )
{
    emit signalsUpdateShcedule(item->getData().id);
    updateDateShow();
    update();
}

void CSchceduleSearchView::slotedititem(CSchceduleSearchItem *item)
{
    emit signalsUpdateShcedule(item->getData().id);
}

void CSchceduleSearchView::slotsetSearch(QString str)
{
    if (str.isEmpty()) return;
    QDateTime date = QDateTime::currentDateTime();

    QDateTime bdate = date.addMonths(-6);
    if (!bdate.isValid()) {
        QDateTime tdate = date;
        tdate.setDate(QDate(date.date().year(), date.date().month(), 1));
        bdate = tdate.addMonths(-6);
    }
    QDateTime edate = date.addMonths(6);
    if (!edate.isValid()) {
        QDateTime tdate = date;
        tdate.setDate(QDate(date.date().year(), date.date().month(), 1));
        edate = tdate.addMonths(7);
        edate = edate.addDays(-1);
    }
    QVector<ScheduleDateRangeInfo> vScheduleInfo;
    m_vlistData.clear();
#if 0
    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfo(bdate.date(), edate.date(), vScheduleInfo);
    for (int i = 0; i < vScheduleInfo.count(); i++) {
        QVector<ScheduleDtailInfo> vData;
        for (int j = 0; j < vScheduleInfo.at(i).vData.count(); j++) {
            if (vScheduleInfo.at(i).vData.at(j).titleName.contains(str))
                vData.append(vScheduleInfo.at(i).vData.at(j));
        }
        if (!vData.isEmpty()) {
            ScheduleDateRangeInfo sinfo;
            sinfo.date = vScheduleInfo.at(i).date;
            sinfo.vData = vData;
            m_vlistData.append(sinfo);
        }
    }
#else
    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->queryScheduleInfo(str, bdate, edate, vScheduleInfo);
    m_vlistData = vScheduleInfo;
    CScheduleDataManage::getScheduleDataManage()->setSearchResult(vScheduleInfo);
#endif
    updateDateShow();
}

void CSchceduleSearchView::slotSelectDate(QDate date)
{
    emit signalDate(date);
}

void CSchceduleSearchView::slotSelectID(const int ID)
{
    emit signalSelectID(ID);
}

void CSchceduleSearchView::resizeEvent(QResizeEvent *event)
{
    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->item(i);
        item11->setSizeHint(QSize(m_gradientItemList->width() - 5, 36)); //每次改变Item的高度
    }
    //remove
    for (int i = 0; i < m_labellist.count(); i++) {
        m_labellist.at(i)->setFixedSize(m_gradientItemList->width() - 20, 35);
        m_labellist.at(i)->update();
    }
    if (m_gradientItemList->count() == 1) {
        QListWidgetItem *item11 = m_gradientItemList->item(0);
        item11->setSizeHint(QSize(m_gradientItemList->width(), height() * 0.7978)); //每次改变Item的高度
        m_labellist.at(0)->setFixedSize(m_gradientItemList->width(), height() * 0.7978);
        m_labellist.at(0)->update();
    }
    DWidget::resizeEvent(event);
}


CSchceduleSearchDateItem::CSchceduleSearchDateItem(QWidget *parent): DLabel(parent)
{
    setAutoFillBackground(true);
}

void CSchceduleSearchDateItem::setBackgroundColor(QColor color1)
{
    m_Backgroundcolor = color1;
}

void CSchceduleSearchDateItem::setText(QColor tcolor, QFont font)
{
    m_textcolor = tcolor;
    m_font = font;
}

void CSchceduleSearchDateItem::setDate(QDate date)
{
    m_date = date;
}

void CSchceduleSearchDateItem::paintEvent(QPaintEvent *e)
{
    int labelwidth = width();
    int labelheight = height();

    QPainter painter(this);
    QRect fillRect = QRect(0, 0, labelwidth, labelheight);
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.setBrush(QBrush(m_Backgroundcolor));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(fillRect, 3, 3);

    painter.setFont(m_font);
    painter.setPen(m_textcolor);

    QLocale locale;
    QString datestr;
    if (locale.language() == QLocale::Chinese) {
        datestr = QString::number(m_date.year()) + tr("Y") + QString::number(m_date.month()) + tr("M") + QString::number(m_date.day()) + tr("D");
    } else {
        datestr = m_date.toString("yyyy/M/d");
    }
    painter.drawText(QRect(12, 8, labelwidth, labelheight - 8), Qt::AlignLeft, datestr);
    painter.end();
}
