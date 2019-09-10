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
#include "customcalendarwidget.h"
#include <QLocale>
#include <QPainter>
#include <QTextCharFormat>
#include <QProxyStyle>
#include <QTableView>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include "cayearmonthedit.h"
class QCustomStyle : public QProxyStyle
{
public:
    QCustomStyle(QWidget *parent)
    {
        setParent(parent);
    };

private:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const
    {
        if (element == PE_FrameFocusRect) {
            return;
        }
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

CCustomCalendarWidget::CCustomCalendarWidget(QWidget *parent)
    : DCalendarWidget(parent)
{
    initControl();
    //setFixedSize(300, 300);
}

CCustomCalendarWidget::~CCustomCalendarWidget()
{

}

void CCustomCalendarWidget::initControl()
{
    layout()->setSizeConstraint(QLayout::SetMinimumSize);
    setLocale(QLocale(QLocale::Chinese));
    setNavigationBarVisible(false);
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
    setStyle(new QCustomStyle(this));

    QTextCharFormat format;
    format.setForeground(m_defaultTextColor);
    format.setBackground(QColor(255, 255, 255));
    QTextCharFormat sformat;
    sformat.setForeground(m_backgroundCircleColor);
    sformat.setBackground(QColor(255, 255, 255));
    setHeaderTextFormat(format);
    setWeekdayTextFormat(Qt::Saturday, sformat);
    setWeekdayTextFormat(Qt::Sunday, sformat);
    setWeekdayTextFormat(Qt::Monday, format);
    setWeekdayTextFormat(Qt::Tuesday, format);
    setWeekdayTextFormat(Qt::Wednesday, format);
    setWeekdayTextFormat(Qt::Thursday, format);
    setWeekdayTextFormat(Qt::Friday, format);

    initTopWidget();
    //initBottomWidget();

    connect(this, &QCalendarWidget::currentPageChanged, [this](int year, int month) {
        setDataLabelTimeText(year, month);
    });
    QTableView *view = findChild<QTableView *>("qt_calendar_calendarview");
    if (view) {
        view->setFixedSize(370, 300);
    }
}

void CCustomCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    if (date == selectedDate()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(m_backgroundCircleColor);

        painter->drawRoundedRect(rect.x(), rect.y() + 3, rect.width(), rect.height() - 6, 3, 3);
        painter->setPen(m_selectedTextColor);

        painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    } else if (date == QDate::currentDate()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(m_backgroundCircleColor);
        painter->drawRoundedRect(rect.x(), rect.y() + 3, rect.width(), rect.height() - 6, 3, 3);
        painter->setBrush(QColor(255, 255, 255));
        painter->drawRoundedRect(rect.x() + 1, rect.y() + 4, rect.width() - 2, rect.height() - 8, 2, 2);
        painter->setPen(m_currentDayTextColor);
        painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    } else  {
        //QCalendarWidget::paintCell(painter, rect, date);
        //return;
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(255, 255, 255));
        painter->drawRect(rect.x(), rect.y() + 3, rect.width(), rect.height() - 6);
        if (date.month() == m_monthLabel->getNum()) {
            painter->setPen(m_defaultTextColor);
            if (date.dayOfWeek() == 6 || date.dayOfWeek() == 7)
                painter->setPen(m_backgroundCircleColor);
        } else {
            painter->setPen(m_notCurrentTextColor);
        }
        painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    }
}

void CCustomCalendarWidget::resizeEvent(QResizeEvent *e)
{
    QCalendarWidget::resizeEvent(e);
}

void CCustomCalendarWidget::initTopWidget()
{
    QWidget *topWidget = new QWidget(this);
    topWidget->setObjectName("CalendarTopWidget");
    topWidget->setFixedHeight(40);
    topWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins(12, 0, 12, 0);
    hboxLayout->setSpacing(4);

    m_leftMonthBtn = new DImageButton(this);
    m_leftMonthBtn->setFixedWidth(36);
    m_leftMonthBtn->setHoverPic(":/resources/icon/previous_hover.svg");
    m_leftMonthBtn->setNormalPic(":/resources/icon/previous_normal.svg");
    m_leftMonthBtn->setPressPic(":/resources/icon/previous_press.svg");


    m_rightMonthBtn = new DImageButton(this);
    m_rightMonthBtn->setFixedWidth(36);
    m_rightMonthBtn->setHoverPic(":/resources/icon/next_hover.svg");
    m_rightMonthBtn->setNormalPic(":/resources/icon/next_normal.svg");
    m_rightMonthBtn->setPressPic(":/resources/icon/next_press.svg");

    //m_dataLabel = new QLabel(this);
    m_monthLabel = new CCaYearMonthEdit(this);
    m_yearLabel = new CCaYearMonthEdit(this);
    m_monthLabel->setRange(1, 12);
    m_yearLabel->setRange(1900, 3000);
    m_monthLabel->setMinimumWidth(80);
    m_monthLabel->setPadStr(tr("M"));
    m_yearLabel->setPadStr(tr("Y"));
    m_yearLabel->setMinimumWidth(100);

    m_leftMonthBtn->setObjectName("CalendarLeftMonthBtn");
    m_rightMonthBtn->setObjectName("CalendarRightMonthBtn");
    hboxLayout->addWidget(m_leftMonthBtn);
    hboxLayout->addStretch();
    hboxLayout->addWidget(m_monthLabel);
    hboxLayout->addWidget(m_yearLabel);
    hboxLayout->addStretch();
    hboxLayout->addWidget(m_rightMonthBtn);
    topWidget->setLayout(hboxLayout);

    //这里见下图1
    QVBoxLayout *vBodyLayout = qobject_cast<QVBoxLayout *>(layout());
    vBodyLayout->insertWidget(0, topWidget);

    //vBodyLayout->setSizeConstraint(QLayout::SetMinimumSize);
    //connect(m_leftMonthBtn, SIGNAL(clicked()), this, SLOT(onbtnClicked()));
    //connect(m_rightMonthBtn, SIGNAL(clicked()), this, SLOT(onbtnClicked()));
    connect(m_leftMonthBtn, &DImageButton::clicked, this, &CCustomCalendarWidget::slotABtPrivMonth);
    connect(m_rightMonthBtn, &DImageButton::clicked, this, &CCustomCalendarWidget::slotABtNextMonth);

    connect(m_monthLabel, &CCaYearMonthEdit::signalsCurrentValueChange, this, &CCustomCalendarWidget::slotmonthChange);
    connect(m_yearLabel, &CCaYearMonthEdit::signalsCurrentValueChange, this, &CCustomCalendarWidget::slotyearChange);

    setDataLabelTimeText(selectedDate().year(), selectedDate().month());
}

void CCustomCalendarWidget::initBottomWidget()
{

}

void CCustomCalendarWidget::setDataLabelTimeText(int year, int month)
{
    m_monthLabel->setCurrentNum(month);
    m_yearLabel->setCurrentNum(year);
    //m_dataLabel->setText(QStringLiteral("%1年%2月").arg(year).arg(month));
}

void CCustomCalendarWidget::onbtnClicked()
{
    //  QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());
    //if (senderBtn == m_leftMonthBtn) {
    //    showPreviousMonth();
    //} else if (senderBtn == m_rightMonthBtn) {
    //    showNextMonth();
    //}
}

void CCustomCalendarWidget::slotmonthChange(int value)
{
    QDate date = selectedDate();
    setSelectedDate(QDate(date.year(), value, date.day()));
}

void CCustomCalendarWidget::slotyearChange(int value)
{
    QDate date = selectedDate();
    setSelectedDate(QDate( value, date.month(), date.day()));
}

void CCustomCalendarWidget::slotABtPrivMonth()
{
    showPreviousMonth();
}

void CCustomCalendarWidget::slotABtNextMonth()
{
    showNextMonth();
}