/*
* Copyright (C) 2019 ~ 2022 Uniontech Software Technology Co.,Ltd.
*
* Author:     xuezifan  <xuezifan@uniontech.com>
*
* Maintainer: xuezifan  <xuezifan@uniontech.com>
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
#include "commonsetting.h"

#include <DCommandLinkButton>
#include <DSettingsWidgetFactory>
#include <DComboBox>
#include <QHBoxLayout>
#include <DSettingsOption>


Commonsetting::Commonsetting(QObject *parent)
    : QObject(parent)
{

}

Commonsetting::~Commonsetting()
{

}

QPair<QWidget*, QWidget*> Commonsetting::createComboboxFirstDayofWeek(QObject *obj)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(obj);

    // 构建自定义Item
    QWidget* widget = new QWidget();
    QComboBox* combobox = new QComboBox(widget);
    QHBoxLayout* layout = new QHBoxLayout(widget);
    QStringList firstDayofWeekList;
    firstDayofWeekList<<tr("Sunday")<<tr("Monday")<<tr("Tuesday")<<tr("Wednesday")<<tr("Thursday")<<tr("Friday")<<tr("Saturday");
    combobox->addItems(firstDayofWeekList);
    combobox->setFixedSize(150,36);
    layout->addWidget(combobox);
    widget->layout()->setAlignment(Qt::AlignRight);
    QPair<QWidget *, QWidget *> optionWidget = DSettingsWidgetFactory::createStandardItem(QByteArray(), option, widget);
    // 获取初始值
    option->setValue(option->defaultValue());
    if (widget != nullptr)
        widget->deleteLater();
    return optionWidget;
}

QPair<QWidget*, QWidget*> Commonsetting::createComboboxTime(QObject *obj)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(obj);

    // 构建自定义Item
    QWidget* widget = new QWidget();
    QComboBox* combobox = new QComboBox(widget);
    QHBoxLayout* layout = new QHBoxLayout(widget);
    QStringList firstDayofWeekList;
    firstDayofWeekList<<tr("24 hour")<<tr("12 hour");
    combobox->addItems(firstDayofWeekList);
    combobox->setFixedSize(150,36);
    layout->addWidget(combobox);
    widget->layout()->setAlignment(Qt::AlignRight);
    QPair<QWidget *, QWidget *> optionWidget = DSettingsWidgetFactory::createStandardItem(QByteArray(), option, widget);
    // 获取初始值
    option->setValue(option->defaultValue());
    if (widget != nullptr)
        widget->deleteLater();
    return optionWidget;
}