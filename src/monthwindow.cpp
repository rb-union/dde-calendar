﻿/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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
#include "monthwindow.h"
#include "monthview.h"
#include "monthdayview.h"
#include "constants.h"
#include "calendardbus.h"
#include <QMessageBox>
#include <QPainter>
#include <DPalette>
DGUI_USE_NAMESPACE
CMonthWindow::CMonthWindow(QWidget *parent): QMainWindow (parent)
{
    setContentsMargins(0, 0, 0, 0);
    initUI();
    initConnection();
    initLunar();
}

CMonthWindow::~CMonthWindow()
{
    disconnect(m_today, &DPushButton::clicked, this, &CMonthWindow::slottoday);
    delete  m_monthDayView;
    m_monthDayView = nullptr;
    delete  m_monthView;
    m_monthView = nullptr;
}

void CMonthWindow::setFirstWeekday(int weekday)
{
    m_monthView->setFirstWeekday(weekday);
}

void CMonthWindow::setDate(QDate date)
{
    m_currentdate = date;
    m_YearLabel->setText(QString::number(date.year()) + tr("Y"));
    m_monthDayView->setCurrentDate(date);
}

void CMonthWindow::setLunarVisible(bool state)
{
    m_monthView->setLunarVisible(state);
    m_YearLunarLabel->setVisible(state);
}

void CMonthWindow::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        DPalette anipa = m_contentBackground->palette();
        anipa.setColor(DPalette::Background, "#F8F8F8");
        m_contentBackground->setPalette(anipa);
        m_contentBackground->setBackgroundRole(DPalette::Background);

        DPalette todaypa = m_today->palette();
        todaypa.setColor(DPalette::ButtonText, QColor("#1D81EC"));
        todaypa.setColor(DPalette::Dark, Qt::white);
        todaypa.setColor(DPalette::Light, Qt::white);
        QColor sbcolor("#002A57");
        sbcolor.setAlphaF(0.05);
        todaypa.setColor(DPalette::Shadow, sbcolor);
        m_today->setPalette(todaypa);

        DPalette pa = m_YearLabel->palette();
        pa.setColor(DPalette::WindowText, QColor("#3B3B3B"));
        m_YearLabel->setPalette(pa);
        m_YearLabel->setForegroundRole(DPalette::WindowText);

        DPalette Lunapa = m_YearLunarLabel->palette();
        Lunapa.setColor(DPalette::WindowText, QColor("#8A8A8A"));
        m_YearLunarLabel->setPalette(Lunapa);
        m_YearLunarLabel->setForegroundRole(DPalette::WindowText);

        DPalette gpa = m_gridWidget->palette();
        gpa.setColor(DPalette::Background, "#F8F8F8");
        m_gridWidget->setPalette(gpa);
        m_gridWidget->setBackgroundRole(DPalette::Background);

    } else if (type == 2) {
        DPalette todaypa = m_today->palette();
        todaypa.setColor(DPalette::ButtonText, QColor("#0081FF"));
        todaypa.setColor(DPalette::Light, "#484848");
        todaypa.setColor(DPalette::Dark, "#414141");
        QColor sbcolor("#000000");
        sbcolor.setAlphaF(0.08);
        todaypa.setColor(DPalette::Shadow, sbcolor);
        m_today->setPalette(todaypa);

        DPalette pa = m_YearLabel->palette();
        pa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
        m_YearLabel->setPalette(pa);
        m_YearLabel->setForegroundRole(DPalette::WindowText);
        DPalette Lunapa = m_YearLunarLabel->palette();
        Lunapa.setColor(DPalette::WindowText, QColor("#798BA8"));
        m_YearLunarLabel->setPalette(Lunapa);
        m_YearLunarLabel->setForegroundRole(DPalette::WindowText);
        DPalette anipa = m_contentBackground->palette();
        anipa.setColor(DPalette::Background, "#252525");
        m_contentBackground->setPalette(anipa);
        m_contentBackground->setBackgroundRole(DPalette::Background);

        DPalette gpa = m_gridWidget->palette();
        gpa.setColor(DPalette::Background, "#252525");
        m_gridWidget->setPalette(gpa);
        m_gridWidget->setBackgroundRole(DPalette::Background);
    }
    m_monthDayView->setTheMe(type);
    m_monthView->setTheMe(type);
}

void CMonthWindow::previousMonth()
{
    slideMonth(false);
}

void CMonthWindow::nextMonth()
{
    slideMonth(true);
}

void CMonthWindow::wheelEvent(QWheelEvent *e)
{
    if (e->delta() < 0) {
        previousMonth();
    } else {
        nextMonth();
    }
}

void CMonthWindow::initUI()
{
    m_contentBackground = new DFrame;
    m_contentBackground->setContentsMargins(0, 0, 0, 0);
    DPalette anipa = m_contentBackground->palette();
    anipa.setColor(DPalette::Background, "#F8F8F8");
    m_contentBackground->setAutoFillBackground(true);
    m_contentBackground->setPalette(anipa);

    m_today = new DPushButton;
    m_today->setText(tr("Return today"));
    m_today->setFixedSize(DDEMonthCalendar::MTodayWindth, DDEMonthCalendar::MTodayHeight);
    DPalette todaypa = m_today->palette();
    todaypa.setColor(DPalette::ButtonText, QColor("#1D81EC"));
    todaypa.setColor(DPalette::Dark, Qt::white);
    todaypa.setColor(DPalette::Light, Qt::white);

    QColor sbcolor("#002A57");
    sbcolor.setAlphaF(0.05);
    todaypa.setColor(DPalette::Shadow, sbcolor);
    QFont todayfont;
    todayfont.setFamily("SourceHanSansSC");
    todayfont.setWeight(QFont::Medium);
    todayfont.setPixelSize(14);
    m_today->setFont(todayfont);
    m_today->setPalette(todaypa);
    m_YearLabel = new DLabel();
    m_YearLabel->setFixedHeight(DDEMonthCalendar::M_YLableHeight);
    m_YearLunarLabel = new DLabel();
    m_YearLunarLabel->setFixedSize(DDEMonthCalendar::M_YLunatLabelWindth, DDEMonthCalendar::M_YLunatLabelHeight);

    QFont ylabelF;
    ylabelF.setFamily("SourceHanSansSC");
    ylabelF.setWeight(QFont::Medium);
    ylabelF.setPixelSize(24);
    m_YearLabel->setFont(ylabelF);
    DPalette pa = m_YearLabel->palette();
    pa.setColor(DPalette::WindowText, QColor("#3B3B3B"));
    m_YearLabel->setPalette(pa);
    //m_YearLabel->setStyleSheet("color:#3B3B3B;");

    ylabelF.setPixelSize(14);
    m_YearLunarLabel->setFont(ylabelF);
    DPalette Lunarpa = m_YearLunarLabel->palette();
    Lunarpa.setColor(DPalette::WindowText, QColor("#8A8A8A"));
    m_YearLunarLabel->setPalette(Lunarpa);

    m_monthDayView = new CMonthDayView(this);

    QHBoxLayout *yeartitleLayout = new QHBoxLayout;
    yeartitleLayout->setMargin(0);
    yeartitleLayout->setSpacing(0);
    yeartitleLayout->setContentsMargins(21, 20, 18, 10);
    yeartitleLayout->addWidget(m_YearLabel);

    QHBoxLayout *yeartitleLayout1 = new QHBoxLayout;
    yeartitleLayout1->setMargin(0);
    yeartitleLayout1->setSpacing(0);
    yeartitleLayout1->setContentsMargins(4, 9, 0, 7);
    yeartitleLayout1->addWidget(m_YearLunarLabel);
    yeartitleLayout->addLayout(yeartitleLayout1);
    //yeartitleLayout->addSpacing(30);
    m_spaceitem = new QSpacerItem(30, 36, QSizePolicy::Fixed, QSizePolicy::Fixed);

    yeartitleLayout->addItem(m_spaceitem);
    yeartitleLayout->addWidget(m_monthDayView, 0, Qt::AlignCenter);
    yeartitleLayout->addStretch();
    yeartitleLayout->addWidget(m_today);

    m_monthView = new CMonthView(this);
    QVBoxLayout *mhLayout = new QVBoxLayout;
    mhLayout->setMargin(0);
    mhLayout->setSpacing(0);
    // mhLayout->setContentsMargins(10, 2, 10, 10);
    mhLayout->addWidget(m_monthView);

    QVBoxLayout *hhLayout = new QVBoxLayout;
    hhLayout->setSpacing(0);
    hhLayout->setMargin(0);

    hhLayout->addLayout(yeartitleLayout);
    m_gridWidget = new DFrame();
    m_gridWidget->setContentsMargins(0, 0, 0, 0);
    m_gridWidget->setAutoFillBackground(true);
    m_gridWidget->setLayout(mhLayout);

    hhLayout->addWidget(m_gridWidget);
    m_contentBackground->setLayout(hhLayout);

    m_animationContainer = new DFrame(m_contentBackground);
    // DPalette anipa = m_animationContainer->palette();
    // anipa.setColor(DPalette::Background, Qt::white);
    //m_animationContainer->setAutoFillBackground(true);
    //m_animationContainer->setPalette(anipa);
    //m_animationContainer->setStyleSheet("QFrame { background: rgba(0, 0, 0, 0) }");
    m_animationContainer->setFixedSize(m_monthView->width(),
                                       m_monthView->height() - DDEMonthCalendar::MDayCellHeight);
    m_animationContainer->move(10, 130);
    m_animationContainer->hide();

    setCentralWidget(m_contentBackground);
}

void CMonthWindow::initConnection()
{
    connect(m_today, &DPushButton::clicked, this, &CMonthWindow::slottoday);
    connect(m_monthView, &CMonthView::signalcurrentLunarDateChanged, this, &CMonthWindow::slotcurrentDateLunarChanged);
    connect(m_monthView, &CMonthView::signalcurrentDateChanged, this, &CMonthWindow::slotcurrentDateChanged);
    connect(m_monthDayView, &CMonthDayView::signalsSelectDate, this, &CMonthWindow::slotSelectedMonth);
    connect(m_monthView, &CMonthView::signalsSchceduleUpdate, this, &CMonthWindow::slotTransitSchedule);
    connect(m_monthDayView, &CMonthDayView::signalsCurrentDate, this, &CMonthWindow::slotSelectedMonth);
}

void CMonthWindow::initLunar()
{
    m_monthView->setLunarVisible(true);
}

void CMonthWindow::slideMonth(bool next)
{
    m_animationContainer->show();
    m_animationContainer->raise();
#if 0
    if (next) {
        if (m_currentdate.month() != 1) {
            m_currentdate = m_currentdate.addMonths(-1);
        } else {
            m_animationContainer->hide();
            return;
        }
    } else {
        if (m_currentdate.month() != 12) {
            m_currentdate = m_currentdate.addMonths(1);
        } else {
            m_animationContainer->hide();
            return;
        }
    }
#else
    if (next) {
        m_currentdate = m_currentdate.addMonths(-1);
    } else {
        m_currentdate = m_currentdate.addMonths(1);
    }
#endif
    setDate(m_currentdate);
}

void CMonthWindow::slotReturnTodayUpdate()
{
    setDate(QDate::currentDate());
}

void CMonthWindow::slotupdateSchedule(int id)
{
    m_monthView->slotSchceduleUpdate(id);
}

void CMonthWindow::slotTransitSchedule(int id)
{
    emit signalsWUpdateShcedule(this, id);
}

void CMonthWindow::resizeEvent(QResizeEvent *event)
{
    int tw = width() ;
    int th = height() - 66;
    int dw = width() * 0.5023 + 0.5;
    int dh = 36;
    int space = (width() - dw) / 2 - 184;
    m_spaceitem->changeSize(space, 36, QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_monthDayView->setFixedSize(dw, dh);
    m_monthView->setFixedSize(tw, th);
    m_animationContainer->setFixedSize(m_monthView->width(),
                                       m_monthView->height() -  th * 0.1042 + 0.5);
    // for (int i = 0; i < m_monthViewList.count(); i++) {
    //   m_monthViewList.at(i)->setFixedSize(tw, th);
    //}
    QMainWindow::resizeEvent(event);
}

void CMonthWindow::slottoday()
{
    emit signalsReturnTodayUpdate(this);
    setDate(QDate::currentDate());
}

void CMonthWindow::slotcurrentDateLunarChanged(QDate date, CaLunarDayInfo detail, int type)
{
    QDate currentdate = m_currentdate;
    m_currentdate = date;
    if (type == 1) {
        m_YearLabel->setText(QString::number(date.year()) + tr("Y"));
        m_YearLunarLabel->setText("-" + detail.mGanZhiYear + detail.mZodiac + "年-");
        m_animationContainer->hide();

    } else if (type == 0) {
        if (date.month() != currentdate.month()) {
            m_monthDayView->setRCurrentDate(date);
        }
    }
    //m_monthDayView->setRCurrentDate(date);
}

void CMonthWindow::slotcurrentDateChanged(QDate date)
{
    m_currentdate = date;
    if (date != QDate::currentDate()) {
        m_today->setEnabled(true);
    } else {
        m_today->setEnabled(false);
    }
}

void CMonthWindow::slotSelectedMonth(QDate date)
{
    m_currentdate = date;
    m_monthView->setCurrentDate(date);
    m_monthView->slotSchceduleUpdate();
}
