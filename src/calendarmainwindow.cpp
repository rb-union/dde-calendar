/*
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
#include "calendarmainwindow.h"
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QWidget>
#include <DAboutDialog>
#include <DHiDPIHelper>
#include "yearwindow.h"
#include "monthwindow.h"
#include "dbuscalendar_adaptor.h"
#include <QSizePolicy>
#include "weekwindow.h"
#include "daywindow.h"
static const int CalendarMTitleHeight = 50;

static const int CalendarMWidth = 860;
static const int CalendarMHeight = 634;

static const int WorkViewWidth = 860;
static const int WorkViewHeight = 584;
Calendarmainwindow::Calendarmainwindow()
{
    setContentsMargins(QMargins(0, 0, 0, 0));
    initUI();
    initConnection();
    initLunar();
    setFixedSize(CalendarMWidth, CalendarMHeight);
    setWindowTitle(tr("Deepin Calendar"));

    new CalendarAdaptor(this);
}

void Calendarmainwindow::initUI()
{
    m_icon = new QLabel(this);
    m_icon->setFixedSize(34, 34);
    m_icon->setPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg")
                      .scaled(m_icon->size() * devicePixelRatioF()));
    m_icon->move(12, 8);

    m_bttongroup = new QButtonGroup();
    m_ybutton = createButon(tr("Y"));
    m_bttongroup->addButton(m_ybutton, 0);
    m_mbutton = createButon(tr("M"));
    m_bttongroup->addButton(m_mbutton, 1);
    m_wbutton = createButon(tr("W"));
    m_bttongroup->addButton(m_wbutton, 2);
    m_dbutton = createButon(tr("D"));
    m_bttongroup->addButton(m_dbutton, 3);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->addWidget(m_ybutton);
    titleLayout->addWidget(m_mbutton);
    titleLayout->addWidget(m_wbutton);
    titleLayout->addWidget(m_dbutton);
    QSpacerItem *lspaceitem = new QSpacerItem(30, CalendarMTitleHeight, QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleLayout->addSpacerItem(lspaceitem);
    m_searchEdit = new DSearchEdit;
    m_searchEdit->setFixedHeight(30);
    m_searchEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleLayout->addWidget(m_searchEdit);
    //m_searchEdit->setSizePolicy(QSizePolicy::Expanding);
    QSpacerItem *rspaceitem = new QSpacerItem(30, CalendarMTitleHeight, QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleLayout->addSpacerItem(rspaceitem);
    QWidget *buttonW = new QWidget;
    buttonW->setLayout(titleLayout);

    DTitlebar *titlebar = this->titlebar();
    titlebar->setFixedHeight(50);
    titlebar->setCustomWidget(buttonW, Qt::AlignLeft);

    m_stackWidget = new QStackedWidget;
    m_stackWidget->setFixedSize(WorkViewWidth, WorkViewHeight);
    createview();
    setCentralWidget(m_stackWidget);
    m_bttongroup->button(0)->setChecked(true);

}

void Calendarmainwindow::initConnection()
{
    connect(m_bttongroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &Calendarmainwindow::slotstackWClicked);
    connect(m_weekWindow, &CWeekWindow::signalsWUpdateShcedule, this, &Calendarmainwindow::slotWUpdateShcedule);
    connect(m_monthWindow, &CMonthWindow::signalsWUpdateShcedule, this, &Calendarmainwindow::slotWUpdateShcedule);
    connect(m_DayWindow, &CDayWindow::signalsWUpdateShcedule, this, &Calendarmainwindow::slotWUpdateShcedule);
    connect(m_searchEdit, &DSearchEdit::returnPressed, this, &Calendarmainwindow::slotSreturnPressed);
    connect(m_searchEdit, &DSearchEdit::textChanged, this, &Calendarmainwindow::slotStextChanged);
    connect(m_weekWindow, &CWeekWindow::signalsReturnTodayUpdate, this, &Calendarmainwindow::slotReturnTodyUpdate);
    connect(m_monthWindow, &CMonthWindow::signalsReturnTodayUpdate, this, &Calendarmainwindow::slotReturnTodyUpdate);
    connect(m_DayWindow, &CDayWindow::signalsReturnTodayUpdate, this, &Calendarmainwindow::slotReturnTodyUpdate);
    connect(m_yearwindow, &CYearWindow::signalsReturnTodayUpdate, this, &Calendarmainwindow::slotReturnTodyUpdate);
}

void Calendarmainwindow::initLunar()
{
    m_yearwindow->setLunarVisible(QLocale::system().name().contains("zh"));
}

void Calendarmainwindow::createview()
{
    m_yearwindow = new CYearWindow;
    m_yearwindow->setDate(QDate::currentDate());
    m_stackWidget->addWidget(m_yearwindow);
#if 1
    m_monthWindow = new CMonthWindow;
    m_monthWindow->setDate(QDate::currentDate());
    m_monthWindow->setFirstWeekday(0);
    m_stackWidget->addWidget(m_monthWindow);
    m_weekWindow  = new CWeekWindow(this);
    m_weekWindow->setFirstWeekday(0);
    m_weekWindow->setDate(QDate::currentDate());
    m_stackWidget->addWidget(m_weekWindow);

    m_DayWindow = new CDayWindow;
    m_DayWindow->setDate(QDate::currentDate());
    m_stackWidget->addWidget(m_DayWindow);
#else
    CYearWindow *yearwindow1 = new CYearWindow;
    yearwindow1->setDate(QDate(2014, 2, 1));
    m_stackWidget->addWidget(yearwindow1);
    CYearWindow *yearwindow2 = new CYearWindow;
    yearwindow2->setDate(QDate(2014, 2, 1));
    m_stackWidget->addWidget(yearwindow2);

    CYearWindow *yearwindow3 = new CYearWindow;
    yearwindow3->setDate(QDate(2018, 2, 1));
    m_stackWidget->addWidget(yearwindow3);
#endif

}

DPushButton *Calendarmainwindow::createButon(QString name)
{
    DPushButton *button = new DPushButton();
    button->setText(name);
    button->setFixedSize(50, 40);
    //button->setPressIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-choose30.svg")
    //.scaled(50, 40));
    return  button;
}

void Calendarmainwindow::slotstackWClicked(int index)
{
    if (index < 0 || index > m_stackWidget->depth() - 1) {

        return;
    }
    m_stackWidget->setCurrentIndex(index);
}

void Calendarmainwindow::slotWUpdateShcedule(QMainWindow *w, int id)
{
    if (w != m_weekWindow)
        m_weekWindow->slotupdateSchedule(id);
    if (w != m_monthWindow)
        m_monthWindow->slotupdateSchedule(id);
    if (w != m_DayWindow)
        m_DayWindow->slotupdateSchedule(id);
}

void Calendarmainwindow::slotReturnTodyUpdate(QMainWindow *w)
{
    if (w != m_weekWindow)
        m_weekWindow->slotReturnTodayUpdate();
    if (w != m_monthWindow)
        m_monthWindow->slotReturnTodayUpdate();
    if (w != m_DayWindow)
        m_DayWindow->slotReturnTodayUpdate();
    if (w != m_yearwindow)
        m_yearwindow->slotReturnTodayUpdate();
}

void Calendarmainwindow::slotSreturnPressed()
{
    m_DayWindow->setSearchText(m_searchEdit->text());
}

void Calendarmainwindow::slotStextChanged()
{
    m_stackWidget->setCurrentIndex(3);
    m_DayWindow->setSearchWFlag(!m_searchEdit->text().isEmpty());
}
